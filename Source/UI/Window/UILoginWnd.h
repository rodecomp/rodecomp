#ifndef RAGNAROK_SOURCE_UI_WINDOW_UILOGINWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UILOGINWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UILoginWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  void SaveLogin();

  /* 00B4 */ UIEditCtrl* idEdit;
  /* 00B8 */ unsigned char unknown_00B8[0x10];
  /* 00C8 */ int saveId;
};

#endif
