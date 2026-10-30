#include "UI/Exchange/UIExchangeUI.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <functional>
#include <string>
#include <windows.h>

template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetOnSelect(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetBinder(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetImages(const std::string&, const std::string&, const std::string&,
                                                                                    const std::string&);
template <>
void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::UpdateScrollBar();

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
