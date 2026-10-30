#ifndef RAGNAROK_SOURCE_UI_RODEX_UIRODEXCOMPOSEWND_H
#define RAGNAROK_SOURCE_UI_RODEX_UIRODEXCOMPOSEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <list>

class UIRodexScrollBar : public UIScrollBar {
public:
  void Paint() override;
};

class UIRodexComposeWnd : public UIFrameWnd {
public:
  ITEM_INFO GetItemAtPoint(int x, int y);

  /* 00B4 */ unsigned char unknown_00B4[0x2C];
  /* 00E8 */ std::list<ITEM_INFO> m_items;
  /* 00F0 */ int m_itemIndex;
  /* 00F4 */ unsigned char unknown_00F4[0x10];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIRodexComposeWnd, m_items) == 0xE8);
static_assert(offsetof(UIRodexComposeWnd, m_itemIndex) == 0xF0);
#endif

#endif
