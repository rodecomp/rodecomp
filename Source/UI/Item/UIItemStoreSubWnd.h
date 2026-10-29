#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTORESUBWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTORESUBWND_H

#include "UI/Item/UIItemBaseWnd.h"

class UIItemStoreSubWnd : public UIItemBaseWnd {
public:
  void Slot26(int x, int y) override;

  ITEM_INFO GetItemAt(int x, int y);
};

#endif
