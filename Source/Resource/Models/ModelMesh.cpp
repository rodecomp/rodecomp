#include "Resource/Models/ModelMesh.h"

#include <cmath>
#include <map>

namespace {
  // INLINED
  void Normalize(float* vector) {
    const float squared = (vector[0] * vector[0] + vector[1] * vector[1]) + vector[2] * vector[2];
    const float length = std::sqrt(squared);
    const float inverse = 1.0f / length;

    vector[0] = inverse * vector[0];
    vector[1] = inverse * vector[1];
    vector[2] = inverse * vector[2];
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
  delete[] this->vertices;
  this->vertices = nullptr;
  this->vertexCount = 0;
  delete[] this->faces;
  this->faces = nullptr;
  this->faceCount = 0;
  delete[] this->textureVertices;
  this->textureVertices = nullptr;
  this->textureVertexCount = 0;
  delete[] this->faceNormals;
  this->faceNormals = nullptr;
  delete[] this->vertexNormals;
  this->vertexNormals = nullptr;
}

// OFFSET: 0x00551FD0
// FUNCTION: fn_02011
void ModelMesh::CalculateNormals() {
  if (!this->faceNormals) {
    this->faceNormals = new std::array<float, 3>[this->faceCount]{};
  }

  for (int index = 0; index < this->faceCount; ++index) {
    const ModelFace& face = this->faces[index];
    const auto& first = this->vertices[face.vertices[0]];
    const auto& second = this->vertices[face.vertices[1]];
    const auto& third = this->vertices[face.vertices[2]];
    const float x1 = first[0] - third[0];
    const float y1 = first[1] - third[1];
    const float z1 = first[2] - third[2];
    const float x2 = second[0] - third[0];
    const float y2 = second[1] - third[1];
    const float z2 = second[2] - third[2];
    auto& normal = this->faceNormals[index];

    normal[0] = y2 * z1 - z2 * y1;
    normal[1] = z2 * x1 - x2 * z1;
    normal[2] = x2 * y1 - y2 * x1;
    Normalize(normal.data());
  }

  std::map<int, std::vector<int>> adjacent;

  for (int face = 0; face < this->faceCount; ++face) {
    for (int corner = 0; corner < 3; ++corner) {
      adjacent[this->faces[face].vertices[corner]].push_back(face);
    }
  }

  if (!this->vertexNormals) {
    this->vertexNormals = new std::array<float, 3>[this->faceCount * 3]{};
  }

  for (int index = 0; index < this->faceCount; ++index) {
    const ModelFace& face = this->faces[index];

    for (int corner = 0; corner < 3; ++corner) {
      auto& normal = this->vertexNormals[index * 3 + corner];

      for (int neighbor : adjacent[face.vertices[corner]]) {
        const ModelFace& other = this->faces[neighbor];

        for (std::uint32_t group : face.smoothing) {
          bool matches = false;

          for (std::uint32_t candidate : other.smoothing) {
            if (candidate == group) {
              matches = true;
              break;
            }
          }

          if (matches) {
            normal[0] = normal[0] + this->faceNormals[neighbor][0];
            normal[1] = this->faceNormals[neighbor][1] + normal[1];
            normal[2] = this->faceNormals[neighbor][2] + normal[2];
          }
        }
      }
    }
  }

  for (int index = 0; index < this->faceCount * 3; ++index) {
    Normalize(this->vertexNormals[index].data());
  }
}
