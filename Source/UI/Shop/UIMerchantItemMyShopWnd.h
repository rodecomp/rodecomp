#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMMYSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMMYSHOPWND_H

#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIMerchantItemMyShopWnd : public UIItemBaseWnd {
public:
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 00F0 */ unsigned char unknown_00F0[0x10];
  /* 0100 */ int unknown_0100;
  /* 0104 */ unsigned char unknown_0104[0x0C];
  /* 0110 */ std::uint32_t startTick;
  /* 0114 */ std::uint32_t duration;
  /* 0118 */ int unknown_0118;
  /* 011C */ int unknown_011C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemMyShopWnd, unknown_0100) == 0x100);
static_assert(offsetof(UIMerchantItemMyShopWnd, startTick) == 0x110);
static_assert(offsetof(UIMerchantItemMyShopWnd, unknown_011C) == 0x11C);
#endif

#endif
