#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H

#include "UI/Window/UIFrameWnd.h"

struct NormalItemSlotWindow;
struct ITEM_INFO;
class UITabControl;
class UIListBoxTitle;
class UIBitmapButton;
class UICheckBox;

class UIEquipWnd : public UIFrameWnd {
public:
  UIEquipWnd();
  bool NeedsFrameRefresh() override;
  void Move(int x, int y) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void StoreInfo() override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  /* 00B4 */ int m_viewOther;
  /* 00B8 */ int m_highlight[3];
  /* 00C4 */ int m_show;
  /* 00C8 */ NormalItemSlotWindow* m_placement;
  /* 00CC */ UIBitmapButton* m_viewButton;
  /* 00D0 */ UIBitmapButton* m_otherButton;
  /* 00D4 */ std::uint32_t m_updateTick;
  /* 00D8 */ int contentOffset;
  /* 00DC */ UICheckBox* allowViewCheck;
  /* 00E0 */ UICheckBox* swapModeCheck;
  /* 00E4 */ unsigned char unknown_00E4[0x08];
  /* 00EC */ UITabControl* m_tabs;
  /* 00F0 */ int m_page;
  /* 00F4 */ int m_contentTop;
  /* 00F8 */ int appearance[8];
  /* 0118 */ int robeId;
  /* 011C */ int costumeJob;
  /* 0120 */ UIListBoxTitle* m_titleList;
  /* 0124 */ UIWindow* swapWindow;
  /* 0128 */ int swapState;
  /* 012C */ UIBitmapButton* swapButton;
  /* 0130 */ UIBitmapButton* unequipButton;
  /* 0134 */ UIWindow* m_scrollBar;
  /* 0138 */ UIWindow* damageSkinButtons[6];
  /* 0150 */ int unknown_0150;
  /* 0154 */ int m_firstSlot;
  /* 0158 */ int m_hoverSlot;
  /* 015C */ int damageSkinMode;
  /* 0160 */ std::vector<std::string> m_damageSkinNames;
  /* 016C */ std::vector<std::string> m_damageSkinIcons;
  /* 0178 */ UIBitmapButton* abilityButton;

  void OnProcess() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  void DrawDamageSkinPage();
  void DrawEquipSlots();
  void RefreshTitleList();
  void NotifyEquipmentChanged();
  void UpdateCharacterView(int reset);
  void RefreshLook();
  ITEM_INFO GetSlotItem(int slot);
  ITEM_INFO ItemIconAt(int x, int y);
  ITEM_INFO ItemEntryAt(int x, int y);
};

#endif
