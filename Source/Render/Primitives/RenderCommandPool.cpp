#include "Render/Primitives/RenderCommandPool.h"

#include <new>

// OFFSET: 0x00560A40
// FUNCTION: fn_02161
RenderCommandPool::RenderCommandPool() {
  this->m_commands.resize(1024);
  this->m_current = this->m_commands.begin();
}

// OFFSET: 0x00CA63D0
// FUNCTION: fn_20958
RenderCommand* RenderCommandPool::Acquire() {
  if (this->m_current == this->m_commands.end()) {
    this->m_commands.emplace_back();
    this->m_current = this->m_commands.end();
    return &this->m_commands.back();
  }

  RenderCommand* command = &*this->m_current++;
  return new (command) RenderCommand;
}

// OFFSET: 0x0055A360
// FUNCTION: fn_02079
void RenderCommandPool::Rewind() {
  this->m_current = this->m_commands.begin();
}
