#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

class UIBarGraph4;
class UIINT64BarGraph;

class UIHomunInfoWnd : public UIFrameWnd {
public:
  UIHomunInfoWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;

  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ UIINT64BarGraph* expBar;
  /* 00C8 */ UIBarGraph4* intimacyBar;
  /* 00CC */ HomunInfoWndInfo* saved;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
};

#endif
