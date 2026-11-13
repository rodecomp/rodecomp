#include "Client/Application/ApplicationMessages.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceOptions.h"

#include <cstdint>
#include <imm.h>

// OFFSET: 0x00E57380
// FUNCTION: fn_24357
void PumpApplicationMessages() {
  MSG message;

  while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
    if (message.message == WM_KEYDOWN && message.wParam == VK_PROCESSKEY) {
      g_serverLanguage->ProcessKey(ImmGetVirtualKey(message.hwnd));
    }

    TranslateMessage(&message);
    DispatchMessageA(&message);
  }

  if (g_windowDragging == true) {
    POINT point;
    GetCursorPos(&point);
    const int deltaX = point.x - g_windowDragStart.x;
    const int deltaY = point.y - g_windowDragStart.y;

    if (deltaX != 0 || deltaY != 0) {
      MoveWindow(
        hWnd, g_windowDragRect.left + deltaX, g_windowDragRect.top + deltaY, g_windowDragRect.right - g_windowDragRect.left,
        g_windowDragRect.bottom - g_windowDragRect.top, TRUE
      );
    }

    if (0 == (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
      g_windowDragging = false;
      if (g_serviceOptions.Get(ServiceOption::ConfineCursor) != 0) {
        (reinterpret_cast<BOOL (*)()>(ClipApplicationCursor))();
      }
    }
  }
}
