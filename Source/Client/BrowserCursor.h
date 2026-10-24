#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERCURSOR_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERCURSOR_H

#include <windows.h>

class CMouse;
class CRenderSystemBase;

// Borrowed CWebBrowser cursor state and the actual global mouse/render
// owners. ShowCursor receives the full integer, not a normalized boolean.
struct BrowserCursor {
  int& state;
  CMouse& mouse;
  CRenderSystemBase& renderSystem;
  HWND const& applicationWindow;

  void Set(int value);
  void Poll();
};

#endif
