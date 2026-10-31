#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIStaticBitmap;

class UISlotBmpButton : public UIBitmapButton {
public:
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetSlot(int slot, int fallback);

protected:
  /* 0120 */ unsigned char unknown_0120[0xC];
  /* 012C */ int m_slot;
  /* 0130 */ UIStaticBitmap* icon;
  /* 0134 */ short unknown_0134;
  /* 0136 */ short unknown_0136;
  /* 0138 */ bool m_inRange;
};

#endif
