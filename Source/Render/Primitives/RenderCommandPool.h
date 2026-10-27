#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERCOMMANDPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERCOMMANDPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <deque>

class RenderCommandPool {
public:
  RenderCommandPool();
  RenderCommand* Acquire();
  void Rewind();

private:
  /* 0050 */ std::deque<RenderCommand> m_commands;
  /* 0064 */ std::deque<RenderCommand>::iterator m_current;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderCommandPool) == 32);
#endif

#endif
