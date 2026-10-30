#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIItemPurchaseWnd : public UIItemBaseWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;

  /* 00F0 */ unsigned char unknown_00F0[0x4];
  /* 00F4 */ ItemPurchaseWndInfo* saved;
  /* 00F8 */ int hovered;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemPurchaseWnd, saved) == 0xF4);
#endif

#endif
