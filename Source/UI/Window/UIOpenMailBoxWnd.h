#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIOPENMAILBOXWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIOPENMAILBOXWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIOpenMailBoxWnd : public UIFrameWnd {
public:
  UIOpenMailBoxWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  virtual void Slot53();

protected:
  UIButtonContext* m_controls;
};

#endif
