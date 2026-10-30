#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINOTICEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINOTICEWND_H

#include "UI/Window/UIFrameWnd.h"

class UINoticeWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot35(int delta) override;

  /* 00B4 */ unsigned char unknown_00B4[0xBC];
  /* 0170 */ UIWindow* m_scrollBar;

protected:
  UIButtonContext* m_controls;
};

#endif
