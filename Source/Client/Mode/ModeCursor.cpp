#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeCursor.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AD52D0
// FUNCTION: fn_17820
CMode::CMode() {
  this->screenshotName = "";
  this->running = 1;
  this->pendingCursorType = -1;
  this->unknown_0018 = 1;
  this->tooltip = nullptr;
  this->tooltipTick = timeGetTime();
  this->cursorAction = 0;
  this->cursorActionFrame = 0;
  this->cursorTick = timeGetTime();
  this->unknown_0028 = 0;
  this->screenshotRequested = 0;
  this->cursorOffsetX = 0.0f;
  this->cursorOffsetY = 0.0f;
  this->unknown_0010 = 0;
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
