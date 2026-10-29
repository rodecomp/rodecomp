#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITVBITMAPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITVBITMAPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UITVBitmapButton : public UIBitmapButton {
public:
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

protected:
  /* 0120 */ int m_value;
};

#endif
