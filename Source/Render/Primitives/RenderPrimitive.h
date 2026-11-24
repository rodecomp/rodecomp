#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVE_H

#include "Render/Drawing/SpriteGeometry.h"

#include <cstddef>
#include <new>
#include <cstdint>

class CTexture;

struct RenderVertex {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
  /* 000C */ float rhw;
  union {
    /* 0010 */ std::uint32_t diffuse;
    /* 0010 */ SpriteColor diffuseChannels;
  };
  /* 0014 */ std::uint32_t specular;
  /* 0018 */ float u;
  /* 001C */ float v;

  RenderVertex* Reset();
};

struct RenderTriangleVertex : RenderVertex {
  RenderTriangleVertex() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->rhw = 0;
    this->diffuse = 0;
    this->specular = 0xFF000000;
    this->u = 0;
    this->v = 0;
  }
};

struct RenderTriangle {
  // OFFSET: 0x00BAE110
  // FUNCTION: fn_46549
  RenderTriangle() {
    ::new (static_cast<void*>(this->vertices)) RenderTriangleVertex[3];
  }

  /* 0000 */ RenderVertex vertices[3];
  /* 0060 */ CTexture* texture;
  /* 0064 */ std::uint32_t sourceBlend;
  /* 0068 */ std::uint32_t destinationBlend;

  bool Visible() const;
};

struct RenderCommand {
  /* 0000 */ RenderVertex* vertices;
  /* 0004 */ std::int32_t vertexCount;
  /* 0008 */ CTexture* texture;
  /* 000C */ std::uint16_t* indices;
  /* 0010 */ std::uint32_t indexCount;
  union {
    /* 0014 */ std::uint16_t depthFlags;
    struct {
      /* 0014 */ unsigned char depthTest;
      /* 0015 */ unsigned char commandMode;
    };
  };
  /* 0016 */ std::uint16_t padding_0016;
  /* 0018 */ std::uint32_t sourceBlend;
  /* 001C */ std::uint32_t destinationBlend;
  /* 0020 */ std::uint32_t textureMode;
  /* 0024 */ std::uint32_t primitiveType;

  RenderCommand();
  // INLINED
  explicit RenderCommand(int) {
  }
  void ApplyDepthAlpha();
  void FreeVertices();
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
static_assert(offsetof(RenderVertex, diffuse) == 0x0010);
static_assert(offsetof(RenderVertex, diffuseChannels) == 0x0010);
static_assert(offsetof(RenderVertex, specular) == 0x0014);
static_assert(offsetof(RenderVertex, u) == 0x0018);
static_assert(offsetof(RenderVertex, v) == 0x001C);
static_assert(offsetof(RenderTriangle, texture) == 0x0060);
static_assert(offsetof(RenderTriangle, destinationBlend) == 0x0068);
static_assert(offsetof(RenderCommand, padding_0016) == 0x0016);
static_assert(offsetof(RenderCommand, primitiveType) == 0x0024);
static_assert(offsetof(RenderPrimitive, quad) == 0x0028);
#endif

#endif
