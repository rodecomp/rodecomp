#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERFRAME_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERFRAME_H

#include "Client/BrowserCursor.h"
#include "Client/BrowserWindowSizing.h"

// Borrowed original browser owner. Presence means the separate external event
// receiver is attached, not merely that an OLE client site exists.
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
