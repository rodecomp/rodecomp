#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x008D2CE0
// FUNCTION: fn_12098
int UIWindow::DrawTitleBar(TextureNames& names, FontCache& fonts, const TextLocale& locale, bool closeButton, const char* title, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  int left = 0;
  int titleHeight = 0;
  auto* leftBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\titlebar_left.bmp")));
  if (leftBar) {
    this->DrawBitmap(0, 0, &leftBar->image, false);
    left = leftBar->image.width;
  }

  int right = 0;
  auto* rightBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\titlebar_right.bmp")));
  if (rightBar) {
    right = rightBar->image.width;
    this->DrawBitmap(width - right, 0, &rightBar->image, false);
  }

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\titlebar_mid.bmp")));
  if (middle) {
    titleHeight = middle->image.height;
    const int extent = width - right - left;
    const int count = extent / middle->image.width + (extent % middle->image.width != 0);

    for (int index = 0; index < count; ++index) {
      this->DrawBitmap(left, 0, &middle->image, false);
      left += middle->image.width;
    }
  }

  this->DrawTextAt(fonts, locale, 19, titleHeight - 13, title, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(fonts, locale, 18, titleHeight - 14, title, 0, 0, 12, 0, 0, 0);

  if (closeButton) {
    auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\sys_base_off.bmp")));
    this->DrawBitmap(3, 3, icon ? &icon->image : nullptr, true);
  }

  return titleHeight;
}

// OFFSET: 0x008D2E50
// FUNCTION: fn_12099
int UIWindow::DrawTitleBarAt(TextureNames& names, FontCache& fonts, const TextLocale& locale, int x, int width, bool closeButton, const char* title) {
  if (width <= 0) {
    width = this->geometry.width - x;
  }

  int left = 0;
  int right = 0;
  int titleHeight = 0;
  auto* leftBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\titlebar_left.bmp")));
  if (leftBar) {
    this->DrawBitmap(x, 0, &leftBar->image, false);
    left = leftBar->image.width;
  }

  auto* rightBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\titlebar_right.bmp")));
  if (rightBar) {
    right = rightBar->image.width;
    this->DrawBitmap(x - right + width, 0, &rightBar->image, false);
  }

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\titlebar_mid.bmp")));
  if (middle) {
    titleHeight = middle->image.height;
    const int extent = width - right - left;
    const int count = extent / middle->image.width + (extent % middle->image.width != 0);

    for (int index = 0; index < count; ++index) {
      this->DrawBitmap(x + left + index * middle->image.width, 0, &middle->image, false);
    }
  }

  this->DrawTextAt(fonts, locale, x + 19, titleHeight - 13, title, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(fonts, locale, x + 18, titleHeight - 14, title, 0, 0, 12, 0, 0, 0);

  if (closeButton) {
    auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\sys_base_off.bmp")));
    if (icon) {
      this->DrawBitmap(x + 3, 3, &icon->image, true);
    }
  }

  return titleHeight;
}
