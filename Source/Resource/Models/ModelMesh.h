#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_MODELMESH_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_MODELMESH_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

struct ModelFace {
  ModelFace();

  /* 0000 */ std::uint16_t vertices[3];
  /* 0006 */ std::uint16_t textureVertices[3];
  /* 000C */ std::uint16_t texture;
  /* 000E */ std::uint16_t unknown_000E;
  /* 0010 */ std::uint32_t twoSided;
  /* 0014 */ std::vector<std::uint32_t> smoothing;
};

struct ModelTextureVertex {
  /* 0000 */ std::uint32_t shaded;
  /* 0004 */ float u;
  /* 0008 */ float v;
};

static_assert(sizeof(ModelTextureVertex) == 12);

struct ModelMesh {
  ModelMesh();
  ~ModelMesh();
  ModelMesh(const ModelMesh&) = delete;
  ModelMesh& operator=(const ModelMesh&) = delete;

  /* 0000 */ int vertexCount;
  /* 0004 */ std::array<float, 3>* vertices;
  /* 0008 */ int faceCount;
  /* 000C */ ModelFace* faces;
  /* 0010 */ int textureVertexCount;
  /* 0014 */ ModelTextureVertex* textureVertices;
  /* 0018 */ std::array<float, 3>* faceNormals;
  /* 001C */ std::array<float, 3>* vertexNormals;
  /* 0020 */ unsigned char unknown_0020[4];
  /* 0024 */ std::array<std::vector<std::uint32_t>, 32> indices;
  /* 01A4 */ unsigned char unknown_01A4[48];

  void CalculateNormals();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModelFace) == 32);
static_assert(offsetof(ModelFace, vertices) == 0x00);
static_assert(offsetof(ModelFace, textureVertices) == 0x06);
static_assert(offsetof(ModelFace, texture) == 0x0C);
static_assert(offsetof(ModelFace, unknown_000E) == 0x0E);
static_assert(offsetof(ModelFace, twoSided) == 0x10);
static_assert(offsetof(ModelFace, smoothing) == 0x14);
static_assert(sizeof(ModelMesh) == 0x1D4);
static_assert(offsetof(ModelMesh, vertices) == 0x04);
static_assert(offsetof(ModelMesh, faceCount) == 0x08);
static_assert(offsetof(ModelMesh, faces) == 0x0C);
static_assert(offsetof(ModelMesh, textureVertexCount) == 0x10);
static_assert(offsetof(ModelMesh, textureVertices) == 0x14);
static_assert(offsetof(ModelMesh, faceNormals) == 0x18);
static_assert(offsetof(ModelMesh, vertexNormals) == 0x1C);
static_assert(offsetof(ModelMesh, indices) == 0x24);
static_assert(offsetof(ModelMesh, unknown_01A4) == 0x1A4);
#endif

#endif
