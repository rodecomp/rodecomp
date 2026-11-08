#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
class UIStaticText;

class UILoginOTPWnd : public UIFrameWnd {
public:
  UILoginOTPWnd();
  void OnCreate(int width, int height) override;
  void OnDestroy() override;
  void OnProcess() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIEditCtrl* authCodeEdit;
  /* 00B8 */ int inputState;
  /* 00BC */ UIStaticText* timerText;
  /* 00C0 */ int timerState;
};

#endif
