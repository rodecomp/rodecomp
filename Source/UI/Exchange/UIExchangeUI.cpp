#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Exchange/UIExchangeUI.h"
#include "UI/Window/UIEnums.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Client/Session/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
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

template __declspec(noinline) std::list<ExchangeUI::CItemData>::~list() noexcept;
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetOnSelect(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetBinder(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetImages(const std::string&, const std::string&, const std::string&,
                                                                                    const std::string&);

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
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
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
  {
    CUIVScrollBar* bar = this->scrollBar;
    const std::function<void(int)> onChange = [this](int position) { this->OnScroll(position); };
    bar->onChange = onChange;
  }
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
  if (this->items.empty()) {
    return;
  }
  if (this->data.size() == 0) {
    for (auto* item : this->items) {
      item->index = -1;
      item->Show(0);
    }
    return;
  }

  const int first = this->items.front()->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      const unsigned int shift = first - row;
      if (shift < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - shift, this->items.end());
      }
    }
  }

  auto it = this->items.begin();
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
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
  CUIImageButton* button = this->tradeButton;
  button->disabled = 1;
  button->Invalidate();
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
    if (amount >= value) {
      amount = value < 0 ? 0 : value;
    }

    mode->SendMsg(MM_ADD_EXCHANGE_ITEM, 0, g_session.exchangeMoney + amount, 0, 0);
    mode->SendMsg(MM_CONCLUDE_EXCHANGE_ITEM, 0, 0, 0, 0);
    CUIImageButton* button = this->lockButton;
    button->disabled = 1;
    button->Invalidate();
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
      if (g_windowMgr.itemDropCntWnd != nullptr) {
        g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 5, 0, 0, 0);
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
      if (quantity <= 0) {
        list->AddData(ExchangeUI::CItemData(ITEM_INFO(), 0), false);
      } else {
        list->AddData(ExchangeUI::CItemData(item, quantity), false);
      }
    }
    this->firstList->UpdateScrollBar();

    this->secondList->ClearData();
    for (int index = 0; index < 10; ++index) {
      ITEM_INFO item = g_session.GetItemSlot(index);
      ExchangeUI::CItemListBox* list = this->secondList;
      const int quantity = item.quantity;
      if (quantity <= 0) {
        list->AddData(ExchangeUI::CItemData(ITEM_INFO(), 0), false);
      } else {
        list->AddData(ExchangeUI::CItemData(item, quantity), false);
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
  item->itemNameText->SetTooltip("");

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

// OFFSET: 0x00A149B0
// FUNCTION: fn_15192
void ExchangeUI::CUIItem::OnCreate(int width, int height) {
  this->slotBackground = new CUIStaticImage();
  this->slotBackground->SetSize(32, 32);
  this->slotBackground->Move(9, 0);
  this->slotBackground->SetImage(std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\itemwin_mid.bmp", 0x75ADC57D87EFBD8FULL)));
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
  this->quantityText->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0xC517FB932D95BB6DULL)));
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
  this->itemNameText->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x23BBD58F8B5FF5C9ULL)));
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
    const auto titleRightSource = OBFUSCATE_SOURCE("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0x17A9E5BF39AD4737ULL);
    // GLOBAL: gl_25394
    thread_local EncryptedName<sizeof(titleRightSource), 0x17A9E5BF39AD4737ULL> titleRightName(titleRightSource.data);
    const std::string titleRight(titleRightName.Text());
    const char titleMiddleSource[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0x03772763F96D9DA5ULL);
    // GLOBAL: gl_25395
    thread_local EncryptedName<sizeof(titleMiddleSource), 0x03772763F96D9DA5ULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const char titleLeftSource[] = OBFUSCATE_SOURCE_CHARS(49, "유저인터페이스\\basic_interface\\titlebar_left.bmp", 0x0B7109397F6B1387ULL);
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
    const char titleFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Gulim", 0x13E33FA1A591AB67ULL);
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
    const char partnerFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Gulim", 0xCD8F077123FB7D6FULL);
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
    const char backgroundSource[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\exchange_bg2.bmp", 0x9949BFD527532943ULL);
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
    const char downSource[] = OBFUSCATE_SOURCE_CHARS(31, "유저인터페이스\\scroll0down.bmp", 0xD97D5DBDBB2F2709ULL);
    // GLOBAL: gl_25400
    thread_local EncryptedName<sizeof(downSource), 0xD97D5DBDBB2F2709ULL> downName(downSource);
    const std::string down(downName.Text());
    const char upSource[] = OBFUSCATE_SOURCE_CHARS(29, "유저인터페이스\\scroll0up.bmp", 0xED796785D1DF137FULL);
    // GLOBAL: gl_25401
    thread_local EncryptedName<sizeof(upSource), 0xED796785D1DF137FULL> upName(upSource);
    const std::string up(upName.Text());
    this->firstList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char backgroundSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\scroll0mid.bmp", 0xA5FD29D7AB6DC59FULL);
    // GLOBAL: gl_25402
    thread_local EncryptedName<sizeof(backgroundSource), 0xA5FD29D7AB6DC59FULL> backgroundName(backgroundSource);
    this->firstList->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const char thumbBottomSource[] = OBFUSCATE_SOURCE_CHARS(35, "유저인터페이스\\scroll0bar_down.bmp", 0xDD3913D5D7F97BE1ULL);
    // GLOBAL: gl_25403
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xDD3913D5D7F97BE1ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const char thumbMiddleSource[] = OBFUSCATE_SOURCE_CHARS(34, "유저인터페이스\\scroll0bar_mid.bmp", 0x050505679903579BULL);
    // GLOBAL: gl_25404
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x050505679903579BULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const char thumbTopSource[] = OBFUSCATE_SOURCE_CHARS(33, "유저인터페이스\\scroll0bar_up.bmp", 0xF14F79170939C7F1ULL);
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
    const char zenySource[] = OBFUSCATE_SOURCE_CHARS(2, "0", 0xBF2B4B31397FF5A7ULL);
    // GLOBAL: gl_25406
    thread_local EncryptedName<sizeof(zenySource), 0xBF2B4B31397FF5A7ULL> zenyName(zenySource);
    this->ownZenyHighlight->SetText(zenyName.Text());
  }
  this->ownZenyHighlight->SetFitText(0);
  this->ownZenyHighlight->SetAlign(2, 6);
  {
    CUIStaticText* text = this->ownZenyHighlight;
    const char zenyFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Arial", 0x83EFBFBB37EDBDBDULL);
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
    const char zenySource[] = OBFUSCATE_SOURCE_CHARS(2, "0", 0x39FF7FB50DEB998FULL);
    // GLOBAL: gl_25408
    thread_local EncryptedName<sizeof(zenySource), 0x39FF7FB50DEB998FULL> zenyName(zenySource);
    this->ownZenyText->SetText(zenyName.Text());
  }
  this->ownZenyText->SetFitText(0);
  this->ownZenyText->SetAlign(2, 6);
  {
    CUIStaticText* text = this->ownZenyText;
    const char zenyFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Arial", 0x2FE993D1DDE1F197ULL);
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
    const char zenySource[] = OBFUSCATE_SOURCE_CHARS(2, "0", 0xAD075B8DB599F1CDULL);
    // GLOBAL: gl_25410
    thread_local EncryptedName<sizeof(zenySource), 0xAD075B8DB599F1CDULL> zenyName(zenySource);
    this->ownZenyShadow->SetText(zenyName.Text());
  }
  this->ownZenyShadow->SetFitText(0);
  this->ownZenyShadow->SetAlign(2, 6);
  {
    CUIStaticText* text = this->ownZenyShadow;
    const char zenyFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Arial", 0x2F8B0F37118D157FULL);
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
    const char downSource[] = OBFUSCATE_SOURCE_CHARS(31, "유저인터페이스\\scroll0down.bmp", 0x4941FD9F21BBB9FDULL);
    // GLOBAL: gl_25412
    thread_local EncryptedName<sizeof(downSource), 0x4941FD9F21BBB9FDULL> downName(downSource);
    const std::string down(downName.Text());
    const char upSource[] = OBFUSCATE_SOURCE_CHARS(29, "유저인터페이스\\scroll0up.bmp", 0x35E5656B5DEDCD75ULL);
    // GLOBAL: gl_25413
    thread_local EncryptedName<sizeof(upSource), 0x35E5656B5DEDCD75ULL> upName(upSource);
    const std::string up(upName.Text());
    this->secondList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char backgroundSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\scroll0mid.bmp", 0xE7978FB5A94DD7FDULL);
    // GLOBAL: gl_25414
    thread_local EncryptedName<sizeof(backgroundSource), 0xE7978FB5A94DD7FDULL> backgroundName(backgroundSource);
    this->secondList->scrollBar->SetBackground(std::string(backgroundName.Text()));
  }
  {
    const char thumbBottomSource[] = OBFUSCATE_SOURCE_CHARS(35, "유저인터페이스\\scroll0bar_down.bmp", 0x7115D9F973F913C9ULL);
    // GLOBAL: gl_25415
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x7115D9F973F913C9ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const char thumbMiddleSource[] = OBFUSCATE_SOURCE_CHARS(34, "유저인터페이스\\scroll0bar_mid.bmp", 0xCB1D253B7FE1F563ULL);
    // GLOBAL: gl_25416
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xCB1D253B7FE1F563ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const char thumbTopSource[] = OBFUSCATE_SOURCE_CHARS(33, "유저인터페이스\\scroll0bar_up.bmp", 0xB7FD8F670FC7B96BULL);
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
    const char zenySource[] = OBFUSCATE_SOURCE_CHARS(2, "0", 0xE55723712993051DULL);
    // GLOBAL: gl_25418
    thread_local EncryptedName<sizeof(zenySource), 0xE55723712993051DULL> zenyName(zenySource);
    this->partnerZenyHighlight->SetText(zenyName.Text());
  }
  this->partnerZenyHighlight->SetFitText(0);
  this->partnerZenyHighlight->SetAlign(2, 6);
  {
    CUIStaticText* text = this->partnerZenyHighlight;
    const char zenyFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Arial", 0x793F370985ED674FULL);
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
    const char zenySource[] = OBFUSCATE_SOURCE_CHARS(2, "0", 0x0F15B16B5BAFD57FULL);
    // GLOBAL: gl_25420
    thread_local EncryptedName<sizeof(zenySource), 0x0F15B16B5BAFD57FULL> zenyName(zenySource);
    this->partnerZenyText->SetText(zenyName.Text());
  }
  this->partnerZenyText->SetFitText(0);
  this->partnerZenyText->SetAlign(2, 6);
  {
    CUIStaticText* text = this->partnerZenyText;
    const char zenyFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Arial", 0x7DE349D717D7F123ULL);
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
    const char zenySource[] = OBFUSCATE_SOURCE_CHARS(2, "0", 0x7321C3814F8799E1ULL);
    // GLOBAL: gl_25422
    thread_local EncryptedName<sizeof(zenySource), 0x7321C3814F8799E1ULL> zenyName(zenySource);
    this->partnerZenyShadow->SetText(zenyName.Text());
  }
  this->partnerZenyShadow->SetFitText(0);
  this->partnerZenyShadow->SetAlign(2, 6);
  {
    CUIStaticText* text = this->partnerZenyShadow;
    const char zenyFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Arial", 0x7941D76335CFA14FULL);
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
    const char disabledSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\checkbox_0.bmp", 0x452FEF0D7FC95565ULL);
    // GLOBAL: gl_25424
    thread_local EncryptedName<sizeof(disabledSource), 0x452FEF0D7FC95565ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\checkbox_0.bmp", 0x9FED7DA189490D5DULL);
    // GLOBAL: gl_25425
    thread_local EncryptedName<sizeof(pressedSource), 0x9FED7DA189490D5DULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\checkbox_1.bmp", 0x839B27DF8F5967B3ULL);
    // GLOBAL: gl_25426
    thread_local EncryptedName<sizeof(hoverSource), 0x839B27DF8F5967B3ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\checkbox_0.bmp", 0xF3CB6F7711A71B6DULL);
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
    const char labelFontSource[] = OBFUSCATE_SOURCE_CHARS(6, "Gulim", 0x176F138D5DD7F365ULL);
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
    const char disabledSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\btn_ok_dis.bmp", 0x4F99DF03C3EF31B7ULL);
    // GLOBAL: gl_25429
    thread_local EncryptedName<sizeof(disabledSource), 0x4F99DF03C3EF31B7ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = OBFUSCATE_SOURCE_CHARS(28, "유저인터페이스\\btn_ok_a.bmp", 0x2D51E3B7236175EDULL);
    // GLOBAL: gl_25430
    thread_local EncryptedName<sizeof(pressedSource), 0x2D51E3B7236175EDULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = OBFUSCATE_SOURCE_CHARS(28, "유저인터페이스\\btn_ok_b.bmp", 0x0F41EBBDA32D1B17ULL);
    // GLOBAL: gl_25431
    thread_local EncryptedName<sizeof(hoverSource), 0x0F41EBBDA32D1B17ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = OBFUSCATE_SOURCE_CHARS(26, "유저인터페이스\\btn_ok.bmp", 0xFBD599E117C547C1ULL);
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
    const char disabledSource[] = OBFUSCATE_SOURCE_CHARS(36, "유저인터페이스\\btn_exchange_dis.bmp", 0x4F637DC51307B759ULL);
    // GLOBAL: gl_25433
    thread_local EncryptedName<sizeof(disabledSource), 0x4F637DC51307B759ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = OBFUSCATE_SOURCE_CHARS(34, "유저인터페이스\\btn_exchange_a.bmp", 0xE52DF30DCBAF7125ULL);
    // GLOBAL: gl_25434
    thread_local EncryptedName<sizeof(pressedSource), 0xE52DF30DCBAF7125ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = OBFUSCATE_SOURCE_CHARS(34, "유저인터페이스\\btn_exchange_b.bmp", 0xCFF3899D95AF355FULL);
    // GLOBAL: gl_25435
    thread_local EncryptedName<sizeof(hoverSource), 0xCFF3899D95AF355FULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = OBFUSCATE_SOURCE_CHARS(32, "유저인터페이스\\btn_exchange.bmp", 0x9171997D6963C9F7ULL);
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
    const char disabledSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\btn_cancel.bmp", 0x33133B977BCD03CDULL);
    // GLOBAL: gl_25437
    thread_local EncryptedName<sizeof(disabledSource), 0x33133B977BCD03CDULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const char pressedSource[] = OBFUSCATE_SOURCE_CHARS(32, "유저인터페이스\\btn_cancel_a.bmp", 0x8D1D0FF1AF2B13F7ULL);
    // GLOBAL: gl_25438
    thread_local EncryptedName<sizeof(pressedSource), 0x8D1D0FF1AF2B13F7ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const char hoverSource[] = OBFUSCATE_SOURCE_CHARS(32, "유저인터페이스\\btn_cancel_b.bmp", 0xB39FA7917BCFBB8DULL);
    // GLOBAL: gl_25439
    thread_local EncryptedName<sizeof(hoverSource), 0xB39FA7917BCFBB8DULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const char normalSource[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\btn_cancel.bmp", 0x2DCB7567394DC1ABULL);
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
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::LoadSkin(bool resolveSkin) {
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
