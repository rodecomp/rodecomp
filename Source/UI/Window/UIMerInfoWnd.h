#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

struct UIButtonContext;

class UIMerInfoWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;

  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ MerInfoWndInfo* saved;

protected:
  UIButtonContext* m_controls;
};

#endif
