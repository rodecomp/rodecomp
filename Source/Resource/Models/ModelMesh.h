#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_MODELMESH_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_MODELMESH_H

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

struct ModelTextureVertex {
  /* 0000 */ std::uint32_t shaded;
  /* 0004 */ float u;
  /* 0008 */ float v;
};

static_assert(sizeof(ModelTextureVertex) == 12);

struct ModelMesh {
  std::vector<std::array<float, 3>> vertices;
  std::vector<ModelTextureVertex> textureVertices;
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
