#include "UI/SelectPackageItemBox/SelectPackageItemBox.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/MessageTable.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <iterator>
#include <new>

// OFFSET: 0x00A693D0
// FUNCTION: fn_16659
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::ClearItems() {
  for (SelectPackageItemBox::CUIItemInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A69430
// FUNCTION: fn_16660
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::ClearItems() {
  for (SelectPackageItemBox::CUITabInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A694F0
// FUNCTION: fn_16661
void CUISelectPackageItemBox::Slot53() {
  this->unknown_00B4 = 0;
  this->Resize(387, 283);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
  this->SetSkinColors(0, 0, 0, 0);
  this->m_buttonBarStyle = 3;
}

// OFFSET: 0x00A6D060
// FUNCTION: fn_16666
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A6D070
// FUNCTION: fn_16667
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A6D080
// FUNCTION: fn_16668
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A6D0C0
// FUNCTION: fn_16669
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A6DB80
// FUNCTION: fn_16676
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::OnScroll(int position) {
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
    SelectPackageItemBox::CUIItemInfo* item = *it;
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

// OFFSET: 0x00A6DD80
// FUNCTION: fn_16677
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::OnScroll(int position) {
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
    SelectPackageItemBox::CUITabInfo* item = *it;
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

// OFFSET: 0x00A6FBC0
// FUNCTION: fn_16779
void CUISelectPackageItemBox::OnCancel() {
}

// OFFSET: 0x00A6FBE0
// FUNCTION: fn_16780
void CUISelectPackageItemBox::OnOk() {
  const int index = static_cast<int>(this->tabList->selected);
  if (index < 0 || static_cast<unsigned int>(index) >= this->tabList->data.size()) {
    return;
  }

  auto it = this->tabList->data.begin();
  std::advance(it, index);
  const int result = reinterpret_cast<WindowDialogs*>(&g_windowMgr)
                       ->Confirm(FormatOwnedString(LookupMessage(3975), it->name.c_str()).c_str(), 2, true, false, 0, "Message", 280, 120, 0);
  if (result == 187) {
    g_modeMgr.GetGameMode()->SendMsg(325, this->sourceIndex, this->packageId, index, 0);
  }
}

// OFFSET: 0x00A6FD00
// FUNCTION: fn_16781
template <>
__declspec(noinline) void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::ClearData() {
  this->data.clear();
  this->selected = static_cast<unsigned int>(-1);
  this->UpdateScrollBar();
}

// OFFSET: 0x00A6FE10
// FUNCTION: fn_16782
void CUISelectPackageItemBox::OnSelectItem(SelectPackageItemBox::CUIItemInfo*, const ITEM_INFO&) {
}

namespace SelectPackageItemBox {
// OFFSET: 0x00A703A0
// FUNCTION: fn_16784
void CUIItemInfo::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A703B0
// FUNCTION: fn_16785
void CUIItemInfo::Slot16() {
  this->gameItem->showOverlay = 1;
  this->gameItem->Invalidate();
}

// OFFSET: 0x00A705E0
// FUNCTION: fn_16787
void CUITabInfo::Slot16() {
}
}

// OFFSET: 0x00A709E0
// FUNCTION: fn_16789
void CUISelectPackageItemBox::OnSelectTab(SelectPackageItemBox::CUITabInfo*, const SelectPackageItemBox::CTabInfoData& data) {
  this->itemList->ClearData();
  if (!Singleton<CSelectPackageItemMgr>::s_instance) {
    Singleton<CSelectPackageItemMgr>::s_instance = new (std::nothrow) CSelectPackageItemMgr;
  }

  std::optional<std::vector<ITEM_INFO>*> items = Singleton<CSelectPackageItemMgr>::s_instance->FindItems(data.packageId, data.tabIndex);
  if (!items) {
    return;
  }

  for (const ITEM_INFO& item : **items) {
    this->itemList->data.push_back(item);
  }

  this->itemList->UpdateScrollBar();
}
