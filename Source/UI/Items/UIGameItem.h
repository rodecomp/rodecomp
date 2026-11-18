#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIGAMEITEM_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIGAMEITEM_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIControl.h"

#include <functional>
#include <string>

class CUIGameItem : public CUIControl {
public:
  inline CUIGameItem();

  /* 0080 */ int mode;
  /* 0084 */ unsigned char showNameTooltip;
  /* 0085 */ unsigned char allowItemInfo;
  /* 0086 */ unsigned char transparent;
  /* 0088 */ unsigned short scale;
  /* 0090 */ ITEM_INFO itemInfo;
  /* 0188 */ std::string name;
  /* 01A0 */ CResPtr<CBitmapRes> image;
  /* 01A4 */ unsigned char showOverlay;
  /* 01A8 */ CResPtr<CBitmapRes> overlay;
  /* 01B0 */ std::function<void(CUIGameItem*)> onClick;

  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  virtual void SetMode(int mode);

  ITEM_INFO* GetItemInfo();
  void ClearItem();
  void SetItemById(int itemId);
  void SetItem(const ITEM_INFO& item);
  void SetScale(short scale);
};

// OFFSET: 0x009F71F0
// FUNCTION: fn_14769
inline CUIGameItem::CUIGameItem()
    : CUIControl(), mode(0), showNameTooltip(1), allowItemInfo(1), transparent(0), scale(100), itemInfo(), name(), image(), showOverlay(0),
      overlay(), onClick() {
}

class CUIGameItemWithNum : public CUIGameItem {
public:
  void Paint() override;
  void SetMode(int mode) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIGameItem, itemInfo) == 0x90);
static_assert(offsetof(CUIGameItem, name) == 0x188);
static_assert(offsetof(CUIGameItem, image) == 0x1A0);
static_assert(offsetof(CUIGameItem, overlay) == 0x1A8);
static_assert(offsetof(CUIGameItem, onClick) == 0x1B0);
#endif

#endif
