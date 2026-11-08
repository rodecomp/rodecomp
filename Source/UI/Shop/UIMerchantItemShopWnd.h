#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class UIStaticText;

class UIMerchantItemShopWnd : public UIItemBaseWnd {
public:
  UIMerchantItemShopWnd(int value);
  std::vector<unsigned char> GetReplayData() override;
  void StoreInfo() override;
  void OnProcess() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  void StartTimer(std::uint32_t value);

  /* 00F0 */ int nonItemHeight;
  /* 00F4 */ UIStaticText* titleText;
  /* 00F8 */ MerchantItemShopWndInfo* saved;
  /* 00FC */ int hovered;
  /* 0100 */ int ownerAid;
  /* 0104 */ UIStaticText* notice;
  /* 0108 */ int buyingZeny;
  /* 010C */ int buyingShop;
  /* 0110 */ std::uint32_t startTick;
  /* 0114 */ std::uint32_t duration;
  /* 0118 */ int footerHeight;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemShopWnd, saved) == 0xF8);
static_assert(offsetof(UIMerchantItemShopWnd, startTick) == 0x110);
static_assert(sizeof(UIMerchantItemShopWnd) == 0x11C);
#endif

#endif
