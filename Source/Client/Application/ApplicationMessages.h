#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONMESSAGES_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONMESSAGES_H

#include <windows.h>

struct TraditionalKeyInput;
struct ServiceOptions;

struct ApplicationMessages {
  HWND& window;
  unsigned char& dragging;
  POINT& dragStart;
  RECT& dragRectangle;
  TraditionalKeyInput* const& traditional;
  ServiceOptions& service;

  void Pump();
};

#endif
