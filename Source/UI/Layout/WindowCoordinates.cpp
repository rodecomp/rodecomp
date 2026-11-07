#include "UI/Window/UIWindow.h"
#include "UI/Layout/WindowCoordinates.h"

#include <cstdint>

// OFFSET: 0x00AF64A0
// FUNCTION: fn_18253
bool WindowBoundsOverlap(const int first[4], const int second[4]) {
  return first[2] > second[0] && first[0] < second[2] && first[3] > second[1] && first[1] < second[3];
}
