#include "Security/ThemidaMgr.h"

#include <new>

// GLOBAL: gl_24891
CThemidaMgr* g_themidaMgr;

// OFFSET: 0x0076EC00
// FUNCTION: fn_08235
CThemidaMgr::CThemidaMgr() : m_valid(true) {
}

// OFFSET: 0x0076EC10
// FUNCTION: fn_08236
CThemidaMgr::~CThemidaMgr() {
}

// OFFSET: 0x0076EDE0
// FUNCTION: fn_08240
bool CThemidaMgr::Create() {
  if (g_themidaMgr == nullptr) {
    g_themidaMgr = new (std::nothrow) CThemidaMgr();
    if (g_themidaMgr != nullptr) {
      if (g_themidaMgr->m_valid) {
        return true;
      }
      delete g_themidaMgr;
      g_themidaMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x0076EE90
// FUNCTION: fn_08241
void CThemidaMgr::Destroy() {
  if (g_themidaMgr != nullptr) {
    delete g_themidaMgr;
    g_themidaMgr = nullptr;
  }
}
