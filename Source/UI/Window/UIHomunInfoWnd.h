#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIHOMUNINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

class UIHomunInfoWnd : public UIFrameWnd {
public:
  void Slot11() override;

  /* 00B4 */ unsigned char unknown_00B4[0x18];
  /* 00CC */ HomunInfoWndInfo* saved;
};

#endif
