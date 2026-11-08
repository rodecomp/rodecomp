#ifndef RAGNAROK_SOURCE_UI_CASHSHOP_UICASHSHOPWND_H
#define RAGNAROK_SOURCE_UI_CASHSHOP_UICASHSHOPWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <vector>

class UIBmpTextButton;
class UIEditCtrl;
class UIEditCtrl_Custom;
class UIScrollBar_Custom;
class UIStaticBitmap;

class UICashShopWnd_Banner : public UIWindow {
public:
  struct Entry {
    /* 0000 */ std::string image;
    /* 0018 */ std::string url;
  };

  UICashShopWnd_Banner();
  void OnProcess() override;
  void Paint() override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ std::vector<Entry> m_entries;
  /* 0088 */ std::vector<UIWindow*> m_buttons;
  /* 0094 */ unsigned int m_index;
  /* 0098 */ std::uint32_t m_lastTick;

  void CreateButtons();
};

class UICashShopWnd_CartItem : public UIWindow {
public:
  UICashShopWnd_CartItem();

  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0080 */ ITEM_INFO m_item;
};

class UICashShopWnd_Cart : public UIWindow {
public:
  UICashShopWnd_Cart();

  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ std::vector<UICashShopWnd_CartItem*> m_slots;
  /* 0088 */ std::vector<ITEM_INFO> m_cartItems;
  /* 0094 */ UIScrollBar_Custom* m_scrollBar;
  /* 0098 */ int m_visibleCount;
  /* 009C */ unsigned int m_scrollIndex;
};

class UICashShopWnd_Item : public UIWindow {
public:
  // INLINED
  UICashShopWnd_Item()
      : UIWindow(0), m_addButton(nullptr), m_largeLayout(false), m_soldOut(nullptr) {
  }

  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ UIBmpTextButton* m_addButton;
  /* 0080 */ ITEM_INFO m_item;
  /* 0178 */ bool m_largeLayout;
  /* 017C */ UIStaticBitmap* m_soldOut;
};

class UICashShopWnd : public UIFrameWnd {
public:
  UICashShopWnd();
  ~UICashShopWnd() override;
  bool NeedsFrameRefresh() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void LayoutPage();

  /* 00B4 */ std::list<ITEM_INFO> m_items;
  /* 00BC */ UICashShopWnd_Banner* m_banner;
  /* 00C0 */ UICashShopWnd_Banner* m_secondBanner;
  /* 00C4 */ std::map<int, UIWindow*> m_tabButtons;
  /* 00CC */ int m_selectedTab;
  /* 00D0 */ int m_itemsPerPage;
  /* 00D4 */ int m_columns;
  /* 00D8 */ int m_page;
  /* 00DC */ int m_pageCount;
  /* 00E0 */ UIEditCtrl* m_searchEdit;
  /* 00E4 */ UIBmpTextButton* m_refreshButton;
  /* 00E8 */ int m_cashPoints;
  /* 00EC */ int m_secondaryPoints;
  /* 00F0 */ int m_totalPrice;
  /* 00F4 */ UIEditCtrl_Custom* m_pointsEdit;
  /* 00F8 */ std::vector<UICashShopWnd_Item*> m_itemWindows;
  /* 0104 */ UICashShopWnd_Cart* m_cart;
};

void FormatMessageBuffer(char* buffer, const char* format, ...);

#endif
