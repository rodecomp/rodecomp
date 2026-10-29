#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UICOLORBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UICOLORBUTTON_H

#include "UI/Buttons/UIButton.h"

class UIColorButton : public UIButton {
public:
  void Paint() override;

  /* 00B4 */ unsigned activeRed;
  /* 00B8 */ unsigned activeGreen;
  /* 00BC */ unsigned activeBlue;
  /* 00C0 */ unsigned normalRed;
  /* 00C4 */ unsigned normalGreen;
  /* 00C8 */ unsigned normalBlue;
};

#endif
