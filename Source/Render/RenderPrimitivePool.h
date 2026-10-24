#ifndef RAGNAROK_SOURCE_RENDER_RENDERPRIMITIVEPOOL_H
#define RAGNAROK_SOURCE_RENDER_RENDERPRIMITIVEPOOL_H

#include "Render/RenderPrimitive.h"

#include <deque>

// Original contiguous renderer subobject at +0x70. std::deque<168-byte value>
// uses one value per block, matching original map/block allocation semantics.
class RenderPrimitivePool {
public:
  RenderPrimitivePool();
  ~RenderPrimitivePool();
  RenderPrimitive* Acquire();
  void Rewind();

protected:
  std::deque<RenderPrimitive> m_commands;
  std::deque<RenderPrimitive>::iterator m_current;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderPrimitivePool) == 32);
#endif

#endif
