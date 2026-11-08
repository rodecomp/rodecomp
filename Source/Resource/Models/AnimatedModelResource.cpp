#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Models/AnimatedModelResource.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/ResourceMgr.h"

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
  if (this->frames) {
    delete[] this->frames;
    this->frames = nullptr;
  }

  if (this->faces) {
    delete[] this->faces;
    this->faces = nullptr;
  }

  if (this->faceNormals) {
    delete[] this->faceNormals;
    this->faceNormals = nullptr;
  }

  if (this->textures) {
    for (int index = 0; index < this->textureCount; ++index) {
      this->textures[index]->Release();
      g_texMgr.Remove(this->textures[index]);
    }

    delete[] this->textures;
    this->textures = nullptr;
  }
}

// OFFSET: 0x007550E0
// FUNCTION: fn_07979
C3dAnimModelRes::C3dAnimModelRes()
    : unknown_0110(0), unknown_0114(nullptr), shade(2), frameCount(0), animationLength(0), speed(1.0f), unknown_0150(0), unknown_0154(0.0f), unknown_0158(0.0f),
      unknown_015C(0.0f), alpha(255) {
}

// OFFSET: 0x00756480
// FUNCTION: fn_07993
C3dAnimModelRes::~C3dAnimModelRes() {
  this->Unload();
}

// OFFSET: 0x007521F0
// FUNCTION: fn_07956
C3dAnimModelRes* C3dAnimModelRes::Create() {
  return new C3dAnimModelRes;
}

// OFFSET: 0x0075C710
// FUNCTION: fn_08023
void C3dAnimModelRes::Unload() {
  if (this->unknown_0114) {
    delete[] this->unknown_0114;
    this->unknown_0114 = nullptr;
    this->unknown_0110 = 0;
  }

  for (std::list<AnimatedModelNode*>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
    delete *it;
  }

  this->nodes.erase(this->nodes.begin(), this->nodes.end());
}

// OFFSET: 0x00752390
// FUNCTION: fn_07958
bool C3dAnimModelRes::Load(const char* name) {
  std::string rootName;
  CFile file;

  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  char magic[5];
  unsigned char major;
  unsigned char minor;
  file.Read(magic, 4);
  magic[4] = 0;
  file.Read(&major, 1);
  file.Read(&minor, 1);

  if (major == 1 && minor >= 2) {
    file.Read(&this->speed, 4);
    file.Read(&this->unknown_0154, 12);
  }

  if (major == 1 && minor == 3) {
    file.Read(&this->alpha, 4);
  }

  file.Read(&this->shade, 4);
  file.Read(&this->frameCount, 4);
  file.Read(&this->animationLength, 4);
  unsigned char reserved[16];
  file.Read(reserved, sizeof(reserved));
  int textureCount;
  file.Read(&textureCount, 4);
  std::string* names = new std::string[textureCount];
  char field[40];

  for (int index = 0; index < textureCount; ++index) {
    file.Read(field, sizeof(field));
    names[index] = std::string(field);
  }

  file.Read(field, sizeof(field));
  rootName = std::string(field);
  int nodeCount;
  file.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    AnimatedModelNode* node = new AnimatedModelNode;
    node->model = this;
    node->frames = new AnimatedModelFrame[this->frameCount];
    std::uint32_t header;
    file.Read(&header, 4);

    if (header != 0x12345678u) {
      ShowErrorDialog(GetRenderSystemHolder()->system, "data corrupt", 0);
      return false;
    }

    file.Read(field, sizeof(field));
    node->name = std::string(field);
    file.Read(field, sizeof(field));
    node->parentName = std::string(field);
    file.Read(&node->textureCount, 4);
    node->textures = new CTexture*[node->textureCount];

    for (int texture = 0; texture < node->textureCount; ++texture) {
      int id;
      file.Read(&id, 4);
      const std::string textureName = names[id];
      node->textures[texture] = g_texMgr.Load(textureName.c_str(), 0, false, true, false);
      node->textures[texture]->Retain();
    }

    file.Read(&node->vertexCount, 4);

    for (int frameIndex = 0; frameIndex < this->frameCount; ++frameIndex) {
      node->frames[frameIndex].vertices = new std::array<float, 3>[node->vertexCount]{};
      file.Read(node->frames[frameIndex].vertices, node->vertexCount * 12);
    }

    file.Read(&node->textureVertexCount, 4);
    file.Read(&node->sharedTextureVertices, 4);

    if (node->sharedTextureVertices) {
      node->frames[0].textureVertices = new ModelTextureVertex[node->textureVertexCount];
      file.Read(node->frames[0].textureVertices, node->textureVertexCount * 12);
    } else {
      for (int frameIndex = 0; frameIndex < this->frameCount; ++frameIndex) {
        node->frames[frameIndex].textureVertices = new ModelTextureVertex[node->textureVertexCount];
        file.Read(node->frames[frameIndex].textureVertices, node->textureVertexCount * 12);
      }
    }

    file.Read(&node->faceCount, 4);
    node->faceNormals = new std::array<float, 3>[node->faceCount];
    node->faces = new AnimatedModelFace[node->faceCount];
    file.Read(node->faces, node->faceCount * 24);
    this->nodes.push_back(node);
    std::uint32_t footer;
    file.Read(&footer, 4);

    if (footer != 0x12345678u) {
      ShowErrorDialog(GetRenderSystemHolder()->system, "data corrupt", 0);
      return false;
    }
  }

  file.Close();
  delete[] names;

  for (AnimatedModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      AnimatedModelNode* parent = nullptr;

      for (AnimatedModelNode* candidate : this->nodes) {
        if (candidate->name == node->parentName.c_str()) {
          parent = candidate;
          break;
        }
      }

      node->parent = parent;
      parent->children.push_back(node);
    }
  }

  this->rootNames.push_back(rootName.c_str());
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

// OFFSET: 0x00753540
// FUNCTION: fn_07961
C3dGrannyModelRes* C3dGrannyModelRes::Create() {
  return new C3dGrannyModelRes;
}

// OFFSET: 0x00753610
// FUNCTION: fn_07962
void C3dGrannyModelRes::Unload() {
  this->Release();
}

// OFFSET: 0x00753620
// FUNCTION: fn_07963
bool C3dGrannyModelRes::Load(const char* name) {
  bool result = false;

  if (!GrannyVersionsMatch_(2, 1, 0, 5)) {
    ShowErrorDialog("doesn't match the .h file used during compilation", 0);
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
    ShowErrorDialog(message.c_str(), 0);
    return false;
  }

  this->file = GrannyReadEntireFileFromMemory(file.GetSize(), file.GetData());
  file.Close();

  if (this->file) {
    granny_file_info* info = GrannyGetFileInfo(this->file);
    std::strcpy(this->fileName, name);

    if (info) {
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
          ShowErrorDialog("This sample only works with single-image : ColorMapTextureType textures", 0);
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
    } else {
      ShowErrorDialog("Unable to access file data .gr2", 0);
    }

    GrannyFreeFileSection(this->file, 1);
    GrannyFreeFileSection(this->file, 2);
    GrannyFreeFileSection(this->file, 4);
    GrannyFreeFileSection(this->file, 5);
  } else {
    ShowErrorDialog("Unable to load file .gr2 (C3dGrannyModelRes :: Load)", 0);
  }

  this->unknown_0110 = 1920.0f;

  for (int index = 0; index < this->instance->meshCount; ++index) {
    if (GrannyGetMeshIndexCount(this->instance->meshes[index].mesh) / 3 > 10000) {
      ShowErrorDialog("too many vertex granny model!", 0);
    }
  }

  this->position[0] = 0.0f;
  this->position[1] = 0.0f;
  this->position[2] = 0.0f;
  this->unknown_03A0 = 10.0f;
  this->unknown_03A4 = 10.0f;
  return result;
}

// INLINED
C3dAnimModelRes::C3dAnimModelRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25558
C3dAnimModelRes g_rsxPrototype("rsx", "model\\");

// INLINED
C3dGrannyModelRes::C3dGrannyModelRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25559
C3dGrannyModelRes g_gr2Prototype("gr2", "model\\3dmob\\");
