#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowBorder.h"

#include <algorithm>
#include <cstdio>

__declspec(noinline) void DrawRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

// OFFSET: 0x00A7BD60
// FUNCTION: fn_16938
void UIWindow::FillRegion(int x, int y, int width, int height, std::uint32_t color) {
  RECT rectangle = {std::max(x, 0), std::max(y, 0), std::min((x + width), this->geometry.width), std::min((y + height), this->geometry.height)};
  this->geometry.surface->Fill(&rectangle, color);
}

// OFFSET: 0x00A7C060
// FUNCTION: fn_16942
void DrawRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color) {
  window.FillRegion((x + 3), y, (width - 6), 1, color);
  window.FillRegion((x + 3), ((y + height) - 1), (width - 6), 1, color);
  window.FillRegion(x, (y + 3), 1, (height - 6), color);
  window.FillRegion(((x + width) - 1), (y + 3), 1, (height - 6), color);

  window.FillRegion((x + 1), (y + 1), 2, 1, color);
  window.FillRegion((x + 1), ((y + height) - 2), 2, 1, color);
  window.FillRegion(((x + width) - 3), (y + 1), 2, 1, color);
  window.FillRegion(((x + width) - 3), ((y + height) - 2), 2, 1, color);

  window.FillRegion((x + 1), (y + 2), 1, 1, color);
  window.FillRegion((x + 1), ((y + height) - 3), 1, 1, color);
  window.FillRegion(((x + width) - 2), (y + 2), 1, 1, color);
  window.FillRegion(((x + width) - 2), ((y + height) - 3), 1, 1, color);
}

// OFFSET: 0x00A7BC30
// FUNCTION: fn_16937
void DrawRectangleBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color) {
  window.FillRegion(x, y, width, 1, color);
  window.FillRegion(x, y, 1, height, color);
  window.FillRegion(((x + width) - 1), y, 1, height, color);
  window.FillRegion(x, ((y + height) - 1), width, 1, color);
}

// OFFSET: 0x00A7C3E0
// FUNCTION: fn_16943
void DrawFilledRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color) {
  window.FillRegion((x + 1), (y + 1), (width - 2), (height - 2), color);
  DrawRoundedBorder(window, x, y, width, height, color);
}

// OFFSET: 0x00A7C4E0
// FUNCTION: fn_16946
void FillTriangle(UIWindow& window, int x0, int y0, int x1, int y1, int x2, int y2, std::uint32_t color) {
  HDC dc;
  POINT points[3] = {{x0, y0}, {x1, y1}, {x2, y2}};
  if (window.geometry.surface->GetDC(&dc)) {
    HRGN region = CreatePolygonRgn(points, 3, WINDING);
    HBRUSH brush = CreateSolidBrush(color);
    FillRgn(dc, region, brush);
    DeleteObject(region);
    DeleteObject(brush);
    window.geometry.surface->ReleaseDC(dc);
  }
}

// OFFSET: 0x008D0920
// FUNCTION: fn_12088
int UIWindow::DrawButtonBar(int style, int width, int rows) {
  if (width == 0) {
    width = this->geometry.width;
  }

  int leftWidth = 0;
  int rightWidth = 0;
  int barHeight = 0;
  char buffer[260];
  const char* name;

  if (style) {
    std::sprintf(buffer, "유저인터페이스\\basic_interface\\btnbar_left%d.bmp", style);
    name = buffer;
  } else {
    name = "유저인터페이스\\basic_interface\\btnbar_left.bmp";
  }

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (left) {
    leftWidth = left->image.width;
    barHeight = left->image.height;

    for (int row = 1; row <= rows; ++row) {
      this->DrawBitmap(0, this->geometry.height - row * barHeight, left, false);
    }
  }

  if (style) {
    std::sprintf(buffer, "유저인터페이스\\basic_interface\\btnbar_right%d.bmp", style);
    name = buffer;
  } else {
    name = "유저인터페이스\\basic_interface\\btnbar_right.bmp";
  }

  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (right) {
    rightWidth = right->image.width;

    for (int row = 1; row <= rows; ++row) {
      this->DrawBitmap(width - rightWidth, this->geometry.height - row * barHeight, right, false);
    }
  }

  if (style) {
    std::sprintf(buffer, "유저인터페이스\\basic_interface\\btnbar_mid%d.bmp", style);
    name = buffer;
  } else {
    name = "유저인터페이스\\basic_interface\\btnbar_mid.bmp";
  }

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (middle) {
    width = width - rightWidth - leftWidth;
    const int middleWidth = middle->image.width;
    const int count = width / middleWidth + (width % middleWidth != 0);

    for (int row = 1; row <= rows; ++row) {
      for (int column = 0; column < count; ++column) {
        this->DrawBitmap(leftWidth + column * middleWidth, this->geometry.height - row * barHeight, middle, false);
      }
    }

    return middle->image.height;
  }

  return 0;
}
