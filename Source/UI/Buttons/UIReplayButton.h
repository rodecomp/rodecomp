#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIREPLAYBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIREPLAYBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIReplayButton : public UIBitmapButton {
public:
  UIReplayButton(UIWindowContext& windows);

  void Slot31(int x, int y) override;
};

#endif
