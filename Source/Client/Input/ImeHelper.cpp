#include "Client/Input/ImeHelper.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <windows.h>

typedef int(__cdecl* ImeHelperProc)(HWND window, const char* version);
typedef ImeHelperProc(__stdcall* QueryImeHelperProc)(DWORD command, const char* version);

// OFFSET: 0x00AFB5D0
// FUNCTION: fn_18304
void NotifyImeHelper() {
  QueryImeHelperProc query = reinterpret_cast<QueryImeHelperProc>(GetProcAddress(LoadLibraryA("IMESCHLP"), "QueryIMEHelper"));
  if (query) {
    ImeHelperProc helper = query(0x9000F003, "5,01,2600,1832");
    if (helper) {
      helper(hWnd, "5,01,2600,1832");
    }
  }
}

// OFFSET: 0x00AFB610
// FUNCTION: fn_18305
void HandleImeError(const int* error) {
  int message;
  switch (*error) {
  case 0x8000E001:
    message = 1046;
    break;
  case 0x8000E003:
    message = 1047;
    break;
  case 0x8000E004:
    message = 1048;
    break;
  case 0x8000E005:
    message = 1049;
    break;
  case 0x8000E006:
    message = 1050;
    break;
  default:
    goto quit;
  }
  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(message), 0, true, false, 0, "Message", 280, 120, 0);

quit:
  NotifyImeHelper();
  GetRagConnection().Close();
  g_modeMgr.Quit();
}

// OFFSET: 0x00AFB780
// FUNCTION: fn_18306
void ImeHelperNoop() {
}
