#ifndef RAGNAROK_SOURCE_UI_SKILL_UIGRAFFISTRBOXWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UIGRAFFISTRBOXWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UIGraffiStrboxWnd : public UIFrameWnd {
public:
  UIGraffiStrboxWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:

public:
  int area[4];
  UIEditCtrl* edit;
};

#endif
