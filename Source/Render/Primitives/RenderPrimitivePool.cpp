#include "Render/Primitives/RenderPrimitivePool.h"

#include <new>

// OFFSET: 0x00561A50
// FUNCTION: fn_02167
RenderPrimitivePool::~RenderPrimitivePool() = default;

// OFFSET: 0x00543EC0
// FUNCTION: fn_01811
RenderPrimitive* RenderPrimitivePool::Acquire() {
  if (this->m_current == this->m_commands.end()) {
    this->m_commands.emplace_back();
    this->m_current = this->m_commands.end();
    return &this->m_commands.back();
  }

  RenderPrimitive* command = &*this->m_current;
  ++this->m_current;
  new (command) RenderPrimitive;

  return command;
}
