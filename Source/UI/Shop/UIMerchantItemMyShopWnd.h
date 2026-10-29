#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMMYSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMMYSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIBasicButton;
class UICheckBox;

extern bool g_vendingSaleAlarm;

class UIMerchantItemMyShopWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;

  void OpenShop();
  void SetDuration(std::uint32_t value);

  /* 00F0 */ unsigned char unknown_00F0[0x8];
  /* 00F8 */ MerchantItemMyShopWndInfo* saved;
  /* 00FC */ unsigned char unknown_00FC[0x4];
  /* 0100 */ int unknown_0100;
  /* 0104 */ UICheckBox* saleAlarm;
  /* 0108 */ unsigned char shopOpen;
  /* 0109 */ unsigned char unknown_0109[0x07];
  /* 0110 */ std::uint32_t startTick;
  /* 0114 */ std::uint32_t duration;
  /* 0118 */ UIBasicButton* actionButton;
  /* 011C */ int unknown_011C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemMyShopWnd, saved) == 0xF8);
static_assert(offsetof(UIMerchantItemMyShopWnd, unknown_0100) == 0x100);
static_assert(offsetof(UIMerchantItemMyShopWnd, saleAlarm) == 0x104);
static_assert(offsetof(UIMerchantItemMyShopWnd, startTick) == 0x110);
static_assert(offsetof(UIMerchantItemMyShopWnd, unknown_011C) == 0x11C);
#endif

#endif
