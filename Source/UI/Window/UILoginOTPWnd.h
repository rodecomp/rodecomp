#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIEditContext;
class UIEditCtrl;
class UIStaticText;

class UILoginOTPWnd : public UIFrameWnd {
public:
  UILoginOTPWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIEditCtrl* unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ UIStaticText* timerText;
  /* 00C0 */ int unknown_00C0;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
};

#endif
