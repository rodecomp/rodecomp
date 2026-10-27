#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDPRIMITIVE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDPRIMITIVE_H

#include <cstddef>
#include <cstdint>
#include <deque>
#include <vector>

class CTexture;

struct LightmappedVertex {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
  /* 000C */ float rhw;
  /* 0010 */ std::uint32_t diffuse;
  /* 0014 */ std::uint32_t specular;
  /* 0018 */ float u;
  /* 001C */ float v;
  /* 0020 */ float lightU;
  /* 0024 */ float lightV;
};
struct LightmappedCommand {
  LightmappedCommand();

  /* 0000 */ LightmappedVertex* vertices;
  /* 0004 */ std::int32_t vertexCount;
  /* 0008 */ std::uint16_t* indices;
  /* 000C */ std::uint32_t indexCount;
  /* 0010 */ CTexture* texture;
  /* 0014 */ CTexture* lightmap;
  /* 0018 */ std::uint32_t primitiveType;
};

struct LightmappedPrimitive {
  LightmappedPrimitive();
  LightmappedPrimitive(const LightmappedPrimitive& other);

  /* 0000 */ LightmappedVertex* vertices;
  /* 0004 */ std::int32_t vertexCount;
  /* 0008 */ std::uint16_t* indices;
  /* 000C */ std::uint32_t indexCount;
  /* 0010 */ CTexture* texture;
  /* 0014 */ CTexture* lightmap;
  /* 0018 */ std::uint32_t primitiveType;
  /* 001C */ LightmappedVertex quad[4];
};

class LightmappedPrimitivePool {
public:
  LightmappedPrimitivePool();
  LightmappedPrimitive* Acquire();
  void Rewind();

private:
  /* 0090 */ std::deque<LightmappedPrimitive> m_commands;
  /* 00A4 */ std::deque<LightmappedPrimitive>::iterator m_current;
};

struct LightmappedQueues {
  /* 01F8 */ std::vector<LightmappedPrimitive*> first;
  /* 0204 */ std::vector<LightmappedPrimitive*> second;
  void Submit(LightmappedPrimitive* primitive, int queue, bool depthAlpha, float depthStart, float depthEnd);
  void Clear();
};

static_assert(sizeof(LightmappedVertex) == 40);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LightmappedPrimitive) == 188);
static_assert(sizeof(LightmappedCommand) == 28);
static_assert(sizeof(LightmappedPrimitivePool) == 0x20);
static_assert(sizeof(LightmappedQueues) == 0x18);
static_assert(offsetof(LightmappedVertex, lightU) == 0x0020);
static_assert(offsetof(LightmappedVertex, lightV) == 0x0024);
static_assert(offsetof(LightmappedCommand, primitiveType) == 0x0018);
static_assert(offsetof(LightmappedPrimitive, quad) == 0x001C);
static_assert(offsetof(LightmappedQueues, first) + 0x01F8 == 0x01F8);
static_assert(offsetof(LightmappedQueues, second) + 0x01F8 == 0x0204);
#endif

#endif
