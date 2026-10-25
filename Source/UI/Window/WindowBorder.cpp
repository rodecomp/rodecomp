#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowBorder.h"

#include <algorithm>

namespace {}

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
  const int left = std::max(x, 0);
  const int top = std::max(y, 0);
  const int right = (x + width);
  const int bottom = (y + height);
  RECT rectangle{left, top, std::min(window.geometry.width, right), std::min(window.geometry.height, (y + 1))};
  window.geometry.surface->Fill(&rectangle, color);

  rectangle = {left, top, std::min(window.geometry.width, (x + 1)), std::min(window.geometry.height, bottom)};
  window.geometry.surface->Fill(&rectangle, color);

  rectangle = {std::max((right - 1), 0), top, std::min(window.geometry.width, right), std::min(window.geometry.height, bottom)};
  window.geometry.surface->Fill(&rectangle, color);

  rectangle = {left, std::max((bottom - 1), 0), std::min(window.geometry.width, right), std::min(window.geometry.height, bottom)};
  window.geometry.surface->Fill(&rectangle, color);
}
