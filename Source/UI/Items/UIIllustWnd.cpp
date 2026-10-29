#include "UI/Items/UIIllustWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0096CBD0
// FUNCTION: fn_13181
void UIIllustWnd::Slot15(int, int) {}

// OFFSET: 0x0096CBE0
// FUNCTION: fn_13182
void UIMultiIllustWnd::Slot15(int, int) {}

// OFFSET: 0x0096CBF0
// FUNCTION: fn_13183
void UIIllustWnd::Slot18() {
  if (this->m_bitmapName.size() != 0) {
    CRes* bitmap = ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str()));
    ResourceMgr::Instance().Remove(bitmap);
  }
}

// OFFSET: 0x0096CC40
// FUNCTION: fn_13184
void UIMultiIllustWnd::Slot18() {
  if (this->m_bitmapName.size() != 0) {
    CRes* bitmap = ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str()));
    ResourceMgr::Instance().Remove(bitmap);
  }
}

// OFFSET: 0x0096CC90
// FUNCTION: fn_13185
void UIIllustWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  int top = 0;
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str())));
  if (this->m_titled != 0) {
    top = this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, false, "", 0);
  }
  this->DrawBitmap(0, top, &bitmap->image, true);
}

// OFFSET: 0x0096CD00
// FUNCTION: fn_13186
void UIMultiIllustWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  int top = 0;
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str())));
  if (this->m_titled != 0) {
    top = this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, false, "", 0);
  }
  this->DrawBitmap(0, top, &bitmap->image, true);
}

// OFFSET: 0x0096CD70
// FUNCTION: fn_13187
void UIMultiIllustWnd::Slot25(int, int) {}

// OFFSET: 0x0096CD80
// FUNCTION: fn_13188
void UIIllustWnd::Slot27(int x, int y) {
  if (this->m_closable != 0) {
    UIFrameWnd::Slot27(x, y);
  }
}

// OFFSET: 0x0096CDA0
// FUNCTION: fn_13189
void UIMultiIllustWnd::Slot27(int x, int y) {
  if (this->m_closable != 0) {
    UIFrameWnd::Slot27(x, y);
  }
}

// OFFSET: 0x0096CDC0
// FUNCTION: fn_13190
int UIIllustWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 201) {
      g_modeMgr.GetGameMode()->SendMsg(100, reinterpret_cast<int>(""), -1, 1, 0);
      g_windowMgr.ReleaseCapture();
    }
    return 0;
  }
  if (message == 34) {
    if (argument1 == -1) {
      this->m_titled = 0;
      this->m_closable = 0;
      this->DeleteChildren();
      return 0;
    }
    if (argument1 == -2) {
      this->m_titled = 1;
      return 0;
    }
    this->AddCloseButton(*this->m_controls, "", 0)->Move(this->geometry.width - 16, 3);
    this->m_closable = 1;
    this->Invalidate();
    return 0;
  }
  if (message != 78) {
    if (this->m_closable == 0) {
      return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  const char* name = reinterpret_cast<const char*>(argument1);
  this->m_bitmapName = "유저인터페이스\\Illust\\";
  this->m_bitmapName += name;
  CBitmapRes* bitmap;
  if (!ResourceMgr::Instance().Exists(this->m_bitmapName.c_str()) ||
      (bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str())))) == nullptr) {
    this->m_bitmapName = "유저인터페이스\\cardbmp\\";
    this->m_bitmapName += name;
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str())));
    if (bitmap == nullptr) {
      this->Invalidate();
      return 0;
    }
  }
  if (this->geometry.height != bitmap->image.height || this->geometry.width != bitmap->image.width) {
    if (this->m_titled != 0) {
      CBitmapRes* titlebar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_left.bmp")));
      this->Resize(bitmap->image.width, titlebar->image.height + bitmap->image.height);
      this->Invalidate();
      return 0;
    }
    this->Resize(bitmap->image.width, bitmap->image.height);
  }
  this->Invalidate();
  return 0;
}

// OFFSET: 0x0096D010
// FUNCTION: fn_13191
int UIMultiIllustWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 201) {
      g_modeMgr.GetGameMode()->SendMsg(100, reinterpret_cast<int>(""), -1, 0, 0);
      g_windowMgr.ReleaseCapture();
    }
    return 0;
  }
  if (message == 34) {
    if (argument1 == 10) {
      this->m_closable = 0;
    } else if (argument1 == 12) {
      this->m_titled = 0;
      this->geometry.currentAlpha = 128;
      this->m_state = -1;
      this->m_closable = 0;
    } else if (argument1 == -1) {
      this->m_titled = 0;
      this->m_closable = 0;
      this->DeleteChildren();
      return 0;
    } else if (argument1 == -2) {
      this->m_titled = 1;
      return 0;
    } else if (argument1 == 5) {
      this->m_state = 5;
      this->m_titled = 0;
      this->m_closable = 0;
      return 0;
    }
    this->AddCloseButton(*this->m_controls, "", 0)->Move(this->geometry.width - 16, 3);
    this->m_closable = 1;
    this->Invalidate();
    return 0;
  }
  if (message != 78) {
    if (this->m_closable == 0) {
      return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  const char* name = reinterpret_cast<const char*>(argument1);
  this->m_bitmapName = "유저인터페이스\\Illust\\";
  this->m_bitmapName += name;
  CBitmapRes* bitmap;
  if (!ResourceMgr::Instance().Exists(this->m_bitmapName.c_str()) ||
      (bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str())))) == nullptr) {
    this->m_bitmapName = "유저인터페이스\\cardbmp\\";
    this->m_bitmapName += name;
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str())));
    if (bitmap == nullptr) {
      this->Invalidate();
      return 0;
    }
  }
  if (this->geometry.height != bitmap->image.height || this->geometry.width != bitmap->image.width) {
    if (this->m_titled != 0) {
      CBitmapRes* titlebar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_left.bmp")));
      this->Resize(bitmap->image.width, titlebar->image.height + bitmap->image.height);
      this->Invalidate();
      return 0;
    }
    this->Resize(bitmap->image.width, bitmap->image.height);
  }
  this->Invalidate();
  return 0;
}

// OFFSET: 0x0096D2C0
// FUNCTION: fn_13192
bool UIIllustWnd::CanHitTest() {
  return this->m_titled != 0 || this->m_closable != 0;
}

// OFFSET: 0x0096D2E0
// FUNCTION: fn_13193
bool UIMultiIllustWnd::CanHitTest() {
  return this->m_titled != 0 || this->m_closable != 0;
}
