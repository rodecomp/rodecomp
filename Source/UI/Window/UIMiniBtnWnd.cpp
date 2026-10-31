#include "UI/Window/UIMiniBtnWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x0092CE50
// FUNCTION: fn_12666
void UIMiniBtnWnd::Paint() {
  if (this->geometry.id < 0) {
    return;
  }

  UIWindow* window = g_windowMgr.LookupWindow(this->m_windowId);
  if (window && window->geometry.visible != 0) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_activeImage.c_str())));
    this->DrawBitmap(0, 0, &bitmap->image, false);
  } else {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_inactiveImage.c_str())));
    this->DrawBitmap(0, 0, &bitmap->image, false);
  }
}

// OFFSET: 0x00930CD0
// FUNCTION: fn_12694
void UIMiniBtnWnd::Slot25(int, int) {
  bool visible;
  UIWindow* window = g_windowMgr.LookupWindow(this->m_windowId);
  if (window) {
    visible = window->geometry.visible == 0;
  } else {
    window = this->m_frameContext->windows.EnsureWindow(this->m_windowId);
    visible = true;
  }

  if (window) {
    window->Show(visible);
    window->SendMsg(this, 6, 271, visible, 0, 0);
  }
  this->Invalidate();
}

// OFFSET: 0x00941120
// FUNCTION: fn_12789
void UIMiniBtnWnd::Toggle() {
  bool visible;
  UIWindow* window = g_windowMgr.LookupWindow(this->m_windowId);
  if (window) {
    visible = window->geometry.visible == 0;
  } else {
    window = this->m_frameContext->windows.EnsureWindow(this->m_windowId);
    visible = true;
  }

  if (window) {
    window->Show(visible);
    window->SendMsg(this, 6, 271, visible, 0, 0);
  }
  this->Invalidate();
}

// OFFSET: 0x009114F0
// FUNCTION: fn_12516
UIMiniBtnWnd::UIMiniBtnWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0), m_windowId(-1) {
}
