#ifndef RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H
#define RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H

#include "Contents/Items/BarterMarketItemMgr.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemShopWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <utility>

class UIExpandedBarterItemPurchaseWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot35(int delta) override;
  void UpdateScrollBar();
  std::pair<bool, BarterMarketEntry> GetEntryAt(int x, int y);
  std::pair<bool, BarterMarketEntry> GetEntryOrChunkAt(int x, int y);
  std::list<BarterMarketEntry>& Entries() { return *reinterpret_cast<std::list<BarterMarketEntry>*>(&this->unknown_00B4); }

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int itemCount;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ UIScrollBar* scrollBar;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int rowCount;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ unsigned char scrollEnabled;
};

class UIExpandedBartermarketWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot35(int delta) override;
  void UpdateScrollBar();
  std::pair<bool, BarterMarketEntry> GetEntryAt(int x, int y);
  std::pair<bool, BarterMarketEntry> GetEntryOrChunkAt(int x, int y);
  std::list<BarterMarketEntry>& Entries() { return *reinterpret_cast<std::list<BarterMarketEntry>*>(&this->unknown_00B4); }

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int itemCount;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ UIScrollBar* scrollBar;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int rowCount;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ unsigned char scrollEnabled;
};

class UIBarterItemPurchaseWnd : public UIItemPurchaseWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  std::pair<bool, BarterMarketItem> GetBarterItemOrPriceAt(int x, int y);
  std::pair<bool, BarterMarketItem> GetBarterItemAt(int x, int y);
  int GetInventoryItemCount(int itemId);
};

class UIBartermarketWnd : public UIItemShopWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  std::pair<bool, BarterMarketItem> GetBarterItemAt(int x, int y);
  std::pair<bool, BarterMarketItem> GetBarterItemOrPriceAt(int x, int y);
};

#endif
