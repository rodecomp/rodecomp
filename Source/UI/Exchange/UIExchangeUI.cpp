#include "UI/Exchange/UIExchangeUI.h"

#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <functional>
#include <string>
#include <windows.h>

template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetOnSelect(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetBinder(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetImages(const std::string&, const std::string&, const std::string&,
                                                                                    const std::string&);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::UpdateScrollBar();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Paint();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Slot35(int value);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnScroll(int position);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::AddNewItem();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Slot53(int resolveSkin);

// GLOBAL: gl_25004
int g_exchangeMyZeny;

// GLOBAL: gl_25005
int g_exchangeOtherZeny;

// OFFSET: 0x00A0F790
// FUNCTION: fn_15176
bool UnusedTrueA0F790() {
  return true;
}

// OFFSET: 0x00A0F7A0
// FUNCTION: fn_15177
bool UnusedTrueA0F7A0() {
  return true;
}

// OFFSET: 0x00A0F7B0
// FUNCTION: fn_15178
bool UnusedTrueA0F7B0() {
  return true;
}

// OFFSET: 0x00A0F840
// FUNCTION: fn_15180
template <>
CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A0FA00
// FUNCTION: fn_15181
template <>
CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::~CUIListBoxEx() {
}

// OFFSET: 0x00A100B0
// FUNCTION: fn_15188
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::ClearItems() {
  for (ExchangeUI::CUIItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A10140
// FUNCTION: fn_15189
void CUIExchangeUI::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(560, 377);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A101E0
// FUNCTION: fn_15190
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A15150
// FUNCTION: fn_15193
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A15160
// FUNCTION: fn_15194
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A157C0
// FUNCTION: fn_15200
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnScroll(int position) {
  auto it = this->items.begin();
  if (it == this->items.end()) {
    return;
  }
  if (this->data.size() == 0) {
    for (; it != this->items.end(); ++it) {
      (*it)->index = -1;
      (*it)->Show(0);
    }
    return;
  }

  const int first = (*it)->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      if (first - row < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - (first - row), this->items.end());
      }
    }
  }

  it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || row >= this->data.size()) {
      break;
    }
    ExchangeUI::CUIItem* item = *it;
    if (item) {
      if (this->rebind || row != static_cast<unsigned int>(item->index)) {
        item->index = row;
        if (this->binder) {
          auto node = this->data.begin();
          std::advance(node, static_cast<int>(row));
          this->binder(item, *node);
        }
      }
      item->SetSelected(row == this->selected);
      item->Move(this->itemX, y);
      item->Show(1);
    }
  }

  for (; it != this->items.end(); ++it) {
    (*it)->index = -1;
    (*it)->Show(0);
  }
}

// OFFSET: 0x00A171A0
// FUNCTION: fn_15286
template <>
int CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::AddData(const ExchangeUI::CItemData& value, bool refresh) {
  this->data.push_back(value);
  if (refresh) {
    this->UpdateScrollBar();
  }
  return this->data.size() - 1;
}

// OFFSET: 0x00A172E0
// FUNCTION: fn_45474
void CUIExchangeUI::Cancel() {
  CMode* mode = g_modeMgr.GetGameMode();
  if (!mode) {
    return;
  }

  mode->SendMsg(0x35, 0, 0, 0, 0);
  this->unknown_0118->disabled = 1;
  this->unknown_0118->Invalidate();
}

// OFFSET: 0x00A17320
// FUNCTION: fn_15287
void CUIExchangeUI::Conclude() {
  CMode* mode = g_modeMgr.GetGameMode();
  if (!mode) {
    return;
  }

  if (this->logCheckBox->checked) {
    std::string text = FormatOwnedString(LookupMessage(0x728), this->partnerName.c_str());
    g_modeMgr.gameMode->SendMsg(0x44, reinterpret_cast<int>(text.c_str()), 0, 0, 0);
  }

  mode->SendMsg(0x36, 0, 0, 0, 0);
  this->tradeButton->disabled = 1;
  this->tradeButton->Invalidate();
}

// OFFSET: 0x00A17420
// FUNCTION: fn_15288
void CUIExchangeUI::SubmitZeny() {
  CMode* mode = g_modeMgr.GetGameMode();
  if (!mode) {
    return;
  }

  char text[24];
  _snprintf_s(text, sizeof(text), "%s", this->zenyEdit->edit->Text());
  if (g_session.IsNumberWithCommas(text)) {
    int amount = g_session.m_zeny;
    const int value = std::atoi(text);
    if (value <= amount) {
      amount = value < 0 ? 0 : value;
    }

    mode->SendMsg(0x33, 0, g_session.unknown_5210 + amount, 0, 0);
    mode->SendMsg(0x34, 0, 0, 0, 0);
    this->lockButton->disabled = 1;
    this->lockButton->Invalidate();
  } else {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x25A), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00A17530
// FUNCTION: fn_15289
void CUIExchangeUI::OnCheckChanged(bool) {
}

// OFFSET: 0x00A17540
// FUNCTION: fn_15290
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::ClearData() {
  this->data.clear();
  this->selected = static_cast<unsigned int>(-1);
  this->UpdateScrollBar();
}

// OFFSET: 0x00A175C0
// FUNCTION: fn_15291
void CUIExchangeUI::Slot10(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  if (this->firstList->unknown_00C4) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 1) {
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    } else {
      g_windowMgr.ShowWindowById(15, 1);
      if (g_windowMgr.window_01EC != nullptr) {
        g_windowMgr.window_01EC->SendMsg(nullptr, 13, 5, 0, 0, 0);
      }
    }
  } else {
    mode->SendMsg(18, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A17670
// FUNCTION: fn_15292
void CUIExchangeUI::OnEditSubmit() {
}

// OFFSET: 0x00A17680
// FUNCTION: fn_15293
void CUIExchangeUI::OnSelectFirstListItem(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&) {
}

// OFFSET: 0x00A176A0
// FUNCTION: fn_15295
void CUIExchangeUI::OnSelectSecondListItem(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&) {
}

// OFFSET: 0x00A176C0
// FUNCTION: fn_15297
void ExchangeUI::CUIItem::Paint() {
  if (this->frozen) {
    unsigned red = 0;
    unsigned green = 0;
    unsigned blue = 0;
    SkinPaletteChannels(ResourceMgr::Instance(), 10, 6, red, green, blue);
    this->ClearSurface(RGB(red, green, blue));
  } else {
    this->ClearSurface(0xFFFFFFFF);
  }
}

// OFFSET: 0x00A17730
// FUNCTION: fn_15298
void CUIExchangeUI::Slot16() {
  this->Move((g_renderer->viewport.width - 640) / 2 + 100, g_renderer->viewport.height * 100 / 480);

  const std::string title = std::format("{} : ", LookupMessage(0xCE));
  this->titleBar->SetTitle(title.c_str(), title.size());

  const char* logText = LookupMessage(0x727);
  this->unknown_010C->SetText(logText, std::strlen(logText));

  for (int count = 10; count != 0; --count) {
    this->firstList->AddData(ExchangeUI::CItemData(ITEM_INFO(), 0), false);
  }
  this->firstList->UpdateScrollBar();

  this->zenyEdit->SetText(std::to_string(0));
  this->SetZeny(0);

  for (int count = 10; count != 0; --count) {
    this->secondList->AddData(ExchangeUI::CItemData(ITEM_INFO(), 0), false);
  }
  this->secondList->UpdateScrollBar();

  this->tradeButton->disabled = 1;
  this->tradeButton->Invalidate();
}

// OFFSET: 0x00A17BD0
// FUNCTION: fn_15299
void ExchangeUI::CUIItem::Slot16() {
  this->itemIcon->showOverlay = 1;
  this->itemIcon->Invalidate();
}

// OFFSET: 0x00A17BF0
// FUNCTION: fn_15300
void ExchangeUI::CUIItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A17C20
// FUNCTION: fn_15301
void CUIExchangeUI::Slot19() {
  if (this->zenyEdit->Slot06() != g_windowMgr.GetHoverWindow()) {
    return;
  }

  char text[24];
  _snprintf_s(text, sizeof(text), "%s", this->zenyEdit->edit->Text());
  if (g_session.IsNumberWithCommas(text)) {
    int amount = std::atoi(text);
    if (amount < 0) {
      amount = 0;
    }

    if (amount > 1000000000) {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x9A3), 2, true, false, 0, "Message", 280, 120, 0);
      FormatGroupedInteger(1000000000, text, sizeof(text));
      this->zenyEdit->SetText(text);
      this->SetZeny(1000000000);
    } else {
      FormatGroupedInteger(amount, text, sizeof(text));
      this->zenyEdit->SetText(text);
      this->SetZeny(amount);
      this->zenyText = text;
    }

    this->zenyEdit->SetCursor(this->zenyEdit->Length());
  } else {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x25A), 0, true, false, 0, "Message", 280, 120, 0);
    this->zenyEdit->SetText(this->zenyText);
  }
}

// OFFSET: 0x00A17F30
// FUNCTION: fn_15302
void ExchangeUI::CUIItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A17F60
// FUNCTION: fn_15303
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::UpdateScrollBar() {
  this->scrollBar->SetTotal(this->data.size() * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A17FB0
// FUNCTION: fn_15304
int CUIExchangeUI::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 0x17:
    this->firstList->ClearData();
    for (int index = 0; index < 10; ++index) {
      ITEM_INFO item = g_session.GetExchangeItem(index);
      ExchangeUI::CItemListBox* list = this->firstList;
      const int quantity = item.quantity;
      if (quantity > 0) {
        list->AddData(ExchangeUI::CItemData(item, quantity), false);
      } else {
        list->AddData(ExchangeUI::CItemData(ITEM_INFO(), 0), false);
      }
    }
    this->firstList->UpdateScrollBar();

    this->secondList->ClearData();
    for (int index = 0; index < 10; ++index) {
      ITEM_INFO item = g_session.GetItemSlot(index);
      ExchangeUI::CItemListBox* list = this->secondList;
      const int quantity = item.quantity;
      if (quantity > 0) {
        list->AddData(ExchangeUI::CItemData(item, quantity), false);
      } else {
        list->AddData(ExchangeUI::CItemData(ITEM_INFO(), 0), false);
      }
    }
    this->secondList->UpdateScrollBar();
    break;
  case 0x26: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode != nullptr) {
      mode->SendMsg(0x33, mode->drag.id, mode->drag.quantity, 0, 0);
      mode->SendMsg(0x12, 0, 0, 0, 0);
    }
    break;
  }
  case 0x32:
    if (argument1 == 0) {
      break;
    }

    this->partnerName = reinterpret_cast<const char*>(argument1);
    if (argument2 > 0) {
      if (g_serviceType == 6) {
        const std::string text = std::format("{} Lv{}", this->partnerName, static_cast<int>(argument3));
        this->unknown_00DC->SetText(text.c_str(), text.size());
      } else {
        const std::string job = ShortcutFilename(argument2);
        const std::string text = std::format("{} Lv{} ({})", this->partnerName, static_cast<int>(argument3), job);
        this->unknown_00DC->SetText(text.c_str(), text.size());
      }
    } else {
      this->unknown_00DC->SetText(this->partnerName.c_str(), this->partnerName.size());
    }
    break;
  case 0x33:
    this->zenyEdit->Show(0);
    this->FreezeItems(true);
    this->UpdateZeny(true);
    if (this->firstList->unknown_00C4 && this->secondList->unknown_00C4) {
      this->tradeButton->disabled = 0;
    } else {
      this->tradeButton->disabled = 1;
    }
    this->tradeButton->Invalidate();
    break;
  case 0x34:
    this->FreezeItems(false);
    this->UpdateZeny(false);
    if (this->firstList->unknown_00C4 && this->secondList->unknown_00C4) {
      this->tradeButton->disabled = 0;
    } else {
      this->tradeButton->disabled = 1;
    }
    this->tradeButton->Invalidate();
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x00A18660
// FUNCTION: fn_15305
void CUIExchangeUI::BindItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data) {
  const int quantity = data.item.quantity;
  const bool visible = quantity > 0;
  item->itemIcon->Show(visible);
  item->unknown_00F0->Show(visible);
  item->unknown_00F4->Show(visible);
  item->itemIcon->SetItem(data.item);

  const std::string count = quantity > 0 ? std::to_string(data.item.quantity) : std::string();
  item->unknown_00F0->SetText(count.c_str(), count.size());

  const std::string name = quantity > 0 ? data.item.DisplayName(0) : std::string();
  item->unknown_00F4->SetText(name.c_str(), name.size());
  item->unknown_00F4->SetTooltip("", 0);

  if (data.frozen) {
    item->SetFrozen(true);
    item->unknown_00E8->Show(0);
  }
}

// OFFSET: 0x00A18940
// FUNCTION: fn_15306
void CUIExchangeUI::SetZeny(int amount) {
  const PriceColors colors = PriceColor(amount);
  this->zenyEdit->SetTextColor(colors.primary);
  if (colors.secondary == static_cast<std::uint32_t>(-1)) {
    this->zenyEdit->ResetSelection();
  } else {
    this->zenyEdit->SetHighlight(colors.secondary);
  }
}

// OFFSET: 0x00A18990
// FUNCTION: fn_15307
void CUIExchangeUI::UpdateZeny(bool mine) {
  CUIStaticText* first;
  CUIStaticText* second;
  CUIStaticText* third;
  int zeny;
  if (mine) {
    first = this->unknown_00E8;
    second = this->unknown_00EC;
    third = this->unknown_00F0;
    zeny = g_exchangeMyZeny;
  } else {
    first = this->unknown_00FC;
    second = this->unknown_0100;
    third = this->unknown_0104;
    zeny = g_exchangeOtherZeny;
  }

  char text[24];
  FormatGroupedInteger(zeny, text, sizeof(text));
  first->SetText(text, std::strlen(text));
  second->SetText(text, std::strlen(text));
  third->SetText(text, std::strlen(text));

  const std::uint32_t color = PriceColor(zeny).primary;
  first->SetColor(color);
  second->SetColor(color);
}

// OFFSET: 0x00A18AA0
// FUNCTION: fn_15308
void CUIExchangeUI::FreezeItems(bool mine) {
  ExchangeUI::CItemListBox* list = mine ? this->firstList : this->secondList;
  for (ExchangeUI::CItemData& data : list->data) {
    data.frozen = true;
  }
  list->UpdateScrollBar();
  list->unknown_00C4 = 1;
  list->Invalidate();
}

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

// OFFSET: 0x00A149B0
// FUNCTION: fn_15192
void ExchangeUI::CUIItem::Slot15(int width, int height) {
  this->unknown_00E8 = new CUIStaticImage(*this->m_context);
  this->unknown_00E8->SetSize(32, 32);
  this->unknown_00E8->Move(9, 0);
  const std::uint8_t backgroundSource[] = {0x4F, 0x12, 0x2F, 0x7D, 0xBD, 0x0B, 0x68, 0xB8, 0x49, 0x59, 0x2F, 0x4B, 0xC0, 0x7F, 0xF1, 0x17,
                                           0xEE, 0xCE, 0x86, 0xE4, 0x22, 0xAC, 0xC3, 0x01, 0xEA, 0xCF, 0x89, 0xE6, 0x1E, 0xA0, 0xF1, 0x1C,
                                           0xFB, 0xD8, 0x82, 0xF0, 0x14, 0xAB, 0xF2, 0x18, 0xE6, 0xD9, 0xC1, 0xE5, 0x10, 0xB5, 0xAD};
  thread_local EncryptedName<sizeof(backgroundSource), 0x75ADC57D87EFBD8FULL> backgroundName(backgroundSource);
  std::string background(backgroundName.Text());
  this->unknown_00E8->SetImage(background);
  this->AddChild(this->unknown_00E8);

  this->itemIcon = new CUIGameItem(*this->m_context);
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(13, 1);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->AddChild(this->itemIcon);

  this->unknown_00F0 = new CUIStaticText(*this->m_context);
  this->unknown_00F0->SetSize(100, 13);
  this->unknown_00F0->Move(25, 15);
  this->unknown_00F0->SetText("", 0);
  this->unknown_00F0->SetFitText(0);
  this->unknown_00F0->SetAlign(0, 6);
  const std::uint8_t nameFontSource[] = {0x2A, 0xCE, 0xF9, 0x44, 0xFE, 0xFB};
  thread_local EncryptedName<sizeof(nameFontSource), 0xC517FB932D95BB6DULL> nameFont(nameFontSource);
  this->unknown_00F0->SetFont(std::string(nameFont.Text()));
  this->unknown_00F0->SetFontHeight(11);
  this->unknown_00F0->SetBold(0);
  this->unknown_00F0->SetColor(0);
  this->unknown_00F0->SetShadow(0, 0xFFFFFF);
  this->unknown_00F0->SetOutline(1, 0xFFFFFF);
  this->AddChild(this->unknown_00F0);

  this->unknown_00F4 = new CUIStaticText(*this->m_context);
  this->unknown_00F4->SetSize(215, 30);
  this->unknown_00F4->Move(45, 0);
  this->unknown_00F4->SetText("", 0);
  this->unknown_00F4->SetFitText(1);
  this->unknown_00F4->SetAlign(0, 6);
  const std::uint8_t countFontSource[] = {0x8E, 0x80, 0x33, 0xE2, 0xE2, 0xD5};
  thread_local EncryptedName<sizeof(countFontSource), 0x23BBD58F8B5FF5C9ULL> countFont(countFontSource);
  this->unknown_00F4->SetFont(std::string(countFont.Text()));
  this->unknown_00F4->SetFontHeight(12);
  this->unknown_00F4->SetBold(0);
  this->unknown_00F4->SetColor(0);
  this->unknown_00F4->SetShadow(0, 0xFFFFFF);
  this->unknown_00F4->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00F4);
}

// OFFSET: 0x00A103F0
// FUNCTION: fn_15191
void CUIExchangeUI::Slot15(int width, int height) {
  this->titleBar = new CUITitleBar(*this->m_context);
  this->titleBar->SetSize(560, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    const std::uint8_t titleRightSource[] = {
      0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x75,
      0x56, 0x34, 0xC4, 0x5A, 0xE0, 0x8C, 0xC7, 0x63, 0x52, 0x35, 0xCB, 0x58, 0xDC, 0x80, 0xF5, 0x63,
      0x5E, 0x33, 0xC1, 0x5C, 0xDD, 0x84, 0xDB, 0x48, 0x45, 0x2E, 0xCA, 0x51, 0xCB, 0xCB, 0xCB, 0x7A,
      0x47, 0x47
    };
    // GLOBAL: gl_25394
    thread_local EncryptedName<sizeof(titleRightSource), 0x17A9E5BF39AD4737ULL> titleRightName(titleRightSource);
    const std::string titleRight(titleRightName.Text());
    const std::uint8_t titleMiddleSource[] = {
      0x65, 0x32, 0xAD, 0x03, 0xA3, 0xE9, 0xB2, 0xCE, 0x63, 0x79, 0xAD, 0x35, 0xDE, 0x9D, 0x2B, 0x61,
      0xC4, 0xEE, 0x04, 0x9A, 0x3C, 0x4E, 0x19, 0x77, 0xC0, 0xEF, 0x0B, 0x98, 0x00, 0x42, 0x2B, 0x77,
      0xCC, 0xE9, 0x01, 0x9C, 0x01, 0x46, 0x05, 0x5C, 0xC8, 0xF4, 0x09, 0xD7, 0x01, 0x4A, 0x07, 0x03
    };
    // GLOBAL: gl_25395
    thread_local EncryptedName<sizeof(titleMiddleSource), 0x03772763F96D9DA5ULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const std::uint8_t titleLeftSource[] = {
      0x47, 0xBC, 0xAB, 0x85, 0xF9, 0xC7, 0xB4, 0xC6, 0x41, 0xF7, 0xAB, 0xB3, 0x84, 0xB3, 0x2D, 0x69,
      0xE6, 0x60, 0x02, 0x1C, 0x66, 0x60, 0x1F, 0x7F, 0xE2, 0x61, 0x0D, 0x1E, 0x5A, 0x6C, 0x2D, 0x7F,
      0xEE, 0x67, 0x07, 0x1A, 0x5B, 0x68, 0x03, 0x54, 0xEB, 0x76, 0x0D, 0x0B, 0x17, 0x6B, 0x1C, 0x7B,
      0x87
    };
    // GLOBAL: gl_25396
    thread_local EncryptedName<sizeof(titleLeftSource), 0x0B7109397F6B1387ULL> titleLeftName(titleLeftSource);
    const std::string titleLeft(titleLeftName.Text());
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 17);
  title->Move(12, 0);
  title->SetText("", 0);
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const std::uint8_t titleFontSource[] = {
      0x20, 0xDE, 0xFD, 0xCC, 0xCC, 0x3F
    };
    // GLOBAL: gl_25397
    thread_local EncryptedName<sizeof(titleFontSource), 0x13E33FA1A591AB67ULL> titleFontName(titleFontSource);
    title->SetFont(std::string(titleFontName.Text()));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  this->unknown_00DC = new CUIStaticText(*this->m_context);
  this->unknown_00DC->SetSize(495, 17);
  this->unknown_00DC->Move(62, 0);
  this->unknown_00DC->SetText("", 0);
  this->unknown_00DC->SetFitText(1);
  this->unknown_00DC->SetAlign(0, 6);
  {
    CUIStaticText* partner = this->unknown_00DC;
    const std::uint8_t partnerFontSource[] = {
      0x28, 0x08, 0x97, 0x4A, 0x1C, 0x07
    };
    // GLOBAL: gl_25398
    thread_local EncryptedName<sizeof(partnerFontSource), 0xCD8F077123FB7D6FULL> partnerFontName(partnerFontSource);
    partner->SetFont(std::string(partnerFontName.Text()));
  }
  this->unknown_00DC->SetFontHeight(12);
  this->unknown_00DC->SetBold(0);
  this->unknown_00DC->SetColor(0);
  this->unknown_00DC->SetShadow(1, 0xB4B4B4);
  this->unknown_00DC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00DC);

  this->unknown_00E0 = new CUIStaticImage(*this->m_context);
  this->unknown_00E0->SetSize(560, 360);
  this->unknown_00E0->Move(0, 17);
  {
    CUIStaticImage* background = this->unknown_00E0;
    const std::uint8_t backgroundSource[] = {
      0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xFB,
      0x22, 0x5A, 0x3A, 0x44, 0x8A, 0xD6, 0x27, 0xED, 0x26, 0x5B, 0x35, 0x46, 0xB6, 0xDA, 0x15, 0xFC,
      0x3B, 0x4A, 0x3B, 0x46, 0xBB, 0xD8, 0x2C, 0xC6, 0x21, 0x4E, 0x61, 0x09, 0xB7, 0xD2, 0x39, 0x99
    };
    // GLOBAL: gl_25399
    thread_local EncryptedName<sizeof(backgroundSource), 0x9949BFD527532943ULL> backgroundName(backgroundSource);
    background->SetImage(std::string(backgroundName.Text()));
  }
  this->AddChild(this->unknown_00E0);

  this->firstList = new ExchangeUI::CItemListBox(*this->m_context);
  this->firstList->itemWidth = 272;
  this->firstList->rowHeight = 30;
  this->firstList->SetSize(272, 300);
  this->firstList->Move(4, 21);
  this->firstList->SetOnSelect([this](ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data) { this->OnSelectFirstListItem(item, data); });
  this->firstList->SetBinder([this](ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data) { this->BindItem(item, data); });
  this->firstList->SetImages("", "", "", "");
  this->firstList->itemX = 0;
  this->firstList->itemY = 0;
  this->firstList->scrollBar->SetSize(13, 300);
  this->firstList->scrollBar->Move(259, 0);
  this->firstList->scrollBar->step = 30;
  {
    const std::uint8_t downSource[] = {
      0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21, 0xAA,
      0x6A, 0x55, 0x40, 0xD7, 0xD1, 0x6D, 0x19, 0xB6, 0x7E, 0x49, 0x01, 0xD9, 0xD0, 0x2D, 0x7D
    };
    // GLOBAL: gl_25400
    thread_local EncryptedName<sizeof(downSource), 0xD97D5DBDBB2F2709ULL> downName(downSource);
    const std::string down(downName.Text());
    const std::uint8_t upSource[] = {
      0xBF, 0xBC, 0x1F, 0x2B, 0x45, 0xA9, 0xBC, 0x20, 0xB9, 0xF7, 0x1F, 0x1D, 0x38, 0xDD, 0x25, 0x9E,
      0x1C, 0x61, 0xB0, 0xBD, 0xE9, 0x57, 0x0C, 0x9D, 0x51, 0x71, 0xB2, 0xA1, 0x85
    };
    // GLOBAL: gl_25401
    thread_local EncryptedName<sizeof(upSource), 0xED796785D1DF137FULL> upName(upSource);
    const std::string up(upName.Text());
    this->firstList->scrollBar->SetButtonImages(up, down);
  }
  {
    const std::uint8_t backgroundSource[] = {
      0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xD6,
      0xFC, 0xB7, 0x02, 0xC7, 0xBB, 0x19, 0x90, 0xCC, 0xFB, 0xEB, 0x0F, 0xC6, 0xA7, 0x29
    };
    // GLOBAL: gl_25402
    thread_local EncryptedName<sizeof(backgroundSource), 0xA5FD29D7AB6DC59FULL> backgroundName(backgroundSource);
    this->firstList->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const std::uint8_t thumbBottomSource[] = {
      0x21, 0xD4, 0x39, 0x2D, 0x15, 0xDD, 0xFC, 0x10, 0x27, 0x9F, 0x39, 0x1B, 0x68, 0xA9, 0x65, 0xAE,
      0x82, 0x09, 0x96, 0xBB, 0xB9, 0x23, 0x5B, 0xBC, 0x93, 0x24, 0x9D, 0xB8, 0xA2, 0x7D, 0x17, 0xBF,
      0x8C, 0x0B, 0xF9
    };
    // GLOBAL: gl_25403
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xDD3913D5D7F97BE1ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
      0x5B, 0xF8, 0xC3, 0x63, 0xA7, 0xCB, 0xC0, 0xC8, 0x5D, 0xB3, 0xC3, 0x55, 0xDA, 0xBF, 0x59, 0x76,
      0xF8, 0x25, 0x6C, 0xF5, 0x0B, 0x35, 0x67, 0x64, 0xE9, 0x08, 0x6E, 0xF0, 0x03, 0x2B, 0x67, 0x68,
      0xEB, 0x57
    };
    // GLOBAL: gl_25404
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x050505679903579BULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
      0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x82,
      0x92, 0xB5, 0x56, 0x65, 0x7B, 0x49, 0x2D, 0x90, 0x83, 0x98, 0x4C, 0x79, 0x39, 0x1B, 0x22, 0x81,
      0xF1
    };
    // GLOBAL: gl_25405
    thread_local EncryptedName<sizeof(thumbTopSource), 0xF14F79170939C7F1ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->firstList->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->firstList->AddNewItem();
  this->AddChild(this->firstList);

  this->unknown_00E8 = new CUIStaticText(*this->m_context);
  this->unknown_00E8->SetSize(150, 18);
  this->unknown_00E8->Move(69, 330);
  {
    const std::uint8_t zenySource[] = {
      0x97, 0xF5
    };
    // GLOBAL: gl_25406
    thread_local EncryptedName<sizeof(zenySource), 0xBF2B4B31397FF5A7ULL> zenyName(zenySource);
    const char* zeny = zenyName.Text();
    this->unknown_00E8->SetText(zeny, std::strlen(zeny));
  }
  this->unknown_00E8->SetFitText(0);
  this->unknown_00E8->SetAlign(2, 6);
  {
    CUIStaticText* text = this->unknown_00E8;
    const std::uint8_t zenyFontSource[] = {
      0xFC, 0xCF, 0x84, 0x56, 0xD7, 0xBF
    };
    // GLOBAL: gl_25407
    thread_local EncryptedName<sizeof(zenyFontSource), 0x83EFBFBB37EDBDBDULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->unknown_00E8->SetFontHeight(16);
  this->unknown_00E8->SetBold(0);
  this->unknown_00E8->SetColor(0xFFFF00);
  this->unknown_00E8->SetShadow(0, 0xFFFFFF);
  this->unknown_00E8->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = new CUIStaticText(*this->m_context);
  this->unknown_00EC->SetSize(150, 18);
  this->unknown_00EC->Move(70, 330);
  {
    const std::uint8_t zenySource[] = {
      0xBF, 0x99
    };
    // GLOBAL: gl_25408
    thread_local EncryptedName<sizeof(zenySource), 0x39FF7FB50DEB998FULL> zenyName(zenySource);
    const char* zeny = zenyName.Text();
    this->unknown_00EC->SetText(zeny, std::strlen(zeny));
  }
  this->unknown_00EC->SetFitText(0);
  this->unknown_00EC->SetAlign(2, 6);
  {
    CUIStaticText* text = this->unknown_00EC;
    const std::uint8_t zenyFontSource[] = {
      0xD6, 0x83, 0x88, 0xBC, 0xBD, 0x93
    };
    // GLOBAL: gl_25409
    thread_local EncryptedName<sizeof(zenyFontSource), 0x2FE993D1DDE1F197ULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->unknown_00EC->SetFontHeight(16);
  this->unknown_00EC->SetBold(0);
  this->unknown_00EC->SetColor(0xFFFF00);
  this->unknown_00EC->SetShadow(0, 0xFFFFFF);
  this->unknown_00EC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00EC);

  this->unknown_00F0 = new CUIStaticText(*this->m_context);
  this->unknown_00F0->SetSize(150, 18);
  this->unknown_00F0->Move(70, 331);
  {
    const std::uint8_t zenySource[] = {
      0xFD, 0xF1
    };
    // GLOBAL: gl_25410
    thread_local EncryptedName<sizeof(zenySource), 0xAD075B8DB599F1CDULL> zenyName(zenySource);
    const char* zeny = zenyName.Text();
    this->unknown_00F0->SetText(zeny, std::strlen(zeny));
  }
  this->unknown_00F0->SetFitText(0);
  this->unknown_00F0->SetAlign(2, 6);
  {
    CUIStaticText* text = this->unknown_00F0;
    const std::uint8_t zenyFontSource[] = {
      0x3E, 0x67, 0xE4, 0x70, 0x5B, 0x0F
    };
    // GLOBAL: gl_25411
    thread_local EncryptedName<sizeof(zenyFontSource), 0x2F8B0F37118D157FULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->unknown_00F0->SetFontHeight(16);
  this->unknown_00F0->SetBold(0);
  this->unknown_00F0->SetColor(0x323232);
  this->unknown_00F0->SetShadow(0, 0xFFFFFF);
  this->unknown_00F0->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00F0);

  this->zenyEdit = new CUIEditBoxEx(*this->m_context);
  this->zenyEdit->Create(100, 16, false);
  this->zenyEdit->Move(140, 330);
  this->zenyEdit->SetText(std::string(""));
  this->zenyEdit->SetTextColor(0);
  this->zenyEdit->SetBackgroundColor(0xE8E8E8);
  this->zenyEdit->SetBorderColor(0xC6C6C6);
  this->zenyEdit->SetBorderWidth(1);
  this->zenyEdit->SetMaximum(13);
  this->zenyEdit->SetEditBounds(0, 0, 0, 0);
  this->zenyEdit->SetMask(0);
  this->zenyEdit->SetAlignment(0);
  this->zenyEdit->SetOnSubmit([this]() { this->OnEditSubmit(); });
  this->AddChild(this->zenyEdit);

  this->secondList = new ExchangeUI::CItemListBox(*this->m_context);
  this->secondList->itemWidth = 272;
  this->secondList->rowHeight = 30;
  this->secondList->SetSize(272, 300);
  this->secondList->Move(283, 21);
  this->secondList->SetOnSelect([this](ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data) { this->OnSelectSecondListItem(item, data); });
  this->secondList->SetBinder([this](ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data) { this->BindItem(item, data); });
  this->secondList->SetImages("", "", "", "");
  this->secondList->itemX = 0;
  this->secondList->itemY = 0;
  this->secondList->scrollBar->SetSize(13, 300);
  this->secondList->scrollBar->Move(259, 0);
  this->secondList->scrollBar->step = 30;
  {
    const std::uint8_t downSource[] = {
      0x3D, 0x16, 0x7B, 0xDB, 0x5F, 0x33, 0x84, 0x84, 0x3B, 0x5D, 0x7B, 0xED, 0x22, 0x47, 0x1D, 0x3A,
      0x9E, 0xCB, 0xD4, 0x4D, 0xF3, 0xCD, 0x25, 0x26, 0x8A, 0xD7, 0x95, 0x43, 0xF2, 0x8D, 0x41
    };
    // GLOBAL: gl_25412
    thread_local EncryptedName<sizeof(downSource), 0x4941FD9F21BBB9FDULL> downName(downSource);
    const std::string down(downName.Text());
    const std::uint8_t upSource[] = {
      0xB5, 0x62, 0x2D, 0xA7, 0xAB, 0xAB, 0x20, 0xF8, 0xB3, 0x29, 0x2D, 0x91, 0xD6, 0xDF, 0xB9, 0x46,
      0x16, 0xBF, 0x82, 0x31, 0x07, 0x55, 0x90, 0x45, 0x5B, 0xAF, 0x80, 0x2D, 0x6B
    };
    // GLOBAL: gl_25413
    thread_local EncryptedName<sizeof(upSource), 0x35E5656B5DEDCD75ULL> upName(upSource);
    const std::string up(upName.Text());
    this->secondList->scrollBar->SetButtonImages(up, down);
  }
  {
    const std::uint8_t backgroundSource[] = {
      0x3D, 0x78, 0x8D, 0x53, 0x75, 0x41, 0x52, 0x2A, 0x3B, 0x33, 0x8D, 0x65, 0x08, 0x35, 0xCB, 0x94,
      0x9E, 0xA5, 0x22, 0xC5, 0xD9, 0xBF, 0xFA, 0x8E, 0x99, 0xF9, 0x2F, 0xC4, 0xC5, 0x8F
    };
    // GLOBAL: gl_25414
    thread_local EncryptedName<sizeof(backgroundSource), 0xE7978FB5A94DD7FDULL> backgroundName(backgroundSource);
    this->secondList->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const std::uint8_t thumbBottomSource[] = {
      0x09, 0xBC, 0x39, 0x89, 0x39, 0x17, 0xD0, 0xBC, 0x0F, 0xF7, 0x39, 0xBF, 0x44, 0x63, 0x49, 0x02,
      0xAA, 0x61, 0x96, 0x1F, 0x95, 0xE9, 0x77, 0x10, 0xBB, 0x4C, 0x9D, 0x1C, 0x8E, 0xB7, 0x3B, 0x13,
      0xA4, 0x63, 0xF9
    };
    // GLOBAL: gl_25415
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x7115D9F973F913C9ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
      0xA3, 0x5A, 0x21, 0x85, 0xFB, 0xEB, 0xD8, 0x06, 0xA5, 0x11, 0x21, 0xB3, 0x86, 0x9F, 0x41, 0xB8,
      0x00, 0x87, 0x8E, 0x13, 0x57, 0x15, 0x7F, 0xAA, 0x11, 0xAA, 0x8C, 0x16, 0x5F, 0x0B, 0x7F, 0xA6,
      0x13, 0xF5
    };
    // GLOBAL: gl_25416
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xCB1D253B7FE1F563ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
      0xAB, 0x16, 0x07, 0xF5, 0xA7, 0x41, 0x38, 0x7A, 0xAD, 0x5D, 0x07, 0xC3, 0xDA, 0x35, 0xA1, 0xC4,
      0x08, 0xCB, 0xA8, 0x63, 0x0B, 0xBF, 0x9F, 0xD6, 0x19, 0xE6, 0xB2, 0x7F, 0x49, 0xED, 0x90, 0xC7,
      0x6B
    };
    // GLOBAL: gl_25417
    thread_local EncryptedName<sizeof(thumbTopSource), 0xB7FD8F670FC7B96BULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->secondList->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->secondList->AddNewItem();
  this->AddChild(this->secondList);

  this->unknown_00FC = new CUIStaticText(*this->m_context);
  this->unknown_00FC->SetSize(150, 18);
  this->unknown_00FC->Move(349, 330);
  {
    const std::uint8_t zenySource[] = {
      0x2D, 0x05
    };
    // GLOBAL: gl_25418
    thread_local EncryptedName<sizeof(zenySource), 0xE55723712993051DULL> zenyName(zenySource);
    const char* zeny = zenyName.Text();
    this->unknown_00FC->SetText(zeny, std::strlen(zeny));
  }
  this->unknown_00FC->SetFitText(0);
  this->unknown_00FC->SetAlign(2, 6);
  {
    CUIStaticText* text = this->unknown_00FC;
    const std::uint8_t zenyFontSource[] = {
      0x0E, 0x15, 0x84, 0xE4, 0x65, 0x37
    };
    // GLOBAL: gl_25419
    thread_local EncryptedName<sizeof(zenyFontSource), 0x793F370985ED674FULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->unknown_00FC->SetFontHeight(16);
  this->unknown_00FC->SetBold(0);
  this->unknown_00FC->SetColor(0xFFFF00);
  this->unknown_00FC->SetShadow(0, 0xFFFFFF);
  this->unknown_00FC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00FC);

  this->unknown_0100 = new CUIStaticText(*this->m_context);
  this->unknown_0100->SetSize(150, 18);
  this->unknown_0100->Move(350, 330);
  {
    const std::uint8_t zenySource[] = {
      0x4F, 0xD5
    };
    // GLOBAL: gl_25420
    thread_local EncryptedName<sizeof(zenySource), 0x0F15B16B5BAFD57FULL> zenyName(zenySource);
    const char* zeny = zenyName.Text();
    this->unknown_0100->SetText(zeny, std::strlen(zeny));
  }
  this->unknown_0100->SetFitText(0);
  this->unknown_0100->SetAlign(2, 6);
  {
    CUIStaticText* text = this->unknown_0100;
    const std::uint8_t zenyFontSource[] = {
      0x62, 0x83, 0xBE, 0x76, 0xBB, 0x49
    };
    // GLOBAL: gl_25421
    thread_local EncryptedName<sizeof(zenyFontSource), 0x7DE349D717D7F123ULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->unknown_0100->SetFontHeight(16);
  this->unknown_0100->SetBold(0);
  this->unknown_0100->SetColor(0xFFFF00);
  this->unknown_0100->SetShadow(0, 0xFFFFFF);
  this->unknown_0100->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_0100);

  this->unknown_0104 = new CUIStaticText(*this->m_context);
  this->unknown_0104->SetSize(150, 18);
  this->unknown_0104->Move(350, 331);
  {
    const std::uint8_t zenySource[] = {
      0xD1, 0x99
    };
    // GLOBAL: gl_25422
    thread_local EncryptedName<sizeof(zenySource), 0x7321C3814F8799E1ULL> zenyName(zenySource);
    const char* zeny = zenyName.Text();
    this->unknown_0104->SetText(zeny, std::strlen(zeny));
  }
  this->unknown_0104->SetFitText(0);
  this->unknown_0104->SetAlign(2, 6);
  {
    CUIStaticText* text = this->unknown_0104;
    const std::uint8_t zenyFontSource[] = {
      0x0E, 0xD3, 0xA6, 0x54, 0x0F, 0xD7
    };
    // GLOBAL: gl_25423
    thread_local EncryptedName<sizeof(zenyFontSource), 0x7941D76335CFA14FULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->unknown_0104->SetFontHeight(16);
  this->unknown_0104->SetBold(0);
  this->unknown_0104->SetColor(0x323232);
  this->unknown_0104->SetShadow(0, 0xFFFFFF);
  this->unknown_0104->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_0104);

  this->logCheckBox = new CUICheckBox(*this->m_context);
  this->logCheckBox->SetSize(10, 10);
  this->logCheckBox->Move(105, 359);
  this->logCheckBox->SetOnCheck([this](bool checked) { this->OnCheckChanged(checked); });
  this->logCheckBox->disabled = 0;
  this->logCheckBox->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0xA5, 0xFA, 0x09, 0x85, 0xCD, 0x21, 0xEA, 0x88, 0xA3, 0xB1, 0x09, 0xB3, 0xB0, 0x55, 0x73, 0x26,
      0x0D, 0x30, 0xAA, 0x14, 0x6F, 0x80, 0x57, 0x1A, 0x55, 0x7B, 0xAB, 0x12, 0x7D, 0xEF
    };
    // GLOBAL: gl_25424
    thread_local EncryptedName<sizeof(disabledSource), 0x452FEF0D7FC95565ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0x9D, 0xA2, 0x89, 0x73, 0x61, 0xB3, 0x28, 0x52, 0x9B, 0xE9, 0x89, 0x45, 0x1C, 0xC7, 0xB1, 0xFC,
      0x35, 0x68, 0x2A, 0xE2, 0xC3, 0x12, 0x95, 0xC0, 0x6D, 0x23, 0x2B, 0xE4, 0xD1, 0x7D
    };
    // GLOBAL: gl_25425
    thread_local EncryptedName<sizeof(pressedSource), 0x9FED7DA189490D5DULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x73, 0xC8, 0x99, 0x75, 0x1F, 0xE9, 0x5E, 0x4E, 0x75, 0x83, 0x99, 0x43, 0x62, 0x9D, 0xC7, 0xE0,
      0xDB, 0x02, 0x3A, 0xE4, 0xBD, 0x48, 0xE3, 0xDC, 0x82, 0x49, 0x3B, 0xE2, 0xAF, 0x27
    };
    // GLOBAL: gl_25426
    thread_local EncryptedName<sizeof(hoverSource), 0x839B27DF8F5967B3ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0xAD, 0xB4, 0x67, 0xEB, 0xB7, 0xA1, 0x0E, 0x3E, 0xAB, 0xFF, 0x67, 0xDD, 0xCA, 0xD5, 0x97, 0x90,
      0x05, 0x7E, 0xC4, 0x7A, 0x15, 0x00, 0xB3, 0xAC, 0x5D, 0x35, 0xC5, 0x7C, 0x07, 0x6F
    };
    // GLOBAL: gl_25427
    thread_local EncryptedName<sizeof(normalSource), 0xF3CB6F7711A71B6DULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->logCheckBox->SetImages(normal, hover, pressed, disabled);
  }
  this->logCheckBox->checked = false;
  this->logCheckBox->Invalidate();
  this->AddChild(this->logCheckBox);

  this->unknown_010C = new CUIStaticText(*this->m_context);
  this->unknown_010C->SetSize(135, 14);
  this->unknown_010C->Move(123, 356);
  this->unknown_010C->SetText("", 0);
  this->unknown_010C->SetFitText(0);
  this->unknown_010C->SetAlign(0, 6);
  {
    CUIStaticText* label = this->unknown_010C;
    const std::uint8_t labelFontSource[] = {
      0x22, 0x86, 0xBB, 0x34, 0xE0, 0x13
    };
    // GLOBAL: gl_25428
    thread_local EncryptedName<sizeof(labelFontSource), 0x176F138D5DD7F365ULL> labelFontName(labelFontSource);
    label->SetFont(std::string(labelFontName.Text()));
  }
  this->unknown_010C->SetFontHeight(12);
  this->unknown_010C->SetBold(1);
  this->unknown_010C->SetColor(0xFF);
  this->unknown_010C->SetShadow(0, 0xFFFFFF);
  this->unknown_010C->SetOutline(1, 0xFFFFFF);
  this->AddChild(this->unknown_010C);

  this->lockButton = new CUIImageButton(*this->m_context);
  this->lockButton->SetSize(42, 20);
  this->lockButton->Move(3, 354);
  this->lockButton->SetOnClick([this]() { this->SubmitZeny(); });
  this->lockButton->disabled = 0;
  this->lockButton->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0x77, 0x9E, 0x2F, 0x39, 0xC3, 0x11, 0x5C, 0x82, 0x71, 0xD5, 0x2F, 0x0F, 0xBE, 0x65, 0xC5, 0x2D,
      0xC3, 0x5F, 0xB0, 0xAC, 0x68, 0x80, 0xFD, 0x26, 0xC4, 0x1F, 0x8D, 0xAE, 0x73, 0xDF
    };
    // GLOBAL: gl_25429
    thread_local EncryptedName<sizeof(disabledSource), 0x4F99DF03C3EF31B7ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0x2D, 0xDA, 0xA1, 0xD9, 0x77, 0x2D, 0x94, 0xE0, 0x2B, 0x91, 0xA1, 0xEF, 0x0A, 0x59, 0x0D, 0x4F,
      0x99, 0x1B, 0x3E, 0x4C, 0xDC, 0xBC, 0x30, 0x03, 0x8F, 0x18, 0x11, 0x23
    };
    // GLOBAL: gl_25430
    thread_local EncryptedName<sizeof(pressedSource), 0x2D51E3B7236175EDULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0xD7, 0xB4, 0xED, 0x59, 0x7D, 0x25, 0x84, 0xC2, 0xD1, 0xFF, 0xED, 0x6F, 0x00, 0x51, 0x1D, 0x6D,
      0x63, 0x75, 0x72, 0xCC, 0xD6, 0xB4, 0x23, 0x21, 0x75, 0x76, 0x5D, 0xA3
    };
    // GLOBAL: gl_25431
    thread_local EncryptedName<sizeof(hoverSource), 0x0F41EBBDA32D1B17ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0x01, 0xE8, 0x05, 0xED, 0x21, 0x57, 0x10, 0x36, 0x07, 0xA3, 0x05, 0xDB, 0x5C, 0x23, 0x89, 0x99,
      0xB5, 0x29, 0x9A, 0x78, 0x8A, 0xB7, 0xB7, 0x96, 0xB1, 0x47
    };
    // GLOBAL: gl_25432
    thread_local EncryptedName<sizeof(normalSource), 0xFBD599E117C547C1ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->lockButton->SetImages(normal, hover, pressed, disabled);
  }
  this->AddChild(this->lockButton);

  this->tradeButton = new CUIImageButton(*this->m_context);
  this->tradeButton->SetSize(42, 20);
  this->tradeButton->Move(260, 354);
  this->tradeButton->SetOnClick([this]() { this->Conclude(); });
  this->tradeButton->disabled = 0;
  this->tradeButton->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0x99, 0x18, 0xC7, 0xE9, 0x05, 0xB3, 0xA6, 0x82, 0x9F, 0x53, 0xC7, 0xDF, 0x78, 0xC7, 0x3F, 0x2D,
      0x2D, 0xD9, 0x58, 0x76, 0xBD, 0x1E, 0x0B, 0x2E, 0x37, 0xD0, 0x62, 0x4C, 0xA1, 0x14, 0x10, 0x61,
      0x3B, 0xDA, 0x77, 0x13
    };
    // GLOBAL: gl_25433
    thread_local EncryptedName<sizeof(disabledSource), 0x4F637DC51307B759ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0xE5, 0xDE, 0x6F, 0x31, 0xCD, 0x3D, 0xE8, 0x28, 0xE3, 0x95, 0x6F, 0x07, 0xB0, 0x49, 0x71, 0x87,
      0x51, 0x1F, 0xF0, 0xAE, 0x75, 0x90, 0x45, 0x84, 0x4B, 0x16, 0xCA, 0x94, 0x6C, 0xDD, 0x4F, 0x88,
      0x55, 0x71
    };
    // GLOBAL: gl_25434
    thread_local EncryptedName<sizeof(pressedSource), 0xE52DF30DCBAF7125ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x9F, 0x9A, 0x6F, 0x6F, 0x5D, 0x47, 0x36, 0x02, 0x99, 0xD1, 0x6F, 0x59, 0x20, 0x33, 0xAF, 0xAD,
      0x2B, 0x5B, 0xF0, 0xF0, 0xE5, 0xEA, 0x9B, 0xAE, 0x31, 0x52, 0xCA, 0xCA, 0xFF, 0xA7, 0x91, 0xA2,
      0x2F, 0x35
    };
    // GLOBAL: gl_25435
    thread_local EncryptedName<sizeof(hoverSource), 0xCFF3899D95AF355FULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0x37, 0x66, 0xA3, 0x93, 0xBD, 0x57, 0xB4, 0x5C, 0x31, 0x2D, 0xA3, 0xA5, 0xC0, 0x23, 0x2D, 0xF3,
      0x83, 0xA7, 0x3C, 0x0C, 0x05, 0xFA, 0x19, 0xF0, 0x99, 0xAE, 0x06, 0x47, 0x1F, 0xF4, 0x01, 0x91
    };
    // GLOBAL: gl_25436
    thread_local EncryptedName<sizeof(normalSource), 0x9171997D6963C9F7ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->tradeButton->SetImages(normal, hover, pressed, disabled);
  }
  this->AddChild(this->tradeButton);

  this->unknown_0118 = new CUIImageButton(*this->m_context);
  this->unknown_0118->SetSize(42, 20);
  this->unknown_0118->Move(510, 354);
  this->unknown_0118->SetOnClick([this]() { this->Cancel(); });
  this->unknown_0118->disabled = 0;
  this->unknown_0118->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0x0D, 0xAC, 0x0D, 0x81, 0x57, 0xF5, 0xD6, 0xFE, 0x0B, 0xE7, 0x0D, 0xB7, 0x2A, 0x81, 0x4F, 0x51,
      0xB9, 0x6D, 0x92, 0x18, 0xF6, 0x55, 0x70, 0x56, 0xA1, 0x2D, 0xAF, 0x16, 0xE7, 0x3B
    };
    // GLOBAL: gl_25437
    thread_local EncryptedName<sizeof(disabledSource), 0x33133B977BCD03CDULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0x37, 0xBC, 0xEB, 0x55, 0x31, 0xC1, 0xD8, 0x40, 0x31, 0xF7, 0xEB, 0x63, 0x4C, 0xB5, 0x41, 0xEF,
      0x83, 0x7D, 0x74, 0xCC, 0x90, 0x61, 0x7E, 0xE8, 0x9B, 0x4C, 0x4A, 0x81, 0x93, 0x62, 0x6D, 0x8D
    };
    // GLOBAL: gl_25438
    thread_local EncryptedName<sizeof(pressedSource), 0x8D1D0FF1AF2B13F7ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x4D, 0x14, 0x0F, 0x81, 0x51, 0x69, 0x5A, 0x7E, 0x4B, 0x5F, 0x0F, 0xB7, 0x2C, 0x1D, 0xC3, 0xD1,
      0xF9, 0xD5, 0x90, 0x18, 0xF0, 0xC9, 0xFC, 0xD6, 0xE1, 0xE4, 0xAD, 0x55, 0xF3, 0xCA, 0xEF, 0xB3
    };
    // GLOBAL: gl_25439
    thread_local EncryptedName<sizeof(hoverSource), 0xB39FA7917BCFBB8DULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0x6B, 0x6E, 0x8D, 0xC3, 0xA7, 0xBB, 0x0E, 0xE0, 0x6D, 0x25, 0x8D, 0xF5, 0xDA, 0xCF, 0x97, 0x4F,
      0xDF, 0xAF, 0x12, 0x5A, 0x06, 0x1B, 0xA8, 0x48, 0xC7, 0xEF, 0x2F, 0x54, 0x17, 0x75
    };
    // GLOBAL: gl_25440
    thread_local EncryptedName<sizeof(normalSource), 0x2DCB7567394DC1ABULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->unknown_0118->SetImages(normal, hover, pressed, disabled);
  }
  this->AddChild(this->unknown_0118);
}

// OFFSET: 0x00A16C50
// FUNCTION: fn_15282
CUIExchangeUI::CUIExchangeUI()
    : titleBar(nullptr), unknown_00DC(nullptr), unknown_00E0(nullptr), firstList(nullptr), unknown_00E8(nullptr), unknown_00EC(nullptr), unknown_00F0(nullptr),
      zenyEdit(nullptr), secondList(nullptr), unknown_00FC(nullptr), unknown_0100(nullptr), unknown_0104(nullptr), logCheckBox(nullptr), unknown_010C(nullptr),
      lockButton(nullptr), tradeButton(nullptr), unknown_0118(nullptr) {
}

// OFFSET: 0x00A16E10
// FUNCTION: fn_15283
ExchangeUI::CUIItem::CUIItem() : CUIListItem(), unknown_00E8(nullptr), itemIcon(nullptr), unknown_00F0(nullptr), unknown_00F4(nullptr) {
}

// OFFSET: 0x00A10030
// FUNCTION: fn_15187
CUIExchangeUI* CreateExchangeUI() {
  return new CUIExchangeUI();
}

// OFFSET: 0x00A151F0
// FUNCTION: fn_15195
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (ExchangeUI::CUIItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->rowHeight <= this->geometry.height) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count);
    for (ExchangeUI::CUIItem*& slot : this->items) {
      ExchangeUI::CUIItem* item = new ExchangeUI::CUIItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (ExchangeUI::CUIItem* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->unknown_00C4) {
          return;
        }
        for (ExchangeUI::CUIItem* other : this->items) {
          if (listItem != other) {
            other->SetSelected(0);
          }
        }
        listItem->SetSelected(1);
        if (listItem->index == static_cast<int>(this->selected) && this->unknown_00E0) {
          return;
        }
        ExchangeUI::CUIItem* selectedItem = dynamic_cast<ExchangeUI::CUIItem*>(listItem);
        if (!selectedItem) {
          return;
        }
        this->selected = selectedItem->index;
        if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
          auto node = this->data.begin();
          std::advance(node, static_cast<int>(this->selected));
          this->onSelect(selectedItem, *node);
        }
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A15530
// FUNCTION: fn_15198
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Slot53(int resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (ExchangeUI::CUIItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A15740
// FUNCTION: fn_15199
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->data.size() * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}
