#include "Client/Application/ErrorDialog.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <algorithm>
#include <cstring>

namespace {
  // INLINED
  void ReadKeys(ResourceByteReader& input, std::vector<ModelRotationKey>& keys, bool scale) {
    int count = 0;
    input.Read(&count, 4);

    for (int index = 0; index < count; ++index) {
      ModelRotationKey key;

      if (scale) {
        key = {0, {0, 0, 0, 1}};
      }

      input.Read(&key, sizeof(key));
      keys.push_back(key);
    }
  }
}

// OFFSET: 0x007555C0
// FUNCTION: fn_07984
C3dModelResV2::C3dModelResV2(ModelResourceContext& context) : C3dModelRes(context), framesPerSecond(0.0f), animationLength(0) {
}

// INLINED
C3dModelResV2::~C3dModelResV2() = default;

// OFFSET: 0x00751ED0
// FUNCTION: fn_07953
C3dModelResV2* C3dModelResV2::Create() {
  return new C3dModelResV2(*this->m_context);
}

// INLINED
inline void C3dModelResV2::ReadVersion(ResourceByteReader& input, unsigned minor) {
  if (minor == 1) {
    this->ReadVersion1(input);
  } else if (minor == 2) {
    this->ReadVersion2(input);
  } else {
    this->ReadVersion3(input);
  }
}

// OFFSET: 0x00751F70
// FUNCTION: fn_07954
bool C3dModelResV2::Load(const char* name) {
  auto& file = this->m_context->file;

  if (!file.Open(name)) {
    file.Close();
    return false;
  }

  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  char magic[5]{};
  input.Read(magic, 4);

  if (std::strcmp(magic, "GRSM") != 0) {
    constexpr char message[] = "모델파일이 아닙니다. 형식을 다시한번 확인해주세요.";
    ShowErrorDialog(this->m_context->renderSystem, message, 0);
    file.Close();

    return false;
  }

  unsigned char major = 0;
  unsigned char minor = 0;
  input.Read(&major, 1);
  this->major = major;
  input.Read(&minor, 1);

  if (minor >= 1 && minor <= 3) {
    this->ReadVersion(input, minor);
  }

  file.Close();
  this->sourceName = name;
  file.Close();

  return true;
}

// OFFSET: 0x00759ED0
// FUNCTION: fn_08020
void C3dModelResV2::ReadVersion1(ResourceByteReader& input) {
  input.Read(&this->animationLength, 4);
  input.Read(&this->shade, 4);
  input.Read(&this->alpha, 1);
  input.Read(&this->framesPerSecond, 4);
  int textureCount = 0;
  input.Read(&textureCount, 4);
  std::vector<std::string> textureNames;

  for (int index = 0; index < textureCount; ++index) {
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    textureNames.emplace_back(name);
  }

  int rootCount = 0;
  input.Read(&rootCount, 4);

  for (int index = 0; index < rootCount; ++index) {
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    this->rootNames.emplace_back(name);
  }

  int nodeCount = 0;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    auto* node = new ModelNodeV2;
    node->model = this;
    node->mesh.reset(new ModelMesh);
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    node->name = name;
    bytes = 0;
    input.Read(&bytes, 4);

    if (static_cast<int>(bytes) > 0) {
      char parentName[40]{};
      input.Read(parentName, bytes);
      node->parentName = parentName;
    }

    int nodeTextureCount = 0;
    input.Read(&nodeTextureCount, 4);

    for (int texture = 0; texture < nodeTextureCount; ++texture) {
      int textureIndex = 0;
      input.Read(&textureIndex, 4);
      CTexture* loaded = g_texMgr.Load(textureNames[textureIndex].c_str(), 0, false, true, false);
      node->textures.push_back(loaded);

      if (loaded) {
        loaded->Retain();
      }
    }

    input.Read(node->transform, sizeof(node->transform));
    ModelMesh& mesh = *node->mesh;
    input.Read(&mesh.vertexCount, 4);
    mesh.vertices = new std::array<float, 3>[mesh.vertexCount]{};
    input.Read(mesh.vertices, static_cast<unsigned>(mesh.vertexCount) * 12u);
    input.Read(&mesh.textureVertexCount, 4);
    mesh.textureVertices = new ModelTextureVertex[mesh.textureVertexCount];
    input.Read(mesh.textureVertices, static_cast<unsigned>(mesh.textureVertexCount) * 12u);
    input.Read(&mesh.faceCount, 4);
    mesh.faces = new ModelFace[mesh.faceCount];

    for (int face = 0; face < mesh.faceCount; ++face) {
      ModelFace& target = mesh.faces[face];
      struct {
        /* 0000 */ std::uint16_t vertices[3];
        /* 0006 */ std::uint16_t textureVertices[3];
        /* 000C */ std::uint16_t texture;
        /* 000E */ std::uint16_t unknown_000E;
        /* 0010 */ std::uint32_t twoSided;
        /* 0014 */ std::uint32_t smoothing;
      } record;
      input.Read(&record, sizeof(record));
      std::memcpy(target.vertices, record.vertices, sizeof(target.vertices));
      std::memcpy(target.textureVertices, record.textureVertices, sizeof(target.textureVertices));
      target.texture = record.texture;
      target.unknown_000E = record.unknown_000E;
      target.twoSided = record.twoSided;

      if (target.smoothing.empty()) {
        target.smoothing.push_back(record.smoothing);
      }
    }

    std::sort(mesh.faces, mesh.faces + mesh.faceCount, [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
    mesh.CalculateNormals();
    ReadKeys(input, node->scaleKeys, false);
    ReadKeys(input, node->rotationKeys, true);
    ReadKeys(input, node->positionKeys, false);
    this->nodes.push_back(node);
    this->meshes[node->name] = node->mesh.get();
  }

  for (ModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      ModelNode* parent = this->FindNode(node->parentName.c_str());

      if (parent) {
        node->parent = parent;
        parent->children.push_back(node);
      }
    }
  }

  int volumeCount = 0;
  input.Read(&volumeCount, 4);

  for (int index = 0; index < volumeCount; ++index) {
    auto* volume = new ModelVolume;
    std::memset(volume, 0, 132);
    input.Read(volume, sizeof(ModelVolume));
    this->volumes.push_back(volume);
  }
}

// OFFSET: 0x0075AB20
// FUNCTION: fn_08021
void C3dModelResV2::ReadVersion2(ResourceByteReader& input) {
  input.Read(&this->animationLength, 4);
  input.Read(&this->shade, 4);
  input.Read(&this->alpha, 1);
  input.Read(&this->framesPerSecond, 4);
  int textureCount = 0;
  input.Read(&textureCount, 4);
  std::vector<std::string> textureNames;

  for (int index = 0; index < textureCount; ++index) {
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    textureNames.emplace_back(name);
  }

  int rootCount = 0;
  input.Read(&rootCount, 4);

  for (int index = 0; index < rootCount; ++index) {
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    this->rootNames.emplace_back(name);
  }

  int nodeCount = 0;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    auto* node = new ModelNodeV2;
    node->model = this;
    node->mesh.reset(new ModelMesh);
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    node->name = name;
    bytes = 0;
    input.Read(&bytes, 4);

    if (static_cast<int>(bytes) > 0) {
      char parentName[40]{};
      input.Read(parentName, bytes);
      node->parentName = parentName;
    }

    int nodeTextureCount = 0;
    input.Read(&nodeTextureCount, 4);

    for (int texture = 0; texture < nodeTextureCount; ++texture) {
      int textureIndex = 0;
      input.Read(&textureIndex, 4);
      CTexture* loaded = g_texMgr.Load(textureNames[textureIndex].c_str(), 0, false, true, false);
      node->textures.push_back(loaded);

      if (loaded) {
        loaded->Retain();
      }
    }

    input.Read(node->transform, sizeof(node->transform));
    ModelMesh& mesh = *node->mesh;
    input.Read(&mesh.vertexCount, 4);
    mesh.vertices = new std::array<float, 3>[mesh.vertexCount]{};
    input.Read(mesh.vertices, static_cast<unsigned>(mesh.vertexCount) * 12u);
    input.Read(&mesh.textureVertexCount, 4);
    mesh.textureVertices = new ModelTextureVertex[mesh.textureVertexCount];
    input.Read(mesh.textureVertices, static_cast<unsigned>(mesh.textureVertexCount) * 12u);
    input.Read(&mesh.faceCount, 4);
    mesh.faces = new ModelFace[mesh.faceCount];

    for (int face = 0; face < mesh.faceCount; ++face) {
      ModelFace& target = mesh.faces[face];
      unsigned size = 0;
      input.Read(&size, 4);
      std::vector<unsigned char> record(size);
      input.Read(record.data(), size);
      std::memcpy(&target, record.data(), 20);
      const unsigned groups = (size - 20u) / 4u;
      target.smoothing.resize(groups);
      std::memcpy(target.smoothing.data(), record.data() + 20, groups * 4u);
    }

    std::sort(mesh.faces, mesh.faces + mesh.faceCount, [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
    mesh.CalculateNormals();
    ReadKeys(input, node->scaleKeys, false);
    ReadKeys(input, node->rotationKeys, true);
    ReadKeys(input, node->positionKeys, false);
    this->nodes.push_back(node);
    this->meshes[node->name] = node->mesh.get();
  }

  for (ModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      ModelNode* parent = this->FindNode(node->parentName.c_str());

      if (parent) {
        node->parent = parent;
        parent->children.push_back(node);
      }
    }
  }

  int volumeCount = 0;
  input.Read(&volumeCount, 4);

  for (int index = 0; index < volumeCount; ++index) {
    auto* volume = new ModelVolume;
    std::memset(volume, 0, 132);
    input.Read(volume, sizeof(ModelVolume));
    this->volumes.push_back(volume);
  }
}

// OFFSET: 0x0075B8E0
// FUNCTION: fn_08022
void C3dModelResV2::ReadVersion3(ResourceByteReader& input) {
  input.Read(&this->animationLength, 4);
  input.Read(&this->shade, 4);
  input.Read(&this->alpha, 1);
  input.Read(&this->framesPerSecond, 4);
  int rootCount = 0;
  input.Read(&rootCount, 4);

  for (int index = 0; index < rootCount; ++index) {
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    this->rootNames.emplace_back(name);
  }

  int nodeCount = 0;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    auto* node = new ModelNodeV2;
    node->model = this;
    node->mesh.reset(new ModelMesh);
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);
    node->name = name;
    bytes = 0;
    input.Read(&bytes, 4);

    if (static_cast<int>(bytes) > 0) {
      char parentName[40]{};
      input.Read(parentName, bytes);
      node->parentName = parentName;
    }

    int nodeTextureCount = 0;
    input.Read(&nodeTextureCount, 4);

    for (int texture = 0; texture < nodeTextureCount; ++texture) {
      unsigned textureBytes = 0;
      input.Read(&textureBytes, 4);
      char textureName[40]{};
      input.Read(textureName, textureBytes);
      CTexture* loaded = g_texMgr.Load(textureName, 0, false, true, false);
      node->textures.push_back(loaded);

      if (loaded) {
        loaded->Retain();
      }
    }

    input.Read(node->transform, sizeof(node->transform));
    ModelMesh& mesh = *node->mesh;
    input.Read(&mesh.vertexCount, 4);
    mesh.vertices = new std::array<float, 3>[mesh.vertexCount]{};
    input.Read(mesh.vertices, static_cast<unsigned>(mesh.vertexCount) * 12u);
    input.Read(&mesh.textureVertexCount, 4);
    mesh.textureVertices = new ModelTextureVertex[mesh.textureVertexCount];
    input.Read(mesh.textureVertices, static_cast<unsigned>(mesh.textureVertexCount) * 12u);
    input.Read(&mesh.faceCount, 4);
    mesh.faces = new ModelFace[mesh.faceCount];

    for (int face = 0; face < mesh.faceCount; ++face) {
      ModelFace& target = mesh.faces[face];
      unsigned size = 0;
      input.Read(&size, 4);
      std::vector<unsigned char> record(size);
      input.Read(record.data(), size);
      std::memcpy(&target, record.data(), 20);
      const unsigned groups = (size - 20u) / 4u;
      target.smoothing.resize(groups);
      std::memcpy(target.smoothing.data(), record.data() + 20, groups * 4u);
    }

    std::sort(mesh.faces, mesh.faces + mesh.faceCount, [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
    mesh.CalculateNormals();
    ReadKeys(input, node->scaleKeys, false);
    ReadKeys(input, node->rotationKeys, true);
    ReadKeys(input, node->positionKeys, false);
    int textureAnimations = 0;
    input.Read(&textureAnimations, 4);

    for (int animation = 0; animation < textureAnimations; ++animation) {
      int texture = 0;
      input.Read(&texture, 4);
      ModelTextureChannels channels;
      int channelCount = 0;
      input.Read(&channelCount, 4);

      for (int channel = 0; channel < channelCount; ++channel) {
        int id = 0;
        int keyCount = 0;
        input.Read(&id, 4);
        input.Read(&keyCount, 4);

        for (int key = 0; key < keyCount; ++key) {
          ModelTextureKey value;
          input.Read(&value, sizeof(value));
          channels[id].push_back(value);
        }
      }

      node->textureKeys[texture] = std::move(channels);
    }

    this->nodes.push_back(node);
    this->meshes[node->name] = node->mesh.get();
  }

  for (ModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      ModelNode* parent = this->FindNode(node->parentName.c_str());

      if (parent) {
        node->parent = parent;
        parent->children.push_back(node);
      }
    }
  }

  int volumeCount = 0;
  input.Read(&volumeCount, 4);

  for (int index = 0; index < volumeCount; ++index) {
    auto* volume = new ModelVolume;
    std::memset(volume, 0, 132);
    input.Read(volume, sizeof(ModelVolume));
    this->volumes.push_back(volume);
  }
}
