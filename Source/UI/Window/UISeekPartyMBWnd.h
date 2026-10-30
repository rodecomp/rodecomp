#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct SeekPartyMBWndInfo;

class UISeekPartyMBWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;

protected:
  /* 00BC */ unsigned char unknown_00BC[0xAC];
  /* 0168 */ unsigned char m_minimized;
  /* 0169 */ unsigned char unknown_0169[0x13];
  /* 017C */ SeekPartyMBWndInfo* m_saved;
  UIButtonContext* m_controls;
};

#endif
