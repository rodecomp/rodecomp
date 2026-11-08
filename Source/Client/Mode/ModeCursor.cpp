#include "Client/Mode/CMode.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AD52D0
// FUNCTION: fn_17820
CMode::CMode() : screenshotName("") {
  this->running = 1;
  this->pendingCursorType = -1;
  this->connectionReady = 1;
  this->tooltip = nullptr;
  this->tooltipTick = timeGetTime();
  this->cursorAction = 0;
  this->cursorActionFrame = 0;
  this->cursorTick = timeGetTime();
  this->lockOnMouse = 0;
  this->screenshotRequested = 0;
  this->cursorOffsetX = 0.0f;
  this->cursorOffsetY = 0.0f;
  this->fadeInRequested = 0;
}

// OFFSET: 0x00AD75E0
// FUNCTION: fn_17850
void CMode::SetCursorAction(int action) {
  if (this->cursorAction == action) {
    return;
  }

  this->cursorAction = action;
  this->cursorActionFrame = 0;
  this->cursorTick = timeGetTime();
  g_windowMgr.SendNotice(15, this->cursorAction, 0, 0, 0, 0);
}
