#include "UI/SelectPackageItemBox/SelectPackageItemBox.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Render/Renderer.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <new>

template __declspec(noinline) std::string::basic_string(const std::string&);

template __declspec(noinline) std::list<ITEM_INFO>::~list() noexcept;
template __declspec(noinline) std::list<SelectPackageItemBox::CTabInfoData>::~list() noexcept;

// OFFSET: 0x00A684E0
// FUNCTION: fn_16646
template <>
CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A686A0
// FUNCTION: fn_16647
template <>
CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
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
void CUISelectPackageItemBox::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->Resize(387, 283);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
  this->SetSkinColors(0, 0, 0, 0);
  this->m_buttonBarStyle = 3;
}

// OFFSET: 0x00A695F0
// FUNCTION: fn_16662
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A696C0
// FUNCTION: fn_16663
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::OnCreate(int width, int height) {
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
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A6D0C0
// FUNCTION: fn_16669
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// INLINED
template <>
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (SelectPackageItemBox::CUIItemInfo* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
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
  if (this->layoutFlag) {
    return;
  }
  for (SelectPackageItemBox::CUITabInfo* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
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

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (SelectPackageItemBox::CUIItemInfo*& slot : this->items) {
      SelectPackageItemBox::CUIItemInfo* item = new SelectPackageItemBox::CUIItemInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
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

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (SelectPackageItemBox::CUITabInfo*& slot : this->items) {
      SelectPackageItemBox::CUITabInfo* item = new SelectPackageItemBox::CUITabInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
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
void CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::LoadSkin(bool resolveSkin) {
  this->normalImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SelectPackageItemBox::CUIItemInfo* item : this->items) {
    item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x00A6D870
// FUNCTION: fn_16673
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::LoadSkin(bool resolveSkin) {
  this->normalImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SelectPackageItemBox::CUITabInfo* item : this->items) {
    item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A6DD80
// FUNCTION: fn_16677
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::OnScroll(int position) {
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
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
  if (list->layoutFlag) {
    return;
  }

  for (SelectPackageItemBox::CUIItemInfo* entry : list->items) {
    if (selection != entry) {
      entry->SetSelected(false);
    }
  }

  selection->SetSelected(true);
  if (selection->index == static_cast<int>(list->selected) && list->selectionFlag) {
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
  if (list->layoutFlag) {
    return;
  }

  for (SelectPackageItemBox::CUITabInfo* entry : list->items) {
    if (selection != entry) {
      entry->SetSelected(false);
    }
  }

  selection->SetSelected(true);
  if (selection->index == static_cast<int>(list->selected) && list->selectionFlag) {
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
  g_windowMgr.CloseWindow(WID_SELECTPACKAGEITEMBOX);
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
                       .Confirm(FormatOwnedString(LookupMessage(MSI_JOB_ABYSS_CHASER), it->name.c_str()).c_str(), 2, true, false, 0, "Message", 280, 120, 0);
  if (result == 187) {
    g_modeMgr.GetGameMode()->SendMsg(MM_USE_PACKAGEITEM, this->sourceIndex, this->packageId, index, 0);
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
  g_windowMgr.CloseWindow(WID_SELECTPACKAGEITEMBOX);
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
void CUIItemInfo::OnInit() {
  auto* const item = this->gameItem;
  item->showOverlay = 1;
  item->Invalidate();
}

// OFFSET: 0x00A705E0
// FUNCTION: fn_16787
void CUITabInfo::OnInit() {
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

// OFFSET: 0x00A69790
// FUNCTION: fn_16664
void SelectPackageItemBox::CUIItemInfo::OnCreate(int width, int height) {
  this->image = new CUIStaticImage();
  this->image->SetSize(200, 34);
  this->image->Move(0, 0);
  // GLOBAL: gl_25124
  this->image->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\selectpackage\\item_list.bmp", 0xD97D5DBDBB2F2709ULL)));
  this->AddChild(this->image);

  this->gameItem = new CUIGameItem();
  this->gameItem->SetMode(0);
  this->gameItem->SetScale(100);
  this->gameItem->Move(6, 5);
  this->gameItem->showNameTooltip = 1;
  this->gameItem->allowItemInfo = 1;
  this->AddChild(this->gameItem);

  this->nameText = new CUIStaticText();
  this->nameText->SetSize(148, 24);
  this->nameText->Move(45, 5);
  this->nameText->SetText("");
  this->nameText->SetFitText(1);
  this->nameText->SetAlign(0, 6);
  // GLOBAL: gl_25123
  this->nameText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xED796785D1DF137FULL)));
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetColor(0);
  this->nameText->SetShadow(1, 0xE7E7E7);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);
}

// OFFSET: 0x00A69DB0
// FUNCTION: fn_45491
void CUISelectPackageItemBox::OnCreate(int width, int height) {
  this->title = new CUITitleBar();
  this->title->SetSize(387, 17);
  this->title->Move(0, 0);
  this->title->SetImages(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(49, "유저인터페이스\\basic_interface\\titlebar_left.bmp", 0xF14F79170939C7F1ULL);
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0xF14F79170939C7F1ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0xA5FD29D7AB6DC59FULL);
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(50, "유저인터페이스\\basic_interface\\titlebar_right.bmp", 0xDD3913D5D7F97BE1ULL);
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xDD3913D5D7F97BE1ULL> name(source);
      return name.Text();
    }())
  );
  this->AddChild(this->title);

  CUIStaticText* titleText = new CUIStaticText();
  titleText->SetSize(250, 17);
  titleText->Move(18, 0);
  titleText->SetText("");
  titleText->SetFitText(0);
  titleText->SetAlign(0, 6);
  titleText->SetFont(
    std::string([] {
      constexpr std::uint8_t bytes[] = OBFUSCATE_SOURCE_BYTES(6, "Gulim", 0x050505679903579BULL);
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x050505679903579BULL> name(source);
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
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x83EFBFBB37EDBDBDULL);
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(47, "유저인터페이스\\basic_interface\\sys_base_on.bmp", 0xBF2B4B31397FF5A7ULL);
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(47, "유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x23BBD58F8B5FF5C9ULL);
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0x23BBD58F8B5FF5C9ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\sys_base_off.bmp", 0xC517FB932D95BB6DULL);
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xC517FB932D95BB6DULL> name(source);
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
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(49, "유저인터페이스\\basic_interface\\sys_close_off.bmp", 0x03772763F96D9DA5ULL);
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x03772763F96D9DA5ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x0B7109397F6B1387ULL);
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x0B7109397F6B1387ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(48, "유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x13E33FA1A591AB67ULL);
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x13E33FA1A591AB67ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(49, "유저인터페이스\\basic_interface\\sys_close_off.bmp", 0x17A9E5BF39AD4737ULL);
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x17A9E5BF39AD4737ULL> name(source);
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
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(46, "유저인터페이스\\selectpackage\\btn_set_list.bmp", 0x9949BFD527532943ULL);
      constexpr EncryptedSource<46> source(bytes);
      thread_local EncryptedName<46, 0x9949BFD527532943ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(47, "유저인터페이스\\selectpackage\\btn_set_press.bmp", 0xCD8F077123FB7D6FULL);
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0xCD8F077123FB7D6FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(46, "유저인터페이스\\selectpackage\\btn_set_list.bmp", 0x637B959D55771F69ULL);
      constexpr EncryptedSource<46> source(bytes);
      thread_local EncryptedName<46, 0x637B959D55771F69ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(46, "유저인터페이스\\selectpackage\\btn_set_list.bmp", 0xD1413DBB01CD8B51ULL);
      constexpr EncryptedSource<46> source(bytes);
      thread_local EncryptedName<46, 0xD1413DBB01CD8B51ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->itemX = 13;
  this->tabList->itemY = 0;
  this->tabList->scrollBar->SetSize(13, 238);
  this->tabList->scrollBar->Move(0, 0);
  this->tabList->scrollBar->step = 34;
  this->tabList->SetScrollBarButtonImages(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(29, "유저인터페이스\\scroll0up.bmp", 0x758F1D0B4F7705B9ULL);
      constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x758F1D0B4F7705B9ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(31, "유저인터페이스\\scroll0down.bmp", 0x891B232D09451965ULL);
      constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x891B232D09451965ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->SetScrollBarBackground(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\scroll0mid.bmp", 0xF161095D0DE18F95ULL);
      constexpr EncryptedSource<30> source(bytes);
      thread_local EncryptedName<30, 0xF161095D0DE18F95ULL> name(source);
      return name.Text();
    }())
  );
  this->tabList->SetScrollBarThumbImages(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(33, "유저인터페이스\\scroll0bar_up.bmp", 0xEDC74F6D13DF71EFULL);
      constexpr EncryptedSource<33> source(bytes);
      thread_local EncryptedName<33, 0xEDC74F6D13DF71EFULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(34, "유저인터페이스\\scroll0bar_mid.bmp", 0x75ADC57D87EFBD8FULL);
      constexpr EncryptedSource<34> source(bytes);
      thread_local EncryptedName<34, 0x75ADC57D87EFBD8FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(35, "유저인터페이스\\scroll0bar_down.bmp", 0x2BAD156DE3E1F9D9ULL);
      constexpr EncryptedSource<35> source(bytes);
      thread_local EncryptedName<35, 0x2BAD156DE3E1F9D9ULL> name(source);
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
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(43, "유저인터페이스\\selectpackage\\item_list.bmp", 0x2FEB8739E5975BD1ULL);
      constexpr EncryptedSource<43> source(bytes);
      thread_local EncryptedName<43, 0x2FEB8739E5975BD1ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(43, "유저인터페이스\\selectpackage\\item_list.bmp", 0x33133B977BCD03CDULL);
      constexpr EncryptedSource<43> source(bytes);
      thread_local EncryptedName<43, 0x33133B977BCD03CDULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(43, "유저인터페이스\\selectpackage\\item_list.bmp", 0xA12DAB392545D72FULL);
      constexpr EncryptedSource<43> source(bytes);
      thread_local EncryptedName<43, 0xA12DAB392545D72FULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(43, "유저인터페이스\\selectpackage\\item_list.bmp", 0x6BA1C1132F033B75ULL);
      constexpr EncryptedSource<43> source(bytes);
      thread_local EncryptedName<43, 0x6BA1C1132F033B75ULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->itemX = 0;
  this->itemList->itemY = 0;
  this->itemList->scrollBar->SetSize(13, 238);
  this->itemList->scrollBar->Move(200, 0);
  this->itemList->scrollBar->step = 34;
  this->itemList->SetScrollBarButtonImages(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(29, "유저인터페이스\\scroll0up.bmp", 0xEF6D1D893F2B2DC5ULL);
      constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0xEF6D1D893F2B2DC5ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(31, "유저인터페이스\\scroll0down.bmp", 0x234977DDB54BC71DULL);
      constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x234977DDB54BC71DULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->SetScrollBarBackground(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(30, "유저인터페이스\\scroll0mid.bmp", 0x6FE153231D879777ULL);
      constexpr EncryptedSource<30> source(bytes);
      thread_local EncryptedName<30, 0x6FE153231D879777ULL> name(source);
      return name.Text();
    }())
  );
  this->itemList->SetScrollBarThumbImages(
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(33, "유저인터페이스\\scroll0bar_up.bmp", 0x39A39F87DD5F535BULL);
      constexpr EncryptedSource<33> source(bytes);
      thread_local EncryptedName<33, 0x39A39F87DD5F535BULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(34, "유저인터페이스\\scroll0bar_mid.bmp", 0x9171997D6963C9F7ULL);
      constexpr EncryptedSource<34> source(bytes);
      thread_local EncryptedName<34, 0x9171997D6963C9F7ULL> name(source);
      return name.Text();
    }()),
    std::string([] {
      constexpr char bytes[] = OBFUSCATE_SOURCE_CHARS(35, "유저인터페이스\\scroll0bar_down.bmp", 0x4F99DF03C3EF31B7ULL);
      constexpr EncryptedSource<35> source(bytes);
      thread_local EncryptedName<35, 0x4F99DF03C3EF31B7ULL> name(source);
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
      constexpr std::uint8_t bytes[] = OBFUSCATE_SOURCE_BYTES(5, "확인", 0xE52DF30DCBAF7125ULL);
      constexpr EncryptedSource<5> source(bytes);
      thread_local EncryptedName<5, 0xE52DF30DCBAF7125ULL> name(source);
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
      constexpr std::uint8_t bytes[] = OBFUSCATE_SOURCE_BYTES(5, "닫기", 0xCFF3899D95AF355FULL);
      constexpr EncryptedSource<5> source(bytes);
      thread_local EncryptedName<5, 0xCFF3899D95AF355FULL> name(source);
      return name.Text();
    }())
  );
  this->cancelButton->SetAutoWidth(1);
  this->AddChild(this->cancelButton);
}

// OFFSET: 0x00A6CDE0
// FUNCTION: fn_16665
void SelectPackageItemBox::CUITabInfo::OnCreate(int width, int height) {
  this->nameText = new CUIStaticText();
  this->nameText->SetSize(148, 34);
  this->nameText->Move(11, 0);
  this->nameText->SetText("");
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 6);
  // GLOBAL: gl_25123
  this->nameText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xE7978FB5A94DD7FDULL)));
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
  std::string image(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\selectpackage\\item_list.bmp", 0x3F5315DFE32B4FAFULL));
  if (data.expireTime > 0) {
    if (data.optionCount > 0) {
      image = std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\selectpackage\\item_list_timeran.bmp", 0x31956551B311FD95ULL));
    } else {
      image = std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\selectpackage\\item_list_time.bmp", 0x393D916561BDC391ULL));
    }
  } else if (data.optionCount > 0) {
    image = std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\selectpackage\\item_list_ranop.bmp", 0xED5143E7C1B39315ULL));
  }

  item->image->SetImage(image);
  item->gameItem->SetItem(data);
  const std::string name = data.DisplayName(0);
  item->nameText->SetText(name.c_str(), name.size());
}

// OFFSET: 0x00A703D0
// FUNCTION: fn_16786
void CUISelectPackageItemBox::OnInit() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  auto* const list = this->itemList;
  list->layoutFlag = 1;
  list->Invalidate();
  auto* const ok = this->okButton;
  ok->disabled = 1;
  ok->Invalidate();
  this->okButton->SetText(LookupMessage(MSI_PRIVATE_AIRPLANE_WARNNING_SELECT_NPC));
  this->cancelButton->SetText(LookupMessage(MSI_ZENY_LOTTO_BUY_10));
}

// OFFSET: 0x00A705F0
// FUNCTION: fn_16788
void CUISelectPackageItemBox::Open(int sourceIndex) {
  this->tabList->ClearData();
  this->itemList->ClearData();
  this->title->SetTitle("", 0);
  this->sourceIndex = sourceIndex;
  const ITEM_INFO item = g_session.LookupClientItemByIdentity(sourceIndex);
  if (item.identity[1] == 0) {
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
  tab->nameText->SetText(data.name);
}

// OFFSET: 0x00A69350
// FUNCTION: fn_16658
CUIFrameWnd* CreateSelectPackageItemBox() {
  return new CUISelectPackageItemBox();
}

// OFFSET: 0x0049B670
// FUNCTION: fn_45555
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateSelectPackageItemBox;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10033, factory);
  return true;
}();

// OFFSET: 0x00A6F470
// FUNCTION: fn_16772
CUISelectPackageItemBox::CUISelectPackageItemBox()
    : title(nullptr), tabList(nullptr), itemList(nullptr), okButton(nullptr), cancelButton(nullptr), sourceIndex(-1), packageId(0) {
}
