#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREFINDWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREFINDWND_H

#include "UI/Item/UIItemBaseWnd.h"

class UIItemStoreFindWnd : public UIItemBaseWnd {
public:
  void Slot26(int x, int y) override;

  ITEM_INFO GetItemAt(int x, int y);
};

#endif
