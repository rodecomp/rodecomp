#ifndef RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H
#define RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H

#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemShopWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIExpandedBarterItemPurchaseWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot35(int delta) override;
  void UpdateScrollBar();

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
};

class UIBartermarketWnd : public UIItemShopWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
};

#endif
