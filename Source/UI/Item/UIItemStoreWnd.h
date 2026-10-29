#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H

#include "UI/Item/UIItemBaseWnd.h"

class UIItemStoreWnd : public UIItemBaseWnd {
public:
  ITEM_INFO GetItemAt(int x, int y);
};

#endif
