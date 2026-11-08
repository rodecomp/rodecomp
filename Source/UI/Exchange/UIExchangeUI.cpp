#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Exchange/UIExchangeUI.h"
#include "UI/Window/UIEnums.h"

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
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"
#include "UI/Window/UIRegisterMgr.h"

#include "Resource/MSGSTRINGID.h"
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
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnWheel(int value);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnScroll(int position);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::AddNewItem();
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::LoadSkin(int resolveSkin);



// INLINED
template <std::uint64_t Key, std::size_t Length>
static const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

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
CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
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
void CUIExchangeUI::Initialize() {
  this->ownedSkinColors = 0;
  this->m_resolveSkin = true;
  this->Resize(560, 377);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x00A101E0
// FUNCTION: fn_15190
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
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
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
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

  mode->SendMsg(MM_CANCEL_EXCHANGE_ITEM, 0, 0, 0, 0);
  CUIImageButton* button = this->cancelButton;
  button->disabled = 1;
  button->Invalidate();
}

// OFFSET: 0x00A17320
// FUNCTION: fn_15287
void CUIExchangeUI::Conclude() {
  CMode* mode = g_modeMgr.GetGameMode();
  if (!mode) {
    return;
  }

  if (this->logCheckBox->checked) {
    std::string text = FormatOwnedString(LookupMessage(MSI_SCREENSHOT_FOR_EXCHANGE_CHARNAME), this->partnerName.c_str());
    g_modeMgr.gameMode->SendMsg(MM_SCREENSHOT, reinterpret_cast<int>(text.c_str()), 0, 0, 0);
  }

  mode->SendMsg(MM_EXEC_EXCHANGE_ITEM, 0, 0, 0, 0);
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

    mode->SendMsg(MM_ADD_EXCHANGE_ITEM, 0, g_session.exchangeMoney + amount, 0, 0);
    mode->SendMsg(MM_CONCLUDE_EXCHANGE_ITEM, 0, 0, 0, 0);
    this->lockButton->disabled = 1;
    this->lockButton->Invalidate();
  } else {
    g_windowMgr.Confirm(LookupMessage(MSI_CAN_INPUT_NUMBER_ONLY), 0, true, false, 0, "Message", 280, 120, 0);
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
void CUIExchangeUI::DragAndDrop(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  if (this->firstList->layoutFlag) {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 1) {
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    } else {
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      if (g_windowMgr.window_01EC != nullptr) {
        g_windowMgr.window_01EC->SendMsg(nullptr, UM_SETSTATE, 5, 0, 0, 0);
      }
    }
  } else {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
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
void CUIExchangeUI::OnInit() {
  this->Move((g_renderer->viewport.width - 640) / 2 + 100, g_renderer->viewport.height * 100 / 480);

  {
    const std::string title = std::format("{} : ", LookupMessage(MSI_EXCHANGE));
    this->titleBar->SetTitle(title);
  }

  this->screenshotLabel->SetText(LookupMessage(MSI_SCREENSHOT_FOR_EXCHANGE));

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

  CUIImageButton* button = this->tradeButton;
  button->disabled = 1;
  button->Invalidate();
}

// OFFSET: 0x00A17BD0
// FUNCTION: fn_15299
void ExchangeUI::CUIItem::OnInit() {
  CUIGameItem* icon = this->itemIcon;
  icon->showOverlay = 1;
  icon->Invalidate();
}

// OFFSET: 0x00A17BF0
// FUNCTION: fn_15300
void ExchangeUI::CUIItem::OnMouseHover(int x, int y) {
  this->itemIcon->OnMouseHover(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A17C20
// FUNCTION: fn_15301
void CUIExchangeUI::OnProcess() {
  if (this->zenyEdit->GetEditWindow() != g_windowMgr.GetHoverWindow()) {
    return;
  }

  char text[24];
  _snprintf_s(text, sizeof(text), "%s", this->zenyEdit->edit->Text());
  if (g_session.IsNumberWithCommas(text)) {
    int amount = std::atoi(text);
    amount = amount < 0 ? 0 : amount;

    if (amount > 1000000000) {
      g_windowMgr.Confirm(LookupMessage(MSI_BASIC_EXP_MSG_INDONESIA), 2, true, false, 0, "Message", 280, 120, 0);
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
    g_windowMgr.Confirm(LookupMessage(MSI_CAN_INPUT_NUMBER_ONLY), 0, true, false, 0, "Message", 280, 120, 0);
    this->zenyEdit->SetText(this->zenyText);
  }
}

// OFFSET: 0x00A17F30
// FUNCTION: fn_15302
void ExchangeUI::CUIItem::OnRBtnDown(int x, int y) {
  this->itemIcon->OnRBtnDown(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
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
  case UM_SET_EXCHANGE_CHAR_NAME:
    if (argument1 == 0) {
      break;
    }

    this->partnerName = reinterpret_cast<const char*>(argument1);
    if (argument2 > 0) {
      if (g_serviceType == 6) {
        this->partnerText->SetText(std::format("{} Lv{}", this->partnerName, static_cast<int>(argument3)));
      } else {
        std::string job = ShortcutFilename(argument2);
        this->partnerText->SetText(std::format("{} Lv{} ({})", this->partnerName, static_cast<int>(argument3), job));
      }
    } else {
      this->partnerText->SetText(this->partnerName);
    }
    break;
  case UM_MYDRAGCLOSED:
    this->zenyEdit->Show(0);
    this->FreezeItems(true);
    this->UpdateZeny(true);
    {
      CUIImageButton* button = this->tradeButton;
      if (this->firstList->layoutFlag && this->secondList->layoutFlag) {
        button->disabled = 0;
      } else {
        button->disabled = 1;
      }
      button->Invalidate();
    }
    break;
  case UM_OTHERDRAGCLOSED:
    this->FreezeItems(false);
    this->UpdateZeny(false);
    {
      CUIImageButton* button = this->tradeButton;
      if (this->firstList->layoutFlag && this->secondList->layoutFlag) {
        button->disabled = 0;
      } else {
        button->disabled = 1;
      }
      button->Invalidate();
    }
    break;
  case UM_REFRESHITEM:
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
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode != nullptr) {
      mode->SendMsg(MM_ADD_EXCHANGE_ITEM, mode->drag.id, mode->drag.quantity, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    }
    break;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x00A18660
// FUNCTION: fn_15305
void CUIExchangeUI::BindItem(ExchangeUI::CUIItem* item, const ExchangeUI::CItemData& data) {
  const int quantity = data.item.quantity;
  const bool visible = quantity > 0;
  item->itemIcon->Show(visible);
  item->quantityText->Show(visible);
  item->itemNameText->Show(visible);
  item->itemIcon->SetItem(data.item);

  item->quantityText->SetText(quantity > 0 ? std::to_string(data.item.quantity) : "");
  item->itemNameText->SetText(quantity > 0 ? data.item.DisplayName(0) : "");
  item->itemNameText->SetTooltip("", 0);

  if (data.frozen) {
    item->SetFrozen(true);
    item->slotBackground->Show(0);
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
    first = this->ownZenyHighlight;
    second = this->ownZenyText;
    third = this->ownZenyShadow;
    zeny = g_session.exchangeMoney;
  } else {
    first = this->partnerZenyHighlight;
    second = this->partnerZenyText;
    third = this->partnerZenyShadow;
    zeny = g_session.otherExchangeMoney;
  }

  char text[24];
  FormatGroupedInteger(zeny, text, sizeof(text));
  first->SetText(text);
  second->SetText(text);
  third->SetText(text);

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
  list->layoutFlag = 1;
  list->Invalidate();
}

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
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
void ExchangeUI::CUIItem::OnCreate(int width, int height) {
  this->slotBackground = new CUIStaticImage();
  this->slotBackground->SetSize(32, 32);
  this->slotBackground->Move(9, 0);
  this->slotBackground->SetImage(std::string(DecryptName<0x75ADC57D87EFBD8FULL>({'\x4F', '\x12', '\x2F', '\x7D', '\xBD', '\x0B', '\x68', '\xB8', '\x49', '\x59', '\x2F', '\x4B', '\xC0', '\x7F', '\xF1', '\x17',
                                   '\xEE', '\xCE', '\x86', '\xE4', '\x22', '\xAC', '\xC3', '\x01', '\xEA', '\xCF', '\x89', '\xE6', '\x1E', '\xA0', '\xF1', '\x1C',
                                   '\xFB', '\xD8', '\x82', '\xF0', '\x14', '\xAB', '\xF2', '\x18', '\xE6', '\xD9', '\xC1', '\xE5', '\x10', '\xB5', '\xAD'})));
  this->AddChild(this->slotBackground);

  this->itemIcon = new CUIGameItem();
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(13, 1);
  this->itemIcon->showNameTooltip = 1;
  this->itemIcon->allowItemInfo = 1;
  this->AddChild(this->itemIcon);

  this->quantityText = new CUIStaticText();
  this->quantityText->SetSize(100, 13);
  this->quantityText->Move(25, 15);
  this->quantityText->SetText("");
  this->quantityText->SetFitText(0);
  this->quantityText->SetAlign(0, 6);
  this->quantityText->SetFont(std::string(DecryptName<0xC517FB932D95BB6DULL>({'\x2A', '\xCE', '\xF9', '\x44', '\xFE', '\xFB'})));
  this->quantityText->SetFontHeight(11);
  this->quantityText->SetBold(0);
  this->quantityText->SetColor(0);
  this->quantityText->SetShadow(0, 0xFFFFFF);
  this->quantityText->SetOutline(1, 0xFFFFFF);
  this->AddChild(this->quantityText);

  this->itemNameText = new CUIStaticText();
  this->itemNameText->SetSize(215, 30);
  this->itemNameText->Move(45, 0);
  this->itemNameText->SetText("");
  this->itemNameText->SetFitText(1);
  this->itemNameText->SetAlign(0, 6);
  this->itemNameText->SetFont(std::string(DecryptName<0x23BBD58F8B5FF5C9ULL>({'\x8E', '\x80', '\x33', '\xE2', '\xE2', '\xD5'})));
  this->itemNameText->SetFontHeight(12);
  this->itemNameText->SetBold(0);
  this->itemNameText->SetColor(0);
  this->itemNameText->SetShadow(0, 0xFFFFFF);
  this->itemNameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->itemNameText);
}

// OFFSET: 0x00A103F0
// FUNCTION: fn_15191
void CUIExchangeUI::OnCreate(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(560, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    const char titleRightSource[] = {
      '\xF7', '\xE8', '\x6D', '\xC3', '\x7F', '\x2B', '\x6C', '\xDA', '\xF1', '\xA3', '\x6D', '\xF5', '\x02', '\x5F', '\xF5', '\x75',
      '\x56', '\x34', '\xC4', '\x5A', '\xE0', '\x8C', '\xC7', '\x63', '\x52', '\x35', '\xCB', '\x58', '\xDC', '\x80', '\xF5', '\x63',
      '\x5E', '\x33', '\xC1', '\x5C', '\xDD', '\x84', '\xDB', '\x48', '\x45', '\x2E', '\xCA', '\x51', '\xCB', '\xCB', '\xCB', '\x7A',
      '\x47', '\x47'
    };
    // GLOBAL: gl_25394
    thread_local EncryptedName<sizeof(titleRightSource), 0x17A9E5BF39AD4737ULL> titleRightName(titleRightSource);
    const std::string titleRight(titleRightName.Text());
    const char titleMiddleSource[] = {
      '\x65', '\x32', '\xAD', '\x03', '\xA3', '\xE9', '\xB2', '\xCE', '\x63', '\x79', '\xAD', '\x35', '\xDE', '\x9D', '\x2B', '\x61',
      '\xC4', '\xEE', '\x04', '\x9A', '\x3C', '\x4E', '\x19', '\x77', '\xC0', '\xEF', '\x0B', '\x98', '\x00', '\x42', '\x2B', '\x77',
      '\xCC', '\xE9', '\x01', '\x9C', '\x01', '\x46', '\x05', '\x5C', '\xC8', '\xF4', '\x09', '\xD7', '\x01', '\x4A', '\x07', '\x03'
    };
    // GLOBAL: gl_25395
    thread_local EncryptedName<sizeof(titleMiddleSource), 0x03772763F96D9DA5ULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const char titleLeftSource[] = {
      '\x47', '\xBC', '\xAB', '\x85', '\xF9', '\xC7', '\xB4', '\xC6', '\x41', '\xF7', '\xAB', '\xB3', '\x84', '\xB3', '\x2D', '\x69',
      '\xE6', '\x60', '\x02', '\x1C', '\x66', '\x60', '\x1F', '\x7F', '\xE2', '\x61', '\x0D', '\x1E', '\x5A', '\x6C', '\x2D', '\x7F',
      '\xEE', '\x67', '\x07', '\x1A', '\x5B', '\x68', '\x03', '\x54', '\xEB', '\x76', '\x0D', '\x0B', '\x17', '\x6B', '\x1C', '\x7B',
      '\x87'
    };
    // GLOBAL: gl_25396
    thread_local EncryptedName<sizeof(titleLeftSource), 0x0B7109397F6B1387ULL> titleLeftName(titleLeftSource);
    const std::string titleLeft(titleLeftName.Text());
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(12, 0);
  title->SetText("");
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const char titleFontSource[] = {
      '\x20', '\xDE', '\xFD', '\xCC', '\xCC', '\x3F'
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

  this->partnerText = new CUIStaticText();
  this->partnerText->SetSize(495, 17);
  this->partnerText->Move(62, 0);
  this->partnerText->SetText("");
  this->partnerText->SetFitText(1);
  this->partnerText->SetAlign(0, 6);
  {
    CUIStaticText* partner = this->partnerText;
    const char partnerFontSource[] = {
      '\x28', '\x08', '\x97', '\x4A', '\x1C', '\x07'
    };
    // GLOBAL: gl_25398
    thread_local EncryptedName<sizeof(partnerFontSource), 0xCD8F077123FB7D6FULL> partnerFontName(partnerFontSource);
    partner->SetFont(std::string(partnerFontName.Text()));
  }
  this->partnerText->SetFontHeight(12);
  this->partnerText->SetBold(0);
  this->partnerText->SetColor(0);
  this->partnerText->SetShadow(1, 0xB4B4B4);
  this->partnerText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->partnerText);

  this->backgroundImage = new CUIStaticImage();
  this->backgroundImage->SetSize(560, 360);
  this->backgroundImage->Move(0, 17);
  {
    CUIStaticImage* background = this->backgroundImage;
    const char backgroundSource[] = {
      '\x83', '\x86', '\x93', '\xDD', '\x15', '\x71', '\x8C', '\x54', '\x85', '\xCD', '\x93', '\xEB', '\x68', '\x05', '\x15', '\xFB',
      '\x22', '\x5A', '\x3A', '\x44', '\x8A', '\xD6', '\x27', '\xED', '\x26', '\x5B', '\x35', '\x46', '\xB6', '\xDA', '\x15', '\xFC',
      '\x3B', '\x4A', '\x3B', '\x46', '\xBB', '\xD8', '\x2C', '\xC6', '\x21', '\x4E', '\x61', '\x09', '\xB7', '\xD2', '\x39', '\x99'
    };
    // GLOBAL: gl_25399
    thread_local EncryptedName<sizeof(backgroundSource), 0x9949BFD527532943ULL> backgroundName(backgroundSource);
    background->SetImage(std::string(backgroundName.Text()));
  }
  this->AddChild(this->backgroundImage);

  this->firstList = new ExchangeUI::CItemListBox();
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
    const char downSource[] = {
      '\xC9', '\x88', '\xEF', '\x41', '\x7D', '\x93', '\xB8', '\x14', '\xCF', '\xC3', '\xEF', '\x77', '\x00', '\xE7', '\x21', '\xAA',
      '\x6A', '\x55', '\x40', '\xD7', '\xD1', '\x6D', '\x19', '\xB6', '\x7E', '\x49', '\x01', '\xD9', '\xD0', '\x2D', '\x7D'
    };
    // GLOBAL: gl_25400
    thread_local EncryptedName<sizeof(downSource), 0xD97D5DBDBB2F2709ULL> downName(downSource);
    const std::string down(downName.Text());
    const char upSource[] = {
      '\xBF', '\xBC', '\x1F', '\x2B', '\x45', '\xA9', '\xBC', '\x20', '\xB9', '\xF7', '\x1F', '\x1D', '\x38', '\xDD', '\x25', '\x9E',
      '\x1C', '\x61', '\xB0', '\xBD', '\xE9', '\x57', '\x0C', '\x9D', '\x51', '\x71', '\xB2', '\xA1', '\x85'
    };
    // GLOBAL: gl_25401
    thread_local EncryptedName<sizeof(upSource), 0xED796785D1DF137FULL> upName(upSource);
    const std::string up(upName.Text());
    this->firstList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char backgroundSource[] = {
      '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xD6',
      '\xFC', '\xB7', '\x02', '\xC7', '\xBB', '\x19', '\x90', '\xCC', '\xFB', '\xEB', '\x0F', '\xC6', '\xA7', '\x29'
    };
    // GLOBAL: gl_25402
    thread_local EncryptedName<sizeof(backgroundSource), 0xA5FD29D7AB6DC59FULL> backgroundName(backgroundSource);
    this->firstList->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const char thumbBottomSource[] = {
      '\x21', '\xD4', '\x39', '\x2D', '\x15', '\xDD', '\xFC', '\x10', '\x27', '\x9F', '\x39', '\x1B', '\x68', '\xA9', '\x65', '\xAE',
      '\x82', '\x09', '\x96', '\xBB', '\xB9', '\x23', '\x5B', '\xBC', '\x93', '\x24', '\x9D', '\xB8', '\xA2', '\x7D', '\x17', '\xBF',
      '\x8C', '\x0B', '\xF9'
    };
    // GLOBAL: gl_25403
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xDD3913D5D7F97BE1ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const char thumbMiddleSource[] = {
      '\x5B', '\xF8', '\xC3', '\x63', '\xA7', '\xCB', '\xC0', '\xC8', '\x5D', '\xB3', '\xC3', '\x55', '\xDA', '\xBF', '\x59', '\x76',
      '\xF8', '\x25', '\x6C', '\xF5', '\x0B', '\x35', '\x67', '\x64', '\xE9', '\x08', '\x6E', '\xF0', '\x03', '\x2B', '\x67', '\x68',
      '\xEB', '\x57'
    };
    // GLOBAL: gl_25404
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x050505679903579BULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const char thumbTopSource[] = {
      '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\x82',
      '\x92', '\xB5', '\x56', '\x65', '\x7B', '\x49', '\x2D', '\x90', '\x83', '\x98', '\x4C', '\x79', '\x39', '\x1B', '\x22', '\x81',
      '\xF1'
    };
    // GLOBAL: gl_25405
    thread_local EncryptedName<sizeof(thumbTopSource), 0xF14F79170939C7F1ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->firstList->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->firstList->AddNewItem();
  this->AddChild(this->firstList);

  this->ownZenyHighlight = new CUIStaticText();
  this->ownZenyHighlight->SetSize(150, 18);
  this->ownZenyHighlight->Move(69, 330);
  {
    const char zenySource[] = {
      '\x97', '\xF5'
    };
    // GLOBAL: gl_25406
    thread_local EncryptedName<sizeof(zenySource), 0xBF2B4B31397FF5A7ULL> zenyName(zenySource);
    this->ownZenyHighlight->SetText(zenyName.Text());
  }
  this->ownZenyHighlight->SetFitText(0);
  this->ownZenyHighlight->SetAlign(2, 6);
  {
    CUIStaticText* text = this->ownZenyHighlight;
    const char zenyFontSource[] = {
      '\xFC', '\xCF', '\x84', '\x56', '\xD7', '\xBF'
    };
    // GLOBAL: gl_25407
    thread_local EncryptedName<sizeof(zenyFontSource), 0x83EFBFBB37EDBDBDULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->ownZenyHighlight->SetFontHeight(16);
  this->ownZenyHighlight->SetBold(0);
  this->ownZenyHighlight->SetColor(0xFFFF00);
  this->ownZenyHighlight->SetShadow(0, 0xFFFFFF);
  this->ownZenyHighlight->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->ownZenyHighlight);

  this->ownZenyText = new CUIStaticText();
  this->ownZenyText->SetSize(150, 18);
  this->ownZenyText->Move(70, 330);
  {
    const char zenySource[] = {
      '\xBF', '\x99'
    };
    // GLOBAL: gl_25408
    thread_local EncryptedName<sizeof(zenySource), 0x39FF7FB50DEB998FULL> zenyName(zenySource);
    this->ownZenyText->SetText(zenyName.Text());
  }
  this->ownZenyText->SetFitText(0);
  this->ownZenyText->SetAlign(2, 6);
  {
    CUIStaticText* text = this->ownZenyText;
    const char zenyFontSource[] = {
      '\xD6', '\x83', '\x88', '\xBC', '\xBD', '\x93'
    };
    // GLOBAL: gl_25409
    thread_local EncryptedName<sizeof(zenyFontSource), 0x2FE993D1DDE1F197ULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->ownZenyText->SetFontHeight(16);
  this->ownZenyText->SetBold(0);
  this->ownZenyText->SetColor(0xFFFF00);
  this->ownZenyText->SetShadow(0, 0xFFFFFF);
  this->ownZenyText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->ownZenyText);

  this->ownZenyShadow = new CUIStaticText();
  this->ownZenyShadow->SetSize(150, 18);
  this->ownZenyShadow->Move(70, 331);
  {
    const char zenySource[] = {
      '\xFD', '\xF1'
    };
    // GLOBAL: gl_25410
    thread_local EncryptedName<sizeof(zenySource), 0xAD075B8DB599F1CDULL> zenyName(zenySource);
    this->ownZenyShadow->SetText(zenyName.Text());
  }
  this->ownZenyShadow->SetFitText(0);
  this->ownZenyShadow->SetAlign(2, 6);
  {
    CUIStaticText* text = this->ownZenyShadow;
    const char zenyFontSource[] = {
      '\x3E', '\x67', '\xE4', '\x70', '\x5B', '\x0F'
    };
    // GLOBAL: gl_25411
    thread_local EncryptedName<sizeof(zenyFontSource), 0x2F8B0F37118D157FULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->ownZenyShadow->SetFontHeight(16);
  this->ownZenyShadow->SetBold(0);
  this->ownZenyShadow->SetColor(0x323232);
  this->ownZenyShadow->SetShadow(0, 0xFFFFFF);
  this->ownZenyShadow->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->ownZenyShadow);

  this->zenyEdit = new CUIEditBoxEx();
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

  this->secondList = new ExchangeUI::CItemListBox();
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
    const char downSource[] = {
      '\x3D', '\x16', '\x7B', '\xDB', '\x5F', '\x33', '\x84', '\x84', '\x3B', '\x5D', '\x7B', '\xED', '\x22', '\x47', '\x1D', '\x3A',
      '\x9E', '\xCB', '\xD4', '\x4D', '\xF3', '\xCD', '\x25', '\x26', '\x8A', '\xD7', '\x95', '\x43', '\xF2', '\x8D', '\x41'
    };
    // GLOBAL: gl_25412
    thread_local EncryptedName<sizeof(downSource), 0x4941FD9F21BBB9FDULL> downName(downSource);
    const std::string down(downName.Text());
    const char upSource[] = {
      '\xB5', '\x62', '\x2D', '\xA7', '\xAB', '\xAB', '\x20', '\xF8', '\xB3', '\x29', '\x2D', '\x91', '\xD6', '\xDF', '\xB9', '\x46',
      '\x16', '\xBF', '\x82', '\x31', '\x07', '\x55', '\x90', '\x45', '\x5B', '\xAF', '\x80', '\x2D', '\x6B'
    };
    // GLOBAL: gl_25413
    thread_local EncryptedName<sizeof(upSource), 0x35E5656B5DEDCD75ULL> upName(upSource);
    const std::string up(upName.Text());
    this->secondList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char backgroundSource[] = {
      '\x3D', '\x78', '\x8D', '\x53', '\x75', '\x41', '\x52', '\x2A', '\x3B', '\x33', '\x8D', '\x65', '\x08', '\x35', '\xCB', '\x94',
      '\x9E', '\xA5', '\x22', '\xC5', '\xD9', '\xBF', '\xFA', '\x8E', '\x99', '\xF9', '\x2F', '\xC4', '\xC5', '\x8F'
    };
    // GLOBAL: gl_25414
    thread_local EncryptedName<sizeof(backgroundSource), 0xE7978FB5A94DD7FDULL> backgroundName(backgroundSource);
    this->secondList->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const char thumbBottomSource[] = {
      '\x09', '\xBC', '\x39', '\x89', '\x39', '\x17', '\xD0', '\xBC', '\x0F', '\xF7', '\x39', '\xBF', '\x44', '\x63', '\x49', '\x02',
      '\xAA', '\x61', '\x96', '\x1F', '\x95', '\xE9', '\x77', '\x10', '\xBB', '\x4C', '\x9D', '\x1C', '\x8E', '\xB7', '\x3B', '\x13',
      '\xA4', '\x63', '\xF9'
    };
    // GLOBAL: gl_25415
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x7115D9F973F913C9ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const char thumbMiddleSource[] = {
      '\xA3', '\x5A', '\x21', '\x85', '\xFB', '\xEB', '\xD8', '\x06', '\xA5', '\x11', '\x21', '\xB3', '\x86', '\x9F', '\x41', '\xB8',
      '\x00', '\x87', '\x8E', '\x13', '\x57', '\x15', '\x7F', '\xAA', '\x11', '\xAA', '\x8C', '\x16', '\x5F', '\x0B', '\x7F', '\xA6',
      '\x13', '\xF5'
    };
    // GLOBAL: gl_25416
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xCB1D253B7FE1F563ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const char thumbTopSource[] = {
      '\xAB', '\x16', '\x07', '\xF5', '\xA7', '\x41', '\x38', '\x7A', '\xAD', '\x5D', '\x07', '\xC3', '\xDA', '\x35', '\xA1', '\xC4',
      '\x08', '\xCB', '\xA8', '\x63', '\x0B', '\xBF', '\x9F', '\xD6', '\x19', '\xE6', '\xB2', '\x7F', '\x49', '\xED', '\x90', '\xC7',
      '\x6B'
    };
    // GLOBAL: gl_25417
    thread_local EncryptedName<sizeof(thumbTopSource), 0xB7FD8F670FC7B96BULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->secondList->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->secondList->AddNewItem();
  this->AddChild(this->secondList);

  this->partnerZenyHighlight = new CUIStaticText();
  this->partnerZenyHighlight->SetSize(150, 18);
  this->partnerZenyHighlight->Move(349, 330);
  {
    const char zenySource[] = {
      '\x2D', '\x05'
    };
    // GLOBAL: gl_25418
    thread_local EncryptedName<sizeof(zenySource), 0xE55723712993051DULL> zenyName(zenySource);
    this->partnerZenyHighlight->SetText(zenyName.Text());
  }
  this->partnerZenyHighlight->SetFitText(0);
  this->partnerZenyHighlight->SetAlign(2, 6);
  {
    CUIStaticText* text = this->partnerZenyHighlight;
    const char zenyFontSource[] = {
      '\x0E', '\x15', '\x84', '\xE4', '\x65', '\x37'
    };
    // GLOBAL: gl_25419
    thread_local EncryptedName<sizeof(zenyFontSource), 0x793F370985ED674FULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->partnerZenyHighlight->SetFontHeight(16);
  this->partnerZenyHighlight->SetBold(0);
  this->partnerZenyHighlight->SetColor(0xFFFF00);
  this->partnerZenyHighlight->SetShadow(0, 0xFFFFFF);
  this->partnerZenyHighlight->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->partnerZenyHighlight);

  this->partnerZenyText = new CUIStaticText();
  this->partnerZenyText->SetSize(150, 18);
  this->partnerZenyText->Move(350, 330);
  {
    const char zenySource[] = {
      '\x4F', '\xD5'
    };
    // GLOBAL: gl_25420
    thread_local EncryptedName<sizeof(zenySource), 0x0F15B16B5BAFD57FULL> zenyName(zenySource);
    this->partnerZenyText->SetText(zenyName.Text());
  }
  this->partnerZenyText->SetFitText(0);
  this->partnerZenyText->SetAlign(2, 6);
  {
    CUIStaticText* text = this->partnerZenyText;
    const char zenyFontSource[] = {
      '\x62', '\x83', '\xBE', '\x76', '\xBB', '\x49'
    };
    // GLOBAL: gl_25421
    thread_local EncryptedName<sizeof(zenyFontSource), 0x7DE349D717D7F123ULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->partnerZenyText->SetFontHeight(16);
  this->partnerZenyText->SetBold(0);
  this->partnerZenyText->SetColor(0xFFFF00);
  this->partnerZenyText->SetShadow(0, 0xFFFFFF);
  this->partnerZenyText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->partnerZenyText);

  this->partnerZenyShadow = new CUIStaticText();
  this->partnerZenyShadow->SetSize(150, 18);
  this->partnerZenyShadow->Move(350, 331);
  {
    const char zenySource[] = {
      '\xD1', '\x99'
    };
    // GLOBAL: gl_25422
    thread_local EncryptedName<sizeof(zenySource), 0x7321C3814F8799E1ULL> zenyName(zenySource);
    this->partnerZenyShadow->SetText(zenyName.Text());
  }
  this->partnerZenyShadow->SetFitText(0);
  this->partnerZenyShadow->SetAlign(2, 6);
  {
    CUIStaticText* text = this->partnerZenyShadow;
    const char zenyFontSource[] = {
      '\x0E', '\xD3', '\xA6', '\x54', '\x0F', '\xD7'
    };
    // GLOBAL: gl_25423
    thread_local EncryptedName<sizeof(zenyFontSource), 0x7941D76335CFA14FULL> zenyFontName(zenyFontSource);
    text->SetFont(std::string(zenyFontName.Text()));
  }
  this->partnerZenyShadow->SetFontHeight(16);
  this->partnerZenyShadow->SetBold(0);
  this->partnerZenyShadow->SetColor(0x323232);
  this->partnerZenyShadow->SetShadow(0, 0xFFFFFF);
  this->partnerZenyShadow->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->partnerZenyShadow);

  this->logCheckBox = new CUICheckBox();
  this->logCheckBox->SetSize(10, 10);
  this->logCheckBox->Move(105, 359);
  this->logCheckBox->SetOnCheck([this](bool checked) { this->OnCheckChanged(checked); });
  this->logCheckBox->disabled = 0;
  this->logCheckBox->Invalidate();
  {
    const char disabledSource[] = {
      '\xA5', '\xFA', '\x09', '\x85', '\xCD', '\x21', '\xEA', '\x88', '\xA3', '\xB1', '\x09', '\xB3', '\xB0', '\x55', '\x73', '\x26',
      '\x0D', '\x30', '\xAA', '\x14', '\x6F', '\x80', '\x57', '\x1A', '\x55', '\x7B', '\xAB', '\x12', '\x7D', '\xEF'
    };
    // GLOBAL: gl_25424
    thread_local EncryptedName<sizeof(disabledSource), 0x452FEF0D7FC95565ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = {
      '\x9D', '\xA2', '\x89', '\x73', '\x61', '\xB3', '\x28', '\x52', '\x9B', '\xE9', '\x89', '\x45', '\x1C', '\xC7', '\xB1', '\xFC',
      '\x35', '\x68', '\x2A', '\xE2', '\xC3', '\x12', '\x95', '\xC0', '\x6D', '\x23', '\x2B', '\xE4', '\xD1', '\x7D'
    };
    // GLOBAL: gl_25425
    thread_local EncryptedName<sizeof(pressedSource), 0x9FED7DA189490D5DULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = {
      '\x73', '\xC8', '\x99', '\x75', '\x1F', '\xE9', '\x5E', '\x4E', '\x75', '\x83', '\x99', '\x43', '\x62', '\x9D', '\xC7', '\xE0',
      '\xDB', '\x02', '\x3A', '\xE4', '\xBD', '\x48', '\xE3', '\xDC', '\x82', '\x49', '\x3B', '\xE2', '\xAF', '\x27'
    };
    // GLOBAL: gl_25426
    thread_local EncryptedName<sizeof(hoverSource), 0x839B27DF8F5967B3ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = {
      '\xAD', '\xB4', '\x67', '\xEB', '\xB7', '\xA1', '\x0E', '\x3E', '\xAB', '\xFF', '\x67', '\xDD', '\xCA', '\xD5', '\x97', '\x90',
      '\x05', '\x7E', '\xC4', '\x7A', '\x15', '\x00', '\xB3', '\xAC', '\x5D', '\x35', '\xC5', '\x7C', '\x07', '\x6F'
    };
    // GLOBAL: gl_25427
    thread_local EncryptedName<sizeof(normalSource), 0xF3CB6F7711A71B6DULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->logCheckBox->SetImages(normal, hover, pressed, disabled);
  }
  this->logCheckBox->checked = false;
  this->logCheckBox->Invalidate();
  this->AddChild(this->logCheckBox);

  this->screenshotLabel = new CUIStaticText();
  this->screenshotLabel->SetSize(135, 14);
  this->screenshotLabel->Move(123, 356);
  this->screenshotLabel->SetText("");
  this->screenshotLabel->SetFitText(0);
  this->screenshotLabel->SetAlign(0, 6);
  {
    CUIStaticText* label = this->screenshotLabel;
    const char labelFontSource[] = {
      '\x22', '\x86', '\xBB', '\x34', '\xE0', '\x13'
    };
    // GLOBAL: gl_25428
    thread_local EncryptedName<sizeof(labelFontSource), 0x176F138D5DD7F365ULL> labelFontName(labelFontSource);
    label->SetFont(std::string(labelFontName.Text()));
  }
  this->screenshotLabel->SetFontHeight(12);
  this->screenshotLabel->SetBold(1);
  this->screenshotLabel->SetColor(0xFF);
  this->screenshotLabel->SetShadow(0, 0xFFFFFF);
  this->screenshotLabel->SetOutline(1, 0xFFFFFF);
  this->AddChild(this->screenshotLabel);

  this->lockButton = new CUIImageButton();
  this->lockButton->SetSize(42, 20);
  this->lockButton->Move(3, 354);
  this->lockButton->SetOnClick([this]() { this->SubmitZeny(); });
  this->lockButton->disabled = 0;
  this->lockButton->Invalidate();
  {
    const char disabledSource[] = {
      '\x77', '\x9E', '\x2F', '\x39', '\xC3', '\x11', '\x5C', '\x82', '\x71', '\xD5', '\x2F', '\x0F', '\xBE', '\x65', '\xC5', '\x2D',
      '\xC3', '\x5F', '\xB0', '\xAC', '\x68', '\x80', '\xFD', '\x26', '\xC4', '\x1F', '\x8D', '\xAE', '\x73', '\xDF'
    };
    // GLOBAL: gl_25429
    thread_local EncryptedName<sizeof(disabledSource), 0x4F99DF03C3EF31B7ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = {
      '\x2D', '\xDA', '\xA1', '\xD9', '\x77', '\x2D', '\x94', '\xE0', '\x2B', '\x91', '\xA1', '\xEF', '\x0A', '\x59', '\x0D', '\x4F',
      '\x99', '\x1B', '\x3E', '\x4C', '\xDC', '\xBC', '\x30', '\x03', '\x8F', '\x18', '\x11', '\x23'
    };
    // GLOBAL: gl_25430
    thread_local EncryptedName<sizeof(pressedSource), 0x2D51E3B7236175EDULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = {
      '\xD7', '\xB4', '\xED', '\x59', '\x7D', '\x25', '\x84', '\xC2', '\xD1', '\xFF', '\xED', '\x6F', '\x00', '\x51', '\x1D', '\x6D',
      '\x63', '\x75', '\x72', '\xCC', '\xD6', '\xB4', '\x23', '\x21', '\x75', '\x76', '\x5D', '\xA3'
    };
    // GLOBAL: gl_25431
    thread_local EncryptedName<sizeof(hoverSource), 0x0F41EBBDA32D1B17ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = {
      '\x01', '\xE8', '\x05', '\xED', '\x21', '\x57', '\x10', '\x36', '\x07', '\xA3', '\x05', '\xDB', '\x5C', '\x23', '\x89', '\x99',
      '\xB5', '\x29', '\x9A', '\x78', '\x8A', '\xB7', '\xB7', '\x96', '\xB1', '\x47'
    };
    // GLOBAL: gl_25432
    thread_local EncryptedName<sizeof(normalSource), 0xFBD599E117C547C1ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->lockButton->SetImages(normal, hover, pressed, disabled);
  }
  this->AddChild(this->lockButton);

  this->tradeButton = new CUIImageButton();
  this->tradeButton->SetSize(42, 20);
  this->tradeButton->Move(260, 354);
  this->tradeButton->SetOnClick([this]() { this->Conclude(); });
  this->tradeButton->disabled = 0;
  this->tradeButton->Invalidate();
  {
    const char disabledSource[] = {
      '\x99', '\x18', '\xC7', '\xE9', '\x05', '\xB3', '\xA6', '\x82', '\x9F', '\x53', '\xC7', '\xDF', '\x78', '\xC7', '\x3F', '\x2D',
      '\x2D', '\xD9', '\x58', '\x76', '\xBD', '\x1E', '\x0B', '\x2E', '\x37', '\xD0', '\x62', '\x4C', '\xA1', '\x14', '\x10', '\x61',
      '\x3B', '\xDA', '\x77', '\x13'
    };
    // GLOBAL: gl_25433
    thread_local EncryptedName<sizeof(disabledSource), 0x4F637DC51307B759ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = {
      '\xE5', '\xDE', '\x6F', '\x31', '\xCD', '\x3D', '\xE8', '\x28', '\xE3', '\x95', '\x6F', '\x07', '\xB0', '\x49', '\x71', '\x87',
      '\x51', '\x1F', '\xF0', '\xAE', '\x75', '\x90', '\x45', '\x84', '\x4B', '\x16', '\xCA', '\x94', '\x6C', '\xDD', '\x4F', '\x88',
      '\x55', '\x71'
    };
    // GLOBAL: gl_25434
    thread_local EncryptedName<sizeof(pressedSource), 0xE52DF30DCBAF7125ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = {
      '\x9F', '\x9A', '\x6F', '\x6F', '\x5D', '\x47', '\x36', '\x02', '\x99', '\xD1', '\x6F', '\x59', '\x20', '\x33', '\xAF', '\xAD',
      '\x2B', '\x5B', '\xF0', '\xF0', '\xE5', '\xEA', '\x9B', '\xAE', '\x31', '\x52', '\xCA', '\xCA', '\xFF', '\xA7', '\x91', '\xA2',
      '\x2F', '\x35'
    };
    // GLOBAL: gl_25435
    thread_local EncryptedName<sizeof(hoverSource), 0xCFF3899D95AF355FULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = {
      '\x37', '\x66', '\xA3', '\x93', '\xBD', '\x57', '\xB4', '\x5C', '\x31', '\x2D', '\xA3', '\xA5', '\xC0', '\x23', '\x2D', '\xF3',
      '\x83', '\xA7', '\x3C', '\x0C', '\x05', '\xFA', '\x19', '\xF0', '\x99', '\xAE', '\x06', '\x47', '\x1F', '\xF4', '\x01', '\x91'
    };
    // GLOBAL: gl_25436
    thread_local EncryptedName<sizeof(normalSource), 0x9171997D6963C9F7ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->tradeButton->SetImages(normal, hover, pressed, disabled);
  }
  this->AddChild(this->tradeButton);

  this->cancelButton = new CUIImageButton();
  this->cancelButton->SetSize(42, 20);
  this->cancelButton->Move(510, 354);
  this->cancelButton->SetOnClick([this]() { this->Cancel(); });
  this->cancelButton->disabled = 0;
  this->cancelButton->Invalidate();
  {
    const char disabledSource[] = {
      '\x0D', '\xAC', '\x0D', '\x81', '\x57', '\xF5', '\xD6', '\xFE', '\x0B', '\xE7', '\x0D', '\xB7', '\x2A', '\x81', '\x4F', '\x51',
      '\xB9', '\x6D', '\x92', '\x18', '\xF6', '\x55', '\x70', '\x56', '\xA1', '\x2D', '\xAF', '\x16', '\xE7', '\x3B'
    };
    // GLOBAL: gl_25437
    thread_local EncryptedName<sizeof(disabledSource), 0x33133B977BCD03CDULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = {
      '\x37', '\xBC', '\xEB', '\x55', '\x31', '\xC1', '\xD8', '\x40', '\x31', '\xF7', '\xEB', '\x63', '\x4C', '\xB5', '\x41', '\xEF',
      '\x83', '\x7D', '\x74', '\xCC', '\x90', '\x61', '\x7E', '\xE8', '\x9B', '\x4C', '\x4A', '\x81', '\x93', '\x62', '\x6D', '\x8D'
    };
    // GLOBAL: gl_25438
    thread_local EncryptedName<sizeof(pressedSource), 0x8D1D0FF1AF2B13F7ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = {
      '\x4D', '\x14', '\x0F', '\x81', '\x51', '\x69', '\x5A', '\x7E', '\x4B', '\x5F', '\x0F', '\xB7', '\x2C', '\x1D', '\xC3', '\xD1',
      '\xF9', '\xD5', '\x90', '\x18', '\xF0', '\xC9', '\xFC', '\xD6', '\xE1', '\xE4', '\xAD', '\x55', '\xF3', '\xCA', '\xEF', '\xB3'
    };
    // GLOBAL: gl_25439
    thread_local EncryptedName<sizeof(hoverSource), 0xB39FA7917BCFBB8DULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = {
      '\x6B', '\x6E', '\x8D', '\xC3', '\xA7', '\xBB', '\x0E', '\xE0', '\x6D', '\x25', '\x8D', '\xF5', '\xDA', '\xCF', '\x97', '\x4F',
      '\xDF', '\xAF', '\x12', '\x5A', '\x06', '\x1B', '\xA8', '\x48', '\xC7', '\xEF', '\x2F', '\x54', '\x17', '\x75'
    };
    // GLOBAL: gl_25440
    thread_local EncryptedName<sizeof(normalSource), 0x2DCB7567394DC1ABULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->cancelButton->SetImages(normal, hover, pressed, disabled);
  }
  this->AddChild(this->cancelButton);
}

// OFFSET: 0x00A16C50
// FUNCTION: fn_15282
CUIExchangeUI::CUIExchangeUI()
    : titleBar(nullptr), partnerText(nullptr), backgroundImage(nullptr), firstList(nullptr), ownZenyHighlight(nullptr), ownZenyText(nullptr), ownZenyShadow(nullptr),
      zenyEdit(nullptr), secondList(nullptr), partnerZenyHighlight(nullptr), partnerZenyText(nullptr), partnerZenyShadow(nullptr), logCheckBox(nullptr), screenshotLabel(nullptr),
      lockButton(nullptr), tradeButton(nullptr), cancelButton(nullptr) {
}

// OFFSET: 0x00A16E10
// FUNCTION: fn_15283
ExchangeUI::CUIItem::CUIItem() : CUIListItem(), slotBackground(nullptr), itemIcon(nullptr), quantityText(nullptr), itemNameText(nullptr) {
}

// OFFSET: 0x00A10030
// FUNCTION: fn_15187
CUIExchangeUI* CreateExchangeUI() {
  return new CUIExchangeUI();
}

// OFFSET: 0x00493AD0
// FUNCTION: fn_45549
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateExchangeUI;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10011, factory);
  return true;
}();

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

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
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
        if (this->layoutFlag) {
          return;
        }
        for (ExchangeUI::CUIItem* other : this->items) {
          if (listItem != other) {
            other->SetSelected(0);
          }
        }
        listItem->SetSelected(1);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
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
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::LoadSkin(int resolveSkin) {
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
