#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UISeekPartyMBWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  /* 00BC */ unsigned char unknown_00BC[0xAC];
  /* 0168 */ unsigned char m_minimized;
  UIButtonContext* m_controls;
};

#endif
