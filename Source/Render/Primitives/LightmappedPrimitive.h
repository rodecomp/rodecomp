#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDPRIMITIVE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_LIGHTMAPPEDPRIMITIVE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <cstdint>
#include <deque>
#include <vector>

class CTexture;

struct LightmappedVertex {
  // INLINED
  LightmappedVertex() : x(0.0f), y(0.0f), z(0.0f), rhw(0.0f), diffuse(0), specular(0xFF000000u), u(0.0f), v(0.0f) {
  }

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
  // INLINED
  explicit LightmappedCommand(int) {
  }
  void DrawAuxiliary(int unused);
  void ApplyAuxiliaryDepthAlpha();

  /* 0000 */ LightmappedVertex* vertices;
  /* 0004 */ std::int32_t vertexCount;
  /* 0008 */ std::uint16_t* indices;
  /* 000C */ std::uint32_t indexCount;
  /* 0010 */ CTexture* texture;
  /* 0014 */ CTexture* lightmap;
  /* 0018 */ std::uint32_t primitiveType;
};

struct ModelRenderCommand : RenderCommand {
  ModelRenderCommand();
};

struct LightmappedPrimitive : LightmappedCommand {
  LightmappedPrimitive();
  LightmappedPrimitive(const LightmappedPrimitive& other);
  LightmappedPrimitive& operator=(const LightmappedPrimitive& other);
  void DrawLightmapped(int unused);
  void ApplyDepthAlpha();

  /* 001C */ LightmappedVertex quad[4];
};

class LightmappedPrimitivePool {
public:
  // INLINED
  __forceinline LightmappedPrimitivePool() {
    this->m_current = this->m_commands.begin();
  }

  // INLINED
  void Init(unsigned count) {
    this->m_commands.resize(count, LightmappedPrimitive());
    this->m_current = this->m_commands.begin();
  }

  LightmappedPrimitive* Acquire();
  void Rewind();

private:
  /* 0090 */ std::deque<LightmappedPrimitive> m_commands;
  /* 00A4 */ std::deque<LightmappedPrimitive>::iterator m_current;
};

static_assert(sizeof(LightmappedVertex) == 40);
// INLINED
inline void LightmappedPrimitivePool::Rewind() {
  this->m_current = this->m_commands.begin();
}

// INLINED
inline LightmappedPrimitive* LightmappedPrimitivePool::Acquire() {
  if (this->m_current == this->m_commands.end()) {
    this->m_commands.emplace_back();
    this->m_current = this->m_commands.end();
    return &this->m_commands.back();
  }

  LightmappedPrimitive* primitive = &*this->m_current++;
  return new (primitive) LightmappedPrimitive;
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LightmappedPrimitive) == 188);
static_assert(sizeof(LightmappedCommand) == 28);
static_assert(sizeof(ModelRenderCommand) == 40);
static_assert(sizeof(LightmappedPrimitivePool) == 0x20);
static_assert(offsetof(LightmappedVertex, lightU) == 0x0020);
static_assert(offsetof(LightmappedVertex, lightV) == 0x0024);
static_assert(offsetof(LightmappedCommand, primitiveType) == 0x0018);
static_assert(offsetof(LightmappedPrimitive, quad) == 0x001C);
#endif

#endif
