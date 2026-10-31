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
class CUIStaticText;

namespace ExchangeUI {
class CUIItem : public CUIListItem {
public:
  /* 00E8 */ CUIStaticImage* unknown_00E8;
  /* 00EC */ CUIGameItem* itemIcon;
  /* 00F0 */ CUIStaticText* unknown_00F0;
  /* 00F4 */ CUIStaticText* unknown_00F4;

  CUIItem();
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot16() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
};

class CItemData {
public:
  CItemData(const ITEM_INFO& item, int quantity) : item(item), quantity(quantity), frozen(false) {
  }

  /* 0000 */ ITEM_INFO item;
  /* 00F8 */ int quantity;
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
  /* 00E8 */ CUIStaticText* unknown_00E8;
  /* 00EC */ CUIStaticText* unknown_00EC;
  /* 00F0 */ CUIStaticText* unknown_00F0;
  /* 00F4 */ CUIEditBoxEx* zenyEdit;
  /* 00F8 */ ExchangeUI::CItemListBox* secondList;
  /* 00FC */ CUIStaticText* unknown_00FC;
  /* 0100 */ CUIStaticText* unknown_0100;
  /* 0104 */ CUIStaticText* unknown_0104;
  /* 0108 */ CUICheckBox* logCheckBox;
  /* 010C */ CUIStaticText* unknown_010C;
  /* 0110 */ CUIImageButton* lockButton;
  /* 0114 */ CUIImageButton* tradeButton;
  /* 0118 */ UIWindow* unknown_0118;
  /* 011C */ std::string partnerName;
  /* 0134 */ std::string zenyText;

  __declspec(noinline) CUIExchangeUI();
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot16() override;
  void Slot19() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual void Slot53();

  void Conclude();
  void SubmitZeny();
  void FreezeItems(bool mine);
  void SetZeny(int amount);
  void UpdateZeny(bool mine);
  void BindItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);

  void OnCheckChanged(bool checked);
  void OnEditSubmit();
  void OnSelectFirstListItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);
  void OnSelectSecondListItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data);
};

extern int g_exchangeMyZeny;
extern int g_exchangeOtherZeny;

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
