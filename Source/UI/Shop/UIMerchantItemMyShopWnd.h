#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMMYSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMMYSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Items/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIBasicButton;
class UICheckBox;
class UIStaticText;

class UIMerchantItemMyShopWnd : public UIItemBaseWnd {
public:
  UIMerchantItemMyShopWnd(int value);
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void OpenShop();
  void SetDuration(std::uint32_t value);

  /* 00F0 */ int buyingZeny;
  /* 00F4 */ UIStaticText* title;
  /* 00F8 */ MerchantItemMyShopWndInfo* saved;
  /* 00FC */ UIStaticText* notice;
  /* 0100 */ int buyingShop;
  /* 0104 */ UICheckBox* saleAlarm;
  /* 0108 */ unsigned char shopOpen;
  /* 0109 */ unsigned char padding_0109[0x03];
  /* 010C */ int offlineStoreId;
  /* 0110 */ std::uint32_t startTick;
  /* 0114 */ std::uint32_t duration;
  /* 0118 */ UIBasicButton* actionButton;
  /* 011C */ int footerHeight;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemMyShopWnd, saved) == 0xF8);
static_assert(offsetof(UIMerchantItemMyShopWnd, buyingShop) == 0x100);
static_assert(offsetof(UIMerchantItemMyShopWnd, saleAlarm) == 0x104);
static_assert(offsetof(UIMerchantItemMyShopWnd, startTick) == 0x110);
static_assert(offsetof(UIMerchantItemMyShopWnd, footerHeight) == 0x11C);
#endif

#endif
