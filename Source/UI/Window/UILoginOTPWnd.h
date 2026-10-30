#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINOTPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UILoginOTPWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
