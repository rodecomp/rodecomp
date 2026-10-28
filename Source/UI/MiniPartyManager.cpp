#include "UI/MiniPartyGrid.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindowMgr.h"

#include <new>

// OFFSET: 0x007435C0
// FUNCTION: fn_07745
CMiniPartyWndAreaMgr* CMiniPartyWndAreaMgr::Instance() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  return g_miniPartyWndAreaMgr;
}

// OFFSET: 0x00AA9ED0
// FUNCTION: fn_17263
void UIWindowMgr::SetScreenSize(int width, int height) {
  this->unknown_047C = width;
  this->unknown_0480 = height;
  this->unknown_0030 = 0;
  this->unknown_0034 = height - this->unknown_0038;
  reinterpret_cast<MiniPartyGrid*>(CMiniPartyWndAreaMgr::Instance())->BuildAvailableCells();
}
