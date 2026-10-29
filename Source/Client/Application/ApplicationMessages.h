#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONMESSAGES_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_APPLICATIONMESSAGES_H

#include <windows.h>

struct CLanguageTC;
struct ServiceOptions;

struct ApplicationMessages {
  HWND& window;
  unsigned char& dragging;
  POINT& dragStart;
  RECT& dragRectangle;
  CLanguageTC* const& traditional;
  ServiceOptions& service;

  void Pump();
};

#endif
