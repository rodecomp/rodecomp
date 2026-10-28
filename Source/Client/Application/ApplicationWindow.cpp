#include "Client/Application/ApplicationWindow.h"

// GLOBAL: gl_24399
HWND hWnd;

// OFFSET: 0x03DE9F37
// FUNCTION: fn_45411
HWND CreateApplicationWindow(
  DWORD extendedStyle, const char* className, const char*, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, HINSTANCE instance,
  void* parameter
) {
  return CreateWindowExA(extendedStyle, className, "Ragnarok 2026", style | WS_SYSMENU, x, y, width, height, parent, menu, instance, parameter);
}

// OFFSET: 0x00E57DD0
// FUNCTION: fn_24360
BOOL ClipApplicationCursor(HWND window) {
  RECT rectangle;
  GetClientRect(window, &rectangle);
  POINT first{rectangle.left, rectangle.top};
  POINT last{rectangle.right, rectangle.bottom};
  ClientToScreen(window, &first);
  ClientToScreen(window, &last);

  RECT clip{first.x, first.y, last.x, last.y};
  return ClipCursor(&clip);
}
