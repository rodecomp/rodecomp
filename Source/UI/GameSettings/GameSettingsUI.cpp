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
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iterator>
#include <list>
#include <string>
#include <vector>

namespace {
template <std::size_t Length>
class EncryptedSource {
public:
  // INLINED
  constexpr explicit EncryptedSource(const std::uint8_t (&source)[Length]) : data{} {
    for (std::size_t index = 0; index < Length; ++index) {
      this->data[index] = static_cast<char>(source[index]);
    }
  }

  char data[Length];
};

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const EncryptedSource<Length>& source) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = static_cast<std::uint8_t>(source.data[index]);
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


// GLOBAL: gl_24509
int g_textureFiltering;

// GLOBAL: gl_24964
int g_textureDetailLevel;

// GLOBAL: gl_25086
std::map<int, int> g_optionHotkeys;

using OptionHotkeyTree = std::_Tree<std::_Tmap_traits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, int>, void*>*>
OptionHotkeyTree::_Find_lower_bound<int>(const int&) const;

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

void SetComboItemSkinText(GameSettingsUI::CUIComboItem* item, const std::string& name);
// OFFSET: 0x00A2C3C0
// FUNCTION: fn_15400
void GameSettingsUI::CUIGroupSkin::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(75, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x60, 0xEF, 0xAA, 0x1D, 0xE9};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x79E915E9AF6F55DDULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x46, 0x00, 0x91, 0xB0, 0x42, 0xBD};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xB7AFBD2FD9FD7501ULL> name(source);
    return name.Text();
  }()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_combo = new CUIComboBox<CUIComboItem, std::string>();
  this->m_combo->Create(162, 16, 5);
  this->m_combo->SetButton(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x25, 0xD4, 0xF3, 0xD3, 0x8D, 0xAB, 0xE8, 0xD4, 0x23, 0x9F, 0xF3, 0xE5, 0xF0, 0xDF, 0x71, 0x7B, 0x84, 0x08, 0x5A, 0x4A, 0x12, 0x0C, 0x43, 0x6D, 0x80, 0x09, 0x55, 0x48, 0x2E, 0x00, 0x71, 0x6D, 0x9D, 0x0F, 0x51, 0x46, 0x35, 0x3A, 0x4F, 0x6D, 0x8B, 0x24, 0x52, 0x07, 0x2F, 0x08, 0x5D, 0x19};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x192D654D29337BE5ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x23, 0x7A, 0x93, 0x0D, 0x65, 0xC9, 0x26, 0x7E, 0x25, 0x31, 0x93, 0x3B, 0x18, 0xBD, 0xBF, 0xD1, 0x82, 0xA6, 0x3A, 0x94, 0xFA, 0x6E, 0x8D, 0xC7, 0x86, 0xA7, 0x35, 0x96, 0xC6, 0x62, 0xBF, 0xC7, 0x9B, 0xA1, 0x31, 0x98, 0xDD, 0x58, 0x81, 0xC7, 0x8D, 0x8A, 0x32, 0xD9, 0xC7, 0x6A, 0x93, 0xB3};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0xB3E307A5F753D5E3ULL> name(source);
    return name.Text();
  }()),
                   std::string([]() {
    constexpr std::uint8_t bytes[] = {0xEB, 0x78, 0x89, 0xD5, 0xCF, 0x1F, 0xE6, 0xC6, 0xED, 0x33, 0x89, 0xE3, 0xB2, 0x6B, 0x7F, 0x69, 0x4A, 0xA4, 0x20, 0x4C, 0x50, 0xB8, 0x4D, 0x7F, 0x4E, 0xA5, 0x2F, 0x4E, 0x6C, 0xB4, 0x7F, 0x7F, 0x53, 0xA3, 0x2B, 0x40, 0x77, 0x8E, 0x41, 0x7F, 0x45, 0x88, 0x28, 0x01, 0x6D, 0xBC, 0x53, 0x0B};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x0B23D10F2F49D72BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x0F, 0x74, 0x11, 0xEF, 0x59, 0xE1, 0xAA, 0xDE, 0x09, 0x3F, 0x11, 0xD9, 0x24, 0x95, 0x33, 0x71, 0xAE, 0xA8, 0xB8, 0x76, 0xC6, 0x46, 0x01, 0x67, 0xAA, 0xA9, 0xB7, 0x74, 0xFA, 0x4A, 0x33, 0x67, 0xB7, 0xAF, 0xB3, 0x7A, 0xE1, 0x70, 0x0D, 0x67, 0xA1, 0x84, 0xB0, 0x3B, 0xFB, 0x42, 0x1F, 0x13};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x136F2F9915D1DBCFULL> name(source);
    return name.Text();
  }()));
  this->m_combo->Move(110, 9);
  this->m_combo->SetOnSelect([this](CUIComboItem* item, const std::string& name) { this->OnSelect(item, name); });
  this->m_combo->SetItemBinder([this](CUIComboItem* item, const std::string& name) {
    SetComboItemSkinText(item, name);
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

  this->listBox->SetScrollBarButtonImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x57, 0x66, 0x09, 0x61, 0x0D, 0x75, 0x74, 0x08, 0x51, 0x2D, 0x09, 0x57, 0x70, 0x01, 0xED,
                                   0xB6, 0xF4, 0xBB, 0xA6, 0xF7, 0xA1, 0x8B, 0xC4, 0xB5, 0xB9, 0xAB, 0xA4, 0xEB, 0xCD};
    constexpr EncryptedSource<29> source(bytes);
    thread_local EncryptedName<29, 0xC5B1BBCD9BC9C997ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x21, 0x2C, 0x11, 0x17, 0xD1, 0x4F, 0x7E, 0x0C, 0x27, 0x67, 0x11, 0x21, 0xAC, 0x3B, 0xE7, 0xB2,
                                     0x82, 0xF1, 0xBE, 0x81, 0x7D, 0xB1, 0xDF, 0xAE, 0x96, 0xED, 0xFF, 0x8F, 0x7C, 0xF1, 0xBB};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xC1BB8111EDD183E1ULL> name(source);
    return name.Text();
  }()));

  this->listBox->SetScrollBarBackground(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4B, 0x32, 0xB7, 0x77, 0x21, 0x13, 0xA2, 0xDA, 0x4D, 0x79, 0xB7, 0x41, 0x5C, 0x67, 0x3B,
                                           0x64, 0xE8, 0xEF, 0x18, 0xE1, 0x8D, 0xED, 0x0A, 0x7E, 0xEF, 0xB3, 0x15, 0xE0, 0x91, 0xDD};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0x1767DDE18D779D8BULL> name(source);
    return name.Text();
  }()));

  this->listBox->SetScrollBarThumbImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x99, 0x2A, 0x47, 0x83, 0x37, 0xE1, 0xFA, 0x2E, 0x9F, 0x61, 0x47, 0xB5, 0x4A, 0x95, 0x63, 0x90, 0x3A,
                                      0xF7, 0xE8, 0x15, 0x9B, 0x1F, 0x5D, 0x82, 0x2B, 0xDA, 0xF2, 0x09, 0xD9, 0x4D, 0x52, 0x93, 0x59};
    constexpr EncryptedSource<33> source(bytes);
    thread_local EncryptedName<33, 0xE33F2FF779878559ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x7F, 0xC6, 0x23, 0x37, 0x45, 0x49, 0x9A, 0x3C, 0x79, 0x8D, 0x23, 0x01, 0x38, 0x3D, 0x03, 0x82, 0xDC,
                                       0x1B, 0x8C, 0xA1, 0xE9, 0xB7, 0x3D, 0x90, 0xCD, 0x36, 0x8E, 0xA4, 0xE1, 0xA9, 0x3D, 0x9C, 0xCF, 0x69};
    constexpr EncryptedSource<34> source(bytes);
    thread_local EncryptedName<34, 0xF15F8785CDE369BFULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x75, 0x20, 0x81, 0xDF, 0xD3, 0x33, 0xEA, 0xB4, 0x73, 0x6B, 0x81, 0xE9, 0xAE, 0x47, 0x73, 0x0A, 0xD6, 0xFD,
                                        0x2E, 0x49, 0x7F, 0xCD, 0x4D, 0x18, 0xC7, 0xD0, 0x25, 0x4A, 0x64, 0x93, 0x01, 0x1B, 0xD8, 0xFF, 0x41};
    constexpr EncryptedSource<35> source(bytes);
    thread_local EncryptedName<35, 0x792FFD1325418FB5ULL> name(source);
    return name.Text();
  }()));

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
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
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
    return std::string();
  }
  return std::string(keyText);
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
  const bool muted = !enabled;
  this->m_bgmSlider->SetTransparent(muted);
  this->m_bgmSlider->dragging = muted;
  this->m_bgmSlider->Invalidate();
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
  RodexSpamOptionPacket packet = {0x0B93, 1, button != this->m_offButton};
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
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
void SetComboItemAdapterText(GameSettingsUI::CUIComboItem* item, const RenderAdapter& adapter) {
  item->m_text->SetText(adapter.description);
}

// OFFSET: 0x00A3FA90
// FUNCTION: fn_15954
void SetComboItemModeText(GameSettingsUI::CUIComboItem* item, const DisplayMode& mode) {
  item->m_text->SetText(mode.description);
}

// OFFSET: 0x00A3FB20
// FUNCTION: fn_15956
void SetComboItemBackendText(GameSettingsUI::CUIComboItem* item, const RenderBackend& backend) {
  item->m_text->SetText(backend.name);
}

// OFFSET: 0x00A3FBA0
// FUNCTION: fn_15958
void SetComboItemSkinText(GameSettingsUI::CUIComboItem* item, const std::string& name) {
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
  this->m_currentPage->Refresh();
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
  this->m_pendingAdapter = SelectedRenderAdapter();
  this->m_adapter = this->m_pendingAdapter;
  DisplayMode mode{static_cast<unsigned>(g_configuredScreenWidth), static_cast<unsigned>(g_configuredScreenHeight), static_cast<unsigned>(g_configuredBitsPerPixel)};
  this->m_pendingMode = std::move(mode);
  this->m_mode = this->m_pendingMode;
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
  this->m_title->SetTooltip([]() {
    constexpr std::uint8_t bytes[] = {0x8A, 0xF1, 0x02, 0x9E, 0x0A, 0x49, 0x18, 0x76, 0xD1, 0x97, 0x42, 0x95, 0x0A, 0x27};
    constexpr EncryptedSource<14> source(bytes);
    thread_local EncryptedName<14, 0x03772763F96D9DA5ULL> name(source);
    return name.Text();
  }());
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
  this->m_text->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x74, 0x16, 0x39, 0x98, 0x44, 0x35};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x49D33529F1556333ULL> name(source);
    return name.Text();
  }()));
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
  this->SetBgmVolume(g_session.GetDefaultOptionValue([]() {
    constexpr std::uint8_t bytes[] = {0xCD, 0xDA, 0x82, 0xD8, 0x2B, 0xAA, 0xC1, 0x00, 0xE2, 0xD8, 0xEF};
    constexpr EncryptedSource<11> source(bytes);
    thread_local EncryptedName<11, 0x75ADC57D87EFBD8FULL> name(source);
    return name.Text();
  }(), 100));
  this->SetBgmEnabled(g_session.GetDefaultOptionValue([]() {
    constexpr std::uint8_t bytes[] = {0x42, 0xD9, 0xF2, 0x40, 0x93};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0xC517FB932D95BB6DULL> name(source);
    return name.Text();
  }(), 1) != 0);
  this->SetEffectVolume(g_session.GetDefaultOptionValue([]() {
    constexpr std::uint8_t bytes[] = {0x8C, 0x93, 0x39, 0xEE, 0xEC, 0xA1, 0xE4, 0x75, 0xA6, 0x99, 0x2A, 0xE6, 0xEA, 0xD5};
    constexpr EncryptedSource<14> source(bytes);
    thread_local EncryptedName<14, 0x23BBD58F8B5FF5C9ULL> name(source);
    return name.Text();
  }(), 100));
  const int effect = g_session.GetDefaultOptionValue([]() {
    constexpr std::uint8_t bytes[] = {0xA8, 0x60, 0x04, 0x0A, 0x57, 0x6D, 0x71};
    constexpr EncryptedSource<7> source(bytes);
    thread_local EncryptedName<7, 0x0B7109397F6B1387ULL> name(source);
    return name.Text();
  }(), 1);
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
      m_adapter(), m_pendingAdapter(), m_mode(), m_pendingMode() {
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
  this->m_optionList->SetScrollBarButtonImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x89, 0xC2, 0xBF, 0x19, 0x07, 0xCF, 0x8C, 0xDC, 0x8F, 0x89, 0xBF, 0x2F, 0x7A, 0xBB, 0x15,
                                   0x62, 0x2A, 0x1F, 0x10, 0x8F, 0xAB, 0x31, 0x3C, 0x61, 0x67, 0x0F, 0x12, 0x93, 0xC7};
    constexpr EncryptedSource<29> source(bytes);
    thread_local EncryptedName<29, 0x114901C7E37F6D49ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB1, 0x2C, 0xAB, 0xF1, 0xD1, 0xC1, 0x98, 0xB0, 0xB7, 0x67, 0xAB, 0xC7, 0xAC, 0xB5, 0x01, 0x0E,
                                     0x12, 0xF1, 0x04, 0x67, 0x7D, 0x3F, 0x39, 0x12, 0x06, 0xED, 0x45, 0x69, 0x7C, 0x7F, 0x5D};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x7D5D0F110B6B8371ULL> name(source);
    return name.Text();
  }()));
  this->m_optionList->SetScrollBarBackground(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x99, 0x28, 0xC3, 0x87, 0x95, 0x05, 0xE2, 0x16, 0x9F, 0x63, 0xC3, 0xB1, 0xE8, 0x71, 0x7B,
                                           0xA8, 0x3A, 0xF5, 0x6C, 0x11, 0x39, 0xFB, 0x4A, 0xB2, 0x3D, 0xA9, 0x61, 0x10, 0x25, 0xCB};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xDB27CB557D038759ULL> name(source);
    return name.Text();
  }()));
  this->m_optionList->SetScrollBarThumbImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x3F, 0x4C, 0x9D, 0xF9, 0x67, 0x61, 0x66, 0xF2, 0x39, 0x07, 0x9D, 0xCF, 0x1A, 0x15, 0xFF, 0x4C, 0x9C,
                                    0x91, 0x32, 0x6F, 0xCB, 0x9F, 0xC1, 0x5E, 0x8D, 0xBC, 0x28, 0x73, 0x89, 0xCD, 0xCE, 0x4F, 0xFF};
    constexpr EncryptedSource<33> source(bytes);
    thread_local EncryptedName<33, 0x3FA3AFA7035DE3FFULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2F, 0x82, 0x97, 0xF7, 0x05, 0x1F, 0x26, 0x4A, 0x29, 0xC9, 0x97, 0xC1, 0x78, 0x6B, 0xBF, 0xF4, 0x8C,
                                       0x5F, 0x38, 0x61, 0xA9, 0xE1, 0x81, 0xE6, 0x9D, 0x72, 0x3A, 0x64, 0xA1, 0xFF, 0x81, 0xEA, 0x9F, 0x2D};
    constexpr EncryptedSource<34> source(bytes);
    thread_local EncryptedName<34, 0x87E3D1C50D572DEFULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4D, 0x12, 0x7B, 0x99, 0x77, 0xF7, 0x7E, 0x9C, 0x4B, 0x59, 0x7B, 0xAF, 0x0A, 0x83, 0xE7, 0x22, 0xEE, 0xCF,
                                       0xD4, 0x0F, 0xDB, 0x09, 0xD9, 0x30, 0xFF, 0xE2, 0xDF, 0x0C, 0xC0, 0x57, 0x95, 0x33, 0xE0, 0xCD, 0xBB};
    constexpr EncryptedSource<35> source(bytes);
    thread_local EncryptedName<35, 0x51BB39B763BBBD8DULL> name(source);
    return name.Text();
  }()));

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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x54, 0xFA, 0xB3, 0xDE, 0xC8, 0x1C, 0x11, 0x86, 0x3C, 0xEA, 0xC7, 0x8B, 0xD1, 0xA1};
    constexpr EncryptedSource<14> source(bytes);
    thread_local EncryptedName<14, 0x4331A17F33095FEBULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x34, 0x52, 0x21, 0x24, 0x94, 0x6F};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xD1056FF94D4D2773ULL> name(source);
    return name.Text();
  }()));
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
  this->m_onButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD7, 0x76, 0xF3, 0x53, 0xB9, 0x29, 0x52, 0xC2, 0xD1, 0x3D, 0xF3, 0x65, 0xC4, 0x5D, 0xCB, 0x7D, 0x76, 0xBD, 0x5A, 0xC6, 0x1B, 0x93, 0xF9, 0x50, 0x78, 0xBF, 0x55, 0x87, 0x1B, 0x8A, 0xE7, 0x0F};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x0F97E779A933D917ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x81, 0x7E, 0x21, 0x19, 0xAB, 0x87, 0x4C, 0xFA, 0x87, 0x35, 0x21, 0x2F, 0xD6, 0xF3, 0xD5, 0x45, 0x20, 0xB5, 0x88, 0x8C, 0x09, 0x3D, 0xE7, 0x68, 0x2E, 0xBF, 0xCF, 0x81, 0x06, 0x39, 0x89};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x3789496BE3E1D141ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0xB7, 0x0C, 0xDF, 0xFC, 0x42, 0xF1, 0x8F, 0x79, 0x9A, 0x02, 0xDD, 0xF3, 0x03, 0xF1, 0x96, 0x67, 0xC5};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xC517FB932D95BB6DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x51, 0xA8, 0x91, 0x36, 0xE4, 0xED, 0xA1, 0xD5, 0x7C, 0xA6, 0x93, 0x39, 0xA5, 0xED, 0xB8, 0xCB, 0x23};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x23BBD58F8B5FF5C9ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(50, 13);
  this->m_onText->Move(127, 12);
  this->m_onText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x3D, 0xAB, 0xAB, 0xB3, 0x88, 0xEB, 0x71};
    constexpr EncryptedSource<7> source(bytes);
    thread_local EncryptedName<7, 0x0B7109397F6B1387ULL> name(source);
    return name.Text();
  }());
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xE2, 0xE8, 0x01, 0x90, 0x0E, 0x27};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x03772763F96D9DA5ULL> name(source);
    return name.Text();
  }()));
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
  this->m_offButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x65, 0x56, 0x23, 0xC4, 0x56, 0xDD, 0x91, 0xC7, 0x48, 0x58, 0x21, 0xCB, 0x17, 0xDD, 0x88, 0xD9, 0x17};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x17A9E5BF39AD4737ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xA7, 0x04, 0x51, 0x5F, 0x61, 0xF1, 0x26, 0xDE, 0xA1, 0x4F, 0x51, 0x69, 0x1C, 0x85, 0xBF, 0x61, 0x06, 0xCF, 0xF8, 0xCA, 0xC3, 0x4B, 0x8D, 0x4C, 0x08, 0xC5, 0xBF, 0xC7, 0xCC, 0x4F, 0xE3};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x13E33FA1A591AB67ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0xBF, 0x0E, 0x19, 0x92, 0x4C, 0x13, 0x73, 0xE1, 0x92, 0x00, 0x1B, 0x9D, 0x0D, 0x13, 0x6A, 0xFF, 0xCD};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xCD8F077123FB7D6FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xEB, 0x22, 0x4D, 0x3A, 0x48, 0xB7, 0xCB, 0x27, 0xC6, 0x2C, 0x4F, 0x35, 0x09, 0xB7, 0xD2, 0x39, 0x99};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x9949BFD527532943ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(222, 12);
  this->m_offText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xED, 0x70, 0x7C, 0xE3, 0x0A, 0xDF, 0x41};
    constexpr EncryptedSource<7> source(bytes);
    thread_local EncryptedName<7, 0xD1413DBB01CD8B51ULL> name(source);
    return name.Text();
  }());
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2E, 0x6A, 0x1B, 0x3C, 0xF0, 0x95};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x637B959D55771F69ULL> name(source);
    return name.Text();
  }()));
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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xC3, 0x18, 0x31, 0xFC, 0x29, 0x0F, 0x69, 0x76, 0xEE, 0xB5};
    constexpr EncryptedSource<10> source(bytes);
    thread_local EncryptedName<10, 0xC399B7091589B50BULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x28, 0xD8, 0x4D, 0x80, 0x52, 0x07};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xB739073FE921AD6FULL> name(source);
    return name.Text();
  }()));
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
  this->m_onButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x8F, 0xE4, 0x73, 0x75, 0xED, 0xDB, 0x84, 0x4A, 0x89, 0xAF, 0x73, 0x43, 0x90, 0xAF, 0x1D, 0xF5, 0x2E, 0x2F, 0xDA, 0xE0, 0x4F, 0x61, 0x2F, 0xD8, 0x20, 0x2D, 0xD5, 0xA1, 0x4F, 0x78, 0x31, 0x87};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x8741152D8FB34B4FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xC7, 0x14, 0x29, 0xB7, 0x39, 0x83, 0x98, 0x58, 0xC1, 0x5F, 0x29, 0x81, 0x44, 0xF7, 0x01, 0xE7, 0x66, 0xDF, 0x80, 0x22, 0x9B, 0x39, 0x33, 0xCA, 0x68, 0xD5, 0xC7, 0x2F, 0x94, 0x3D, 0x5D};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x955D4DF94DE9BB07ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD7, 0x56, 0x8F, 0x43, 0x3B, 0x7B, 0xEA, 0x08, 0xD1, 0x1D, 0x8F, 0x75, 0x46, 0x0F, 0x73, 0xB7, 0x76, 0x9D, 0x26, 0xD6, 0x99, 0xC1, 0x41, 0x9A, 0x78, 0x9F, 0x29, 0x97, 0x99, 0xD8, 0x5F, 0xC5};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xC52FB5FBB94FF917ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x05, 0x62, 0x47, 0x0F, 0x61, 0x6D, 0x22, 0x5E, 0x03, 0x29, 0x47, 0x39, 0x1C, 0x19, 0xBB, 0xE1, 0xA4, 0xA9, 0xEE, 0x9A, 0xC3, 0xD7, 0x89, 0xCC, 0xAA, 0xAB, 0xE1, 0xDB, 0xC3, 0xCE, 0x97, 0x93};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x93E7A3A1F587CDC5ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(150, 13);
  this->m_onText->Move(167, 12);
  this->m_onText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xE7, 0xFD, 0xC4, 0xD1, 0xCB, 0x68, 0x4B, 0xC6, 0x07, 0xB9, 0xF7, 0xD8, 0xE6, 0xC5};
    constexpr EncryptedSource<14> source(bytes);
    thread_local EncryptedName<14, 0x2FF3C5036D070127ULL> name(source);
    return name.Text();
  }());
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xF4, 0x12, 0xAF, 0x3A, 0x94, 0x9B};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x17359BF953C367B3ULL> name(source);
    return name.Text();
  }()));
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
  this->m_offButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x95, 0x52, 0x75, 0x21, 0x07, 0x7F, 0xB0, 0xFE, 0x93, 0x19, 0x75, 0x17, 0x7A, 0x0B, 0x29, 0x41, 0x34, 0x99, 0xDC, 0xB4, 0xA5, 0xC5, 0x1B, 0x6C, 0x3A, 0x9B, 0xD3, 0xF5, 0xA5, 0xDC, 0x05, 0x33};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x3375B1C7DBB5FD55ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xDF, 0x9E, 0x0D, 0x1D, 0x2D, 0xBB, 0xA8, 0xD0, 0xD9, 0xD5, 0x0D, 0x2B, 0x50, 0xCF, 0x31, 0x6F, 0x7E, 0x55, 0xA4, 0x88, 0x8F, 0x01, 0x03, 0x42, 0x70, 0x5F, 0xE3, 0x85, 0x80, 0x05, 0x6D};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x1D6D75EDE7CD311FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x61, 0x92, 0x5F, 0x2D, 0xA3, 0x41, 0xE2, 0x94, 0x67, 0xD9, 0x5F, 0x1B, 0xDE, 0x35, 0x7B, 0x2B, 0xC0, 0x59, 0xF6, 0xB8, 0x01, 0xFB, 0x49, 0x06, 0xCE, 0x5B, 0xF9, 0xF9, 0x01, 0xE2, 0x57, 0x59};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x59278F63D79F3DA1ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x77, 0xA8, 0x97, 0x37, 0xEB, 0x29, 0xEE, 0x4C, 0x71, 0xE3, 0x97, 0x01, 0x96, 0x5D, 0x77, 0xF3, 0xD6, 0x63, 0x3E, 0xA2, 0x49, 0x93, 0x45, 0xDE, 0xD8, 0x61, 0x31, 0xE3, 0x49, 0x8A, 0x5B, 0x81};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x812BE72BCD5707B7ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(287, 12);
  this->m_offText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xCA, 0x0B, 0x8F, 0xF2, 0x61, 0xB4, 0x55, 0x6B, 0xF9, 0x1C, 0xA2, 0x5F};
    constexpr EncryptedSource<12> source(bytes);
    thread_local EncryptedName<12, 0xD3755DD95F47A909ULL> name(source);
    return name.Text();
  }());
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2A, 0x96, 0x29, 0xCA, 0x80, 0xC1};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x31D7C1EDA345E36DULL> name(source);
    return name.Text();
  }()));
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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x92, 0x54, 0x6C, 0x22, 0x86, 0x75, 0x1F, 0x0B, 0xC6, 0x47, 0x7A, 0xA3, 0xFD, 0x0E, 0xFE, 0x6F, 0x23};
    constexpr EncryptedSource<17> source(bytes);
    thread_local EncryptedName<17, 0xCB3FBD4183DB8323ULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x0A, 0xA4, 0xA7, 0xAE, 0x94, 0x77};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x7FCD77F9C7CBD14DULL> name(source);
    return name.Text();
  }()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_adapterCombo = new CUIComboBox<CUIComboItem, RenderAdapter>();
  this->m_adapterCombo->Create(162, 16, 5);
  this->m_adapterCombo->SetButton(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xF9, 0x4E, 0x69, 0x15, 0x21, 0x69, 0x96, 0xCA, 0xFF, 0x05, 0x69, 0x23, 0x5C, 0x1D, 0x0F, 0x65, 0x58, 0x92, 0xC0, 0x8C, 0xBE, 0xCE, 0x3D, 0x73, 0x5C, 0x93, 0xCF, 0x8E, 0x82, 0xC2, 0x0F, 0x73, 0x41, 0x95, 0xCB, 0x80, 0x99, 0xF8, 0x31, 0x73, 0x57, 0xBE, 0xC8, 0xC1, 0x83, 0xCA, 0x23, 0x07};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x0753A7E1EFA9E139ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD9, 0xC2, 0x0F, 0x05, 0x9D, 0xCF, 0x04, 0xB2, 0xDF, 0x89, 0x0F, 0x33, 0xE0, 0xBB, 0x9D, 0x1D, 0x78, 0x1E, 0xA6, 0x9C, 0x02, 0x68, 0xAF, 0x0B, 0x7C, 0x1F, 0xA9, 0x9E, 0x3E, 0x64, 0x9D, 0x0B, 0x61, 0x19, 0xAD, 0x90, 0x25, 0x5E, 0xA3, 0x0B, 0x77, 0x32, 0xAE, 0xD1, 0x3F, 0x6C, 0xB1, 0x7F};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x7FC1015DFFCF6D19ULL> name(source);
    return name.Text();
  }()),
                                  std::string([]() {
    constexpr std::uint8_t bytes[] = {0x97, 0x2C, 0x29, 0x2F, 0x41, 0x0B, 0x88, 0x8A, 0x91, 0x67, 0x29, 0x19, 0x3C, 0x7F, 0x11, 0x25, 0x36, 0xF0, 0x80, 0xB6, 0xDE, 0xAC, 0x23, 0x33, 0x32, 0xF1, 0x8F, 0xB4, 0xE2, 0xA0, 0x11, 0x33, 0x2F, 0xF7, 0x8B, 0xBA, 0xF9, 0x9A, 0x2F, 0x33, 0x39, 0xDC, 0x88, 0xFB, 0xE3, 0xA8, 0x3D, 0x47};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x474DC581D5E98357ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xAD, 0xF0, 0x39, 0xC9, 0x23, 0xB5, 0x90, 0xF2, 0xAB, 0xBB, 0x39, 0xFF, 0x5E, 0xC1, 0x09, 0x5D, 0x0C, 0x2C, 0x90, 0x50, 0xBC, 0x12, 0x3B, 0x4B, 0x08, 0x2D, 0x9F, 0x52, 0x80, 0x1E, 0x09, 0x4B, 0x15, 0x2B, 0x9B, 0x5C, 0x9B, 0x24, 0x37, 0x4B, 0x03, 0x00, 0x98, 0x1D, 0x81, 0x16, 0x25, 0x3F};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x3F557BE333F95F6DULL> name(source);
    return name.Text();
  }()));
  this->m_adapterCombo->Move(150, 9);
  this->m_adapterCombo->SetOnSelect([this](CUIComboItem* item, const RenderAdapter& adapter) { this->OnAdapterSelect(item, adapter); });
  this->m_adapterCombo->SetItemBinder([this](CUIComboItem* item, const RenderAdapter& adapter) {
    SetComboItemAdapterText(item, adapter);
  });
  this->AddChild(this->m_adapterCombo);

  this->m_modeText = new CUIStaticText();
  this->m_modeText->SetSize(130, 13);
  this->m_modeText->Move(19, 44);
  this->m_modeText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xAA, 0xDA, 0x8E, 0xA8, 0x68, 0x81, 0xAF, 0x9C, 0xC3, 0xB6, 0xCA, 0xBC, 0x1A, 0x69, 0x33, 0xE8, 0xDA, 0xA9, 0x39};
    constexpr EncryptedSource<19> source(bytes);
    thread_local EncryptedName<19, 0x5B8F49AF09390D1BULL> name(source);
    return name.Text();
  }());
  this->m_modeText->SetFitText(0);
  this->m_modeText->SetAlign(0, 0);
  this->m_modeText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xA8, 0x56, 0xB9, 0x86, 0x74, 0x51};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x3B915119EFD523EFULL> name(source);
    return name.Text();
  }()));
  this->m_modeText->SetFontHeight(12);
  this->m_modeText->SetBold(1);
  this->m_modeText->SetColor(0);
  this->m_modeText->SetShadow(0, 0xFFFFFF);
  this->m_modeText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_modeText);

  this->m_modeCombo = new CUIComboBox<CUIComboItem, DisplayMode>();
  this->m_modeCombo->Create(162, 16, 5);
  this->m_modeCombo->SetButton(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x5B, 0x02, 0xB5, 0x35, 0x11, 0xF3, 0x28, 0xA0, 0x5D, 0x49, 0xB5, 0x03, 0x6C, 0x87, 0xB1, 0x0F, 0xFA, 0xDE, 0x1C, 0xAC, 0x8E, 0x54, 0x83, 0x19, 0xFE, 0xDF, 0x13, 0xAE, 0xB2, 0x58, 0xB1, 0x19, 0xE3, 0xD9, 0x17, 0xA0, 0xA9, 0x62, 0x8F, 0x19, 0xF5, 0xF2, 0x14, 0xE1, 0xB3, 0x50, 0x9D, 0x6D};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x6DED3DD1CF75AD9BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x01, 0x7C, 0x1B, 0x8D, 0x5B, 0xBB, 0x06, 0x84, 0x07, 0x37, 0x1B, 0xBB, 0x26, 0xCF, 0x9F, 0x2B, 0xA0, 0xA0, 0xB2, 0x14, 0xC4, 0x1C, 0xAD, 0x3D, 0xA4, 0xA1, 0xBD, 0x16, 0xF8, 0x10, 0x9F, 0x3D, 0xB9, 0xA7, 0xB9, 0x18, 0xE3, 0x2A, 0xA1, 0x3D, 0xAF, 0x8C, 0xBA, 0x59, 0xF9, 0x18, 0xB3, 0x49};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x49C3759B77DBD3C1ULL> name(source);
    return name.Text();
  }()),
                               std::string([]() {
    constexpr std::uint8_t bytes[] = {0x3D, 0x52, 0xBD, 0x09, 0x21, 0xDB, 0x3C, 0xD6, 0x3B, 0x19, 0xBD, 0x3F, 0x5C, 0xAF, 0xA5, 0x79, 0x9C, 0x8E, 0x14, 0x90, 0xBE, 0x7C, 0x97, 0x6F, 0x98, 0x8F, 0x1B, 0x92, 0x82, 0x70, 0xA5, 0x6F, 0x85, 0x89, 0x1F, 0x9C, 0x99, 0x4A, 0x9B, 0x6F, 0x93, 0xA2, 0x1C, 0xDD, 0x83, 0x78, 0x89, 0x1B};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x1BF915E1F37DFDFDULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x1F, 0x76, 0x9D, 0x93, 0xD3, 0x5D, 0x3A, 0xCC, 0x19, 0x3D, 0x9D, 0xA5, 0xAE, 0x29, 0xA3, 0x63, 0xBE, 0xAA, 0x34, 0x0A, 0x4C, 0xFA, 0x91, 0x75, 0xBA, 0xAB, 0x3B, 0x08, 0x70, 0xF6, 0xA3, 0x75, 0xA7, 0xAD, 0x3F, 0x06, 0x6B, 0xCC, 0x9D, 0x75, 0xB1, 0x86, 0x3C, 0x47, 0x71, 0xFE, 0x8F, 0x01};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x01FF9313695DD9DFULL> name(source);
    return name.Text();
  }()));
  this->m_modeCombo->Move(150, 41);
  this->m_modeCombo->SetOnSelect([this](CUIComboItem* item, const DisplayMode& mode) { this->OnModeSelect(item, mode); });
  this->m_modeCombo->SetItemBinder([this](CUIComboItem* item, const DisplayMode& mode) {
    SetComboItemModeText(item, mode);
  });
  this->AddChild(this->m_modeCombo);
}

// OFFSET: 0x00A27130
// FUNCTION: fn_15396
void GameSettingsUI::CUIGroupGraphicsAPI::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xEE, 0xE2, 0x18, 0x34, 0x5A, 0x41, 0xD3, 0x8E, 0x0F, 0x7C, 0xAF};
    constexpr EncryptedSource<11> source(bytes);
    thread_local EncryptedName<11, 0xCFF3899D95AF355FULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x62, 0x04, 0xC3, 0xA2, 0x60, 0xF3};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xE52DF30DCBAF7125ULL> name(source);
    return name.Text();
  }()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);

  this->m_combo = new CUIComboBox<CUIComboItem, RenderBackend>();
  this->m_combo->Create(162, 16, 5);
  this->m_combo->SetButton(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x99, 0x18, 0xC7, 0xE9, 0x05, 0xB3, 0xA6, 0x82, 0x9F, 0x53, 0xC7, 0xDF, 0x78, 0xC7, 0x3F, 0x2D, 0x38, 0xC4, 0x6E, 0x70, 0x9A, 0x14, 0x0D, 0x3B, 0x3C, 0xC5, 0x61, 0x72, 0xA6, 0x18, 0x3F, 0x3B, 0x21, 0xC3, 0x65, 0x7C, 0xBD, 0x22, 0x01, 0x3B, 0x37, 0xE8, 0x66, 0x3D, 0xA7, 0x10, 0x13, 0x4F};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x4F637DC51307B759ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x6B, 0x6E, 0x8D, 0xC3, 0xA7, 0xBB, 0x0E, 0xE0, 0x6D, 0x25, 0x8D, 0xF5, 0xDA, 0xCF, 0x97, 0x4F, 0xCA, 0xB2, 0x24, 0x5A, 0x38, 0x1C, 0xA5, 0x59, 0xCE, 0xB3, 0x2B, 0x58, 0x04, 0x10, 0x97, 0x59, 0xD3, 0xB5, 0x2F, 0x56, 0x1F, 0x2A, 0xA9, 0x59, 0xC5, 0x9E, 0x2C, 0x17, 0x05, 0x18, 0xBB, 0x2D};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x2DCB7567394DC1ABULL> name(source);
    return name.Text();
  }()),
                           std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4D, 0x14, 0x0F, 0x81, 0x51, 0x69, 0x5A, 0x7E, 0x4B, 0x5F, 0x0F, 0xB7, 0x2C, 0x1D, 0xC3, 0xD1, 0xEC, 0xC8, 0xA6, 0x18, 0xCE, 0xCE, 0xF1, 0xC7, 0xE8, 0xC9, 0xA9, 0x1A, 0xF2, 0xC2, 0xC3, 0xC7, 0xF5, 0xCF, 0xAD, 0x14, 0xE9, 0xF8, 0xFD, 0xC7, 0xE3, 0xE4, 0xAE, 0x55, 0xF3, 0xCA, 0xEF, 0xB3};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0xB39FA7917BCFBB8DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x37, 0xBC, 0xEB, 0x55, 0x31, 0xC1, 0xD8, 0x40, 0x31, 0xF7, 0xEB, 0x63, 0x4C, 0xB5, 0x41, 0xEF, 0x96, 0x60, 0x42, 0xCC, 0xAE, 0x66, 0x73, 0xF9, 0x92, 0x61, 0x4D, 0xCE, 0x92, 0x6A, 0x41, 0xF9, 0x8F, 0x67, 0x49, 0xC0, 0x89, 0x50, 0x7F, 0xF9, 0x99, 0x4C, 0x4A, 0x81, 0x93, 0x62, 0x6D, 0x8D};
    constexpr EncryptedSource<48> source(bytes);
    thread_local EncryptedName<48, 0x8D1D0FF1AF2B13F7ULL> name(source);
    return name.Text();
  }()));
  this->m_combo->Move(150, 9);
  this->m_combo->SetOnSelect([this](CUIComboItem* item, const RenderBackend& backend) { this->OnSelect(item, backend); });
  this->m_combo->SetItemBinder([this](CUIComboItem* item, const RenderBackend& backend) {
    SetComboItemBackendText(item, backend);
  });
  this->AddChild(this->m_combo);
}

// OFFSET: 0x00A27D00
// FUNCTION: fn_15397
void GameSettingsUI::CUIGroupLogInOut::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(100, 12);
  this->m_title->Move(19, 12);
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xC8, 0x1B, 0x1E, 0x8C, 0xAB, 0x7F, 0x35, 0xB1, 0xB4, 0x6D, 0x1D, 0x5B};
    constexpr EncryptedSource<12> source(bytes);
    thread_local EncryptedName<12, 0x0F15B16B5BAFD57FULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x64, 0x84, 0xBB, 0x7E, 0xBA, 0x49};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x7DE349D717D7F123ULL> name(source);
    return name.Text();
  }()));
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
  this->m_onButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x21, 0x36, 0x47, 0xB5, 0x41, 0x0D, 0xE4, 0xBE, 0x27, 0x7D, 0x47, 0x83, 0x3C, 0x79, 0x7D, 0x01, 0x80, 0xFD, 0xEE, 0x20, 0xE3, 0xB7, 0x4F, 0x2C, 0x8E, 0xFF, 0xE1, 0x61, 0xE3, 0xAE, 0x51, 0x73};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x7321C3814F8799E1ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x8F, 0x0E, 0x0F, 0xCF, 0xA3, 0x19, 0x84, 0xB4, 0x89, 0x45, 0x0F, 0xF9, 0xDE, 0x6D, 0x1D, 0x0B, 0x2E, 0xC5, 0xA6, 0x5A, 0x01, 0xA3, 0x2F, 0x26, 0x20, 0xCF, 0xE1, 0x57, 0x0E, 0xA7, 0x41};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x7941D76335CFA14FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x0D, 0xAC, 0x0D, 0x81, 0x57, 0xF5, 0xD6, 0xFE, 0x0B, 0xE7, 0x0D, 0xB7, 0x2A, 0x81, 0x4F, 0x41, 0xAC, 0x67, 0xA4, 0x14, 0xF5, 0x4F, 0x7D, 0x6C, 0xA2, 0x65, 0xAB, 0x55, 0xF5, 0x56, 0x63, 0x33};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x33133B977BCD03CDULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x11, 0xF4, 0x57, 0x1F, 0xF9, 0x49, 0x2E, 0xE2, 0x17, 0xBF, 0x57, 0x29, 0x84, 0x3D, 0xB7, 0x5D, 0xB0, 0x3F, 0xFE, 0x8A, 0x5B, 0xF3, 0x85, 0x70, 0xBE, 0x3D, 0xF1, 0xCB, 0x5B, 0xEA, 0x9B, 0x2F};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x2FEB8739E5975BD1ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(100, 12);
  this->m_onText->Move(127, 12);
  this->m_onText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xB1, 0xEA, 0xB2, 0xCD, 0x13};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x6BA1C1132F033B75ULL> name(source);
    return name.Text();
  }());
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x68, 0xA2, 0x29, 0x4C, 0x54, 0xAB};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xA12DAB392545D72FULL> name(source);
    return name.Text();
  }()));
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
  this->m_offButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xDD, 0x68, 0x8B, 0x4F, 0x1D, 0xB9, 0x8C, 0xEE, 0xDB, 0x23, 0x8B, 0x79, 0x60, 0xCD, 0x15, 0x51, 0x7C, 0xA3, 0x22, 0xDA, 0xBF, 0x03, 0x27, 0x7C, 0x72, 0xA1, 0x2D, 0x9B, 0xBF, 0x1A, 0x39, 0x23};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x234977DDB54BC71DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x05, 0x82, 0xEB, 0xC5, 0x49, 0xD3, 0xA8, 0x22, 0x03, 0xC9, 0xEB, 0xF3, 0x34, 0xA7, 0x31, 0x9D, 0xA4, 0x49, 0x42, 0x50, 0xEB, 0x69, 0x03, 0xB0, 0xAA, 0x43, 0x05, 0x5D, 0xE4, 0x6D, 0x6D};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xEF6D1D893F2B2DC5ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x9B, 0xFC, 0x9F, 0x27, 0x47, 0x51, 0x66, 0xF4, 0x9D, 0xB7, 0x9F, 0x11, 0x3A, 0x25, 0xFF, 0x4B, 0x3A, 0x37, 0x36, 0xB2, 0xE5, 0xEB, 0xCD, 0x66, 0x34, 0x35, 0x39, 0xF3, 0xE5, 0xF2, 0xD3, 0x39};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x39A39F87DD5F535BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB7, 0x38, 0x47, 0xE7, 0xE3, 0x9D, 0x24, 0xA2, 0xB1, 0x73, 0x47, 0xD1, 0x9E, 0xE9, 0xBD, 0x1D, 0x16, 0xF3, 0xEE, 0x72, 0x41, 0x27, 0x8F, 0x30, 0x18, 0xF1, 0xE1, 0x33, 0x41, 0x3E, 0x91, 0x6F};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x6FE153231D879777ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 12);
  this->m_offText->Move(222, 12);
  this->m_offText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x05, 0xC5, 0x5E, 0x21, 0x03};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x4F99DF03C3EF31B7ULL> name(source);
    return name.Text();
  }());
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB0, 0xBC, 0x0F, 0x00, 0x10, 0x99};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x9171997D6963C9F7ULL> name(source);
    return name.Text();
  }()));
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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xCE, 0xF1, 0x65, 0x6A, 0xA5, 0xD8, 0x95, 0x51, 0xCE, 0xAF, 0x27, 0x11, 0x22, 0x67};
    constexpr EncryptedSource<14> source(bytes);
    thread_local EncryptedName<14, 0xED796785D1DF137FULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4E, 0x52, 0x43, 0xD2, 0xD0, 0x5D};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xD97D5DBDBB2F2709ULL> name(source);
    return name.Text();
  }()));
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
  this->m_highButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xD7, 0xFE, 0xA1, 0x04, 0xC4, 0xB5, 0x5D, 0x93, 0xFA, 0xF0, 0xA3, 0x0B, 0x85, 0xB5, 0x44, 0x8D, 0xA5};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xA5FD29D7AB6DC59FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x83, 0x90, 0xA3, 0x50, 0x66, 0x75, 0x0D, 0x21, 0xAE, 0x9E, 0xA9, 0x17, 0x6B, 0x7A, 0x09, 0x4F};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xF14F79170939C7F1ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x5B, 0xF8, 0xC3, 0x63, 0xA7, 0xCB, 0xC0, 0xC8, 0x5D, 0xB3, 0xC3, 0x55, 0xDA, 0xBF, 0x59, 0x77, 0xFA, 0x33, 0x6A, 0xF6, 0x05, 0x71, 0x6B, 0x5A, 0xF4, 0x31, 0x65, 0xB7, 0x05, 0x68, 0x75, 0x05};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x050505679903579BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x21, 0xD4, 0x39, 0x2D, 0x15, 0xDD, 0xFC, 0x10, 0x27, 0x9F, 0x39, 0x1B, 0x68, 0xA9, 0x65, 0xAF, 0x80, 0x1F, 0x90, 0xB8, 0xB7, 0x67, 0x57, 0x82, 0x8E, 0x1D, 0x9F, 0xF9, 0xB7, 0x7E, 0x49, 0xDD};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xDD3913D5D7F97BE1ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const highButton = this->m_highButton;
    highButton->checked = true;
    highButton->Invalidate();
  }
  this->AddChild(this->m_highButton);
  this->m_highText = new CUIStaticText();
  this->m_highText->SetSize(100, 13);
  this->m_highText->Move(127, 12);
  this->m_highText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x14, 0x01, 0xBF, 0x84, 0x31};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0xBF2B4B31397FF5A7ULL> name(source);
    return name.Text();
  }());
  this->m_highText->SetFitText(0);
  this->m_highText->SetAlign(0, 0);
  this->m_highText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xFA, 0xC8, 0x81, 0x5E, 0xD6, 0xBF};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x83EFBFBB37EDBDBDULL> name(source);
    return name.Text();
  }()));
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
  this->m_normalButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xAD, 0xB4, 0x67, 0xEB, 0xB7, 0xA1, 0x0E, 0x3E, 0xAB, 0xFF, 0x67, 0xDD, 0xCA, 0xD5, 0x97, 0x81, 0x0C, 0x7F, 0xCE, 0x7E, 0x15, 0x1B, 0xA5, 0xAC, 0x02, 0x7D, 0xC1, 0x3F, 0x15, 0x02, 0xBB, 0xF3};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xF3CB6F7711A71B6DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x73, 0xC8, 0x99, 0x75, 0x1F, 0xE9, 0x5E, 0x4E, 0x75, 0x83, 0x99, 0x43, 0x62, 0x9D, 0xC7, 0xF1, 0xD2, 0x03, 0x30, 0xE0, 0xBD, 0x53, 0xF5, 0xDC, 0xDC, 0x09, 0x77, 0xED, 0xB2, 0x57, 0x9B};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x839B27DF8F5967B3ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x9D, 0xA2, 0x89, 0x73, 0x61, 0xB3, 0x28, 0x52, 0x9B, 0xE9, 0x89, 0x45, 0x1C, 0xC7, 0xB1, 0xED, 0x3C, 0x69, 0x20, 0xE6, 0xC3, 0x09, 0x83, 0xC0, 0x32, 0x6B, 0x2F, 0xA7, 0xC3, 0x10, 0x9D, 0x9F};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x9FED7DA189490D5DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xA5, 0xFA, 0x09, 0x85, 0xCD, 0x21, 0xEA, 0x88, 0xA3, 0xB1, 0x09, 0xB3, 0xB0, 0x55, 0x73, 0x37, 0x04, 0x31, 0xA0, 0x10, 0x6F, 0x9B, 0x41, 0x1A, 0x0A, 0x33, 0xAF, 0x51, 0x6F, 0x82, 0x5F, 0x45};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x452FEF0D7FC95565ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const normalButton = this->m_normalButton;
    normalButton->checked = false;
    normalButton->Invalidate();
  }
  this->AddChild(this->m_normalButton);
  this->m_normalText = new CUIStaticText();
  this->m_normalText->SetSize(100, 13);
  this->m_normalText->Move(222, 12);
  this->m_normalText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xDF, 0x4B, 0x12, 0xB6, 0x8D};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x176F138D5DD7F365ULL> name(source);
    return name.Text();
  }());
  this->m_normalText->SetFitText(0);
  this->m_normalText->SetAlign(0, 0);
  this->m_normalText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x86, 0x32, 0xA9, 0x7E, 0x8C, 0x99};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xFBD599E117C547C1ULL> name(source);
    return name.Text();
  }()));
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
  this->m_idleButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD7, 0xB4, 0xED, 0x59, 0x7D, 0x25, 0x84, 0xC2, 0xD1, 0xFF, 0xED, 0x6F, 0x00, 0x51, 0x1D, 0x7D, 0x76, 0x7F, 0x44, 0xCC, 0xDF, 0x9F, 0x2F, 0x50, 0x78, 0x7D, 0x4B, 0x8D, 0xDF, 0x86, 0x31, 0x0F};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x0F41EBBDA32D1B17ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2D, 0xDA, 0xA1, 0xD9, 0x77, 0x2D, 0x94, 0xE0, 0x2B, 0x91, 0xA1, 0xEF, 0x0A, 0x59, 0x0D, 0x5F, 0x8C, 0x11, 0x08, 0x4C, 0xD5, 0x97, 0x3F, 0x72, 0x82, 0x1B, 0x4F, 0x41, 0xDA, 0x93, 0x51};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x2D51E3B7236175EDULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xA3, 0x5A, 0x21, 0x85, 0xFB, 0xEB, 0xD8, 0x06, 0xA5, 0x11, 0x21, 0xB3, 0x86, 0x9F, 0x41, 0xB9, 0x02, 0x91, 0x88, 0x10, 0x59, 0x51, 0x73, 0x94, 0x0C, 0x93, 0x87, 0x51, 0x59, 0x48, 0x6D, 0xCB};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xCB1D253B7FE1F563ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x09, 0xBC, 0x39, 0x89, 0x39, 0x17, 0xD0, 0xBC, 0x0F, 0xF7, 0x39, 0xBF, 0x44, 0x63, 0x49, 0x03, 0xA8, 0x77, 0x90, 0x1C, 0x9B, 0xAD, 0x7B, 0x2E, 0xA6, 0x75, 0x9F, 0x5D, 0x9B, 0xB4, 0x65, 0x71};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x7115D9F973F913C9ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const idleButton = this->m_idleButton;
    idleButton->checked = false;
    idleButton->Invalidate();
  }
  this->AddChild(this->m_idleButton);
  this->m_idleText = new CUIStaticText();
  this->m_idleText->SetSize(100, 13);
  this->m_idleText->Move(317, 12);
  this->m_idleText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xAE, 0xB2, 0x53, 0x94, 0x71};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0xE55723712993051DULL> name(source);
    return name.Text();
  }());
  this->m_idleText->SetFitText(0);
  this->m_idleText->SetAlign(0, 0);
  this->m_idleText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x08, 0x12, 0x81, 0xEC, 0x64, 0x37};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x793F370985ED674FULL> name(source);
    return name.Text();
  }()));
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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xDA, 0xF5, 0x83, 0xE4, 0x2D};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x891B232D09451965ULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xFE, 0x70, 0x1B, 0x26, 0x66, 0x1D};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x758F1D0B4F7705B9ULL> name(source);
    return name.Text();
  }()));
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
  this->m_onButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2F, 0xDE, 0x1F, 0xE9, 0xAD, 0x81, 0x02, 0x20, 0x29, 0x95, 0x1F, 0xDF, 0xD0, 0xF5, 0x9B, 0x9F, 0x8E, 0x15, 0xB6, 0x7C, 0x0F, 0x3B, 0xA9, 0xB2, 0x80, 0x17, 0xB9, 0x3D, 0x0F, 0x22, 0xB7, 0xED};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xEDC74F6D13DF71EFULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x55, 0x20, 0x21, 0xF7, 0x9D, 0xC7, 0xA4, 0x3C, 0x53, 0x6B, 0x21, 0xC1, 0xE0, 0xB3, 0x3D, 0x83, 0xF4, 0xEB, 0x88, 0x62, 0x3F, 0x7D, 0x0F, 0xAE, 0xFA, 0xE1, 0xCF, 0x6F, 0x30, 0x79, 0x61};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xF161095D0DE18F95ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x19, 0x56, 0x21, 0x19, 0xAD, 0xDB, 0x68, 0xE6, 0x1F, 0x1D, 0x21, 0x2F, 0xD0, 0xAF, 0xF1, 0x59, 0xB8, 0x9D, 0x88, 0x8C, 0x0F, 0x61, 0xC3, 0x74, 0xB6, 0x9F, 0x87, 0xCD, 0x0F, 0x78, 0xDD, 0x2B};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x2BAD156DE3E1F9D9ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4F, 0x12, 0x2F, 0x7D, 0xBD, 0x0B, 0x68, 0xB8, 0x49, 0x59, 0x2F, 0x4B, 0xC0, 0x7F, 0xF1, 0x07, 0xEE, 0xD9, 0x86, 0xE8, 0x1F, 0xB1, 0xC3, 0x2A, 0xE0, 0xDB, 0x89, 0xA9, 0x1F, 0xA8, 0xDD, 0x75};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x75ADC57D87EFBD8FULL> name(source);
    return name.Text();
  }()));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(100, 13);
  this->m_onText->Move(127, 12);
  this->m_onText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x37, 0x69, 0x5E, 0xEA, 0x95, 0xBF, 0x50, 0xF9, 0x75, 0xB9, 0x57, 0xFB, 0x08, 0xBA, 0xFF};
    constexpr EncryptedSource<15> source(bytes);
    thread_local EncryptedName<15, 0x39FF7FB50DEB998FULL> name(source);
    return name.Text();
  }());
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD0, 0x84, 0x8D, 0xB4, 0xBC, 0x93};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x2FE993D1DDE1F197ULL> name(source);
    return name.Text();
  }()));
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
  this->m_offButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x0D, 0x5E, 0x59, 0x4F, 0x4D, 0x95, 0xC2, 0x60, 0x0B, 0x15, 0x59, 0x79, 0x30, 0xE1, 0x5B, 0xDF, 0xAC, 0x95, 0xF0, 0xDA, 0xEF, 0x2F, 0x69, 0xF2, 0xA2, 0x97, 0xFF, 0x9B, 0xEF, 0x36, 0x77, 0xAD};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xAD075B8DB599F1CDULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xBF, 0xBA, 0x4D, 0xEB, 0xF7, 0xC1, 0x4E, 0xE2, 0xB9, 0xF1, 0x4D, 0xDD, 0x8A, 0xB5, 0xD7, 0x5D, 0x1E, 0x71, 0xE4, 0x7E, 0x55, 0x7B, 0xE5, 0x70, 0x10, 0x7B, 0xA3, 0x73, 0x5A, 0x7F, 0x8B};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x2F8B0F37118D157FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB5, 0x62, 0x2D, 0xA7, 0xAB, 0xAB, 0x20, 0xF8, 0xB3, 0x29, 0x2D, 0x91, 0xD6, 0xDF, 0xB9, 0x47, 0x14, 0xA9, 0x84, 0x32, 0x09, 0x11, 0x8B, 0x6A, 0x1A, 0xAB, 0x8B, 0x73, 0x09, 0x08, 0x95, 0x35};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x35E5656B5DEDCD75ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x3D, 0x16, 0x7B, 0xDB, 0x5F, 0x33, 0x84, 0x84, 0x3B, 0x5D, 0x7B, 0xED, 0x22, 0x47, 0x1D, 0x3B, 0x9C, 0xDD, 0xD2, 0x4E, 0xFD, 0x89, 0x2F, 0x16, 0x92, 0xDF, 0xDD, 0x0F, 0xFD, 0x90, 0x31, 0x49};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x4941FD9F21BBB9FDULL> name(source);
    return name.Text();
  }()));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(130, 13);
  this->m_offText->Move(127, 36);
  this->m_offText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x3D, 0x04, 0x8D, 0x6E, 0x95, 0x4F, 0x38, 0x27, 0x07, 0xF7, 0xF1, 0x5F, 0x08, 0x4A, 0xB7, 0x57, 0x38, 0x6D, 0x83, 0xA9};
    constexpr EncryptedSource<20> source(bytes);
    thread_local EncryptedName<20, 0xE7978FB5A94DD7FDULL> name(source);
    return name.Text();
  }());
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2C, 0xCC, 0xAB, 0x66, 0x0A, 0x8F};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xB7FD8F670FC7B96BULL> name(source);
    return name.Text();
  }()));
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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xEA, 0x4A, 0x42, 0xC5, 0xFA, 0xD6, 0xFF, 0xD7, 0xE2, 0x6C, 0x59, 0x2B};
    constexpr EncryptedSource<12> source(bytes);
    thread_local EncryptedName<12, 0x6BDF334F2BFDAD51ULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x06, 0x48, 0x3B, 0x1C, 0x54, 0x9F};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x674D9F3975573D41ULL> name(source);
    return name.Text();
  }()));
  this->m_title->SetFontHeight(12);
  this->m_title->SetBold(1);
  this->m_title->SetColor(0);
  this->m_title->SetShadow(0, 0xFFFFFF);
  this->m_title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_title);
  this->m_bgmText = new CUIStaticText();
  this->m_bgmText->SetSize(50, 13);
  this->m_bgmText->Move(110, 12);
  this->m_bgmText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xFA, 0xD1, 0xEB, 0x8D, 0xA7, 0x90, 0x71};
    constexpr EncryptedSource<7> source(bytes);
    thread_local EncryptedName<7, 0xC9712D676B5B3943ULL> name(source);
    return name.Text();
  }());
  this->m_bgmText->SetFitText(0);
  this->m_bgmText->SetAlign(0, 0);
  this->m_bgmText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xFE, 0x58, 0x51, 0x5A, 0xE0, 0x93};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xED15938D333D2DB9ULL> name(source);
    return name.Text();
  }()));
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
  static_cast<CUIHScrollBar*>(this->m_bgmSlider)->SetButtonImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xFB, 0x48, 0xC1, 0xE1, 0x01, 0x6F, 0xEE, 0x70, 0xFD, 0x03, 0xC1, 0xD7, 0x7C, 0x1B, 0x77, 0xCE, 0x58, 0x95, 0x6E, 0x77, 0xAD, 0x90, 0x47, 0xD8, 0x5D, 0x93, 0x2F, 0x79, 0xAC, 0xD1, 0x2B};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xBD2BA1C11B01E73BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD1, 0x4C, 0x93, 0x63, 0x71, 0x93, 0xB0, 0x86, 0xD7, 0x07, 0x93, 0x55, 0x0C, 0xE7, 0x29, 0x38, 0x72, 0x91, 0x3C, 0xF5, 0xDD, 0x6C, 0x07, 0x22, 0x76, 0x8B, 0x27, 0xB7, 0xD3, 0x30, 0x05, 0x4B};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x4B755DB19953E311ULL> name(source);
    return name.Text();
  }()));
  this->m_bgmSlider->SetBackground(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4D, 0x2E, 0xD1, 0x09, 0x73, 0x67, 0xAC, 0x30, 0x4B, 0x65, 0xD1, 0x3F, 0x0E, 0x13, 0x35, 0x8E, 0xEE, 0xF3, 0x7E, 0x9F, 0xDF, 0x98, 0x04, 0x94, 0xE9, 0xAF, 0x73, 0x9E, 0xC3, 0xA9};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xFD69A9B3F311818DULL> name(source);
    return name.Text();
  }()));
  static_cast<CUIHScrollBar*>(this->m_bgmSlider)->SetThumbImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xEB, 0xB0, 0x29, 0x51, 0x01, 0x31, 0xE8, 0xB2, 0xED, 0xFB, 0x29, 0x67, 0x7C, 0x45, 0x71, 0x0C, 0x48, 0x6D, 0x86, 0xC7, 0xAD, 0xCE, 0x4F, 0x1E, 0x59, 0x40, 0x85, 0xCE, 0xA7, 0x8B, 0x03, 0x1D, 0x46, 0x6F, 0xE9};
    constexpr EncryptedSource<35> source(bytes);
    thread_local EncryptedName<35, 0x7F2DFFC1ABE91F2BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xCD, 0xCC, 0x4D, 0xF3, 0x85, 0x9D, 0x4A, 0x14, 0xCB, 0x87, 0x4D, 0xC5, 0xF8, 0xE9, 0xD3, 0xAA, 0x6E, 0x11, 0xE2, 0x65, 0x29, 0x62, 0xED, 0xB8, 0x7F, 0x3C, 0xE0, 0x60, 0x21, 0x7D, 0xED, 0xB4, 0x7D, 0x63};
    constexpr EncryptedSource<34> source(bytes);
    thread_local EncryptedName<34, 0xD98F5345098D630DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x53, 0xDA, 0x1F, 0x15, 0x8D, 0xED, 0xCA, 0x56, 0x55, 0x91, 0x1F, 0x23, 0xF0, 0x99, 0x53, 0xE8, 0xF0, 0x07, 0xB0, 0x83, 0x21, 0x12, 0x6D, 0xFA, 0xE1, 0x2A, 0xAD, 0x86, 0x2A, 0x4B, 0x7B, 0xB5, 0xF1, 0x18, 0xAF, 0xEF};
    constexpr EncryptedSource<36> source(bytes);
    thread_local EncryptedName<36, 0x9B0F234DEFDF7593ULL> name(source);
    return name.Text();
  }()));
  this->AddChild(this->m_bgmSlider);
  this->m_bgmMute = new CUICheckBox();
  this->m_bgmMute->SetSize(10, 10);
  this->m_bgmMute->Move(334, 13);
  this->m_bgmMute->SetOnCheck([this](bool checked) { this->OnBgmMute(checked); });
  this->m_bgmMute->disabled = 0;
  this->m_bgmMute->Invalidate();
  this->m_bgmMute->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xAD, 0x10, 0x15, 0xDD, 0x2B, 0x87, 0x62, 0x1A, 0xAB, 0x5B, 0x15, 0xEB, 0x56, 0xF3, 0xFB, 0xB4, 0x05, 0xDA, 0xB6, 0x4C, 0x89, 0x26, 0xDF, 0x88, 0x5D, 0x91, 0xB7, 0x4A, 0x9B, 0x49};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xD7A749EB27D5BF6DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x99, 0xA2, 0x0B, 0x55, 0xBD, 0xAD, 0x50, 0x7C, 0x9F, 0xE9, 0x0B, 0x63, 0xC0, 0xD9, 0xC9, 0xD2, 0x31, 0x68, 0xA8, 0xC4, 0x1F, 0x0C, 0xED, 0xEE, 0x68, 0x23, 0xA9, 0xC2, 0x0D, 0x63};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xB195637DAFCB0D59ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x95, 0x94, 0x41, 0x0D, 0x8D, 0x25, 0x68, 0x60, 0x93, 0xDF, 0x41, 0x3B, 0xF0, 0x51, 0xF1, 0xCE, 0x3D, 0x5E, 0xE2, 0x9C, 0x2F, 0x84, 0xD5, 0xF2, 0x65, 0x15, 0xE3, 0x9A, 0x3D, 0xEB};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xADADEB4DF7813B55ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x01, 0x36, 0x0D, 0x1F, 0x41, 0xE3, 0x80, 0x14, 0x07, 0x7D, 0x0D, 0x29, 0x3C, 0x97, 0x19, 0xBA, 0xA9, 0xFC, 0xAE, 0x8E, 0xE3, 0x42, 0x3D, 0x86, 0xF1, 0xB7, 0xAF, 0x88, 0xF1, 0x2D};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xD9452D81E5CD99C1ULL> name(source);
    return name.Text();
  }()));
  this->m_bgmMute->checked = false;
  this->m_bgmMute->Invalidate();
  this->AddChild(this->m_bgmMute);
  this->m_bgmMuteText = new CUIStaticText();
  this->m_bgmMuteText->SetSize(50, 13);
  this->m_bgmMuteText->Move(348, 12);
  this->m_bgmMuteText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xE9, 0x43, 0xD0, 0xBB, 0x4F};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0xF715514F5961B75BULL> name(source);
    return name.Text();
  }());
  this->m_bgmMuteText->SetFitText(0);
  this->m_bgmMuteText->SetAlign(0, 0);
  this->m_bgmMuteText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xDE, 0x4C, 0x91, 0x58, 0x38, 0xBB};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x4191BB5531FD3999ULL> name(source);
    return name.Text();
  }()));
  this->m_bgmMuteText->SetFontHeight(12);
  this->m_bgmMuteText->SetBold(0);
  this->m_bgmMuteText->SetColor(0);
  this->m_bgmMuteText->SetShadow(0, 0xFFFFFF);
  this->m_bgmMuteText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->m_bgmMuteText);
  this->m_effectText = new CUIStaticText();
  this->m_effectText->SetSize(50, 13);
  this->m_effectText->Move(110, 36);
  this->m_effectText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x1B, 0x86, 0xDF, 0xEB, 0x69, 0xA2, 0x9D};
    constexpr EncryptedSource<7> source(bytes);
    thread_local EncryptedName<7, 0xB19D1FA9116F39D3ULL> name(source);
    return name.Text();
  }());
  this->m_effectText->SetFitText(0);
  this->m_effectText->SetAlign(0, 0);
  this->m_effectText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xE8, 0x7C, 0x65, 0x8E, 0xAA, 0xC9};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x9569C9C7E70909AFULL> name(source);
    return name.Text();
  }()));
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
  static_cast<CUIHScrollBar*>(this->m_effectSlider)->SetButtonImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xAB, 0xE4, 0x7B, 0xFB, 0x47, 0xA1, 0x6C, 0x7A, 0xAD, 0xAF, 0x7B, 0xCD, 0x3A, 0xD5, 0xF5, 0xC4, 0x08, 0x39, 0xD4, 0x6D, 0xEB, 0x5E, 0xC5, 0xD2, 0x0D, 0x3F, 0x95, 0x63, 0xEA, 0x1F, 0xA9};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xB7A96F8701BB4B6BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x09, 0xAE, 0x2D, 0x15, 0x27, 0xA7, 0xDA, 0x6E, 0x0F, 0xE5, 0x2D, 0x23, 0x5A, 0xD3, 0x43, 0xD0, 0xAA, 0x73, 0x82, 0x83, 0x8B, 0x58, 0x6D, 0xCA, 0xAE, 0x69, 0x99, 0xC1, 0x85, 0x04, 0x6F, 0xA3};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xA31F69E7EFED01C9ULL> name(source);
    return name.Text();
  }()));
  this->m_effectSlider->SetBackground(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x91, 0x58, 0x9B, 0x8D, 0x5D, 0x6F, 0xE2, 0x7C, 0x97, 0x13, 0x9B, 0xBB, 0x20, 0x1B, 0x7B, 0xC2, 0x32, 0x85, 0x34, 0x1B, 0xF1, 0x90, 0x4A, 0xD8, 0x35, 0xD9, 0x39, 0x1A, 0xED, 0xA1};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xB127A19D775BF751ULL> name(source);
    return name.Text();
  }()));
  static_cast<CUIHScrollBar*>(this->m_effectSlider)->SetThumbImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x9D, 0x44, 0xB3, 0xB3, 0x5B, 0x87, 0xA4, 0xC2, 0x9B, 0x0F, 0xB3, 0x85, 0x26, 0xF3, 0x3D, 0x7C, 0x3E, 0x99, 0x1C, 0x25, 0xF7, 0x78, 0x03, 0x6E, 0x2F, 0xB4, 0x1F, 0x2C, 0xFD, 0x3D, 0x4F, 0x6D, 0x30, 0x9B, 0x73};
    constexpr EncryptedSource<35> source(bytes);
    thread_local EncryptedName<35, 0x0F61499B4973EB5DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xBF, 0xF4, 0xB5, 0x5F, 0x67, 0x3B, 0x24, 0x48, 0xB9, 0xBF, 0xB5, 0x69, 0x1A, 0x4F, 0xBD, 0xF6, 0x1C, 0x29, 0x1A, 0xC9, 0xCB, 0xC4, 0x83, 0xE4, 0x0D, 0x04, 0x18, 0xCC, 0xC3, 0xDB, 0x83, 0xE8, 0x0F, 0x5B};
    constexpr EncryptedSource<34> source(bytes);
    thread_local EncryptedName<34, 0x85E1F5A7A5755B7FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xE7, 0x0A, 0x53, 0x65, 0xC1, 0x9D, 0xA4, 0x60, 0xE1, 0x41, 0x53, 0x53, 0xBC, 0xE9, 0x3D, 0xDE, 0x44, 0xD7, 0xFC, 0xF3, 0x6D, 0x62, 0x03, 0xCC, 0x55, 0xFA, 0xE1, 0xF6, 0x66, 0x3B, 0x15, 0x83, 0x45, 0xC8, 0xE3, 0x9F};
    constexpr EncryptedSource<36> source(bytes);
    thread_local EncryptedName<36, 0xAD6153019F93A527ULL> name(source);
    return name.Text();
  }()));
  this->AddChild(this->m_effectSlider);
  this->m_effectMute = new CUICheckBox();
  this->m_effectMute->SetSize(10, 10);
  this->m_effectMute->Move(334, 37);
  this->m_effectMute->SetOnCheck([this](bool checked) { this->OnEffectMute(checked); });
  this->m_effectMute->disabled = 0;
  this->m_effectMute->Invalidate();
  this->m_effectMute->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x99, 0xD0, 0x67, 0x5B, 0xD7, 0x0B, 0x30, 0x72, 0x9F, 0x9B, 0x67, 0x6D, 0xAA, 0x7F, 0xA9, 0xDC, 0x31, 0x1A, 0xC4, 0xCA, 0x75, 0xAA, 0x8D, 0xE0, 0x69, 0x51, 0xC5, 0xCC, 0x67, 0xC5};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xBFF5C517A1A77F59ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xFD, 0xD2, 0x95, 0xDB, 0x01, 0xBB, 0x5C, 0xA8, 0xFB, 0x99, 0x95, 0xED, 0x7C, 0xCF, 0xC5, 0x06, 0x55, 0x18, 0x36, 0x4A, 0xA3, 0x1A, 0xE1, 0x3A, 0x0C, 0x53, 0x37, 0x4C, 0xB1, 0x75};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0x659975C121557D3DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x51, 0xC4, 0xAB, 0xE5, 0x43, 0x05, 0x0A, 0x8C, 0x57, 0x8F, 0xAB, 0xD3, 0x3E, 0x71, 0x93, 0x22, 0xF9, 0x0E, 0x08, 0x74, 0xE1, 0xA4, 0xB7, 0x1E, 0xA1, 0x45, 0x09, 0x72, 0xF3, 0xCB};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0x41CFCB831F6B6B91ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB5, 0x90, 0xA1, 0x51, 0xEF, 0xBF, 0x22, 0x90, 0xB3, 0xDB, 0xA1, 0x67, 0x92, 0xCB, 0xBB, 0x3E, 0x1D, 0x5A, 0x02, 0xC0, 0x4D, 0x1E, 0x9F, 0x02, 0x45, 0x11, 0x03, 0xC6, 0x5F, 0x71};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0x5DE7712FAB613F75ULL> name(source);
    return name.Text();
  }()));
  this->m_effectMute->checked = false;
  this->m_effectMute->Invalidate();
  this->AddChild(this->m_effectMute);
  this->m_effectMuteText = new CUIStaticText();
  this->m_effectMuteText->SetSize(50, 13);
  this->m_effectMuteText->Move(348, 36);
  this->m_effectMuteText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xB3, 0xF9, 0xF6, 0xBF, 0xED};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x81F58DED5D470D01ULL> name(source);
    return name.Text();
  }());
  this->m_effectMuteText->SetFitText(0);
  this->m_effectMuteText->SetAlign(0, 0);
  this->m_effectMuteText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x8C, 0x98, 0xDF, 0x42, 0xEA, 0x31};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x454D31872BB3EDCBULL> name(source);
    return name.Text();
  }()));
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
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xAE, 0x7D, 0xC4, 0xD4, 0x4D, 0x62, 0xD7, 0x83, 0xD5, 0x69, 0x23, 0xD2, 0x23, 0x2A, 0xE4, 0xFA, 0xA6, 0xC7};
    constexpr EncryptedSource<18> source(bytes);
    thread_local EncryptedName<18, 0x4F1791FB1503C713ULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x94, 0x98, 0xFB, 0x7C, 0x20, 0x5B};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xA1A55B4D1597EDD3ULL> name(source);
    return name.Text();
  }()));
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
  static_cast<CUIHScrollBar*>(this->m_slider)->SetButtonImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xC1, 0xF8, 0x57, 0x25, 0xD3, 0x4B, 0x96, 0x1C, 0xC7, 0xB3, 0x57, 0x13, 0xAE, 0x3F, 0x0F, 0xA2, 0x62, 0x25, 0xF8, 0xB3, 0x7F, 0xB4, 0x3F, 0xB4, 0x67, 0x23, 0xB9, 0xBD, 0x7E, 0xF5, 0x53};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0xD1538513DF975701ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x5F, 0x6C, 0x87, 0x47, 0x2B, 0x89, 0x24, 0x5C, 0x59, 0x27, 0x87, 0x71, 0x56, 0xFD, 0xBD, 0xE2, 0xFC, 0xB1, 0x28, 0xD1, 0x87, 0x76, 0x93, 0xF8, 0xF8, 0xAB, 0x33, 0x93, 0x89, 0x2A, 0x91, 0x91};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x91E147EBBD47C39FULL> name(source);
    return name.Text();
  }()));
  this->m_slider->SetBackground(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x45, 0xCE, 0x4B, 0xED, 0xBF, 0xAF, 0x60, 0x62, 0x43, 0x85, 0x4B, 0xDB, 0xC2, 0xDB, 0xF9, 0xDC, 0xE6, 0x13, 0xE4, 0x7B, 0x13, 0x50, 0xC8, 0xC6, 0xE1, 0x4F, 0xE9, 0x7A, 0x0F, 0x61};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0xAFA5617F178B6185ULL> name(source);
    return name.Text();
  }()));
  static_cast<CUIHScrollBar*>(this->m_slider)->SetThumbImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x8D, 0x8A, 0x59, 0xB3, 0x93, 0x8D, 0x44, 0xCA, 0x8B, 0xC1, 0x59, 0x85, 0xEE, 0xF9, 0xDD, 0x74, 0x2E, 0x57, 0xF6, 0x25, 0x3F, 0x72, 0xE3, 0x66, 0x3F, 0x7A, 0xF5, 0x2C, 0x35, 0x37, 0xAF, 0x65, 0x20, 0x55, 0x99};
    constexpr EncryptedSource<35> source(bytes);
    thread_local EncryptedName<35, 0x078143534999254DULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xA1, 0x00, 0x4F, 0xC3, 0x4B, 0x07, 0x3A, 0xDA, 0xA7, 0x4B, 0x4F, 0xF5, 0x36, 0x73, 0xA3, 0x64, 0x02, 0xDD, 0xE0, 0x55, 0xE7, 0xF8, 0x9D, 0x76, 0x13, 0xF0, 0xE2, 0x50, 0xEF, 0xE7, 0x9D, 0x7A, 0x11, 0xAF};
    constexpr EncryptedSource<34> source(bytes);
    thread_local EncryptedName<34, 0x17FFC98B398FAF61ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xF3, 0x3C, 0x8D, 0x6F, 0x5B, 0x5B, 0x9E, 0x56, 0xF5, 0x77, 0x8D, 0x59, 0x26, 0x2F, 0x07, 0xE8, 0x50, 0xE1, 0x22, 0xF9, 0xF7, 0xA4, 0x39, 0xFA, 0x41, 0xCC, 0x3F, 0xFC, 0xFC, 0xFD, 0x2F, 0xB5, 0x51, 0xFE, 0x3D, 0x95};
    constexpr EncryptedSource<36> source(bytes);
    thread_local EncryptedName<36, 0x9B5B959B954D9333ULL> name(source);
    return name.Text();
  }()));
  this->AddChild(this->m_slider);
}

// OFFSET: 0x00A30490
// FUNCTION: fn_15403
void GameSettingsUI::CUIGroupTextureDetailLevel::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0x34, 0x1B, 0x54, 0x07, 0x6E, 0xC9, 0x55, 0xBA, 0x29, 0x78, 0x1A, 0x08, 0x18, 0x0D};
    constexpr EncryptedSource<14> source(bytes);
    thread_local EncryptedName<14, 0x7D750DADBDE9C3F1ULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xF8, 0xC8, 0xB3, 0x56, 0x20, 0x2D};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x33832D4D3FDFBDBFULL> name(source);
    return name.Text();
  }()));
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
  static_cast<CUIHScrollBar*>(this->m_slider)->SetButtonImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x6F, 0xE0, 0xEB, 0x19, 0x1F, 0xDB, 0x96, 0xF2, 0x69, 0xAB, 0xEB, 0x2F, 0x62, 0xAF, 0x0F, 0x4C, 0xCC, 0x3D, 0x44, 0x8F, 0xB3, 0x24, 0x3F, 0x5A, 0xC9, 0x3B, 0x05, 0x81, 0xB2, 0x65, 0x53};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x3F5315DFE32B4FAFULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x29, 0x7C, 0xFD, 0xFD, 0xF5, 0xE7, 0xCE, 0x28, 0x2F, 0x37, 0xFD, 0xCB, 0x88, 0x93, 0x57, 0x96, 0x8A, 0xA1, 0x52, 0x6B, 0x59, 0x18, 0x79, 0x8C, 0x8E, 0xBB, 0x49, 0x29, 0x57, 0x44, 0x7B, 0xE5};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0xE50B2935073DD3E9ULL> name(source);
    return name.Text();
  }()));
  this->m_slider->SetBackground(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x51, 0x6C, 0x7D, 0x9B, 0xA5, 0x5F, 0xF8, 0xF4, 0x57, 0x27, 0x7D, 0xAD, 0xD8, 0x2B, 0x61, 0x4A, 0xF2, 0xB1, 0xD2, 0x0D, 0x09, 0xA0, 0x50, 0x50, 0xF5, 0xED, 0xDF, 0x0C, 0x15, 0x91};
    constexpr EncryptedSource<30> source(bytes);
    thread_local EncryptedName<30, 0x393D916561BDC391ULL> name(source);
    return name.Text();
  }()));
  static_cast<CUIHScrollBar*>(this->m_slider)->SetThumbImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x55, 0x52, 0xD1, 0x49, 0x91, 0xAB, 0x50, 0xFC, 0x53, 0x19, 0xD1, 0x7F, 0xEC, 0xDF, 0xC9, 0x42, 0xF6, 0x8F, 0x7E, 0xDF, 0x3D, 0x54, 0xF7, 0x50, 0xE7, 0xA2, 0x7D, 0xD6, 0x37, 0x11, 0xBB, 0x53, 0xF8, 0x8D, 0x11};
    constexpr EncryptedSource<35> source(bytes);
    thread_local EncryptedName<35, 0x31956551B311FD95ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x19, 0x96, 0xD3, 0x83, 0x0B, 0x4D, 0xB8, 0x32, 0x1F, 0xDD, 0xD3, 0xB5, 0x76, 0x39, 0x21, 0x8C, 0xBA, 0x4B, 0x7C, 0x15, 0xA7, 0xB2, 0x1F, 0x9E, 0xAB, 0x66, 0x7E, 0x10, 0xAF, 0xAD, 0x1F, 0x92, 0xA9, 0x39};
    constexpr EncryptedSource<34> source(bytes);
    thread_local EncryptedName<34, 0xFF7D83CB791339D9ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD5, 0x3C, 0x73, 0x3B, 0x27, 0x8D, 0x94, 0x20, 0xD3, 0x77, 0x73, 0x0D, 0x5A, 0xF9, 0x0D, 0x9E, 0x76, 0xE1, 0xDC, 0xAD, 0x8B, 0x72, 0x33, 0x8C, 0x67, 0xCC, 0xC1, 0xA8, 0x80, 0x2B, 0x25, 0xC3, 0x77, 0xFE, 0xC3, 0xC1};
    constexpr EncryptedSource<36> source(bytes);
    thread_local EncryptedName<36, 0xED5143E7C1B39315ULL> name(source);
    return name.Text();
  }()));
  this->AddChild(this->m_slider);
}

// OFFSET: 0x00A31070
// FUNCTION: fn_15404
void GameSettingsUI::CUIGroupTrilinear::OnCreate(int width, int height) {
  this->m_title = new CUIStaticText();
  this->m_title->SetSize(130, 13);
  this->m_title->Move(19, 12);
  this->m_title->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xAB, 0xCD, 0x83, 0xDE, 0x65, 0xCF, 0x3B, 0x16, 0xD3, 0x8D, 0x15, 0xEA, 0x6F, 0xC6, 0x42, 0x61, 0xD8, 0x63};
    constexpr EncryptedSource<18> source(bytes);
    thread_local EncryptedName<18, 0xD98F03A52D35636DULL> name(source);
    return name.Text();
  }());
  this->m_title->SetFitText(0);
  this->m_title->SetAlign(0, 0);
  this->m_title->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x96, 0x6C, 0xF1, 0xE6, 0x06, 0x8B};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x0F0F8B6B8F9D19D1ULL> name(source);
    return name.Text();
  }()));
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
  this->m_onButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB1, 0x56, 0x99, 0x0F, 0x41, 0xD3, 0xA0, 0x5C, 0xB7, 0x1D, 0x99, 0x39, 0x3C, 0xA7, 0x39, 0xE3, 0x10, 0x9D, 0x30, 0x9A, 0xE3, 0x69, 0x0B, 0xCE, 0x1E, 0x9F, 0x3F, 0xDB, 0xE3, 0x70, 0x15, 0x91};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x91651D81F559F971ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xCB, 0xD4, 0x1F, 0x9B, 0x8B, 0xA9, 0x14, 0xBE, 0xCD, 0x9F, 0x1F, 0xAD, 0xF6, 0xDD, 0x8D, 0x01, 0x6A, 0x1F, 0xB6, 0x0E, 0x29, 0x13, 0xBF, 0x2C, 0x64, 0x15, 0xF1, 0x03, 0x26, 0x17, 0xD1};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x73D1674B61DF7B0BULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x17, 0xE0, 0xC3, 0x6F, 0x19, 0x4B, 0x6A, 0xF2, 0x11, 0xAB, 0xC3, 0x59, 0x64, 0x3F, 0xF3, 0x4D, 0xB6, 0x2B, 0x6A, 0xFA, 0xBB, 0xF1, 0xC1, 0x60, 0xB8, 0x29, 0x65, 0xBB, 0xBB, 0xE8, 0xDF, 0x3F};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x3FAF85D995034FD7ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x79, 0xA2, 0xEF, 0x5B, 0x83, 0x35, 0xCA, 0x84, 0x7F, 0xE9, 0xEF, 0x6D, 0xFE, 0x41, 0x53, 0x3B, 0xD8, 0x69, 0x46, 0xCE, 0x21, 0x8F, 0x61, 0x16, 0xD6, 0x6B, 0x49, 0x8F, 0x21, 0x96, 0x7F, 0x49};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x490FFB43A12F0DB9ULL> name(source);
    return name.Text();
  }()));
  {
    auto* const onButton = this->m_onButton;
    onButton->checked = false;
    onButton->Invalidate();
  }
  this->AddChild(this->m_onButton);
  this->m_onText = new CUIStaticText();
  this->m_onText->SetSize(150, 13);
  this->m_onText->Move(167, 12);
  this->m_onText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xF7, 0xF6, 0x82, 0xED, 0xBD};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0x2D394DBD0F332733ULL> name(source);
    return name.Text();
  }());
  this->m_onText->SetFitText(0);
  this->m_onText->SetAlign(0, 0);
  this->m_onText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x00, 0x54, 0xD7, 0x82, 0x86, 0x77};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x151F77EBEBBB2147ULL> name(source);
    return name.Text();
  }()));
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
  this->m_offButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x21, 0x34, 0x33, 0x81, 0x2D, 0x49, 0x3A, 0xAC, 0x27, 0x7F, 0x33, 0xB7, 0x50, 0x3D, 0xA3, 0x13, 0x80, 0xFF, 0x9A, 0x14, 0x8F, 0xF3, 0x91, 0x3E, 0x8E, 0xFD, 0x95, 0x55, 0x8F, 0xEA, 0x8F, 0x61};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x61FF87ED7BF39BE1ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x3F, 0x2A, 0xE7, 0xF7, 0xA5, 0xF1, 0x5C, 0xB0, 0x39, 0x61, 0xE7, 0xC1, 0xD8, 0x85, 0xC5, 0x0F, 0x9E, 0xE1, 0x4E, 0x62, 0x07, 0x4B, 0xF7, 0x22, 0x90, 0xEB, 0x09, 0x6F, 0x08, 0x4F, 0x99};
    constexpr EncryptedSource<31> source(bytes);
    thread_local EncryptedName<31, 0x7D993F650D2785FFULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xEF, 0x30, 0x73, 0x21, 0x2F, 0x07, 0xD2, 0x56, 0xE9, 0x7B, 0x73, 0x17, 0x52, 0x73, 0x4B, 0xE9, 0x4E, 0xFB, 0xDA, 0xB4, 0x8D, 0xBD, 0x79, 0xC4, 0x40, 0xF9, 0xD5, 0xF5, 0x8D, 0xA4, 0x67, 0x9B};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x9B17C9EFDBB39F2FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x1D, 0x7A, 0x89, 0xBB, 0xB7, 0xDB, 0xFE, 0xE0, 0x1B, 0x31, 0x89, 0x8D, 0xCA, 0xAF, 0x67, 0x5F, 0xBC, 0xB1, 0x20, 0x2E, 0x15, 0x61, 0x55, 0x72, 0xB2, 0xB3, 0x2F, 0x6F, 0x15, 0x78, 0x4B, 0x2D};
    constexpr EncryptedSource<32> source(bytes);
    thread_local EncryptedName<32, 0x2D3B15774149D5DDULL> name(source);
    return name.Text();
  }()));
  {
    auto* const offButton = this->m_offButton;
    offButton->checked = false;
    offButton->Invalidate();
  }
  this->AddChild(this->m_offButton);
  this->m_offText = new CUIStaticText();
  this->m_offText->SetSize(100, 13);
  this->m_offText->Move(287, 12);
  this->m_offText->SetText([]() {
    constexpr std::uint8_t bytes[] = {0xC3, 0xEF, 0x42, 0x1D, 0x87};
    constexpr EncryptedSource<5> source(bytes);
    thread_local EncryptedName<5, 0xC7912587FFF31B71ULL> name(source);
    return name.Text();
  }());
  this->m_offText->SetFitText(0);
  this->m_offText->SetAlign(0, 0);
  this->m_offText->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x2A, 0x46, 0x63, 0xFC, 0xB6, 0x45};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0x69D345DB950F336DULL> name(source);
    return name.Text();
  }()));
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
  Singleton<CGameSettingsMgr>::s_instance->SetOption(this->m_optionId, IsServiceOption(this->m_optionId) ^ checked, false);
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

  GameSettingsOption* option = *found;
  item->m_check->Show(option->type == 0);
  CUICheckBox* check = item->m_check;
  if (check->geometry.visible) {
    check->checked = IsServiceOption(item->m_optionId) ^ g_serviceOptions.Get(item->m_optionId);
    check->Invalidate();
  }
  item->m_runButton->Show(option->type == 1);
  item->m_label->SetText(option->title);
  item->m_label->tooltipText = option->tooltip;
  item->m_label->disabled = option->tipBoxId <= 0;
  item->m_label->Invalidate();
  item->m_label->SetId(option->tipBoxId);
  item->m_description->SetText(option->description);
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
  this->m_optionList->layoutFlag = 1;
  this->m_optionList->Invalidate();
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
        item->m_label->tooltipText = (*option)->tooltip;
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
          item->m_label->tooltipText = (*option)->tooltip;
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
    titleBar->SetImages(std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x09, 0x18, 0xE5, 0x6D, 0x7B, 0xFD, 0x46, 0xCA, 0x0F, 0x53, 0xE5, 0x5B, 0x06, 0x89, 0xDF, 0x65, 0xA8, 0xC4, 0x4C, 0xF4, 0xE4, 0x5A, 0xED, 0x73,
        0xAC, 0xC5, 0x43, 0xF6, 0xD8, 0x56, 0xDF, 0x73, 0xA0, 0xC3, 0x49, 0xF2, 0xD9, 0x52, 0xF1, 0x58, 0xA5, 0xD2, 0x43, 0xE3, 0x95, 0x51, 0xEE, 0x77,
        0xC9};
    constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x078333BB9725B7C9ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x75, 0xDA, 0xA1, 0xA5, 0x3D, 0x95, 0xDE, 0x42, 0x73, 0x91, 0xA1, 0x93, 0x40, 0xE1, 0x47, 0xED, 0xD4, 0x06, 0x08, 0x3C, 0xA2, 0x32, 0x75, 0xFB,
        0xD0, 0x07, 0x07, 0x3E, 0x9E, 0x3E, 0x47, 0xFB, 0xDC, 0x01, 0x0D, 0x3A, 0x9F, 0x3A, 0x69, 0xD0, 0xD8, 0x1C, 0x05, 0x71, 0x9F, 0x36, 0x6B, 0x8F};
    constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x8F1B5BFD5F6175B5ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x03, 0x6C, 0x15, 0x71, 0x9B, 0x25, 0xFA, 0x46, 0x05, 0x27, 0x15, 0x47, 0xE6, 0x51, 0x63, 0xE9, 0xA2, 0xB0, 0xBC, 0xE8, 0x04, 0x82, 0x51, 0xFF,
        0xA6, 0xB1, 0xB3, 0xEA, 0x38, 0x8E, 0x63, 0xFF, 0xAA, 0xB7, 0xB9, 0xEE, 0x39, 0x8A, 0x4D, 0xD4, 0xB1, 0xAA, 0xB2, 0xE3, 0x2F, 0xC5, 0x5D, 0xE6,
        0xB3, 0xC3};
    constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x8B3FEB5B8BD5C3C3ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    title->SetText([]() {
      constexpr std::uint8_t bytes[] = {0x17, 0x67, 0xC5, 0x1A, 0x85, 0xDF, 0x58, 0xCA, 0x03, 0xB3};
    constexpr EncryptedSource<10> source(bytes);
      thread_local EncryptedName<10, 0x0BEB63A5C905B3A7ULL> name(source);
      return name.Text();
    }());
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string([]() {
      constexpr std::uint8_t bytes[] = {0x76, 0x2C, 0x47, 0x68, 0x0C, 0xCD};
    constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x8B65CD61012B5931ULL> name(source);
      return name.Text();
    }()));
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
    baseButton->SetImages(std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x25, 0x20, 0x8D, 0xEF, 0xF7, 0xCF, 0x96, 0x3A, 0x23, 0x6B, 0x8D, 0xD9, 0x8A, 0xBB, 0x0F, 0x95, 0x84, 0xFC, 0x24, 0x76, 0x68, 0x68, 0x3D, 0x83,
        0x80, 0xFD, 0x2B, 0x74, 0x54, 0x64, 0x0F, 0x84, 0x9C, 0xFC, 0x12, 0x77, 0x56, 0x72, 0x36, 0xA8, 0x8A, 0xE9, 0x2B, 0x3B, 0x55, 0x6C, 0x23, 0xF7};
    constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xF7530137154D8FE5ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x01, 0x60, 0x13, 0xC9, 0x57, 0xD3, 0x38, 0xA4, 0x07, 0x2B, 0x13, 0xFF, 0x2A, 0xA7, 0xA1, 0x0B, 0xA0, 0xBC, 0xBA, 0x50, 0xC8, 0x74, 0x93, 0x1D,
        0xA4, 0xBD, 0xB5, 0x52, 0xF4, 0x78, 0xA1, 0x1A, 0xB8, 0xBC, 0x8C, 0x51, 0xF6, 0x6E, 0x98, 0x36, 0xAE, 0xA1, 0xFD, 0x51, 0xFA, 0x6D, 0xFD};
    constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0x69FD1D9733D3CFC1ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x07, 0x62, 0x25, 0xC7, 0xDF, 0xB3, 0xA0, 0x38, 0x01, 0x29, 0x25, 0xF1, 0xA2, 0xC7, 0x39, 0x97, 0xA6, 0xBE, 0x8C, 0x5E, 0x40, 0x14, 0x0B, 0x81,
        0xA2, 0xBF, 0x83, 0x5C, 0x7C, 0x18, 0x39, 0x86, 0xBE, 0xBE, 0xBA, 0x5F, 0x7E, 0x0E, 0x00, 0xAA, 0xA8, 0xA3, 0xCB, 0x5F, 0x72, 0x0D, 0x65};
    constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0xF5657D1F3DE5CDC7ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x23, 0xF8, 0xD5, 0xA7, 0xD7, 0x4F, 0x1E, 0xD2, 0x25, 0xB3, 0xD5, 0x91, 0xAA, 0x3B, 0x87, 0x7D, 0x82, 0x24, 0x7C, 0x3E, 0x48, 0xE8, 0xB5, 0x6B,
        0x86, 0x25, 0x73, 0x3C, 0x74, 0xE4, 0x87, 0x6C, 0x9A, 0x24, 0x4A, 0x3F, 0x76, 0xF2, 0xBE, 0x40, 0x8C, 0x31, 0x73, 0x73, 0x75, 0xEC, 0xAB, 0x1F};
    constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x1FDB81175D1557E3ULL> name(source);
      return name.Text();
    }()));
  }
  this->m_titleBar->AddChild(baseButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(386, 3);
  closeButton->SetOnClick([this]() { this->OnTitleClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    closeButton->SetImages(std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x7F, 0x2A, 0xE7, 0x87, 0x93, 0xED, 0x3E, 0x2E, 0x79, 0x61, 0xE7, 0xB1, 0xEE, 0x99, 0xA7, 0x81, 0xDE, 0xF6, 0x4E, 0x1E, 0x0C, 0x4A, 0x95, 0x97,
        0xDA, 0xF7, 0x41, 0x1C, 0x30, 0x46, 0xA7, 0x90, 0xC6, 0xF6, 0x78, 0x1E, 0x3F, 0x4C, 0x88, 0x86, 0xE0, 0xEA, 0x41, 0x1B, 0x7D, 0x41, 0x96, 0x93,
        0xBF};
    constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0xE3FB23537D2785BFULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x5F, 0x56, 0x81, 0x37, 0x39, 0xA5, 0x62, 0x7A, 0x59, 0x1D, 0x81, 0x01, 0x44, 0xD1, 0xFB, 0xD5, 0xFE, 0x8A, 0x28, 0xAE, 0xA6, 0x02, 0xC9, 0xC3,
        0xFA, 0x8B, 0x27, 0xAC, 0x9A, 0x0E, 0xFB, 0xC4, 0xE6, 0x8A, 0x1E, 0xAE, 0x95, 0x04, 0xD4, 0xD2, 0xC0, 0x96, 0x2F, 0xE3, 0x9B, 0x06, 0xD7, 0xB7};
    constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xB7A76BF9CD41F99FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x17, 0xE8, 0x3D, 0x99, 0xCD, 0xF7, 0xEE, 0xBE, 0x11, 0xA3, 0x3D, 0xAF, 0xB0, 0x83, 0x77, 0x11, 0xB6, 0x34, 0x94, 0x00, 0x52, 0x50, 0x45, 0x07,
        0xB2, 0x35, 0x9B, 0x02, 0x6E, 0x5C, 0x77, 0x00, 0xAE, 0x34, 0xA2, 0x00, 0x61, 0x56, 0x58, 0x16, 0x88, 0x28, 0x93, 0x4D, 0x6F, 0x54, 0x5B, 0x73};
    constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x732B390D63FD47D7ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x51, 0x86, 0xB9, 0xB9, 0x1B, 0xC7, 0x60, 0xBC, 0x57, 0xCD, 0xB9, 0x8F, 0x66, 0xB3, 0xF9, 0x13, 0xF0, 0x5A, 0x10, 0x20, 0x84, 0x60, 0xCB, 0x05,
        0xF4, 0x5B, 0x1F, 0x22, 0xB8, 0x6C, 0xF9, 0x02, 0xE8, 0x5A, 0x26, 0x20, 0xB7, 0x66, 0xD6, 0x14, 0xCE, 0x46, 0x1F, 0x25, 0xF5, 0x6B, 0xC8, 0x01,
        0x91};
    constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x71A509DB43792991ULL> name(source);
      return name.Text();
    }()));
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
    basicButton->SetText(std::string([]() {
      constexpr std::uint8_t bytes[] = {0xCC, 0xB1, 0x35, 0x1A, 0xDF, 0x51, 0xDA, 0x1C, 0xD9, 0x53};
    constexpr EncryptedSource<10> source(bytes);
      thread_local EncryptedName<10, 0xDD69EDFFA18F537DULL> name(source);
      return name.Text();
    }()));
  }
  basicButton->SetFitText(0);
  basicButton->SetAlign(6, 6);
  {
    basicButton->SetFont(std::string([]() {
      constexpr std::uint8_t bytes[] = {0x34, 0x54, 0x8B, 0x2E, 0x46, 0x79};
    constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x8BBF792B47E72173ULL> name(source);
      return name.Text();
    }()));
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
    effectButton->SetText(std::string([]() {
      constexpr std::uint8_t bytes[] = {0x49, 0x17, 0xA5, 0x9C, 0x2D, 0x35, 0xEF, 0x0F, 0x3A, 0x1A, 0xC7, 0x79};
    constexpr EncryptedSource<12> source(bytes);
      thread_local EncryptedName<12, 0xB3CF9BEB7963DB89ULL> name(source);
      return name.Text();
    }()));
  }
  effectButton->SetFitText(0);
  effectButton->SetAlign(6, 6);
  {
    effectButton->SetFont(std::string([]() {
      constexpr std::uint8_t bytes[] = {0x10, 0x8A, 0x81, 0x26, 0xC2, 0xB5};
    constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0xB5D7B5AF4FEDFF57ULL> name(source);
      return name.Text();
    }()));
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
    controlButton->SetText(std::string([]() {
      constexpr std::uint8_t bytes[] = {0xBD, 0x36, 0xD3, 0x9B, 0x1A, 0x80, 0xBD, 0x27, 0xCA, 0x36, 0xB1, 0x35};
    constexpr EncryptedSource<12> source(bytes);
      thread_local EncryptedName<12, 0x9B9D51AD3515F779ULL> name(source);
      return name.Text();
    }()));
  }
  controlButton->SetFitText(0);
  controlButton->SetAlign(6, 6);
  {
    controlButton->SetFont(std::string([]() {
      constexpr std::uint8_t bytes[] = {0xF0, 0x46, 0x49, 0x60, 0xD6, 0x35};
    constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x038535BB092533B7ULL> name(source);
      return name.Text();
    }()));
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
    graphicsButton->SetText(std::string([]() {
      constexpr std::uint8_t bytes[] = {0x62, 0xA0, 0x50, 0xC8, 0x90, 0x0F, 0x0B, 0x47, 0x60, 0xB6, 0x43, 0x69};
    constexpr EncryptedSource<12> source(bytes);
      thread_local EncryptedName<12, 0xFB2BC75769E777D3ULL> name(source);
      return name.Text();
    }()));
  }
  graphicsButton->SetFitText(0);
  graphicsButton->SetAlign(6, 6);
  {
    graphicsButton->SetFont(std::string([]() {
      constexpr std::uint8_t bytes[] = {0xBA, 0xC0, 0x5F, 0x86, 0x3C, 0x37};
    constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x91493751EF33B5FDULL> name(source);
      return name.Text();
    }()));
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
    etcButton->SetText(std::string([]() {
      constexpr std::uint8_t bytes[] = {0xB2, 0xD7, 0x5C, 0x0D, 0x93, 0xCB, 0x7A, 0xAA, 0xA7, 0x35};
    constexpr EncryptedSource<10> source(bytes);
      thread_local EncryptedName<10, 0x6BC977B3B5993503ULL> name(source);
      return name.Text();
    }()));
  }
  etcButton->SetFitText(0);
  etcButton->SetAlign(6, 6);
  {
    etcButton->SetFont(std::string([]() {
      constexpr std::uint8_t bytes[] = {0xB4, 0x60, 0x1F, 0x4A, 0x14, 0xF7};
    constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0xB5F3F779237315F3ULL> name(source);
      return name.Text();
    }()));
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
    button->SetImages(std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x11, 0x76, 0xD9, 0xE3, 0x6B, 0x95, 0x18, 0xAA, 0x17, 0x3D, 0xD9, 0xD5, 0x16, 0xE1, 0x81, 0x05, 0xA5, 0xB7, 0x46, 0x6B, 0xCE, 0x28, 0xB8, 0x13,
        0xFF, 0xBB, 0x74, 0x69, 0xAB};
    constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x67DD5BAB1919D9D1ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xB9, 0x02, 0x47, 0x89, 0xB3, 0x21, 0x26, 0x56, 0xBF, 0x49, 0x47, 0xBF, 0xCE, 0x55, 0xBF, 0xF9, 0x0D, 0xC3, 0xD8, 0x01, 0x16, 0x9C, 0x86, 0xEF,
        0x26, 0xCF, 0xA9, 0x11, 0x1E, 0x9F, 0xE3};
    constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x9BE3EF737387AD79ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xA9, 0x2A, 0x9F, 0xDF, 0x77, 0xB9, 0xAE, 0x74, 0xAF, 0x61, 0x9F, 0xE9, 0x0A, 0xCD, 0x37, 0xDB, 0x1D, 0xEB, 0x00, 0x57, 0xD2, 0x04, 0x0E, 0xCD,
        0x36, 0xE4, 0x71, 0x47, 0xDA, 0x07, 0x6B};
    constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0xB96B77B7255F8569ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x9F, 0xD6, 0x61, 0x1D, 0x31, 0x03, 0x9A, 0x4A, 0x99, 0x9D, 0x61, 0x2B, 0x4C, 0x77, 0x03, 0xE5, 0x2B, 0x17, 0xFE, 0x95, 0x94, 0xBE, 0x3A, 0xF3,
        0x71, 0x1B, 0xCC, 0x97, 0xF1};
    constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x875FCDF1E7A1795FULL> name(source);
      return name.Text();
    }()));
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
    button->SetImages(std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x9B, 0x32, 0xE5, 0xD7, 0x47, 0xE3, 0xE8, 0xCA, 0x9D, 0x79, 0xE5, 0xE1, 0x3A, 0x97, 0x71, 0x65, 0x2F, 0xF3, 0x7A, 0x4C, 0xF7, 0x5D, 0x41, 0x7E,
        0x75, 0xFF, 0x48, 0x5D, 0x87};
    constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x072D2D872D259D5BULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x77, 0xCE, 0x3D, 0x45, 0xF7, 0xE3, 0x4A, 0x50, 0x71, 0x85, 0x3D, 0x73, 0x8A, 0x97, 0xD3, 0xFF, 0xC3, 0x0F, 0xA2, 0xDE, 0x47, 0x5D, 0xE3, 0xE4,
        0xE8, 0x03, 0xD3, 0xDD, 0x5A, 0x5D, 0x8F};
    constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x9D8F2D37BFFD61B7ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xB5, 0xCA, 0xA1, 0x23, 0x8D, 0xC7, 0x1A, 0xB4, 0xB3, 0x81, 0xA1, 0x15, 0xF0, 0xB3, 0x83, 0x1B, 0x01, 0x0B, 0x3E, 0xB8, 0x3D, 0x79, 0xB3, 0x00,
        0x2A, 0x04, 0x4F, 0xBB, 0x20, 0x79, 0xDF};
    constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x79DF094DD9616575ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xC7, 0x72, 0xB7, 0x99, 0xA3, 0xFB, 0x0E, 0xDC, 0xC1, 0x39, 0xB7, 0xAF, 0xDE, 0x8F, 0x97, 0x73, 0x73, 0xB3, 0x28, 0x02, 0x13, 0x45, 0xA7, 0x68,
        0x29, 0xBF, 0x1A, 0x13, 0x63};
    constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x11CB35636377DD07ULL> name(source);
      return name.Text();
    }()));
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
    button->SetImages(std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xF7, 0x52, 0xB5, 0xAF, 0xF5, 0x0F, 0x90, 0x8A, 0xF1, 0x19, 0xB5, 0x99, 0x88, 0x7B, 0x09, 0x25, 0x43, 0x93, 0x2A, 0x36, 0x59, 0xAE, 0x26, 0x22,
        0x19, 0x9F, 0x18, 0x25, 0x35};
    constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x4755C1355575FD37ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xE9, 0x60, 0xE9, 0xFD, 0x23, 0xE7, 0x32, 0xA6, 0xEF, 0x2B, 0xE9, 0xCB, 0x5E, 0x93, 0xAB, 0x09, 0x5D, 0xA1, 0x76, 0x64, 0x8F, 0x46, 0x84, 0x0E,
        0x76, 0xAD, 0x07, 0x65, 0x8E, 0x59, 0xF7};
    constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x6BF729E30729CF29ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0xE7, 0x3C, 0xBF, 0xF5, 0x9B, 0xD1, 0xF2, 0xF0, 0xE1, 0x77, 0xBF, 0xC3, 0xE6, 0xA5, 0x6B, 0x5F, 0x53, 0xFD, 0x20, 0x6C, 0x37, 0x70, 0x44, 0x58,
        0x78, 0xF2, 0x51, 0x6D, 0x36, 0x6F, 0x37};
    constexpr EncryptedSource<31> source(bytes);
      thread_local EncryptedName<31, 0x3D371F5B0F7F9327ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr std::uint8_t bytes[] = {
        0x3D, 0x6E, 0xB3, 0xB9, 0xE7, 0x9F, 0xD8, 0xD2, 0x3B, 0x25, 0xB3, 0x8F, 0x9A, 0xEB, 0x41, 0x7D, 0x89, 0xAF, 0x2C, 0x20, 0x4B, 0x3E, 0x6E, 0x7A,
        0xD3, 0xA3, 0x1E, 0x33, 0x27};
    constexpr EncryptedSource<29> source(bytes);
      thread_local EncryptedName<29, 0x1F1D51274373C1FDULL> name(source);
      return name.Text();
    }()));
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
  this->m_check->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x15, 0x96, 0xEB, 0xB3, 0x27, 0x69, 0x52, 0x68, 0x13, 0xDD, 0xEB, 0x85, 0x5A, 0x1D, 0xCB, 0xE2, 0xB4, 0x54, 0x4E, 0x1A, 0x82, 0xD3, 0xE3, 0xCC, 0xBB, 0x5E, 0x58, 0x1C, 0xAE, 0xFB, 0xE3, 0xCA, 0xB2, 0x5E, 0x47, 0x2C, 0xB8, 0xC8, 0xF1, 0xC3, 0xFB, 0x5B, 0x46, 0x39, 0xE7};
    constexpr EncryptedSource<45> source(bytes);
    thread_local EncryptedName<45, 0xA597A7E7492B39D5ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xB3, 0x40, 0x7B, 0x2B, 0x15, 0x2D, 0x6E, 0x32, 0xB5, 0x0B, 0x7B, 0x1D, 0x68, 0x59, 0xF7, 0xB8, 0x12, 0x82, 0xDE, 0x82, 0xB0, 0x97, 0xDF, 0x96, 0x1D, 0x88, 0xC8, 0x84, 0x9C, 0xBF, 0xDF, 0x90, 0x14, 0x88, 0xD7, 0xB4, 0x8A, 0x8C, 0xC5, 0xD1, 0x11, 0x82, 0xCB, 0xD1};
    constexpr EncryptedSource<44> source(bytes);
    thread_local EncryptedName<44, 0xFFABE3D5D1BBEF73ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0xCF, 0xE0, 0x85, 0x55, 0x5D, 0x8B, 0x5C, 0xCE, 0xC9, 0xAB, 0x85, 0x63, 0x20, 0xFF, 0xC5, 0x44, 0x6E, 0x22, 0x20, 0xFC, 0xF8, 0x31, 0xED, 0x6A, 0x61, 0x28, 0x36, 0xFA, 0xD4, 0x19, 0xED, 0x6C, 0x68, 0x28, 0x29, 0xCA, 0xC2, 0x2A, 0xFF, 0x65, 0x21, 0x2D, 0x28, 0xDF, 0x9D};
    constexpr EncryptedSource<45> source(bytes);
    thread_local EncryptedName<45, 0x0399459DAF454F0FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x6B, 0x00, 0x4D, 0x77, 0x73, 0x43, 0x62, 0x18, 0x6D, 0x4B, 0x4D, 0x41, 0x0E, 0x37, 0xFB, 0x92, 0xCA, 0xC2, 0xE8, 0xDE, 0xD6, 0xF9, 0xD3, 0xBC, 0xC5, 0xC8, 0xFE, 0xD8, 0xFA, 0xD1, 0xD3, 0xBA, 0xCC, 0xC8, 0xE1, 0xE8, 0xEC, 0xE2, 0xC1, 0xB3, 0x85, 0xCD, 0xE0, 0xFD, 0xB3};
    constexpr EncryptedSource<45> source(bytes);
    thread_local EncryptedName<45, 0xD5A78DB38D8DAFABULL> name(source);
    return name.Text();
  }()));
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
  runButton->SetImages(std::string([]() {
    constexpr std::uint8_t bytes[] = {0xD7, 0xFE, 0x21, 0xBB, 0xF9, 0x0D, 0x46, 0x4A, 0xD1, 0xB5, 0x21, 0x8D, 0x84, 0x79, 0xDF, 0xC0, 0x76, 0x3C, 0x84, 0x12, 0x5C, 0xB7, 0xF7, 0xEE, 0x79, 0x36, 0x92, 0x14, 0x70, 0x9F, 0xE1, 0xF3, 0x79, 0x0E, 0x8E, 0x31, 0x4D, 0xAA, 0xEC, 0xE9, 0x76, 0x21, 0x91, 0x2D, 0x40, 0x9C, 0xEC, 0xF2, 0x63, 0x7F, 0x83, 0x2C, 0x49, 0xC3};
    constexpr EncryptedSource<54> source(bytes);
    thread_local EncryptedName<54, 0x8783C33941E15117ULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x5F, 0x72, 0x43, 0xE5, 0xB7, 0x8B, 0xDE, 0xEA, 0x59, 0x39, 0x43, 0xD3, 0xCA, 0xFF, 0x47, 0x60, 0xFE, 0xB0, 0xE6, 0x4C, 0x12, 0x31, 0x6F, 0x4E, 0xF1, 0xBA, 0xF0, 0x4A, 0x3E, 0x19, 0x79, 0x53, 0xF1, 0x82, 0xEC, 0x6F, 0x03, 0x2C, 0x74, 0x49, 0xFE, 0xAD, 0xF3, 0x73, 0x0E, 0x1A, 0x6B, 0x55, 0xFA, 0xAE, 0xF0, 0x31, 0x15, 0x28, 0x6B, 0x27};
    constexpr EncryptedSource<56> source(bytes);
    thread_local EncryptedName<56, 0x271B45771F83DD9FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x4F, 0xF6, 0xA1, 0x11, 0x75, 0xD7, 0xC6, 0x0A, 0x49, 0xBD, 0xA1, 0x27, 0x08, 0xA3, 0x5F, 0x80, 0xEE, 0x34, 0x04, 0xB8, 0xD0, 0x6D, 0x77, 0xAE, 0xE1, 0x3E, 0x12, 0xBE, 0xFC, 0x45, 0x61, 0xB3, 0xE1, 0x06, 0x0E, 0x9B, 0xC1, 0x70, 0x6C, 0xA9, 0xEE, 0x29, 0x11, 0x87, 0xCC, 0x46, 0x6C, 0xB1, 0xEA, 0x2B, 0x4F, 0x89, 0xD8, 0x69, 0x03};
    constexpr EncryptedSource<55> source(bytes);
    thread_local EncryptedName<55, 0xC70319B5EB61598FULL> name(source);
    return name.Text();
  }()), std::string([]() {
    constexpr std::uint8_t bytes[] = {0x17, 0x3A, 0xDF, 0x61, 0x91, 0x8B, 0x42, 0xC6, 0x11, 0x71, 0xDF, 0x57, 0xEC, 0xFF, 0xDB, 0x4C, 0xB6, 0xF8, 0x7A, 0xC8, 0x34, 0x31, 0xF3, 0x62, 0xB9, 0xF2, 0x6C, 0xCE, 0x18, 0x19, 0xE5, 0x7F, 0xB9, 0xCA, 0x70, 0xEB, 0x25, 0x2C, 0xE8, 0x65, 0xB6, 0xE5, 0x6F, 0xF7, 0x28, 0x1A, 0xE3, 0x62, 0xA4, 0xF4, 0x7D, 0xF7, 0x34, 0x6B, 0xE5, 0x66, 0xA7, 0x95};
    constexpr EncryptedSource<58> source(bytes);
    thread_local EncryptedName<58, 0x0B8745519B1F95D7ULL> name(source);
    return name.Text();
  }()));
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
  this->m_label->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x7A, 0x34, 0xE7, 0xA0, 0x94, 0x1B};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xADC51BF9C98B413DULL> name(source);
    return name.Text();
  }()));
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
  this->m_description->SetFont(std::string([]() {
    constexpr std::uint8_t bytes[] = {0x16, 0x80, 0xA3, 0x20, 0x92, 0x17};
    constexpr EncryptedSource<6> source(bytes);
    thread_local EncryptedName<6, 0xDF8117FF49CFF551ULL> name(source);
    return name.Text();
  }()));
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
