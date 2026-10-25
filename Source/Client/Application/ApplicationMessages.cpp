#include "Client/Application/ApplicationMessages.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Input/TraditionalKeyInput.h"
#include "Contents/Configuration/ServiceOptions.h"

#include <cstdint>
#include <imm.h>

// OFFSET: 0x00E57380
// FUNCTION: fn_24357
void ApplicationMessages::Pump() {
  MSG message;

  while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
    if (message.message == WM_KEYDOWN && message.wParam == VK_PROCESSKEY) {
      const UINT key = ImmGetVirtualKey(message.hwnd);
      if (this->traditional) {
        this->traditional->Key(static_cast<int>(key));
      }
    }

    TranslateMessage(&message);
    DispatchMessageA(&message);
  }

  if (this->dragging == 1) {
    POINT point;
    GetCursorPos(&point);
    const int deltaX = point.x - this->dragStart.x;
    const int deltaY = point.y - this->dragStart.y;

    if (deltaX != 0 || deltaY != 0) {
      MoveWindow(
        this->window, this->dragRectangle.left + deltaX, this->dragRectangle.top + deltaY, this->dragRectangle.right - this->dragRectangle.left,
        this->dragRectangle.bottom - this->dragRectangle.top, TRUE
      );
    }

    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0) {
      this->dragging = 0;
      if (this->service.Get(ServiceOption::ConfineCursor) != 0) {
        ClipApplicationCursor(this->window);
      }
    }
  }
}
