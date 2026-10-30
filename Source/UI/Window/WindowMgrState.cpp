#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"

#include <cstdint>
#include <new>

// OFFSET: 0x00A911E0
// FUNCTION: fn_17156
void UIWindowMgr::NotifyHoverWindow() {
  if (this->hoverWindow) {
    this->hoverWindow->Slot24(0);
  }
}

// OFFSET: 0x00A91400
// FUNCTION: fn_17159
UIWindow* UIWindowMgr::GetGuildWindow() {
  if (this->window_0320) {
    return this->window_0320;
  }
  if (this->window_0324) {
    return this->window_0324;
  }
  if (this->unknown_0328) {
    return this->unknown_0328;
  }
  if (this->window_032C) {
    return this->window_032C;
  }
  if (this->unknown_0330) {
    return this->unknown_0330;
  }
  if (this->unknown_0334) {
    return this->unknown_0334;
  }
  return this->unknown_0338;
}

// OFFSET: 0x00A91670
// FUNCTION: fn_17162
int UIWindowMgr::FindMiniPartyCell() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  return g_miniPartyWndAreaMgr->FindFreeCell();
}

// OFFSET: 0x00A916F0
// FUNCTION: fn_17163
UIWindow* UIWindowMgr::GetHoverWindow() {
  return this->hoverWindow;
}

// OFFSET: 0x00A91C10
// FUNCTION: fn_17169
int UIWindowMgr::GetRenderedWindowIds(int* ids) {
  if (ids == nullptr) {
    return 0;
  }

  for (UIWindow* window : this->rendered) {
    *ids++ = window->geometry.id;
  }

  return static_cast<int>(this->rendered.size());
}

// OFFSET: 0x00A91C50
// FUNCTION: fn_17170
void UIWindowMgr::HideWindow01C8() {
  if (this->window_01C8) {
    this->window_01C8->Show(0);
  }
}

// OFFSET: 0x00A91D90
// FUNCTION: fn_17172
bool UIWindowMgr::HasUnknown030C() {
  const std::uint32_t* unknown = reinterpret_cast<const std::uint32_t*>(this->unknown_030C);
  return unknown != nullptr && unknown[1] != 0;
}
