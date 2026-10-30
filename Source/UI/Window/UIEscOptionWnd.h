#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H

#include "UI/Window/UIFrameWnd.h"

class UIButton;
struct UIButtonContext;

class UIEscOptionWnd : public UIFrameWnd {
public:
  void Paint() override;
  void Slot15(int width, int height) override;
  void LayoutButtons();

  /* 00B4 */ UIButton* m_buttons[7];
  /* 00D0 */ int m_mode;
  /* 00D4 */ int m_titleHeight;

protected:
  UIButtonContext* m_controls;
};

#endif
