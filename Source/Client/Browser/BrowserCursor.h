#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCURSOR_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERCURSOR_H

#include <windows.h>

class CMouse;
class CRenderSystemBase;

struct BrowserCursor {
  int& state;
  CMouse& mouse;
  CRenderSystemBase& renderSystem;
  HWND const& applicationWindow;

  void Set(int value);
  void Poll();
};

#endif
