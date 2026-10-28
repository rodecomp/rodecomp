#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONWINDOW_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONWINDOW_H

#include <windows.h>

extern HWND hWnd;

HWND CreateApplicationWindow(
  DWORD extendedStyle, const char* className, const char* ignoredTitle, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu,
  HINSTANCE instance, void* parameter
);

BOOL ClipApplicationCursor(HWND window);

#endif
