#include "Contents/ConfigMgr.h"

#include <new>

// GLOBAL: gl_24838
CConfigMgr* g_configMgr = nullptr;

// OFFSET: 0x007A6FE0
// FUNCTION: fn_09042
CConfigMgr::~CConfigMgr() {
}

// OFFSET: 0x007A7070
// FUNCTION: fn_09044
bool CConfigMgr::Create() {
  if (g_configMgr) {
    return false;
  }

  g_configMgr = new (std::nothrow) CConfigMgr();
  if (!g_configMgr) {
    return false;
  }

  if (!g_configMgr->m_initialized) {
    delete g_configMgr;
    g_configMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x007A7160
// FUNCTION: fn_09045
void CConfigMgr::Update() {
}

// OFFSET: 0x007A7170
// FUNCTION: fn_09046
void CConfigMgr::Destroy() {
  if (g_configMgr) {
    delete g_configMgr;
    g_configMgr = nullptr;
  }
}
