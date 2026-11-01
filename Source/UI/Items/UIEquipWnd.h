#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H

#include "UI/Window/UIFrameWnd.h"

struct NormalItemSlotWindow;
struct ITEM_INFO;
class UITabControl;
class UIListBoxTitle;

class UIEquipWnd : public UIFrameWnd {
public:
  UIEquipWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool Slot03() override;
  void Move(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  /* 00B4 */ int m_viewOther;
  /* 00B8 */ int m_highlight[3];
  /* 00C4 */ int m_show;
  /* 00C8 */ NormalItemSlotWindow* m_placement;
  /* 00CC */ unsigned char unknown_00CC[0x08];
  /* 00D4 */ std::uint32_t m_updateTick;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ unsigned char unknown_00DC[0x10];
  /* 00EC */ UITabControl* m_tabs;
  /* 00F0 */ int m_page;
  /* 00F4 */ int m_contentTop;
  /* 00F8 */ int unknown_00F8[8];
  /* 0118 */ int unknown_0118;
  /* 011C */ int unknown_011C;
  /* 0120 */ UIListBoxTitle* m_titleList;
  /* 0124 */ UIWindow* unknown_0124;
  /* 0128 */ int unknown_0128;
  /* 012C */ int unknown_012C;
  /* 0130 */ int unknown_0130;
  /* 0134 */ UIWindow* m_scrollBar;
  /* 0138 */ int unknown_0138[6];
  /* 0150 */ int unknown_0150;
  /* 0154 */ int m_firstSlot;
  /* 0158 */ int m_hoverSlot;
  /* 015C */ int unknown_015C;
  /* 0160 */ std::vector<std::string> m_damageSkinNames;
  /* 016C */ std::vector<std::string> m_damageSkinIcons;
  /* 0178 */ int unknown_0178;

  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int delta) override;

  void DrawDamageSkinPage();
  void DrawEquipSlots();
  void RefreshTitleList();
  void NotifyEquipmentChanged();
  void UpdateCharacterView(int reset);
  ITEM_INFO GetSlotItem(int slot);
  ITEM_INFO ItemIconAt(int x, int y);
  ITEM_INFO ItemEntryAt(int x, int y);
};

#endif
