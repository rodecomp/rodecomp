#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBASICINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBASICINFOWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

class UIBasicInfoWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ int expandedHeight;
  /* 00BC */ unsigned char mode;
  /* 00BD */ unsigned char unknown_00BD[0x7];
  /* 00C4 */ BasicInfoWndInfo* saved;
};

#endif
