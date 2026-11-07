#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIStaticText;

class UIMerchantItemPurchaseWnd : public UIItemBaseWnd {
public:
  UIMerchantItemPurchaseWnd(int value);
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ UIStaticText* total;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ MerchantItemPurchaseWndInfo* saved;
  /* 0100 */ int hovered;
  /* 0104 */ int unknown_0104;
  /* 0108 */ int unknown_0108;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemPurchaseWnd, saved) == 0xFC);
#endif

#endif
