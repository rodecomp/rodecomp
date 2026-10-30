#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "Render/Renderer.h"
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

// OFFSET: 0x00AAADF0
// FUNCTION: fn_17272
int UIWindowMgr::ScaleToViewportWidth(int value) {
  return g_renderer->viewport.width * value / 640;
}

// OFFSET: 0x00AAAE20
// FUNCTION: fn_17273
int UIWindowMgr::CenterOnViewportX(int x) {
  return (g_renderer->viewport.width - 640) / 2 + x;
}

// OFFSET: 0x00AAAE40
// FUNCTION: fn_17274
int UIWindowMgr::CenterOnViewportY(int y) {
  return (g_renderer->viewport.height - 480) / 2 + y;
}

// OFFSET: 0x00AAAE60
// FUNCTION: fn_17275
void UIWindowMgr::ConfirmActiveDialog() {
  UIWindow* window = this->unknown_01F8;
  if (window || (window = this->unknown_01F0) != nullptr || (window = this->unknown_01F4) != nullptr || (window = this->unknown_0214) != nullptr ||
      (window = this->unknown_021C) != nullptr || (window = this->unknown_020C) != nullptr || (window = this->unknown_0374) != nullptr ||
      (window = this->hoverWindow) != nullptr) {
    window->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return;
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    if ((*entry)->Slot02()) {
      (*entry)->SendMsg(nullptr, 18, 0, 0, 0, 0);
    }
  }
}
