#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVEPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVEPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <deque>

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
