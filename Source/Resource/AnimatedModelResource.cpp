#include "Client/ErrorDialog.h"
#include "Render/Texture.h"
#include "Render/TextureCache.h"
#include "Resource/AnimatedModelResource.h"
#include "Resource/BitmapResource.h"
#include "Resource/ResourceByteReader.h"

#include <cstring>

AnimatedModelNode::AnimatedModelNode(TextureCache& textures)
    : model(nullptr), parent(nullptr), unknown_0040(0), vertexCount(0), textureVertexCount(0), faceCount(0),
      sharedTextureVertices(0), m_textures(textures) {
}

AnimatedModelNode::~AnimatedModelNode() {
  for (CTexture* texture : this->textures) {
    texture->Release();
    this->m_textures.Remove(texture);
  }
}

// OFFSET: 0x007521F0
// FUNCTION: fn_07956
C3dAnimModelRes::C3dAnimModelRes(ModelResourceContext& context)
    : shade(2), frameCount(0), animationLength(0), speed(1.0f), unknown_0150(0), unknown_0154(0.0f), unknown_0158(0.0f),
      unknown_015C(0.0f), alpha(255), m_context(&context) {
}

// OFFSET: 0x00753510
// FUNCTION: fn_07960
C3dAnimModelRes::~C3dAnimModelRes() {
  this->Unload();
}

// OFFSET: 0x007521F0
// FUNCTION: fn_07956
C3dAnimModelRes* C3dAnimModelRes::Create() {
  return new C3dAnimModelRes(*this->m_context);
}

// OFFSET: 0x00752380
// FUNCTION: fn_07957
// OFFSET: 0x0075C710
// FUNCTION: fn_08023
void C3dAnimModelRes::Unload() {
  for (AnimatedModelNode* node : this->nodes) {
    delete node;
  }

  this->nodes.clear();
  // The original retains root-name strings until object destruction.
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
    auto node = std::make_unique<AnimatedModelNode>(this->m_context->textures);
    node->model = this;
    node->frames.resize(static_cast<unsigned>(this->frameCount));
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
    int count;
    input.Read(&count, 4);

    for (int texture = 0; texture < count; ++texture) {
      int id;
      input.Read(&id, 4);
      CTexture* value = this->m_context->textures.Load(names[id].c_str(), 0, false, true, false);
      value->Retain();
      node->textures.push_back(value);
    }

    input.Read(&node->vertexCount, 4);

    for (AnimatedModelFrame& frame : node->frames) {
      frame.vertices.resize(static_cast<unsigned>(node->vertexCount));
      input.Read(frame.vertices.data(), static_cast<unsigned>(node->vertexCount) * 12u);
    }

    input.Read(&node->textureVertexCount, 4);
    input.Read(&node->sharedTextureVertices, 4);

    if (node->sharedTextureVertices) {
      node->frames[0].textureVertices.resize(static_cast<unsigned>(node->textureVertexCount));
      input.Read(node->frames[0].textureVertices.data(), static_cast<unsigned>(node->textureVertexCount) * 12u);
    } else {
      for (AnimatedModelFrame& frame : node->frames) {
        frame.textureVertices.resize(static_cast<unsigned>(node->textureVertexCount));
        input.Read(frame.textureVertices.data(), static_cast<unsigned>(node->textureVertexCount) * 12u);
      }
    }

    input.Read(&node->faceCount, 4);
    node->faceNormals.resize(static_cast<unsigned>(node->faceCount));
    node->faces.resize(static_cast<unsigned>(node->faceCount));
    input.Read(node->faces.data(), static_cast<unsigned>(node->faceCount) * 24u);
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
