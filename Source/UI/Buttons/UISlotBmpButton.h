#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UISlotBmpButton : public UIBitmapButton {
public:
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;

protected:
  /* 0120 */ unsigned char unknown_0120[0x14];
  /* 0134 */ short unknown_0134;
  /* 0136 */ short unknown_0136;
};

#endif
