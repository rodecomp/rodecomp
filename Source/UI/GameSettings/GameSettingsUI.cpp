#include "UI/GameSettings/GameSettingsUI.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/SkinMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/DisplayMode.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/RenderAdapter.h"
#include "Render/RenderBackends.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/StringUtil.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <list>
#include <string>
#include <vector>

namespace {
  struct RodexSpamOptionPacket {
    /* 0000 */ std::int16_t id;
    /* 0004 */ int option;
    /* 0008 */ int value;
  };
  static_assert(sizeof(RodexSpamOptionPacket) == 12);
}

template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::SelectItem(CUIListItem* item);
template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::SelectItem(CUIListItem* item);
template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::SelectItem(CUIListItem* item);
template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::SelectItem(CUIListItem* item);

// OFFSET: 0x00A344D0
// OFFSET: 0x00A34500
// OFFSET: 0x00A34530
// OFFSET: 0x00A34560
// FUNCTION: fn_15407
// FUNCTION: fn_15408
// FUNCTION: fn_15409
// FUNCTION: fn_15410
template <class T, class D> void CUIComboBox<T, D>::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xC6C6C6);
}

template void CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Paint();
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Paint();
template void CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Paint();
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Paint();

// OFFSET: 0x00A40040
// OFFSET: 0x00A40050
// OFFSET: 0x00A40060
// OFFSET: 0x00A40070
// FUNCTION: fn_15962
// FUNCTION: fn_15963
// FUNCTION: fn_15964
// FUNCTION: fn_15965
template <class T, class D> unsigned int CUIComboBox<T, D>::Selected() const {
  return this->listBox->selected;
}

template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Selected() const;

// OFFSET: 0x00A41FE0
// OFFSET: 0x00A42060
// OFFSET: 0x00A420E0
// OFFSET: 0x00A42160
// FUNCTION: fn_16009
// FUNCTION: fn_16010
// FUNCTION: fn_16011
// FUNCTION: fn_16012
template <class T, class D> void CUIComboBox<T, D>::UpdateListHeight() {
  const int height = std::min<unsigned int>(this->listBox->data.size(), this->visibleItems) * this->itemHeight;
  if (height != this->listBox->geometry.height) {
    this->listBox->Resize(this->listBox->geometry.width, height);
  }
  this->listBox->UpdateScrollBar();
}

template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::UpdateListHeight();
template void CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::UpdateListHeight();
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::UpdateListHeight();
template void CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::UpdateListHeight();

// OFFSET: 0x00A3EA80

template int CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::AddItem(const std::string& value, bool refresh);

// OFFSET: 0x00A34590
// FUNCTION: fn_15411
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A345A0
// FUNCTION: fn_15412
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A345B0
// FUNCTION: fn_15413
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A345C0
// FUNCTION: fn_15414
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A345D0
// FUNCTION: fn_15415
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A345E0
// FUNCTION: fn_15416
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A34620
// FUNCTION: fn_15417
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A34660
// FUNCTION: fn_15418
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A346A0
// FUNCTION: fn_15419
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A346E0
// FUNCTION: fn_15420
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A353D0
// FUNCTION: fn_15426
void CUICheckBox::SetOnCheck(std::function<void(bool)> callback) {
  this->onCheck = callback;
}

// OFFSET: 0x00A36170
// FUNCTION: fn_15437
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::OnScroll(int position) {
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
    GameSettingsUI::CUIComboItem* item = *it;
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

// OFFSET: 0x00A36370
// FUNCTION: fn_15438
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::OnScroll(int position) {
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
    GameSettingsUI::CUIComboItem* item = *it;
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

// OFFSET: 0x00A36570
// FUNCTION: fn_15439
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::OnScroll(int position) {
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
    GameSettingsUI::CUIComboItem* item = *it;
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

// OFFSET: 0x00A36770
// FUNCTION: fn_15440
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::OnScroll(int position) {
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
    GameSettingsUI::CUIComboItem* item = *it;
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

// OFFSET: 0x00A36970
// FUNCTION: fn_15441
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::OnScroll(int position) {
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
    GameSettingsUI::CUIOptionItem* item = *it;
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

// OFFSET: 0x00A36CF0
// FUNCTION: fn_15442
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
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

// OFFSET: 0x00A36DD0
// FUNCTION: fn_15443
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
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

// OFFSET: 0x00A36EB0
// FUNCTION: fn_15444
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
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

// OFFSET: 0x00A36F90
// FUNCTION: fn_15445
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
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

namespace GameSettingsUI {
// OFFSET: 0x00A40420
// FUNCTION: fn_15970
void CUIGroup::Apply() {
}

// OFFSET: 0x00A40430
// FUNCTION: fn_15971
void CUIGroupEmblem::Apply() {
  g_serviceOptions.Set(0xF3, 0);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xDC8)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A40480
// FUNCTION: fn_15972
void CUIGroupLogInOut::Apply() {
  g_serviceOptions.Set(0xA5, 1);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x414)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A404F0
// FUNCTION: fn_15974
void CUIGroupRodexSpam::Apply() {
  RodexSpamOptionPacket packet;
  packet.id = 0x0B93;
  packet.option = 1;
  packet.value = 1;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00A40550
// FUNCTION: fn_15975
void CUIGroupSkin::Apply() {
  g_skinInfo.Select(-1);
}

// OFFSET: 0x00A408D0
// FUNCTION: fn_15977
void CUIPage::Apply() {
  for (UIWindow* child : this->m_children) {
    static_cast<CUIGroup*>(child)->Apply();
  }
}

// OFFSET: 0x00A40B90
// FUNCTION: fn_15979
void CUIGroupSound::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A40BA0
// FUNCTION: fn_15980
void CUIGroupSpriteDetailLevel::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A40BB0
// FUNCTION: fn_15981
void CUIGroupTextureDetailLevel::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A40BC0
// FUNCTION: fn_15982
void CUIListPage::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xCECECE);
}

// OFFSET: 0x00A40BF0
// FUNCTION: fn_15983
void CUIOptionItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A40C00
// FUNCTION: fn_15984
void CUIPage::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xCECECE);
}

// OFFSET: 0x00A40030

// OFFSET: 0x00A3F6A0

// OFFSET: 0x00A3FBD0

// OFFSET: 0x00A42460

// OFFSET: 0x00A427A0

// OFFSET: 0x00A3F950

// OFFSET: 0x00A436E0
// FUNCTION: fn_16034
void CUIGroupGraphics::Refresh() {
  this->LoadAdapters();
}

// OFFSET: 0x00A42BD0

// OFFSET: 0x00A3FAC0
// FUNCTION: fn_15955
void CUIGroupGraphicsAPI::OnSelect(CUIComboItem* item, const RenderBackend& backend) {
  CUIListBoxEx<CUIComboItem, RenderBackend>* list = this->m_combo->listBox;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto node = list->data.begin();
    std::advance(node, static_cast<int>(list->selected));
    this->m_pendingBackend = node->identity;
    static_cast<CUIGraphicsPage*>(this->parent)->LoadAdapters();
  }
}

// OFFSET: 0x00A436F0
// FUNCTION: fn_16035
void CUIGroupGraphicsAPI::Refresh() {
  this->LoadBackends();
}

// OFFSET: 0x00A3FCD0

// OFFSET: 0x00A40C30
// FUNCTION: fn_15985
void CUIComboItem::Slot16() {
}

// OFFSET: 0x00A41D80
// FUNCTION: fn_15999
void CUIListPage::Slot16() {
}

// OFFSET: 0x00A41D90
// FUNCTION: fn_16000
void CUIOptionItem::Slot16() {
}

// OFFSET: 0x00A41DA0
// FUNCTION: fn_16001
void CUIPage::Slot16() {
  int y = 0;
  for (UIWindow* child : this->m_children) {
    child->Move(child->geometry.x, y);
    y += child->geometry.height;
  }
}

// OFFSET: 0x00A3F360
// FUNCTION: fn_15942
void CUIGroupSound::SetBgmEnabled(bool enabled) {
  const unsigned char previous = g_serviceOptions.Get(7);
  const bool muted = !enabled;
  this->m_bgmSlider->SetTransparent(muted);
  this->m_bgmSlider->dragging = muted;
  this->m_bgmSlider->Invalidate();
  g_serviceOptions.Set(7, enabled);
  if (previous != enabled) {
    g_modeMgr.gameMode->SendMsg(0x5A, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A3F3D0
// FUNCTION: fn_15943
void CUIGroupSound::SetBgmVolume(int volume) {
  CSoundMgr::SetVolume(g_soundMgr, volume);
  g_soundMgr->SetTargetVolume(CSoundMgr::Volume(g_soundMgr));
  this->m_bgmSlider->SetThumbTooltip(std::to_string(CSoundMgr::Volume(g_soundMgr)));
}

// OFFSET: 0x00A3F4C0

// OFFSET: 0x00A3F560
// FUNCTION: fn_15945
void CUIGroupEmblem::SetEnabled(bool enabled) {
  g_serviceOptions.Set(0xF3, enabled);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(enabled ? 0xDC7 : 0xDC8)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A3F5C0
// FUNCTION: fn_15946
void CUIGroupLogInOut::SetEnabled(bool enabled) {
  g_serviceOptions.Set(0xA5, enabled);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(enabled ? 0x414 : 0x415)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A3F6B0
// FUNCTION: fn_15948
void CUIGroupSound::OnBgmMute(bool muted) {
  this->SetBgmEnabled(!muted);
}

// OFFSET: 0x00A3F6D0

// OFFSET: 0x00A401C0

// OFFSET: 0x00A40270
// FUNCTION: fn_15968
void CUIListPage::Slot54(CUIOptionItem* item, const TALKTYPE& type) {
}

// OFFSET: 0x00A41E30
// FUNCTION: fn_16003
void CUIGroupEmblem::OnSelect(CUIRadioButton* button) {
  this->SetEnabled(button == this->m_onButton);
}

// OFFSET: 0x00A41E90
// FUNCTION: fn_16005
void CUIGroupLogInOut::OnSelect(CUIRadioButton* button) {
  this->SetEnabled(button == this->m_onButton);
}

// OFFSET: 0x00A41F30

// OFFSET: 0x00A42FE0
// FUNCTION: fn_16020
void CUIGroupSound::OnBgmScroll(int position) {
  this->SetBgmVolume(position);
}

// OFFSET: 0x00A42FF0
// FUNCTION: fn_16021
void CUIGroupSound::OnEffectScroll(int position) {
  this->SetEffectVolume(position);
}

// OFFSET: 0x00A43650

// OFFSET: 0x00A436A0

// OFFSET: 0x00A43700

// OFFSET: 0x00A43810

// OFFSET: 0x00A43870

// OFFSET: 0x00A43A40

// OFFSET: 0x00A43B70
// FUNCTION: fn_16045
void CUIPage::Refresh() {
  for (UIWindow* child : this->m_children) {
    static_cast<CUIGroup*>(child)->Refresh();
  }
}
}

// OFFSET: 0x00A41DE0
// FUNCTION: fn_16002
void CUIGameSettingsUI::Slot25(int x, int y) {
  if (this->m_titleBar->IsPointInside(x, y)) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x00A3F0F0
// FUNCTION: fn_15939
void CUIGameSettingsUI::sub_A3F0F0() {
}

// OFFSET: 0x00A40240
// FUNCTION: fn_15967
bool CUIGameSettingsUI::IsTab4Selected() {
  const int id = this->m_tab->current ? this->m_tab->current->geometry.id : -1;
  return id == 4;
}

// OFFSET: 0x00A43380
// FUNCTION: fn_16024
void CUIGameSettingsUI::sub_A43380() {
}

// OFFSET: 0x00A43390
// FUNCTION: fn_16025
void CUIGameSettingsUI::sub_A43390() {
}

// OFFSET: 0x00A433A0
// FUNCTION: fn_16026
void CUIGameSettingsUI::sub_A433A0() {
}

// OFFSET: 0x00A433B0
// FUNCTION: fn_16027
void CUIGameSettingsUI::sub_A433B0() {
}

// OFFSET: 0x00A433C0
// FUNCTION: fn_16028
void CUIGameSettingsUI::sub_A433C0() {
}
