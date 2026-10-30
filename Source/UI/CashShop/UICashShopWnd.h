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
class UIStaticBitmap;
struct UIButtonContext;

class UICashShopWnd_Banner : public UIWindow {
public:
  struct Entry {
    /* 0000 */ std::string image;
    /* 0018 */ std::string url;
  };

  void Slot19() override;
  void Paint() override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ std::vector<Entry> m_entries;
  /* 0088 */ std::vector<UIWindow*> m_buttons;
  /* 0094 */ unsigned int m_index;
  /* 0098 */ std::uint32_t m_lastTick;
};

class UICashShopWnd_CartItem : public UIWindow {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0080 */ ITEM_INFO m_item;
  UIButtonContext* m_controls;
};

class UICashShopWnd_Cart : public UIWindow {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Paint() override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ std::vector<UICashShopWnd_CartItem*> m_slots;
  /* 0088 */ std::vector<ITEM_INFO> m_cartItems;
  /* 0094 */ UIWindow* m_scrollBar;
  /* 0098 */ int m_visibleCount;
  /* 009C */ unsigned int m_scrollIndex;
};

class UICashShopWnd_Item : public UIWindow {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot33(int x, int y) override;

  /* 007C */ UIBmpTextButton* m_addButton;
  /* 0080 */ ITEM_INFO m_item;
  /* 0178 */ bool m_largeLayout;
  /* 017C */ UIStaticBitmap* m_soldOut;
  UIButtonContext* m_controls;
};

class UICashShopWnd : public UIFrameWnd {
public:
  UICashShopWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool Slot03() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot19() override;
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
  /* 00E0 */ UIEditCtrl* unknown_00E0;
  /* 00E4 */ UIWindow* unknown_00E4;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ int unknown_00EC;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ UIEditCtrl* unknown_00F4;
  /* 00F8 */ std::vector<UICashShopWnd_Item*> m_itemWindows;
  /* 0104 */ UICashShopWnd_Cart* m_cart;
};

void FormatMessageBuffer(char* buffer, const char* format, ...);

#endif
