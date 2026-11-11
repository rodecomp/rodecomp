#ifndef RAGNAROK_SOURCE_UI_EVENTS_UIOPENROULLETTEWND_H
#define RAGNAROK_SOURCE_UI_EVENTS_UIOPENROULLETTEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIOpenRoulletteWnd : public UIFrameWnd {
public:
  UIOpenRoulletteWnd();
  void OnProcess() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
