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
  UIExpandedBarterItemPurchaseWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  bool Purchase();
  void UpdateScrollBar();
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryOrChunkAt(int x, int y);
  int GetEntryIndexAt(int x, int y);

  /* 00B4 */ std::list<BarterMarketEntry> entries;
  /* 00BC */ UIResizer* resizer;
  /* 00C0 */ UIScrollBar* scrollBar;
  /* 00C4 */ int firstRow;
  /* 00C8 */ int rowCount;
  /* 00CC */ int hoveredRow;
  /* 00D0 */ unsigned char scrollEnabled;
  /* 00D4 */ UIExpandedBarterPurchaseButton* buyButton;
  /* 00D8 */ UIBitmapButton* cancelButton;
};

class UIExpandedBartermarketWnd : public UIFrameWnd {
public:
  UIExpandedBartermarketWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScrollBar();
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketEntry> GetEntryOrChunkAt(int x, int y);
  int GetEntryIndexAt(int x, int y);

  /* 00B4 */ std::list<BarterMarketEntry> entries;
  /* 00BC */ UIWindow* resizer;
  /* 00C0 */ UIScrollBar* scrollBar;
  /* 00C4 */ int firstRow;
  /* 00C8 */ int rowCount;
  /* 00CC */ int hoveredRow;
  /* 00D0 */ unsigned char scrollEnabled;
};

class UIBarterItemPurchaseWnd : public UIItemPurchaseWnd {
public:
  UIBarterItemPurchaseWnd();
  void Paint() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  bool Purchase();
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemOrPriceAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemAt(int x, int y);
  int GetInventoryItemCount(int itemId);
};

class UIBartermarketWnd : public UIItemShopWnd {
public:
  UIBartermarketWnd();
  void Paint() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemAt(int x, int y);
  __declspec(noinline) std::pair<bool, BarterMarketItem> GetBarterItemOrPriceAt(int x, int y);
  int GetBarterItemIndexAt(int x, int y);
};

class UIPurchaseButton : public UIBitmapButton {
public:
  // OFFSET: 0x008401C0
  // FUNCTION: fn_10540
  UIPurchaseButton() = default;
  void OnMouseHover(int x, int y) override;
};

class UIExpandedBarterPurchaseButton : public UIBitmapButton {
public:
  // OFFSET: 0x00844CA0
  // FUNCTION: fn_10568
  UIExpandedBarterPurchaseButton() = default;
  void OnMouseHover(int x, int y) override;
  std::string FormatChunkText(int refine, std::string name, int amount);
};


#endif
