#include "UI/Window/UIWindow.h"
#include "UI/Layout/WindowCoordinates.h"

#include <cstdint>

// OFFSET: 0x00AF64A0
// FUNCTION: fn_18253
bool WindowBoundsOverlap(const int first[4], const int second[4]) {
  return first[2] > second[0] && first[0] < second[2] && first[3] > second[1] && first[1] < second[3];
}

// OFFSET: 0x00859050
// FUNCTION: fn_45622
bool WindowsOverlap(const UIWindow* first, const UIWindow* second) {
  if (!first || !second) {
    return false;
  }
  const WindowGeometry& firstGeometry = first->geometry;
  const WindowGeometry& secondGeometry = second->geometry;
  const int firstBounds[4] = {
      firstGeometry.x, firstGeometry.y, firstGeometry.x + firstGeometry.width, firstGeometry.y + firstGeometry.height};
  const int secondBounds[4] = {
      secondGeometry.x, secondGeometry.y, secondGeometry.x + secondGeometry.width, secondGeometry.y + secondGeometry.height};
  return WindowBoundsOverlap(firstBounds, secondBounds);
}

// OFFSET: 0x00869D80
