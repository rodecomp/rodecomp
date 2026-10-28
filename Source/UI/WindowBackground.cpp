#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowBorder.h"

#include <cstring>
#include <windows.h>

// OFFSET: 0x008D03E0
// FUNCTION: fn_12085
void UIWindow::DrawPanelBackground(int y, int width, const char* bitmapName) {
  if (std::strlen(bitmapName) != 0) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(bitmapName)));
    if (bitmap) {
      this->DrawBitmap(0, y, &bitmap->image, false);
    }
    return;
  }

  if (width == 0) {
    width = this->geometry.width;
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  DrawRoundedBorder(*this, 0, y, width, this->geometry.height - y, RGB(red, green, blue));
  this->FillRegion(0, y, width, 5, RGB(red, green, blue));

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, y, width - 2, this->geometry.height - y - 2, RGB(red, green, blue));
  this->FillRegion(2, this->geometry.height - 2, width - 4, 1, RGB(red, green, blue));
}

// OFFSET: 0x008D0530
// FUNCTION: fn_12086
void UIWindow::DrawPanelBackgroundAt(int x, int y, int width, const char* bitmapName) {
  if (bitmapName && std::strlen(bitmapName) != 0) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(bitmapName)));
    if (bitmap) {
      this->DrawBitmap(x, y, &bitmap->image, false);
    }
    return;
  }

  if (width == 0) {
    width = this->geometry.width - x;
  }

  const int height = this->geometry.height;
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  DrawRoundedBorder(*this, x, y, width, height - y, RGB(red, green, blue));
  this->FillRegion(x, y, width, 5, RGB(red, green, blue));

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(x + 1, y, width - 2, height - y - 2, RGB(red, green, blue));
  this->FillRegion(x + 2, height - 2, width - 4, 1, RGB(red, green, blue));
}
