#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEXCHANGEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIExchangeWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ UIWindow* m_scrollBar;

  void Slot35(int delta) override;
};

#endif
