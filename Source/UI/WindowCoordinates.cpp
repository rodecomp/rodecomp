#include "UI/UIWindow.h"
#include "UI/WindowCoordinates.h"

#include <bit>
#include <cstdint>

namespace {
  int Add(int first, int second) {
    return std::bit_cast<int>(static_cast<std::uint32_t>(first) + static_cast<std::uint32_t>(second));
  }
}

// OFFSET: 0x00A91B70
// FUNCTION: fn_45428
void WindowScreenPosition(const UIWindow* window, int& x, int& y) {
  int horizontal = 0;
  int vertical = 0;
  for (const UIWindow* node = window; node; node = static_cast<const UIWindow*>(node->parent)) {
    horizontal = Add(horizontal, node->geometry.x);
    vertical = Add(vertical, node->geometry.y);
  }
  x = horizontal;
  y = vertical;
}

// OFFSET: 0x00A91BA0
// FUNCTION: fn_45429
void WindowScreenBounds(const UIWindow* window, int& left, int& top, int& right, int& bottom) {
  bottom = 0;
  right = 0;
  top = 0;
  left = 0;

  int horizontal = 0;
  int vertical = 0;
  for (const UIWindow* node = window; node; node = static_cast<const UIWindow*>(node->parent)) {
    horizontal = Add(horizontal, node->geometry.x);
    vertical = Add(vertical, node->geometry.y);
  }
  left = horizontal;
  top = vertical;
  if (window) {
    right = Add(window->geometry.width, left);
    bottom = Add(window->geometry.height, top);
  }
}

// OFFSET: 0x00AF64A0
// FUNCTION: fn_18253
bool WindowBoundsOverlap(const int first[4], const int second[4]) {
  return first[2] > second[0] &&
         first[0] < second[2] &&
         first[3] > second[1] &&
         first[1] < second[3];
}
