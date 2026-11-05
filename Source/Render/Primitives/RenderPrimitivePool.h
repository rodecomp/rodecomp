#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVEPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERPRIMITIVEPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <deque>

class RenderPrimitivePool {
public:
  // INLINED
  __forceinline RenderPrimitivePool() {
    this->m_commands.resize(512, RenderPrimitive());
    this->m_current = this->m_commands.begin();
  }

  ~RenderPrimitivePool();
  RenderPrimitive* Acquire();
  void Rewind();

protected:
  /* 0070 */ std::deque<RenderPrimitive> m_commands;
  /* 0084 */ std::deque<RenderPrimitive>::iterator m_current;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderPrimitivePool) == 32);
#endif

#endif
