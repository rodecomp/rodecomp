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

// GLOBAL: gl_25004
int g_exchangeMyZeny;

// GLOBAL: gl_25005
int g_exchangeOtherZeny;

// OFFSET: 0x00A0F840

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
      reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
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
