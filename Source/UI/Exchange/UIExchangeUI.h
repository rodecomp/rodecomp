#ifndef RAGNAROK_SOURCE_UI_EXCHANGE_UIEXCHANGEUI_H
#define RAGNAROK_SOURCE_UI_EXCHANGE_UIEXCHANGEUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <cstddef>

namespace ExchangeUI {
class CUIItem : public CUIListItem {
public:
  void Paint() override;
};

class CItemData {
public:
  /* 0000 */ ITEM_INFO item;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ bool unknown_00FC;
};

using CItemListBox = CUIListBoxEx<CUIItem, CItemData>;
}

class CUIExchangeUI : public CUIFrameWnd {
public:
  void OnCheckChanged(bool checked);
  void OnEditSubmit();
  void OnSelectFirstListItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);
  void OnSelectSecondListItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ExchangeUI::CItemData) == 0x100);
static_assert(offsetof(ExchangeUI::CItemData, unknown_00FC) == 0xFC);
static_assert(offsetof(ExchangeUI::CItemListBox, imageNames) == 0x90);
static_assert(offsetof(ExchangeUI::CItemListBox, disabledName) == 0x98);
static_assert(offsetof(ExchangeUI::CItemListBox, scrollBar) == 0xB0);
static_assert(offsetof(ExchangeUI::CItemListBox, data) == 0xD4);
static_assert(offsetof(ExchangeUI::CItemListBox, onSelect) == 0xE8);
static_assert(offsetof(ExchangeUI::CItemListBox, binder) == 0x110);
#endif

#endif
