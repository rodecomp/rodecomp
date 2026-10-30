#ifndef RAGNAROK_SOURCE_UI_EXCHANGE_UIEXCHANGEUI_H
#define RAGNAROK_SOURCE_UI_EXCHANGE_UIEXCHANGEUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/FrameSkin.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CUICheckBox;

namespace ExchangeUI {
class CUIItem : public CUIListItem {
public:
  /* 00E8 */ UIWindow* unknown_00E8;
  /* 00EC */ CUIGameItem* itemIcon;

  void Paint() override;
  void Slot16() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
};

class CItemData {
public:
  /* 0000 */ ITEM_INFO item;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ bool frozen;
};

using CItemListBox = CUIListBoxEx<CUIItem, CItemData>;
}

class CUIExchangeUI : public CUIFrameWnd {
public:
  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ CUIStaticText* unknown_00DC;
  /* 00E0 */ CUIStaticImage* unknown_00E0;
  /* 00E4 */ ExchangeUI::CItemListBox* firstList;
  /* 00E8 */ UIWindow* unknown_00E8;
  /* 00EC */ UIWindow* unknown_00EC;
  /* 00F0 */ UIWindow* unknown_00F0;
  /* 00F4 */ CUIEditBoxEx* zenyEdit;
  /* 00F8 */ ExchangeUI::CItemListBox* secondList;
  /* 00FC */ UIWindow* unknown_00FC;
  /* 0100 */ UIWindow* unknown_0100;
  /* 0104 */ UIWindow* unknown_0104;
  /* 0108 */ CUICheckBox* logCheckBox;
  /* 010C */ UIWindow* unknown_010C;
  /* 0110 */ CUIImageButton* lockButton;
  /* 0114 */ CUIImageButton* tradeButton;
  /* 0118 */ UIWindow* unknown_0118;
  /* 011C */ std::string partnerName;
  /* 0134 */ std::string zenyText;

  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot19() override;

  void Conclude();
  void SubmitZeny();
  void FreezeItems(bool mine);
  void SetZeny(int amount);

  void OnCheckChanged(bool checked);
  void OnEditSubmit();
  void OnSelectFirstListItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);
  void OnSelectSecondListItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ExchangeUI::CItemData) == 0x100);
static_assert(offsetof(ExchangeUI::CItemData, frozen) == 0xFC);
static_assert(offsetof(ExchangeUI::CItemListBox, imageNames) == 0x90);
static_assert(offsetof(ExchangeUI::CItemListBox, disabledName) == 0x98);
static_assert(offsetof(ExchangeUI::CItemListBox, scrollBar) == 0xB0);
static_assert(offsetof(ExchangeUI::CItemListBox, data) == 0xD4);
static_assert(offsetof(ExchangeUI::CItemListBox, onSelect) == 0xE8);
static_assert(offsetof(ExchangeUI::CItemListBox, binder) == 0x110);
#endif

#endif
