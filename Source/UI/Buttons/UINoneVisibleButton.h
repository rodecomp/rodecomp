#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UINONEVISIBLEBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UINONEVISIBLEBUTTON_H

#include "UI/Buttons/UIButton.h"

class UINoneVisibleButton : public UIButton {
public:
  UINoneVisibleButton(UIWindowContext& windowContext);
  void Paint() override;
};

#endif
