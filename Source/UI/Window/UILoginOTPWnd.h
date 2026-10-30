#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
class UIStaticText;

class UILoginOTPWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Slot19() override;
  void Paint() override;

  /* 00B4 */ unsigned char unknown_00B4[0x8];
  /* 00BC */ UIStaticText* timerText;

protected:
  UIButtonContext* m_controls;
};

#endif
