#include "UI/Text/UIStaticText_LF.h"

#include <cstdint>
#include <string>

// OFFSET: 0x0094ECF0
// FUNCTION: fn_12915
void UIStaticText_LF::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->sub_A84400(
    0, 0,
    *reinterpret_cast<const std::string*>(reinterpret_cast<const char*>(this) + 0x7C),
    this->geometry.width,
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0x94),
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0x98),
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0x9C),
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0xA0),
    *reinterpret_cast<const std::uint32_t*>(reinterpret_cast<const char*>(this) + 0xA4),
    *reinterpret_cast<const unsigned char*>(reinterpret_cast<const char*>(this) + 0xA8)
  );
}

// OFFSET: 0x0094EEA0
// FUNCTION: fn_12920
void UIStaticText_LF::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x, y);
  }
}

// OFFSET: 0x0094EFA0
// FUNCTION: fn_12925
void UIStaticText_LF::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x, y);
  }
}

// OFFSET: 0x0094F300
// FUNCTION: fn_12936
void UIStaticText_LF::Slot35(int delta) {
  if (this->parent) {
    this->parent->Slot35(delta);
  }
}
