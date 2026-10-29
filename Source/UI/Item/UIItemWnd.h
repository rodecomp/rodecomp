#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMWND_H

#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>

class UIItemWnd : public UIItemBaseWnd {
public:
  void Slot18() override;
  void Slot19() override;

  ITEM_INFO GetItemAt(int x, int y);

  /* 00F0 */ unsigned char unknown_00F0[0x38];
  /* 0128 */ UIWindow* unknown_0128;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemWnd, unknown_0128) == 0x128);
#endif

#endif
