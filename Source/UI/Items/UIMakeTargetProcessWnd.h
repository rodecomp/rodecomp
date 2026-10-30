#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETPROCESSWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETPROCESSWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMakeTargetProcessWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  ITEM_INFO GetItemAt(int x, int y);

  /* 00B4 */ int makeType;
  /* 00B8 */ ITEM_INFO materials[3];

protected:
  UIButtonContext* m_controls;
};

#endif
