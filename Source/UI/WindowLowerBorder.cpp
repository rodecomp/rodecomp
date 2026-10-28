#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
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

  const int height = this->geometry.height;
  int leftWidth = 0;
  int rightWidth = 0;
  int barHeight = 0;

  CBitmapRes* left =
    this->LoadStyledBitmap(style, "유저인터페이스\\basic_interface\\btnbar_left%d.bmp", "유저인터페이스\\basic_interface\\btnbar_left.bmp");
  if (left) {
    leftWidth = left->image.width;
    barHeight = left->image.height;

    for (int row = 1; row <= rows; ++row) {
      this->DrawBitmap(x, height - row * barHeight, &left->image, false);
    }
  }

  CBitmapRes* right =
    this->LoadStyledBitmap(style, "유저인터페이스\\basic_interface\\btnbar_right%d.bmp", "유저인터페이스\\basic_interface\\btnbar_right.bmp");
  if (right) {
    rightWidth = right->image.width;

    for (int row = 1; row <= rows; ++row) {
      this->DrawBitmap(x - rightWidth + width, height - row * barHeight, &right->image, false);
    }
  }

  CBitmapRes* middle =
    this->LoadStyledBitmap(style, "유저인터페이스\\basic_interface\\btnbar_mid%d.bmp", "유저인터페이스\\basic_interface\\btnbar_mid.bmp");
  if (!middle) {
    return 0;
  }

  const int middleWidth = middle->image.width;
  const int extent = width - rightWidth - leftWidth;
  const int count = extent / middleWidth + (extent % middleWidth != 0);
  const int startX = leftWidth + x;

  for (int row = 1; row <= rows; ++row) {
    int columnX = startX;
    for (int column = 0; column < count; ++column) {
      this->DrawBitmap(columnX, height - row * barHeight, &middle->image, false);
      columnX += middleWidth;
    }
  }

  return middle->image.height;
}

// OFFSET: 0x008D7570
// FUNCTION: fn_12120
CBitmapRes* UIWindow::LoadStyledBitmap(int style, const char* format, const char* name) {
  if (style > 0) {
    if (format && std::strlen(format) != 0) {
      std::string filename = FormatOwnedString(format, style);
      return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(filename.c_str())));
    }
  } else if (name && std::strlen(name) != 0) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  }
  return nullptr;
}
