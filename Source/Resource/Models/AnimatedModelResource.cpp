#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Models/AnimatedModelResource.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"

#include <cstdlib>
#include <cstring>
#include <string>

// GLOBAL: gl_24742
GrannyAnimationLibrary g_grannyAnimationLibrary;

// INLINED
AnimatedModelFrame::AnimatedModelFrame() : vertices(nullptr), textureVertices(nullptr) {
}

// OFFSET: 0x00756680
// FUNCTION: fn_07994
AnimatedModelFrame::~AnimatedModelFrame() {
  if (this->vertices) {
    delete[] this->vertices;
  }

  if (this->textureVertices) {
    delete[] this->textureVertices;
  }
}

// INLINED
AnimatedModelNode::AnimatedModelNode()
    : model(nullptr), parent(nullptr), vertexCount(0), faceCount(0), faceNormals(nullptr), frames(nullptr), faces(nullptr), textures(nullptr) {
}

// INLINED
AnimatedModelNode::~AnimatedModelNode() {
  delete[] this->frames;
  delete[] this->faces;
  delete[] this->faceNormals;

  if (this->textures) {
    for (int index = 0; index < this->textureCount; ++index) {
      this->textures[index]->Release();
      g_texMgr.Remove(this->textures[index]);
    }

    delete[] this->textures;
  }
}

// OFFSET: 0x007550E0
// FUNCTION: fn_07979
C3dAnimModelRes::C3dAnimModelRes(ModelResourceContext& context)
    : unknown_0110(0), unknown_0114(nullptr), shade(2), frameCount(0), animationLength(0), speed(1.0f), unknown_0150(0), unknown_0154(0.0f), unknown_0158(0.0f),
      unknown_015C(0.0f), alpha(255), m_context(&context) {
}

// OFFSET: 0x00756480
// FUNCTION: fn_07993
C3dAnimModelRes::~C3dAnimModelRes() {
  this->Unload();
}

// OFFSET: 0x007521F0
// FUNCTION: fn_07956
C3dAnimModelRes* C3dAnimModelRes::Create() {
  return new C3dAnimModelRes(*this->m_context);
}

// OFFSET: 0x00752380
// OFFSET: 0x0075C710
// FUNCTION: fn_08023
void C3dAnimModelRes::Unload() {
  if (this->unknown_0114) {
    delete[] this->unknown_0114;
    this->unknown_0114 = nullptr;
    this->unknown_0110 = 0;
  }

  for (AnimatedModelNode* node : this->nodes) {
    delete node;
  }

  this->nodes.clear();
}

// OFFSET: 0x00752390
// FUNCTION: fn_07958
bool C3dAnimModelRes::Load(const char* name) {
  auto& file = this->m_context->file;

  if (!file.Open(name)) {
    file.Close();
    return false;
  }

  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  char magic[5];
  unsigned char major;
  unsigned char minor;
  input.Read(magic, 4);
  magic[4] = 0;
  input.Read(&major, 1);
  input.Read(&minor, 1);

  if (major == 1 && minor >= 2) {
    input.Read(&this->speed, 4);
    input.Read(&this->unknown_0154, 12);
  }

  if (major == 1 && minor == 3) {
    input.Read(&this->alpha, 4);
  }

  input.Read(&this->shade, 4);
  input.Read(&this->frameCount, 4);
  input.Read(&this->animationLength, 4);
  unsigned char reserved[16];
  input.Read(reserved, sizeof(reserved));
  unsigned textureCount;
  input.Read(&textureCount, 4);
  std::vector<std::string> names;
  char field[40];

  for (unsigned index = 0; index < textureCount; ++index) {
    input.Read(field, sizeof(field));
    names.emplace_back(field);
  }

  input.Read(field, sizeof(field));
  const std::string rootName(field);
  int nodeCount;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    auto node = std::make_unique<AnimatedModelNode>();
    node->model = this;
    node->frames = new AnimatedModelFrame[this->frameCount];
    std::uint32_t marker;
    input.Read(&marker, 4);

    if (marker != 0x12345678u) {
      ShowErrorDialog(this->m_context->renderSystem, "data corrupt", 0);
      file.Close();
      return false;
    }

    input.Read(field, sizeof(field));
    node->name = field;
    input.Read(field, sizeof(field));
    node->parentName = field;
    input.Read(&node->textureCount, 4);
    node->textures = new CTexture*[node->textureCount];

    for (int texture = 0; texture < node->textureCount; ++texture) {
      int id;
      input.Read(&id, 4);
      CTexture* value = g_texMgr.Load(names[id].c_str(), 0, false, true, false);
      value->Retain();
      node->textures[texture] = value;
    }

    input.Read(&node->vertexCount, 4);

    for (int frameIndex = 0; frameIndex < this->frameCount; ++frameIndex) {
      AnimatedModelFrame& frame = node->frames[frameIndex];
      frame.vertices = new std::array<float, 3>[node->vertexCount]{};
      input.Read(frame.vertices, static_cast<unsigned>(node->vertexCount) * 12u);
    }

    input.Read(&node->textureVertexCount, 4);
    input.Read(&node->sharedTextureVertices, 4);

    if (node->sharedTextureVertices) {
      node->frames[0].textureVertices = new ModelTextureVertex[node->textureVertexCount];
      input.Read(node->frames[0].textureVertices, static_cast<unsigned>(node->textureVertexCount) * 12u);
    } else {
      for (int frameIndex = 0; frameIndex < this->frameCount; ++frameIndex) {
        AnimatedModelFrame& frame = node->frames[frameIndex];
        frame.textureVertices = new ModelTextureVertex[node->textureVertexCount];
        input.Read(frame.textureVertices, static_cast<unsigned>(node->textureVertexCount) * 12u);
      }
    }

    input.Read(&node->faceCount, 4);
    node->faceNormals = new std::array<float, 3>[node->faceCount];
    node->faces = new AnimatedModelFace[node->faceCount];
    input.Read(node->faces, static_cast<unsigned>(node->faceCount) * 24u);
    this->nodes.push_back(node.release());
    input.Read(&marker, 4);

    if (marker != 0x12345678u) {
      ShowErrorDialog(this->m_context->renderSystem, "data corrupt", 0);
      file.Close();
      return false;
    }
  }

  file.Close();

  for (AnimatedModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      AnimatedModelNode* parent = nullptr;

      for (AnimatedModelNode* candidate : this->nodes) {
        if (candidate->name == node->parentName) {
          parent = candidate;
          break;
        }
      }

      node->parent = parent;
      parent->children.push_back(node);
    }
  }

  this->rootNames.push_back(rootName);
  file.Close();
  return true;
}

// INLINED
GrannyAnimationLibrary::~GrannyAnimationLibrary() {
  if (this->instance) {
    this->FreeInstance();

    for (auto& job : this->animations) {
      for (granny_animation*& animation : job) {
        if (animation) {
          animation = nullptr;
        }
      }
    }

    GrannyFreeLocalPose(this->localPose);
    GrannyFreeFile(this->file);
  }
}

// OFFSET: 0x00755340
// FUNCTION: fn_07981
C3dGrannyModelRes::C3dGrannyModelRes() {
  this->position[0] = 0.0f;
  this->position[1] = 0.0f;
  this->position[2] = 0.0f;
  this->instance = nullptr;
  this->model = nullptr;
  this->job = 0;
  this->unknown_03B0 = 0;
}

// INLINED
C3dGrannyModelRes::C3dGrannyModelRes(ModelResourceContext& context) : m_context(&context) {
  this->position[0] = 0.0f;
  this->position[1] = 0.0f;
  this->position[2] = 0.0f;
  this->instance = nullptr;
  this->model = nullptr;
  this->job = 0;
  this->unknown_03B0 = 0;
}

// OFFSET: 0x00753540
// FUNCTION: fn_07961
C3dGrannyModelRes* C3dGrannyModelRes::Create() {
  return new C3dGrannyModelRes(*this->m_context);
}

// OFFSET: 0x00753620
// FUNCTION: fn_07963
bool C3dGrannyModelRes::Load(const char* name) {
  bool result = false;

  if (!GrannyVersionsMatch_(2, 1, 0, 5)) {
    ShowErrorDialog(GetRenderSystemHolder()->system, "doesn't match the .h file used during compilation", 0);
    return false;
  }

  this->animations = &g_grannyAnimationLibrary;
  this->modelCount = 0;
  this->maxBoneCount = 0;
  this->textureCount = 0;
  this->unknown_0280 = 0;
  const char* cursor = std::strchr(name, '_');

  if (!cursor) {
    return false;
  }

  ++cursor;
  char number[10];
  int length = 0;

  for (; *cursor != '.'; ++cursor) {
    if (length >= 4) {
      break;
    }

    number[length++] = *cursor;
  }

  number[length] = 0;
  this->job = std::atoi(number);
  CFile file;

  if (!file.Open(name, 0, true, false)) {
    const std::string path(name);
    const std::string message = std::string("Cannot find File : ") + path;
    ShowErrorDialog(GetRenderSystemHolder()->system, message.c_str(), 0);
    return false;
  }

  this->file = GrannyReadEntireFileFromMemory(file.GetSize(), file.GetData());
  file.Close();

  if (!this->file) {
    ShowErrorDialog(GetRenderSystemHolder()->system, "Unable to load file .gr2 (C3dGrannyModelRes :: Load)", 0);
  } else {
    granny_file_info* info = GrannyGetFileInfo(this->file);
    std::strcpy(this->fileName, name);

    if (!info) {
      ShowErrorDialog(GetRenderSystemHolder()->system, "Unable to access file data .gr2", 0);
    } else {
      this->textureCount = info->textureCount;
      this->textures = new GrannyTextureEntry[this->textureCount];

      for (int index = 0; index < this->textureCount; ++index) {
        this->textures[index].texture = nullptr;
        const granny_texture* texture = info->textures[index];
        GrannyTextureEntry& entry = this->textures[index];
        entry.name = texture->fromFileName;

        if (texture->textureType == 0 && texture->imageCount == 1) {
          entry.texture = this->CreateTexture(texture, 0);
        } else {
          ShowErrorDialog(GetRenderSystemHolder()->system, "This sample only works with single-image : ColorMapTextureType textures", 0);
        }
      }

      this->modelCount = info->modelCount;
      this->instance = new GrannyModelInstance;
      const granny_model* model = info->models[0];
      this->InstantiateModel(this->instance, model);
      this->skeleton = info->skeletons[0];
      this->model = model;
      this->localPose = GrannyNewLocalPose(this->maxBoneCount);

      if (this->deformBufferSize) {
        this->unknown_0280 = 4;
      }

      result = true;
    }

    GrannyFreeFileSection(this->file, 1);
    GrannyFreeFileSection(this->file, 2);
    GrannyFreeFileSection(this->file, 4);
    GrannyFreeFileSection(this->file, 5);
  }

  this->unknown_0110 = 1920.0f;

  for (int index = 0; index < this->instance->meshCount; ++index) {
    if (GrannyGetMeshIndexCount(this->instance->meshes[index].mesh) / 3 > 10000) {
      ShowErrorDialog(GetRenderSystemHolder()->system, "too many vertex granny model!", 0);
    }
  }

  this->position[0] = 0.0f;
  this->position[1] = 0.0f;
  this->position[2] = 0.0f;
  this->unknown_03A0 = 10.0f;
  this->unknown_03A4 = 10.0f;
  file.Close();
  return result;
}
