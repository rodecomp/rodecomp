#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISLOTBMPBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIStaticBitmap;

class UISlotBmpButton : public UIBitmapButton {
  friend class UINewSelectCharWnd;

public:
  UISlotBmpButton(int index, bool);
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetSlot(int slot, int fallback);

protected:
  /* 0120 */ int m_index;
  /* 0124 */ bool m_selected;
  /* 0128 */ int m_displayMode;
  /* 012C */ int m_slot;
  /* 0130 */ UIStaticBitmap* icon;
  /* 0134 */ short m_animationTick;
  /* 0136 */ short m_frameInterval;
  /* 0138 */ bool m_inRange;
};

#endif
