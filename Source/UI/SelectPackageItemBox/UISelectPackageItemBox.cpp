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

// OFFSET: 0x00A6D0C0

// OFFSET: 0x00A6DB80

// OFFSET: 0x00A6DD80

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
