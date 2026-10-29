#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPTITLE_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPTITLE_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class UIMerchantShopTitle : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void ResetCursor(int x, int y) override;

  /* 007C */ int unknown_007C;
  /* 0080 */ std::uint32_t ownerAid;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int buyingShop;
};

#endif
