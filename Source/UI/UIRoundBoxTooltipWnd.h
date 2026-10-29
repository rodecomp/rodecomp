#ifndef RAGNAROK_SOURCE_UI_UIROUNDBOXTOOLTIPWND_H
#define RAGNAROK_SOURCE_UI_UIROUNDBOXTOOLTIPWND_H

#include "UI/Window/UIFrameWnd.h"

class UIRoundBoxTooltipWnd : public UIFrameWnd {
public:
  void Paint() override;

  /* 00B4 */ UIWindow* viewer;
};

#endif
