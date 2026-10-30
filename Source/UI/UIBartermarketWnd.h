#ifndef RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H
#define RAGNAROK_SOURCE_UI_UIBARTERMARKETWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIExpandedBarterItemPurchaseWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot35(int delta) override;

  /* 00B4 */ unsigned char unknown_00B4[0x0C];
  /* 00C0 */ UIWindow* scrollBar;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int rowCount;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ unsigned char scrollEnabled;
};

class UIExpandedBartermarketWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot35(int delta) override;

  /* 00B4 */ unsigned char unknown_00B4[0x0C];
  /* 00C0 */ UIWindow* scrollBar;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int rowCount;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ unsigned char scrollEnabled;
};

#endif
