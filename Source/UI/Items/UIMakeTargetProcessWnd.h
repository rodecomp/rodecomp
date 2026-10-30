#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETPROCESSWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETPROCESSWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

class UIMakeTargetProcessWnd : public UIFrameWnd {
public:
  ITEM_INFO GetItemAt(int x, int y);

  /* 00B4 */ int makeType;
  /* 00B8 */ ITEM_INFO materials[3];
};

#endif
