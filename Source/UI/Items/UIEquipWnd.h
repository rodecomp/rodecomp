#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H

#include "UI/Window/UIFrameWnd.h"

struct NormalItemSlotWindow;

class UIEquipWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Move(int x, int y) override;
  void Slot11() override;
  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ int m_show;
  /* 00C8 */ NormalItemSlotWindow* m_placement;
  /* 00CC */ unsigned char unknown_00CC[0x24];
  /* 00F0 */ int m_page;
  /* 00F4 */ unsigned char unknown_00F4[0x30];
  /* 0124 */ UIWindow* unknown_0124;
  /* 0128 */ unsigned char unknown_0128[0x0C];
  /* 0134 */ UIWindow* m_scrollBar;
  /* 0138 */ unsigned char unknown_0138[0x1C];
  /* 0154 */ int m_firstSlot;
  /* 0158 */ int m_hoverSlot;

  void Slot19() override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

  void NotifyEquipmentChanged();
};

#endif
