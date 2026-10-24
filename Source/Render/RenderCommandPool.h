#ifndef RAGNAROK_SOURCE_RENDER_RENDERCOMMANDPOOL_H
#define RAGNAROK_SOURCE_RENDER_RENDERCOMMANDPOOL_H

#include "Render/RenderPrimitive.h"

#include <deque>

// Original renderer subobject+0x50; commands borrow their vertex storage.
class RenderCommandPool {
public:
  RenderCommandPool();
  RenderCommand* Acquire();
  void Rewind();

private:
  std::deque<RenderCommand> m_commands;
  std::deque<RenderCommand>::iterator m_current;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderCommandPool) == 32);
#endif

#endif
