#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIMerchantItemWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Paint() override;
  void Slot32(int x, int y) override;

  /* 00F0 */ MerchantItemWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantItemWnd, saved) == 0xF0);
#endif

#endif
