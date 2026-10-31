#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMPURCHASEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIStaticText;

class UIMerchantItemPurchaseWnd : public UIItemBaseWnd {
public:
  UIMerchantItemPurchaseWnd(UIWindowContext& windows, UIFrameContext& frames, int value);
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;

  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ UIStaticText* total;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ MerchantItemPurchaseWndInfo* saved;
  /* 0100 */ int hovered;
  /* 0104 */ unsigned char unknown_0104[0x4];
  /* 0108 */ int unknown_0108;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemPurchaseWnd, saved) == 0xFC);
#endif

#endif
