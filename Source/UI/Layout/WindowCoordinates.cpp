#include "UI/Window/UIWindow.h"
#include "UI/Layout/WindowCoordinates.h"

#include <cstdint>

// OFFSET: 0x00859050
// FUNCTION: fn_45622
__forceinline bool WindowsOverlap(const UIWindow* first, const UIWindow* second) {
  if (first && second) {
    const WindowGeometry& secondGeometry = second->geometry;
    const int secondBounds[4] = {
        secondGeometry.x, secondGeometry.y, secondGeometry.width + secondGeometry.x, secondGeometry.height + secondGeometry.y};
    const WindowGeometry& firstGeometry = first->geometry;
    const int firstBounds[4] = {
        firstGeometry.x, firstGeometry.y, firstGeometry.width + firstGeometry.x, firstGeometry.height + firstGeometry.y};
    return WindowBoundsOverlap(firstBounds, secondBounds);
  }

  return false;
}

// OFFSET: 0x00869D80
// FUNCTION: fn_45623
void MoveOverlappingWindowBeside(UIWindow* window, const UIWindow* anchor, bool moveLeft) {
  if (WindowsOverlap(window, anchor)) {
    const int x = moveLeft ? anchor->geometry.x - window->geometry.width - 3
                           : anchor->geometry.x + anchor->geometry.width + 3;
    window->Move(x, window->geometry.y);
  }
}
