#include "UI/Window/UIMiniBtnWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"

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
