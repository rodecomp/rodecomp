#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIMerchantMirrorItemWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;

  /* 00F0 */ MerchantMirrorItemWndInfo* saved;
  /* 00F4 */ int unknown_00F4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantMirrorItemWnd, saved) == 0xF0);
#endif

#endif
