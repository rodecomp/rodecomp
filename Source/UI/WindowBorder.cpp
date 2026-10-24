#include "Render/DeviceContext.h"
#include "UI/UIWindow.h"
#include "UI/WindowBorder.h"

#include <algorithm>
#include <bit>

namespace {

  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }

}

// OFFSET: 0x00A7C060
// FUNCTION: fn_16942
void DrawRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color) {
  window.FillRegion(Add(x, 3), y, Add(width, -6), 1, color);
  window.FillRegion(Add(x, 3), Add(Add(y, height), -1), Add(width, -6), 1, color);
  window.FillRegion(x, Add(y, 3), 1, Add(height, -6), color);
  window.FillRegion(Add(Add(x, width), -1), Add(y, 3), 1, Add(height, -6), color);

  window.FillRegion(Add(x, 1), Add(y, 1), 2, 1, color);
  window.FillRegion(Add(x, 1), Add(Add(y, height), -2), 2, 1, color);
  window.FillRegion(Add(Add(x, width), -3), Add(y, 1), 2, 1, color);
  window.FillRegion(Add(Add(x, width), -3), Add(Add(y, height), -2), 2, 1, color);

  window.FillRegion(Add(x, 1), Add(y, 2), 1, 1, color);
  window.FillRegion(Add(x, 1), Add(Add(y, height), -3), 1, 1, color);
  window.FillRegion(Add(Add(x, width), -2), Add(y, 2), 1, 1, color);
  window.FillRegion(Add(Add(x, width), -2), Add(Add(y, height), -3), 1, 1, color);
}

// OFFSET: 0x00A7BC30
// FUNCTION: fn_16937
void DrawRectangleBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color) {
  const int left = std::max(x, 0);
  const int top = std::max(y, 0);
  const int right = Add(x, width);
  const int bottom = Add(y, height);
  RECT rectangle{left, top, std::min(window.geometry.width, right), std::min(window.geometry.height, Add(y, 1))};
  window.geometry.surface->Fill(&rectangle, color);

  rectangle = {left, top, std::min(window.geometry.width, Add(x, 1)), std::min(window.geometry.height, bottom)};
  window.geometry.surface->Fill(&rectangle, color);

  rectangle = {
    std::max(Add(right, -1), 0), top, std::min(window.geometry.width, right), std::min(window.geometry.height, bottom)
  };
  window.geometry.surface->Fill(&rectangle, color);

  rectangle = {
    left, std::max(Add(bottom, -1), 0), std::min(window.geometry.width, right), std::min(window.geometry.height, bottom)
  };
  window.geometry.surface->Fill(&rectangle, color);
}
