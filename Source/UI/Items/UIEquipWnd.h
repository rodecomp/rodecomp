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
  /* 00F4 */ int m_contentTop;
  /* 00F8 */ unsigned char unknown_00F8[0x2C];
  /* 0124 */ UIWindow* unknown_0124;
  /* 0128 */ unsigned char unknown_0128[0x0C];
  /* 0134 */ UIWindow* m_scrollBar;
  /* 0138 */ unsigned char unknown_0138[0x1C];
  /* 0154 */ int m_firstSlot;
  /* 0158 */ int m_hoverSlot;
  /* 015C */ unsigned char unknown_015C[0x04];
  /* 0160 */ std::vector<std::string> m_damageSkinNames;
  /* 016C */ std::vector<std::string> m_damageSkinIcons;

  void Slot19() override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

  void DrawDamageSkinPage();
  void NotifyEquipmentChanged();
  void UpdateCharacterView(int reset);
};

#endif
