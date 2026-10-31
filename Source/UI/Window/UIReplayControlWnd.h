#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYCONTROLWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYCONTROLWND_H

#include "UI/Buttons/UIReplayButton.h"
#include "UI/Window/UIFrameWnd.h"

class UIBarGraph4;
class UIStaticText;
struct UIButtonContext;

class UIReplayControlWnd : public UIFrameWnd {
public:
  UIReplayControlWnd(UIWindowContext& windows, UIFrameContext& frames);

  bool Slot03() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  bool RefreshInfo();

  /* 00B4 */ UIReplayButton* m_playButton;
  /* 00B8 */ UIReplayButton* m_pauseButton;
  /* 00BC */ bool m_paused;
  /* 00C0 */ UIWindow* m_buttons[4];
  /* 00D0 */ UIBarGraph4* m_progress;
  /* 00D4 */ UIStaticText* m_texts[4];

protected:
  UIButtonContext* m_controls;
};

#endif
