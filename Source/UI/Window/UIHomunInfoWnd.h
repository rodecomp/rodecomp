#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

class UIINT64BarGraph;

class UIHomunInfoWnd : public UIFrameWnd {
public:
  void Slot11() override;

  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ UIINT64BarGraph* expBar;
  /* 00C8 */ unsigned char unknown_00C8[0x04];
  /* 00CC */ HomunInfoWndInfo* saved;
};

#endif
