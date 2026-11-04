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

  list->selected = selectedItem->index;
  if (list->onSelect && selectedItem->index >= 0 && static_cast<unsigned int>(selectedItem->index) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = selectedItem->index; index > 0; --index) {
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

  list->selected = selectedTab->index;
  if (list->onSelect && selectedTab->index >= 0 && static_cast<unsigned int>(selectedTab->index) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = selectedTab->index; index > 0; --index) {
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
  std::advance(it, index);
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
      const std::uint8_t source[] = {
        0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x93,
        0x90, 0xB4, 0x50, 0x6A, 0x48, 0x10, 0x21, 0x85, 0x94, 0xB5, 0x5F, 0x68, 0x74, 0x1C, 0x13, 0x85,
        0x98, 0xB3, 0x55, 0x6C, 0x75, 0x18, 0x3D, 0xAE, 0x9D, 0xA2, 0x5F, 0x7D, 0x39, 0x1B, 0x22, 0x81,
        0xF1
      };
      thread_local EncryptedName<sizeof(source), 0xF14F79170939C7F1ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xC7,
        0xFE, 0xB6, 0x04, 0xC8, 0x88, 0x40, 0x93, 0xD1, 0xFA, 0xB7, 0x0B, 0xCA, 0xB4, 0x4C, 0xA1, 0xD1,
        0xF6, 0xB1, 0x01, 0xCE, 0xB5, 0x48, 0x8F, 0xFA, 0xF2, 0xAC, 0x09, 0x85, 0xB5, 0x44, 0x8D, 0xA5
      };
      thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x21, 0xD4, 0x39, 0x2D, 0x15, 0xDD, 0xFC, 0x10, 0x27, 0x9F, 0x39, 0x1B, 0x68, 0xA9, 0x65, 0xBF,
        0x80, 0x08, 0x90, 0xB4, 0x8A, 0x7A, 0x57, 0xA9, 0x84, 0x09, 0x9F, 0xB6, 0xB6, 0x76, 0x65, 0xA9,
        0x88, 0x0F, 0x95, 0xB2, 0xB7, 0x72, 0x4B, 0x82, 0x93, 0x12, 0x9E, 0xBF, 0xA1, 0x3D, 0x5B, 0xB0,
        0x91, 0x7B
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
      const std::uint8_t source[] = {
        0x7D, 0x12, 0x2D, 0xCD, 0x7B, 0x71, 0x2A, 0x4E, 0x7B, 0x59, 0x2D, 0xFB, 0x06, 0x05, 0xB3, 0xE1,
        0xDC, 0xCE, 0x84, 0x54, 0xE4, 0xD6, 0x81, 0xF7, 0xD8, 0xCF, 0x8B, 0x56, 0xD8, 0xDA, 0xB3, 0xF0,
        0xC4, 0xCE, 0xB2, 0x55, 0xDA, 0xCC, 0x8A, 0xDC, 0xD2, 0xDB, 0x8B, 0x19, 0xD9, 0xD2, 0x9F, 0x83
      };
      thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x67, 0x5A, 0xBF, 0xC3, 0xF1, 0x85, 0xEE, 0x72, 0x61, 0x11, 0xBF, 0xF5, 0x8C, 0xF1, 0x77, 0xDD,
        0xC6, 0x86, 0x16, 0x5A, 0x6E, 0x22, 0x45, 0xCB, 0xC2, 0x87, 0x19, 0x58, 0x52, 0x2E, 0x77, 0xCC,
        0xDE, 0x86, 0x20, 0x5B, 0x50, 0x38, 0x4E, 0xE0, 0xC8, 0x9B, 0x51, 0x5B, 0x5C, 0x3B, 0x2B
      };
      thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x41,
        0xA8, 0x86, 0x36, 0xE8, 0xD0, 0xBC, 0xD5, 0x57, 0xAC, 0x87, 0x39, 0xEA, 0xEC, 0xB0, 0xE7, 0x50,
        0xB0, 0x86, 0x00, 0xE9, 0xEE, 0xA6, 0xDE, 0x7C, 0xA6, 0x9B, 0x71, 0xE9, 0xE2, 0xA5, 0xBB
      };
      thread_local EncryptedName<sizeof(source), 0x23BBD58F8B5FF5C9ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0xA7,
        0x0C, 0xC8, 0xFC, 0x4E, 0xCC, 0x92, 0x79, 0xB1, 0x08, 0xC9, 0xF3, 0x4C, 0xF0, 0x9E, 0x4B, 0xB6,
        0x14, 0xC8, 0xCA, 0x4F, 0xF2, 0x88, 0x72, 0x9A, 0x02, 0xDD, 0xF3, 0x03, 0xF1, 0x96, 0x67, 0xC5
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
      const std::uint8_t source[] = {
        0x65, 0x32, 0xAD, 0x03, 0xA3, 0xE9, 0xB2, 0xCE, 0x63, 0x79, 0xAD, 0x35, 0xDE, 0x9D, 0x2B, 0x61,
        0xC4, 0xEE, 0x04, 0x9A, 0x3C, 0x4E, 0x19, 0x77, 0xC0, 0xEF, 0x0B, 0x98, 0x00, 0x42, 0x2B, 0x70,
        0xDC, 0xEE, 0x32, 0x9A, 0x0F, 0x48, 0x04, 0x66, 0xFA, 0xF2, 0x0B, 0x9F, 0x4D, 0x45, 0x1A, 0x73,
        0xA5
      };
      thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x47, 0xBC, 0xAB, 0x85, 0xF9, 0xC7, 0xB4, 0xC6, 0x41, 0xF7, 0xAB, 0xB3, 0x84, 0xB3, 0x2D, 0x69,
        0xE6, 0x60, 0x02, 0x1C, 0x66, 0x60, 0x1F, 0x7F, 0xE2, 0x61, 0x0D, 0x1E, 0x5A, 0x6C, 0x2D, 0x78,
        0xFE, 0x60, 0x34, 0x1C, 0x55, 0x66, 0x02, 0x6E, 0xD8, 0x7C, 0x05, 0x51, 0x5B, 0x64, 0x01, 0x0B
      };
      thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xA7, 0x04, 0x51, 0x5F, 0x61, 0xF1, 0x26, 0xDE, 0xA1, 0x4F, 0x51, 0x69, 0x1C, 0x85, 0xBF, 0x71,
        0x06, 0xD8, 0xF8, 0xC6, 0xFE, 0x56, 0x8D, 0x67, 0x02, 0xD9, 0xF7, 0xC4, 0xC2, 0x5A, 0xBF, 0x60,
        0x1E, 0xD8, 0xCE, 0xC6, 0xCD, 0x50, 0x90, 0x76, 0x38, 0xC4, 0xFF, 0x8B, 0xC3, 0x52, 0x93, 0x13
      };
      thread_local EncryptedName<sizeof(source), 0x13E33FA1A591AB67ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x75,
        0x56, 0x34, 0xC4, 0x5A, 0xE0, 0x8C, 0xC7, 0x63, 0x52, 0x35, 0xCB, 0x58, 0xDC, 0x80, 0xF5, 0x64,
        0x4E, 0x34, 0xF2, 0x5A, 0xD3, 0x8A, 0xDA, 0x72, 0x68, 0x28, 0xCB, 0x5F, 0x91, 0x87, 0xC4, 0x67,
        0x37
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
      const std::uint8_t source[] = {
        0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xEA,
        0x26, 0x45, 0x36, 0x44, 0xA1, 0xCF, 0x28, 0xFA, 0x28, 0x48, 0x34, 0x42, 0x89, 0xDD, 0x3D, 0xF7,
        0x1C, 0x5A, 0x36, 0x53, 0x8A, 0xD3, 0x20, 0xEA, 0x37, 0x07, 0x31, 0x4A, 0xA5, 0xBF
      };
      thread_local EncryptedName<sizeof(source), 0x9949BFD527532943ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0xBE,
        0x0A, 0x11, 0x9E, 0x40, 0x05, 0x77, 0xEE, 0xAE, 0x04, 0x1C, 0x9C, 0x46, 0x2D, 0x65, 0xFB, 0xA3,
        0x30, 0x0E, 0x9E, 0x57, 0x2E, 0x77, 0xFD, 0xA8, 0x1C, 0x0E, 0xD5, 0x41, 0x1C, 0x77, 0x8F
      };
      thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xA9, 0xB0, 0xB7, 0xAF, 0x5D, 0x5B, 0xBE, 0xAE, 0xAF, 0xFB, 0xB7, 0x99, 0x20, 0x2F, 0x27, 0x10,
        0x0C, 0x73, 0x12, 0x36, 0xE9, 0xE5, 0x1A, 0x00, 0x02, 0x7E, 0x10, 0x30, 0xC1, 0xF7, 0x0F, 0x0D,
        0x36, 0x6C, 0x12, 0x21, 0xC2, 0xF9, 0x12, 0x10, 0x1D, 0x31, 0x15, 0x38, 0xED, 0x95
      };
      thread_local EncryptedName<sizeof(source), 0x637B959D55771F69ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x91, 0x24, 0x0D, 0xFB, 0x7B, 0xF3, 0x84, 0x1C, 0x97, 0x6F, 0x0D, 0xCD, 0x06, 0x87, 0x1D, 0xA2,
        0x34, 0xE7, 0xA8, 0x62, 0xCF, 0x4D, 0x20, 0xB2, 0x3A, 0xEA, 0xAA, 0x64, 0xE7, 0x5F, 0x35, 0xBF,
        0x0E, 0xF8, 0xA8, 0x75, 0xE4, 0x51, 0x28, 0xA2, 0x25, 0xA5, 0xAF, 0x6C, 0xCB, 0x3D
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
      const std::uint8_t source[] = {
        0x79, 0xAA, 0xB7, 0xB5, 0xCB, 0xD3, 0x4A, 0xB8, 0x7F, 0xE1, 0xB7, 0x83, 0xB6, 0xA7, 0xD3, 0x06,
        0xDA, 0x77, 0x18, 0x23, 0x67, 0x2D, 0xFA, 0x05, 0x97, 0x67, 0x1A, 0x3F, 0x0B
      };
      thread_local EncryptedName<sizeof(source), 0x758F1D0B4F7705B9ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xA5, 0xB6, 0x85, 0xF3, 0xED, 0xED, 0xDE, 0x44, 0xA3, 0xFD, 0x85, 0xC5, 0x90, 0x99, 0x47, 0xFA,
        0x06, 0x6B, 0x2A, 0x65, 0x41, 0x13, 0x7F, 0xE6, 0x12, 0x77, 0x6B, 0x6B, 0x40, 0x53, 0x1B
      };
      thread_local EncryptedName<sizeof(source), 0x891B232D09451965ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->scrollBar->SetBackground(
    std::string([] {
      const std::uint8_t source[] = {
        0x55, 0x20, 0x21, 0xF7, 0x9D, 0xC7, 0xA4, 0x3C, 0x53, 0x6B, 0x21, 0xC1, 0xE0, 0xB3, 0x3D, 0x82,
        0xF6, 0xFD, 0x8E, 0x61, 0x31, 0x39, 0x0C, 0x98, 0xF1, 0xA1, 0x83, 0x60, 0x2D, 0x09
      };
      thread_local EncryptedName<sizeof(source), 0xF161095D0DE18F95ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->scrollBar->SetThumbImages(
    std::string([] {
      const std::uint8_t source[] = {
        0x2F, 0xDE, 0x1F, 0xE9, 0xAD, 0x81, 0x02, 0x20, 0x29, 0x95, 0x1F, 0xDF, 0xD0, 0xF5, 0x9B, 0x9E,
        0x8C, 0x03, 0xB0, 0x7F, 0x01, 0x7F, 0xA5, 0x8C, 0x9D, 0x2E, 0xAA, 0x63, 0x43, 0x2D, 0xAA, 0x9D,
        0xEF
      };
      thread_local EncryptedName<sizeof(source), 0xEDC74F6D13DF71EFULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x4F, 0x12, 0x2F, 0x7D, 0xBD, 0x0B, 0x68, 0xB8, 0x49, 0x59, 0x2F, 0x4B, 0xC0, 0x7F, 0xF1, 0x06,
        0xEC, 0xCF, 0x80, 0xEB, 0x11, 0xF5, 0xCF, 0x14, 0xFD, 0xE2, 0x82, 0xEE, 0x19, 0xEB, 0xCF, 0x18,
        0xFF, 0xBD
      };
      thread_local EncryptedName<sizeof(source), 0x75ADC57D87EFBD8FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x19, 0x56, 0x21, 0x19, 0xAD, 0xDB, 0x68, 0xE6, 0x1F, 0x1D, 0x21, 0x2F, 0xD0, 0xAF, 0xF1, 0x58,
        0xBA, 0x8B, 0x8E, 0x8F, 0x01, 0x25, 0xCF, 0x4A, 0xAB, 0xA6, 0x85, 0x8C, 0x1A, 0x7B, 0x83, 0x49,
        0xB4, 0x89, 0xE1
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
      const std::uint8_t source[] = {
        0x11, 0xF4, 0x57, 0x1F, 0xF9, 0x49, 0x2E, 0xE2, 0x17, 0xBF, 0x57, 0x29, 0x84, 0x3D, 0xB7, 0x5C,
        0xB4, 0x37, 0xF2, 0x86, 0x4D, 0xF7, 0x8A, 0x4C, 0xBA, 0x3A, 0xF0, 0x80, 0x65, 0xEE, 0x9F, 0x4A,
        0xBC, 0x04, 0xFB, 0x8C, 0x4A, 0xF3, 0xC5, 0x4D, 0xBC, 0x2B, 0x97
      };
      thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x0D, 0xAC, 0x0D, 0x81, 0x57, 0xF5, 0xD6, 0xFE, 0x0B, 0xE7, 0x0D, 0xB7, 0x2A, 0x81, 0x4F, 0x40,
        0xA8, 0x6F, 0xA8, 0x18, 0xE3, 0x4B, 0x72, 0x50, 0xA6, 0x62, 0xAA, 0x1E, 0xCB, 0x52, 0x67, 0x56,
        0xA0, 0x5C, 0xA1, 0x12, 0xE4, 0x4F, 0x3D, 0x51, 0xA0, 0x73, 0xCD
      };
      thread_local EncryptedName<sizeof(source), 0x33133B977BCD03CDULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xEF, 0x78, 0x85, 0xDF, 0xF9, 0x65, 0xE8, 0x6C, 0xE9, 0x33, 0x85, 0xE9, 0x84, 0x11, 0x71, 0xD2,
        0x4A, 0xBB, 0x20, 0x46, 0x4D, 0xDB, 0x4C, 0xC2, 0x44, 0xB6, 0x22, 0x40, 0x65, 0xC2, 0x59, 0xC4,
        0x42, 0x88, 0x29, 0x4C, 0x4A, 0xDF, 0x03, 0xC3, 0x42, 0xA7, 0x45
      };
      thread_local EncryptedName<sizeof(source), 0xA12DAB392545D72FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xB5, 0x94, 0xC3, 0xD5, 0xD3, 0x0F, 0x64, 0xA6, 0xB3, 0xDF, 0xC3, 0xE3, 0xAE, 0x7B, 0xFD, 0x18,
        0x10, 0x57, 0x66, 0x4C, 0x67, 0xB1, 0xC0, 0x08, 0x1E, 0x5A, 0x64, 0x4A, 0x4F, 0xA8, 0xD5, 0x0E,
        0x18, 0x64, 0x6F, 0x46, 0x60, 0xB5, 0x8F, 0x09, 0x18, 0x4B, 0x03
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
      const std::uint8_t source[] = {
        0x05, 0x82, 0xEB, 0xC5, 0x49, 0xD3, 0xA8, 0x22, 0x03, 0xC9, 0xEB, 0xF3, 0x34, 0xA7, 0x31, 0x9C,
        0xA6, 0x5F, 0x44, 0x53, 0xE5, 0x2D, 0x18, 0x9F, 0xEB, 0x4F, 0x46, 0x4F, 0x89
      };
      thread_local EncryptedName<sizeof(source), 0xEF6D1D893F2B2DC5ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0xDD, 0x68, 0x8B, 0x4F, 0x1D, 0xB9, 0x8C, 0xEE, 0xDB, 0x23, 0x8B, 0x79, 0x60, 0xCD, 0x15, 0x50,
        0x7E, 0xB5, 0x24, 0xD9, 0xB1, 0x47, 0x2D, 0x4C, 0x6A, 0xA9, 0x65, 0xD7, 0xB0, 0x07, 0x49
      };
      thread_local EncryptedName<sizeof(source), 0x234977DDB54BC71DULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->scrollBar->SetBackground(
    std::string([] {
      const std::uint8_t source[] = {
        0xB7, 0x38, 0x47, 0xE7, 0xE3, 0x9D, 0x24, 0xA2, 0xB1, 0x73, 0x47, 0xD1, 0x9E, 0xE9, 0xBD, 0x1C,
        0x14, 0xE5, 0xE8, 0x71, 0x4F, 0x63, 0x8C, 0x06, 0x13, 0xB9, 0xE5, 0x70, 0x53, 0x53
      };
      thread_local EncryptedName<sizeof(source), 0x6FE153231D879777ULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->scrollBar->SetThumbImages(
    std::string([] {
      const std::uint8_t source[] = {
        0x9B, 0xFC, 0x9F, 0x27, 0x47, 0x51, 0x66, 0xF4, 0x9D, 0xB7, 0x9F, 0x11, 0x3A, 0x25, 0xFF, 0x4A,
        0x38, 0x21, 0x30, 0xB1, 0xEB, 0xAF, 0xC1, 0x58, 0x29, 0x0C, 0x2A, 0xAD, 0xA9, 0xFD, 0xCE, 0x49,
        0x5B
      };
      thread_local EncryptedName<sizeof(source), 0x39A39F87DD5F535BULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x37, 0x66, 0xA3, 0x93, 0xBD, 0x57, 0xB4, 0x5C, 0x31, 0x2D, 0xA3, 0xA5, 0xC0, 0x23, 0x2D, 0xE2,
        0x94, 0xBB, 0x0C, 0x05, 0x11, 0xA9, 0x13, 0xF0, 0x85, 0x96, 0x0E, 0x00, 0x19, 0xB7, 0x13, 0xFC,
        0x87, 0xC9
      };
      thread_local EncryptedName<sizeof(source), 0x9171997D6963C9F7ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      const std::uint8_t source[] = {
        0x77, 0x9E, 0x2F, 0x39, 0xC3, 0x11, 0x5C, 0x82, 0x71, 0xD5, 0x2F, 0x0F, 0xBE, 0x65, 0xC5, 0x3C,
        0xD4, 0x43, 0x80, 0xAF, 0x6F, 0xEF, 0xFB, 0x2E, 0xC5, 0x6E, 0x8B, 0xAC, 0x74, 0xB1, 0xB7, 0x2D,
        0xDA, 0x41, 0xEF
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
