#include "Render/Primitives/RenderVertexPool.h"

#include <algorithm>

// OFFSET: 0x00561690
// FUNCTION: fn_02162
RenderVertexPool::~RenderVertexPool() {
  this->Release();
}

// OFFSET: 0x00562290
// FUNCTION: fn_02177
__declspec(noinline) void RenderVertexPool::Release() {
  for (Block* block : this->m_active) {
    delete block;
  }
  this->m_active.clear();

  for (Block* block : this->m_unused) {
    delete block;
  }
  this->m_unused.clear();
}

// OFFSET: 0x00559AA0
// FUNCTION: fn_02070
RenderVertexPool::Block& RenderVertexPool::Activate(unsigned capacity) {
  Block* block = nullptr;
  auto previous = this->m_unused.before_begin();

  for (auto current = this->m_unused.begin(); current != this->m_unused.end(); ++current) {
    if ((*current)->capacity() >= capacity) {
      block = *current;
      this->m_active.splice_after(this->m_active.before_begin(), this->m_unused, previous);
      break;
    }

    previous = current;
  }

  if (!block) {
    block = new Block();
    block->reserve(capacity);
    this->m_active.push_front(block);
  }

  return *block;
}

// OFFSET: 0x00ABC950
// FUNCTION: fn_17537
RenderVertex* RenderVertexPool::Acquire(unsigned count) {
  if (count == 0) {
    return nullptr;
  }

  RenderVertex* vertices = nullptr;
  for (Block* block : this->m_active) {
    vertices = block->Append(count);
    if (vertices) {
      break;
    }
  }

  if (!vertices) {
    vertices = this->Activate(std::max(this->m_minimum, count)).Append(count);
  }

  if (vertices) {
    this->m_used += count;
  }

  return vertices;
}

// OFFSET: 0x00AB77A0
// FUNCTION: fn_17486
RenderVertex* RenderVertexPool::Block::Append(unsigned count) {
  const unsigned first = static_cast<unsigned>(this->size());
  if (first + count > this->capacity()) {
    return nullptr;
  }

  this->resize(first + count);

  return this->data() + (this->size() - count);
}
