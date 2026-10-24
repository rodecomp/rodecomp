#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATIONMESSAGES_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATIONMESSAGES_H

#include <windows.h>

struct TraditionalKeyInput;
struct ServiceOptions;

namespace client {
  // Borrowed application globals. A non-null traditional input view denotes
  // CLanguageTC; base/TH/ARB virtual-key delivery is an established empty entry.
  struct ApplicationMessages {
    HWND& window;
    unsigned char& dragging;
    POINT& dragStart;
    RECT& dragRectangle;
    TraditionalKeyInput* const& traditional;
    ServiceOptions& service;

    // Drains all messages (including WM_QUIT) through Translate/Dispatch, then
    // updates native window dragging. Does not invent the main window procedure.
    void Pump();
  };
}

#endif
