#include "UI/Window/UILoginWnd.h"
#include "Client/Application/ApplicationWindow.h"

#include <windows.h>
#include <shellapi.h>

// GLOBAL: gl_21920
const char* g_webPageUrl = "";

// OFFSET: 0x008BB0B0
// FUNCTION: fn_11794
void UILoginWnd::OpenWebPage() {
  ShellExecuteA(hWnd, "open", g_webPageUrl, nullptr, nullptr, SW_SHOWNORMAL);
}
