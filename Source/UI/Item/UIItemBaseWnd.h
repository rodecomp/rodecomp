#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMBASEWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMBASEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <list>

struct UIButtonContext;

class UIItemBaseWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;

  ITEM_INFO GetItemByIndex(int index);
  ITEM_INFO GetItemAt(int x, int y);

  /* 00B4 */ int showQuantity;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ unsigned char unknown_00BC[0x10];

protected:
  UIButtonContext* m_controls;

public:
  /* 00D8 */ int scroll;
  /* 00DC */ int visibleRows;
  /* 00E0 */ int columns;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ std::list<ITEM_INFO> items;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemBaseWnd, scroll) == 0xD8);
static_assert(offsetof(UIItemBaseWnd, items) == 0xE8);
static_assert(sizeof(UIItemBaseWnd) == 0xF0);
#endif

#endif
