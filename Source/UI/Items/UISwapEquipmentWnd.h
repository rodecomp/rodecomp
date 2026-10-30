#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISWAPEQUIPMENTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISWAPEQUIPMENTWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct ITEM_INFO;
struct UIButtonContext;

extern unsigned long g_swapEquipmentTick;

class UISwapBitmapButton : public UIBitmapButton {
public:
  using UIBitmapButton::UIBitmapButton;
  void Paint() override;

  /* 0120 */ std::uint32_t m_textColor;
};

class UISwapEquipmentWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  ITEM_INFO ItemAtPoint(int x, int y);
  __declspec(noinline) ITEM_INFO GetSlotItem(int slot);
  void SetSlotValue(int index, int value);

protected:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ UISwapBitmapButton* m_swapButton;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ std::uint32_t unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int unknown_00DC;
  UIButtonContext* m_controls;
};

#endif
