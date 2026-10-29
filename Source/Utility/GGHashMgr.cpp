#include "Utility/GGHashMgr.h"

// GLOBAL: gl_24843
CGGHashMgr* g_ggHashMgr = nullptr;

// OFFSET: 0x0076B7C0
// FUNCTION: fn_08189
CGGHashMgr::~CGGHashMgr() {
  if (this->m_hash) {
    delete this->m_hash;
    this->m_hash = nullptr;
  }

  DeleteCriticalSection(&this->m_lock);
}

// OFFSET: 0x0076BEE0
// FUNCTION: fn_08198
void CGGHashMgr::Destroy() {
  if (g_ggHashMgr) {
    delete g_ggHashMgr;
    g_ggHashMgr = nullptr;
  }
}
