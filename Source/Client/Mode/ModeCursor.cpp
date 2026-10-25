#include "Client/Mode/ModeCursor.h"

#include <windows.h>
#include <timeapi.h>

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
