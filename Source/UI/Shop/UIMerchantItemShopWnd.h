#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIMerchantItemShopWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Slot19() override;

  /* 00F0 */ unsigned char unknown_00F0[0x8];
  /* 00F8 */ MerchantItemShopWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemShopWnd, saved) == 0xF8);
#endif

#endif
