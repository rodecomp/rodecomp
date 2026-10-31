#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPTITLE_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTSHOPTITLE_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class UIStaticText;
struct UIButtonContext;
struct ForbiddenWords;

class UIMerchantShopTitle : public UIWindow {
public:
  UIMerchantShopTitle(UIWindowContext& windows, UIButtonContext& controls, ForbiddenWords& forbiddenWords, int buyingShop);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 007C */ UIStaticText* label;
  /* 0080 */ std::uint32_t ownerAid;
  /* 0084 */ std::int16_t unknown_0084;
  /* 0086 */ unsigned char unknown_0086[2];
  /* 0088 */ int buyingShop;

protected:
  UIButtonContext* m_controls;
  ForbiddenWords* m_forbiddenWords;
};

#endif
