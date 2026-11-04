#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERVERTEXPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERVERTEXPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <forward_list>
#include <memory>
#include <vector>

class RenderVertexPool {
public:
  RenderVertexPool();
  ~RenderVertexPool();
  RenderVertex* Acquire(unsigned count);
  void Rewind();

private:
  struct Block : std::vector<RenderVertex> {
    RenderVertex* Append(unsigned count);
  };

  /* 0000 */ unsigned m_minimum;
  /* 0004 */ unsigned m_used;
  /* 0008 */ std::forward_list<Block*> m_active;
  /* 000C */ std::forward_list<Block*> m_unused;
  Block& Activate(unsigned capacity);
  void Release();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderVertexPool) == 16);
#endif

#endif
