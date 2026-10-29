#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMPURCHASEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIItemPurchaseWnd : public UIItemBaseWnd {
public:
  void Slot11() override;

  /* 00F0 */ unsigned char unknown_00F0[0x4];
  /* 00F4 */ ItemPurchaseWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemPurchaseWnd, saved) == 0xF4);
#endif

#endif
