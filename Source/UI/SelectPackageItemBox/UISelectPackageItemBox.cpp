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
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <new>

// OFFSET: 0x00A684E0
// FUNCTION: fn_16646
template <>
CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), unknown_0084(), unknown_0088(), unknown_008C(), imageNames(), disabledName(), scrollBar(nullptr),
      itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), unknown_00C5{}, items(), data(),
      selected(static_cast<unsigned int>(-1)), unknown_00E0(1), rebind(0), unknown_00E2{}, onSelect(), binder() {
}

// OFFSET: 0x00A686A0
// FUNCTION: fn_16647
template <>
CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), unknown_0084(), unknown_0088(), unknown_008C(), imageNames(), disabledName(), scrollBar(nullptr),
      itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), unknown_00C5{}, items(), data(),
      selected(static_cast<unsigned int>(-1)), unknown_00E0(1), rebind(0), unknown_00E2{}, onSelect(), binder() {
}

// OFFSET: 0x00A68860

// OFFSET: 0x00A689C0

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
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A696C0
// FUNCTION: fn_16663
template <>
void CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
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
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  const std::string name = naming.DisplayName(data, 0);
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
