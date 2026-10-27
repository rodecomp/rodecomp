#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERVERTEXPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERVERTEXPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <forward_list>
#include <memory>
#include <vector>

class RenderVertexPool {
public:
  RenderVertexPool();
  RenderVertex* Acquire(unsigned count);
  void Rewind();

private:
  using Block = std::vector<RenderVertex>;
  /* 00B0 */ unsigned m_minimum;
  /* 00B4 */ unsigned m_used;
  /* 00B8 */ std::forward_list<std::unique_ptr<Block>> m_active;
  /* 00BC */ std::forward_list<std::unique_ptr<Block>> m_unused;
  Block& Activate(unsigned capacity);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderVertexPool) == 16);
#endif

#endif
