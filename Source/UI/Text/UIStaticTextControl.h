#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXTCONTROL_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXTCONTROL_H

#include "UI/Window/UIControl.h"

class CUIVScrollBar;

class CUIStaticText : public CUIControl {
public:
  /* 0084 */ unsigned char unknown_0084[0x3C];
  /* 00C0 */ CUIVScrollBar* scrollBar;
  /* 00C4 */ unsigned char unknown_00C4[0x38];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIStaticText) == 0xFC);
#endif

#endif
