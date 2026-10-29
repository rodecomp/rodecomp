#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UICheckBox;

class UIItemStoreWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  ITEM_INFO GetItemAt(int x, int y);
  void SetTabChecked(int index, bool checked);

  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ unsigned char unknown_00F4[0x04];
  /* 00F8 */ unsigned char unknown_00F8[0x08];
  /* 0100 */ UICheckBox* tabs[8];
  /* 0120 */ int unknown_0120[8];
  /* 0140 */ unsigned char unknown_0140[0x24];
  /* 0164 */ WindowPlacement* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreWnd, tabs) == 0x100);
static_assert(offsetof(UIItemStoreWnd, saved) == 0x164);
#endif

#endif
