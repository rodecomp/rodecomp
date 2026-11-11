#ifndef RAGNAROK_SOURCE_UI_STATUS_UIREBIRTHWND_H
#define RAGNAROK_SOURCE_UI_STATUS_UIREBIRTHWND_H

#include "UI/Window/UIFrameWnd.h"

class UIRebirthWnd : public UIFrameWnd {
public:
  UIRebirthWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
