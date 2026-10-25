#ifndef RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINPUTHANDOFF_H
#define RAGNAROK_SOURCE_CLIENT_BROWSER_BROWSERINPUTHANDOFF_H

#include <windows.h>

class CMouse;
struct ServiceOptions;
struct ApplicationActivation;

struct BrowserInputHandoff {
  unsigned char& restoreOption;
  HWND const& applicationWindow;
  ServiceOptions& service;
  CMouse& mouse;
  ApplicationActivation& activation;

  void Enter();
  void Restore();
};

#endif
