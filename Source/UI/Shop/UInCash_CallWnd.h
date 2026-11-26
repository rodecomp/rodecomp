#ifndef RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Items/UIItemBaseWnd.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>

struct WindowPlacement;
class UIScrollBar;
class UIButton;
class UIEditCtrl;
class UIStaticText;

struct NCashWndInfo;
class UIBitmapButton;

enum NCashButtonSlot : int {
  NCASH_BUTTON_TAB_NEW = 0,
  NCASH_BUTTON_TAB_POPULAR = 1,
  NCASH_BUTTON_TAB_LIMITED = 2,
  NCASH_BUTTON_TAB_RENTAL_EQUIP = 3,
  NCASH_BUTTON_TAB_PERMANENT_EQUIP = 4,
  NCASH_BUTTON_TAB_BUFF = 5,
  NCASH_BUTTON_TAB_RECOVERY = 6,
  NCASH_BUTTON_TAB_ETC = 7,
  NCASH_BUTTON_TAB_BARGAIN = 8,
  NCASH_BUTTON_TAB_SEARCH = 9,
  NCASH_BUTTON_FIRST_PAGE = 10,
  NCASH_BUTTON_PREV_PAGE = 11,
  NCASH_BUTTON_NEXT_PAGE = 12,
  NCASH_BUTTON_LAST_PAGE = 13,
  NCASH_BUTTON_BUY = 14,
  NCASH_BUTTON_RESET_CART = 15,
  NCASH_BUTTON_CLOSE = 16,
  NCASH_BUTTON_FIND = 17,
  NCASH_BUTTON_REFRESH = 18,
  NCASH_BUTTON_COUNT = 19,
  NCASH_BUTTON_OUT_OF_RANGE_19 = 19,
};

class UInCashWnd : public UIItemBaseWnd {
public:
  UInCashWnd();
  bool NeedsFrameRefresh() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void StoreInfo() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  ITEM_INFO ItemAt(int x, int y);
  void DrawItems();

  /* 00F0 */ UIEditCtrl* m_pointEdit;
  /* 00F4 */ UIEditCtrl* m_searchEdit;
  /* 00F8 */ std::list<ITEM_INFO> m_otherItems;
  /* 0100 */ NCashWndInfo* m_placement;
  /* 0104 */ UIBitmapButton* m_buttons[NCASH_BUTTON_COUNT];
  /* 0150 */ UIScrollBar* m_otherScroll;
  /* 0154 */ UIStaticText* m_title;
  /* 0158 */ int m_firstOther;
  /* 015C */ int m_selected;
  /* 0160 */ int m_pageCount;
  /* 0164 */ int m_page;
  /* 0168 */ int m_tab;
  /* 016C */ int m_pointBalance;
  /* 0170 */ int m_totalPrice;
  /* 0174 */ int m_selectedIndex;
  /* 0178 */ int m_cashBalance;
  /* 017C */ int m_rowHeight;
  /* 0180 */ int m_listTop;
  /* 0184 */ int m_listHeight;
};

class UInCash_CallWnd : public UIFrameWnd {
public:
  UInCash_CallWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
