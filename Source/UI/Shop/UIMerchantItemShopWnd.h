#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIMerchantItemShopWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Slot19() override;
  void Slot10(int x, int y, std::intptr_t value) override;

  void StartTimer(std::uint32_t value);

  /* 00F0 */ unsigned char unknown_00F0[0x8];
  /* 00F8 */ MerchantItemShopWndInfo* saved;
  /* 00FC */ unsigned char unknown_00FC[0x14];
  /* 0110 */ std::uint32_t startTick;
  /* 0114 */ std::uint32_t duration;
  /* 0118 */ int unknown_0118;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemShopWnd, saved) == 0xF8);
static_assert(offsetof(UIMerchantItemShopWnd, startTick) == 0x110);
static_assert(sizeof(UIMerchantItemShopWnd) == 0x11C);
#endif

#endif
