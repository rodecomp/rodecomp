#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISPRITEBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISPRITEBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <string>

class UISpriteButton : public UIButton {
public:
  int FrameHeight(int action, int frame);
  int FrameWidth(int action, int frame);

  /* 00CC */ std::string spriteName;
  /* 00E4 */ std::string actionName;
};

#endif
