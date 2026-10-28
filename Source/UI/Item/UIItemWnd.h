#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMWND_H

#include "UI/Item/UIItemBaseWnd.h"

class UIItemWnd : public UIItemBaseWnd {
public:
  void Slot18() override;
  void Slot19() override;

  /* 0128 */ UIWindow* unknown_0128;
};

#endif
