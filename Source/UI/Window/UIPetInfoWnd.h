#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPETINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPETINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

class UIPetInfoWnd : public UIFrameWnd {
public:
  void Slot11() override;

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ PetInfoWndInfo* saved;
};

#endif
