#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIStaticText;

class UIMerchantItemShopWnd : public UIItemBaseWnd {
public:
  UIMerchantItemShopWnd(UIWindowContext& windows, UIFrameContext& frames, int value);
  void Slot11() override;
  void Slot19() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  void StartTimer(std::uint32_t value);

  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ UIStaticText* unknown_00F4;
  /* 00F8 */ MerchantItemShopWndInfo* saved;
  /* 00FC */ int hovered;
  /* 0100 */ int unknown_0100;
  /* 0104 */ UIStaticText* notice;
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
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
