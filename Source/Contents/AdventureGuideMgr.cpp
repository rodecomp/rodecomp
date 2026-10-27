#include "Contents/AdventureGuideMgr.h"

// GLOBAL: gl_24502
CAdventureGuideMgr* g_adventureGuideMgr;

// OFFSET: 0x00660F70
// FUNCTION: fn_05222
CAdventureGuideMgr::CAdventureGuideMgr() : m_initialized(true) {
}

// OFFSET: 0x006629D0
// FUNCTION: fn_05227
CAdventureGuideMgr::~CAdventureGuideMgr() {
}

// OFFSET: 0x00663680
// FUNCTION: fn_05235
void CAdventureGuideMgr::Destroy() {
  if (g_adventureGuideMgr) {
    delete g_adventureGuideMgr;
    g_adventureGuideMgr = nullptr;
  }
}
