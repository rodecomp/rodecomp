#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skin/SkinMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Device/DisplayMode.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/Device/DeviceSelection.h"
#include "Render/Renderer.h"
#include "Render/Textures/TextureCache.h"
#include "Render/Device/RenderAdapter.h"
#include "Render/Device/RenderBackends.h"
#include "Resource/ResourcePreload.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Events/UIRoulletteWnd.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iterator>
#include <list>
#include <string>
#include <vector>

template __declspec(noinline) std::list<RenderAdapter>::~list() noexcept;
template __declspec(noinline) std::list<DisplayMode>::~list() noexcept;
template __declspec(noinline) std::list<RenderBackend>::~list() noexcept;

namespace {
  struct RodexSpamOptionPacket {
    /* 0000 */ int id;
    /* 0004 */ int option;
    /* 0008 */ int value;
  };
  static_assert(sizeof(RodexSpamOptionPacket) == 12);
}

// GLOBAL: gl_24768
int g_configuredRenderBackend;

// GLOBAL: gl_24509
int g_textureFiltering;

// GLOBAL: gl_24964
int g_textureDetailLevel;

// GLOBAL: gl_25086
std::map<int, int> g_optionHotkeys;

// GLOBAL: gl_25229
bool g_restartClient;

// OFFSET: 0x00A3D0A0
// FUNCTION: fn_15905
RenderAdapter::RenderAdapter() : backend(0), ordinal(0), drawGuid{}, renderGuid{}, deviceGuid{} {
}

template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::SelectItem(CUIListItem* item);
template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::SelectItem(CUIListItem* item);
template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::SelectItem(CUIListItem* item);
template <>
__declspec(noinline) void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::SelectItem(CUIListItem* item);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::AddNewItem();
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::AddNewItem();
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::AddNewItem();
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::AddNewItem();
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::AddNewItem();
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::Resize(int width, int height);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::Resize(int width, int height);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::Resize(int width, int height);
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::Resize(int width, int height);
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::Resize(int width, int height);

// OFFSET: 0x00A2C3C0
// FUNCTION: fn_15400
void GameSettingsUI::CUIGroupSkin::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(75, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("스킨", 0x79E915E9AF6F55DDULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB7AFBD2FD9FD7501ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_combo = new CUIComboBox<CUIComboItem, std::string>();
  this->m_combo->Create(162, 16, 5);
  this->m_combo->SetButton(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x192D654D29337BE5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0xB3E307A5F753D5E3ULL)),
                   std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x0B23D10F2F49D72BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x136F2F9915D1DBCFULL)));
  this->m_combo->Move(110, 9);
  this->m_combo->SetOnSelect([this](CUIComboItem* item, const std::string& name) { this->OnSelect(item, name); });
  this->m_combo->SetItemBinder([this](CUIComboItem* item, const std::string& name) {
    this->BindItem(item, name);
  });
  this->AddChild(this->m_combo);
}

// OFFSET: 0x00A344D0
// FUNCTION: fn_15407
// OFFSET: 0x00A34500
// FUNCTION: fn_15408
// OFFSET: 0x00A34530
// FUNCTION: fn_15409
// OFFSET: 0x00A34560
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
// FUNCTION: fn_15962
// OFFSET: 0x00A40050
// FUNCTION: fn_15963
// OFFSET: 0x00A40060
// FUNCTION: fn_15964
// OFFSET: 0x00A40070
// FUNCTION: fn_15965
template <class T, class D> unsigned int CUIComboBox<T, D>::Selected() const {
  return this->listBox->selected;
}

template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Selected() const;
template unsigned int CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Selected() const;

// OFFSET: 0x00A41FE0
// FUNCTION: fn_16009
// OFFSET: 0x00A42060
// FUNCTION: fn_16010
// OFFSET: 0x00A420E0
// FUNCTION: fn_16011
// OFFSET: 0x00A42160
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

// OFFSET: 0x00A1C1B0
// FUNCTION: fn_15379
// OFFSET: 0x00A1CC90
// FUNCTION: fn_15380
// OFFSET: 0x00A1D770
// FUNCTION: fn_15381
// OFFSET: 0x00A1E250
// FUNCTION: fn_15382
template <class T, class D> void CUIComboBox<T, D>::Create(int width, int itemHeight, int visibleItems) {
  this->visibleItems = visibleItems;
  this->width = width;
  this->itemHeight = itemHeight;
  this->SetSize(width + 2, itemHeight + 2);

  this->display = new T();
  this->display->SetSize(this->width, this->itemHeight);
  this->display->Move(1, 1);
  this->AddChild(this->display);

  this->button = new CUIButton();
  this->button->SetSize(this->display->geometry.width, this->display->geometry.height);
  this->button->Move(this->display->geometry.x, this->display->geometry.y);
  this->button->SetOnClick([this]() {
    if (!g_windowMgr.CloseDropDown()) {
      int x = 0;
      int y = this->geometry.height - 1;
      this->ToScreen(x, y);
      g_windowMgr.ShowDropDown(this, this->listBox, x, y);
    }
  });
  this->AddChild(this->button);

  CUIListBoxEx<T, D>* listBox = new CUIListBoxEx<T, D>();
  this->listBox = listBox;
  const int listWidth = this->width;
  const int listRowHeight = this->itemHeight;
  listBox->itemWidth = listWidth;
  listBox->rowHeight = listRowHeight;
  listBox->SetSize(listWidth + 13, listRowHeight);
  this->listBox->Move(1, 1);
  this->listBox->SetOnSelect([this](T* item, const D& value) {
    g_windowMgr.CloseDropDown();
    if (this->onChange) {
      this->onChange(this->display, value);
    }
    if (this->onSelect) {
      this->onSelect(item, value);
    }
  });

  this->listBox->scrollBar->SetSize(13, this->itemHeight);
  this->listBox->scrollBar->Move(this->width, 0);
  this->listBox->scrollBar->step = this->itemHeight;

  this->listBox->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0up.bmp", 0xC5B1BBCD9BC9C997ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0down.bmp", 0xC1BB8111EDD183E1ULL)));

  this->listBox->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0mid.bmp", 0x1767DDE18D779D8BULL)));

  this->listBox->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_up.bmp", 0xE33F2FF779878559ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_mid.bmp", 0xF15F8785CDE369BFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_down.bmp", 0x792FFD1325418FB5ULL)));

  this->listBox->AddNewItem();
}
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderAdapter>::Create(int width, int itemHeight, int visibleItems);
template void CUIComboBox<GameSettingsUI::CUIComboItem, DisplayMode>::Create(int width, int itemHeight, int visibleItems);
template void CUIComboBox<GameSettingsUI::CUIComboItem, RenderBackend>::Create(int width, int itemHeight, int visibleItems);
template void CUIComboBox<GameSettingsUI::CUIComboItem, std::string>::Create(int width, int itemHeight, int visibleItems);

// INLINED
template <class T, class D>
void CUIComboBox<T, D>::SetItemBinder(std::function<void(T*, const D&)> binder) {
  this->onChange = binder;
  this->listBox->SetBinder(binder);
}

// INLINED
template <class T, class D>
void CUIComboBox<T, D>::SetButton(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& disabled) {
  if (this->dropDownButton != nullptr) {
    this->RemoveChild(this->dropDownButton, 1);
    this->dropDownButton = nullptr;
    this->Resize(this->geometry.width - 18, this->geometry.height);
  }
  if (this->dropDownButton == nullptr) {
    this->Resize(this->geometry.width + 18, this->geometry.height);
    this->dropDownButton = new CUIImageButton();
    this->dropDownButton->SetSize(18, 18);
    this->dropDownButton->Move(this->geometry.width - 18, 0);
    this->dropDownButton->SetImages(normal, hover, pressed, disabled);
    this->dropDownButton->SetOnClick([this]() {
      if (!g_windowMgr.CloseDropDown()) {
        int x = 0;
        int y = this->geometry.height - 1;
        this->ToScreen(x, y);
        g_windowMgr.ShowDropDown(this, this->listBox, x, y);
      }
    });
    this->AddChild(this->dropDownButton);
  }
}

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
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A34620
// FUNCTION: fn_15417
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A34660
// FUNCTION: fn_15418
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A346A0
// FUNCTION: fn_15419
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A346E0
// FUNCTION: fn_15420
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A353D0
// FUNCTION: fn_15426
void CUICheckBox::SetOnCheck(std::function<void(bool)> callback) {
  this->onCheck = callback;
}

// OFFSET: 0x00A354A0
// FUNCTION: fn_15427
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A356B0
// FUNCTION: fn_15428
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A358C0
// FUNCTION: fn_15429
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A35AD0
// FUNCTION: fn_15430
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (GameSettingsUI::CUIComboItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A35CE0
// FUNCTION: fn_15431
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (GameSettingsUI::CUIOptionItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A35EF0
// FUNCTION: fn_15432
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A35F70
// FUNCTION: fn_15433
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A35FF0
// FUNCTION: fn_15434
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A36070
// FUNCTION: fn_15435
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A360F0
// FUNCTION: fn_15436
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A348B0
// FUNCTION: fn_15421
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (GameSettingsUI::CUIComboItem*& slot : this->items) {
      GameSettingsUI::CUIComboItem* item = new GameSettingsUI::CUIComboItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (GameSettingsUI::CUIComboItem* other : this->items) {
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

// OFFSET: 0x00A34AE0
// FUNCTION: fn_15422
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (GameSettingsUI::CUIComboItem*& slot : this->items) {
      GameSettingsUI::CUIComboItem* item = new GameSettingsUI::CUIComboItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (GameSettingsUI::CUIComboItem* other : this->items) {
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

// OFFSET: 0x00A34D10
// FUNCTION: fn_15423
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (GameSettingsUI::CUIComboItem*& slot : this->items) {
      GameSettingsUI::CUIComboItem* item = new GameSettingsUI::CUIComboItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (GameSettingsUI::CUIComboItem* other : this->items) {
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

// OFFSET: 0x00A34F40
// FUNCTION: fn_15424
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (GameSettingsUI::CUIComboItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (GameSettingsUI::CUIComboItem*& slot : this->items) {
      GameSettingsUI::CUIComboItem* item = new GameSettingsUI::CUIComboItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (GameSettingsUI::CUIComboItem* other : this->items) {
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

// INLINED
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (GameSettingsUI::CUIOptionItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  GameSettingsUI::CUIOptionItem* selectedItem = dynamic_cast<GameSettingsUI::CUIOptionItem*>(item);
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

// OFFSET: 0x00A35170
// FUNCTION: fn_15425
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (GameSettingsUI::CUIOptionItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (GameSettingsUI::CUIOptionItem*& slot : this->items) {
      GameSettingsUI::CUIOptionItem* item = new GameSettingsUI::CUIOptionItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (GameSettingsUI::CUIOptionItem* other : this->items) {
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

// OFFSET: 0x00A3C440
// FUNCTION: fn_15887
__declspec(noinline) GameSettingsUI::CUIComboItem::CUIComboItem() : CUIListItem(), m_text(nullptr) {
}

// OFFSET: 0x00A3CF80
// FUNCTION: fn_15903
__declspec(noinline) GameSettingsUI::CUIOptionItem::CUIOptionItem()
    : CUIListItem(), m_check(nullptr), m_runButton(nullptr), m_label(nullptr), m_description(nullptr), m_optionId(300) {
}

// OFFSET: 0x00A36170
// FUNCTION: fn_15437
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::OnScroll(int position) {
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A36370
// FUNCTION: fn_15438
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::OnScroll(int position) {
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A36570
// FUNCTION: fn_15439
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::OnScroll(int position) {
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A36770
// FUNCTION: fn_15440
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::OnScroll(int position) {
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A36970
// FUNCTION: fn_15441
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::OnScroll(int position) {
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

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A1F210
// FUNCTION: fn_15384
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F2E0
// FUNCTION: fn_15385
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F3B0
// FUNCTION: fn_15386
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F480
// FUNCTION: fn_15387
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A1F550
// FUNCTION: fn_15388
template <>
void CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A36CF0
// FUNCTION: fn_15442
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    int count = static_cast<int>(this->selected);
    if (count > 0) {
      do {
        ++node;
        --count;
      } while (count > 0);
    }
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A36DD0
// FUNCTION: fn_15443
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    int count = static_cast<int>(this->selected);
    if (count > 0) {
      do {
        ++node;
        --count;
      } while (count > 0);
    }
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A36EB0
// FUNCTION: fn_15444
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    int count = static_cast<int>(this->selected);
    if (count > 0) {
      do {
        ++node;
        --count;
      } while (count > 0);
    }
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A36F90
// FUNCTION: fn_15445
template <>
void CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (GameSettingsUI::CUIComboItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  GameSettingsUI::CUIComboItem* selectedItem = dynamic_cast<GameSettingsUI::CUIComboItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    int count = static_cast<int>(this->selected);
    if (count > 0) {
      do {
        ++node;
        --count;
      } while (count > 0);
    }
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
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ADDSKILL_FAIL_INCLUDE_SKILL_YOU_CANNOT_LEARN)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A40480
// FUNCTION: fn_15972
void CUIGroupLogInOut::Apply() {
  g_serviceOptions.Set(0xA5, 1);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LOGINOUT_ON)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A404F0
// FUNCTION: fn_15974
void CUIGroupRodexSpam::Apply() {
  RodexSpamOptionPacket packet = {0x0B93, 1, 1};
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(static_cast<std::int16_t>(packet.id)), &packet);
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
std::string CUIListPage::GetHotkeyText(int tab, int index) {
  int key1 = 0;
  int key2 = 0;
  std::string hotkeyText;
  std::string hotkeyDescription;
  const char* keyText = nullptr;
  const char* description = nullptr;
  g_session.m_hotkeyScript->Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description);
  if (IsNullOrEmpty(keyText)) {
    g_session.m_hotkeyScript->Call("GetOriginalHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description);
  }

  if (IsNullOrEmpty(keyText)) {
    return "";
  }
  return keyText;
}

// OFFSET: 0x00A40C30
// FUNCTION: fn_15985
void CUIComboItem::OnInit() {
}

// OFFSET: 0x00A41D80
// FUNCTION: fn_15999
void CUIListPage::OnInit() {
}

// OFFSET: 0x00A41D90
// FUNCTION: fn_16000
void CUIOptionItem::OnInit() {
}

// OFFSET: 0x00A41DA0
// FUNCTION: fn_16001
void CUIPage::OnInit() {
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
  this->m_bgmSlider->SetTransparent(!enabled);
  CUIVScrollBar* const slider = this->m_bgmSlider;
  slider->dragging = !enabled;
  slider->Invalidate();
  g_serviceOptions.Set(7, enabled);
  if (previous != enabled) {
    g_modeMgr.gameMode->SendMsg(MM_REFRESH_MUSIC, 0, 0, 0, 0);
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
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(enabled ? 0xDC7 : 0xDC8)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00A3F5C0
// FUNCTION: fn_15946
void CUIGroupLogInOut::SetEnabled(bool enabled) {
  g_serviceOptions.Set(0xA5, enabled);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(enabled ? 0x414 : 0x415)), 0x64FFFF, 0, 0, 0);
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
  CUIVScrollBar* const slider = this->m_effectSlider;
  slider->dragging = muted;
  slider->Invalidate();
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
  if (this->m_fullscreen->m_onButton->checked != static_cast<bool>(g_configuredFullscreen)) {
    return true;
  }
  return false;
}

// OFFSET: 0x00A40270
// FUNCTION: fn_15968
void CUIListPage::OnSelectOption(CUIOptionItem* item, const TALKTYPE& type) {
}

// OFFSET: 0x00A41E30
// FUNCTION: fn_16003
void CUIGroupEmblem::OnSelect(CUIRadioButton* button) {
  this->SetEnabled(this->m_onButton == button);
}

// OFFSET: 0x00A41E90
// FUNCTION: fn_16005
void CUIGroupLogInOut::OnSelect(CUIRadioButton* button) {
  this->SetEnabled(this->m_onButton == button);
}

// OFFSET: 0x00A41EB0
// FUNCTION: fn_16006
void CUIGroupProcessPriority::OnSelect(CUIRadioButton* button) {
  if (this->m_highButton == button) {
    g_session.m_priorityClass = HIGH_PRIORITY_CLASS;
    SetPriorityClass(GetCurrentProcess(), g_session.m_priorityClass);
  } else if (this->m_normalButton == button) {
    g_session.m_priorityClass = NORMAL_PRIORITY_CLASS;
    SetPriorityClass(GetCurrentProcess(), g_session.m_priorityClass);
  } else if (this->m_idleButton == button) {
    g_session.m_priorityClass = IDLE_PRIORITY_CLASS;
    SetPriorityClass(GetCurrentProcess(), g_session.m_priorityClass);
  }
}

// OFFSET: 0x00A41F30
// FUNCTION: fn_16007
void CUIGroupRodexSpam::OnSelect(CUIRadioButton* button) {
  RodexSpamOptionPacket packet = {0x0B93, 1, this->m_offButton != button};
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(static_cast<std::int16_t>(packet.id)), &packet);
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
  auto* const onButton = this->m_onButton;
  onButton->checked = g_configuredFullscreen != 0;
  onButton->Invalidate();
  auto* const offButton = this->m_offButton;
  offButton->checked = g_configuredFullscreen == 0;
  offButton->Invalidate();
}

// OFFSET: 0x00A43700
// FUNCTION: fn_16036
void CUIGroupLogInOut::Refresh() {
  auto* const onButton = this->m_onButton;
  onButton->checked = g_serviceOptions.Get(0xA5);
  onButton->Invalidate();
  auto* const offButton = this->m_offButton;
  offButton->checked = !g_serviceOptions.Get(0xA5);
  offButton->Invalidate();
}

// OFFSET: 0x00A43760
// FUNCTION: fn_16037
void CUIGroupProcessPriority::Refresh() {
  auto* const highButton = this->m_highButton;
  highButton->checked = g_session.m_priorityClass == HIGH_PRIORITY_CLASS;
  highButton->Invalidate();
  auto* const normalButton = this->m_normalButton;
  normalButton->checked = g_session.m_priorityClass == NORMAL_PRIORITY_CLASS;
  normalButton->Invalidate();
  auto* const idleButton = this->m_idleButton;
  idleButton->checked = g_session.m_priorityClass == IDLE_PRIORITY_CLASS;
  idleButton->Invalidate();
}

// OFFSET: 0x00A437D0
// FUNCTION: fn_16038
void CUIGroupRodexSpam::Refresh() {
  auto* const onButton = this->m_onButton;
  onButton->checked = g_gameOptionFlag;
  onButton->Invalidate();
  auto* const offButton = this->m_offButton;
  offButton->checked = !g_gameOptionFlag;
  offButton->Invalidate();
}

// OFFSET: 0x00A43810
// FUNCTION: fn_16039
void CUIGroupSkin::Refresh() {
  CUIListBoxEx<CUIComboItem, std::string>* list = this->m_combo->listBox;
  const int index = g_skinInfo.selected + 1;
  list->scrollBar->SetPosition(list->rowHeight * index);
  for (CUIComboItem* item : list->items) {
    if (index == item->index) {
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
  CUIVScrollBar* const bgmSlider = this->m_bgmSlider;
  bgmSlider->dragging = bgmMuted;
  bgmSlider->Invalidate();
  CUICheckBox* const bgmMute = this->m_bgmMute;
  bgmMute->checked = bgmMuted;
  bgmMute->Invalidate();

  this->m_effectSlider->SetTotal(135);
  this->m_effectSlider->SetVisible(this->m_effectSlider->PositionFromOffset(8));
  this->m_effectSlider->SetPosition(g_soundMgr->sampleVolume);
  this->m_effectSlider->SetThumbTooltip(std::to_string(g_soundMgr->sampleVolume));
  const bool effectMuted = !g_serviceOptions.Get(0xB);
  this->m_effectSlider->SetTransparent(effectMuted);
  CUIVScrollBar* const effectSlider = this->m_effectSlider;
  effectSlider->dragging = effectMuted;
  effectSlider->Invalidate();
  CUICheckBox* const effectMute = this->m_effectMute;
  effectMute->checked = effectMuted;
  effectMute->Invalidate();
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
  auto* const onButton = this->m_onButton;
  onButton->checked = g_textureFiltering != 0;
  onButton->Invalidate();
  auto* const offButton = this->m_offButton;
  offButton->checked = g_textureFiltering == 0;
  offButton->Invalidate();
}

// OFFSET: 0x00A43B70
// FUNCTION: fn_16045
void CUIPage::Refresh() {
  for (UIWindow* child : this->m_children) {
    static_cast<CUIGroup*>(child)->Refresh();
  }
}
}

// OFFSET: 0x00A3C4A0
// FUNCTION: fn_15888
CUIGameSettingsUI::CUIGameSettingsUI()
    : CUIFrameWnd(), m_titleBar(nullptr), m_currentPage(nullptr), m_graphicsPage(nullptr), m_pages(), m_tab(nullptr), m_pageContainer(nullptr),
      m_applyButton(nullptr), m_closeButton(nullptr) {
}

// OFFSET: 0x00A1EE20
// FUNCTION: fn_15383
void CUIGameSettingsUI::Initialize() {
  this->m_ownsBackgroundColor = true;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = true;
  this->Resize(400, 350);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
  this->SetSkinColors(0, 0, 3, 1);
  this->m_buttonBarStyle = 3;
}

// OFFSET: 0x00A41DE0
// FUNCTION: fn_16002
void CUIGameSettingsUI::OnLBtnDown(int x, int y) {
  if (this->m_titleBar->IsPointInside(x, y)) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x00A3F0F0
// FUNCTION: fn_15939
void CUIGameSettingsUI::OnTitleIconClick() {
}

// OFFSET: 0x00A3F100
// FUNCTION: fn_45480
void CUIGameSettingsUI::OnApply() {
  const bool modified = this->m_graphicsPage->IsModified();
  if (g_windowMgr.Confirm(LookupMessage(modified ? 3168 : 3167), 2, true, false, 0, "Message", 280, 120, 0) != 188) {
    this->m_graphicsPage->ApplySettings();
    this->UpdateApplyButton();
  }
}

// OFFSET: 0x00A3F170
// FUNCTION: fn_45481
void CUIGameSettingsUI::OnCloseButton() {
  g_windowMgr.CloseWindow(WID_GAMESETTINGSUI);
}

// OFFSET: 0x00A3F940
// FUNCTION: fn_45482
void CUIGameSettingsUI::OnTitleClose() {
  g_windowMgr.CloseWindow(WID_GAMESETTINGSUI);
}

// OFFSET: 0x00A40240
// FUNCTION: fn_15967
__declspec(noinline) bool CUIGameSettingsUI::IsTab4Selected() {
  const int id = this->m_tab->current ? this->m_tab->current->geometry.id : -1;
  return id == 4;
}

// OFFSET: 0x00A43380
// FUNCTION: fn_16024
void CUIGameSettingsUI::OnBasicTabClick() {
}

// OFFSET: 0x00A43390
// FUNCTION: fn_16025
void CUIGameSettingsUI::OnControlTabClick() {
}

// OFFSET: 0x00A433A0
// FUNCTION: fn_16026
void CUIGameSettingsUI::OnEffectTabClick() {
}

// OFFSET: 0x00A433B0
// FUNCTION: fn_16027
void CUIGameSettingsUI::OnEtcTabClick() {
}

// OFFSET: 0x00A433C0
// FUNCTION: fn_16028
void CUIGameSettingsUI::OnGraphicsTabClick() {
}

// OFFSET: 0x00A42FC0
// FUNCTION: fn_16019
void GameSettingsUI::CUIGroupGraphicsAPI::Apply() {
  this->m_backend = this->m_pendingBackend;
  g_configuredRenderBackend = this->m_pendingBackend;
}

// OFFSET: 0x00A404D0
// FUNCTION: fn_15973
void SetNormalProcessPriority() {
  g_session.m_priorityClass = NORMAL_PRIORITY_CLASS;
  HANDLE process = GetCurrentProcess();
  SetPriorityClass(process, NORMAL_PRIORITY_CLASS);
}

// OFFSET: 0x00A421E0
// FUNCTION: fn_16013
void CUIGameSettingsUI::UpdateApplyButton() {
  GameSettingsUI::CUIGraphicsPage* page = this->m_graphicsPage;
  const bool visible = this->m_tab->current && this->m_tab->current->geometry.id == 3 &&
                       (page->IsModified() || page->m_spriteDetailLevel->m_slider->position != g_effectDetailLevel ||
                        page->m_textureDetailLevel->m_slider->position != g_textureDetailLevel ||
                        page->m_trilinear->m_onButton->checked != static_cast<bool>(g_textureFiltering));
  this->m_applyButton->Show(visible);
}

// OFFSET: 0x00A41E10
// FUNCTION: fn_45466
__declspec(noinline) void CUIGameSettingsUI::SelectTab4() {
  this->m_tab->Select(static_cast<CUITabButton*>(this->m_tab->FindChildById(4)));
}

// OFFSET: 0x00A41E50
// FUNCTION: fn_16004
void __stdcall OnFullscreenOptionChanged(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(WID_GAMESETTINGSUI));
  if (wnd) {
    wnd->UpdateApplyButton();
  }
}

// OFFSET: 0x00A41FA0
// FUNCTION: fn_16008
void __stdcall OnTrilinearOptionChanged(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(WID_GAMESETTINGSUI));
  if (wnd) {
    wnd->UpdateApplyButton();
  }
}

// OFFSET: 0x00A43000
// FUNCTION: fn_16022
void __stdcall OnSpriteDetailLevelChanged(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(WID_GAMESETTINGSUI));
  if (wnd) {
    wnd->UpdateApplyButton();
  }
}

// OFFSET: 0x00A43040
// FUNCTION: fn_16023
void __stdcall OnTextureDetailLevelChanged(int option) {
  CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(WID_GAMESETTINGSUI));
  if (wnd) {
    wnd->UpdateApplyButton();
  }
}

// OFFSET: 0x00A433D0
// FUNCTION: fn_16029
void CUIGameSettingsUI::OnTabChanged(UIWindow* window) {
  this->m_pageContainer->Show(window != this->m_tab->FindChildById(3));
  this->UpdateApplyButton();
}

// OFFSET: 0x00A3F9B0
// FUNCTION: fn_15952
void GameSettingsUI::CUIGroupGraphics::BindAdapterItem(CUIComboItem* item, const RenderAdapter& adapter) {
  item->m_text->SetText(adapter.description);
}

// OFFSET: 0x00A3FA90
// FUNCTION: fn_15954
void GameSettingsUI::CUIGroupGraphics::BindModeItem(CUIComboItem* item, const DisplayMode& mode) {
  item->m_text->SetText(mode.description);
}

// OFFSET: 0x00A3FB20
// FUNCTION: fn_15956
void GameSettingsUI::CUIGroupGraphicsAPI::BindItem(CUIComboItem* item, const RenderBackend& backend) {
  item->m_text->SetText(backend.name);
}

// OFFSET: 0x00A3FBA0
// FUNCTION: fn_15958
void GameSettingsUI::CUIGroupSkin::BindItem(CUIComboItem* item, const std::string& name) {
  item->m_text->SetText(name.c_str(), name.size());
}

// OFFSET: 0x00A41C60
// FUNCTION: fn_15996
void GameSettingsUI::CUIGroupSpriteDetailLevel::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_SPRITE_RESOLUTION));
}

// OFFSET: 0x00A41CA0
// FUNCTION: fn_15997
void GameSettingsUI::CUIGroupTextureDetailLevel::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_TEXTURE_RESOLUTION));
}

// OFFSET: 0x00AA2760
// FUNCTION: fn_17216
void ToggleGameSettingsTab4() {
  UIWindow* window = g_windowMgr.LookupWindow(WID_GAMESETTINGSUI);
  if (!window) {
    window = g_windowMgr.EnsureWindow(WID_GAMESETTINGSUI);
  }

  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(window);
  if (settings->IsTab4Selected()) {
    g_windowMgr.CloseWindow(WID_GAMESETTINGSUI);
    return;
  }

  settings->SelectTab4();
}

// OFFSET: 0x00AAB070
// FUNCTION: fn_17277
void UIWindowMgr::RefreshGameSettingOption(int option) {
  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(this->LookupWindow(WID_GAMESETTINGSUI));
  if (settings) {
    settings->RefreshOption(option);
  }
}

// OFFSET: 0x00AAB090
// FUNCTION: fn_17278
void UIWindowMgr::RefreshGameSettings() {
  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(this->LookupWindow(WID_GAMESETTINGSUI));
  if (settings) {
    settings->RefreshOptions();
  }
}

// OFFSET: 0x00AAB0B0
// FUNCTION: fn_17279
void UIWindowMgr::RefreshGameSettingHotkeys() {
  CUIGameSettingsUI* settings = static_cast<CUIGameSettingsUI*>(this->LookupWindow(WID_GAMESETTINGSUI));
  if (settings) {
    settings->RefreshHotkeyLabels();
  }
}

// OFFSET: 0x00A1BF50
// FUNCTION: fn_15373
CUIFrameWnd* CreateGameSettingsUI() {
  return new CUIGameSettingsUI();
}

// OFFSET: 0x00494730
// FUNCTION: fn_45545
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateGameSettingsUI;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10014, factory);
  return true;
}();

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
__declspec(noinline) void CUIGameSettingsUI::RefreshOption(int id) {
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
  this->m_currentPage->CUIPage::Refresh();
}

// OFFSET: 0x00A41120
// FUNCTION: fn_15987
void GameSettingsUI::CUIGroupEmblem::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_GAME_SETTINGS_HIDE));
  this->m_onText->SetText(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY));
  this->m_offText->SetText(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY_HIGH));
}

// OFFSET: 0x00A411C0
// FUNCTION: fn_15988
void GameSettingsUI::CUIGroupFullscreen::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_MACRO_CHECKER_MAP_NOT_PROPER_MAP));
  this->m_onText->SetText(LookupMessage(MSI_TAREN_TRWSDK_FILTER_SENSITIVEWORDS_ERROR));
  this->m_offText->SetText(LookupMessage(MSI_TAREN_TRWSDK_FILTER_NO_SEND_BECAUSE_SENSITIVEWORDS_ERROR));
}

// OFFSET: 0x00A41260
// FUNCTION: fn_15989
void GameSettingsUI::CUIGroupGraphics::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_GRAPHIC_SETTING_MOUSE_SET));
  this->m_modeText->SetText(LookupMessage(MSI_STORE_ASSISTANT_TRADE_FEE));
  GetRenderSystemHolder();
  this->m_adapter = this->m_pendingAdapter = SelectedRenderAdapter();
  this->m_mode = this->m_pendingMode = DisplayMode{static_cast<unsigned>(g_configuredScreenWidth), static_cast<unsigned>(g_configuredScreenHeight), static_cast<unsigned>(g_configuredBitsPerPixel)};
}

// OFFSET: 0x00A41510
// FUNCTION: fn_15990
void GameSettingsUI::CUIGroupGraphicsAPI::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_ENCHANT_MARK_2));
  const int backend = GetRenderSystemHolder()->Backend();
  this->m_pendingBackend = backend;
  this->m_backend = backend;
}

// OFFSET: 0x00A41710
// FUNCTION: fn_15992
void GameSettingsUI::CUIGroupProcessPriority::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY_NORMAL_TOOLTIP));
  this->m_highText->SetText(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY_IDLE));
  this->m_highText->SetTooltip(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY_IDLE_TOOLTIP));
  this->m_normalText->SetText(LookupMessage(MSI_GAME_SETTINGS_AURA));
  this->m_normalText->SetTooltip(LookupMessage(MSI_GAME_SETTINGS_SIMPLE_AURA));
  this->m_idleText->SetText(LookupMessage(MSI_GAME_SETTINGS_SIMPLE_AURA_OFF));
  this->m_idleText->SetTooltip(LookupMessage(MSI_GAME_SETTINGS_SIMPLE_AURA_ON));
}

// OFFSET: 0x00A41870
// FUNCTION: fn_15993
void GameSettingsUI::CUIGroupRodexSpam::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_MOVE));
  this->m_onText->SetText(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY_NORMAL));
  this->m_offText->SetText(LookupMessage(MSI_GAME_SETTINGS_PROCESS_PRIORITY_HIGH_TOOLTIP));
}

// OFFSET: 0x00A41CE0
// FUNCTION: fn_15998
void GameSettingsUI::CUIGroupTrilinear::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_REFUSE_SDK_AUTH_FAILED));
  this->m_onText->SetText(LookupMessage(MSI_GAME_SETTINGS_RODEX_SPAM_ON));
  this->m_offText->SetText(LookupMessage(MSI_GAME_SETTINGS_RODEX_SPAM_OFF));
}

// OFFSET: 0x00A193A0
// FUNCTION: fn_15341
template <>
__declspec(noinline) CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A19560
// FUNCTION: fn_15342
template <>
__declspec(noinline) CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A19720
// FUNCTION: fn_15343
template <>
__declspec(noinline) CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A198E0
// FUNCTION: fn_15344
template <>
__declspec(noinline) CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A19AA0
// FUNCTION: fn_15345
template <>
__declspec(noinline) CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A41B60
// FUNCTION: fn_15995
void GameSettingsUI::CUIGroupSound::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_SOUND_OPTIONWND));
  this->m_bgmText->SetText(LookupMessage(MSI_BGM));
  this->m_bgmMuteText->SetText(LookupMessage(MSI_GAME_SETTINGS_RODEX_SPAM_OFF));
  this->m_effectText->SetText(LookupMessage(MSI_SOUND));
  this->m_effectMuteText->SetText(LookupMessage(MSI_GAME_SETTINGS_RODEX_SPAM_OFF));
}

// OFFSET: 0x00A41560
// FUNCTION: fn_15991
void GameSettingsUI::CUIGroupLogInOut::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_CAST_STAT));
  this->m_onText->SetText(LookupMessage(MSI_GAME_SETTINGS_RODEX_SPAM_ON));
  this->m_offText->SetText(LookupMessage(MSI_GAME_SETTINGS_RODEX_SPAM_OFF));
  this->m_title->SetTooltip(OBFUSCATE_SOURCE_TEXT("/loginout\x0A/li", 0x03772763F96D9DA5ULL));
}

// OFFSET: 0x00A1F890
// FUNCTION: fn_15390
void GameSettingsUI::CUIComboItem::OnCreate(int width, int height) {
  this->m_text = new CUIStaticText();
  this->m_text->SetSize(155, 16);
  this->m_text->Move(3, 0);
  this->m_text->SetText("");
  this->m_text->SetFitText(1);
  this->m_text->SetAlign(0, 6);
  this->m_text->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x49D33529F1556333ULL)));
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
    CUIGameSettingsUI* wnd = dynamic_cast<CUIGameSettingsUI*>(g_windowMgr.LookupWindow(WID_GAMESETTINGSUI));
    if (wnd) {
      wnd->UpdateApplyButton();
    }
  }
}

// OFFSET: 0x00A41910
// FUNCTION: fn_15994
void GameSettingsUI::CUIGroupSkin::OnInit() {
  this->m_title->SetText(LookupMessage(MSI_SKIN));
  this->m_combo->AddItem(std::string(LookupMessage(MSI_ACHIEVEMENT_COMMAND)), false);
  for (int index = 0; index < static_cast<int>(g_skinInfo.prefixes.size()); ++index) {
    this->m_combo->AddItem(std::string(g_skinInfo.Name(index)), false);
  }
  this->m_combo->UpdateListHeight();
}

// OFFSET: 0x00A40560
// FUNCTION: fn_15976
void GameSettingsUI::CUIGroupSound::Apply() {
  this->SetBgmVolume(g_session.GetDefaultOptionValue(OBFUSCATE_SOURCE_TEXT("Bgm_Volume", 0x75ADC57D87EFBD8FULL), 100));
  this->SetBgmEnabled(g_session.GetDefaultOptionValue(OBFUSCATE_SOURCE_TEXT("/bgm", 0xC517FB932D95BB6DULL), 1) != 0);
  this->SetEffectVolume(g_session.GetDefaultOptionValue(OBFUSCATE_SOURCE_TEXT("Effect_Volume", 0x23BBD58F8B5FF5C9ULL), 100));
  const int effect = g_session.GetDefaultOptionValue(OBFUSCATE_SOURCE_TEXT("/sound", 0x0B7109397F6B1387ULL), 1);
  const bool muted = effect == 0;
  this->m_effectSlider->SetTransparent(muted);
  this->m_effectSlider->dragging = muted;
  this->m_effectSlider->Invalidate();
  g_serviceOptions.Set(0xB, effect != 0);
}

// OFFSET: 0x00A19C60
// FUNCTION: fn_15346
inline CUIButton::CUIButton() : CUIControl(), state(0), disabled(0), tooltip(), clickWindow(nullptr), hoverWindow(nullptr) {
}

// OFFSET: 0x00A19D30
// FUNCTION: fn_15347
__declspec(noinline) CUIRadioButton::CUIRadioButton() : CUICheckBox(), onSelect() {
}

// OFFSET: 0x00A3C390
// FUNCTION: fn_15886
GameSettingsUI::CUIBasicPage::CUIBasicPage() : CUIPage(), m_skin(nullptr), m_sound(nullptr), m_emblem(nullptr), m_rodexSpam(nullptr), m_processPriority(nullptr), m_logInOut(nullptr) {
}

// OFFSET: 0x00A3C5D0
// FUNCTION: fn_15889
GameSettingsUI::CUIGraphicsPage::CUIGraphicsPage() : CUIPage(), m_graphicsApi(nullptr), m_graphics(nullptr), m_spriteDetailLevel(nullptr), m_textureDetailLevel(nullptr), m_fullscreen(nullptr), m_trilinear(nullptr) {
}

// OFFSET: 0x00A3C680
// FUNCTION: fn_15890
GameSettingsUI::CUIGroupEmblem::CUIGroupEmblem() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_onButton = nullptr;
  this->m_onText = nullptr;
  this->m_offButton = nullptr;
  this->m_offText = nullptr;
}

// OFFSET: 0x00A3C730
// FUNCTION: fn_15891
GameSettingsUI::CUIGroupFullscreen::CUIGroupFullscreen() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_onButton = nullptr;
  this->m_onText = nullptr;
  this->m_offButton = nullptr;
  this->m_offText = nullptr;
}

// OFFSET: 0x00A3C7E0
// FUNCTION: fn_15892
GameSettingsUI::CUIGroupGraphics::CUIGroupGraphics()
    : CUIGroup(), m_adapterCombo(nullptr), m_modeText(nullptr), m_modeCombo(nullptr),
      m_adapter(), m_pendingAdapter() {
}

// OFFSET: 0x00A3C910
// FUNCTION: fn_15893
GameSettingsUI::CUIGroupGraphicsAPI::CUIGroupGraphicsAPI() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_combo = nullptr;
  this->m_backend = -1;
  this->m_pendingBackend = -1;
}

// OFFSET: 0x00A3C9B0
// FUNCTION: fn_15894
GameSettingsUI::CUIGroupLogInOut::CUIGroupLogInOut() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_onButton = nullptr;
  this->m_onText = nullptr;
  this->m_offButton = nullptr;
  this->m_offText = nullptr;
}

// OFFSET: 0x00A3CA60
// FUNCTION: fn_15895
GameSettingsUI::CUIGroupProcessPriority::CUIGroupProcessPriority() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_highButton = nullptr;
  this->m_highText = nullptr;
  this->m_normalButton = nullptr;
  this->m_normalText = nullptr;
  this->m_idleButton = nullptr;
  this->m_idleText = nullptr;
}

// OFFSET: 0x00A3CB20
// FUNCTION: fn_15896
GameSettingsUI::CUIGroupRodexSpam::CUIGroupRodexSpam() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_onButton = nullptr;
  this->m_onText = nullptr;
  this->m_offButton = nullptr;
  this->m_offText = nullptr;
}

// OFFSET: 0x00A3CBD0
// FUNCTION: fn_15897
GameSettingsUI::CUIGroupSkin::CUIGroupSkin() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_combo = nullptr;
}

// OFFSET: 0x00A3CC60
// FUNCTION: fn_15898
GameSettingsUI::CUIGroupSound::CUIGroupSound() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_bgmText = nullptr;
  this->m_bgmSlider = nullptr;
  this->m_bgmMute = nullptr;
  this->m_bgmMuteText = nullptr;
  this->m_effectText = nullptr;
  this->m_effectSlider = nullptr;
  this->m_effectMute = nullptr;
  this->m_effectMuteText = nullptr;
}

// OFFSET: 0x00A3CD30
// FUNCTION: fn_15899
GameSettingsUI::CUIGroupSpriteDetailLevel::CUIGroupSpriteDetailLevel() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_slider = nullptr;
}

// OFFSET: 0x00A3CDC0
// FUNCTION: fn_15900
GameSettingsUI::CUIGroupTextureDetailLevel::CUIGroupTextureDetailLevel() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_slider = nullptr;
}

// OFFSET: 0x00A3CE50
// FUNCTION: fn_15901
GameSettingsUI::CUIGroupTrilinear::CUIGroupTrilinear() : CUIGroup(0) {
  this->m_title = nullptr;
  this->m_onButton = nullptr;
  this->m_onText = nullptr;
  this->m_offButton = nullptr;
  this->m_offText = nullptr;
}

// OFFSET: 0x00A3CF00
// FUNCTION: fn_15902
GameSettingsUI::CUIListPage::CUIListPage() : CUIControl(), m_optionList(nullptr) {
}

// OFFSET: 0x00A1F620
// FUNCTION: fn_15389
void GameSettingsUI::CUIBasicPage::OnCreate(int width, int height) {
  this->m_skin = new CUIGroupSkin();
  this->m_skin->SetSize(400, 36);
  this->m_skin->Move(0, 0);
  this->AddChild(this->m_skin);
  this->m_sound = new CUIGroupSound();
  this->m_sound->SetSize(400, 60);
  this->m_sound->Move(0, 36);
  this->AddChild(this->m_sound);
  this->m_emblem = new CUIGroupEmblem();
  this->m_emblem->SetSize(400, 36);
  this->m_emblem->Move(0, 96);
  this->AddChild(this->m_emblem);
  this->m_rodexSpam = new CUIGroupRodexSpam();
  this->m_rodexSpam->SetSize(400, 60);
  this->m_rodexSpam->Move(0, 132);
  this->AddChild(this->m_rodexSpam);
  this->m_processPriority = new CUIGroupProcessPriority();
  this->m_processPriority->SetSize(400, 36);
  this->m_processPriority->Move(0, 192);
  this->AddChild(this->m_processPriority);
  this->m_logInOut = new CUIGroupLogInOut();
  this->m_logInOut->SetSize(400, 36);
  this->m_logInOut->Move(0, 228);
  this->AddChild(this->m_logInOut);
}

// OFFSET: 0x00A32550
// FUNCTION: fn_15405
void GameSettingsUI::CUIListPage::OnCreate(int width, int height) {
  auto* const optionList = new CUIListBoxEx<CUIOptionItem, TALKTYPE>;
  this->m_optionList = optionList;
  optionList->itemWidth = 387;
  optionList->rowHeight = 26;
  optionList->SetSize(400, 286);
  this->m_optionList->Move(0, 1);
  this->m_optionList->SetOnSelect([this](CUIOptionItem* item, const TALKTYPE& type) { this->CUIListPage::OnSelectOption(item, type); });
  this->m_optionList->SetBinder([this](CUIOptionItem* item, const TALKTYPE& type) { this->BindOption(item, type); });
  this->m_optionList->SetImages("", "", "", "");
  {
    auto* const list = this->m_optionList;
    list->itemX = 0;
    list->itemY = 0;
  }

  this->m_optionList->scrollBar->SetSize(13, 286);
  this->m_optionList->scrollBar->Move(387, 0);
  this->m_optionList->scrollBar->step = 26;
  this->m_optionList->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0up.bmp", 0x114901C7E37F6D49ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0down.bmp", 0x7D5D0F110B6B8371ULL)));
  this->m_optionList->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0mid.bmp", 0xDB27CB557D038759ULL)));
  this->m_optionList->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_up.bmp", 0x3FA3AFA7035DE3FFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_mid.bmp", 0x87E3D1C50D572DEFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll0bar_down.bmp", 0x51BB39B763BBBD8DULL)));

  this->m_optionList->AddNewItem();
  this->AddChild(this->m_optionList);
}

// INLINED
inline CUIHScrollBar::CUIHScrollBar() : CUIVScrollBar() {
}

// OFFSET: 0x00A22E00
// FUNCTION: fn_15392
void GameSettingsUI::CUIGraphicsPage::OnCreate(int width, int height) {
  this->m_graphicsApi = new CUIGroupGraphicsAPI();
  this->m_graphicsApi->SetSize(400, 32);
  this->m_graphicsApi->Move(0, 0);
  this->AddChild(this->m_graphicsApi);

  this->m_graphics = new CUIGroupGraphics();
  this->m_graphics->SetSize(400, 64);
  this->m_graphics->Move(0, 32);
  this->AddChild(this->m_graphics);

  this->m_spriteDetailLevel = new CUIGroupSpriteDetailLevel();
  this->m_spriteDetailLevel->SetSize(400, 32);
  this->m_spriteDetailLevel->Move(0, 96);
  this->AddChild(this->m_spriteDetailLevel);

  this->m_textureDetailLevel = new CUIGroupTextureDetailLevel();
  this->m_textureDetailLevel->SetSize(400, 32);
  this->m_textureDetailLevel->Move(0, 128);
  this->AddChild(this->m_textureDetailLevel);

  this->m_fullscreen = new CUIGroupFullscreen();
  this->m_fullscreen->SetSize(400, 32);
  this->m_fullscreen->Move(0, 160);
  this->AddChild(this->m_fullscreen);

  this->m_trilinear = new CUIGroupTrilinear();
  this->m_trilinear->SetSize(400, 32);
  this->m_trilinear->Move(0, 192);
  this->AddChild(this->m_trilinear);
}

// OFFSET: 0x00A23070
// FUNCTION: fn_15393
void GameSettingsUI::CUIGroupEmblem::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(85, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("엠블렘 테두리", 0x4331A17F33095FEBULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xD1056FF94D4D2773ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton();
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(110, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const onButton = this->m_onButton;
    onButton->disabled = 0;
    onButton->Invalidate();
  }
  this->m_onButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x0F97E779A933D917ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x3789496BE3E1D141ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xC517FB932D95BB6DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x23BBD58F8B5FF5C9ULL)));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(50, 13);
  this->m_onText->Move(127, 12);
  this->m_onText->SetText(OBFUSCATE_SOURCE_TEXT("보이기", 0x0B7109397F6B1387ULL));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x03772763F96D9DA5ULL)));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton();
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(205, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const offButton = this->m_offButton;
    offButton->disabled = 0;
    offButton->Invalidate();
  }
  this->m_offButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x17A9E5BF39AD4737ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x13E33FA1A591AB67ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xCD8F077123FB7D6FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x9949BFD527532943ULL)));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(222, 12);
  this->m_offText->SetText(OBFUSCATE_SOURCE_TEXT("숨기기", 0xD1413DBB01CD8B51ULL));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x637B959D55771F69ULL)));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A24510
// FUNCTION: fn_15394
void GameSettingsUI::CUIGroupFullscreen::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("화면 모드", 0xC399B7091589B50BULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB739073FE921AD6FULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton();
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(150, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { OnFullscreenOptionChanged(reinterpret_cast<int>(button)); });
  {
    auto* const onButton = this->m_onButton;
    onButton->disabled = 0;
    onButton->Invalidate();
  }
  this->m_onButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x8741152D8FB34B4FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x955D4DF94DE9BB07ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xC52FB5FBB94FF917ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x93E7A3A1F587CDC5ULL)));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(150, 13);
  this->m_onText->Move(167, 12);
  this->m_onText->SetText(OBFUSCATE_SOURCE_TEXT("전체화면 모드", 0x2FF3C5036D070127ULL));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x17359BF953C367B3ULL)));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton();
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(270, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { OnFullscreenOptionChanged(reinterpret_cast<int>(button)); });
  {
    auto* const offButton = this->m_offButton;
    offButton->disabled = 0;
    offButton->Invalidate();
  }
  this->m_offButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x3375B1C7DBB5FD55ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x1D6D75EDE7CD311FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x59278F63D79F3DA1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x812BE72BCD5707B7ULL)));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(287, 12);
  this->m_offText->SetText(OBFUSCATE_SOURCE_TEXT("창화면 모드", 0xD3755DD95F47A909ULL));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x31D7C1EDA345E36DULL)));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A259E0
// FUNCTION: fn_15395
void GameSettingsUI::CUIGroupGraphics::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("그래픽 장치 설정", 0xCB3FBD4183DB8323ULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x7FCD77F9C7CBD14DULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_adapterCombo = new CUIComboBox<CUIComboItem, RenderAdapter>();
  this->m_adapterCombo->Create(162, 16, 5);
  this->m_adapterCombo->SetButton(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x0753A7E1EFA9E139ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x7FC1015DFFCF6D19ULL)),
                                  std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x474DC581D5E98357ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x3F557BE333F95F6DULL)));
  this->m_adapterCombo->Move(150, 9);
  this->m_adapterCombo->SetOnSelect([this](CUIComboItem* item, const RenderAdapter& adapter) { this->OnAdapterSelect(item, adapter); });
  this->m_adapterCombo->SetItemBinder([this](CUIComboItem* item, const RenderAdapter& adapter) {
    this->BindAdapterItem(item, adapter);
  });
  this->AddChild(this->m_adapterCombo);

  this->m_modeText = new CUIStaticText();
  this->m_modeText->SetSize(130, 13);
  this->m_modeText->Move(19, 44);
  this->m_modeText->SetText(OBFUSCATE_SOURCE_TEXT("그래픽 해상도 설정", 0x5B8F49AF09390D1BULL));
  this->m_modeText->SetFitText(0);
  this->m_modeText->SetAlign(0, 0);
  this->m_modeText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x3B915119EFD523EFULL)));
  this->m_modeText->SetFontHeight(12);
  this->m_modeText->SetBold(1);
  this->m_modeText->SetColor(0);
  this->m_modeText->SetShadow(0, 0xFFFFFF);
  this->m_modeText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_modeText);

  this->m_modeCombo = new CUIComboBox<CUIComboItem, DisplayMode>();
  this->m_modeCombo->Create(162, 16, 5);
  this->m_modeCombo->SetButton(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x6DED3DD1CF75AD9BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x49C3759B77DBD3C1ULL)),
                               std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x1BF915E1F37DFDFDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x01FF9313695DD9DFULL)));
  this->m_modeCombo->Move(150, 41);
  this->m_modeCombo->SetOnSelect([this](CUIComboItem* item, const DisplayMode& mode) { this->OnModeSelect(item, mode); });
  this->m_modeCombo->SetItemBinder([this](CUIComboItem* item, const DisplayMode& mode) {
    this->BindModeItem(item, mode);
  });
  this->AddChild(this->m_modeCombo);
}

// OFFSET: 0x00A27130
// FUNCTION: fn_15396
void GameSettingsUI::CUIGroupGraphicsAPI::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("그래픽 API", 0xCFF3899D95AF355FULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xE52DF30DCBAF7125ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_combo = new CUIComboBox<CUIComboItem, RenderBackend>();
  this->m_combo->Create(162, 16, 5);
  this->m_combo->SetButton(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x4F637DC51307B759ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x2DCB7567394DC1ABULL)),
                           std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0xB39FA7917BCFBB8DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\txtbox_btn_a.bmp", 0x8D1D0FF1AF2B13F7ULL)));
  this->m_combo->Move(150, 9);
  this->m_combo->SetOnSelect([this](CUIComboItem* item, const RenderBackend& backend) { this->OnSelect(item, backend); });
  this->m_combo->SetItemBinder([this](CUIComboItem* item, const RenderBackend& backend) {
    this->BindItem(item, backend);
  });
  this->AddChild(this->m_combo);
}

// OFFSET: 0x00A27D00
// FUNCTION: fn_15397
void GameSettingsUI::CUIGroupLogInOut::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(100, 12);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("로그인 알림", 0x0F15B16B5BAFD57FULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x7DE349D717D7F123ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton();
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(110, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const onButton = this->m_onButton;
    onButton->disabled = 0;
    onButton->Invalidate();
  }
  this->m_onButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x7321C3814F8799E1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x7941D76335CFA14FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x33133B977BCD03CDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x2FEB8739E5975BD1ULL)));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(100, 12);
  this->m_onText->Move(127, 12);
  this->m_onText->SetText(OBFUSCATE_SOURCE_TEXT("켜기", 0x6BA1C1132F033B75ULL));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xA12DAB392545D72FULL)));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton();
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(205, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const offButton = this->m_offButton;
    offButton->disabled = 0;
    offButton->Invalidate();
  }
  this->m_offButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x234977DDB54BC71DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0xEF6D1D893F2B2DC5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x39A39F87DD5F535BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x6FE153231D879777ULL)));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 12);
  this->m_offText->Move(222, 12);
  this->m_offText->SetText(OBFUSCATE_SOURCE_TEXT("끄기", 0x4F99DF03C3EF31B7ULL));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x9171997D6963C9F7ULL)));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A29180
// FUNCTION: fn_15398
void GameSettingsUI::CUIGroupProcessPriority::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(100, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("기본 우선순위", 0xED796785D1DF137FULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xD97D5DBDBB2F2709ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_highButton = new CUIRadioButton();
  this->m_highButton->SetSize(12, 12);
  this->m_highButton->Move(110, 12);
  this->m_highButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const highButton = this->m_highButton;
    highButton->disabled = 0;
    highButton->Invalidate();
  }
  this->m_highButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xA5FD29D7AB6DC59FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0xF14F79170939C7F1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x050505679903579BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xDD3913D5D7F97BE1ULL)));
  {
    auto* const highButton = this->m_highButton;
    highButton->checked = true;
    highButton->Invalidate();
  }
  this->AddChild(this->m_highButton);
  this->m_highText = new CUIStaticText();
  this->m_highText->SetSize(100, 13);
  this->m_highText->Move(127, 12);
  this->m_highText->SetText(OBFUSCATE_SOURCE_TEXT("높음", 0xBF2B4B31397FF5A7ULL));
  this->m_highText->SetFitText(0);
  this->m_highText->SetAlign(0, 0);
  this->m_highText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x83EFBFBB37EDBDBDULL)));
  this->m_highText->SetFontHeight(12);
  this->m_highText->SetBold(0);
  this->m_highText->SetColor(0);
  this->m_highText->SetShadow(0, 0xFFFFFF);
  this->m_highText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_highText);
  this->m_normalButton = new CUIRadioButton();
  this->m_normalButton->SetSize(12, 12);
  this->m_normalButton->Move(205, 12);
  this->m_normalButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const normalButton = this->m_normalButton;
    normalButton->disabled = 0;
    normalButton->Invalidate();
  }
  this->m_normalButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xF3CB6F7711A71B6DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x839B27DF8F5967B3ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x9FED7DA189490D5DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x452FEF0D7FC95565ULL)));
  {
    auto* const normalButton = this->m_normalButton;
    normalButton->checked = false;
    normalButton->Invalidate();
  }
  this->AddChild(this->m_normalButton);
  this->m_normalText = new CUIStaticText();
  this->m_normalText->SetSize(100, 13);
  this->m_normalText->Move(222, 12);
  this->m_normalText->SetText(OBFUSCATE_SOURCE_TEXT("보통", 0x176F138D5DD7F365ULL));
  this->m_normalText->SetFitText(0);
  this->m_normalText->SetAlign(0, 0);
  this->m_normalText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xFBD599E117C547C1ULL)));
  this->m_normalText->SetFontHeight(12);
  this->m_normalText->SetBold(0);
  this->m_normalText->SetColor(0);
  this->m_normalText->SetShadow(0, 0xFFFFFF);
  this->m_normalText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_normalText);
  this->m_idleButton = new CUIRadioButton();
  this->m_idleButton->SetSize(12, 12);
  this->m_idleButton->Move(300, 12);
  this->m_idleButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const idleButton = this->m_idleButton;
    idleButton->disabled = 0;
    idleButton->Invalidate();
  }
  this->m_idleButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x0F41EBBDA32D1B17ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x2D51E3B7236175EDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xCB1D253B7FE1F563ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x7115D9F973F913C9ULL)));
  {
    auto* const idleButton = this->m_idleButton;
    idleButton->checked = false;
    idleButton->Invalidate();
  }
  this->AddChild(this->m_idleButton);
  this->m_idleText = new CUIStaticText();
  this->m_idleText->SetSize(100, 13);
  this->m_idleText->Move(317, 12);
  this->m_idleText->SetText(OBFUSCATE_SOURCE_TEXT("낮음", 0xE55723712993051DULL));
  this->m_idleText->SetFitText(0);
  this->m_idleText->SetAlign(0, 0);
  this->m_idleText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x793F370985ED674FULL)));
  this->m_idleText->SetFontHeight(12);
  this->m_idleText->SetBold(0);
  this->m_idleText->SetColor(0);
  this->m_idleText->SetShadow(0, 0xFFFFFF);
  this->m_idleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_idleText);
}

// OFFSET: 0x00A2AEC0
// FUNCTION: fn_15399
void GameSettingsUI::CUIGroupRodexSpam::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(70, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("우편", 0x891B232D09451965ULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x758F1D0B4F7705B9ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton();
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(110, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const onButton = this->m_onButton;
    onButton->disabled = 0;
    onButton->Invalidate();
  }
  this->m_onButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xEDC74F6D13DF71EFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0xF161095D0DE18F95ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x2BAD156DE3E1F9D9ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x75ADC57D87EFBD8FULL)));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(100, 13);
  this->m_onText->Move(127, 12);
  this->m_onText->SetText(OBFUSCATE_SOURCE_TEXT("모든 유저 수신", 0x39FF7FB50DEB998FULL));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x2FE993D1DDE1F197ULL)));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton();
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(110, 36);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { this->OnSelect(button); });
  {
    auto* const offButton = this->m_offButton;
    offButton->disabled = 0;
    offButton->Invalidate();
  }
  this->m_offButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xAD075B8DB599F1CDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x2F8B0F37118D157FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x35E5656B5DEDCD75ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x4941FD9F21BBB9FDULL)));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(130, 13);
  this->m_offText->Move(127, 36);
  this->m_offText->SetText(OBFUSCATE_SOURCE_TEXT("임의 유저 수신 거부", 0xE7978FB5A94DD7FDULL));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB7FD8F670FC7B96BULL)));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A2CF80
// FUNCTION: fn_15401
void GameSettingsUI::CUIGroupSound::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(75, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("사운드 설정", 0x6BDF334F2BFDAD51ULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x674D9F3975573D41ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_bgmText = new CUIStaticText();
  this->m_bgmText->SetSize(50, 13);
  this->m_bgmText->Move(110, 12);
  this->m_bgmText->SetText(OBFUSCATE_SOURCE_TEXT("배경음", 0xC9712D676B5B3943ULL));
  this->m_bgmText->SetFitText(0);
  this->m_bgmText->SetAlign(0, 0);
  this->m_bgmText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xED15938D333D2DB9ULL)));
  this->m_bgmText->SetFontHeight(12);
  this->m_bgmText->SetBold(0);
  this->m_bgmText->SetColor(0);
  this->m_bgmText->SetShadow(0, 0xFFFFFF);
  this->m_bgmText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_bgmText);
  this->m_bgmSlider = new CUIHScrollBar();
  this->m_bgmSlider->SetSize(174, 13);
  this->m_bgmSlider->Move(152, 11);
  this->m_bgmSlider->SetOnChange([this](int position) { this->OnBgmScroll(position); });
  this->m_bgmSlider->SetTotal(128);
  this->m_bgmSlider->SetVisible(128);
  this->m_bgmSlider->step = 1;
  static_cast<CUIHScrollBar*>(this->m_bgmSlider)->SetButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1left.bmp", 0xBD2BA1C11B01E73BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1right.bmp", 0x4B755DB19953E311ULL)));
  this->m_bgmSlider->SetBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1mid.bmp", 0xFD69A9B3F311818DULL)));
  static_cast<CUIHScrollBar*>(this->m_bgmSlider)->SetThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_left.bmp", 0x7F2DFFC1ABE91F2BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_mid.bmp", 0xD98F5345098D630DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_right.bmp", 0x9B0F234DEFDF7593ULL)));
  this->AddChild(this->m_bgmSlider);
  this->m_bgmMute = new CUICheckBox();
  this->m_bgmMute->SetSize(10, 10);
  this->m_bgmMute->Move(334, 13);
  this->m_bgmMute->SetOnCheck([this](bool checked) { this->OnBgmMute(checked); });
  this->m_bgmMute->disabled = 0;
  this->m_bgmMute->Invalidate();
  this->m_bgmMute->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_0.bmp", 0xD7A749EB27D5BF6DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_1.bmp", 0xB195637DAFCB0D59ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_0.bmp", 0xADADEB4DF7813B55ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_0.bmp", 0xD9452D81E5CD99C1ULL)));
  this->m_bgmMute->checked = false;
  this->m_bgmMute->Invalidate();
  this->AddChild(this->m_bgmMute);
  this->m_bgmMuteText = new CUIStaticText();
  this->m_bgmMuteText->SetSize(50, 13);
  this->m_bgmMuteText->Move(348, 12);
  this->m_bgmMuteText->SetText(OBFUSCATE_SOURCE_TEXT("끄기", 0xF715514F5961B75BULL));
  this->m_bgmMuteText->SetFitText(0);
  this->m_bgmMuteText->SetAlign(0, 0);
  this->m_bgmMuteText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x4191BB5531FD3999ULL)));
  this->m_bgmMuteText->SetFontHeight(12);
  this->m_bgmMuteText->SetBold(0);
  this->m_bgmMuteText->SetColor(0);
  this->m_bgmMuteText->SetShadow(0, 0xFFFFFF);
  this->m_bgmMuteText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_bgmMuteText);
  this->m_effectText = new CUIStaticText();
  this->m_effectText->SetSize(50, 13);
  this->m_effectText->Move(110, 36);
  this->m_effectText->SetText(OBFUSCATE_SOURCE_TEXT("효과음", 0xB19D1FA9116F39D3ULL));
  this->m_effectText->SetFitText(0);
  this->m_effectText->SetAlign(0, 0);
  this->m_effectText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x9569C9C7E70909AFULL)));
  this->m_effectText->SetFontHeight(12);
  this->m_effectText->SetBold(0);
  this->m_effectText->SetColor(0);
  this->m_effectText->SetShadow(0, 0xFFFFFF);
  this->m_effectText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_effectText);
  this->m_effectSlider = new CUIHScrollBar();
  this->m_effectSlider->SetSize(174, 13);
  this->m_effectSlider->Move(152, 35);
  this->m_effectSlider->SetOnChange([this](int position) { this->OnEffectScroll(position); });
  this->m_effectSlider->SetTotal(128);
  this->m_effectSlider->SetVisible(128);
  this->m_effectSlider->step = 1;
  static_cast<CUIHScrollBar*>(this->m_effectSlider)->SetButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1left.bmp", 0xB7A96F8701BB4B6BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1right.bmp", 0xA31F69E7EFED01C9ULL)));
  this->m_effectSlider->SetBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1mid.bmp", 0xB127A19D775BF751ULL)));
  static_cast<CUIHScrollBar*>(this->m_effectSlider)->SetThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_left.bmp", 0x0F61499B4973EB5DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_mid.bmp", 0x85E1F5A7A5755B7FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_right.bmp", 0xAD6153019F93A527ULL)));
  this->AddChild(this->m_effectSlider);
  this->m_effectMute = new CUICheckBox();
  this->m_effectMute->SetSize(10, 10);
  this->m_effectMute->Move(334, 37);
  this->m_effectMute->SetOnCheck([this](bool checked) { this->OnEffectMute(checked); });
  this->m_effectMute->disabled = 0;
  this->m_effectMute->Invalidate();
  this->m_effectMute->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_0.bmp", 0xBFF5C517A1A77F59ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_1.bmp", 0x659975C121557D3DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_0.bmp", 0x41CFCB831F6B6B91ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\checkbox_0.bmp", 0x5DE7712FAB613F75ULL)));
  this->m_effectMute->checked = false;
  this->m_effectMute->Invalidate();
  this->AddChild(this->m_effectMute);
  this->m_effectMuteText = new CUIStaticText();
  this->m_effectMuteText->SetSize(50, 13);
  this->m_effectMuteText->Move(348, 36);
  this->m_effectMuteText->SetText(OBFUSCATE_SOURCE_TEXT("끄기", 0x81F58DED5D470D01ULL));
  this->m_effectMuteText->SetFitText(0);
  this->m_effectMuteText->SetAlign(0, 0);
  this->m_effectMuteText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x454D31872BB3EDCBULL)));
  this->m_effectMuteText->SetFontHeight(12);
  this->m_effectMuteText->SetBold(0);
  this->m_effectMuteText->SetColor(0);
  this->m_effectMuteText->SetShadow(0, 0xFFFFFF);
  this->m_effectMuteText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_effectMuteText);
}

// OFFSET: 0x00A2F880
// FUNCTION: fn_15402
void GameSettingsUI::CUIGroupSpriteDetailLevel::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("스프라이트 해상도", 0x4F1791FB1503C713ULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xA1A55B4D1597EDD3ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_slider = new CUIHScrollBar();
  this->m_slider->SetSize(175, 13);
  this->m_slider->Move(153, 11);
  this->m_slider->SetOnChange([this](int position) { OnSpriteDetailLevelChanged(position); });
  this->m_slider->SetTotal(128);
  this->m_slider->SetVisible(128);
  this->m_slider->step = 1;
  static_cast<CUIHScrollBar*>(this->m_slider)->SetButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1left.bmp", 0xD1538513DF975701ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1right.bmp", 0x91E147EBBD47C39FULL)));
  this->m_slider->SetBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1mid.bmp", 0xAFA5617F178B6185ULL)));
  static_cast<CUIHScrollBar*>(this->m_slider)->SetThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_left.bmp", 0x078143534999254DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_mid.bmp", 0x17FFC98B398FAF61ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_right.bmp", 0x9B5B959B954D9333ULL)));
  this->AddChild(this->m_slider);
}

// OFFSET: 0x00A30490
// FUNCTION: fn_15403
void GameSettingsUI::CUIGroupTextureDetailLevel::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("텍스쳐 해상도", 0x7D750DADBDE9C3F1ULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x33832D4D3FDFBDBFULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_slider = new CUIHScrollBar();
  this->m_slider->SetSize(175, 13);
  this->m_slider->Move(153, 11);
  this->m_slider->SetOnChange([this](int position) { OnTextureDetailLevelChanged(position); });
  this->m_slider->SetTotal(128);
  this->m_slider->SetVisible(128);
  this->m_slider->step = 1;
  static_cast<CUIHScrollBar*>(this->m_slider)->SetButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1left.bmp", 0x3F5315DFE32B4FAFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1right.bmp", 0xE50B2935073DD3E9ULL)));
  this->m_slider->SetBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1mid.bmp", 0x393D916561BDC391ULL)));
  static_cast<CUIHScrollBar*>(this->m_slider)->SetThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_left.bmp", 0x31956551B311FD95ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_mid.bmp", 0xFF7D83CB791339D9ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\scroll1bar_right.bmp", 0xED5143E7C1B39315ULL)));
  this->AddChild(this->m_slider);
}

// OFFSET: 0x00A31070
// FUNCTION: fn_15404
void GameSettingsUI::CUIGroupTrilinear::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText(OBFUSCATE_SOURCE_TEXT("트라이니어 필터링", 0xD98F03A52D35636DULL));
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x0F0F8B6B8F9D19D1ULL)));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_onButton = new CUIRadioButton();
  this->m_onButton->SetSize(12, 12);
  this->m_onButton->Move(150, 12);
  this->m_onButton->SetOnSelect([this](CUIRadioButton* button) { OnTrilinearOptionChanged(reinterpret_cast<int>(button)); });
  {
    auto* const onButton = this->m_onButton;
    onButton->disabled = 0;
    onButton->Invalidate();
  }
  this->m_onButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x91651D81F559F971ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x73D1674B61DF7B0BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x3FAF85D995034FD7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x490FFB43A12F0DB9ULL)));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(150, 13);
  this->m_onText->Move(167, 12);
  this->m_onText->SetText(OBFUSCATE_SOURCE_TEXT("켜기", 0x2D394DBD0F332733ULL));
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x151F77EBEBBB2147ULL)));
  this->m_onText->SetFontHeight(12);
  this->m_onText->SetBold(0);
  this->m_onText->SetColor(0);
  this->m_onText->SetShadow(0, 0xFFFFFF);
  this->m_onText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_onText);
  this->m_offButton = new CUIRadioButton();
  this->m_offButton->SetSize(12, 12);
  this->m_offButton->Move(270, 12);
  this->m_offButton->SetOnSelect([this](CUIRadioButton* button) { OnTrilinearOptionChanged(reinterpret_cast<int>(button)); });
  {
    auto* const offButton = this->m_offButton;
    offButton->disabled = 0;
    offButton->Invalidate();
  }
  this->m_offButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x61FF87ED7BF39BE1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x7D993F650D2785FFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x9B17C9EFDBB39F2FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x2D3B15774149D5DDULL)));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(287, 12);
  this->m_offText->SetText(OBFUSCATE_SOURCE_TEXT("끄기", 0xC7912587FFF31B71ULL));
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x69D345DB950F336DULL)));
  this->m_offText->SetFontHeight(12);
  this->m_offText->SetBold(0);
  this->m_offText->SetColor(0);
  this->m_offText->SetShadow(0, 0xFFFFFF);
  this->m_offText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_offText);
}

// OFFSET: 0x00A3FB50
// FUNCTION: fn_15957
void GameSettingsUI::CUIGroupSkin::OnSelect(CUIComboItem* item, const std::string& name) {
  const int selected = this->m_combo->listBox->selected;
  if (selected != -1) {
    const int skin = selected - 1;
    if (g_skinInfo.selected != skin) {
      g_skinInfo.Select(skin);
    }
    g_windowMgr.RefreshFrameSkins();
    g_windowMgr.InvalidateAll();
  }
}

// OFFSET: 0x00A3F2A0
// FUNCTION: fn_15941
void GameSettingsUI::CUIOptionItem::RunFunction() {
  if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
    Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
  }
  Singleton<CGameSettingsMgr>::s_instance->RunFunction(this->m_optionId);
}

// OFFSET: 0x00A3F720
// FUNCTION: fn_15950
void GameSettingsUI::CUIOptionItem::OnCheck(bool checked) {
  if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
    Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
  }
  CGameSettingsMgr* const settings = Singleton<CGameSettingsMgr>::s_instance;
  settings->SetOption(this->m_optionId, IsServiceOption(this->m_optionId) ^ checked, false);
}

// OFFSET: 0x00A40280
// FUNCTION: fn_15969
void GameSettingsUI::CUIListPage::BindOption(CUIOptionItem* item, const TALKTYPE& id) {
  item->m_optionId = id;
  if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
    Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
  }
  const std::optional<GameSettingsOption*> found = Singleton<CGameSettingsMgr>::s_instance->FindOption(id);
  if (!found.has_value()) {
    return;
  }

  item->m_check->Show((*found)->type == 0);
  CUICheckBox* check = item->m_check;
  if (check->geometry.visible) {
    check->checked = IsServiceOption(item->m_optionId) ^ g_serviceOptions.Get(item->m_optionId);
    check->Invalidate();
  }
  item->m_runButton->Show((*found)->type == 1);
  item->m_label->SetText((*found)->title);
  item->m_label->tooltipText = (*found)->tooltip;
  auto* const label = item->m_label;
  label->disabled = (*found)->tipBoxId <= 0;
  label->Invalidate();
  item->m_label->SetId((*found)->tipBoxId);
  item->m_description->SetText((*found)->description);
}

// OFFSET: 0x00A42290
// FUNCTION: fn_16014
void GameSettingsUI::CUIListPage::LoadOptions(int tab) {
  if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
    Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
  }
  const std::list<int> ids = Singleton<CGameSettingsMgr>::s_instance->GetOptionIds(tab);
  for (const int id : ids) {
    this->m_optionList->data.push_back(static_cast<TALKTYPE>(id));
  }
  this->m_optionList->UpdateScrollBar();
  auto* const list = this->m_optionList;
  list->layoutFlag = 1;
  list->Invalidate();
  this->RefreshOptions();
}

// OFFSET: 0x00A43BF0
// FUNCTION: fn_16047
void GameSettingsUI::CUIListPage::RefreshOptions() {
  for (CUIOptionItem* item : this->m_optionList->items) {
    if (!item->geometry.visible) {
      continue;
    }
    CUICheckBox* check = item->m_check;
    check->checked = IsServiceOption(item->m_optionId) ^ g_serviceOptions.Get(item->m_optionId);
    check->Invalidate();
  }
  this->RefreshHotkeyLabels();
}

// OFFSET: 0x00A43410
// FUNCTION: fn_16030
void GameSettingsUI::CUIListPage::RefreshHotkeyLabels() {
  for (CUIOptionItem* item : this->m_optionList->items) {
    if (!item->geometry.visible) {
      continue;
    }
    const auto hotkey = g_optionHotkeys.find(item->m_optionId);
    if (hotkey == g_optionHotkeys.end()) {
      continue;
    }

    const std::string text = this->GetHotkeyText(1, hotkey->second);
    if (text.empty()) {
      if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
        Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
      }
      const std::optional<GameSettingsOption*> option = Singleton<CGameSettingsMgr>::s_instance->FindOption(item->m_optionId);
      if (option.has_value()) {
        std::string& tooltip = item->m_label->tooltipText;
        tooltip = (*option)->tooltip;
      }
    } else {
      item->m_label->tooltipText = text;
    }
  }
}

// OFFSET: 0x00A43BA0
// FUNCTION: fn_16046
void CUIGameSettingsUI::RefreshOptions() {
  for (UIWindow* child : this->m_currentPage->m_children) {
    static_cast<GameSettingsUI::CUIGroup*>(child)->Refresh();
  }
  for (GameSettingsUI::CUIListPage* page : this->m_pages) {
    page->RefreshOptions();
  }
}

// OFFSET: 0x00A43C70
// FUNCTION: fn_16048
void CUIGameSettingsUI::RefreshHotkeyLabels() {
  for (GameSettingsUI::CUIListPage* page : this->m_pages) {
    for (GameSettingsUI::CUIOptionItem* item : page->m_optionList->items) {
      if (!item->geometry.visible) {
        continue;
      }
      const auto hotkey = g_optionHotkeys.find(item->m_optionId);
      if (hotkey == g_optionHotkeys.end()) {
        continue;
      }

      const std::string text = page->GetHotkeyText(1, hotkey->second);
      if (text.empty()) {
        if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
          Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
        }
        const std::optional<GameSettingsOption*> option = Singleton<CGameSettingsMgr>::s_instance->FindOption(item->m_optionId);
        if (option.has_value()) {
          std::string& tooltip = item->m_label->tooltipText;
          tooltip = (*option)->tooltip;
        }
      } else {
        item->m_label->tooltipText = text;
      }
    }
  }
}

// OFFSET: 0x00A3F180
// FUNCTION: fn_15940
void CUIGameSettingsUI::ResetToDefaults() {
  if (g_windowMgr.Confirm(LookupMessage(MSI_GRAPHIC_SETTING_FOG_CHECK), 2, true, false, 0, "Message", 280, 120, 0) == 0xBC) {
    return;
  }

  for (UIWindow* child : this->m_currentPage->m_children) {
    static_cast<GameSettingsUI::CUIGroup*>(child)->Apply();
  }
  if (Singleton<CGameSettingsMgr>::s_instance == nullptr) {
    Singleton<CGameSettingsMgr>::s_instance = new (std::nothrow) CGameSettingsMgr;
  }
  Singleton<CGameSettingsMgr>::s_instance->ResetToDefaults();
  for (UIWindow* child : this->m_currentPage->m_children) {
    static_cast<GameSettingsUI::CUIGroup*>(child)->Refresh();
  }
  for (GameSettingsUI::CUIListPage* page : this->m_pages) {
    page->RefreshOptions();
  }
}

// OFFSET: 0x00A1A1C0
// FUNCTION: fn_15348
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderAdapter>::~CUIListBoxEx() {
}

// OFFSET: 0x00A1A320
// FUNCTION: fn_15349
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, DisplayMode>::~CUIListBoxEx() {
}

// OFFSET: 0x00A1A480
// FUNCTION: fn_15350
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, RenderBackend>::~CUIListBoxEx() {
}

// OFFSET: 0x00A1A5E0
// FUNCTION: fn_15351
template <>
CUIListBoxEx<GameSettingsUI::CUIComboItem, std::string>::~CUIListBoxEx() {
}

// OFFSET: 0x00A1A740
// FUNCTION: fn_15352
template <>
CUIListBoxEx<GameSettingsUI::CUIOptionItem, TALKTYPE>::~CUIListBoxEx() {
}

// OFFSET: 0x00A40900
// FUNCTION: fn_15978
void GameSettingsUI::CUIGraphicsPage::ApplySettings() {
  if (this->IsModified()) {
    CUIGroupGraphicsAPI* graphicsApi = this->m_graphicsApi;
    graphicsApi->m_backend = graphicsApi->m_pendingBackend;
    g_configuredRenderBackend = graphicsApi->m_pendingBackend;

    CUIGroupGraphics* graphics = this->m_graphics;
    graphics->m_adapter = graphics->m_pendingAdapter;
    graphics->m_mode = graphics->m_pendingMode;
    switch (g_configuredRenderBackend) {
    case 1:
      strncpy_s(g_deviceName, sizeof(g_deviceName), graphics->m_pendingAdapter.description.c_str(), _TRUNCATE);
      std::memcpy(g_guidDevice, &graphics->m_pendingAdapter.renderGuid, sizeof(GUID));
      std::memcpy(g_guidDriver, &graphics->m_pendingAdapter.drawGuid, sizeof(GUID));
      g_configuredDeviceCount = graphics->m_adapterCombo->listBox->selected;
      g_configuredModeCount = graphics->m_modeCombo->listBox->selected;
      break;
    case 2:
      g_configuredDeviceGuid = graphics->m_pendingAdapter.deviceGuid;
      strncpy_s(g_configuredDeviceName, sizeof(g_configuredDeviceName), graphics->m_pendingAdapter.deviceName.c_str(), _TRUNCATE);
      break;
    }
    g_configuredScreenWidth = graphics->m_pendingMode.width;
    g_configuredScreenHeight = graphics->m_pendingMode.height;
    g_configuredBitsPerPixel = graphics->m_pendingMode.bitsPerPixel;
    g_configuredFullscreen = this->m_fullscreen->m_onButton->checked;
    g_restartClient = true;
    GetRagConnection().Close();
    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
    }
    return;
  }

  const int spriteLevel = this->m_spriteDetailLevel->m_slider->position;
  if (spriteLevel != g_effectDetailLevel) {
    g_effectDetailLevel = spriteLevel;
    switch (spriteLevel) {
    case 0:
      g_spriteHalfWidth = 1;
      g_spriteHalfHeight = 1;
      break;
    case 1:
      g_spriteHalfWidth = 0;
      g_spriteHalfHeight = 1;
      break;
    default:
      g_spriteHalfWidth = 0;
      g_spriteHalfHeight = 0;
      break;
    }
  }

  bool reloadTextures = false;
  const int textureLevel = this->m_textureDetailLevel->m_slider->position;
  if (textureLevel != g_textureDetailLevel) {
    g_textureDetailLevel = textureLevel;
    int reduction = 1;
    if (textureLevel == 0) {
      reduction = 4;
    } else if (textureLevel == 1) {
      reduction = 2;
    }
    if (reduction != g_textureReduction) {
      g_textureReduction = reduction;
      reloadTextures = true;
    }
  }

  CUIRadioButton* trilinearButton = this->m_trilinear->m_onButton;
  if (trilinearButton->checked != static_cast<bool>(g_textureFiltering)) {
    g_textureFiltering = trilinearButton->checked;
    GetRenderSystemHolder()->UpdateOption();
    g_texMgr.Reload();
  } else if (reloadTextures) {
    g_texMgr.Reload();
  }
}

// OFFSET: 0x00A1FB10
// FUNCTION: fn_15391
void CUIGameSettingsUI::OnCreate(int width, int height) {
  this->m_titleBar = new CUITitleBar();
  this->m_titleBar->SetSize(400, 17);
  this->m_titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->m_titleBar;
    titleBar->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0x078333BB9725B7C9ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0x8F1B5BFD5F6175B5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0x8B3FEB5B8BD5C3C3ULL)));
  }
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    title->SetText(OBFUSCATE_SOURCE_TEXT("게임 설정", 0x0BEB63A5C905B3A7ULL));
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x8B65CD61012B5931ULL)));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  CUITitleBar* titleBar = this->m_titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* baseButton = new CUIImageButton();
  baseButton->SetSize(11, 11);
  baseButton->Move(3, 3);
  baseButton->SetOnClick([this]() { this->OnTitleIconClick(); });
  baseButton->disabled = 0;
  baseButton->Invalidate();
  {
    baseButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0xF7530137154D8FE5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x69FD1D9733D3CFC1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0xF5657D1F3DE5CDC7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x1FDB81175D1557E3ULL)));
  }
  this->m_titleBar->AddChild(baseButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(386, 3);
  closeButton->SetOnClick([this]() { this->OnTitleClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    closeButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xE3FB23537D2785BFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0xB7A76BF9CD41F99FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x732B390D63FD47D7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0x71A509DB43792991ULL)));
  }
  this->m_titleBar->SetSecondButton(closeButton);

  this->m_currentPage = new GameSettingsUI::CUIBasicPage();
  this->m_currentPage->SetSize(400, 288);
  this->m_currentPage->Move(0, 34);
  this->AddChild(this->m_currentPage);

  this->m_graphicsPage = new GameSettingsUI::CUIGraphicsPage();
  this->m_graphicsPage->SetSize(400, 288);
  this->m_graphicsPage->Move(0, 34);
  this->AddChild(this->m_graphicsPage);

  for (int index = 0; index < 3; ++index) {
    GameSettingsUI::CUIListPage* page = new GameSettingsUI::CUIListPage();
    page->SetSize(400, 288);
    this->AddChild(page);
    this->m_pages.push_back(page);
  }
  this->m_pages[0]->Move(0, 34);
  this->m_pages[1]->Move(0, 34);
  this->m_pages[2]->Move(0, 34);

  this->m_tab = new CUITab();
  this->m_tab->SetSize(400, 18);
  this->m_tab->Move(0, 17);
  this->m_tab->SetOnChange([this](CUITabButton* button) { this->OnTabChanged(button); });
  this->AddChild(this->m_tab);

  CUITabButton* basicButton = new CUITabButton();
  basicButton->SetSize(72, 18);
  basicButton->Move(0, 0);
  basicButton->SetOnClick([this]() { this->OnBasicTabClick(); });
  basicButton->disabled = 0;
  basicButton->Invalidate();
  basicButton->SetId(0);
  {
    basicButton->SetText(std::string(OBFUSCATE_SOURCE_TEXT("기본 설정", 0xDD69EDFFA18F537DULL)));
  }
  basicButton->SetFitText(0);
  basicButton->SetAlign(6, 6);
  {
    basicButton->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x8BBF792B47E72173ULL)));
  }
  basicButton->SetFontHeight(12);
  basicButton->SetBold(0);
  basicButton->SetTextColors(0, 0, 0, 0x808080);
  basicButton->SetShadow(0, 0xFFFFFF);
  basicButton->SetOutline(0, 0xFFFFFF);
  basicButton->SetSkinColors(0, 0, 0, 0, 3, 1);
  this->m_tab->AddChild(basicButton);

  CUITabButton* effectButton = new CUITabButton();
  effectButton->SetSize(72, 18);
  effectButton->Move(71, 0);
  effectButton->SetOnClick([this]() { this->OnEffectTabClick(); });
  effectButton->disabled = 0;
  effectButton->Invalidate();
  effectButton->SetId(1);
  {
    effectButton->SetText(std::string(OBFUSCATE_SOURCE_TEXT("이펙트 설정", 0xB3CF9BEB7963DB89ULL)));
  }
  effectButton->SetFitText(0);
  effectButton->SetAlign(6, 6);
  {
    effectButton->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB5D7B5AF4FEDFF57ULL)));
  }
  effectButton->SetFontHeight(12);
  effectButton->SetBold(0);
  effectButton->SetTextColors(0, 0, 0, 0x808080);
  effectButton->SetShadow(0, 0xFFFFFF);
  effectButton->SetOutline(0, 0xFFFFFF);
  effectButton->useSkinColors = 1;
  this->m_tab->AddChild(effectButton);

  CUITabButton* controlButton = new CUITabButton();
  controlButton->SetSize(72, 18);
  controlButton->Move(142, 0);
  controlButton->SetOnClick([this]() { this->OnControlTabClick(); });
  controlButton->disabled = 0;
  controlButton->Invalidate();
  controlButton->SetId(2);
  {
    controlButton->SetText(std::string(OBFUSCATE_SOURCE_TEXT("컨트롤 설정", 0x9B9D51AD3515F779ULL)));
  }
  controlButton->SetFitText(0);
  controlButton->SetAlign(6, 6);
  {
    controlButton->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x038535BB092533B7ULL)));
  }
  controlButton->SetFontHeight(12);
  controlButton->SetBold(0);
  controlButton->SetTextColors(0, 0, 0, 0x808080);
  controlButton->SetShadow(0, 0xFFFFFF);
  controlButton->SetOutline(0, 0xFFFFFF);
  controlButton->SetSkinColors(0, 0, 0, 0, 3, 1);
  this->m_tab->AddChild(controlButton);

  CUITabButton* graphicsButton = new CUITabButton();
  graphicsButton->SetSize(72, 18);
  graphicsButton->Move(213, 0);
  graphicsButton->SetOnClick([this]() { this->OnGraphicsTabClick(); });
  graphicsButton->disabled = 0;
  graphicsButton->Invalidate();
  graphicsButton->SetId(3);
  {
    graphicsButton->SetText(std::string(OBFUSCATE_SOURCE_TEXT("그래픽 설정", 0xFB2BC75769E777D3ULL)));
  }
  graphicsButton->SetFitText(0);
  graphicsButton->SetAlign(6, 6);
  {
    graphicsButton->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x91493751EF33B5FDULL)));
  }
  graphicsButton->SetFontHeight(12);
  graphicsButton->SetBold(0);
  graphicsButton->SetTextColors(0, 0, 0, 0x808080);
  graphicsButton->SetShadow(0, 0xFFFFFF);
  graphicsButton->SetOutline(0, 0xFFFFFF);
  graphicsButton->SetSkinColors(0, 0, 0, 0, 3, 1);
  this->m_tab->AddChild(graphicsButton);

  CUITabButton* etcButton = new CUITabButton();
  etcButton->SetSize(72, 18);
  etcButton->Move(284, 0);
  etcButton->SetOnClick([this]() { this->OnEtcTabClick(); });
  etcButton->disabled = 0;
  etcButton->Invalidate();
  etcButton->SetId(4);
  {
    etcButton->SetText(std::string(OBFUSCATE_SOURCE_TEXT("기타 설정", 0x6BC977B3B5993503ULL)));
  }
  etcButton->SetFitText(0);
  etcButton->SetAlign(6, 6);
  {
    etcButton->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB5F3F779237315F3ULL)));
  }
  etcButton->SetFontHeight(12);
  etcButton->SetBold(0);
  etcButton->SetTextColors(0, 0, 0, 0x808080);
  etcButton->SetShadow(0, 0xFFFFFF);
  etcButton->SetOutline(0, 0xFFFFFF);
  etcButton->SetSkinColors(0, 0, 0, 0, 3, 1);
  this->m_tab->AddChild(etcButton);

  this->m_pageContainer = new CUIImageButton();
  this->m_pageContainer->SetSize(42, 20);
  this->m_pageContainer->Move(5, 326);
  this->m_pageContainer->SetOnClick([this]() { this->ResetToDefaults(); });
  this->m_pageContainer->disabled = 0;
  this->m_pageContainer->Invalidate();
  {
    CUIImageButton* button = this->m_pageContainer;
    button->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_reset.bmp", 0x67DD5BAB1919D9D1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_reset_b.bmp", 0x9BE3EF737387AD79ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_reset_a.bmp", 0xB96B77B7255F8569ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_reset.bmp", 0x875FCDF1E7A1795FULL)));
  }
  this->AddChild(this->m_pageContainer);

  this->m_applyButton = new CUIImageButton();
  this->m_applyButton->SetSize(42, 20);
  this->m_applyButton->Move(301, 326);
  this->m_applyButton->SetOnClick([this]() { this->OnApply(); });
  this->m_applyButton->disabled = 0;
  this->m_applyButton->Invalidate();
  {
    CUIImageButton* button = this->m_applyButton;
    button->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_apply.bmp", 0x072D2D872D259D5BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_apply_b.bmp", 0x9D8F2D37BFFD61B7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_apply_a.bmp", 0x79DF094DD9616575ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_apply.bmp", 0x11CB35636377DD07ULL)));
  }
  this->AddChild(this->m_applyButton);

  this->m_closeButton = new CUIImageButton();
  this->m_closeButton->SetSize(42, 20);
  this->m_closeButton->Move(348, 326);
  this->m_closeButton->SetOnClick([this]() { this->OnCloseButton(); });
  this->m_closeButton->disabled = 0;
  this->m_closeButton->Invalidate();
  {
    CUIImageButton* button = this->m_closeButton;
    button->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_close.bmp", 0x4755C1355575FD37ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_close_b.bmp", 0x6BF729E30729CF29ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_close_a.bmp", 0x3D371F5B0F7F9327ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\btn_close.bmp", 0x1F1D51274373C1FDULL)));
  }
  this->AddChild(this->m_closeButton);

  basicButton->page = this->m_currentPage;
  graphicsButton->page = this->m_graphicsPage;
  effectButton->page = this->m_pages[0];
  controlButton->page = this->m_pages[1];
  etcButton->page = this->m_pages[2];
  this->m_tab->Select(basicButton);
}

// OFFSET: 0x00A40C40
// FUNCTION: fn_15986
void CUIGameSettingsUI::OnInit() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  static_cast<CUITitleBar*>(this->m_titleBar)->SetTitle(LookupMessage(MSI_COMMAND_LIST));
  static_cast<CUITextButton*>(this->m_tab->FindChildById(0))->SetText(LookupMessage(MSI_GAME_SETTINGS_EMBLEM_FRAME));
  static_cast<CUITextButton*>(this->m_tab->FindChildById(1))->SetText(LookupMessage(MSI_GAME_SETTINGS_ON));
  static_cast<CUITextButton*>(this->m_tab->FindChildById(2))->SetText(LookupMessage(MSI_GAME_SETTINGS_OFF));
  static_cast<CUITextButton*>(this->m_tab->FindChildById(3))->SetText(LookupMessage(MSI_GAME_SETTINGS_SHOW));
  static_cast<CUITextButton*>(this->m_tab->FindChildById(4))->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_GLOBAL_POST));

  for (UIWindow* child : this->m_currentPage->m_children) {
    static_cast<GameSettingsUI::CUIGroup*>(child)->Refresh();
  }
  for (UIWindow* child : this->m_graphicsPage->m_children) {
    static_cast<GameSettingsUI::CUIGroup*>(child)->Refresh();
  }
  for (GameSettingsUI::CUIListPage* page : this->m_pages) {
    page->LoadOptions(this->m_tab->FindButton(page)->geometry.id);
  }
}

// OFFSET: 0x00A33250
// FUNCTION: fn_15406
void GameSettingsUI::CUIOptionItem::OnCreate(int width, int height) {
  this->m_check = new CUICheckBox();
  this->m_check->SetSize(34, 15);
  this->m_check->Move(8, 5);
  this->m_check->SetOnCheck([this](bool checked) { this->OnCheck(checked); });
  this->m_check->disabled = 0;
  this->m_check->Invalidate();
  this->m_check->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\toggle_off.bmp", 0xA597A7E7492B39D5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\toggle_on.bmp", 0xFFABE3D5D1BBEF73ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\toggle_off.bmp", 0x0399459DAF454F0FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\toggle_off.bmp", 0xD5A78DB38D8DAFABULL)));
  this->m_check->checked = false;
  this->m_check->Invalidate();
  this->AddChild(this->m_check);

  CUIImageButton* runButton = new CUIImageButton();
  this->m_runButton = runButton;
  runButton->SetSize(34, 15);
  runButton->Move(7, 5);
  runButton->SetOnClick([this]() { this->RunFunction(); });
  runButton->disabled = 0;
  runButton->Invalidate();
  runButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\btn_optionapply_out.bmp", 0x8783C33941E15117ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\btn_optionapply_press.bmp", 0x271B45771F83DD9FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\btn_optionapply_over.bmp", 0xC70319B5EB61598FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\GameSettingsUI\\btn_optionapply_disable.bmp", 0x0B8745519B1F95D7ULL)));
  this->AddChild(this->m_runButton);

  this->m_label = new CUITextButton();
  this->m_label->SetSize(85, 13);
  this->m_label->Move(47, 6);
  this->m_label->SetOnClick([this]() {
    if (this->m_label->geometry.id > 0) {
      if (UITipboxWnd* tipbox = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(WID_TIPBOXWND))) {
        tipbox->ShowTip(this->m_label->geometry.id);
      }
    }
  });
  this->m_label->disabled = 0;
  this->m_label->Invalidate();
  this->m_label->SetText(std::string());
  this->m_label->SetFitText(true);
  this->m_label->SetAlign(0, 0);
  this->m_label->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xADC51BF9C98B413DULL)));
  this->m_label->SetFontHeight(12);
  this->m_label->SetBold(1);
  this->m_label->SetTextColors(0, 0, 0, 0);
  this->m_label->SetShadow(false, 0xFFFFFF);
  this->m_label->SetOutline(false, 0xFFFFFF);
  this->AddChild(this->m_label);

  this->m_description = new CUIStaticText();
  this->m_description->SetSize(252, 13);
  this->m_description->Move(138, 6);
  this->m_description->SetText("", 0);
  this->m_description->SetFitText(1);
  this->m_description->SetAlign(0, 0);
  this->m_description->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xDF8117FF49CFF551ULL)));
  this->m_description->SetFontHeight(12);
  this->m_description->SetBold(0);
  this->m_description->SetColor(0);
  this->m_description->SetShadow(0, 0xFFFFFF);
  this->m_description->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_description);
}

// OFFSET: 0x00A3F320
// FUNCTION: fn_45512
void GameSettingsUI::CUIOptionItem::OnClickLabel() {
  if (this->m_label->geometry.id > 0) {
    if (UITipboxWnd* tipbox = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(WID_TIPBOXWND))) {
      tipbox->ShowTip(this->m_label->geometry.id);
    }
  }
}
