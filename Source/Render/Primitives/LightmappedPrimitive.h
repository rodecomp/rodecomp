#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDPRIMITIVE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDPRIMITIVE_H

#include <cstdint>
#include <deque>
#include <vector>

class CTexture;

struct LightmappedVertex {
  float x;
  float y;
  float z;
  float rhw;
  std::uint32_t diffuse;
  std::uint32_t specular;
  float u;
  float v;
  float lightU;
  float lightV;
};
struct LightmappedCommand {
  LightmappedCommand();

  LightmappedVertex* vertices;
  std::int32_t vertexCount;
  std::uint16_t* indices;
  std::uint32_t indexCount;
  CTexture* texture;
  CTexture* lightmap;
  std::uint32_t primitiveType;
};

struct LightmappedPrimitive {
  LightmappedPrimitive();
  LightmappedPrimitive(const LightmappedPrimitive& other);

  LightmappedVertex* vertices;
  std::int32_t vertexCount;
  std::uint16_t* indices;
  std::uint32_t indexCount;
  CTexture* texture;
  CTexture* lightmap;
  std::uint32_t primitiveType;
  LightmappedVertex quad[4];
};

class LightmappedPrimitivePool {
public:
  LightmappedPrimitivePool();
  LightmappedPrimitive* Acquire();
  void Rewind();

private:
  std::deque<LightmappedPrimitive> m_commands;
  std::deque<LightmappedPrimitive>::iterator m_current;
};

struct LightmappedQueues {
  std::vector<LightmappedPrimitive*> first;
  std::vector<LightmappedPrimitive*> second;
  void Submit(LightmappedPrimitive* primitive, int queue, bool depthAlpha, float depthStart, float depthEnd);
  void Clear();
};

static_assert(sizeof(LightmappedVertex) == 40);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LightmappedPrimitive) == 188);
static_assert(sizeof(LightmappedCommand) == 28);
#endif

#endif
