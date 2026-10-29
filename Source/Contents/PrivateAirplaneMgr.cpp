#include "Contents/PrivateAirplaneMgr.h"

extern "C" {
#include <lua.h>
}

// GLOBAL: gl_24636
CPrivateAirplaneMgr* g_privateAirplaneItems;

// OFFSET: 0x006D0620
// FUNCTION: fn_06505
void CPrivateAirplaneMgr::Destroy() {
  if (g_privateAirplaneItems != nullptr) {
    delete g_privateAirplaneItems;
    g_privateAirplaneItems = nullptr;
  }
}

// OFFSET: 0x006D0640

// OFFSET: 0x006D0860

// OFFSET: 0x006D0950

// OFFSET: 0x006D0EF0

// OFFSET: 0x006D10E0

// OFFSET: 0x006D1260
