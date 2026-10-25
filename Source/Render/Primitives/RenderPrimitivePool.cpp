#include "Render/Primitives/RenderPrimitivePool.h"

#include <new>

// OFFSET: 0x00560A40
// FUNCTION: fn_02161
RenderPrimitivePool::RenderPrimitivePool() {
  this->m_commands.resize(512, RenderPrimitive());
  this->m_current = this->m_commands.begin();
}

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

  RenderPrimitive* command = &*this->m_current++;

  return new (command) RenderPrimitive;
}

// OFFSET: 0x0055A360
// FUNCTION: fn_02079
void RenderPrimitivePool::Rewind() {
  this->m_current = this->m_commands.begin();
}
