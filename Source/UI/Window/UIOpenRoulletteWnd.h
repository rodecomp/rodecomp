#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIOPENROULLETTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIOPENROULLETTEWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIOpenRoulletteWnd : public UIFrameWnd {
public:
  UIOpenRoulletteWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot19() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;
};

#endif
