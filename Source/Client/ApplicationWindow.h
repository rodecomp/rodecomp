#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATIONWINDOW_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATIONWINDOW_H

#include <windows.h>

namespace client {
  // Recovered CreateWindowExA wrapper, not the full application initializer.
  // Forces the title "Ragnarok 2026" and adds WS_SYSMENU to the caller's style.
  HWND CreateApplicationWindow(
    DWORD extendedStyle,
    const char* className,
    const char* ignoredTitle,
    DWORD style,
    int x,
    int y,
    int width,
    int height,
    HWND parent,
    HMENU menu,
    HINSTANCE instance,
    void* parameter
  );

  // Original global main-window handle is borrowed; clips its client rectangle
  // after converting both corners to screen coordinates. Returns ClipCursor.
  BOOL ClipApplicationCursor(HWND window);
}

#endif
