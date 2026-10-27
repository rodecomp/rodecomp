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
  std::string ReadName(ResourceByteReader& input) {
    unsigned bytes = 0;
    input.Read(&bytes, 4);
    char name[40]{};
    input.Read(name, bytes);

    return name;
  }

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
    constexpr char message[] = "\xB8\xF0\xB5\xA8\xC6\xC4\xC0\xCF\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9. \xC7\xFC\xBD\xC4\xC0\xBB "
                               "\xB4\xD9\xBD\xC3\xC7\xD1\xB9\xF8 \xC8\xAE\xC0\xCE\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.";
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

// INLINED
inline void C3dModelResV2::ReadNode(ResourceByteReader& input, unsigned minor, const std::vector<std::string>& textureNames) {
  auto node = std::make_unique<ModelNodeV2>();
  node->model = this;
  node->mesh = std::make_unique<ModelMesh>();
  node->name = ReadName(input);
  node->parentName = ReadName(input);
  int textureCount = 0;
  input.Read(&textureCount, 4);

  for (int index = 0; index < textureCount; ++index) {
    std::string name;

    if (minor < 3) {
      int texture = 0;
      input.Read(&texture, 4);
      name = textureNames[texture];
    } else {
      name = ReadName(input);
    }

    CTexture* texture = this->m_context->textures.Load(name.c_str(), 0, false, true, false);
    node->textures.push_back(texture);

    if (texture) {
      texture->Retain();
    }
  }

  input.Read(node->transform, sizeof(node->transform));
  ModelMesh& mesh = *node->mesh;
  unsigned vertexCount = 0;
  input.Read(&vertexCount, 4);
  mesh.vertices.resize(vertexCount);
  input.Read(mesh.vertices.data(), vertexCount * 12u);
  unsigned textureVertexCount = 0;
  input.Read(&textureVertexCount, 4);
  mesh.textureVertices.resize(textureVertexCount);
  input.Read(mesh.textureVertices.data(), textureVertexCount * 12u);
  int faceCount = 0;
  input.Read(&faceCount, 4);
  mesh.faces.resize(static_cast<unsigned>(faceCount));

  for (ModelFace& face : mesh.faces) {
    std::vector<unsigned char> bytes;
    unsigned char fixed[24];
    const unsigned char* record;
    unsigned size;

    if (minor == 1) {
      input.Read(fixed, sizeof(fixed));
      record = fixed;
      size = sizeof(fixed);
    } else {
      size = 0;
      input.Read(&size, 4);
      bytes.resize(size);
      input.Read(bytes.data(), size);
      record = bytes.data();
    }

    std::memcpy(face.vertices, record, sizeof(face.vertices));
    std::memcpy(face.textureVertices, record + 6, sizeof(face.textureVertices));
    std::memcpy(&face.texture, record + 12, sizeof(face.texture));
    std::memcpy(&face.twoSided, record + 16, sizeof(face.twoSided));
    const unsigned groups = (size - 20u) / 4u;
    face.smoothing.resize(groups);
    std::memcpy(face.smoothing.data(), record + 20, groups * 4u);
  }

  // INLINED
  std::sort(mesh.faces.begin(), mesh.faces.end(), [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
  mesh.CalculateNormals();
  ReadKeys(input, node->scaleKeys, false);
  ReadKeys(input, node->rotationKeys, true);
  ReadKeys(input, node->positionKeys, false);

  if (minor == 3) {
    int textureAnimations = 0;
    input.Read(&textureAnimations, 4);

    for (int index = 0; index < textureAnimations; ++index) {
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
  }

  this->meshes[node->name] = node->mesh.get();
  this->nodes.push_back(node.release());
}

// OFFSET: 0x00759ED0
// OFFSET: 0x0075AB20
// OFFSET: 0x0075B8E0
void C3dModelResV2::ReadVersion(ResourceByteReader& input, unsigned minor) {
  input.Read(&this->animationLength, 4);
  input.Read(&this->shade, 4);
  input.Read(&this->alpha, 1);
  input.Read(&this->framesPerSecond, 4);
  std::vector<std::string> textureNames;
  int textureCount = 0;
  input.Read(&textureCount, 4);

  if (minor < 3) {
    for (int index = 0; index < textureCount; ++index) {
      unsigned bytes = 0;
      input.Read(&bytes, 4);
      char name[40]{};
      input.Read(name, bytes);
      textureNames.emplace_back(name);
    }
  } else {
    for (int index = 0; index < textureCount; ++index) {
      unsigned bytes = 0;
      input.Read(&bytes, 4);
      char name[40]{};
      input.Read(name, bytes);
      this->rootNames.emplace_back(name);
    }
  }

  if (minor < 3) {
    int rootCount = 0;
    input.Read(&rootCount, 4);

    for (int index = 0; index < rootCount; ++index) {
      unsigned bytes = 0;
      input.Read(&bytes, 4);
      char name[40]{};
      input.Read(name, bytes);
      this->rootNames.emplace_back(name);
    }
  }

  int nodeCount = 0;
  input.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    this->ReadNode(input, minor, textureNames);
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
