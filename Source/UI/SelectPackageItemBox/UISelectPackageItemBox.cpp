#include "UI/SelectPackageItemBox/SelectPackageItemBox.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Render/Renderer.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <new>

template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::AddNewItem();
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::AddNewItem();
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Slot53(int resolveSkin);
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Slot53(int resolveSkin);
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Resize(int width, int height);
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Resize(int width, int height);

// OFFSET: 0x00A684E0
// FUNCTION: fn_16646
template <>
CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::CUIListBoxEx()
    : CUIControl(), unknown_0084(), unknown_0088(), unknown_008C(), imageNames(), disabledName(), scrollBar(nullptr),
      itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), unknown_00C5{}, items(), data(),
      selected(static_cast<unsigned int>(-1)), unknown_00E0(1), rebind(0), unknown_00E2{}, onSelect(), binder() {
}

// OFFSET: 0x00A686A0
// FUNCTION: fn_16647
template <>
CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::CUIListBoxEx()
    : CUIControl(), unknown_0084(), unknown_0088(), unknown_008C(), imageNames(), disabledName(), scrollBar(nullptr),
      itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), unknown_00C5{}, items(), data(),
      selected(static_cast<unsigned int>(-1)), unknown_00E0(1), rebind(0), unknown_00E2{}, onSelect(), binder() {
}

// OFFSET: 0x00A68860
// FUNCTION: fn_16648
template <>
CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::~CUIListBoxEx() {
}

// OFFSET: 0x00A689C0
// FUNCTION: fn_16649
template <>
CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::~CUIListBoxEx() {
}

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

template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::OnScroll(int position);
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::OnScroll(int position);

// OFFSET: 0x00A695F0
// FUNCTION: fn_16662
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A696C0
// FUNCTION: fn_16663
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
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

// INLINED
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (SelectPackageItemBox::CUIItemInfo* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  SelectPackageItemBox::CUIItemInfo* selectedItem = dynamic_cast<SelectPackageItemBox::CUIItemInfo*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    std::advance(node, static_cast<int>(this->selected));
    this->onSelect(selectedItem, *node);
  }
}

// INLINED
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (SelectPackageItemBox::CUITabInfo* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  SelectPackageItemBox::CUITabInfo* selectedTab = dynamic_cast<SelectPackageItemBox::CUITabInfo*>(item);
  if (!selectedTab) {
    return;
  }
  this->selected = selectedTab->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    std::advance(node, static_cast<int>(this->selected));
    this->onSelect(selectedTab, *node);
  }
}

// OFFSET: 0x00A6D1A0
// FUNCTION: fn_16670
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (SelectPackageItemBox::CUIItemInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->rowHeight <= this->geometry.height) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count, nullptr);
    for (SelectPackageItemBox::CUIItemInfo*& slot : this->items) {
      SelectPackageItemBox::CUIItemInfo* item = new SelectPackageItemBox::CUIItemInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (SelectPackageItemBox::CUIItemInfo* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        this->SelectItem(listItem);
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A6D400
// FUNCTION: fn_16671
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (SelectPackageItemBox::CUITabInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->rowHeight <= this->geometry.height) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count, nullptr);
    for (SelectPackageItemBox::CUITabInfo*& slot : this->items) {
      SelectPackageItemBox::CUITabInfo* item = new SelectPackageItemBox::CUITabInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (SelectPackageItemBox::CUITabInfo* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        this->SelectItem(listItem);
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A6D660
// FUNCTION: fn_16672
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Slot53(int resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SelectPackageItemBox::CUIItemInfo* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A6D870
// FUNCTION: fn_16673
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Slot53(int resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SelectPackageItemBox::CUITabInfo* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A6DA80
// FUNCTION: fn_16674
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A6DB00
// FUNCTION: fn_16675
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
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

template <class T, class D> struct ListHighlightCallback {
  virtual ~ListHighlightCallback() = default;

  /* 0004 */ CUIListBoxEx<T, D>* list;

  void operator()(CUIListItem* const& item);
};

template <class T, class D> struct ListSelectCallback {
  virtual ~ListSelectCallback() = default;

  /* 0004 */ CUIListBoxEx<T, D>* list;

  void operator()(CUIListItem* const& item);
};

// OFFSET: 0x00A6E970
// FUNCTION: fn_16709
template <>
void ListHighlightCallback<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::operator()(CUIListItem* const& item) {
  CUIListItem* highlighted = item;
  for (SelectPackageItemBox::CUIItemInfo* entry : this->list->items) {
    entry->SetHighlighted(highlighted == entry);
  }
}

// OFFSET: 0x00A6E9C0
// FUNCTION: fn_16710
template <>
void ListHighlightCallback<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::operator()(CUIListItem* const& item) {
  CUIListItem* highlighted = item;
  for (SelectPackageItemBox::CUITabInfo* entry : this->list->items) {
    entry->SetHighlighted(highlighted == entry);
  }
}

// OFFSET: 0x00A6EA90
// FUNCTION: fn_16715
template <>
void ListSelectCallback<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::operator()(CUIListItem* const& item) {
  CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>* list = this->list;
  CUIListItem* selection = item;
  if (list->unknown_00C4) {
    return;
  }

  for (SelectPackageItemBox::CUIItemInfo* entry : list->items) {
    if (selection != entry) {
      entry->SetSelected(false);
    }
  }

  selection->SetSelected(true);
  if (selection->index == static_cast<int>(list->selected) && list->unknown_00E0) {
    return;
  }

  SelectPackageItemBox::CUIItemInfo* selectedItem = dynamic_cast<SelectPackageItemBox::CUIItemInfo*>(selection);
  if (!selectedItem) {
    return;
  }

  const int selectedIndex = selectedItem->index;
  list->selected = selectedIndex;
  if (list->onSelect && selectedIndex >= 0 && static_cast<unsigned int>(selectedIndex) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = selectedIndex; index > 0; --index) {
      ++node;
    }
    list->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A6EB70
// FUNCTION: fn_16716
template <>
void ListSelectCallback<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::operator()(CUIListItem* const& item) {
  CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>* list = this->list;
  CUIListItem* selection = item;
  if (list->unknown_00C4) {
    return;
  }

  for (SelectPackageItemBox::CUITabInfo* entry : list->items) {
    if (selection != entry) {
      entry->SetSelected(false);
    }
  }

  selection->SetSelected(true);
  if (selection->index == static_cast<int>(list->selected) && list->unknown_00E0) {
    return;
  }

  SelectPackageItemBox::CUITabInfo* selectedTab = dynamic_cast<SelectPackageItemBox::CUITabInfo*>(selection);
  if (!selectedTab) {
    return;
  }

  const int selectedIndex = selectedTab->index;
  list->selected = selectedIndex;
  if (list->onSelect && selectedIndex >= 0 && static_cast<unsigned int>(selectedIndex) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = selectedIndex; index > 0; --index) {
      ++node;
    }
    list->onSelect(selectedTab, *node);
  }
}

// OFFSET: 0x00A6F3F0
// FUNCTION: fn_16771
SelectPackageItemBox::CUIItemInfo::CUIItemInfo() : image(nullptr), gameItem(nullptr), nameText(nullptr) {
}

// OFFSET: 0x00A6F580
// FUNCTION: fn_16773
SelectPackageItemBox::CUITabInfo::CUITabInfo() : nameText(nullptr) {
}

// OFFSET: 0x00A6FBC0
// FUNCTION: fn_16779
void CUISelectPackageItemBox::OnCancel() {
}

// OFFSET: 0x00A6FBD0
// FUNCTION: fn_45494
void CUISelectPackageItemBox::OnClickClose() {
  g_windowMgr.CloseWindow(0x2731);
}

// OFFSET: 0x00A6FBE0
// FUNCTION: fn_16780
void CUISelectPackageItemBox::OnOk() {
  const int index = static_cast<int>(this->tabList->selected);
  if (index < 0 || static_cast<unsigned int>(index) >= this->tabList->data.size()) {
    return;
  }

  auto it = this->tabList->data.begin();
  if (index > 0) {
    int count = index;
    do {
      ++it;
      --count;
    } while (count > 0);
  }
  const int result = g_windowMgr
                       .Confirm(FormatOwnedString(LookupMessage(3975), it->name.c_str()).c_str(), 2, true, false, 0, "Message", 280, 120, 0);
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

// OFFSET: 0x00A6FE00
// FUNCTION: fn_45493
void CUISelectPackageItemBox::OnClose() {
  g_windowMgr.CloseWindow(0x2731);
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

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  template <typename Byte>
  explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
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

// INLINED
template <std::uint64_t Key, std::size_t Length, typename Byte>
const char* DecryptName(const Byte (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

// OFFSET: 0x00A69790
// FUNCTION: fn_16664
void SelectPackageItemBox::CUIItemInfo::Slot15(int width, int height) {
  this->image = new CUIStaticImage();
  this->image->SetSize(200, 34);
  this->image->Move(0, 0);
  const std::uint8_t imageSource[] = {0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21,
                                      0xAA, 0x6C, 0x4B, 0x4A, 0xD8, 0xC9, 0x2D, 0x1C, 0xBA, 0x62, 0x46, 0x48, 0xDE, 0xE1, 0x34,
                                      0x09, 0xBC, 0x64, 0x78, 0x43, 0xD2, 0xCE, 0x29, 0x53, 0xBB, 0x64, 0x57, 0x2F};
  // GLOBAL: gl_25124
  thread_local EncryptedName<sizeof(imageSource), 0xD97D5DBDBB2F2709ULL> imageName(imageSource);
  std::string background(imageName.Text());
  this->image->SetImage(background);
  this->AddChild(this->image);

  this->gameItem = new CUIGameItem();
  this->gameItem->SetMode(0);
  this->gameItem->SetScale(100);
  this->gameItem->Move(6, 5);
  this->gameItem->unknown_0084 = 1;
  this->gameItem->unknown_0085 = 1;
  this->AddChild(this->gameItem);

  this->nameText = new CUIStaticText();
  this->nameText->SetSize(148, 24);
  this->nameText->Move(45, 5);
  this->nameText->SetText("", 0);
  this->nameText->SetFitText(1);
  this->nameText->SetAlign(0, 6);
  const std::uint8_t nameFontSource[] = {0x38, 0x66, 0xB3, 0xB8, 0xE8, 0x67};
  // GLOBAL: gl_25125
  thread_local EncryptedName<sizeof(nameFontSource), 0xED796785D1DF137FULL> nameFont(nameFontSource);
  this->nameText->SetFont(std::string(nameFont.Text()));
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetColor(0);
  this->nameText->SetShadow(1, 0xE7E7E7);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);
}

// OFFSET: 0x00A69DB0
// FUNCTION: fn_45491
void CUISelectPackageItemBox::Slot15(int width, int height) {
  this->title = new CUITitleBar();
  this->title->SetSize(387, 17);
  this->title->Move(0, 0);
  this->title->SetImages(
    std::string([] {
      const char source[] = {
        '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\x93',
        '\x90', '\xB4', '\x50', '\x6A', '\x48', '\x10', '\x21', '\x85', '\x94', '\xB5', '\x5F', '\x68', '\x74', '\x1C', '\x13', '\x85',
        '\x98', '\xB3', '\x55', '\x6C', '\x75', '\x18', '\x3D', '\xAE', '\x9D', '\xA2', '\x5F', '\x7D', '\x39', '\x1B', '\x22', '\x81',
        '\xF1'
      };
      thread_local EncryptedName<sizeof(source), 0xF14F79170939C7F1ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xC7',
        '\xFE', '\xB6', '\x04', '\xC8', '\x88', '\x40', '\x93', '\xD1', '\xFA', '\xB7', '\x0B', '\xCA', '\xB4', '\x4C', '\xA1', '\xD1',
        '\xF6', '\xB1', '\x01', '\xCE', '\xB5', '\x48', '\x8F', '\xFA', '\xF2', '\xAC', '\x09', '\x85', '\xB5', '\x44', '\x8D', '\xA5'
      };
      thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x21', '\xD4', '\x39', '\x2D', '\x15', '\xDD', '\xFC', '\x10', '\x27', '\x9F', '\x39', '\x1B', '\x68', '\xA9', '\x65', '\xBF',
        '\x80', '\x08', '\x90', '\xB4', '\x8A', '\x7A', '\x57', '\xA9', '\x84', '\x09', '\x9F', '\xB6', '\xB6', '\x76', '\x65', '\xA9',
        '\x88', '\x0F', '\x95', '\xB2', '\xB7', '\x72', '\x4B', '\x82', '\x93', '\x12', '\x9E', '\xBF', '\xA1', '\x3D', '\x5B', '\xB0',
        '\x91', '\x7B'
      };
      thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> name(source);
      return name.Text();
    }())
  );
  this->AddChild(this->title);

  CUIStaticText* titleText = new CUIStaticText();
  titleText->SetSize(250, 17);
  titleText->Move(18, 0);
  titleText->SetText("", 0);
  titleText->SetFitText(0);
  titleText->SetAlign(0, 6);
  titleText->SetFont(
    std::string([] {
      const std::uint8_t source[] = {0xDC, 0x22, 0x6F, 0xF0, 0x0A, 0x05};
      thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> name(source);
      return name.Text();
    }())
  );
  titleText->SetFontHeight(12);
  titleText->SetBold(0);
  titleText->SetColor(0);
  titleText->SetShadow(1, 0xFFFFFF);
  titleText->SetOutline(0, 0xFFFFFF);
  this->title->titleText = titleText;
  this->title->AddChild(titleText);

  CUIImageButton* systemButton = new CUIImageButton();
  systemButton->SetSize(11, 11);
  systemButton->Move(3, 3);
  systemButton->SetOnClick([this]() { this->OnCancel(); });
  systemButton->disabled = 0;
  systemButton->Invalidate();
  systemButton->SetImages(
    std::string([] {
      const char source[] = {
        '\x7D', '\x12', '\x2D', '\xCD', '\x7B', '\x71', '\x2A', '\x4E', '\x7B', '\x59', '\x2D', '\xFB', '\x06', '\x05', '\xB3', '\xE1',
        '\xDC', '\xCE', '\x84', '\x54', '\xE4', '\xD6', '\x81', '\xF7', '\xD8', '\xCF', '\x8B', '\x56', '\xD8', '\xDA', '\xB3', '\xF0',
        '\xC4', '\xCE', '\xB2', '\x55', '\xDA', '\xCC', '\x8A', '\xDC', '\xD2', '\xDB', '\x8B', '\x19', '\xD9', '\xD2', '\x9F', '\x83'
      };
      thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x67', '\x5A', '\xBF', '\xC3', '\xF1', '\x85', '\xEE', '\x72', '\x61', '\x11', '\xBF', '\xF5', '\x8C', '\xF1', '\x77', '\xDD',
        '\xC6', '\x86', '\x16', '\x5A', '\x6E', '\x22', '\x45', '\xCB', '\xC2', '\x87', '\x19', '\x58', '\x52', '\x2E', '\x77', '\xCC',
        '\xDE', '\x86', '\x20', '\x5B', '\x50', '\x38', '\x4E', '\xE0', '\xC8', '\x9B', '\x51', '\x5B', '\x5C', '\x3B', '\x2B'
      };
      thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x09', '\x5A', '\x9F', '\x71', '\x4F', '\x1B', '\x7E', '\xEE', '\x0F', '\x11', '\x9F', '\x47', '\x32', '\x6F', '\xE7', '\x41',
        '\xA8', '\x86', '\x36', '\xE8', '\xD0', '\xBC', '\xD5', '\x57', '\xAC', '\x87', '\x39', '\xEA', '\xEC', '\xB0', '\xE7', '\x50',
        '\xB0', '\x86', '\x00', '\xE9', '\xEE', '\xA6', '\xDE', '\x7C', '\xA6', '\x9B', '\x71', '\xE9', '\xE2', '\xA5', '\xBB'
      };
      thread_local EncryptedName<sizeof(source), 0x23BBD58F8B5FF5C9ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xAD', '\x14', '\x55', '\xD7', '\x53', '\x35', '\xD2', '\x08', '\xAB', '\x5F', '\x55', '\xE1', '\x2E', '\x41', '\x4B', '\xA7',
        '\x0C', '\xC8', '\xFC', '\x4E', '\xCC', '\x92', '\x79', '\xB1', '\x08', '\xC9', '\xF3', '\x4C', '\xF0', '\x9E', '\x4B', '\xB6',
        '\x14', '\xC8', '\xCA', '\x4F', '\xF2', '\x88', '\x72', '\x9A', '\x02', '\xDD', '\xF3', '\x03', '\xF1', '\x96', '\x67', '\xC5'
      };
      thread_local EncryptedName<sizeof(source), 0xC517FB932D95BB6DULL> name(source);
      return name.Text();
    }())
  );
  this->title->AddChild(systemButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(373, 3);
  closeButton->SetOnClick([this]() { this->OnClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  closeButton->SetImages(
    std::string([] {
      const char source[] = {
        '\x65', '\x32', '\xAD', '\x03', '\xA3', '\xE9', '\xB2', '\xCE', '\x63', '\x79', '\xAD', '\x35', '\xDE', '\x9D', '\x2B', '\x61',
        '\xC4', '\xEE', '\x04', '\x9A', '\x3C', '\x4E', '\x19', '\x77', '\xC0', '\xEF', '\x0B', '\x98', '\x00', '\x42', '\x2B', '\x70',
        '\xDC', '\xEE', '\x32', '\x9A', '\x0F', '\x48', '\x04', '\x66', '\xFA', '\xF2', '\x0B', '\x9F', '\x4D', '\x45', '\x1A', '\x73',
        '\xA5'
      };
      thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x47', '\xBC', '\xAB', '\x85', '\xF9', '\xC7', '\xB4', '\xC6', '\x41', '\xF7', '\xAB', '\xB3', '\x84', '\xB3', '\x2D', '\x69',
        '\xE6', '\x60', '\x02', '\x1C', '\x66', '\x60', '\x1F', '\x7F', '\xE2', '\x61', '\x0D', '\x1E', '\x5A', '\x6C', '\x2D', '\x78',
        '\xFE', '\x60', '\x34', '\x1C', '\x55', '\x66', '\x02', '\x6E', '\xD8', '\x7C', '\x05', '\x51', '\x5B', '\x64', '\x01', '\x0B'
      };
      thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xA7', '\x04', '\x51', '\x5F', '\x61', '\xF1', '\x26', '\xDE', '\xA1', '\x4F', '\x51', '\x69', '\x1C', '\x85', '\xBF', '\x71',
        '\x06', '\xD8', '\xF8', '\xC6', '\xFE', '\x56', '\x8D', '\x67', '\x02', '\xD9', '\xF7', '\xC4', '\xC2', '\x5A', '\xBF', '\x60',
        '\x1E', '\xD8', '\xCE', '\xC6', '\xCD', '\x50', '\x90', '\x76', '\x38', '\xC4', '\xFF', '\x8B', '\xC3', '\x52', '\x93', '\x13'
      };
      thread_local EncryptedName<sizeof(source), 0x13E33FA1A591AB67ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xF7', '\xE8', '\x6D', '\xC3', '\x7F', '\x2B', '\x6C', '\xDA', '\xF1', '\xA3', '\x6D', '\xF5', '\x02', '\x5F', '\xF5', '\x75',
        '\x56', '\x34', '\xC4', '\x5A', '\xE0', '\x8C', '\xC7', '\x63', '\x52', '\x35', '\xCB', '\x58', '\xDC', '\x80', '\xF5', '\x64',
        '\x4E', '\x34', '\xF2', '\x5A', '\xD3', '\x8A', '\xDA', '\x72', '\x68', '\x28', '\xCB', '\x5F', '\x91', '\x87', '\xC4', '\x67',
        '\x37'
      };
      thread_local EncryptedName<sizeof(source), 0x17A9E5BF39AD4737ULL> name(source);
      return name.Text();
    }())
  );
  this->title->SetSecondButton(closeButton);

  this->tabList = new CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>();
  this->tabList->itemWidth = 159;
  this->tabList->rowHeight = 34;
  this->tabList->SetSize(172, 238);
  this->tabList->Move(1, 17);
  this->tabList->SetOnSelect([this](SelectPackageItemBox::CUITabInfo* tab, const SelectPackageItemBox::CTabInfoData& data) { this->OnSelectTab(tab, data); });
  this->tabList->SetBinder([this](SelectPackageItemBox::CUITabInfo* tab, const SelectPackageItemBox::CTabInfoData& data) { this->BindTab(tab, data); });
  this->tabList->SetImages(
    std::string([] {
      const char source[] = {
        '\x83', '\x86', '\x93', '\xDD', '\x15', '\x71', '\x8C', '\x54', '\x85', '\xCD', '\x93', '\xEB', '\x68', '\x05', '\x15', '\xEA',
        '\x26', '\x45', '\x36', '\x44', '\xA1', '\xCF', '\x28', '\xFA', '\x28', '\x48', '\x34', '\x42', '\x89', '\xDD', '\x3D', '\xF7',
        '\x1C', '\x5A', '\x36', '\x53', '\x8A', '\xD3', '\x20', '\xEA', '\x37', '\x07', '\x31', '\x4A', '\xA5', '\xBF'
      };
      thread_local EncryptedName<sizeof(source), 0x9949BFD527532943ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xAF', '\xD2', '\x3B', '\xD9', '\xB1', '\xC9', '\x4A', '\x00', '\xA9', '\x99', '\x3B', '\xEF', '\xCC', '\xBD', '\xD3', '\xBE',
        '\x0A', '\x11', '\x9E', '\x40', '\x05', '\x77', '\xEE', '\xAE', '\x04', '\x1C', '\x9C', '\x46', '\x2D', '\x65', '\xFB', '\xA3',
        '\x30', '\x0E', '\x9E', '\x57', '\x2E', '\x77', '\xFD', '\xA8', '\x1C', '\x0E', '\xD5', '\x41', '\x1C', '\x77', '\x8F'
      };
      thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xA9', '\xB0', '\xB7', '\xAF', '\x5D', '\x5B', '\xBE', '\xAE', '\xAF', '\xFB', '\xB7', '\x99', '\x20', '\x2F', '\x27', '\x10',
        '\x0C', '\x73', '\x12', '\x36', '\xE9', '\xE5', '\x1A', '\x00', '\x02', '\x7E', '\x10', '\x30', '\xC1', '\xF7', '\x0F', '\x0D',
        '\x36', '\x6C', '\x12', '\x21', '\xC2', '\xF9', '\x12', '\x10', '\x1D', '\x31', '\x15', '\x38', '\xED', '\x95'
      };
      thread_local EncryptedName<sizeof(source), 0x637B959D55771F69ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x91', '\x24', '\x0D', '\xFB', '\x7B', '\xF3', '\x84', '\x1C', '\x97', '\x6F', '\x0D', '\xCD', '\x06', '\x87', '\x1D', '\xA2',
        '\x34', '\xE7', '\xA8', '\x62', '\xCF', '\x4D', '\x20', '\xB2', '\x3A', '\xEA', '\xAA', '\x64', '\xE7', '\x5F', '\x35', '\xBF',
        '\x0E', '\xF8', '\xA8', '\x75', '\xE4', '\x51', '\x28', '\xA2', '\x25', '\xA5', '\xAF', '\x6C', '\xCB', '\x3D'
      };
      thread_local EncryptedName<sizeof(source), 0xD1413DBB01CD8B51ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->itemX = 13;
  this->tabList->itemY = 0;
  this->tabList->scrollBar->SetSize(13, 238);
  this->tabList->scrollBar->Move(0, 0);
  this->tabList->scrollBar->step = 34;
  this->tabList->scrollBar->SetButtonImages(
    std::string([] {
      const char source[] = {
        '\x79', '\xAA', '\xB7', '\xB5', '\xCB', '\xD3', '\x4A', '\xB8', '\x7F', '\xE1', '\xB7', '\x83', '\xB6', '\xA7', '\xD3', '\x06',
        '\xDA', '\x77', '\x18', '\x23', '\x67', '\x2D', '\xFA', '\x05', '\x97', '\x67', '\x1A', '\x3F', '\x0B'
      };
      thread_local EncryptedName<sizeof(source), 0x758F1D0B4F7705B9ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xA5', '\xB6', '\x85', '\xF3', '\xED', '\xED', '\xDE', '\x44', '\xA3', '\xFD', '\x85', '\xC5', '\x90', '\x99', '\x47', '\xFA',
        '\x06', '\x6B', '\x2A', '\x65', '\x41', '\x13', '\x7F', '\xE6', '\x12', '\x77', '\x6B', '\x6B', '\x40', '\x53', '\x1B'
      };
      thread_local EncryptedName<sizeof(source), 0x891B232D09451965ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->scrollBar->SetBackground(
    std::string([] {
      const char source[] = {
        '\x55', '\x20', '\x21', '\xF7', '\x9D', '\xC7', '\xA4', '\x3C', '\x53', '\x6B', '\x21', '\xC1', '\xE0', '\xB3', '\x3D', '\x82',
        '\xF6', '\xFD', '\x8E', '\x61', '\x31', '\x39', '\x0C', '\x98', '\xF1', '\xA1', '\x83', '\x60', '\x2D', '\x09'
      };
      thread_local EncryptedName<sizeof(source), 0xF161095D0DE18F95ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->scrollBar->SetThumbImages(
    std::string([] {
      const char source[] = {
        '\x2F', '\xDE', '\x1F', '\xE9', '\xAD', '\x81', '\x02', '\x20', '\x29', '\x95', '\x1F', '\xDF', '\xD0', '\xF5', '\x9B', '\x9E',
        '\x8C', '\x03', '\xB0', '\x7F', '\x01', '\x7F', '\xA5', '\x8C', '\x9D', '\x2E', '\xAA', '\x63', '\x43', '\x2D', '\xAA', '\x9D',
        '\xEF'
      };
      thread_local EncryptedName<sizeof(source), 0xEDC74F6D13DF71EFULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x4F', '\x12', '\x2F', '\x7D', '\xBD', '\x0B', '\x68', '\xB8', '\x49', '\x59', '\x2F', '\x4B', '\xC0', '\x7F', '\xF1', '\x06',
        '\xEC', '\xCF', '\x80', '\xEB', '\x11', '\xF5', '\xCF', '\x14', '\xFD', '\xE2', '\x82', '\xEE', '\x19', '\xEB', '\xCF', '\x18',
        '\xFF', '\xBD'
      };
      thread_local EncryptedName<sizeof(source), 0x75ADC57D87EFBD8FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x19', '\x56', '\x21', '\x19', '\xAD', '\xDB', '\x68', '\xE6', '\x1F', '\x1D', '\x21', '\x2F', '\xD0', '\xAF', '\xF1', '\x58',
        '\xBA', '\x8B', '\x8E', '\x8F', '\x01', '\x25', '\xCF', '\x4A', '\xAB', '\xA6', '\x85', '\x8C', '\x1A', '\x7B', '\x83', '\x49',
        '\xB4', '\x89', '\xE1'
      };
      thread_local EncryptedName<sizeof(source), 0x2BAD156DE3E1F9D9ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->AddNewItem();
  this->AddChild(this->tabList);

  this->itemList = new CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>();
  this->itemList->itemWidth = 200;
  this->itemList->rowHeight = 34;
  this->itemList->SetSize(213, 238);
  this->itemList->Move(173, 17);
  this->itemList->SetOnSelect([this](SelectPackageItemBox::CUIItemInfo* item, const ITEM_INFO& data) { this->OnSelectItem(item, data); });
  this->itemList->SetBinder([this](SelectPackageItemBox::CUIItemInfo* item, const ITEM_INFO& data) { this->BindItem(item, data); });
  this->itemList->SetImages(
    std::string([] {
      const char source[] = {
        '\x11', '\xF4', '\x57', '\x1F', '\xF9', '\x49', '\x2E', '\xE2', '\x17', '\xBF', '\x57', '\x29', '\x84', '\x3D', '\xB7', '\x5C',
        '\xB4', '\x37', '\xF2', '\x86', '\x4D', '\xF7', '\x8A', '\x4C', '\xBA', '\x3A', '\xF0', '\x80', '\x65', '\xEE', '\x9F', '\x4A',
        '\xBC', '\x04', '\xFB', '\x8C', '\x4A', '\xF3', '\xC5', '\x4D', '\xBC', '\x2B', '\x97'
      };
      thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x0D', '\xAC', '\x0D', '\x81', '\x57', '\xF5', '\xD6', '\xFE', '\x0B', '\xE7', '\x0D', '\xB7', '\x2A', '\x81', '\x4F', '\x40',
        '\xA8', '\x6F', '\xA8', '\x18', '\xE3', '\x4B', '\x72', '\x50', '\xA6', '\x62', '\xAA', '\x1E', '\xCB', '\x52', '\x67', '\x56',
        '\xA0', '\x5C', '\xA1', '\x12', '\xE4', '\x4F', '\x3D', '\x51', '\xA0', '\x73', '\xCD'
      };
      thread_local EncryptedName<sizeof(source), 0x33133B977BCD03CDULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xEF', '\x78', '\x85', '\xDF', '\xF9', '\x65', '\xE8', '\x6C', '\xE9', '\x33', '\x85', '\xE9', '\x84', '\x11', '\x71', '\xD2',
        '\x4A', '\xBB', '\x20', '\x46', '\x4D', '\xDB', '\x4C', '\xC2', '\x44', '\xB6', '\x22', '\x40', '\x65', '\xC2', '\x59', '\xC4',
        '\x42', '\x88', '\x29', '\x4C', '\x4A', '\xDF', '\x03', '\xC3', '\x42', '\xA7', '\x45'
      };
      thread_local EncryptedName<sizeof(source), 0xA12DAB392545D72FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xB5', '\x94', '\xC3', '\xD5', '\xD3', '\x0F', '\x64', '\xA6', '\xB3', '\xDF', '\xC3', '\xE3', '\xAE', '\x7B', '\xFD', '\x18',
        '\x10', '\x57', '\x66', '\x4C', '\x67', '\xB1', '\xC0', '\x08', '\x1E', '\x5A', '\x64', '\x4A', '\x4F', '\xA8', '\xD5', '\x0E',
        '\x18', '\x64', '\x6F', '\x46', '\x60', '\xB5', '\x8F', '\x09', '\x18', '\x4B', '\x03'
      };
      thread_local EncryptedName<sizeof(source), 0x6BA1C1132F033B75ULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->itemX = 0;
  this->itemList->itemY = 0;
  this->itemList->scrollBar->SetSize(13, 238);
  this->itemList->scrollBar->Move(200, 0);
  this->itemList->scrollBar->step = 34;
  this->itemList->scrollBar->SetButtonImages(
    std::string([] {
      const char source[] = {
        '\x05', '\x82', '\xEB', '\xC5', '\x49', '\xD3', '\xA8', '\x22', '\x03', '\xC9', '\xEB', '\xF3', '\x34', '\xA7', '\x31', '\x9C',
        '\xA6', '\x5F', '\x44', '\x53', '\xE5', '\x2D', '\x18', '\x9F', '\xEB', '\x4F', '\x46', '\x4F', '\x89'
      };
      thread_local EncryptedName<sizeof(source), 0xEF6D1D893F2B2DC5ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\xDD', '\x68', '\x8B', '\x4F', '\x1D', '\xB9', '\x8C', '\xEE', '\xDB', '\x23', '\x8B', '\x79', '\x60', '\xCD', '\x15', '\x50',
        '\x7E', '\xB5', '\x24', '\xD9', '\xB1', '\x47', '\x2D', '\x4C', '\x6A', '\xA9', '\x65', '\xD7', '\xB0', '\x07', '\x49'
      };
      thread_local EncryptedName<sizeof(source), 0x234977DDB54BC71DULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->scrollBar->SetBackground(
    std::string([] {
      const char source[] = {
        '\xB7', '\x38', '\x47', '\xE7', '\xE3', '\x9D', '\x24', '\xA2', '\xB1', '\x73', '\x47', '\xD1', '\x9E', '\xE9', '\xBD', '\x1C',
        '\x14', '\xE5', '\xE8', '\x71', '\x4F', '\x63', '\x8C', '\x06', '\x13', '\xB9', '\xE5', '\x70', '\x53', '\x53'
      };
      thread_local EncryptedName<sizeof(source), 0x6FE153231D879777ULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->scrollBar->SetThumbImages(
    std::string([] {
      const char source[] = {
        '\x9B', '\xFC', '\x9F', '\x27', '\x47', '\x51', '\x66', '\xF4', '\x9D', '\xB7', '\x9F', '\x11', '\x3A', '\x25', '\xFF', '\x4A',
        '\x38', '\x21', '\x30', '\xB1', '\xEB', '\xAF', '\xC1', '\x58', '\x29', '\x0C', '\x2A', '\xAD', '\xA9', '\xFD', '\xCE', '\x49',
        '\x5B'
      };
      thread_local EncryptedName<sizeof(source), 0x39A39F87DD5F535BULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x37', '\x66', '\xA3', '\x93', '\xBD', '\x57', '\xB4', '\x5C', '\x31', '\x2D', '\xA3', '\xA5', '\xC0', '\x23', '\x2D', '\xE2',
        '\x94', '\xBB', '\x0C', '\x05', '\x11', '\xA9', '\x13', '\xF0', '\x85', '\x96', '\x0E', '\x00', '\x19', '\xB7', '\x13', '\xFC',
        '\x87', '\xC9'
      };
      thread_local EncryptedName<sizeof(source), 0x9171997D6963C9F7ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const char source[] = {
        '\x77', '\x9E', '\x2F', '\x39', '\xC3', '\x11', '\x5C', '\x82', '\x71', '\xD5', '\x2F', '\x0F', '\xBE', '\x65', '\xC5', '\x3C',
        '\xD4', '\x43', '\x80', '\xAF', '\x6F', '\xEF', '\xFB', '\x2E', '\xC5', '\x6E', '\x8B', '\xAC', '\x74', '\xB1', '\xB7', '\x2D',
        '\xDA', '\x41', '\xEF'
      };
      thread_local EncryptedName<sizeof(source), 0x4F99DF03C3EF31B7ULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->AddNewItem();
  this->AddChild(this->itemList);

  this->okButton = new CUIBasicButton();
  this->okButton->Create(42, 20);
  this->okButton->Move(279, 259);
  this->okButton->SetOnClick([this]() { this->OnOk(); });
  this->okButton->disabled = 0;
  this->okButton->Invalidate();
  this->okButton->SetText(
    std::string([] {
      const std::uint8_t source[] = {0xED, 0xDF, 0x6F, 0x05, 0x0D};
      thread_local EncryptedName<sizeof(source), 0xE52DF30DCBAF7125ULL> name(source);
      return name.Text();
    }())
  );
  this->okButton->SetAutoWidth(1);
  this->AddChild(this->okButton);

  this->cancelButton = new CUIBasicButton();
  this->cancelButton->Create(42, 20);
  this->cancelButton->Move(331, 259);
  this->cancelButton->SetOnClick([this]() { this->OnClickClose(); });
  this->cancelButton->disabled = 0;
  this->cancelButton->Invalidate();
  this->cancelButton->SetText(
    std::string([] {
      const std::uint8_t source[] = {0xEB, 0xE8, 0x1E, 0x77, 0x9D};
      thread_local EncryptedName<sizeof(source), 0xCFF3899D95AF355FULL> name(source);
      return name.Text();
    }())
  );
  this->cancelButton->SetAutoWidth(1);
  this->AddChild(this->cancelButton);
}

// OFFSET: 0x00A6CDE0
// FUNCTION: fn_16665
void SelectPackageItemBox::CUITabInfo::Slot15(int width, int height) {
  this->nameText = new CUIStaticText();
  this->nameText->SetSize(148, 34);
  this->nameText->Move(11, 0);
  this->nameText->SetText("", 0);
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 6);
  const std::uint8_t nameFontSource[] = {0xBA, 0xA2, 0x21, 0xC0, 0xD8, 0x8F};
  // GLOBAL: gl_25123
  thread_local EncryptedName<sizeof(nameFontSource), 0xE7978FB5A94DD7FDULL> nameFont(nameFontSource);
  this->nameText->SetFont(std::string(nameFont.Text()));
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetColor(0);
  this->nameText->SetShadow(0, 0xFFFFFF);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);
}

// OFFSET: 0x00A6FE20
// FUNCTION: fn_16783
void CUISelectPackageItemBox::BindItem(SelectPackageItemBox::CUIItemInfo* item, const ITEM_INFO& data) {
  const std::uint8_t listSource[] = {0x6F, 0xE0, 0xEB, 0x19, 0x1F, 0xDB, 0x96, 0xF2, 0x69, 0xAB, 0xEB, 0x2F, 0x62, 0xAF, 0x0F,
                                     0x4C, 0xCA, 0x23, 0x4E, 0x80, 0xAB, 0x65, 0x32, 0x5C, 0xC4, 0x2E, 0x4C, 0x86, 0x83, 0x7C,
                                     0x27, 0x5A, 0xC2, 0x10, 0x47, 0x8A, 0xAC, 0x61, 0x7D, 0x5D, 0xC2, 0x3F, 0x2B};
  thread_local EncryptedName<sizeof(listSource), 0x3F5315DFE32B4FAFULL> defaultImage(listSource);
  std::string image(defaultImage.Text());
  if (data.unknown_0068 > 0) {
    if (data.unknown_0098 > 0) {
      const std::uint8_t timeRandomSource[] = {0x55, 0x52, 0xD1, 0x49, 0x91, 0xAB, 0x50, 0xFC, 0x53, 0x19, 0xD1, 0x7F, 0xEC,
                                               0xDF, 0xC9, 0x42, 0xF0, 0x91, 0x74, 0xD0, 0x25, 0x15, 0xF4, 0x52, 0xFE, 0x9C,
                                               0x76, 0xD6, 0x0D, 0x0C, 0xE1, 0x54, 0xF8, 0xA2, 0x7D, 0xDA, 0x22, 0x11, 0xCA,
                                               0x45, 0xFC, 0x90, 0x74, 0xC1, 0x30, 0x0B, 0xBB, 0x53, 0xF8, 0x8D, 0x11};
      thread_local EncryptedName<sizeof(timeRandomSource), 0x31956551B311FD95ULL> timeRandom(timeRandomSource);
      image = std::string(timeRandom.Text());
    } else {
      const std::uint8_t timeSource[] = {0x51, 0x6C, 0x7D, 0x9B, 0xA5, 0x5F, 0xF8, 0xF4, 0x57, 0x27, 0x7D, 0xAD, 0xD8, 0x2B, 0x61, 0x4A,
                                         0xF4, 0xAF, 0xD8, 0x02, 0x11, 0xE1, 0x5C, 0x5A, 0xFA, 0xA2, 0xDA, 0x04, 0x39, 0xF8, 0x49, 0x5C,
                                         0xFC, 0x9C, 0xD1, 0x08, 0x16, 0xE5, 0x62, 0x4D, 0xF8, 0xAE, 0xD8, 0x4F, 0x07, 0xFC, 0x4D, 0x39};
      thread_local EncryptedName<sizeof(timeSource), 0x393D916561BDC391ULL> timeLimited(timeSource);
      image = std::string(timeLimited.Text());
    }
  } else if (data.unknown_0098 > 0) {
    const std::uint8_t randomSource[] = {0xD5, 0x3C, 0x73, 0x3B, 0x27, 0x8D, 0x94, 0x20, 0xD3, 0x77, 0x73, 0x0D, 0x5A, 0xF9, 0x0D, 0x9E, 0x70,
                                         0xFF, 0xD6, 0xA2, 0x93, 0x33, 0x30, 0x8E, 0x7E, 0xF2, 0xD4, 0xA4, 0xBB, 0x2A, 0x25, 0x88, 0x78, 0xCC,
                                         0xDF, 0xA8, 0x94, 0x37, 0x0E, 0x9F, 0x74, 0xFD, 0xDC, 0xB1, 0xC9, 0x21, 0x3C, 0x9D, 0x15};
    thread_local EncryptedName<sizeof(randomSource), 0xED5143E7C1B39315ULL> randomOption(randomSource);
    image = std::string(randomOption.Text());
  }

  item->image->SetImage(image);
  item->gameItem->SetItem(data);
  const std::string name = data.DisplayName(0);
  item->nameText->SetText(name.c_str(), name.size());
}

// OFFSET: 0x00A703D0
// FUNCTION: fn_16786
void CUISelectPackageItemBox::Slot16() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  this->itemList->unknown_00C4 = 1;
  this->itemList->Invalidate();
  this->okButton->disabled = 1;
  this->okButton->Invalidate();
  this->okButton->SetText(LookupMessage(3384));
  this->cancelButton->SetText(LookupMessage(3045));
}

// OFFSET: 0x00A705F0
// FUNCTION: fn_16788
void CUISelectPackageItemBox::Open(int sourceIndex) {
  this->tabList->ClearData();
  this->itemList->ClearData();
  this->title->SetTitle("", 0);
  this->sourceIndex = sourceIndex;
  const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, sourceIndex);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  this->packageId = std::atoi(item.itemId.c_str());
  const char* name = FindItemDescription(this->packageId).identifiedName;
  this->title->SetTitle(name, std::strlen(name));
  if (!Singleton<CSelectPackageItemMgr>::s_instance) {
    Singleton<CSelectPackageItemMgr>::s_instance = new (std::nothrow) CSelectPackageItemMgr;
  }

  auto package = Singleton<CSelectPackageItemMgr>::s_instance->m_packages.find(this->packageId);
  if (package != Singleton<CSelectPackageItemMgr>::s_instance->m_packages.end()) {
    for (const auto& tab : package->second) {
      this->tabList->data.push_back(SelectPackageItemBox::CTabInfoData{this->packageId, tab.first, tab.second.name});
    }
    this->tabList->UpdateScrollBar();
  }

  this->okButton->disabled = 0;
  this->okButton->Invalidate();
}

// OFFSET: 0x00A70B90
// FUNCTION: fn_16790
void CUISelectPackageItemBox::BindTab(SelectPackageItemBox::CUITabInfo* tab, const SelectPackageItemBox::CTabInfoData& data) {
  tab->nameText->SetText(data.name.c_str(), data.name.size());
}

// OFFSET: 0x00A69350
// FUNCTION: fn_16658
CUIFrameWnd* CreateSelectPackageItemBox() {
  return new CUISelectPackageItemBox();
}

// OFFSET: 0x00A6F470
// FUNCTION: fn_16772
CUISelectPackageItemBox::CUISelectPackageItemBox()
    : title(nullptr), tabList(nullptr), itemList(nullptr), okButton(nullptr), cancelButton(nullptr), sourceIndex(-1), packageId(0) {
}
