#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISWAPEQUIPMENTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISWAPEQUIPMENTWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct ITEM_INFO;

extern unsigned long g_swapEquipmentTick;

class UISwapBitmapButton : public UIBitmapButton {
public:
  using UIBitmapButton::UIBitmapButton;
  void Paint() override;

  /* 0120 */ std::uint32_t m_textColor;
};

class UISwapEquipmentWnd : public UIFrameWnd {
  friend class UIEquipWnd;

public:
  UISwapEquipmentWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  ITEM_INFO ItemAtPoint(int x, int y);
  __declspec(noinline) ITEM_INFO GetSlotItem(int slot);
  void SetSlotValue(int index, int value);
  void DrawEquipmentSlots();

protected:
  /* 00B4 */ std::uint32_t padding_00B4;
  /* 00B8 */ UISwapBitmapButton* m_swapButton;
  /* 00BC */ int equipmentPage;
  /* 00C0 */ std::uint32_t previewTick;
  /* 00C4 */ int topAccessory;
  /* 00C8 */ int midAccessory;
  /* 00CC */ int bottomAccessory;
  /* 00D0 */ int weaponView;
  /* 00D4 */ int hoverSlot;
  /* 00D8 */ int hoverSecondarySlot;
  /* 00DC */ int hoverTertiarySlot;
};

#endif
