#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITVBITMAPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITVBITMAPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UITVBitmapButton : public UIBitmapButton {
  friend class UITreeView;

public:
  UITVBitmapButton();
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;

protected:
  /* 0120 */ int m_value;
};

#endif
