#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Window/UIWindow.h"
#include "Utility/Text/FormatString.h"

#include <cstring>
#include <string>

// OFFSET: 0x008D0BE0
// FUNCTION: fn_12089
int UIWindow::DrawButtonBarAt(int style, int x, int width, int rows) {
  if (width == 0) {
    width = this->geometry.width - x;
  }

  int leftWidth = 0;
  int rightWidth = 0;
  int barHeight = 0;
  const int height = this->geometry.height;

  CBitmapRes* left =
    this->LoadStyledBitmap(style, "유저인터페이스\\basic_interface\\btnbar_left%d.bmp", "유저인터페이스\\basic_interface\\btnbar_left.bmp");
  if (left) {
    leftWidth = left->image.width;
    barHeight = left->image.height;

    for (int row = 1; row <= rows; ++row) {
      this->DrawBitmap(x, height - row * barHeight, left, false);
    }
  }

  CBitmapRes* right =
    this->LoadStyledBitmap(style, "유저인터페이스\\basic_interface\\btnbar_right%d.bmp", "유저인터페이스\\basic_interface\\btnbar_right.bmp");
  if (right) {
    rightWidth = right->image.width;

    for (int row = 1; row <= rows; ++row) {
      this->DrawBitmap(x - rightWidth + width, height - row * barHeight, right, false);
    }
  }

  CBitmapRes* middle =
    this->LoadStyledBitmap(style, "유저인터페이스\\basic_interface\\btnbar_mid%d.bmp", "유저인터페이스\\basic_interface\\btnbar_mid.bmp");
  if (middle) {
    const auto remaining = [&]() { return width - rightWidth - leftWidth; };
    width = remaining();
    const int middleWidth = middle->image.width;
    const int count = (width % middleWidth != 0) + width / middle->image.width;
    const int startX = leftWidth + x;

    for (int row = 1; row <= rows; ++row) {
      for (int column = 0; column < count; ++column) {
        this->DrawBitmap(startX + column * middleWidth, height - row * barHeight, middle, false);
      }
    }

    return middle->image.height;
  }

  return 0;
}

// OFFSET: 0x008D7570
// FUNCTION: fn_12120
CBitmapRes* UIWindow::LoadStyledBitmap(int style, const char* format, const char* name) {
  CBitmapRes* bitmap = nullptr;
  if (style > 0) {
    if (format && std::strlen(format) != 0) {
      std::string filename = FormatOwnedString(format, style);
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(filename.c_str())));
    }
  } else if (name && std::strlen(name) != 0) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  }
  return bitmap;
}
