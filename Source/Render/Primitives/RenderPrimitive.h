#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVE_H

#include <cstddef>
#include <cstdint>

class CTexture;

struct RenderVertex {
  float x;
  float y;
  float z;
  float rhw;
  std::uint32_t diffuse;
  std::uint32_t specular;
  float u;
  float v;

  RenderVertex* Reset();
};

struct RenderTriangle {
  RenderVertex vertices[3];
  CTexture* texture;
  std::uint32_t unknown_0064;
  std::uint32_t unknown_0068;

  bool Visible() const;
};

struct RenderCommand {
  RenderVertex* vertices;
  std::int32_t vertexCount;
  CTexture* texture;
  std::uint16_t* indices;
  std::uint32_t indexCount;
  std::uint16_t unknown_0014;
  std::uint16_t unknown_0016;
  std::uint32_t unknown_0018;
  std::uint32_t unknown_001C;
  std::uint32_t unknown_0020;
  std::uint32_t primitiveType;

  RenderCommand();
  RenderCommand* FreeVertices();
  void AllocateTriangles(std::uint32_t triangleCount);
  void SetPosition(std::uint32_t index, const float* position);
  void Draw(int unused);
};

struct RenderPrimitive : RenderCommand {
  RenderVertex quad[4];

  RenderPrimitive();
  RenderPrimitive(const RenderPrimitive& other);
  RenderPrimitive& operator=(const RenderPrimitive& other);
};

static_assert(sizeof(RenderVertex) == 32);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderCommand) == 40);
static_assert(sizeof(RenderTriangle) == 108);
static_assert(sizeof(RenderPrimitive) == 168);
#endif

#endif
