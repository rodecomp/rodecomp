#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceByteReader.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00750B10
// FUNCTION: fn_07950
void C3dModelResV1::ReadNode(ResourceByteReader& input, unsigned minor, const std::vector<std::string>& textureNames) {
  auto node = std::make_unique<ModelNode>(this->m_context->textures);
  node->model = this;
  node->alpha = this->alpha;
  char field[40]{};
  input.Read(field, sizeof(field));
  node->name = field;
  input.Read(field, sizeof(field));
  node->parentName = field;
  int textureCount = 0;
  input.Read(&textureCount, 4);

  for (int index = 0; index < textureCount; ++index) {
    int textureIndex = 0;
    input.Read(&textureIndex, 4);
    CTexture* texture = this->m_context->textures.Load(textureNames[textureIndex].c_str(), 0, false, true, false);
    node->textures.push_back(texture);

    if (texture) {
      texture->Retain();
    }
  }

  float matrix[12];
  input.Read(matrix, sizeof(matrix));
  input.Read(node->position, sizeof(node->position));
  input.Read(&node->angle, 4);
  input.Read(node->axis, sizeof(node->axis));
  input.Read(node->scale, sizeof(node->scale));
  node->mesh = std::make_unique<ModelMesh>();
  ModelMesh& mesh = *node->mesh;
  int vertexCount = 0;
  input.Read(&vertexCount, 4);
  mesh.vertices.resize(static_cast<unsigned>(vertexCount));
  input.Read(mesh.vertices.data(), static_cast<unsigned>(vertexCount) * 12u);

  for (auto& vertex : mesh.vertices) {
    const float y = ((vertex[0] * matrix[1] + vertex[1] * matrix[4]) + matrix[7] * vertex[2]) + matrix[10];
    const float z = ((vertex[0] * matrix[2] + vertex[1] * matrix[5]) + matrix[8] * vertex[2]) + matrix[11];
    vertex[0] = ((vertex[0] * matrix[0] + vertex[1] * matrix[3]) + vertex[2] * matrix[6]) + matrix[9];
    vertex[1] = y;
    vertex[2] = z;
  }

  int textureVertexCount = 0;
  input.Read(&textureVertexCount, 4);
  mesh.textureVertices.resize(static_cast<unsigned>(textureVertexCount));

  if (minor == 1) {
    float uv[2];

    for (auto& vertex : mesh.textureVertices) {
      input.Read(uv, sizeof(uv));
      vertex = {0xFFFFFFFFu, uv[0], uv[1]};
    }
  } else {
    input.Read(mesh.textureVertices.data(), static_cast<unsigned>(textureVertexCount) * 12u);
  }

  std::uint32_t faceCount = 0;
  input.Read(&faceCount, 4);
  mesh.faces.resize(faceCount);
  unsigned char record[24];

  for (ModelFace& face : mesh.faces) {
    input.Read(record, minor == 1 ? 20 : 24);
    std::memcpy(face.vertices, record, sizeof(face.vertices));
    std::memcpy(face.textureVertices, record + 6, sizeof(face.textureVertices));
    std::memcpy(&face.texture, record + 12, sizeof(face.texture));
    std::memcpy(&face.twoSided, record + 16, sizeof(face.twoSided));
    std::uint32_t group = 0;

    if (minor != 1) {
      std::memcpy(&group, record + 20, sizeof(group));
    }

    face.smoothing.push_back(group);
  }

  std::sort(mesh.faces.begin(), mesh.faces.end(), [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
  mesh.CalculateNormals();
  int rotationCount;
  input.Read(&rotationCount, 4);

  for (int index = 0; index < rotationCount; ++index) {
    ModelRotationKey key;
    input.Read(&key, sizeof(key));
    node->rotationKeys.push_back(key);
  }

  this->meshes[node->name] = node->mesh.get();
  this->nodes.push_back(node.release());
}
