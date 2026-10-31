#include "UI/GameSettings/GameSettingsUI.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
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
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iterator>
#include <list>
#include <string>
#include <vector>

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

void SetComboItemSkinText(GameSettingsUI::CUIComboItem* item, const std::string& name);
// OFFSET: 0x00A2C3C0
// FUNCTION: fn_15400
void GameSettingsUI::CUIGroupSkin::Slot15(int width, int height) {
  this->m_title = new CUIStaticText(*this->m_context);
  this->m_title->SetSize(75, 13);
  this->m_title->Move(19, 12);
  const std::uint8_t titleSource[] = {0x60, 0xEF, 0xAA, 0x1D, 0xE9};
  thread_local EncryptedName<sizeof(titleSource), 0x79E915E9AF6F55DDULL> titleText(titleSource);
  const char* title = titleText.Text();
  this->m_title->SetText(title, std::strlen(title));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  const std::uint8_t fontSource[] = {0x46, 0x00, 0x91, 0xB0, 0x42, 0xBD};
  thread_local EncryptedName<sizeof(fontSource), 0xB7AFBD2FD9FD7501ULL> fontText(fontSource);
  this->m_title->SetFont(std::string(fontText.Text()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_combo = new CUIComboBox<CUIComboItem, std::string>(*this->m_context);
  reinterpret_cast<void(__thiscall*)(CUIComboBox<CUIComboItem, std::string>*, int, int, int)>(0x00A1E250)(this->m_combo, 162, 16, 5);
  CUIComboBox<CUIComboItem, std::string>* combo = this->m_combo;
  const std::uint8_t disabledSource[] = {0x0F, 0x74, 0x11, 0xEF, 0x59, 0xE1, 0xAA, 0xDE, 0x09, 0x3F, 0x11, 0xD9, 0x24, 0x95, 0x33, 0x71, 0xAE, 0xA8, 0xB8, 0x76, 0xC6, 0x46, 0x01, 0x67, 0xAA, 0xA9, 0xB7, 0x74, 0xFA, 0x4A, 0x33, 0x67, 0xB7, 0xAF, 0xB3, 0x7A, 0xE1, 0x70, 0x0D, 0x67, 0xA1, 0x84, 0xB0, 0x3B, 0xFB, 0x42, 0x1F, 0x13};
  thread_local EncryptedName<sizeof(disabledSource), 0x136F2F9915D1DBCFULL> disabledImage(disabledSource);
  const std::string disabledName(disabledImage.Text());
  const std::uint8_t pressedSource[] = {0xEB, 0x78, 0x89, 0xD5, 0xCF, 0x1F, 0xE6, 0xC6, 0xED, 0x33, 0x89, 0xE3, 0xB2, 0x6B, 0x7F, 0x69, 0x4A, 0xA4, 0x20, 0x4C, 0x50, 0xB8, 0x4D, 0x7F, 0x4E, 0xA5, 0x2F, 0x4E, 0x6C, 0xB4, 0x7F, 0x7F, 0x53, 0xA3, 0x2B, 0x40, 0x77, 0x8E, 0x41, 0x7F, 0x45, 0x88, 0x28, 0x01, 0x6D, 0xBC, 0x53, 0x0B};
  thread_local EncryptedName<sizeof(pressedSource), 0x0B23D10F2F49D72BULL> pressedImage(pressedSource);
  const std::string pressedName(pressedImage.Text());
  const std::uint8_t hoverSource[] = {0x23, 0x7A, 0x93, 0x0D, 0x65, 0xC9, 0x26, 0x7E, 0x25, 0x31, 0x93, 0x3B, 0x18, 0xBD, 0xBF, 0xD1, 0x82, 0xA6, 0x3A, 0x94, 0xFA, 0x6E, 0x8D, 0xC7, 0x86, 0xA7, 0x35, 0x96, 0xC6, 0x62, 0xBF, 0xC7, 0x9B, 0xA1, 0x31, 0x98, 0xDD, 0x58, 0x81, 0xC7, 0x8D, 0x8A, 0x32, 0xD9, 0xC7, 0x6A, 0x93, 0xB3};
  thread_local EncryptedName<sizeof(hoverSource), 0xB3E307A5F753D5E3ULL> hoverImage(hoverSource);
  const std::string hoverName(hoverImage.Text());
  const std::uint8_t normalSource[] = {0x25, 0xD4, 0xF3, 0xD3, 0x8D, 0xAB, 0xE8, 0xD4, 0x23, 0x9F, 0xF3, 0xE5, 0xF0, 0xDF, 0x71, 0x7B, 0x84, 0x08, 0x5A, 0x4A, 0x12, 0x0C, 0x43, 0x6D, 0x80, 0x09, 0x55, 0x48, 0x2E, 0x00, 0x71, 0x6D, 0x9D, 0x0F, 0x51, 0x46, 0x35, 0x3A, 0x4F, 0x6D, 0x8B, 0x24, 0x52, 0x07, 0x2F, 0x08, 0x5D, 0x19};
  thread_local EncryptedName<sizeof(normalSource), 0x192D654D29337BE5ULL> normalImage(normalSource);
  const std::string normalName(normalImage.Text());
  if (combo->unknown_0088 != nullptr) {
    combo->RemoveChild(combo->unknown_0088, 1);
    combo->unknown_0088 = nullptr;
    combo->Resize(combo->geometry.width - 18, combo->geometry.height);
  }
  if (combo->unknown_0088 == nullptr) {
    combo->Resize(combo->geometry.width + 18, combo->geometry.height);
    combo->unknown_0088 = new CUIImageButton(*this->m_context);
    combo->unknown_0088->SetSize(18, 18);
    combo->unknown_0088->Move(combo->geometry.width - 18, 0);
    combo->unknown_0088->SetImages(normalName, hoverName, pressedName, disabledName);
    combo->unknown_0088->SetOnClick([combo]() {
      if (!reinterpret_cast<bool(__fastcall*)(UIWindowMgr*)>(0x00A8C750)(&g_windowMgr)) {
        int x = 0;
        int y = combo->geometry.height - 1;
        combo->ToScreen(x, y);
        reinterpret_cast<void(__thiscall*)(UIWindowMgr*, UIWindow*, UIWindow*, int, int)>(0x00AA27B0)(&g_windowMgr, combo, combo->listBox, x, y);
      }
    });
    combo->AddChild(combo->unknown_0088);
  }

  combo->Move(110, 9);
  combo->onSelect = [this](CUIComboItem*, const std::string&) { reinterpret_cast<void(__fastcall*)(CUIGroupSkin*)>(0x00A3FB50)(this); };
  const std::function<void(CUIComboItem*, const std::string&)> binder = [this](CUIComboItem* item, const std::string& name) {
    SetComboItemSkinText(item, name);
  };
  combo->onChange = binder;
  combo->listBox->SetBinder(binder);
  this->AddChild(this->m_combo);
}

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

// OFFSET: 0x00A41EB0
// FUNCTION: fn_16006
void CUIGroupProcessPriority::OnSelect(CUIRadioButton* button) {
  if (this->m_highButton == button) {
    dwPriorityClass = HIGH_PRIORITY_CLASS;
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
  } else if (this->m_normalButton == button) {
    dwPriorityClass = NORMAL_PRIORITY_CLASS;
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
  } else if (this->m_idleButton == button) {
    dwPriorityClass = IDLE_PRIORITY_CLASS;
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
  }
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

// OFFSET: 0x00A43760
// FUNCTION: fn_16037
void CUIGroupProcessPriority::Refresh() {
  this->m_highButton->checked = dwPriorityClass == HIGH_PRIORITY_CLASS;
  this->m_highButton->Invalidate();
  this->m_normalButton->checked = dwPriorityClass == NORMAL_PRIORITY_CLASS;
  this->m_normalButton->Invalidate();
  this->m_idleButton->checked = dwPriorityClass == IDLE_PRIORITY_CLASS;
  this->m_idleButton->Invalidate();
}

// OFFSET: 0x00A437D0
// FUNCTION: fn_16038
void CUIGroupRodexSpam::Refresh() {
  this->m_onButton->checked = g_gameOptionFlag;
  this->m_onButton->Invalidate();
  this->m_offButton->checked = !g_gameOptionFlag;
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

// OFFSET: 0x00A41E10
// FUNCTION: fn_45466
void CUIGameSettingsUI::SelectTab4() {
  this->m_tab->Select(static_cast<CUITabButton*>(this->m_tab->FindChildById(4)));
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

// OFFSET: 0x00A3F9B0
// FUNCTION: fn_15952
void SetComboItemAdapterText(GameSettingsUI::CUIComboItem* item, const RenderAdapter& adapter) {
  item->m_text->SetText(adapter.description.c_str(), adapter.description.size());
}

// OFFSET: 0x00A3FA90
// FUNCTION: fn_15954
void SetComboItemModeText(GameSettingsUI::CUIComboItem* item, const DisplayMode& mode) {
  item->m_text->SetText(mode.description.c_str(), mode.description.size());
}

// OFFSET: 0x00A3FB20
// FUNCTION: fn_15956
void SetComboItemBackendText(GameSettingsUI::CUIComboItem* item, const RenderBackend& backend) {
  item->m_text->SetText(backend.name.c_str(), backend.name.size());
}

// OFFSET: 0x00A3FBA0
// FUNCTION: fn_15958
void SetComboItemSkinText(GameSettingsUI::CUIComboItem* item, const std::string& name) {
  item->m_text->SetText(name.c_str(), name.size());
}

// OFFSET: 0x00A41C60
// FUNCTION: fn_15996
void GameSettingsUI::CUIGroupSpriteDetailLevel::Slot16() {
  const char* text = LookupMessage(0x144);
  this->m_title->SetText(text, std::strlen(text));
}

// OFFSET: 0x00A41CA0
// FUNCTION: fn_15997
void GameSettingsUI::CUIGroupTextureDetailLevel::Slot16() {
  const char* text = LookupMessage(0x145);
  this->m_title->SetText(text, std::strlen(text));
}

// OFFSET: 0x00AA2760
// FUNCTION: fn_17216
void ToggleGameSettingsTab4() {
  UIWindow* window = g_windowMgr.LookupWindow(0x271E);
  if (!window) {
    window = g_windowMgr.EnsureWindow(0x271E);
  }

  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(window);
  if (settings->IsTab4Selected()) {
    g_windowMgr.CloseWindow(0x271E);
    return;
  }

  settings->SelectTab4();
}

// OFFSET: 0x00AAB070
// FUNCTION: fn_17277
void UIWindowMgr::sub_AAB070(int option) {
  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(this->LookupWindow(0x271E));
  if (settings) {
    settings->sub_A43590(option);
  }
}

// OFFSET: 0x00AAB090
// FUNCTION: fn_17278
void UIWindowMgr::sub_AAB090() {
  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(this->LookupWindow(0x271E));
  if (settings) {
    settings->sub_A43BA0();
  }
}

// OFFSET: 0x00AAB0B0
// FUNCTION: fn_17279
void UIWindowMgr::sub_AAB0B0() {
  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(this->LookupWindow(0x271E));
  if (settings) {
    settings->sub_A43C70();
  }
}

// OFFSET: 0x00A1BFD0
// FUNCTION: fn_15374
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::ClearItems() {
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A1C030
// FUNCTION: fn_15375
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::ClearItems() {
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A1C090
// FUNCTION: fn_15376
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::ClearItems() {
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A1C0F0
// FUNCTION: fn_15377
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::ClearItems() {
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A1C150
// FUNCTION: fn_15378
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::ClearItems() {
  for (GameSettingsUI::CUIOptionItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A43B00
// FUNCTION: fn_16044
bool GameSettingsUI::CUIListPage::SetOptionChecked(int id) {
  for (GameSettingsUI::CUIOptionItem* item : this->m_optionList->items) {
    if (item->m_optionId == id) {
      CUICheckBox* check = item->m_check;
      check->checked = IsServiceOption(id) ^ g_serviceOptions.Get(id);
      check->Invalidate();
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00A43590
// FUNCTION: fn_16031
__declspec(noinline) void CUIGameSettingsUI::sub_A43590(int id) {
  for (GameSettingsUI::CUIListPage* page : this->m_pages) {
    for (GameSettingsUI::CUIOptionItem* item : page->m_optionList->items) {
      if (item->m_optionId == id) {
        CUICheckBox* check = item->m_check;
        check->checked = IsServiceOption(id) ^ g_serviceOptions.Get(id);
        check->Invalidate();
        return;
      }
    }
  }
  this->m_currentPage->Refresh();
}

// OFFSET: 0x00A41120
// FUNCTION: fn_15987
void GameSettingsUI::CUIGroupEmblem::Slot16() {
  const char* title = LookupMessage(0x1032);
  this->m_title->SetText(title, std::strlen(title));
  const char* onText = LookupMessage(0x1035);
  this->m_onText->SetText(onText, std::strlen(onText));
  const char* offText = LookupMessage(0x1036);
  this->m_offText->SetText(offText, std::strlen(offText));
}

// OFFSET: 0x00A411C0
// FUNCTION: fn_15988
void GameSettingsUI::CUIGroupFullscreen::Slot16() {
  const char* title = LookupMessage(0x1057);
  this->m_title->SetText(title, std::strlen(title));
  const char* onText = LookupMessage(0x1058);
  this->m_onText->SetText(onText, std::strlen(onText));
  const char* offText = LookupMessage(0x1059);
  this->m_offText->SetText(offText, std::strlen(offText));
}

// OFFSET: 0x00A41510
// FUNCTION: fn_15990
void GameSettingsUI::CUIGroupGraphicsAPI::Slot16() {
  const char* text = LookupMessage(0x1001);
  this->m_title->SetText(text, std::strlen(text));
  const int backend = GetRenderSystemHolder()->Backend();
  this->m_pendingBackend = backend;
  this->m_backend = backend;
}

// OFFSET: 0x00A41560

// OFFSET: 0x00A41710
// FUNCTION: fn_15992
void GameSettingsUI::CUIGroupProcessPriority::Slot16() {
  const char* title = LookupMessage(0x1039);
  this->m_title->SetText(title, std::strlen(title));
  const char* highText = LookupMessage(0x103A);
  this->m_highText->SetText(highText, std::strlen(highText));
  const char* highTooltip = LookupMessage(0x103B);
  this->m_highText->SetTooltip(highTooltip, std::strlen(highTooltip));
  const char* normalText = LookupMessage(0x103C);
  this->m_normalText->SetText(normalText, std::strlen(normalText));
  const char* normalTooltip = LookupMessage(0x103D);
  this->m_normalText->SetTooltip(normalTooltip, std::strlen(normalTooltip));
  const char* idleText = LookupMessage(0x103E);
  this->m_idleText->SetText(idleText, std::strlen(idleText));
  const char* idleTooltip = LookupMessage(0x103F);
  this->m_idleText->SetTooltip(idleTooltip, std::strlen(idleTooltip));
}

// OFFSET: 0x00A41870
// FUNCTION: fn_15993
void GameSettingsUI::CUIGroupRodexSpam::Slot16() {
  const char* title = LookupMessage(0xC40);
  this->m_title->SetText(title, std::strlen(title));
  const char* onText = LookupMessage(0x1038);
  this->m_onText->SetText(onText, std::strlen(onText));
  const char* offText = LookupMessage(0x1037);
  this->m_offText->SetText(offText, std::strlen(offText));
}

// OFFSET: 0x00A41CE0
// FUNCTION: fn_15998
void GameSettingsUI::CUIGroupTrilinear::Slot16() {
  const char* title = LookupMessage(0x105A);
  this->m_title->SetText(title, std::strlen(title));
  const char* onText = LookupMessage(0x1033);
  this->m_onText->SetText(onText, std::strlen(onText));
  const char* offText = LookupMessage(0x1034);
  this->m_offText->SetText(offText, std::strlen(offText));
}

// OFFSET: 0x00A19560
// FUNCTION: fn_15342
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A19720
// FUNCTION: fn_15343
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A198E0
// FUNCTION: fn_15344
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A19AA0
// FUNCTION: fn_15345
template <>
CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A41B60
// FUNCTION: fn_15995
void GameSettingsUI::CUIGroupSound::Slot16() {
  const char* title = LookupMessage(0x5CD);
  this->m_title->SetText(title, std::strlen(title));
  const char* bgmText = LookupMessage(0x5D7);
  this->m_bgmText->SetText(bgmText, std::strlen(bgmText));
  const char* bgmOffText = LookupMessage(0x1034);
  this->unknown_0090->SetText(bgmOffText, std::strlen(bgmOffText));
  const char* effectText = LookupMessage(0x5D8);
  this->m_effectText->SetText(effectText, std::strlen(effectText));
  const char* effectOffText = LookupMessage(0x1034);
  this->unknown_00A0->SetText(effectOffText, std::strlen(effectOffText));
}

// OFFSET: 0x00A41560
// FUNCTION: fn_15991
void GameSettingsUI::CUIGroupLogInOut::Slot16() {
  const char* title = LookupMessage(0x107D);
  this->m_title->SetText(title, std::strlen(title));
  const char* onText = LookupMessage(0x1033);
  this->m_onText->SetText(onText, std::strlen(onText));
  const char* offText = LookupMessage(0x1034);
  this->m_offText->SetText(offText, std::strlen(offText));
  const std::uint8_t source[] = {0x8A, 0xF1, 0x02, 0x9E, 0x0A, 0x49, 0x18, 0x76, 0xD1, 0x97, 0x42, 0x95, 0x0A, 0x27};
  thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> tooltip(source);
  const char* tooltipText = tooltip.Text();
  this->m_title->SetTooltip(tooltipText, std::strlen(tooltipText));
}

// OFFSET: 0x00A1A320

// OFFSET: 0x00A1A480

// OFFSET: 0x00A1A5E0

// OFFSET: 0x00A1F890
// FUNCTION: fn_15390
void GameSettingsUI::CUIComboItem::Slot15(int width, int height) {
  this->m_text = new CUIStaticText(*this->m_context);
  this->m_text->SetSize(155, 16);
  this->m_text->Move(3, 0);
  this->m_text->SetText("", 0);
  this->m_text->SetFitText(1);
  this->m_text->SetAlign(0, 6);
  const std::uint8_t fontSource[] = {0x74, 0x16, 0x39, 0x98, 0x44, 0x35};
  thread_local EncryptedName<sizeof(fontSource), 0x49D33529F1556333ULL> fontText(fontSource);
  this->m_text->SetFont(std::string(fontText.Text()));
  this->m_text->SetFontHeight(12);
  this->m_text->SetBold(0);
  this->m_text->SetColor(0);
  this->m_text->SetShadow(0, 0xFFFFFF);
  this->m_text->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_text);
}

// OFFSET: 0x00A3F9E0
// FUNCTION: fn_15953
void GameSettingsUI::CUIGroupGraphics::OnModeSelect(CUIComboItem* item, const DisplayMode& mode) {
  CUIListBoxEx<CUIComboItem, DisplayMode>* list = this->m_modeCombo->listBox;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto node = list->data.begin();
    std::advance(node, static_cast<int>(list->selected));
    this->m_pendingMode = *node;
    CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(0x271E));
    if (wnd) {
      wnd->sub_A421E0();
    }
  }
}

// OFFSET: 0x00A41910
// FUNCTION: fn_15994
void GameSettingsUI::CUIGroupSkin::Slot16() {
  const char* title = LookupMessage(0x5D9);
  this->m_title->SetText(title, std::strlen(title));
  this->m_combo->AddItem(std::string(LookupMessage(0xC12)), false);
  for (int index = 0; index < static_cast<int>(g_skinInfo.prefixes.size()); ++index) {
    this->m_combo->AddItem(std::string(g_skinInfo.Name(index)), false);
  }
  this->m_combo->UpdateListHeight();
}

// OFFSET: 0x00A40560
// FUNCTION: fn_15976
void GameSettingsUI::CUIGroupSound::Apply() {
  const std::uint8_t bgmVolumeSource[] = {0xCD, 0xDA, 0x82, 0xD8, 0x2B, 0xAA, 0xC1, 0x00, 0xE2, 0xD8, 0xEF};
  thread_local EncryptedName<sizeof(bgmVolumeSource), 0x75ADC57D87EFBD8FULL> bgmVolumeName(bgmVolumeSource);
  this->SetBgmVolume(g_session.GetDefaultOptionValue(bgmVolumeName.Text(), 100));
  const std::uint8_t bgmSource[] = {0x42, 0xD9, 0xF2, 0x40, 0x93};
  thread_local EncryptedName<sizeof(bgmSource), 0xC517FB932D95BB6DULL> bgmName(bgmSource);
  this->SetBgmEnabled(g_session.GetDefaultOptionValue(bgmName.Text(), 1) != 0);
  const std::uint8_t effectVolumeSource[] = {0x8C, 0x93, 0x39, 0xEE, 0xEC, 0xA1, 0xE4, 0x75, 0xA6, 0x99, 0x2A, 0xE6, 0xEA, 0xD5};
  thread_local EncryptedName<sizeof(effectVolumeSource), 0x23BBD58F8B5FF5C9ULL> effectVolumeName(effectVolumeSource);
  this->SetEffectVolume(g_session.GetDefaultOptionValue(effectVolumeName.Text(), 100));
  const std::uint8_t effectSource[] = {0xA8, 0x60, 0x04, 0x0A, 0x57, 0x6D, 0x71};
  thread_local EncryptedName<sizeof(effectSource), 0x0B7109397F6B1387ULL> effectName(effectSource);
  const int effect = g_session.GetDefaultOptionValue(effectName.Text(), 1);
  const bool muted = effect == 0;
  this->m_effectSlider->SetTransparent(muted);
  this->m_effectSlider->dragging = muted;
  this->m_effectSlider->Invalidate();
  g_serviceOptions.Set(0xB, effect != 0);
}

// OFFSET: 0x00A19D30
// FUNCTION: fn_15347
CUIRadioButton::CUIRadioButton(UIWindowContext& context) : CUICheckBox(context), onSelect() {
}

// OFFSET: 0x00A3C390
// FUNCTION: fn_15886
GameSettingsUI::CUIBasicPage::CUIBasicPage(UIWindowContext& context) : CUIPage(context), m_skin(nullptr), m_sound(nullptr), m_emblem(nullptr), m_rodexSpam(nullptr), m_processPriority(nullptr), m_logInOut(nullptr) {
}

// OFFSET: 0x00A3C5D0
// FUNCTION: fn_15889
GameSettingsUI::CUIGraphicsPage::CUIGraphicsPage(UIWindowContext& context) : CUIPage(context), m_graphicsApi(nullptr), m_graphics(nullptr), m_spriteDetailLevel(nullptr), m_textureDetailLevel(nullptr), m_fullscreen(nullptr), m_trilinear(nullptr) {
}

// OFFSET: 0x00A3C680
// FUNCTION: fn_15890
GameSettingsUI::CUIGroupEmblem::CUIGroupEmblem(UIWindowContext& context) : CUIGroup(context), m_onButton(nullptr), m_onText(nullptr), m_offButton(nullptr), m_offText(nullptr) {
}

// OFFSET: 0x00A3C730
// FUNCTION: fn_15891
GameSettingsUI::CUIGroupFullscreen::CUIGroupFullscreen(UIWindowContext& context) : CUIGroup(context), m_onButton(nullptr), m_onText(nullptr), m_offButton(nullptr), m_offText(nullptr) {
}

// OFFSET: 0x00A3C910
// FUNCTION: fn_15893
GameSettingsUI::CUIGroupGraphicsAPI::CUIGroupGraphicsAPI(UIWindowContext& context) : CUIGroup(context), m_combo(nullptr), m_backend(-1), m_pendingBackend(-1) {
}

// OFFSET: 0x00A3C9B0
// FUNCTION: fn_15894
GameSettingsUI::CUIGroupLogInOut::CUIGroupLogInOut(UIWindowContext& context) : CUIGroup(context), m_onButton(nullptr), m_onText(nullptr), m_offButton(nullptr), m_offText(nullptr) {
}

// OFFSET: 0x00A3CA60
// FUNCTION: fn_15895
GameSettingsUI::CUIGroupProcessPriority::CUIGroupProcessPriority(UIWindowContext& context) : CUIGroup(context), m_highButton(nullptr), m_highText(nullptr), m_normalButton(nullptr), m_normalText(nullptr), m_idleButton(nullptr), m_idleText(nullptr) {
}

// OFFSET: 0x00A3CB20
// FUNCTION: fn_15896
GameSettingsUI::CUIGroupRodexSpam::CUIGroupRodexSpam(UIWindowContext& context) : CUIGroup(context), m_onButton(nullptr), m_onText(nullptr), m_offButton(nullptr), m_offText(nullptr) {
}

// OFFSET: 0x00A3CBD0
// FUNCTION: fn_15897
GameSettingsUI::CUIGroupSkin::CUIGroupSkin(UIWindowContext& context) : CUIGroup(context), m_combo(nullptr) {
}

// OFFSET: 0x00A3CC60
// FUNCTION: fn_15898
GameSettingsUI::CUIGroupSound::CUIGroupSound(UIWindowContext& context) : CUIGroup(context), m_bgmText(nullptr), m_bgmSlider(nullptr), m_bgmMute(nullptr), unknown_0090(nullptr), m_effectText(nullptr), m_effectSlider(nullptr), m_effectMute(nullptr), unknown_00A0(nullptr) {
}

// OFFSET: 0x00A3CD30
// FUNCTION: fn_15899
GameSettingsUI::CUIGroupSpriteDetailLevel::CUIGroupSpriteDetailLevel(UIWindowContext& context) : CUIGroup(context), m_slider(nullptr) {
}

// OFFSET: 0x00A3CDC0
// FUNCTION: fn_15900
GameSettingsUI::CUIGroupTextureDetailLevel::CUIGroupTextureDetailLevel(UIWindowContext& context) : CUIGroup(context), m_slider(nullptr) {
}

// OFFSET: 0x00A3CE50
// FUNCTION: fn_15901
GameSettingsUI::CUIGroupTrilinear::CUIGroupTrilinear(UIWindowContext& context) : CUIGroup(context), m_onButton(nullptr), m_onText(nullptr), m_offButton(nullptr), m_offText(nullptr) {
}

// OFFSET: 0x00A3CF00
// FUNCTION: fn_15902
GameSettingsUI::CUIListPage::CUIListPage(UIWindowContext& context) : CUIControl(context), m_optionList(nullptr) {
}

// OFFSET: 0x00A1F620
// FUNCTION: fn_15389
void GameSettingsUI::CUIBasicPage::Slot15(int width, int height) {
  this->m_skin = new CUIGroupSkin(*this->m_context);
  this->m_skin->SetSize(400, 36);
  this->m_skin->Move(0, 0);
  this->AddChild(this->m_skin);
  this->m_sound = new CUIGroupSound(*this->m_context);
  this->m_sound->SetSize(400, 60);
  this->m_sound->Move(0, 36);
  this->AddChild(this->m_sound);
  this->m_emblem = new CUIGroupEmblem(*this->m_context);
  this->m_emblem->SetSize(400, 36);
  this->m_emblem->Move(0, 96);
  this->AddChild(this->m_emblem);
  this->m_rodexSpam = new CUIGroupRodexSpam(*this->m_context);
  this->m_rodexSpam->SetSize(400, 60);
  this->m_rodexSpam->Move(0, 132);
  this->AddChild(this->m_rodexSpam);
  this->m_processPriority = new CUIGroupProcessPriority(*this->m_context);
  this->m_processPriority->SetSize(400, 36);
  this->m_processPriority->Move(0, 192);
  this->AddChild(this->m_processPriority);
  this->m_logInOut = new CUIGroupLogInOut(*this->m_context);
  this->m_logInOut->SetSize(400, 36);
  this->m_logInOut->Move(0, 228);
  this->AddChild(this->m_logInOut);
}

// INLINED
inline CUIHScrollBar::CUIHScrollBar(UIWindowContext& context) : CUIVScrollBar(context) {
}

// OFFSET: 0x00A23070
// FUNCTION: fn_15393
void GameSettingsUI::CUIGroupEmblem::Slot15(int width, int height) {
  this->m_title = new CUIStaticText(*this->m_context);
  this->m_title->SetSize(85, 13);
  this->m_title->Move(19, 12);
  const std::uint8_t titleNameSource[] = {0x54, 0xFA, 0xB3, 0xDE, 0xC8, 0x1C, 0x11, 0x86, 0x3C, 0xEA, 0xC7, 0x8B, 0xD1, 0xA1};
  thread_local EncryptedName<sizeof(titleNameSource), 0x4331A17F33095FEBULL> titleName(titleNameSource);
  const char* titleText = titleName.Text();
  this->m_title->SetText(titleText, std::strlen(titleText));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  const std::uint8_t titleFontSource[] = {0x34, 0x52, 0x21, 0x24, 0x94, 0x6F};
  thread_local EncryptedName<sizeof(titleFontSource), 0xD1056FF94D4D2773ULL> titleFont(titleFontSource);
  this->m_title->SetFont(std::string(titleFont.Text()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton(*this->m_context);
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(110, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  this->m_onButton->disabled = 0;
  this->m_onButton->Invalidate();
  const std::uint8_t onButtonNormalSource[] = {0xD7, 0x76, 0xF3, 0x53, 0xB9, 0x29, 0x52, 0xC2, 0xD1, 0x3D, 0xF3, 0x65, 0xC4, 0x5D, 0xCB, 0x7D, 0x76, 0xBD, 0x5A, 0xC6, 0x1B, 0x93, 0xF9, 0x50, 0x78, 0xBF, 0x55, 0x87, 0x1B, 0x8A, 0xE7, 0x0F};
  thread_local EncryptedName<sizeof(onButtonNormalSource), 0x0F97E779A933D917ULL> onButtonNormal(onButtonNormalSource);
  const std::uint8_t onButtonHoverSource[] = {0x81, 0x7E, 0x21, 0x19, 0xAB, 0x87, 0x4C, 0xFA, 0x87, 0x35, 0x21, 0x2F, 0xD6, 0xF3, 0xD5, 0x45, 0x20, 0xB5, 0x88, 0x8C, 0x09, 0x3D, 0xE7, 0x68, 0x2E, 0xBF, 0xCF, 0x81, 0x06, 0x39, 0x89};
  thread_local EncryptedName<sizeof(onButtonHoverSource), 0x3789496BE3E1D141ULL> onButtonHover(onButtonHoverSource);
  const std::uint8_t onButtonPressedSource[] = {0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0xB7, 0x0C, 0xDF, 0xFC, 0x42, 0xF1, 0x8F, 0x79, 0x9A, 0x02, 0xDD, 0xF3, 0x03, 0xF1, 0x96, 0x67, 0xC5};
  thread_local EncryptedName<sizeof(onButtonPressedSource), 0xC517FB932D95BB6DULL> onButtonPressed(onButtonPressedSource);
  const std::uint8_t onButtonDisabledSource[] = {0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x51, 0xA8, 0x91, 0x36, 0xE4, 0xED, 0xA1, 0xD5, 0x7C, 0xA6, 0x93, 0x39, 0xA5, 0xED, 0xB8, 0xCB, 0x23};
  thread_local EncryptedName<sizeof(onButtonDisabledSource), 0x23BBD58F8B5FF5C9ULL> onButtonDisabled(onButtonDisabledSource);
  this->m_onButton->SetImages(std::string(onButtonNormal.Text()), std::string(onButtonHover.Text()), std::string(onButtonPressed.Text()), std::string(onButtonDisabled.Text()));
  this->m_onButton->checked = false;
  this->m_onButton->Invalidate();
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText(*this->m_context);
  this->m_onText->SetSize(50, 13);
  this->m_onText->Move(127, 12);
  const std::uint8_t onLabelNameSource[] = {0x3D, 0xAB, 0xAB, 0xB3, 0x88, 0xEB, 0x71};
  thread_local EncryptedName<sizeof(onLabelNameSource), 0x0B7109397F6B1387ULL> onLabelName(onLabelNameSource);
  const char* onLabelText = onLabelName.Text();
  this->m_onText->SetText(onLabelText, std::strlen(onLabelText));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  const std::uint8_t onLabelFontSource[] = {0xE2, 0xE8, 0x01, 0x90, 0x0E, 0x27};
  thread_local EncryptedName<sizeof(onLabelFontSource), 0x03772763F96D9DA5ULL> onLabelFont(onLabelFontSource);
  this->m_onText->SetFont(std::string(onLabelFont.Text()));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton(*this->m_context);
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(205, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  this->m_offButton->disabled = 0;
  this->m_offButton->Invalidate();
  const std::uint8_t offButtonNormalSource[] = {0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x65, 0x56, 0x23, 0xC4, 0x56, 0xDD, 0x91, 0xC7, 0x48, 0x58, 0x21, 0xCB, 0x17, 0xDD, 0x88, 0xD9, 0x17};
  thread_local EncryptedName<sizeof(offButtonNormalSource), 0x17A9E5BF39AD4737ULL> offButtonNormal(offButtonNormalSource);
  const std::uint8_t offButtonHoverSource[] = {0xA7, 0x04, 0x51, 0x5F, 0x61, 0xF1, 0x26, 0xDE, 0xA1, 0x4F, 0x51, 0x69, 0x1C, 0x85, 0xBF, 0x61, 0x06, 0xCF, 0xF8, 0xCA, 0xC3, 0x4B, 0x8D, 0x4C, 0x08, 0xC5, 0xBF, 0xC7, 0xCC, 0x4F, 0xE3};
  thread_local EncryptedName<sizeof(offButtonHoverSource), 0x13E33FA1A591AB67ULL> offButtonHover(offButtonHoverSource);
  const std::uint8_t offButtonPressedSource[] = {0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0xBF, 0x0E, 0x19, 0x92, 0x4C, 0x13, 0x73, 0xE1, 0x92, 0x00, 0x1B, 0x9D, 0x0D, 0x13, 0x6A, 0xFF, 0xCD};
  thread_local EncryptedName<sizeof(offButtonPressedSource), 0xCD8F077123FB7D6FULL> offButtonPressed(offButtonPressedSource);
  const std::uint8_t offButtonDisabledSource[] = {0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xEB, 0x22, 0x4D, 0x3A, 0x48, 0xB7, 0xCB, 0x27, 0xC6, 0x2C, 0x4F, 0x35, 0x09, 0xB7, 0xD2, 0x39, 0x99};
  thread_local EncryptedName<sizeof(offButtonDisabledSource), 0x9949BFD527532943ULL> offButtonDisabled(offButtonDisabledSource);
  this->m_offButton->SetImages(std::string(offButtonNormal.Text()), std::string(offButtonHover.Text()), std::string(offButtonPressed.Text()), std::string(offButtonDisabled.Text()));
  this->m_offButton->checked = false;
  this->m_offButton->Invalidate();
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText(*this->m_context);
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(222, 12);
  const std::uint8_t offLabelNameSource[] = {0xED, 0x70, 0x7C, 0xE3, 0x0A, 0xDF, 0x41};
  thread_local EncryptedName<sizeof(offLabelNameSource), 0xD1413DBB01CD8B51ULL> offLabelName(offLabelNameSource);
  const char* offLabelText = offLabelName.Text();
  this->m_offText->SetText(offLabelText, std::strlen(offLabelText));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  const std::uint8_t offLabelFontSource[] = {0x2E, 0x6A, 0x1B, 0x3C, 0xF0, 0x95};
  thread_local EncryptedName<sizeof(offLabelFontSource), 0x637B959D55771F69ULL> offLabelFont(offLabelFontSource);
  this->m_offText->SetFont(std::string(offLabelFont.Text()));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A24510
// FUNCTION: fn_15394
void GameSettingsUI::CUIGroupFullscreen::Slot15(int width, int height) {
  this->m_title = new CUIStaticText(*this->m_context);
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  const std::uint8_t titleNameSource[] = {0xC3, 0x18, 0x31, 0xFC, 0x29, 0x0F, 0x69, 0x76, 0xEE, 0xB5};
  thread_local EncryptedName<sizeof(titleNameSource), 0xC399B7091589B50BULL> titleName(titleNameSource);
  const char* titleText = titleName.Text();
  this->m_title->SetText(titleText, std::strlen(titleText));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  const std::uint8_t titleFontSource[] = {0x28, 0xD8, 0x4D, 0x80, 0x52, 0x07};
  thread_local EncryptedName<sizeof(titleFontSource), 0xB739073FE921AD6FULL> titleFont(titleFontSource);
  this->m_title->SetFont(std::string(titleFont.Text()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton(*this->m_context);
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(150, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { sub_A41E50(reinterpret_cast<int>(button)); });
  this->m_onButton->disabled = 0;
  this->m_onButton->Invalidate();
  const std::uint8_t onButtonNormalSource[] = {0x8F, 0xE4, 0x73, 0x75, 0xED, 0xDB, 0x84, 0x4A, 0x89, 0xAF, 0x73, 0x43, 0x90, 0xAF, 0x1D, 0xF5, 0x2E, 0x2F, 0xDA, 0xE0, 0x4F, 0x61, 0x2F, 0xD8, 0x20, 0x2D, 0xD5, 0xA1, 0x4F, 0x78, 0x31, 0x87};
  thread_local EncryptedName<sizeof(onButtonNormalSource), 0x8741152D8FB34B4FULL> onButtonNormal(onButtonNormalSource);
  const std::uint8_t onButtonHoverSource[] = {0xC7, 0x14, 0x29, 0xB7, 0x39, 0x83, 0x98, 0x58, 0xC1, 0x5F, 0x29, 0x81, 0x44, 0xF7, 0x01, 0xE7, 0x66, 0xDF, 0x80, 0x22, 0x9B, 0x39, 0x33, 0xCA, 0x68, 0xD5, 0xC7, 0x2F, 0x94, 0x3D, 0x5D};
  thread_local EncryptedName<sizeof(onButtonHoverSource), 0x955D4DF94DE9BB07ULL> onButtonHover(onButtonHoverSource);
  const std::uint8_t onButtonPressedSource[] = {0xD7, 0x56, 0x8F, 0x43, 0x3B, 0x7B, 0xEA, 0x08, 0xD1, 0x1D, 0x8F, 0x75, 0x46, 0x0F, 0x73, 0xB7, 0x76, 0x9D, 0x26, 0xD6, 0x99, 0xC1, 0x41, 0x9A, 0x78, 0x9F, 0x29, 0x97, 0x99, 0xD8, 0x5F, 0xC5};
  thread_local EncryptedName<sizeof(onButtonPressedSource), 0xC52FB5FBB94FF917ULL> onButtonPressed(onButtonPressedSource);
  const std::uint8_t onButtonDisabledSource[] = {0x05, 0x62, 0x47, 0x0F, 0x61, 0x6D, 0x22, 0x5E, 0x03, 0x29, 0x47, 0x39, 0x1C, 0x19, 0xBB, 0xE1, 0xA4, 0xA9, 0xEE, 0x9A, 0xC3, 0xD7, 0x89, 0xCC, 0xAA, 0xAB, 0xE1, 0xDB, 0xC3, 0xCE, 0x97, 0x93};
  thread_local EncryptedName<sizeof(onButtonDisabledSource), 0x93E7A3A1F587CDC5ULL> onButtonDisabled(onButtonDisabledSource);
  this->m_onButton->SetImages(std::string(onButtonNormal.Text()), std::string(onButtonHover.Text()), std::string(onButtonPressed.Text()), std::string(onButtonDisabled.Text()));
  this->m_onButton->checked = false;
  this->m_onButton->Invalidate();
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText(*this->m_context);
  this->m_onText->SetSize(150, 13);
  this->m_onText->Move(167, 12);
  const std::uint8_t onLabelNameSource[] = {0xE7, 0xFD, 0xC4, 0xD1, 0xCB, 0x68, 0x4B, 0xC6, 0x07, 0xB9, 0xF7, 0xD8, 0xE6, 0xC5};
  thread_local EncryptedName<sizeof(onLabelNameSource), 0x2FF3C5036D070127ULL> onLabelName(onLabelNameSource);
  const char* onLabelText = onLabelName.Text();
  this->m_onText->SetText(onLabelText, std::strlen(onLabelText));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  const std::uint8_t onLabelFontSource[] = {0xF4, 0x12, 0xAF, 0x3A, 0x94, 0x9B};
  thread_local EncryptedName<sizeof(onLabelFontSource), 0x17359BF953C367B3ULL> onLabelFont(onLabelFontSource);
  this->m_onText->SetFont(std::string(onLabelFont.Text()));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton(*this->m_context);
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(270, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { sub_A41E50(reinterpret_cast<int>(button)); });
  this->m_offButton->disabled = 0;
  this->m_offButton->Invalidate();
  const std::uint8_t offButtonNormalSource[] = {0x95, 0x52, 0x75, 0x21, 0x07, 0x7F, 0xB0, 0xFE, 0x93, 0x19, 0x75, 0x17, 0x7A, 0x0B, 0x29, 0x41, 0x34, 0x99, 0xDC, 0xB4, 0xA5, 0xC5, 0x1B, 0x6C, 0x3A, 0x9B, 0xD3, 0xF5, 0xA5, 0xDC, 0x05, 0x33};
  thread_local EncryptedName<sizeof(offButtonNormalSource), 0x3375B1C7DBB5FD55ULL> offButtonNormal(offButtonNormalSource);
  const std::uint8_t offButtonHoverSource[] = {0xDF, 0x9E, 0x0D, 0x1D, 0x2D, 0xBB, 0xA8, 0xD0, 0xD9, 0xD5, 0x0D, 0x2B, 0x50, 0xCF, 0x31, 0x6F, 0x7E, 0x55, 0xA4, 0x88, 0x8F, 0x01, 0x03, 0x42, 0x70, 0x5F, 0xE3, 0x85, 0x80, 0x05, 0x6D};
  thread_local EncryptedName<sizeof(offButtonHoverSource), 0x1D6D75EDE7CD311FULL> offButtonHover(offButtonHoverSource);
  const std::uint8_t offButtonPressedSource[] = {0x61, 0x92, 0x5F, 0x2D, 0xA3, 0x41, 0xE2, 0x94, 0x67, 0xD9, 0x5F, 0x1B, 0xDE, 0x35, 0x7B, 0x2B, 0xC0, 0x59, 0xF6, 0xB8, 0x01, 0xFB, 0x49, 0x06, 0xCE, 0x5B, 0xF9, 0xF9, 0x01, 0xE2, 0x57, 0x59};
  thread_local EncryptedName<sizeof(offButtonPressedSource), 0x59278F63D79F3DA1ULL> offButtonPressed(offButtonPressedSource);
  const std::uint8_t offButtonDisabledSource[] = {0x77, 0xA8, 0x97, 0x37, 0xEB, 0x29, 0xEE, 0x4C, 0x71, 0xE3, 0x97, 0x01, 0x96, 0x5D, 0x77, 0xF3, 0xD6, 0x63, 0x3E, 0xA2, 0x49, 0x93, 0x45, 0xDE, 0xD8, 0x61, 0x31, 0xE3, 0x49, 0x8A, 0x5B, 0x81};
  thread_local EncryptedName<sizeof(offButtonDisabledSource), 0x812BE72BCD5707B7ULL> offButtonDisabled(offButtonDisabledSource);
  this->m_offButton->SetImages(std::string(offButtonNormal.Text()), std::string(offButtonHover.Text()), std::string(offButtonPressed.Text()), std::string(offButtonDisabled.Text()));
  this->m_offButton->checked = false;
  this->m_offButton->Invalidate();
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText(*this->m_context);
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(287, 12);
  const std::uint8_t offLabelNameSource[] = {0xCA, 0x0B, 0x8F, 0xF2, 0x61, 0xB4, 0x55, 0x6B, 0xF9, 0x1C, 0xA2, 0x5F};
  thread_local EncryptedName<sizeof(offLabelNameSource), 0xD3755DD95F47A909ULL> offLabelName(offLabelNameSource);
  const char* offLabelText = offLabelName.Text();
  this->m_offText->SetText(offLabelText, std::strlen(offLabelText));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  const std::uint8_t offLabelFontSource[] = {0x2A, 0x96, 0x29, 0xCA, 0x80, 0xC1};
  thread_local EncryptedName<sizeof(offLabelFontSource), 0x31D7C1EDA345E36DULL> offLabelFont(offLabelFontSource);
  this->m_offText->SetFont(std::string(offLabelFont.Text()));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A27D00
// FUNCTION: fn_15397
void GameSettingsUI::CUIGroupLogInOut::Slot15(int width, int height) {
  this->m_title = new CUIStaticText(*this->m_context);
  this->m_title->SetSize(100, 12);
  this->m_title->Move(19, 12);
  const std::uint8_t titleNameSource[] = {0xC8, 0x1B, 0x1E, 0x8C, 0xAB, 0x7F, 0x35, 0xB1, 0xB4, 0x6D, 0x1D, 0x5B};
  thread_local EncryptedName<sizeof(titleNameSource), 0x0F15B16B5BAFD57FULL> titleName(titleNameSource);
  const char* titleText = titleName.Text();
  this->m_title->SetText(titleText, std::strlen(titleText));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  const std::uint8_t titleFontSource[] = {0x64, 0x84, 0xBB, 0x7E, 0xBA, 0x49};
  thread_local EncryptedName<sizeof(titleFontSource), 0x7DE349D717D7F123ULL> titleFont(titleFontSource);
  this->m_title->SetFont(std::string(titleFont.Text()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton(*this->m_context);
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(110, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  this->m_onButton->disabled = 0;
  this->m_onButton->Invalidate();
  const std::uint8_t onButtonNormalSource[] = {0x21, 0x36, 0x47, 0xB5, 0x41, 0x0D, 0xE4, 0xBE, 0x27, 0x7D, 0x47, 0x83, 0x3C, 0x79, 0x7D, 0x01, 0x80, 0xFD, 0xEE, 0x20, 0xE3, 0xB7, 0x4F, 0x2C, 0x8E, 0xFF, 0xE1, 0x61, 0xE3, 0xAE, 0x51, 0x73};
  thread_local EncryptedName<sizeof(onButtonNormalSource), 0x7321C3814F8799E1ULL> onButtonNormal(onButtonNormalSource);
  const std::uint8_t onButtonHoverSource[] = {0x8F, 0x0E, 0x0F, 0xCF, 0xA3, 0x19, 0x84, 0xB4, 0x89, 0x45, 0x0F, 0xF9, 0xDE, 0x6D, 0x1D, 0x0B, 0x2E, 0xC5, 0xA6, 0x5A, 0x01, 0xA3, 0x2F, 0x26, 0x20, 0xCF, 0xE1, 0x57, 0x0E, 0xA7, 0x41};
  thread_local EncryptedName<sizeof(onButtonHoverSource), 0x7941D76335CFA14FULL> onButtonHover(onButtonHoverSource);
  const std::uint8_t onButtonPressedSource[] = {0x0D, 0xAC, 0x0D, 0x81, 0x57, 0xF5, 0xD6, 0xFE, 0x0B, 0xE7, 0x0D, 0xB7, 0x2A, 0x81, 0x4F, 0x41, 0xAC, 0x67, 0xA4, 0x14, 0xF5, 0x4F, 0x7D, 0x6C, 0xA2, 0x65, 0xAB, 0x55, 0xF5, 0x56, 0x63, 0x33};
  thread_local EncryptedName<sizeof(onButtonPressedSource), 0x33133B977BCD03CDULL> onButtonPressed(onButtonPressedSource);
  const std::uint8_t onButtonDisabledSource[] = {0x11, 0xF4, 0x57, 0x1F, 0xF9, 0x49, 0x2E, 0xE2, 0x17, 0xBF, 0x57, 0x29, 0x84, 0x3D, 0xB7, 0x5D, 0xB0, 0x3F, 0xFE, 0x8A, 0x5B, 0xF3, 0x85, 0x70, 0xBE, 0x3D, 0xF1, 0xCB, 0x5B, 0xEA, 0x9B, 0x2F};
  thread_local EncryptedName<sizeof(onButtonDisabledSource), 0x2FEB8739E5975BD1ULL> onButtonDisabled(onButtonDisabledSource);
  this->m_onButton->SetImages(std::string(onButtonNormal.Text()), std::string(onButtonHover.Text()), std::string(onButtonPressed.Text()), std::string(onButtonDisabled.Text()));
  this->m_onButton->checked = false;
  this->m_onButton->Invalidate();
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText(*this->m_context);
  this->m_onText->SetSize(100, 12);
  this->m_onText->Move(127, 12);
  const std::uint8_t onLabelNameSource[] = {0xB1, 0xEA, 0xB2, 0xCD, 0x13};
  thread_local EncryptedName<sizeof(onLabelNameSource), 0x6BA1C1132F033B75ULL> onLabelName(onLabelNameSource);
  const char* onLabelText = onLabelName.Text();
  this->m_onText->SetText(onLabelText, std::strlen(onLabelText));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  const std::uint8_t onLabelFontSource[] = {0x68, 0xA2, 0x29, 0x4C, 0x54, 0xAB};
  thread_local EncryptedName<sizeof(onLabelFontSource), 0xA12DAB392545D72FULL> onLabelFont(onLabelFontSource);
  this->m_onText->SetFont(std::string(onLabelFont.Text()));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton(*this->m_context);
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(205, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  this->m_offButton->disabled = 0;
  this->m_offButton->Invalidate();
  const std::uint8_t offButtonNormalSource[] = {0xDD, 0x68, 0x8B, 0x4F, 0x1D, 0xB9, 0x8C, 0xEE, 0xDB, 0x23, 0x8B, 0x79, 0x60, 0xCD, 0x15, 0x51, 0x7C, 0xA3, 0x22, 0xDA, 0xBF, 0x03, 0x27, 0x7C, 0x72, 0xA1, 0x2D, 0x9B, 0xBF, 0x1A, 0x39, 0x23};
  thread_local EncryptedName<sizeof(offButtonNormalSource), 0x234977DDB54BC71DULL> offButtonNormal(offButtonNormalSource);
  const std::uint8_t offButtonHoverSource[] = {0x05, 0x82, 0xEB, 0xC5, 0x49, 0xD3, 0xA8, 0x22, 0x03, 0xC9, 0xEB, 0xF3, 0x34, 0xA7, 0x31, 0x9D, 0xA4, 0x49, 0x42, 0x50, 0xEB, 0x69, 0x03, 0xB0, 0xAA, 0x43, 0x05, 0x5D, 0xE4, 0x6D, 0x6D};
  thread_local EncryptedName<sizeof(offButtonHoverSource), 0xEF6D1D893F2B2DC5ULL> offButtonHover(offButtonHoverSource);
  const std::uint8_t offButtonPressedSource[] = {0x9B, 0xFC, 0x9F, 0x27, 0x47, 0x51, 0x66, 0xF4, 0x9D, 0xB7, 0x9F, 0x11, 0x3A, 0x25, 0xFF, 0x4B, 0x3A, 0x37, 0x36, 0xB2, 0xE5, 0xEB, 0xCD, 0x66, 0x34, 0x35, 0x39, 0xF3, 0xE5, 0xF2, 0xD3, 0x39};
  thread_local EncryptedName<sizeof(offButtonPressedSource), 0x39A39F87DD5F535BULL> offButtonPressed(offButtonPressedSource);
  const std::uint8_t offButtonDisabledSource[] = {0xB7, 0x38, 0x47, 0xE7, 0xE3, 0x9D, 0x24, 0xA2, 0xB1, 0x73, 0x47, 0xD1, 0x9E, 0xE9, 0xBD, 0x1D, 0x16, 0xF3, 0xEE, 0x72, 0x41, 0x27, 0x8F, 0x30, 0x18, 0xF1, 0xE1, 0x33, 0x41, 0x3E, 0x91, 0x6F};
  thread_local EncryptedName<sizeof(offButtonDisabledSource), 0x6FE153231D879777ULL> offButtonDisabled(offButtonDisabledSource);
  this->m_offButton->SetImages(std::string(offButtonNormal.Text()), std::string(offButtonHover.Text()), std::string(offButtonPressed.Text()), std::string(offButtonDisabled.Text()));
  this->m_offButton->checked = false;
  this->m_offButton->Invalidate();
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText(*this->m_context);
  this->m_offText->SetSize(100, 12);
  this->m_offText->Move(222, 12);
  const std::uint8_t offLabelNameSource[] = {0x05, 0xC5, 0x5E, 0x21, 0x03};
  thread_local EncryptedName<sizeof(offLabelNameSource), 0x4F99DF03C3EF31B7ULL> offLabelName(offLabelNameSource);
  const char* offLabelText = offLabelName.Text();
  this->m_offText->SetText(offLabelText, std::strlen(offLabelText));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  const std::uint8_t offLabelFontSource[] = {0xB0, 0xBC, 0x0F, 0x00, 0x10, 0x99};
  thread_local EncryptedName<sizeof(offLabelFontSource), 0x9171997D6963C9F7ULL> offLabelFont(offLabelFontSource);
  this->m_offText->SetFont(std::string(offLabelFont.Text()));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A29180

// OFFSET: 0x00A2AEC0

// OFFSET: 0x00A2CF80

// OFFSET: 0x00A2F880

// OFFSET: 0x00A30490

// OFFSET: 0x00A31070
