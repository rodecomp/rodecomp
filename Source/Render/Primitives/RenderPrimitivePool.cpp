#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Renderer.h"

#include <new>

// OFFSET: 0x00561A50
// FUNCTION: fn_02167
RenderPrimitivePool::~RenderPrimitivePool() = default;

namespace {
struct PrimitivePoolAccess : RenderPrimitivePool {
  // INLINED
  inline RenderPrimitive* AcquireLocal() {
    if (this->m_commands.end() == this->m_current) {
      this->m_commands.emplace_back();
      this->m_current = this->m_commands.end();
      return &this->m_commands.back();
    }

    RenderPrimitive* command = &*this->m_current;
    ++this->m_current;
    new (command) RenderPrimitive;

    return command;
  }
};
}

// OFFSET: 0x00543EC0
// FUNCTION: fn_01811
RenderPrimitive* CRenderer::AcquirePrimitive() {
  return static_cast<PrimitivePoolAccess&>(this->primitives).AcquireLocal();
}
