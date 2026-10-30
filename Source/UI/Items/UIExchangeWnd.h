#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIExchangeWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIWindow* m_buttons[3];
  /* 00C0 */ UIWindow* unknown_00C0;
  /* 00C4 */ UIWindow* m_scrollBar;
  /* 00C8 */ unsigned char unknown_00C8[0x04];
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ unsigned char unknown_00D4[0x04];
  /* 00D8 */ int unknown_00D8;

  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot35(int delta) override;

  void LayoutButtons();
  void CancelInput();
};

#endif
