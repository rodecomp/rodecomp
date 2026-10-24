#ifndef RAGNAROK_SOURCE_RESOURCE_MODELMESH_H
#define RAGNAROK_SOURCE_RESOURCE_MODELMESH_H

#include <array>
#include <cstdint>
#include <vector>

struct ModelFace {
  ModelFace();

  std::uint16_t vertices[3];
  std::uint16_t textureVertices[3];
  std::uint16_t texture;
  std::uint16_t unknown_000E;
  std::uint32_t twoSided;
  std::vector<std::uint32_t> smoothing;
};

// Model mesh owns positions, texture vertices, faces and normal arrays.
// The original 0x1D4-byte owner also holds 32 index vectors used by other paths.
struct ModelMesh {
  std::vector<std::array<float, 3>> vertices;
  std::vector<std::array<std::uint32_t, 3>> textureVertices;
  std::vector<ModelFace> faces;
  std::vector<std::array<float, 3>> faceNormals;
  std::vector<std::array<float, 3>> vertexNormals;
  std::array<std::vector<std::uint32_t>, 32> indices;

  void CalculateNormals();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModelFace) == 32);
#endif

#endif
