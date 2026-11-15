#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Renderer.h"

#include <new>

// OFFSET: 0x00561A50
// FUNCTION: fn_02167
RenderPrimitivePool::~RenderPrimitivePool() = default;

// OFFSET: 0x00543EC0
// FUNCTION: fn_01811
RenderPrimitive* CRenderer::AcquirePrimitive() {
  if (this->primitives.m_current == this->primitives.m_commands.end()) {
    this->primitives.m_commands.emplace_back();
    this->primitives.m_current = this->primitives.m_commands.end();
    return &this->primitives.m_commands.back();
  }

  RenderPrimitive* command = &*this->primitives.m_current;
  ++this->primitives.m_current;
  new (command) RenderPrimitive;

  return command;
}
