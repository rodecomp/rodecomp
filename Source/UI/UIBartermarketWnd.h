#ifndef RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H
#define RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H

#include "Contents/Items/BarterMarketItemMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemShopWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <utility>

class UIExpandedBarterPurchaseButton;
class UIResizer;

class UIExpandedBarterItemPurchaseWnd : public UIFrameWnd {
public:
  UIExpandedBarterItemPurchaseWnd(UIWindowContext& windowContext, UIFrameContext& frameContext);
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  bool Purchase();
  void UpdateScrollBar();
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryOrChunkAt(int x, int y);
  std::list<BarterMarketEntry>& Entries() { return *reinterpret_cast<std::list<BarterMarketEntry>*>(&this->unknown_00B4); }

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int itemCount;
  /* 00BC */ UIResizer* resizer;
  /* 00C0 */ UIScrollBar* scrollBar;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int rowCount;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ unsigned char scrollEnabled;
  /* 00D4 */ UIExpandedBarterPurchaseButton* buyButton;
  /* 00D8 */ UIBitmapButton* cancelButton;
};

class UIExpandedBartermarketWnd : public UIFrameWnd {
public:
  UIExpandedBartermarketWnd(UIWindowContext& windowContext, UIFrameContext& frameContext);
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScrollBar();
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryOrChunkAt(int x, int y);
  std::list<BarterMarketEntry>& Entries() { return *reinterpret_cast<std::list<BarterMarketEntry>*>(&this->unknown_00B4); }

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int itemCount;
  /* 00BC */ UIWindow* unknown_00BC;
  /* 00C0 */ UIScrollBar* scrollBar;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int rowCount;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ unsigned char scrollEnabled;
};

class UIBarterItemPurchaseWnd : public UIItemPurchaseWnd {
public:
  void Paint() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  bool Purchase();
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemOrPriceAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemAt(int x, int y);
  int GetInventoryItemCount(int itemId);
};

class UIBartermarketWnd : public UIItemShopWnd {
public:
  void Paint() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemOrPriceAt(int x, int y);
};

class UIPurchaseButton : public UIBitmapButton {
public:
  void Slot28(int x, int y) override;
};

class UIExpandedBarterPurchaseButton : public UIBitmapButton {
public:
  void Slot28(int x, int y) override;
  std::string FormatChunkText(int refine, std::string name, int amount);
};

#endif
