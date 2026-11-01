#ifndef RAGNAROK_SOURCE_UI_TEXT_UICHARINFOBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UICHARINFOBALLOONTEXT_H

#include "UI/Text/UITransBalloonText.h"

class UICharInfoBalloonText : public UITransBalloonText {
public:
  // INLINED
  UICharInfoBalloonText(UIWindowContext& windows, BalloonTextContext& text) : UITransBalloonText(windows, text) {}
  void Paint() override;
};

#endif
