#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERCOMMANDPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERCOMMANDPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <deque>
#include <new>

class RenderCommandPool {
public:
  // INLINED
  __forceinline RenderCommandPool() {
    this->m_current = this->m_commands.begin();
  }

  // INLINED
  void Init(unsigned count) {
    this->m_commands.resize(count, RenderCommand());
    this->m_current = this->m_commands.begin();
  }

  RenderCommand* Acquire();
  void Rewind();

private:
  /* 0050 */ std::deque<RenderCommand> m_commands;
  /* 0064 */ std::deque<RenderCommand>::iterator m_current;
};

// OFFSET: 0x00CA63D0
// FUNCTION: fn_20958
inline RenderCommand* RenderCommandPool::Acquire() {
  if (this->m_current == this->m_commands.end()) {
    this->m_commands.emplace_back();
    this->m_current = this->m_commands.end();
    return &this->m_commands.back();
  }

  RenderCommand* command = &*this->m_current++;
  return new (command) RenderCommand;
}

// INLINED
inline void RenderCommandPool::Rewind() {
  this->m_current = this->m_commands.begin();
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderCommandPool) == 32);
#endif

#endif
