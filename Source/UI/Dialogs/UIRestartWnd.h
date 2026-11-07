#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIRESTARTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIRESTARTWND_H

#include "UI/Window/UIFrameWnd.h"

class UIRestartWnd : public UIFrameWnd {
public:
  UIRestartWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void OnCreate(int width, int height) override;

  /* 00B4 */ unsigned char unknown_00B4[4];
};

#endif
