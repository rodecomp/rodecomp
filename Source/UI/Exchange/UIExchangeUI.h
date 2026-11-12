#ifndef RAGNAROK_SOURCE_UI_EXCHANGE_UIEXCHANGEUI_H
#define RAGNAROK_SOURCE_UI_EXCHANGE_UIEXCHANGEUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Window/FrameSkin.h"
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
  /* 00E8 */ CUIStaticImage* slotBackground;
  /* 00EC */ CUIGameItem* itemIcon;
  /* 00F0 */ CUIStaticText* quantityText;
  /* 00F4 */ CUIStaticText* itemNameText;

  CUIItem();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnInit() override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
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
  /* 00DC */ CUIStaticText* partnerText;
  /* 00E0 */ CUIStaticImage* backgroundImage;
  /* 00E4 */ ExchangeUI::CItemListBox* firstList;
  /* 00E8 */ CUIStaticText* ownZenyHighlight;
  /* 00EC */ CUIStaticText* ownZenyText;
  /* 00F0 */ CUIStaticText* ownZenyShadow;
  /* 00F4 */ CUIEditBoxEx* zenyEdit;
  /* 00F8 */ ExchangeUI::CItemListBox* secondList;
  /* 00FC */ CUIStaticText* partnerZenyHighlight;
  /* 0100 */ CUIStaticText* partnerZenyText;
  /* 0104 */ CUIStaticText* partnerZenyShadow;
  /* 0108 */ CUICheckBox* logCheckBox;
  /* 010C */ CUIStaticText* screenshotLabel;
  /* 0110 */ CUIImageButton* lockButton;
  /* 0114 */ CUIImageButton* tradeButton;
  /* 0118 */ CUIImageButton* cancelButton;
  /* 011C */ std::string partnerName;
  /* 0134 */ std::string zenyText;

  __declspec(noinline) CUIExchangeUI();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void OnProcess() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual void Initialize();

  void Cancel();
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

template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::UpdateScrollBar();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Paint();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnWheel(int value);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnScroll(int position);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::AddNewItem();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Resize(int width, int height);

#endif
