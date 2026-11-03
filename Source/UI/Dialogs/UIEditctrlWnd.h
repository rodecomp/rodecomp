#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIEDITCTRLWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIEDITCTRLWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UIEditctrlWnd : public UIFrameWnd {
public:
  UIEditctrlWnd();
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UIEditCtrl* edit;
};

#endif
