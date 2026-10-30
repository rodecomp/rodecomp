#include "Client/Input/ImeHelper.h"
#include "Client/Application/ApplicationWindow.h"

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

// OFFSET: 0x00AFB780
// FUNCTION: fn_18306
void ImeHelperNoop() {
}
