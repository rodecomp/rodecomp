#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIDEBUFFREMOVEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIDEBUFFREMOVEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBasicButton;
struct UIButtonContext;

class UIDebuffRemoveWnd : public UIFrameWnd {
public:
  UIDebuffRemoveWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ UIBasicButton* m_button;
  UIButtonContext* m_controls;
};

#endif
