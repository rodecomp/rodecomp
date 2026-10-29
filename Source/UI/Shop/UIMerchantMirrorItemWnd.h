#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIMerchantMirrorItemWnd : public UIItemBaseWnd {
public:
  void Slot11() override;

  /* 00F0 */ MerchantMirrorItemWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantMirrorItemWnd, saved) == 0xF0);
#endif

#endif
