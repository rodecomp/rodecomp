#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIGAMEITEM_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIGAMEITEM_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIControl.h"

class CUIGameItem : public CUIControl {
public:
  /* 0080 */ int unknown_0080;
  /* 0084 */ unsigned char unknown_0084;
  /* 0085 */ unsigned char unknown_0085;
  /* 0086 */ unsigned char unknown_0086;
  /* 0088 */ short scale;
  /* 0090 */ ITEM_INFO itemInfo;

  ITEM_INFO* GetItemInfo();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIGameItem, itemInfo) == 0x90);
#endif

#endif
