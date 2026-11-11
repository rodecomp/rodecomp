#ifndef RAGNAROK_SOURCE_UI_REPLAY_UIREPLAYBUTTON_H
#define RAGNAROK_SOURCE_UI_REPLAY_UIREPLAYBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIReplayButton : public UIBitmapButton {
public:
  UIReplayButton();

  void OnLBtnUp(int x, int y) override;
};

#endif
