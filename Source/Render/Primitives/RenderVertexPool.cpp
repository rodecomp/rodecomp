#include "Render/Primitives/RenderVertexPool.h"

#include <algorithm>

namespace {
  // INLINED
  void Append(std::vector<RenderVertex>& block, unsigned count) {
    const unsigned first = static_cast<unsigned>(block.size());
    block.resize(first + count);

    for (unsigned index = first; index < block.size(); ++index) {
      block[index].specular = 0xFF000000u;
    }
  }
}

// INLINED
RenderVertexPool::RenderVertexPool() : m_minimum(10240), m_used(0) {
  this->Activate(this->m_minimum);
}

// OFFSET: 0x00559AA0
// FUNCTION: fn_02070
RenderVertexPool::Block& RenderVertexPool::Activate(unsigned capacity) {
  auto previous = this->m_unused.before_begin();

  for (auto current = this->m_unused.begin(); current != this->m_unused.end(); ++current) {
    if ((*current)->capacity() >= capacity) {
      this->m_active.splice_after(this->m_active.before_begin(), this->m_unused, previous);
      return *this->m_active.front();
    }

    previous = current;
  }

  auto block = std::make_unique<Block>();
  block->reserve(capacity);
  this->m_active.push_front(std::move(block));

  return *this->m_active.front();
}

// OFFSET: 0x00ABC950
// FUNCTION: fn_17537
RenderVertex* RenderVertexPool::Acquire(unsigned count) {
  if (count == 0) {
    return nullptr;
  }

  for (auto& block : this->m_active) {
    if (block->capacity() - block->size() >= count) {
      const unsigned first = static_cast<unsigned>(block->size());
      Append(*block, count);
      this->m_used += count;

      return block->data() + first;
    }
  }

  Block& block = this->Activate(std::max(this->m_minimum, count));
  Append(block, count);
  this->m_used += count;

  return block.data();
}

// OFFSET: 0x00AB77A0
RenderVertex* RenderVertexPool::Block::Append(unsigned count) {
  const unsigned first = static_cast<unsigned>(this->size());
  if (first + count > this->capacity()) {
    return nullptr;
  }

  this->resize(first + count);

  for (unsigned index = first; index < this->size(); ++index) {
    (*this)[index].specular = 0xFF000000u;
  }

  return this->data() + (this->size() - count);
}

// INLINED
void RenderVertexPool::Rewind() {
  for (auto& block : this->m_active) {
    block->clear();
  }

  this->m_unused.splice_after(this->m_unused.before_begin(), this->m_active);
  this->m_used = 0;
  this->Activate(this->m_minimum);
}
