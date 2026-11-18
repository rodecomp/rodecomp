#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONWINDOW_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONWINDOW_H

#include <windows.h>

extern HWND hWnd;

BOOL ClipApplicationCursor();
BOOL ReleaseApplicationCursor();
LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

#endif
