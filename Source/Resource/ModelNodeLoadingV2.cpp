#include "Render/Texture.h"
#include "Render/TextureCache.h"
#include "Resource/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <algorithm>
#include <cstring>

namespace {
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

// Node-decoding loops within modern version readers, not a separate machine entry.
void C3dModelResV2::ReadNode(ResourceByteReader& input, unsigned minor, const std::vector<std::string>& textureNames) {
  auto node = std::make_unique<ModelNode>(this->m_context->textures);
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

  std::sort(mesh.faces.begin(), mesh.faces.end(), [](const ModelFace& a, const ModelFace& b) {
    return a.texture < b.texture;
  });
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
      std::map<int, std::vector<std::array<std::uint32_t, 2>>> channels;
      int channelCount = 0;
      input.Read(&channelCount, 4);

      for (int channel = 0; channel < channelCount; ++channel) {
        int id = 0;
        int keyCount = 0;
        input.Read(&id, 4);
        input.Read(&keyCount, 4);

        for (int key = 0; key < keyCount; ++key) {
          std::array<std::uint32_t, 2> value;
          input.Read(value.data(), 8);
          channels[id].push_back(value);
        }
      }

      node->textureKeys[texture] = std::move(channels);
    }
  }

  this->meshes[node->name] = node->mesh.get();
  this->nodes.push_back(node.release());
}
