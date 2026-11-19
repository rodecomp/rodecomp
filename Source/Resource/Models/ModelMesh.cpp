#include "Resource/Models/ModelMesh.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <map>

namespace {
  // INLINED
  void Normalize(ShadeVector& vector) {
    const float squared = (vector.x * vector.x + vector.y * vector.y) + vector.z * vector.z;
    const float inverse = 1.0f / SquareRoot(squared);

    vector.x = inverse * vector.x;
    vector.y = inverse * vector.y;
    vector.z = inverse * vector.z;
  }
}

// OFFSET: 0x00755880
// FUNCTION: fn_07988
ModelFace::ModelFace() = default;

// OFFSET: 0x00551BA0
// FUNCTION: fn_02007
ModelMesh::ModelMesh()
    : vertexCount(0), vertices(nullptr), faceCount(0), faces(nullptr), textureVertexCount(0), textureVertices(nullptr), faceNormals(nullptr),
      vertexNormals(nullptr) {
}

// OFFSET: 0x00551E80
// FUNCTION: fn_02009
ModelMesh::~ModelMesh() {
  if (this->vertices) {
    delete[] this->vertices;
    this->vertices = nullptr;
  }
  this->vertexCount = 0;
  if (this->faces) {
    delete[] this->faces;
    this->faces = nullptr;
  }
  this->faceCount = 0;
  if (this->textureVertices) {
    delete[] this->textureVertices;
    this->textureVertices = nullptr;
  }
  this->textureVertexCount = 0;
  if (this->faceNormals) {
    delete[] this->faceNormals;
    this->faceNormals = nullptr;
  }
  if (this->vertexNormals) {
    delete[] this->vertexNormals;
    this->vertexNormals = nullptr;
  }
}

// OFFSET: 0x00551F80
// FUNCTION: fn_02010
ModelFace::~ModelFace() = default;

// OFFSET: 0x00551FD0
// FUNCTION: fn_02011
void ModelMesh::CalculateNormals() {
  if (!this->faceNormals) {
    this->faceNormals = new ShadeVector[this->faceCount];
  }

  for (int index = 0; index < this->faceCount; ++index) {
    auto& normal = this->faceNormals[index];
    const ModelFace& face = this->faces[index];
    const auto& first = this->vertices[face.vertices[0]];
    const auto& third = this->vertices[face.vertices[2]];
    const auto& second = this->vertices[face.vertices[1]];
    const float x1 = first[0] - third[0];
    const float y1 = first[1] - third[1];
    const float z1 = first[2] - third[2];
    const float x2 = second[0] - third[0];
    const float y2 = second[1] - third[1];
    const float z2 = second[2] - third[2];

    normal.x = y2 * z1 - z2 * y1;
    normal.y = z2 * x1 - x2 * z1;
    normal.z = x2 * y1 - y2 * x1;
    Normalize(this->faceNormals[index]);
  }

  std::map<int, std::vector<int>> adjacent;

  for (int face = 0; face < this->faceCount; ++face) {
    for (int corner = 0; corner < 3; ++corner) {
      adjacent[this->faces[face].vertices[corner]].push_back(face);
    }
  }

  if (!this->vertexNormals) {
    this->vertexNormals = new ShadeVector[this->faceCount * 3];
  }

  for (int index = 0; index < this->faceCount; ++index) {
    const ModelFace& face = this->faces[index];

    for (int corner = 0; corner < 3; ++corner) {
      auto& neighbors = adjacent[face.vertices[corner]];
      for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
        const ModelFace& other = this->faces[*it];
        for (auto group = face.smoothing.begin(); group != face.smoothing.end(); ++group) {
          if (std::find(other.smoothing.begin(), other.smoothing.end(), *group) != other.smoothing.end()) {
            const ShadeVector& faceNormal = this->faceNormals[*it];
            ShadeVector& normal = this->vertexNormals[index * 3 + corner];
            normal.x = normal.x + faceNormal.x;
            normal.y = faceNormal.y + normal.y;
            normal.z = faceNormal.z + normal.z;
          }
        }
      }
    }
  }

  for (int index = 0; index < this->faceCount * 3; ++index) {
    Normalize(this->vertexNormals[index]);
  }
}
