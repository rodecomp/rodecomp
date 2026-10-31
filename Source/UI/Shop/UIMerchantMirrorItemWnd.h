#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIMerchantMirrorItemWnd : public UIItemBaseWnd {
public:
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot11() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00F0 */ MerchantMirrorItemWndInfo* saved;
  /* 00F4 */ int unknown_00F4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantMirrorItemWnd, saved) == 0xF0);
#endif

#endif
