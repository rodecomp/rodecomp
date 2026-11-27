#include "Client/Input/ImeHelper.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <windows.h>

typedef int(__cdecl* ImeHelperProc)(HWND window, const char* version);
typedef ImeHelperProc(__stdcall* QueryImeHelperProc)(DWORD command, const char* version);

// OFFSET: 0x00AFB5D0
// FUNCTION: fn_18304
void NotifyImeHelper() {
  constexpr DWORD SGP_COMMAND_UNKNOWN_0x9000F003 = 0x9000F003;
  QueryImeHelperProc query = reinterpret_cast<QueryImeHelperProc>(GetProcAddress(LoadLibraryA("IMESCHLP"), "QueryIMEHelper"));
  if (query) {
    ImeHelperProc helper = query(SGP_COMMAND_UNKNOWN_0x9000F003, "5,01,2600,1832");
    if (helper) {
      helper(hWnd, "5,01,2600,1832");
    }
  }
}

// OFFSET: 0x00AFB610
// FUNCTION: fn_18305
void HandleImeError(const int* error) {
  switch (*error) {
  case SGP_CODE_ALREADY_RUNNING:
    g_windowMgr.Confirm(LookupMessage(MSI_SGP_CODE_ALREADY_RUNNING), 0, true, false, 0, "메시지", 280, 120, 0);
    break;
  case SGP_CODE_DETECTED_MACRO:
    g_windowMgr.Confirm(LookupMessage(MSI_SGP_CODE_DETECTED_MACRO), 0, true, false, 0, "메시지", 280, 120, 0);
    break;
  case SGP_CODE_DETECTED_SPEEDHACK:
    g_windowMgr.Confirm(LookupMessage(MSI_SGP_CODE_DETECTED_SPEEDHACK), 0, true, false, 0, "메시지", 280, 120, 0);
    break;
  case SGP_CODE_DETECTED_APIHOOK:
    g_windowMgr.Confirm(LookupMessage(MSI_SGP_CODE_DETECTED_APIHOOK), 0, true, false, 0, "메시지", 280, 120, 0);
    break;
  case SGP_CODE_DETECTED_MSGHOOK:
    g_windowMgr.Confirm(LookupMessage(MSI_SGP_CODE_DETECTED_MSGHOOK), 0, true, false, 0, "메시지", 280, 120, 0);
    break;
  }

  NotifyImeHelper();
  GetRagConnection().Close();
  g_modeMgr.Quit();
}

// OFFSET: 0x00AFB780
// FUNCTION: fn_18306
void ImeHelperNoop() {
}
