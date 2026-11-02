#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIStaticBitmap;

class UISlotBmpButton : public UIBitmapButton {
  friend class UINewSelectCharWnd;

public:
  UISlotBmpButton(UIWindowContext& windows, UIButtonContext& buttons, int index, bool);
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetSlot(int slot, int fallback);

protected:
  /* 0120 */ int m_index;
  /* 0124 */ bool unknown_0124;
  /* 0128 */ int unknown_0128;
  /* 012C */ int m_slot;
  /* 0130 */ UIStaticBitmap* icon;
  /* 0134 */ short unknown_0134;
  /* 0136 */ short unknown_0136;
  /* 0138 */ bool m_inRange;
};

#endif
