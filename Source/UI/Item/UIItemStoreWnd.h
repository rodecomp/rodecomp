#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIItemStoreWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  ITEM_INFO GetItemAt(int x, int y);

  /* 00F0 */ unsigned char unknown_00F0[0x74];
  /* 0164 */ WindowPlacement* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreWnd, saved) == 0x164);
#endif

#endif
