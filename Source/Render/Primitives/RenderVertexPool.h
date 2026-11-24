#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERVERTEXPOOL_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERVERTEXPOOL_H

#include "Render/Primitives/RenderPrimitive.h"

#include <forward_list>
#include <memory>
#include <vector>

class RenderVertexPool {
public:
  // INLINED
  RenderVertexPool() : m_minimum(1024), m_used(0) {
  }

  // INLINED
  void Init(unsigned minimum) {
    this->m_used = 0;
    this->m_minimum = minimum;
    DeleteBlocks(this->m_active);
    DeleteBlocks(this->m_unused);
    this->Activate(this->m_minimum);
  }

  ~RenderVertexPool();
  RenderVertex* Acquire(unsigned count);
  void Rewind();

private:
  struct Vertex : RenderVertex {
    Vertex() {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->rhw = 0.0f;
      this->diffuse = 0;
      this->specular = 0xFF000000u;
      this->u = 0.0f;
      this->v = 0.0f;
    }
  };

  struct Block : std::vector<Vertex> {
    RenderVertex* Append(unsigned count);
  };

  /* 0000 */ unsigned m_minimum;
  /* 0004 */ unsigned m_used;
  /* 0008 */ std::forward_list<Block*> m_active;
  /* 000C */ std::forward_list<Block*> m_unused;
  Block& Activate(unsigned capacity);
  void Release();
  static void DeleteBlocks(std::forward_list<Block*>& blocks);
};

// INLINED
inline void RenderVertexPool::DeleteBlocks(std::forward_list<Block*>& blocks) {
  for (Block* block : blocks) {
    delete block;
  }
  blocks.clear();
}

// INLINED
inline void RenderVertexPool::Rewind() {
  for (auto& block : this->m_active) {
    block->clear();
  }

  this->m_unused.splice_after(this->m_unused.before_begin(), this->m_active);
  this->m_used = 0;
  this->Activate(this->m_minimum);
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderVertexPool) == 16);
#endif

#endif
