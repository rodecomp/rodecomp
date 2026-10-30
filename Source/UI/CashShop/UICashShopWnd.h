#ifndef RAGNAROK_SOURCE_UI_CASHSHOP_UICASHSHOPWND_H
#define RAGNAROK_SOURCE_UI_CASHSHOP_UICASHSHOPWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <list>
#include <map>
#include <vector>

class UICashShopWnd_Banner;

class UICashShopWnd_Cart : public UIWindow {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
};

class UICashShopWnd_CartItem : public UIWindow {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
};

class UICashShopWnd_Item : public UIWindow {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
};

class UICashShopWnd : public UIFrameWnd {
public:
  UICashShopWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool Slot03() override;
  void Slot10(int x, int y, std::intptr_t value) override;

  /* 00B4 */ std::list<ITEM_INFO> m_items;
  /* 00BC */ UICashShopWnd_Banner* m_banner;
  /* 00C0 */ UICashShopWnd_Banner* m_secondBanner;
  /* 00C4 */ std::map<int, UIWindow*> m_tabButtons;
  /* 00CC */ int m_selectedTab;
  /* 00D0 */ int m_itemsPerPage;
  /* 00D4 */ int m_columns;
  /* 00D8 */ int m_page;
  /* 00DC */ int m_pageCount;
  /* 00E0 */ UIWindow* unknown_00E0;
  /* 00E4 */ UIWindow* unknown_00E4;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ int unknown_00EC;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ UIWindow* unknown_00F4;
  /* 00F8 */ std::vector<UICashShopWnd_Item*> m_itemWindows;
  /* 0104 */ UICashShopWnd_Cart* m_cart;
};

void FormatMessageBuffer(char* buffer, const char* format, ...);

#endif
