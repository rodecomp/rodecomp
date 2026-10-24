#ifndef RAGNAROK_SOURCE_CLIENT_BROWSERINPUTHANDOFF_H
#define RAGNAROK_SOURCE_CLIENT_BROWSERINPUTHANDOFF_H

#include <windows.h>

class CMouse;
struct ServiceOptions;
namespace client {
  struct ApplicationActivation;
}

// Borrowed original CWebBrowser service-option latch, main HWND and global
// input owners. Restoration retains the latch; it is not a one-shot operation.
struct BrowserInputHandoff {
  unsigned char& restoreOption;
  HWND const& applicationWindow;
  ServiceOptions& service;
  CMouse& mouse;
  client::ApplicationActivation& activation;

  void Enter();
  void Restore();
};

#endif
