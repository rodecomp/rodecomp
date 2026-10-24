#include "UI/WindowGeometry.h"

#include <windows.h>
// clang-format off
#include <mmsystem.h>
// clang-format on

// OFFSET: 0x00A79A90
WindowGeometry::WindowGeometry()
    : width(0), height(0), x(0), y(0), surface(nullptr), visible(1), id(600), unknown_0030(0), unknown_0034(0),
      targetAlpha(255), alphaStart(timeGetTime()), currentAlpha(255) {
}

// OFFSET: 0x00A81D40
// FUNCTION: fn_16987
void WindowGeometry::Move(int newX, int newY) {
  this->x = newX;
  this->y = newY;
}

// OFFSET: 0x005C9630
// FUNCTION: fn_03632
void WindowGeometry::Show(int state) {
  this->visible = state;
}

// OFFSET: 0x005C8F60
// FUNCTION: fn_03629
void WindowGeometry::SetId(int value) {
  this->id = value;
}

// OFFSET: 0x00A7D6F0
// FUNCTION: fn_16954
std::uint32_t WindowGeometry::Color() {
  std::uint32_t value = this->currentAlpha;
  if (value != this->targetAlpha) {
    const std::uint32_t step = (timeGetTime() - this->alphaStart) >> 4;
    const std::uint32_t candidate = value < this->targetAlpha ? value + step : value - step;
    const bool keepCandidate =
      value < this->targetAlpha ? candidate < this->targetAlpha : this->targetAlpha < candidate;
    value = keepCandidate ? candidate : this->targetAlpha;
    this->currentAlpha = value;
  }
  if (value > 255) {
    value = this->targetAlpha;
    this->currentAlpha = value;
  }
  // Adds 0xFFFFFF, rather than OR: retain wrap at unusual values.
  return (value << 24) + 0xFFFFFF;
}
