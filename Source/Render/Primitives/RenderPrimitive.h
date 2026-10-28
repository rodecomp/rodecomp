#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVE_H

#include <cstddef>
#include <cstdint>

class CTexture;

struct RenderVertex {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
  /* 000C */ float rhw;
  /* 0010 */ std::uint32_t diffuse;
  /* 0014 */ std::uint32_t specular;
  /* 0018 */ float u;
  /* 001C */ float v;

  RenderVertex* Reset();
};

struct RenderTriangle {
  /* 0000 */ RenderVertex vertices[3];
  /* 0060 */ CTexture* texture;
  /* 0064 */ std::uint32_t unknown_0064;
  /* 0068 */ std::uint32_t unknown_0068;

  bool Visible() const;
};

struct RenderCommand {
  /* 0000 */ RenderVertex* vertices;
  /* 0004 */ std::int32_t vertexCount;
  /* 0008 */ CTexture* texture;
  /* 000C */ std::uint16_t* indices;
  /* 0010 */ std::uint32_t indexCount;
  /* 0014 */ std::uint16_t unknown_0014;
  /* 0016 */ std::uint16_t unknown_0016;
  /* 0018 */ std::uint32_t unknown_0018;
  /* 001C */ std::uint32_t unknown_001C;
  /* 0020 */ std::uint32_t unknown_0020;
  /* 0024 */ std::uint32_t primitiveType;

  RenderCommand();
  RenderCommand* FreeVertices();
  void AllocateTriangles(std::uint32_t triangleCount);
  void SetPosition(std::uint32_t index, const float* position);
  void Draw(int unused);
};

struct RenderPrimitive : RenderCommand {
  /* 0028 */ RenderVertex quad[4];

  RenderPrimitive();
  RenderPrimitive(const RenderPrimitive& other);
  RenderPrimitive& operator=(const RenderPrimitive& other);
};

void DrawPrimitive(RenderCommand* primitive);

static_assert(sizeof(RenderVertex) == 32);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderCommand) == 40);
static_assert(sizeof(RenderTriangle) == 108);
static_assert(sizeof(RenderPrimitive) == 168);
static_assert(offsetof(RenderVertex, rhw) == 0x000C);
static_assert(offsetof(RenderVertex, specular) == 0x0014);
static_assert(offsetof(RenderVertex, u) == 0x0018);
static_assert(offsetof(RenderVertex, v) == 0x001C);
static_assert(offsetof(RenderTriangle, texture) == 0x0060);
static_assert(offsetof(RenderTriangle, unknown_0068) == 0x0068);
static_assert(offsetof(RenderCommand, unknown_0016) == 0x0016);
static_assert(offsetof(RenderCommand, primitiveType) == 0x0024);
static_assert(offsetof(RenderPrimitive, quad) == 0x0028);
#endif

#endif
