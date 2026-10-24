#include "Client/ModeCursor.h"

#include <windows.h>
// clang-format off
#include <mmsystem.h>
// clang-format on

// OFFSET: 0x00AD52D0
// FUNCTION: fn_17820
ModeCursor::ModeCursor() : type(0), frame(0) {
}

// OFFSET: 0x00AD75E0
// FUNCTION: fn_17850
void ModeCursor::Set(std::int32_t value, std::uint32_t& changeTick) {
  if (this->type == value) {
    return;
  }
  this->type = value;
  this->frame = 0;
  changeTick = timeGetTime();
}
