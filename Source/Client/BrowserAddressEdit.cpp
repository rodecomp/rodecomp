#include "Client/BrowserAddressEdit.h"

// OFFSET: 0x00AB21F0
// FUNCTION: fn_17412
LRESULT BrowserAddressEdit::Message(HWND window, UINT message, WPARAM value, LPARAM flags) {
  if (value == VK_RETURN) {
    if (message == WM_KEYUP || message == WM_CHAR) {
      return 0;
    }

    if (message == WM_KEYDOWN && this->activeHost) {
      SendMessageA(*this->activeHost, WM_COMMAND, 1000, 0);
      return 0;
    }
  }

  return CallWindowProcA(this->previous, window, message, value, flags);
}
