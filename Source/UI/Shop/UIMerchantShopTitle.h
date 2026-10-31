#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPTITLE_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPTITLE_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class UIStaticText;
struct UIButtonContext;

class UIMerchantShopTitle : public UIWindow {
public:
  UIMerchantShopTitle(UIWindowContext& windows, UIButtonContext& controls, int buyingShop);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot32(int x, int y) override;

  /* 007C */ UIStaticText* label;
  /* 0080 */ std::uint32_t ownerAid;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int buyingShop;

protected:
  UIButtonContext* m_controls;
};

#endif
