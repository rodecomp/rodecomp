#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERFRAME_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERFRAME_H

#include "Client/Browser/BrowserCursor.h"
#include "Client/Browser/BrowserWindowSizing.h"

struct BrowserFrame {
  BrowserWindowSizing& sizing;
  BrowserCursor& cursor;
  HWND const& host;
  HWND const& browserWindow;
  const bool& hasExternalEvents;

  void Normalize();
  void Poll();
};

#endif
