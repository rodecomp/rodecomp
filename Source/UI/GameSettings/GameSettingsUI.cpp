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

// GLOBAL: gl_24768
int g_configuredRenderBackend;

// GLOBAL: gl_24396
int dwPriorityClass;

// GLOBAL: gl_24509
int g_textureFiltering;

// GLOBAL: gl_24964
int g_textureDetailLevel;

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
// FUNCTION: fn_15933
template <class T, class D> int CUIComboBox<T, D>::AddItem(const D& value, bool refresh) {
  const int index = this->listBox->AddData(value, false);
  if (refresh) {
    this->UpdateListHeight();
  }
  return index;
}

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

// OFFSET: 0x00A1F210
// FUNCTION: fn_15384
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F2E0
// FUNCTION: fn_15385
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F3B0
// FUNCTION: fn_15386
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F480
// FUNCTION: fn_15387
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F550
// FUNCTION: fn_15388
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
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
// FUNCTION: fn_15961
int CUIGraphicsPage::GetPendingBackend() {
  return this->m_graphicsApi->m_pendingBackend;
}

// OFFSET: 0x00A3F6A0
// FUNCTION: fn_15947
void CUIGraphicsPage::LoadAdapters() {
  this->m_graphics->LoadAdapters();
}

// OFFSET: 0x00A3FBD0
// FUNCTION: fn_15959
int CUIGroupGraphics::GetPendingBackend() {
  return static_cast<CUIGraphicsPage*>(this->parent)->GetPendingBackend();
}

// OFFSET: 0x00A42460
// FUNCTION: fn_16015
void CUIGroupGraphics::LoadAdapters() {
  this->m_adapterCombo->listBox->ClearData();

  const std::vector<RenderAdapter> adapters = EnumerateRenderAdapters(this->GetPendingBackend());
  for (const RenderAdapter& adapter : adapters) {
    this->m_adapterCombo->listBox->AddData(adapter, false);
  }
  this->m_adapterCombo->UpdateListHeight();

  std::list<RenderAdapter>& data = this->m_adapterCombo->listBox->data;
  if (data.size() == 0) {
    return;
  }

  const auto found = std::find_if(data.begin(), data.end(), [this](const RenderAdapter& adapter) { return this->m_pendingAdapter.Matches(adapter); });
  const int index = found != data.end() ? static_cast<int>(std::distance(data.begin(), found)) : 0;

  CUIListBoxEx<CUIComboItem, RenderAdapter>* list = this->m_adapterCombo->listBox;
  list->scrollBar->SetPosition(list->rowHeight * index);
  for (CUIComboItem* item : list->items) {
    if (item->index == index) {
      list->SelectItem(item);
      break;
    }
  }
}

// OFFSET: 0x00A427A0
// FUNCTION: fn_16016
void CUIGroupGraphics::LoadModes() {
  this->m_modeCombo->listBox->ClearData();

  const std::vector<DisplayMode> modes = EnumerateDisplayModes(this->GetPendingBackend(), this->m_pendingAdapter.ordinal);
  for (const DisplayMode& mode : modes) {
    this->m_modeCombo->listBox->AddData(mode, false);
  }
  this->m_modeCombo->UpdateListHeight();

  std::list<DisplayMode>& data = this->m_modeCombo->listBox->data;
  if (data.size() == 0) {
    return;
  }

  auto found = std::find_if(data.begin(), data.end(), [this](const DisplayMode& mode) { return this->m_pendingMode.Matches(mode); });
  if (found == data.end()) {
    DisplayMode fallback{};
    ScreenDimensions screen;
    const ScreenDimensions* resolution = DefaultResolution(&screen);
    fallback.width = resolution->width;
    fallback.height = resolution->height;
    fallback.bitsPerPixel = g_configuredBitsPerPixel;
    found = std::find_if(data.begin(), data.end(), [fallback](const DisplayMode& mode) { return fallback.Matches(mode); });
  }
  const int index = found != data.end() ? static_cast<int>(std::distance(data.begin(), found)) : 0;

  CUIListBoxEx<CUIComboItem, DisplayMode>* list = this->m_modeCombo->listBox;
  list->scrollBar->SetPosition(list->rowHeight * index);
  for (CUIComboItem* item : list->items) {
    if (item->index == index) {
      list->SelectItem(item);
      break;
    }
  }
}

// OFFSET: 0x00A3F950
// FUNCTION: fn_15951
void CUIGroupGraphics::OnAdapterSelect(CUIComboItem* item, const RenderAdapter& adapter) {
  CUIListBoxEx<CUIComboItem, RenderAdapter>* list = this->m_adapterCombo->listBox;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto node = list->data.begin();
    std::advance(node, static_cast<int>(list->selected));
    this->m_pendingAdapter = *node;
    this->LoadModes();
  }
}

// OFFSET: 0x00A436E0
// FUNCTION: fn_16034
void CUIGroupGraphics::Refresh() {
  this->LoadAdapters();
}

// OFFSET: 0x00A42BD0
// FUNCTION: fn_16017
void CUIGroupGraphicsAPI::LoadBackends() {
  this->m_combo->listBox->ClearData();

  const std::vector<RenderBackend> backends = EnumerateRenderBackends();
  for (const RenderBackend& backend : backends) {
    this->m_combo->listBox->AddData(backend, false);
  }
  this->m_combo->UpdateListHeight();

  std::list<RenderBackend>& data = this->m_combo->listBox->data;
  if (data.size() == 0) {
    return;
  }

  const auto found = std::find_if(data.begin(), data.end(), [this](const RenderBackend& backend) { return backend.identity == this->m_pendingBackend; });
  const int index = found != data.end() ? static_cast<int>(std::distance(data.begin(), found)) : 0;

  CUIListBoxEx<CUIComboItem, RenderBackend>* list = this->m_combo->listBox;
  list->scrollBar->SetPosition(list->rowHeight * index);
  for (CUIComboItem* item : list->items) {
    if (item->index == index) {
      list->SelectItem(item);
      break;
    }
  }
}

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
// FUNCTION: fn_15960
std::string GetHotkeyText(int tab, int index) {
  HotkeyInfo info;
  const char* keyText = nullptr;
  const char* description = nullptr;
  g_luaScript->Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &info.key1, &info.key2, &keyText, &description);
  if (IsNullOrEmpty(keyText)) {
    g_luaScript->Call("GetOriginalHotKeyInfo", "dd>ddss", tab + 1, index, &info.key1, &info.key2, &keyText, &description);
  }

  std::string text;
  if (IsNullOrEmpty(keyText)) {
    text = "";
  } else {
    text = keyText;
  }
  return text;
}

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
// FUNCTION: fn_15944
void CUIGroupSound::SetEffectVolume(int volume) {
  g_soundMgr->SetSampleVolume(volume);
  g_soundMgr->Set3DVolume(volume);
  this->m_effectSlider->SetThumbTooltip(std::to_string(g_soundMgr->sampleVolume));
}

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
// FUNCTION: fn_15949
void CUIGroupSound::OnEffectMute(bool muted) {
  this->m_effectSlider->SetTransparent(muted);
  this->m_effectSlider->dragging = muted;
  this->m_effectSlider->Invalidate();
  g_serviceOptions.Set(0xB, !muted);
}

// OFFSET: 0x00A401C0
// FUNCTION: fn_15966
bool CUIGraphicsPage::IsModified() {
  if (this->m_graphicsApi->m_backend != this->m_graphicsApi->m_pendingBackend) {
    return true;
  }
  CUIGroupGraphics* graphics = this->m_graphics;
  if (!graphics->m_adapter.Matches(graphics->m_pendingAdapter)) {
    return true;
  }
  if (!graphics->m_mode.Matches(graphics->m_pendingMode)) {
    return true;
  }
  return this->m_fullscreen->m_onButton->checked != (g_configuredFullscreen != 0);
}

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
// FUNCTION: fn_16007
void CUIGroupRodexSpam::OnSelect(CUIRadioButton* button) {
  RodexSpamOptionPacket packet;
  packet.id = 0x0B93;
  packet.option = 1;
  packet.value = button != this->m_offButton;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

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
// FUNCTION: fn_16032
void CUIGroupEmblem::Refresh() {
  const bool enabled = g_serviceOptions.Get(0xF3);
  this->m_onButton->checked = enabled;
  this->m_onButton->Invalidate();
  this->m_offButton->checked = !enabled;
  this->m_offButton->Invalidate();
}

// OFFSET: 0x00A436A0
// FUNCTION: fn_16033
void CUIGroupFullscreen::Refresh() {
  this->m_onButton->checked = g_configuredFullscreen != 0;
  this->m_onButton->Invalidate();
  this->m_offButton->checked = g_configuredFullscreen == 0;
  this->m_offButton->Invalidate();
}

// OFFSET: 0x00A43700
// FUNCTION: fn_16036
void CUIGroupLogInOut::Refresh() {
  this->m_onButton->checked = g_serviceOptions.Get(0xA5);
  this->m_onButton->Invalidate();
  this->m_offButton->checked = !g_serviceOptions.Get(0xA5);
  this->m_offButton->Invalidate();
}

// OFFSET: 0x00A43810
// FUNCTION: fn_16039
void CUIGroupSkin::Refresh() {
  CUIListBoxEx<CUIComboItem, std::string>* list = this->m_combo->listBox;
  const int index = g_skinInfo.selected + 1;
  list->scrollBar->SetPosition(list->rowHeight * index);
  for (CUIComboItem* item : list->items) {
    if (item->index == index) {
      list->SelectItem(item);
      return;
    }
  }
}

// OFFSET: 0x00A43870
// FUNCTION: fn_16040
void CUIGroupSound::Refresh() {
  this->m_bgmSlider->SetTotal(135);
  this->m_bgmSlider->SetVisible(this->m_bgmSlider->PositionFromOffset(8));
  this->m_bgmSlider->SetPosition(CSoundMgr::Volume(g_soundMgr));
  this->m_bgmSlider->SetThumbTooltip(std::to_string(CSoundMgr::Volume(g_soundMgr)));
  const bool bgmMuted = !g_serviceOptions.Get(7);
  this->m_bgmSlider->SetTransparent(bgmMuted);
  this->m_bgmSlider->dragging = bgmMuted;
  this->m_bgmSlider->Invalidate();
  this->m_bgmMute->checked = bgmMuted;
  this->m_bgmMute->Invalidate();

  this->m_effectSlider->SetTotal(135);
  this->m_effectSlider->SetVisible(this->m_effectSlider->PositionFromOffset(8));
  this->m_effectSlider->SetPosition(g_soundMgr->sampleVolume);
  this->m_effectSlider->SetThumbTooltip(std::to_string(g_soundMgr->sampleVolume));
  const bool effectMuted = !g_serviceOptions.Get(0xB);
  this->m_effectSlider->SetTransparent(effectMuted);
  this->m_effectSlider->dragging = effectMuted;
  this->m_effectSlider->Invalidate();
  this->m_effectMute->checked = effectMuted;
  this->m_effectMute->Invalidate();
}

// OFFSET: 0x00A43A40
// FUNCTION: fn_16041
void CUIGroupSpriteDetailLevel::Refresh() {
  this->m_slider->SetTotal(3);
  this->m_slider->SetVisible(1);
  this->m_slider->SetPosition(std::min(static_cast<unsigned int>(g_effectDetailLevel), 2u));
}

// OFFSET: 0x00A43A80
// FUNCTION: fn_16042
void CUIGroupTextureDetailLevel::Refresh() {
  this->m_slider->SetTotal(3);
  this->m_slider->SetVisible(1);
  this->m_slider->SetPosition(std::min(static_cast<unsigned int>(g_textureDetailLevel), 2u));
}

// OFFSET: 0x00A43AC0
// FUNCTION: fn_16043
void CUIGroupTrilinear::Refresh() {
  this->m_onButton->checked = g_textureFiltering != 0;
  this->m_onButton->Invalidate();
  this->m_offButton->checked = g_textureFiltering == 0;
  this->m_offButton->Invalidate();
}

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

// OFFSET: 0x00A42FC0
// FUNCTION: fn_16019
void GameSettingsUI::CUIGroupGraphicsAPI::Apply() {
  this->m_backend = this->m_pendingBackend;
  g_configuredRenderBackend = this->m_pendingBackend;
}

// OFFSET: 0x00A404D0
// FUNCTION: fn_15973
void sub_A404D0() {
  dwPriorityClass = NORMAL_PRIORITY_CLASS;
  SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
}

// OFFSET: 0x00A421E0
// FUNCTION: fn_16013
void CUIGameSettingsUI::sub_A421E0() {
  bool visible = false;
  if (this->m_tab->current && this->m_tab->current->geometry.id == 3) {
    GameSettingsUI::CUIGraphicsPage* page = this->m_graphicsPage;
    if (page->IsModified() || page->m_spriteDetailLevel->m_slider->position != g_effectDetailLevel ||
        page->m_textureDetailLevel->m_slider->position != g_textureDetailLevel ||
        page->m_trilinear->m_onButton->checked != (g_textureFiltering != 0)) {
      visible = true;
    }
  }
  this->m_applyButton->Show(visible);
}

// OFFSET: 0x00A41E50
// FUNCTION: fn_16004
void sub_A41E50(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(0x271E));
  if (wnd) {
    wnd->sub_A421E0();
  }
}

// OFFSET: 0x00A41FA0
// FUNCTION: fn_16008
void sub_A41FA0(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(0x271E));
  if (wnd) {
    wnd->sub_A421E0();
  }
}

// OFFSET: 0x00A43000
// FUNCTION: fn_16022
void sub_A43000(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(0x271E));
  if (wnd) {
    wnd->sub_A421E0();
  }
}

// OFFSET: 0x00A43040
// FUNCTION: fn_16023
void sub_A43040(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(0x271E));
  if (wnd) {
    wnd->sub_A421E0();
  }
}

// OFFSET: 0x00A433D0
// FUNCTION: fn_16029
void CUIGameSettingsUI::sub_A433D0(UIWindow* window) {
  this->m_pageContainer->Show(window != this->m_tab->FindChildById(3));
  this->sub_A421E0();
}
