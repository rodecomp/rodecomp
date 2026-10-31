#include "Client/Browser/BrowserAddressEdit.h"
#include "Client/Browser/BrowserMessages.h"

#include <oleauto.h>

// OFFSET: 0x00AB20F0
// FUNCTION: fn_17411
void __fastcall sub_AB20F0(BSTR* address) {
  SysFreeString(*address);
}

// OFFSET: 0x00AB21F0
// FUNCTION: fn_17412
LRESULT BrowserAddressEdit::Message(HWND window, UINT message, WPARAM value, LPARAM flags) {
  if (value == VK_RETURN) {
    if (message == WM_KEYUP || message == WM_CHAR) {
      return 0;
    }

    if (message == WM_KEYDOWN && this->activeHost) {
      SendMessageA(*this->activeHost, WM_COMMAND, static_cast<WPARAM>(BrowserCommand::Navigate), 0);
      return 0;
    }
  }

  return CallWindowProcA(this->previous, window, message, value, flags);
}
