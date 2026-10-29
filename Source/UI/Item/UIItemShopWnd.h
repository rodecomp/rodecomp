#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSHOPWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSHOPWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIItemShopWnd : public UIItemBaseWnd {
public:
  void Slot11() override;

  /* 00F0 */ unsigned char unknown_00F0[0xC];
  /* 00FC */ ItemShopWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemShopWnd, saved) == 0xFC);
#endif

#endif
