#include "Client/Session.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/AdventureGuideMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/TipBoxTitles.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "Render/Renderer.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/Items/ItemDescription.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <cstring>
#include <format>
#include <list>
#include <regex>
#include <utility>

namespace {

template <std::size_t Length>
class EncryptedSource {
public:
  // INLINED
  template <typename Byte>
  constexpr explicit EncryptedSource(const Byte (&source)[Length]) : data{} {
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

  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
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

template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();
template <> __declspec(noinline) void CUIListBox<AdventureGuide::CUISubTab>::SelectItem(CUIListItem* item);
template <> AdventureGuide::CUISubTab* CUIListBox<AdventureGuide::CUISubTab>::CreateItem();
template <> AdventureGuide::CUITab* CUIListBox<AdventureGuide::CUITab>::CreateItem();
template <> void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::AddNewItem();
template <> void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::AddNewItem();
template <> void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::AddNewItem();
template <> void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::AddNewItem();

// OFFSET: 0x009C28B0
// FUNCTION: fn_13993
// OFFSET: 0x009C2980
// FUNCTION: fn_13994
template <class T> void CUIListBox<T>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) {
    int y = 0;
    for (T* item : this->items) {
      if (position <= item->geometry.height + y && y < this->geometry.height + position) {
        item->Show(1);
        item->Move(this->itemX, this->itemY - position + y);
      } else {
        item->Show(0);
      }
      y += item->geometry.height;
    }
  });
  this->AddChild(this->scrollBar);
}

template void CUIListBox<AdventureGuide::CUISubTab>::OnCreate(int, int);
template void CUIListBox<AdventureGuide::CUITab>::OnCreate(int, int);

// OFFSET: 0x009C2A50
// FUNCTION: fn_13995
// OFFSET: 0x009C2B20
// FUNCTION: fn_13996
// OFFSET: 0x009C2BF0
// FUNCTION: fn_13997
// OFFSET: 0x009C2CC0
// FUNCTION: fn_13998
template <class T, class D> void CUIListBoxEx<T, D>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnCreate(int, int);

// OFFSET: 0x009C12B0
// FUNCTION: fn_13987
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::ClearItems() {
  for (AdventureGuide::CUIItemInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C1310
// FUNCTION: fn_13988
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::ClearItems() {
  for (AdventureGuide::CUIMonsterInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C1370
// FUNCTION: fn_13989
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::ClearItems() {
  for (AdventureGuide::CUIQuestInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C13D0
// FUNCTION: fn_13990
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::ClearItems() {
  for (AdventureGuide::CUITabInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C2D90
// FUNCTION: fn_13999
void CUIAdventureGuide::OnCreate(int width, int height) {
  this->background = new CUIStaticImage();
  this->background->SetSize(750, 468);
  this->background->Move(0, 0);
  {
    CUIStaticImage* background = this->background;
    background->SetImage(std::string([]() {
      constexpr char bytes[] = {
        '\x49', '\x74', '\xA3', '\x83', '\x2B', '\x55', '\x0A', '\x7E', '\x4F', '\x3F', '\xA3', '\xB5', '\x56', '\x21', '\x93', '\xF2', '\xED', '\xAD', '\x06',
        '\x17', '\x9F', '\xEE', '\xBD', '\xD6', '\xCE', '\xAE', '\x0A', '\x1D', '\x8E', '\xC7', '\xAD', '\xD4', '\xD6', '\xBC', '\x16', '\x10', '\x8F', '\xFE',
        '\xE1', '\xD1', '\xE4', '\xAB', '\x63'};
      constexpr EncryptedSource<43> source(bytes);
      thread_local EncryptedName<43, 0xB3CF9BEB7963DB89ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->background);

  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(750, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    titleBar->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\x23', '\xF8', '\xD5', '\xA7', '\xD7', '\x4F', '\x1E', '\xD2', '\x25', '\xB3', '\xD5', '\x91', '\xAA', '\x3B', '\x87', '\x7D', '\x82', '\x24', '\x7C',
        '\x3E', '\x48', '\xE8', '\xB5', '\x6B', '\x86', '\x25', '\x73', '\x3C', '\x74', '\xE4', '\x87', '\x6B', '\x8A', '\x23', '\x79', '\x38', '\x75', '\xE0',
        '\xA9', '\x40', '\x8F', '\x32', '\x73', '\x29', '\x39', '\xE3', '\xB6', '\x6F', '\xE3'};
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x1FDB81175D1557E3ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x7F', '\x2A', '\xE7', '\x87', '\x93', '\xED', '\x3E', '\x2E', '\x79', '\x61', '\xE7', '\xB1', '\xEE', '\x99', '\xA7', '\x81', '\xDE', '\xF6', '\x4E',
        '\x1E', '\x0C', '\x4A', '\x95', '\x97', '\xDA', '\xF7', '\x41', '\x1C', '\x30', '\x46', '\xA7', '\x97', '\xD6', '\xF1', '\x4B', '\x18', '\x31', '\x42',
        '\x89', '\xBC', '\xD2', '\xEC', '\x43', '\x53', '\x31', '\x4E', '\x8B', '\xE3'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xE3FB23537D2785BFULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x5F', '\x56', '\x81', '\x37', '\x39', '\xA5', '\x62', '\x7A', '\x59', '\x1D', '\x81', '\x01', '\x44', '\xD1', '\xFB', '\xD5', '\xFE', '\x8A', '\x28',
        '\xAE', '\xA6', '\x02', '\xC9', '\xC3', '\xFA', '\x8B', '\x27', '\xAC', '\x9A', '\x0E', '\xFB', '\xC3', '\xF6', '\x8D', '\x2D', '\xA8', '\x9B', '\x0A',
        '\xD5', '\xE8', '\xED', '\x90', '\x26', '\xA5', '\x8D', '\x45', '\xC5', '\xDA', '\xEF', '\xF9'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xB7A76BF9CD41F99FULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText("");
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x90', '\x32', '\x91', '\x0A', '\x60', '\x39'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x732B390D63FD47D7ULL> name(source);
      return name.Text();
    }()));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    menuButton->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\x47', '\x08', '\x85', '\x89', '\xD5', '\x47', '\xB6', '\xC4', '\x41', '\x43', '\x85', '\xBF', '\xA8', '\x33', '\x2F', '\x6B', '\xE6', '\xD4', '\x2C',
        '\x10', '\x4A', '\xE0', '\x1D', '\x7D', '\xE2', '\xD5', '\x23', '\x12', '\x76', '\xEC', '\x2F', '\x7A', '\xFE', '\xD4', '\x1A', '\x11', '\x74', '\xFA',
        '\x16', '\x56', '\xE8', '\xC1', '\x23', '\x5D', '\x77', '\xE4', '\x03', '\x09'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x097389157345A787ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xF7', '\xDA', '\x6D', '\x5D', '\x03', '\x6F', '\x56', '\x92', '\xF1', '\x91', '\x6D', '\x6B', '\x7E', '\x1B', '\xCF', '\x3D', '\x56', '\x06', '\xC4',
        '\xC4', '\x9C', '\xC8', '\xFD', '\x2B', '\x52', '\x07', '\xCB', '\xC6', '\xA0', '\xC4', '\xCF', '\x2C', '\x4E', '\x06', '\xF2', '\xC5', '\xA2', '\xD2',
        '\xF6', '\x00', '\x58', '\x1B', '\x83', '\xC5', '\xAE', '\xD1', '\x93'};
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0x5F93A1C3A7AD7537ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xDB', '\x66', '\x01', '\x15', '\x11', '\xB3', '\xCA', '\xAC', '\xDD', '\x2D', '\x01', '\x23', '\x6C', '\xC7', '\x53', '\x03', '\x7A', '\xBA', '\xA8',
        '\x8C', '\x8E', '\x14', '\x61', '\x15', '\x7E', '\xBB', '\xA7', '\x8E', '\xB2', '\x18', '\x53', '\x12', '\x62', '\xBA', '\x9E', '\x8D', '\xB0', '\x0E',
        '\x6A', '\x3E', '\x74', '\xA7', '\xEF', '\x8D', '\xBC', '\x0D', '\x0F'};
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0x610F7DD1EFC1C91BULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x49', '\xC4', '\x29', '\x65', '\x8B', '\x91', '\x5C', '\x08', '\x4F', '\x8F', '\x29', '\x53', '\xF6', '\xE5', '\xC5', '\xA7', '\xE8', '\x18', '\x80',
        '\xFC', '\x14', '\x36', '\xF7', '\xB1', '\xEC', '\x19', '\x8F', '\xFE', '\x28', '\x3A', '\xC5', '\xB6', '\xF0', '\x18', '\xB6', '\xFD', '\x2A', '\x2C',
        '\xFC', '\x9A', '\xE6', '\x0D', '\x8F', '\xB1', '\x29', '\x32', '\xE9', '\xC5'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xC5995F4B9FE96B89ULL> name(source);
      return name.Text();
    }()));
  }
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(736, 3);
  closeButton->SetOnClick([this]() { this->OnClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    closeButton->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\xE9', '\x60', '\xE9', '\xFD', '\x23', '\xE7', '\x32', '\xA6', '\xEF', '\x2B', '\xE9', '\xCB', '\x5E', '\x93', '\xAB', '\x09', '\x48', '\xBC', '\x40',
        '\x64', '\xBC', '\x40', '\x99', '\x1F', '\x4C', '\xBD', '\x4F', '\x66', '\x80', '\x4C', '\xAB', '\x18', '\x50', '\xBC', '\x76', '\x64', '\x8F', '\x46',
        '\x84', '\x0E', '\x76', '\xA0', '\x4F', '\x61', '\xCD', '\x4B', '\x9A', '\x1B', '\x29'};
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x6BF729E30729CF29ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xE7', '\x3C', '\xBF', '\xF5', '\x9B', '\xD1', '\xF2', '\xF0', '\xE1', '\x77', '\xBF', '\xC3', '\xE6', '\xA5', '\x6B', '\x5F', '\x46', '\xE0', '\x16',
        '\x6C', '\x04', '\x76', '\x59', '\x49', '\x42', '\xE1', '\x19', '\x6E', '\x38', '\x7A', '\x6B', '\x4E', '\x5E', '\xE0', '\x20', '\x6C', '\x37', '\x70',
        '\x44', '\x58', '\x78', '\xFC', '\x11', '\x21', '\x39', '\x72', '\x47', '\x3D'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x3D371F5B0F7F9327ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x3D', '\x6E', '\xB3', '\xB9', '\xE7', '\x9F', '\xD8', '\xD2', '\x3B', '\x25', '\xB3', '\x8F', '\x9A', '\xEB', '\x41', '\x7D', '\x9C', '\xB2', '\x1A',
        '\x20', '\x78', '\x38', '\x73', '\x6B', '\x98', '\xB3', '\x15', '\x22', '\x44', '\x34', '\x41', '\x6C', '\x84', '\xB2', '\x2C', '\x20', '\x4B', '\x3E',
        '\x6E', '\x7A', '\xA2', '\xAE', '\x1D', '\x6D', '\x45', '\x3C', '\x6D', '\x1F'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x1F1D51274373C1FDULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x53', '\x34', '\xE5', '\x23', '\xE5', '\xE3', '\x68', '\x90', '\x55', '\x7F', '\xE5', '\x15', '\x98', '\x97', '\xF1', '\x3F', '\xF2', '\xE8', '\x4C',
        '\xBA', '\x7A', '\x44', '\xC3', '\x29', '\xF6', '\xE9', '\x43', '\xB8', '\x46', '\x48', '\xF1', '\x2E', '\xEA', '\xE8', '\x7A', '\xBA', '\x49', '\x42',
        '\xDE', '\x38', '\xCC', '\xF4', '\x43', '\xBF', '\x0B', '\x4F', '\xC0', '\x2D', '\x93'};
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x5DAD2D25D9259B93ULL> name(source);
      return name.Text();
    }()));
  }
  this->titleBar->SetSecondButton(closeButton);

  this->characterInfo = new AdventureGuide::CUICharacterInfo();
  this->characterInfo->SetSize(161, 41);
  this->characterInfo->Move(1, 19);
  this->AddChild(this->characterInfo);

  this->titleText = new CUIStaticText();
  this->titleText->SetSize(104, 20);
  this->titleText->Move(362, 28);
  this->titleText->SetText("");
  this->titleText->SetFitText(0);
  this->titleText->SetAlign(6, 6);
  {
    CUIStaticText* titleText = this->titleText;
    titleText->SetFont(std::string([]() {
      constexpr char bytes[] = {'\xF0', '\x14', '\x91', '\xD6', '\x5A', '\x2D'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x9D8F2D37BFFD61B7ULL> name(source);
      return name.Text();
    }()));
  }
  this->titleText->SetFontHeight(14);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x89261B);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->intro = new AdventureGuide::CUIIntro();
  this->intro->SetSize(637, 373);
  this->intro->Move(112, 63);
  this->AddChild(this->intro);

  this->quest = new AdventureGuide::CUIQuest();
  this->quest->SetSize(627, 387);
  this->quest->Move(122, 81);
  this->AddChild(this->quest);

  this->monster = new AdventureGuide::CUIMonster();
  this->monster->SetSize(627, 387);
  this->monster->Move(122, 81);
  this->AddChild(this->monster);

  this->item = new AdventureGuide::CUIItem();
  this->item->SetSize(627, 387);
  this->item->Move(122, 81);
  this->AddChild(this->item);

  this->tabs = new CUIListBox<AdventureGuide::CUITab>();
  this->tabs->SetSize(106, 372);
  this->tabs->Move(1, 63);
  this->tabs->SetOnSelect([this](AdventureGuide::CUITab* tab) { this->OnSelectTab(tab); });
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\xB5', '\xCA', '\xA1', '\x23', '\x8D', '\xC7', '\x1A', '\xB4', '\xB3', '\x81', '\xA1', '\x15', '\xF0', '\xB3', '\x83', '\x38', '\x11', '\x13', '\x04',
        '\xB7', '\x39', '\x7C', '\xAD', '\x1C', '\x32', '\x10', '\x08', '\xBD', '\x28', '\x55', '\xAB', '\x18', '\x17', '\x3A', '\x03', '\xAD', '\x23', '\x56',
        '\xB1', '\x16', '\x07', '\x08', '\x00', '\xB5', '\x63', '\x6B', '\xB2', '\x09', '\x75'};
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x79DF094DD9616575ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xC7', '\x72', '\xB7', '\x99', '\xA3', '\xFB', '\x0E', '\xDC', '\xC1', '\x39', '\xB7', '\xAF', '\xDE', '\x8F', '\x97', '\x50', '\x63', '\xAB', '\x12',
        '\x0D', '\x17', '\x40', '\xB9', '\x74', '\x40', '\xA8', '\x1E', '\x07', '\x06', '\x69', '\xBF', '\x70', '\x65', '\x82', '\x15', '\x17', '\x0D', '\x6A',
        '\xBB', '\x63', '\x62', '\xAE', '\x04', '\x4D', '\x01', '\x58', '\xBB', '\x11'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x11CB35636377DD07ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xF7', '\x52', '\xB5', '\xAF', '\xF5', '\x0F', '\x90', '\x8A', '\xF1', '\x19', '\xB5', '\x99', '\x88', '\x7B', '\x09', '\x06', '\x53', '\x8B', '\x10',
        '\x3B', '\x41', '\xB4', '\x27', '\x22', '\x70', '\x88', '\x1C', '\x31', '\x50', '\x9D', '\x21', '\x26', '\x55', '\xA2', '\x17', '\x21', '\x5B', '\x9E',
        '\x3A', '\x31', '\x52', '\x8F', '\x5B', '\x37', '\x58', '\xB1', '\x55'};
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0x4755C1355575FD37ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xB9', '\x02', '\x47', '\x89', '\xB3', '\x21', '\x26', '\x56', '\xBF', '\x49', '\x47', '\xBF', '\xCE', '\x55', '\xBF', '\xDA', '\x1D', '\xDB', '\xE2',
        '\x1D', '\x07', '\x9A', '\x91', '\xFE', '\x3E', '\xD8', '\xEE', '\x17', '\x16', '\xB3', '\x97', '\xFA', '\x1B', '\xF2', '\xE5', '\x07', '\x1D', '\xB0',
        '\x8D', '\xF4', '\x0B', '\xC0', '\xE6', '\x1F', '\x5D', '\x8D', '\x8E', '\xEB', '\x79'};
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x9BE3EF737387AD79ULL> name(source);
      return name.Text();
    }()));
  }

  this->tabs->itemWidth = 98;
  this->tabs->rowHeight = 40;
  this->tabs->itemX = 8;
  this->tabs->itemY = 0;
  this->tabs->scrollBar->SetSize(8, 372);
  this->tabs->scrollBar->Move(0, 0);
  this->tabs->scrollBar->step = 20;
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->scrollBar->SetButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\xA9', '\x2A', '\x9F', '\xDF', '\x77', '\xB9', '\xAE', '\x74', '\xAF', '\x61', '\x9F', '\xE9', '\x0A', '\xCD', '\x37', '\xF8', '\x0D', '\xF3', '\x3A',
        '\x4B', '\xC3', '\x02', '\x19', '\xDC', '\x2E', '\xF0', '\x36', '\x41', '\xD2', '\x2B', '\x18', '\xDA', '\x1B', '\xEA', '\x33', '\x49', '\xE8', '\x16',
        '\x19', '\xCB', '\x06', '\xF2', '\x00', '\x50', '\xC7', '\x59', '\x09', '\xD4', '\x19', '\x85'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xB96B77B7255F8569ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x9F', '\xD6', '\x61', '\x1D', '\x31', '\x03', '\x9A', '\x4A', '\x99', '\x9D', '\x61', '\x2B', '\x4C', '\x77', '\x03', '\xC6', '\x3B', '\x0F', '\xC4',
        '\x89', '\x85', '\xB8', '\x2D', '\xE2', '\x18', '\x0C', '\xC8', '\x83', '\x94', '\x91', '\x2C', '\xE4', '\x2D', '\x16', '\xCD', '\x8B', '\xAE', '\xAC',
        '\x2D', '\xF5', '\x30', '\x0E', '\xFE', '\x83', '\x9E', '\xBA', '\x31', '\xA9', '\x3D', '\x14', '\xD1', '\xE7'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x875FCDF1E7A1795FULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->scrollBar->SetBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x9B', '\x32', '\xE5', '\xD7', '\x47', '\xE3', '\xE8', '\xCA', '\x9D', '\x79', '\xE5', '\xE1', '\x3A', '\x97', '\x71', '\x46', '\x3F', '\xEB', '\x40',
        '\x43', '\xF3', '\x58', '\x5F', '\x62', '\x1C', '\xE8', '\x4C', '\x49', '\xE2', '\x71', '\x5E', '\x64', '\x29', '\xF2', '\x49', '\x41', '\xD8', '\x4F',
        '\x4A', '\x58', '\x36', '\xF4', '\x41', '\x03', '\xE5', '\x40', '\x5D', '\x07'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x072D2D872D259D5BULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->scrollBar->SetThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x4D', '\xBC', '\xBF', '\xE9', '\xA1', '\x2B', '\xF0', '\x38', '\x4B', '\xF7', '\xBF', '\xDF', '\xDC', '\x5F', '\x69', '\xB4', '\xE9', '\x65', '\x1A',
        '\x7D', '\x15', '\x90', '\x47', '\x90', '\xCA', '\x66', '\x16', '\x77', '\x04', '\xB9', '\x46', '\x96', '\xFF', '\x7C', '\x13', '\x7F', '\x3E', '\x91',
        '\x5D', '\x80', '\xE0', '\x71', '\x20', '\x66', '\x11', '\xCB', '\x57', '\x98', '\xFD', '\x13'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xF535E561137F138DULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x11', '\xCE', '\x31', '\x73', '\xAF', '\x9F', '\x20', '\x1E', '\x17', '\x85', '\x31', '\x45', '\xD2', '\xEB', '\xB9', '\x92', '\xB5', '\x17', '\x94',
        '\xE7', '\x1B', '\x24', '\x97', '\xB6', '\x96', '\x14', '\x98', '\xED', '\x0A', '\x0D', '\x96', '\xB0', '\xA3', '\x0E', '\x9D', '\xE5', '\x30', '\x25',
        '\x8D', '\xA6', '\xBC', '\x03', '\xAE', '\xE4', '\x06', '\x35', '\xCB', '\xB1', '\xBC', '\x11', '\xF1'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xD3E5516F89F161D1ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x97', '\x40', '\x11', '\x4F', '\x6B', '\xB1', '\x38', '\x92', '\x91', '\x0B', '\x11', '\x79', '\x16', '\xC5', '\xA1', '\x1E', '\x33', '\x99', '\xB4',
        '\xDB', '\xDF', '\x0A', '\x8F', '\x3A', '\x10', '\x9A', '\xB8', '\xD1', '\xCE', '\x23', '\x8E', '\x3C', '\x25', '\x80', '\xBD', '\xD9', '\xF4', '\x0B',
        '\x95', '\x2A', '\x3A', '\x8D', '\x8E', '\xD1', '\xC4', '\x08', '\x93', '\x71', '\x35', '\x82', '\xA1', '\xB5'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x5FFD7FABB5D1EF57ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->tabs);
}

// OFFSET: 0x009DBF20
// FUNCTION: fn_45467
void CUIAdventureGuide::OnClose() {
  g_windowMgr.CloseWindow(WID_ADVENTUREGUIDE);
}

// OFFSET: 0x009C14F0
// FUNCTION: fn_13991
void CUIAdventureGuide::Initialize() {
  this->ownedSkinColors = 0;
  this->Resize(750, 468);
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x009DB710
// FUNCTION: fn_14308
void CUIAdventureGuide::OnButtonClick() {
}

// OFFSET: 0x009DBFD0
// FUNCTION: fn_14313
void CRenewQuestMgr::AddStatusRequest(unsigned int id) {
  this->m_statusRequests.insert(id);
}

namespace AdventureGuide {

// OFFSET: 0x009DC0B0
// FUNCTION: fn_14314
void CUITab::OnSelectSubTab(CUISubTab* subTab) {
  if (subTab) {
    this->page->LoadPage(this->type, subTab->id);
  }
}

// OFFSET: 0x009DC0E0
// FUNCTION: fn_14315
void CUIIntro::OnSelectContents(CUITabInfo*, const Guide_Intro::Contents&) {
}

// OFFSET: 0x009DC210
// FUNCTION: fn_14318
void CUIItem::OnSelectItem(CUIItemInfo* item, const CItemData&) {
  if (item) {
    this->navigationTarget = item->description;
  }
  const bool selected = !this->navigationTarget.empty();
  this->findButton->Show(selected);
  this->navigationButton->Show(selected);
}

// OFFSET: 0x009DC270
// FUNCTION: fn_14319
void CUIMonster::OnSelectMonster(CUIMonsterInfo* item, const CMonsterData&) {
  if (item) {
    this->navigationTarget = item->nameText->displayText;
  }
  this->navigationButton->Show(!this->navigationTarget.empty());
}

// OFFSET: 0x009DC2C0
// FUNCTION: fn_14320
void CUIQuest::OnSelectQuest(CUIQuestInfo* item, const CQuestInfo&) {
  if (item) {
    this->navigationTarget = item->title;
  }
  const bool selected = !this->navigationTarget.empty();
  this->findButton->Show(selected);
  this->navigationButton->Show(selected);
}

// OFFSET: 0x009DC320
// FUNCTION: fn_14321
void CUIItem::BindItem(CUIItemInfo* item, const CItemData& data) {
  ITEM_INFO info;
  info.SetItemId(data.id);
  item->location = this->location;
  info.identified = 1;
  item->itemId = data.id;
  const ITEM_DESC& desc = FindItemDescription(std::atoi(info.itemId.c_str()));
  const std::string image = "유저인터페이스\\collection\\" + std::string(desc.identifiedResource) + ".bmp";
  item->nameText->SetText(std::string(desc.identifiedName));
  item->image->SetImage(image);
  reinterpret_cast<CUIQuestInfo*>(item)->SetStatusLines(data.first, data.second, data.third);
  item->SetDescription(data.type, info.Description());
  reinterpret_cast<CUITextViewer*>(item->sourcesViewer)->Clear();
  for (const std::string& value : data.values) {
    reinterpret_cast<CUITextViewer*>(item->sourcesViewer)->AddLine(value);
  }
  item->description = data.text;
}

// OFFSET: 0x009DC770
// FUNCTION: fn_14323
void __stdcall BindQuestInfo(CUIQuestInfo* owner, const CQuestInfo& data) {
  switch (data.type) {
  case 1:
    owner->SetTitle(data.id);
    owner->SetViewerLines(data.rewards);
    owner->title = data.title;
    reinterpret_cast<CUITextViewer*>(owner->statusViewer)->Clear();
    reinterpret_cast<CUITextViewer*>(owner->statusViewer)->AddLine(data.text);
    owner->questId = data.questId;
    {
      const bool isNew = g_renewQuestMgr->IsNewQuest(data.questId);
      owner->stampImage->Show(isNew);
    }
    break;
  case 3:
    owner->SetTitle(data.id);
    owner->SetViewerLines(data.rewards);
    owner->title = data.title;
    reinterpret_cast<CUITextViewer*>(owner->statusViewer)->Clear();
    reinterpret_cast<CUITextViewer*>(owner->statusViewer)->AddLine(data.text);
    owner->stampImage->Show(0);
    return;
  case 4:
    owner->SetTip(data.id);
    owner->SetViewerLines(data.rewards);
    owner->title = data.title;
    reinterpret_cast<CUITextViewer*>(owner->statusViewer)->Clear();
    reinterpret_cast<CUITextViewer*>(owner->statusViewer)->AddLine(data.text);
    owner->stampImage->Show(0);
    return;
  }
}

// OFFSET: 0x009DC8E0
// FUNCTION: fn_14324
void CUIIntro::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DC8F0
// FUNCTION: fn_14325
void CUIItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DC900
// FUNCTION: fn_14326
void CUIMonster::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DC940
// FUNCTION: fn_14328
void CUIQuest::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DCFF0
// FUNCTION: fn_14331
void CUIIntro::OnInit() {
  const std::string& title = g_adventureGuideMgr->introTitle;
  const std::map<int, AdventureGuideText>& texts = g_adventureGuideMgr->introTexts;
  if (texts.empty()) {
    return;
  }

  for (const auto& [id, text] : texts) {
    this->contents->data.emplace_back(text.first, text.second);
  }

  this->contents->UpdateScrollBar();
  this->image->SetImage(title);
  this->contents->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD1C0
// FUNCTION: fn_14333
void CUIItem::OnInit() {
}

// OFFSET: 0x009DD6B0
// FUNCTION: fn_14338
void CUIQuest::OnInit() {
}

// OFFSET: 0x009DD1D0
// FUNCTION: fn_14334
void CUIItemInfo::OnInit() {
  this->descriptionViewer->scrollBar->SetHidden(1);
  this->optionsViewer->scrollBar->SetHidden(1);
  this->sourcesViewer->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DEF40
// FUNCTION: fn_14358
void CUIItemInfo::SetDescription(int type, const std::vector<const char*>& description) {
  reinterpret_cast<CUITextViewer*>(this->descriptionViewer)->Clear();

  if (type == 5) {
    this->type = 5;
  } else if (type == 6) {
    this->type = 4;
  }

  int lines = 0;
  for (const char* value : description) {
    if (lines > 6) {
      break;
    }

    std::string text(value);
    constexpr char highlight[] = {'^', '7', '7', '7', '7', '7', '7'};
    if (text.find(highlight, 0, sizeof(highlight)) == std::string::npos) {
      continue;
    }

    std::cmatch match;
    std::regex pattern("\\^000000");
    std::regex_search(text.c_str(), text.c_str() + text.size(), match, pattern);
    while (!match.empty()) {
      std::string line = match.prefix().str() + match[0].str();
      reinterpret_cast<CUITextViewer*>(this->descriptionViewer)->AddLine(line);
      ++lines;
      text = match.suffix().str();
      text.erase(0, text.find_first_not_of(" \r\n\t\v\f"));
      std::regex_search(text.c_str(), text.c_str() + text.size(), match, pattern);
    }
  }
}

// OFFSET: 0x009DD210
// FUNCTION: fn_14335
void CUIMonster::OnInit() {
  std::list<AdventureGuideEpisode> episodes;
  g_adventureGuideMgr->GetEpisodeList(episodes);
  for (const AdventureGuideEpisode& episode : episodes) {
    CMonsterData data;
    data.name = episode.name;
    data.values = episode.values;
    this->monsterList->data.push_back(data);
  }
  this->monsterList->UpdateScrollBar();
}

// OFFSET: 0x009DD680
// FUNCTION: fn_14337
void CUIMonsterInfo::OnInit() {
  this->detailsViewer->scrollBar->SetHidden(1);
  this->valuesViewer->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD6C0
// FUNCTION: fn_14339
void CUIQuestInfo::OnInit() {
  this->titleViewer->scrollBar->SetHidden(1);
  CUIVScrollBar* first = this->titleViewer->scrollBar;
  first->dragging = 0;
  first->Invalidate();
  this->linesViewer->scrollBar->SetHidden(1);
  CUIVScrollBar* last = this->statusViewer->scrollBar;
  last->dragging = 1;
  last->Invalidate();
}

// OFFSET: 0x009DD720
// FUNCTION: fn_14340
void CUISubTab::OnInit() {
  this->stampImage->Show(0);
}

// OFFSET: 0x009DD730
// FUNCTION: fn_14341
void CUITab::OnInit() {
}

// OFFSET: 0x009DD740
// FUNCTION: fn_14342
void CUITabInfo::OnInit() {
}

// OFFSET: 0x009DC0F0
// FUNCTION: fn_14316
void CUIIntro::BindContents(CUITabInfo* item, const Guide_Intro::Contents& contents) {
  item->titleText->SetText(contents.first);
  item->descriptionText->SetText(contents.second);
}

// OFFSET: 0x009E0350
// FUNCTION: fn_14361
void CUIQuestInfo::SetTitle(int id) {
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->Clear();

  const TipBoxEntry& entry = g_itemTagOwner->GetTip(id);
  if (entry.title.empty() || entry.paragraphs.empty()) {
    reinterpret_cast<CUITextViewer*>(this->titleViewer)->AddLine(std::to_string(id));
    return;
  }

  std::string text = entry.title;
  std::cmatch match;
  std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("Q(.+)\\("));
  const std::string whitespace(" \r\n\t\v\f");
  if (match.size() > 1) {
    text = match[1].str();
  }

  text.erase(0, text.find_first_not_of(whitespace));
  text.erase(text.find_last_not_of(whitespace) + 1);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetBold(1);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->AddLine(text);

  text = entry.paragraphs[0];
  std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("\n"));
  if (match.size() > 0) {
    text = match.prefix().str();
  }

  text.erase(0, text.find_first_not_of(whitespace));
  text.erase(text.find_last_not_of(whitespace) + 1);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->AddLine(text);
}

// OFFSET: 0x009DFAD0
// FUNCTION: fn_14359
void CUIQuestInfo::SetStatusLines(bool first, bool second, bool third) {
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->Clear();
  if (first == true) {
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0xC86464u);
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_ENCHANT_IMPOSSIBLE)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0x6464C8u);
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_GRADE_ENCHANT_POSSIBLE)));
  }
  if (second == true) {
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0xC86464u);
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_GRADE_ENCHANT_IMPOSSIBLE)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0x6464C8u);
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_RUNESYSTEM_EQUIP_OUT)));
  }
  if (third == true) {
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0xC86464u);
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_RUNESYSTEM_EQUIP_NOTPUT)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0x6464C8u);
    reinterpret_cast<CUITextViewer*>(this->statusViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_RUNESYSTEM_RUNESET_TITLE)));
  }
}

// OFFSET: 0x009E07B0
// FUNCTION: fn_14362
void CUIQuestInfo::SetViewerLines(const std::vector<std::string>& lines) {
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->Clear();
  for (const std::string& line : lines) {
    reinterpret_cast<CUITextViewer*>(this->linesViewer)->AddLine(line);
  }
}

// OFFSET: 0x009DDA40
// FUNCTION: fn_14347
void CUIIntro::LoadPage(int, int) {
}

// OFFSET: 0x009DDA50
// FUNCTION: fn_14348
void CUIItem::LoadPage(int type, int id) {
  this->itemList->ClearData();
  this->location = id;
  std::list<std::pair<int, AdventureGuideEntry>> entries;
  g_adventureGuideMgr->GetEntryList(entries, type, id);
  for (const auto& [key, entry] : entries) {
    CItemData data;
    data.id = key;
    data.first = entry.firstFlag;
    data.second = entry.secondFlag;
    data.third = entry.thirdFlag;
    data.categoryId = entry.minLevel;
    data.categoryIndex = entry.maxLevel;
    data.text = entry.text;
    data.values = entry.values;
    data.type = type;
    this->itemList->data.push_back(data);
  }
  this->itemList->UpdateScrollBar();
  this->findButton->Show(0);
  this->navigationButton->Show(0);
}

// OFFSET: 0x009DDE50
// FUNCTION: fn_14350
void CUIMonster::LoadPage(int, int) {
  this->navigationButton->Show(0);
}

// OFFSET: 0x009DDE60
// FUNCTION: fn_14351
void CUIQuest::LoadPage(int type, int id) {
  this->questList->ClearData();
  std::list<std::pair<int, AdventureGuideContents>> contents;
  g_adventureGuideMgr->GetContentsList(contents, type, id);
  for (const auto& [key, content] : contents) {
    CQuestInfo data;
    data.id = key;
    data.categoryType = content.flag;
    data.categoryId = content.minLevel;
    data.categoryIndex = content.maxLevel;
    data.questId = content.questId;
    data.title = content.first;
    data.text = content.second;
    data.rewards = content.values;
    data.type = type;
    this->questList->data.push_back(data);
    if (type == 1 && !g_renewQuestMgr->IsNewQuest(content.questId)) {
      g_renewQuestMgr->AddStatusRequest(content.questId);
    }
  }
  this->questList->UpdateScrollBar();
  if (type == 1) {
    g_renewQuestMgr->SendQuestStatusRequest();
  }
  this->findButton->Show(0);
  this->navigationButton->Show(0);
}

// OFFSET: 0x009DE410
// FUNCTION: fn_14353
void CUITab::SetSelected(bool selected) {
  CUIListItem::SetSelected(selected);
  const bool expand = selected && this->subTabs && !this->subTabs->geometry.visible;
  if (this->subTabs) {
    this->subTabs->Show(expand);
  }
  this->Resize(this->geometry.width, this->collapsedHeight + (expand ? this->subTabs->geometry.height : 0));
  CUIListBox<CUITab>* list = dynamic_cast<CUIListBox<CUITab>*>(this->parent);
  if (list) {
    list->UpdateScrollBar();
  }
  if (this->page) {
    this->page->Show(selected);
  }
}

}

// OFFSET: 0x009DE4C0
// FUNCTION: fn_14354
template <>
void CUIListBox<AdventureGuide::CUISubTab>::SelectItem(CUIListItem* item) {
  if (this->locked) {
    return;
  }
  for (AdventureGuide::CUISubTab* entry : this->items) {
    if (item != entry) {
      entry->SetSelected(false);
    }
  }
  item->SetSelected(true);
  if (this->selected != item) {
    AdventureGuide::CUISubTab* selected = dynamic_cast<AdventureGuide::CUISubTab*>(item);
    if (selected) {
      this->selected = selected;
      if (this->onSelect) {
        this->onSelect(selected);
      }
    }
  }
}

// OFFSET: 0x009DE560
// FUNCTION: fn_14355
template <>
void CUIListBox<AdventureGuide::CUITab>::SelectItem(CUIListItem* item) {
  if (this->locked) {
    return;
  }
  for (AdventureGuide::CUITab* entry : this->items) {
    if (item != entry) {
      entry->SetSelected(false);
    }
  }
  item->SetSelected(true);
  if (this->selected != item) {
    AdventureGuide::CUITab* selected = dynamic_cast<AdventureGuide::CUITab*>(item);
    if (selected) {
      this->selected = selected;
      if (this->onSelect) {
        this->onSelect(selected);
      }
    }
  }
}

// OFFSET: 0x009DC950
// FUNCTION: fn_14329
void CUIAdventureGuide::OnInit() {
  this->titleBar->SetTitle(LookupMessage(MSI_RUNESYSTEM_CLOSE_RODEX));

  CAdventureGuideMgr* mgr = g_adventureGuideMgr;
  if (mgr->m_intros.empty()) {
    return;
  }

  std::map<int, AdventureGuide::CUIPage*> pages = {
      {0, this->intro},
      {1, this->quest},
      {2, this->monster},
      {3, this->quest},
      {4, this->quest},
      {5, this->item},
      {6, this->item},
  };

  const int level = g_session.m_level;
  for (const auto& [type, intro] : mgr->m_intros) {
    AdventureGuide::CUITab* tab = this->tabs->CreateItem();
    tab->type = type;
    tab->labelText->SetText(intro.text);
    tab->title = intro.title;
    tab->collapsedHeight = this->tabs->rowHeight;

    auto page = pages.find(static_cast<int>(tab->type));
    if (pages.end() != page) {
      tab->page = page->second;
    }

    if (!intro.details.empty()) {
      std::map<int, AdventureGuideDetail> details;
      std::copy_if(intro.details.begin(), intro.details.end(), std::inserter(details, details.end()),
                   [type, level](const std::pair<const int, AdventureGuideDetail>& entry) {
                     return g_adventureGuideMgr->IsAvailable(type, entry.first);
                   });
      if (details.empty()) {
        continue;
      }
      CUIListBox<AdventureGuide::CUISubTab>* subTabs = tab->CreateSubTabList();
      subTabs->Resize(subTabs->geometry.width, subTabs->rowHeight * static_cast<int>(details.size()));
      for (const auto& [id, detail] : details) {
        AdventureGuide::CUISubTab* subTab = subTabs->CreateItem();
        subTab->labelText->SetText(detail.text);
        subTab->id = id;
        subTab->questId = detail.questId;
        if (tab->type == 1) {
          const bool isNew = g_renewQuestMgr->IsNewQuest(detail.questId);
          subTab->stampImage->Show(isNew);
          if (!isNew) {
            g_renewQuestMgr->AddStatusRequest(detail.questId);
          }
        }
      }
      tab->Resize(tab->geometry.width, this->tabs->rowHeight + subTabs->geometry.height);
    }
  }

  if (!this->tabs->items.empty()) {
    AdventureGuide::CUITab* first = this->tabs->items.front();
    if (first) {
      this->tabs->SelectItem(first);
    }
  }
  g_renewQuestMgr->SendQuestStatusRequest();
}

// OFFSET: 0x009DE7D0
// FUNCTION: fn_14356
void AdventureGuide::CUIQuestInfo::SetTip(int id) {
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->Clear();

  const TipBoxEntry& tip = g_itemTagOwner->GetTip(id);
  if (tip.title.empty() || tip.paragraphs.empty()) {
    reinterpret_cast<CUITextViewer*>(this->titleViewer)->AddLine(std::to_string(id));
    return;
  }

  std::string title = tip.title;
  std::match_results<const char*> match;
  std::regex_search(title.c_str(), title.c_str() + title.size(), match, std::regex("\\#"));
  const std::string whitespace(" \r\n\t\v\f");
  if (!match.empty()) {
    title = match.prefix().str();
  }

  title.erase(0, title.find_first_not_of(whitespace));
  title.erase(title.find_last_not_of(whitespace) + 1);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetBold(true);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->AddLine(title);

  title = tip.paragraphs.front();
  std::regex_search(title.c_str(), title.c_str() + title.size(), match, std::regex("\n"));
  if (!match.empty()) {
    title = match.prefix().str();
  }
  title.erase(0, title.find_first_not_of(whitespace));
  title.erase(title.find_last_not_of(whitespace) + 1);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetBold(false);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->AddLine(title);
}

// OFFSET: 0x009DEBF0
// FUNCTION: fn_14357
void CUIAdventureGuide::MarkQuest(int questId) {
  for (AdventureGuide::CUITab* tab : this->tabs->items) {
    if (tab->type != 1) {
      continue;
    }

    if (tab->subTabs) {
      std::list<AdventureGuide::CUISubTab*> subTabs = tab->subTabs->items;
      auto found = std::find_if(subTabs.begin(), subTabs.end(), [questId](AdventureGuide::CUISubTab* subTab) {
        return subTab->questId == static_cast<unsigned int>(questId);
      });
      if (found != subTabs.end()) {
        (*found)->stampImage->Show(1);
      }
    }

    AdventureGuide::CUIQuest* quest = dynamic_cast<AdventureGuide::CUIQuest*>(tab->page);
    if (quest) {
      std::vector<AdventureGuide::CUIQuestInfo*> items = quest->questList->items;
      auto found = std::find_if(items.begin(), items.end(), [questId](AdventureGuide::CUIQuestInfo* item) {
        return item->questId == questId;
      });
      if (found != items.end()) {
        (*found)->stampImage->Show(1);
      }
    }
  }
}

// OFFSET: 0x009DC140
// FUNCTION: fn_14317
void CUIAdventureGuide::OnSelectTab(AdventureGuide::CUITab* tab) {
  this->titleText->SetText(tab->title);
  AdventureGuide::CUIPage* page = tab->page;
  if (!page) {
    return;
  }

  const int type = tab->type;
  CUIListBox<AdventureGuide::CUISubTab>* subTabs = tab->subTabs;
  if (subTabs) {
    if (!subTabs->items.empty()) {
      AdventureGuide::CUISubTab* first = subTabs->items.front();
      if (first) {
        subTabs->SelectItem(first);
        if (first != subTabs->selected) {
          return;
        }
      }
    }

    if (subTabs->selected) {
      page->LoadPage(type, subTabs->selected->id);
    } else {
      page->LoadPage(type, 0);
    }
  } else {
    page->LoadPage(type, 0);
  }
}

namespace AdventureGuide {

// OFFSET: 0x009C5400
// FUNCTION: fn_14000
void CUICharacterInfo::OnCreate(int width, int height) {
  this->jobIcon = new CUIStaticImage();
  this->jobIcon->SetSize(25, 25);
  this->jobIcon->Move(10, 8);
  this->jobIcon->SetImage(std::string(""));
  this->AddChild(this->jobIcon);

  this->level = new CUIStaticText();
  this->level->SetSize(50, 12);
  this->level->Move(46, 6);
  this->level->SetText("");
  this->level->SetFitText(0);
  this->level->SetAlign(0, 0);
  this->level->SetFont(std::string([]() {
    constexpr char bytes[] = {'\xFE', '\x70', '\x1B', '\x26', '\x66', '\x1D'};
    constexpr EncryptedSource<6> source(bytes);
    // GLOBAL: gl_25126
    thread_local EncryptedName<6, 0x758F1D0B4F7705B9ULL> levelFont(source);
    return levelFont.Text();
  }()));
  this->level->SetFontHeight(12);
  this->level->SetBold(0);
  this->level->SetColor(0);
  this->level->SetShadow(1, 0xFFFFFF);
  this->level->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->level);

  this->jobLevel = new CUIStaticText();
  this->jobLevel->SetSize(50, 12);
  this->jobLevel->Move(97, 6);
  this->jobLevel->SetText("");
  this->jobLevel->SetFitText(0);
  this->jobLevel->SetAlign(0, 0);
  this->jobLevel->SetFont(std::string([]() {
    constexpr char bytes[] = {'\x70', '\x32', '\xC1', '\x50', '\xD2', '\xE5'};
    constexpr EncryptedSource<6> source(bytes);
    // GLOBAL: gl_25127
    thread_local EncryptedName<6, 0x17A9E5BF39AD4737ULL> jobLevelFont(source);
    return jobLevelFont.Text();
  }()));
  this->jobLevel->SetFontHeight(12);
  this->jobLevel->SetBold(0);
  this->jobLevel->SetColor(0);
  this->jobLevel->SetShadow(1, 0xFFFFFF);
  this->jobLevel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->jobLevel);

  this->name = new CUIStaticText();
  this->name->SetSize(100, 12);
  this->name->Move(45, 22);
  this->name->SetText("");
  this->name->SetFitText(1);
  this->name->SetAlign(0, 0);
  this->name->SetFont(std::string([]() {
    constexpr char bytes[] = {'\x20', '\xDE', '\xFD', '\xCC', '\xCC', '\x3F'};
    constexpr EncryptedSource<6> source(bytes);
    // GLOBAL: gl_25128
    thread_local EncryptedName<6, 0x13E33FA1A591AB67ULL> nameFont(source);
    return nameFont.Text();
  }()));
  this->name->SetFontHeight(12);
  this->name->SetBold(0);
  this->name->SetColor(0);
  this->name->SetShadow(1, 0xFFFFFF);
  this->name->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->name);
}

// OFFSET: 0x009C8D00
// FUNCTION: fn_14003
void CUIItemInfo::OnCreate(int width, int height) {
  this->image = new CUIStaticImage();
  this->image->SetSize(75, 100);
  this->image->Move(45, 14);
  this->image->SetImage(std::string(""));
  this->AddChild(this->image);

  this->nameText = new CUIStaticText();
  this->nameText->SetSize(280, 12);
  this->nameText->Move(165, 13);
  this->nameText->SetText("");
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 0);
  {
    CUIStaticText* name = this->nameText;
    name->SetFont(std::string([]() {
      constexpr char bytes[] = {'\xA2', '\x0E', '\x5F', '\x40', '\x20', '\x65'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x192D654D29337BE5ULL> name(source);
      return name.Text();
    }()));
  }
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(1);
  this->nameText->SetColor(0xCE6B52);
  this->nameText->SetShadow(0, 0xFFFFFF);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);

  CUITextViewer* description = new CUITextViewer();
  this->descriptionViewer = reinterpret_cast<CUIStaticText*>(description);
  description->SetSize(173, 97);
  description->Move(165, 26);
  description->SetTextOffset(0, 6);
  {
    description->SetFont(std::string([]() {
      constexpr char bytes[] = {'\xA4', '\xA0', '\x3F', '\x9E', '\xC8', '\x07'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0xB3E307A5F753D5E3ULL> name(source);
      return name.Text();
    }()));
  }
  description->SetFontHeight(12);
  description->SetBold(0);
  description->SetTextColor(0);
  description->SetOutline(0, 0xFFFF00FF);
  description->SetLineSpacing(6);
  description->SetBackgroundColor(0xFFFF00FF);
  description->SetPadding(0, 0, 0, 0);
  description->SetScrollBarSize(8, 97);
  description->SetScrollStep(20);
  {
    description->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\x99', '\x28', '\xC3', '\x87', '\x95', '\x05', '\xE2', '\x16', '\x9F', '\x63', '\xC3', '\xB1', '\xE8', '\x71', '\x7B', '\x9A', '\x3D', '\xF1', '\x66',
        '\x13', '\x21', '\xBE', '\x55', '\xBE', '\x1E', '\xF2', '\x6A', '\x19', '\x30', '\x97', '\x54', '\xB8', '\x2B', '\xE8', '\x6F', '\x11', '\x0A', '\xAA',
        '\x55', '\xA9', '\x36', '\xF0', '\x5C', '\x08', '\x25', '\xE5', '\x45', '\xB6', '\x29', '\x87'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xDB27CB557D038759ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x3F', '\x4C', '\x9D', '\xF9', '\x67', '\x61', '\x66', '\xF2', '\x39', '\x07', '\x9D', '\xCF', '\x1A', '\x15', '\xFF', '\x7E', '\x9B', '\x95', '\x38',
        '\x6D', '\xD3', '\xDA', '\xD1', '\x5A', '\xB8', '\x96', '\x34', '\x67', '\xC2', '\xF3', '\xD0', '\x5C', '\x8D', '\x8C', '\x31', '\x6F', '\xF8', '\xCE',
        '\xD1', '\x4D', '\x90', '\x94', '\x02', '\x67', '\xC8', '\xD8', '\xCD', '\x11', '\x9D', '\x8E', '\x2D', '\x03'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x3FA3AFA7035DE3FFULL> name(source);
      return name.Text();
    }()));
  }
  {
    description->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x2F', '\x82', '\x97', '\xF7', '\x05', '\x1F', '\x26', '\x4A', '\x29', '\xC9', '\x97', '\xC1', '\x78', '\x6B', '\xBF', '\xC6', '\x8B', '\x5B', '\x32',
        '\x63', '\xB1', '\xA4', '\x91', '\xE2', '\xA8', '\x58', '\x3E', '\x69', '\xA0', '\x8D', '\x90', '\xE4', '\x9D', '\x42', '\x3B', '\x61', '\x9A', '\xB3',
        '\x84', '\xD8', '\x82', '\x44', '\x33', '\x23', '\xA7', '\xBC', '\x93', '\x87'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x87E3D1C50D572DEFULL> name(source);
      return name.Text();
    }()));
  }
  {
    description->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x4D', '\x12', '\x7B', '\x99', '\x77', '\xF7', '\x7E', '\x9C', '\x4B', '\x59', '\x7B', '\xAF', '\x0A', '\x83', '\xE7', '\x10', '\xE9', '\xCB', '\xDE',
        '\x0D', '\xC3', '\x4C', '\xC9', '\x34', '\xCA', '\xC8', '\xD2', '\x07', '\xD2', '\x65', '\xC8', '\x32', '\xFF', '\xD2', '\xD7', '\x0F', '\xE8', '\x4D',
        '\xD3', '\x24', '\xE0', '\xDF', '\xE4', '\x16', '\xC7', '\x17', '\xD9', '\x3C', '\xFD', '\xBD'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x51BB39B763BBBD8DULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xFD', '\xEE', '\x4B', '\x33', '\x39', '\xD5', '\x00', '\x60', '\xFB', '\xA5', '\x4B', '\x05', '\x44', '\xA1', '\x99', '\xEC', '\x59', '\x37', '\xEE',
        '\xA7', '\x8D', '\x6E', '\xB7', '\xC8', '\x7A', '\x34', '\xE2', '\xAD', '\x9C', '\x47', '\xB6', '\xCE', '\x4F', '\x2E', '\xE7', '\xA5', '\xA6', '\x6F',
        '\xAD', '\xD8', '\x50', '\x23', '\xD4', '\xA4', '\x90', '\x7F', '\xEB', '\xCF', '\x50', '\x31', '\x8B'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xADC51BF9C98B413DULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x91', '\x5A', '\x0F', '\xB3', '\x3F', '\xD9', '\x44', '\x12', '\x97', '\x11', '\x0F', '\x85', '\x42', '\xAD', '\xDD', '\x9E', '\x35', '\x83', '\xAA',
        '\x27', '\x8B', '\x62', '\xF3', '\xBA', '\x16', '\x80', '\xA6', '\x2D', '\x9A', '\x4B', '\xF2', '\xBC', '\x23', '\x9A', '\xA3', '\x25', '\xA0', '\x63',
        '\xE9', '\xAA', '\x3C', '\x97', '\x90', '\x2D', '\x90', '\x60', '\xEF', '\xF1', '\x33', '\x98', '\xBF', '\x49'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xDF8117FF49CFF551ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(description);

  CUITextViewer* options = new CUITextViewer();
  this->optionsViewer = reinterpret_cast<CUIStaticText*>(options);
  options->SetSize(115, 90);
  options->Move(338, 26);
  options->SetTextOffset(0, 8);
  {
    options->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x0E', '\x18', '\x13', '\x8A', '\xAA', '\x01'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x114901C7E37F6D49ULL> name(source);
      return name.Text();
    }()));
  }
  options->SetFontHeight(12);
  options->SetBold(0);
  options->SetTextColor(0);
  options->SetOutline(0, 0xFFFF00FF);
  options->SetLineSpacing(6);
  options->SetBackgroundColor(0xFFFF00FF);
  options->SetPadding(0, 0, 0, 0);
  options->SetScrollBarSize(8, 90);
  options->SetScrollStep(20);
  {
    options->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\xB1', '\x2C', '\xAB', '\xF1', '\xD1', '\xC1', '\x98', '\xB0', '\xB7', '\x67', '\xAB', '\xC7', '\xAC', '\xB5', '\x01', '\x3C', '\x15', '\xF5', '\x0E',
        '\x65', '\x65', '\x7A', '\x2F', '\x18', '\x36', '\xF6', '\x02', '\x6F', '\x74', '\x53', '\x2E', '\x1E', '\x03', '\xEC', '\x07', '\x67', '\x4E', '\x6E',
        '\x2F', '\x0F', '\x1E', '\xF4', '\x34', '\x7E', '\x61', '\x21', '\x3F', '\x10', '\x01', '\x83'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x7D5D0F110B6B8371ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x9B', '\x18', '\xA1', '\xA3', '\x8F', '\x9F', '\xD0', '\x3A', '\x9D', '\x53', '\xA1', '\x95', '\xF2', '\xEB', '\x49', '\xB6', '\x3F', '\xC1', '\x04',
        '\x37', '\x3B', '\x24', '\x67', '\x92', '\x1C', '\xC2', '\x08', '\x3D', '\x2A', '\x0D', '\x66', '\x94', '\x29', '\xD8', '\x0D', '\x35', '\x10', '\x30',
        '\x67', '\x85', '\x34', '\xC0', '\x3E', '\x3D', '\x20', '\x26', '\x7B', '\xD9', '\x39', '\xDA', '\x11', '\x59'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xF715514F5961B75BULL> name(source);
      return name.Text();
    }()));
  }
  {
    options->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x59', '\x96', '\x3D', '\xCB', '\x95', '\x75', '\x54', '\x8C', '\x5F', '\xDD', '\x3D', '\xFD', '\xE8', '\x01', '\xCD', '\x00', '\xFD', '\x4F', '\x98',
        '\x5F', '\x21', '\xCE', '\xE3', '\x24', '\xDE', '\x4C', '\x94', '\x55', '\x30', '\xE7', '\xE2', '\x22', '\xEB', '\x56', '\x91', '\x5D', '\x0A', '\xD9',
        '\xF6', '\x1E', '\xF4', '\x50', '\x99', '\x1F', '\x37', '\xD6', '\xE1', '\x41'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x4191BB5531FD3999ULL> name(source);
      return name.Text();
    }()));
  }
  {
    options->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x13', '\x96', '\xAF', '\xEB', '\x69', '\xD1', '\x58', '\x7C', '\x15', '\xDD', '\xAF', '\xDD', '\x14', '\xA5', '\xC1', '\xF0', '\xB7', '\x4F', '\x0A',
        '\x7F', '\xDD', '\x6A', '\xEF', '\xD4', '\x94', '\x4C', '\x06', '\x75', '\xCC', '\x43', '\xEE', '\xD2', '\xA1', '\x56', '\x03', '\x7D', '\xF6', '\x6B',
        '\xF5', '\xC4', '\xBE', '\x5B', '\x30', '\x64', '\xD9', '\x31', '\xFF', '\xDC', '\xA3', '\x39'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xB19D1FA9116F39D3ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x6F', '\xA6', '\xC9', '\x1D', '\x07', '\x07', '\xAC', '\x58', '\x69', '\xED', '\xC9', '\x2B', '\x7A', '\x73', '\x35', '\xD4', '\xCB', '\x7F', '\x6C',
        '\x89', '\xB3', '\xBC', '\x1B', '\xF0', '\xE8', '\x7C', '\x60', '\x83', '\xA2', '\x95', '\x1A', '\xF6', '\xDD', '\x66', '\x65', '\x8B', '\x98', '\xBD',
        '\x01', '\xE0', '\xC2', '\x6B', '\x56', '\x8A', '\xAE', '\xAD', '\x47', '\xF7', '\xC2', '\x79', '\x09'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0x9569C9C7E70909AFULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xAD', '\x10', '\x15', '\xDD', '\x2B', '\x87', '\x62', '\x1A', '\xAB', '\x5B', '\x15', '\xEB', '\x56', '\xF3', '\xFB', '\x96', '\x09', '\xC9', '\xB0',
        '\x49', '\x9F', '\x3C', '\xD5', '\xB2', '\x2A', '\xCA', '\xBC', '\x43', '\x8E', '\x15', '\xD4', '\xB4', '\x1F', '\xD0', '\xB9', '\x4B', '\xB4', '\x3D',
        '\xCF', '\xA2', '\x00', '\xDD', '\x8A', '\x43', '\x84', '\x3E', '\xC9', '\xF9', '\x0F', '\xD2', '\xA5', '\x27'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xD7A749EB27D5BF6DULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(options);

  CUITextViewer* sources = new CUITextViewer();
  this->sourcesViewer = reinterpret_cast<CUIStaticText*>(sources);
  sources->SetSize(108, 118);
  sources->Move(480, 5);
  sources->SetTextOffset(0, 6);
  {
    sources->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x1E', '\x78', '\xA7', '\xC6', '\x10', '\x63'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0xB195637DAFCB0D59ULL> name(source);
      return name.Text();
    }()));
  }
  sources->SetFontHeight(12);
  sources->SetBold(0);
  sources->SetTextColor(0);
  sources->SetOutline(0, 0xFFFF00FF);
  sources->SetLineSpacing(6);
  sources->SetBackgroundColor(0xFFFF00FF);
  sources->SetPadding(0, 0, 0, 0);
  sources->SetScrollBarSize(8, 118);
  sources->SetScrollStep(20);
  {
    sources->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\x95', '\x94', '\x41', '\x0D', '\x8D', '\x25', '\x68', '\x60', '\x93', '\xDF', '\x41', '\x3B', '\xF0', '\x51', '\xF1', '\xEC', '\x31', '\x4D', '\xE4',
        '\x99', '\x39', '\x9E', '\xDF', '\xC8', '\x12', '\x4E', '\xE8', '\x93', '\x28', '\xB7', '\xDE', '\xCE', '\x27', '\x54', '\xED', '\x9B', '\x12', '\x8A',
        '\xDF', '\xDF', '\x3A', '\x4C', '\xDE', '\x82', '\x3D', '\xC5', '\xCF', '\xC0', '\x25', '\x3B'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xADADEB4DF7813B55ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x01', '\x36', '\x0D', '\x1F', '\x41', '\xE3', '\x80', '\x14', '\x07', '\x7D', '\x0D', '\x29', '\x3C', '\x97', '\x19', '\x98', '\xA5', '\xEF', '\xA8',
        '\x8B', '\xF5', '\x58', '\x37', '\xBC', '\x86', '\xEC', '\xA4', '\x81', '\xE4', '\x71', '\x36', '\xBA', '\xB3', '\xF6', '\xA1', '\x89', '\xDE', '\x4C',
        '\x37', '\xAB', '\xAE', '\xEE', '\x92', '\x81', '\xEE', '\x5A', '\x2B', '\xF7', '\xA3', '\xF4', '\xBD', '\xE5'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xD9452D81E5CD99C1ULL> name(source);
      return name.Text();
    }()));
  }
  {
    sources->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x4D', '\x2E', '\xD1', '\x09', '\x73', '\x67', '\xAC', '\x30', '\x4B', '\x65', '\xD1', '\x3F', '\x0E', '\x13', '\x35', '\xBC', '\xE9', '\xF7', '\x74',
        '\x9D', '\xC7', '\xDC', '\x1B', '\x98', '\xCA', '\xF4', '\x78', '\x97', '\xD6', '\xF5', '\x1A', '\x9E', '\xFF', '\xEE', '\x7D', '\x9F', '\xEC', '\xCB',
        '\x0E', '\xA2', '\xE0', '\xE8', '\x75', '\xDD', '\xD1', '\xC4', '\x19', '\xFD'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xFD69A9B3F311818DULL> name(source);
      return name.Text();
    }()));
  }
  {
    sources->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\xEB', '\xB0', '\x29', '\x51', '\x01', '\x31', '\xE8', '\xB2', '\xED', '\xFB', '\x29', '\x67', '\x7C', '\x45', '\x71', '\x3E', '\x4F', '\x69', '\x8C',
        '\xC5', '\xB5', '\x8A', '\x5F', '\x1A', '\x6C', '\x6A', '\x80', '\xCF', '\xA4', '\xA3', '\x5E', '\x1C', '\x59', '\x70', '\x85', '\xC7', '\x9E', '\x8B',
        '\x45', '\x0A', '\x46', '\x7D', '\xB6', '\xDE', '\xB1', '\xD1', '\x4F', '\x12', '\x5B', '\x1F'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x7F2DFFC1ABE91F2BULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xCD', '\xCC', '\x4D', '\xF3', '\x85', '\x9D', '\x4A', '\x14', '\xCB', '\x87', '\x4D', '\xC5', '\xF8', '\xE9', '\xD3', '\x98', '\x69', '\x15', '\xE8',
        '\x67', '\x31', '\x26', '\xFD', '\xBC', '\x4A', '\x16', '\xE4', '\x6D', '\x20', '\x0F', '\xFC', '\xBA', '\x7F', '\x0C', '\xE1', '\x65', '\x1A', '\x27',
        '\xE7', '\xAC', '\x60', '\x01', '\xD2', '\x64', '\x2C', '\x37', '\xA1', '\xBB', '\x60', '\x13', '\x8D'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xD98F5345098D630DULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x53', '\xDA', '\x1F', '\x15', '\x8D', '\xED', '\xCA', '\x56', '\x55', '\x91', '\x1F', '\x23', '\xF0', '\x99', '\x53', '\xDA', '\xF7', '\x03', '\xBA',
        '\x81', '\x39', '\x56', '\x7D', '\xFE', '\xD4', '\x00', '\xB6', '\x8B', '\x28', '\x7F', '\x7C', '\xF8', '\xE1', '\x1A', '\xB3', '\x83', '\x12', '\x57',
        '\x67', '\xEE', '\xFE', '\x17', '\x80', '\x8B', '\x22', '\x54', '\x61', '\xB5', '\xF1', '\x18', '\xAF', '\xEF'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x9B0F234DEFDF7593ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(sources);
}

// OFFSET: 0x009CC580
// FUNCTION: fn_14005
void CUIMonsterInfo::OnCreate(int width, int height) {
  this->nameText = new CUIStaticText();
  this->nameText->SetSize(280, 12);
  this->nameText->Move(165, 19);
  this->nameText->SetText("");
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 0);
  {
    CUIStaticText* name = this->nameText;
    name->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x82', '\x58', '\x47', '\x56', '\xE4', '\x1D'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0xEF6D1D893F2B2DC5ULL> name(source);
      return name.Text();
    }()));
  }
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetColor(0xCE6B52);
  this->nameText->SetShadow(1, 0xDEDEDE);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);

  this->detailsViewer = reinterpret_cast<CUIStaticText*>(new CUITextViewer());
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetSize(288, 84);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->Move(165, 39);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetTextOffset(0, 6);
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->detailsViewer);
    details->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x1C', '\x26', '\x33', '\xB4', '\xEA', '\x9F'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x39A39F87DD5F535BULL> name(source);
      return name.Text();
    }()));
  }
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetPadding(0, 0, 0, 0);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetScrollBarSize(8, 84);
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetScrollStep(20);
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->detailsViewer);
    details->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\xB7', '\x38', '\x47', '\xE7', '\xE3', '\x9D', '\x24', '\xA2', '\xB1', '\x73', '\x47', '\xD1', '\x9E', '\xE9', '\xBD', '\x2E', '\x13', '\xE1', '\xE2',
        '\x73', '\x57', '\x26', '\x93', '\x0A', '\x30', '\xE2', '\xEE', '\x79', '\x46', '\x0F', '\x92', '\x0C', '\x05', '\xF8', '\xEB', '\x71', '\x7C', '\x32',
        '\x93', '\x1D', '\x18', '\xE0', '\xD8', '\x68', '\x53', '\x7D', '\x83', '\x02', '\x07', '\x97'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x6FE153231D879777ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x0D', '\xAC', '\x0D', '\x81', '\x57', '\xF5', '\xD6', '\xFE', '\x0B', '\xE7', '\x0D', '\xB7', '\x2A', '\x81', '\x4F', '\x72', '\xA9', '\x75', '\xA8',
        '\x15', '\xE3', '\x4E', '\x61', '\x56', '\x8A', '\x76', '\xA4', '\x1F', '\xF2', '\x67', '\x60', '\x50', '\xBF', '\x6C', '\xA1', '\x17', '\xC8', '\x5A',
        '\x61', '\x41', '\xA2', '\x74', '\x92', '\x1F', '\xF8', '\x4C', '\x7D', '\x1D', '\xAF', '\x6E', '\xBD', '\x7B'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x33133B977BCD03CDULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->detailsViewer);
    details->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x11', '\xF4', '\x57', '\x1F', '\xF9', '\x49', '\x2E', '\xE2', '\x17', '\xBF', '\x57', '\x29', '\x84', '\x3D', '\xB7', '\x6E', '\xB5', '\x2D', '\xF2',
        '\x8B', '\x4D', '\xF2', '\x99', '\x4A', '\x96', '\x2E', '\xFE', '\x81', '\x5C', '\xDB', '\x98', '\x4C', '\xA3', '\x34', '\xFB', '\x89', '\x66', '\xE5',
        '\x8C', '\x70', '\xBC', '\x32', '\xF3', '\xCB', '\x5B', '\xEA', '\x9B', '\x2F'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x2FEB8739E5975BD1ULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->detailsViewer);
    details->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\xB5', '\x94', '\xC3', '\xD5', '\xD3', '\x0F', '\x64', '\xA6', '\xB3', '\xDF', '\xC3', '\xE3', '\xAE', '\x7B', '\xFD', '\x2A', '\x11', '\x4D', '\x66',
        '\x41', '\x67', '\xB4', '\xD3', '\x0E', '\x32', '\x4E', '\x6A', '\x4B', '\x76', '\x9D', '\xD2', '\x08', '\x07', '\x54', '\x6F', '\x43', '\x4C', '\xB5',
        '\xC9', '\x1E', '\x18', '\x59', '\x5C', '\x5A', '\x63', '\xEF', '\xC3', '\x06', '\x05', '\x3B'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x6BA1C1132F033B75ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xEF', '\x78', '\x85', '\xDF', '\xF9', '\x65', '\xE8', '\x6C', '\xE9', '\x33', '\x85', '\xE9', '\x84', '\x11', '\x71', '\xE0', '\x4B', '\xA1', '\x20',
        '\x4B', '\x4D', '\xDE', '\x5F', '\xC4', '\x68', '\xA2', '\x2C', '\x41', '\x5C', '\xF7', '\x5E', '\xC2', '\x5D', '\xB8', '\x29', '\x49', '\x66', '\xDF',
        '\x45', '\xD4', '\x42', '\xB5', '\x1A', '\x48', '\x50', '\xCF', '\x03', '\xC3', '\x42', '\xA7', '\x45'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xA12DAB392545D72FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xD7', '\xFE', '\x21', '\xBB', '\xF9', '\x0D', '\x46', '\x4A', '\xD1', '\xB5', '\x21', '\x8D', '\x84', '\x79', '\xDF', '\xC6', '\x73', '\x27', '\x84',
        '\x2F', '\x4D', '\xB6', '\xF1', '\xE2', '\x50', '\x24', '\x88', '\x25', '\x5C', '\x9F', '\xF0', '\xE4', '\x65', '\x3E', '\x8D', '\x2D', '\x66', '\xB7',
        '\xEB', '\xF2', '\x7A', '\x33', '\xBE', '\x25', '\x56', '\xB4', '\xED', '\xA9', '\x75', '\x3C', '\x91', '\x41'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x8783C33941E15117ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->detailsViewer);

  this->valuesViewer = reinterpret_cast<CUIStaticText*>(new CUITextViewer());
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetSize(108, 118);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->Move(480, 5);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetTextOffset(0, 6);
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->valuesViewer);
    values->SetFont(std::string([]() {
      constexpr char bytes[] = {'\xD8', '\xA8', '\xEF', '\x76', '\x1A', '\x45'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x271B45771F83DD9FULL> name(source);
      return name.Text();
    }()));
  }
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetPadding(0, 0, 0, 0);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetScrollBarSize(8, 118);
  reinterpret_cast<CUITextViewer*>(this->valuesViewer)->SetScrollStep(20);
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->valuesViewer);
    values->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\x4F', '\xF6', '\xA1', '\x11', '\x75', '\xD7', '\xC6', '\x0A', '\x49', '\xBD', '\xA1', '\x27', '\x08', '\xA3', '\x5F', '\x86', '\xEB', '\x2F', '\x04',
        '\x85', '\xC1', '\x6C', '\x71', '\xA2', '\xC8', '\x2C', '\x08', '\x8F', '\xD0', '\x45', '\x70', '\xA4', '\xFD', '\x36', '\x0D', '\x87', '\xEA', '\x78',
        '\x71', '\xB5', '\xE0', '\x2E', '\x3E', '\x9E', '\xC5', '\x37', '\x61', '\xAA', '\xFF', '\x59'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xC70319B5EB61598FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x17', '\x3A', '\xDF', '\x61', '\x91', '\x8B', '\x42', '\xC6', '\x11', '\x71', '\xDF', '\x57', '\xEC', '\xFF', '\xDB', '\x4A', '\xB3', '\xE3', '\x7A',
        '\xF5', '\x25', '\x30', '\xF5', '\x6E', '\x90', '\xE0', '\x76', '\xFF', '\x34', '\x19', '\xF4', '\x68', '\xA5', '\xFA', '\x73', '\xF7', '\x0E', '\x24',
        '\xF5', '\x79', '\xB8', '\xE2', '\x40', '\xFF', '\x3E', '\x32', '\xE9', '\x25', '\xB5', '\xF8', '\x6F', '\x9B'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x0B8745519B1F95D7ULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->valuesViewer);
    values->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x15', '\x96', '\xEB', '\xB3', '\x27', '\x69', '\x52', '\x68', '\x13', '\xDD', '\xEB', '\x85', '\x5A', '\x1D', '\xCB', '\xE4', '\xB1', '\x4F', '\x4E',
        '\x27', '\x93', '\xD2', '\xE5', '\xC0', '\x92', '\x4C', '\x42', '\x2D', '\x82', '\xFB', '\xE4', '\xC6', '\xA7', '\x56', '\x47', '\x25', '\xB8', '\xC5',
        '\xF0', '\xFA', '\xB8', '\x50', '\x4F', '\x67', '\x85', '\xCA', '\xE7', '\xA5'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xA597A7E7492B39D5ULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->valuesViewer);
    values->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\xB3', '\x40', '\x7B', '\x2B', '\x15', '\x2D', '\x6E', '\x32', '\xB5', '\x0B', '\x7B', '\x1D', '\x68', '\x59', '\xF7', '\xBE', '\x17', '\x99', '\xDE',
        '\xBF', '\xA1', '\x96', '\xD9', '\x9A', '\x34', '\x9A', '\xD2', '\xB5', '\xB0', '\xBF', '\xD8', '\x9C', '\x01', '\x80', '\xD7', '\xBD', '\x8A', '\x97',
        '\xC3', '\x8A', '\x1E', '\x8D', '\xE4', '\xA4', '\xA5', '\xCD', '\xC9', '\x92', '\x03', '\xEF'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xFFABE3D5D1BBEF73ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xCF', '\xE0', '\x85', '\x55', '\x5D', '\x8B', '\x5C', '\xCE', '\xC9', '\xAB', '\x85', '\x63', '\x20', '\xFF', '\xC5', '\x42', '\x6B', '\x39', '\x20',
        '\xC1', '\xE9', '\x30', '\xEB', '\x66', '\x48', '\x3A', '\x2C', '\xCB', '\xF8', '\x19', '\xEA', '\x60', '\x7D', '\x20', '\x29', '\xC3', '\xC2', '\x31',
        '\xF1', '\x76', '\x62', '\x2D', '\x1A', '\xC2', '\xF4', '\x21', '\xB7', '\x61', '\x62', '\x3F', '\x45'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0x0399459DAF454F0FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x6B', '\x00', '\x4D', '\x77', '\x73', '\x43', '\x62', '\x18', '\x6D', '\x4B', '\x4D', '\x41', '\x0E', '\x37', '\xFB', '\x94', '\xCF', '\xD9', '\xE8',
        '\xE3', '\xC7', '\xF8', '\xD5', '\xB0', '\xEC', '\xDA', '\xE4', '\xE9', '\xD6', '\xD1', '\xD4', '\xB6', '\xD9', '\xC0', '\xE1', '\xE1', '\xEC', '\xF9',
        '\xCF', '\xA0', '\xC6', '\xCD', '\xD2', '\xE9', '\xDC', '\xFA', '\xC9', '\xFB', '\xC9', '\xC2', '\xFD', '\x8D'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xD5A78DB38D8DAFABULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->valuesViewer);
}

// OFFSET: 0x009CFAF0
// FUNCTION: fn_14007
void CUIQuestInfo::OnCreate(int width, int height) {
  this->stampImage = new CUIStaticImage();
  this->stampImage->SetSize(85, 70);
  this->stampImage->Move(250, 10);
  {
    CUIStaticImage* stamp = static_cast<CUIStaticImage*>(this->stampImage);
    stamp->SetImage(std::string([]() {
      constexpr char bytes[] = {
        '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\xB0', '\x95', '\xB1', '\x5C',
        '\x67', '\x63', '\x0C', '\x3D', '\x94', '\xB6', '\xB2', '\x50', '\x6D', '\x72', '\x25', '\x3C', '\x85', '\x90', '\xAA', '\x49', '\x56', '\x74', '\x16',
        '\x22', '\x81', '\x9D', '\xA2', '\x4D', '\x6C', '\x73', '\x57', '\x2D', '\x9C', '\x81', '\xC7'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xF14F79170939C7F1ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->stampImage);

  this->titleViewer = reinterpret_cast<CUIStaticText*>(new CUITextViewer());
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetSize(360, 57);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->Move(25, 13);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetTextOffset(0, 0);
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->titleViewer);
    title->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x32', '\xB8', '\x81', '\x34', '\x06', '\x65'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x35E5656B5DEDCD75ULL> name(source);
      return name.Text();
    }()));
  }
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetBold(1);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetPadding(15, 5, 5, 5);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetScrollBarSize(8, 87);
  reinterpret_cast<CUITextViewer*>(this->titleViewer)->SetScrollStep(20);
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->titleViewer);
    title->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\x3D', '\x16', '\x7B', '\xDB', '\x5F', '\x33', '\x84', '\x84', '\x3B', '\x5D', '\x7B', '\xED', '\x22', '\x47', '\x1D', '\x08', '\x99', '\xCF', '\xDE',
        '\x4F', '\xEB', '\x88', '\x33', '\x2C', '\xBA', '\xCC', '\xD2', '\x45', '\xFA', '\xA1', '\x32', '\x2A', '\x8F', '\xD6', '\xD7', '\x4D', '\xC0', '\x9C',
        '\x33', '\x3B', '\x92', '\xCE', '\xE4', '\x54', '\xEF', '\xD3', '\x23', '\x24', '\x8D', '\xB9'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x4941FD9F21BBB9FDULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x3D', '\x78', '\x8D', '\x53', '\x75', '\x41', '\x52', '\x2A', '\x3B', '\x33', '\x8D', '\x65', '\x08', '\x35', '\xCB', '\xA6', '\x99', '\xA1', '\x28',
        '\xC7', '\xC1', '\xFA', '\xE5', '\x82', '\xBA', '\xA2', '\x24', '\xCD', '\xD0', '\xD3', '\xE4', '\x84', '\x8F', '\xB8', '\x21', '\xC5', '\xEA', '\xEE',
        '\xE5', '\x95', '\x92', '\xA0', '\x12', '\xCD', '\xDA', '\xF8', '\xF9', '\xC9', '\x9F', '\xBA', '\x3D', '\xA9'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xE7978FB5A94DD7FDULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->titleViewer);
    title->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\xAB', '\x16', '\x07', '\xF5', '\xA7', '\x41', '\x38', '\x7A', '\xAD', '\x5D', '\x07', '\xC3', '\xDA', '\x35', '\xA1', '\xF6', '\x0F', '\xCF', '\xA2',
        '\x61', '\x13', '\xFA', '\x8F', '\xD2', '\x2C', '\xCC', '\xAE', '\x6B', '\x02', '\xD3', '\x8E', '\xD4', '\x19', '\xD6', '\xAB', '\x63', '\x38', '\xED',
        '\x9A', '\xE8', '\x06', '\xD0', '\xA3', '\x21', '\x05', '\xE2', '\x8D', '\xB7'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xB7FD8F670FC7B96BULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->titleViewer);
    title->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x4F', '\x36', '\x2B', '\xF7', '\x75', '\xB1', '\x3A', '\xF4', '\x49', '\x7D', '\x2B', '\xC1', '\x08', '\xC5', '\xA3', '\x78', '\xEB', '\xEF', '\x8E',
        '\x63', '\xC1', '\x0A', '\x8D', '\x5C', '\xC8', '\xEC', '\x82', '\x69', '\xD0', '\x23', '\x8C', '\x5A', '\xFD', '\xF6', '\x87', '\x61', '\xEA', '\x0B',
        '\x97', '\x4C', '\xE2', '\xFB', '\xB4', '\x78', '\xC5', '\x51', '\x9D', '\x54', '\xFF', '\x99'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x39FF7FB50DEB998FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x57', '\x5E', '\x21', '\x27', '\x11', '\x5D', '\x2C', '\xE2', '\x51', '\x15', '\x21', '\x11', '\x6C', '\x29', '\xB5', '\x6E', '\xF3', '\x87', '\x84',
        '\xB3', '\xA5', '\xE6', '\x9B', '\x4A', '\xD0', '\x84', '\x88', '\xB9', '\xB4', '\xCF', '\x9A', '\x4C', '\xE5', '\x9E', '\x8D', '\xB1', '\x8E', '\xE7',
        '\x81', '\x5A', '\xFA', '\x93', '\xBE', '\xB0', '\xB8', '\xF7', '\xC7', '\x4D', '\xFA', '\x81', '\xE1'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0x2FE993D1DDE1F197ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x0D', '\x5E', '\x59', '\x4F', '\x4D', '\x95', '\xC2', '\x60', '\x0B', '\x15', '\x59', '\x79', '\x30', '\xE1', '\x5B', '\xEC', '\xA9', '\x87', '\xFC',
        '\xDB', '\xF9', '\x2E', '\x75', '\xC8', '\x8A', '\x84', '\xF0', '\xD1', '\xE8', '\x07', '\x74', '\xCE', '\xBF', '\x9E', '\xF5', '\xD9', '\xD2', '\x2F',
        '\x6F', '\xD8', '\xA0', '\x93', '\xC6', '\xD1', '\xE2', '\x2C', '\x69', '\x83', '\xAF', '\x9C', '\xE9', '\xB5'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xAD075B8DB599F1CDULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->titleViewer);

  this->linesViewer = reinterpret_cast<CUIStaticText*>(new CUITextViewer());
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetSize(200, 87);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->Move(400, 5);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetTextOffset(0, 6);
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->linesViewer);
    lines->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x38', '\x60', '\xE1', '\x78', '\x5A', '\x0F'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0x2F8B0F37118D157FULL> name(source);
      return name.Text();
    }()));
  }
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetBold(1);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetPadding(15, 5, 5, 5);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetScrollBarSize(8, 87);
  reinterpret_cast<CUITextViewer*>(this->linesViewer)->SetScrollStep(20);
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->linesViewer);
    lines->SetScrollBarButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\xBF', '\x7A', '\x6F', '\xA1', '\xAB', '\x7F', '\xD0', '\xC2', '\xB9', '\x31', '\x6F', '\x97', '\xD6', '\x0B', '\x49', '\x4E', '\x1B', '\xA3', '\xCA',
        '\x35', '\x1F', '\xC4', '\x67', '\x6A', '\x38', '\xA0', '\xC6', '\x3F', '\x0E', '\xED', '\x66', '\x6C', '\x0D', '\xBA', '\xC3', '\x37', '\x34', '\xD0',
        '\x67', '\x7D', '\x10', '\xA2', '\xF0', '\x2E', '\x1B', '\x9F', '\x77', '\x62', '\x0F', '\xD5'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x0F15B16B5BAFD57FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xE3', '\x5E', '\x17', '\xED', '\x17', '\x87', '\x26', '\xB0', '\xE5', '\x15', '\x17', '\xDB', '\x6A', '\xF3', '\xBF', '\x3C', '\x47', '\x87', '\xB2',
        '\x79', '\xA3', '\x3C', '\x91', '\x18', '\x64', '\x84', '\xBE', '\x73', '\xB2', '\x15', '\x90', '\x1E', '\x51', '\x9E', '\xBB', '\x7B', '\x88', '\x28',
        '\x91', '\x0F', '\x4C', '\x86', '\x88', '\x73', '\xB8', '\x3E', '\x8D', '\x53', '\x41', '\x9C', '\xA7', '\x17'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x7DE349D717D7F123ULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->linesViewer);
    lines->SetScrollBarBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x21', '\x36', '\x47', '\xB5', '\x41', '\x0D', '\xE4', '\xBE', '\x27', '\x7D', '\x47', '\x83', '\x3C', '\x79', '\x7D', '\x32', '\x85', '\xEF', '\xE2',
        '\x21', '\xF5', '\xB6', '\x53', '\x16', '\xA6', '\xEC', '\xEE', '\x2B', '\xE4', '\x9F', '\x52', '\x10', '\x93', '\xF6', '\xEB', '\x23', '\xDE', '\xA1',
        '\x46', '\x2C', '\x8C', '\xF0', '\xE3', '\x61', '\xE3', '\xAE', '\x51', '\x73'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x7321C3814F8799E1ULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->linesViewer);
    lines->SetScrollBarThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x8F', '\x0E', '\x0F', '\xCF', '\xA3', '\x19', '\x84', '\xB4', '\x89', '\x45', '\x0F', '\xF9', '\xDE', '\x6D', '\x1D', '\x38', '\x2B', '\xD7', '\xAA',
        '\x5B', '\x17', '\xA2', '\x33', '\x1C', '\x08', '\xD4', '\xA6', '\x51', '\x06', '\x8B', '\x32', '\x1A', '\x3D', '\xCE', '\xA3', '\x59', '\x3C', '\xA3',
        '\x29', '\x0C', '\x22', '\xC3', '\x90', '\x40', '\x13', '\xF9', '\x23', '\x14', '\x3F', '\xA1'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x7941D76335CFA14FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xA3', '\x5A', '\x21', '\x85', '\xFB', '\xEB', '\xD8', '\x06', '\xA5', '\x11', '\x21', '\xB3', '\x86', '\x9F', '\x41', '\x8A', '\x07', '\x83', '\x84',
        '\x11', '\x4F', '\x50', '\x6F', '\xAE', '\x24', '\x80', '\x88', '\x1B', '\x5E', '\x79', '\x6E', '\xA8', '\x11', '\x9A', '\x8D', '\x13', '\x64', '\x51',
        '\x75', '\xBE', '\x0E', '\x97', '\xBE', '\x12', '\x52', '\x41', '\x33', '\xA9', '\x0E', '\x85', '\xE1'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xCB1D253B7FE1F563ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x09', '\xBC', '\x39', '\x89', '\x39', '\x17', '\xD0', '\xBC', '\x0F', '\xF7', '\x39', '\xBF', '\x44', '\x63', '\x49', '\x30', '\xAD', '\x65', '\x9C',
        '\x1D', '\x8D', '\xAC', '\x67', '\x14', '\x8E', '\x66', '\x90', '\x17', '\x9C', '\x85', '\x66', '\x12', '\xBB', '\x7C', '\x95', '\x1F', '\xA6', '\xAD',
        '\x7D', '\x04', '\xA4', '\x71', '\xA6', '\x17', '\x96', '\xAE', '\x7B', '\x5F', '\xAB', '\x7E', '\x89', '\x73'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x7115D9F973F913C9ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->linesViewer);

  this->statusViewer = reinterpret_cast<CUIStaticText*>(new CUITextViewer());
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetSize(360, 13);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->Move(25, 73);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextOffset(0, 0);
  {
    CUITextViewer* status = reinterpret_cast<CUITextViewer*>(this->statusViewer);
    status->SetFont(std::string([]() {
      constexpr char bytes[] = {'\x5A', '\x70', '\xFF', '\x40', '\x1C', '\x23'};
      constexpr EncryptedSource<6> source(bytes);
      thread_local EncryptedName<6, 0xE55723712993051DULL> name(source);
      return name.Text();
    }()));
  }
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetPadding(15, 0, 3, 0);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetScrollBarSize(8, 12);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetScrollStep(1);
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetScrollBarButtonImages("", "");
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetScrollBarBackground("");
  reinterpret_cast<CUITextViewer*>(this->statusViewer)->SetScrollBarThumbImages("", "", "");
  this->AddChild(this->statusViewer);
}

// OFFSET: 0x009DCE10
// FUNCTION: fn_14330
void CUICharacterInfo::OnInit() {
  this->jobIcon->SetImage(std::format("유저인터페이스\\renewalparty\\icon_jobs_{}.bmp", g_session.m_job));
  const int level = g_session.m_level;
  this->level->SetText(std::format("Lv. {}", level));
  const int jobLevel = g_session.m_jobLevel;
  this->jobLevel->SetText(std::format("Jv. {}", jobLevel));
  this->name->SetText(g_session.DecodePlayerName());
}

// OFFSET: 0x009DD750
// FUNCTION: fn_14343
void CUIItemInfo::OnRBtnDown(int, int) {
  ITEM_INFO info;
  info.SetItemId(this->itemId);
  info.identity[0] = this->type;
  info.identified = 1;
  int location;
  if (this->type == 5) {
    location = this->location <= 9 ? 2 : 0x22;
  } else if (this->type == 4) {
    switch (this->location) {
    case 0:
      location = 0x100;
      break;
    case 1:
      location = 0x200;
      break;
    case 2:
      location = 1;
      break;
    case 3:
      location = 0x10;
      break;
    case 4:
      location = 4;
      break;
    case 5:
      location = 0x20;
      break;
    case 6:
      location = 0x40;
      break;
    case 7:
      location = 0x88;
      break;
    case 8:
      location = 8;
      break;
    case 9:
      location = 0x80;
      break;
    default:
      location = 0;
      break;
    }
  } else {
    location = 0;
  }
  info.identity[2] = location;

  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
  if (window) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
  }
}

// OFFSET: 0x009DC720
// FUNCTION: fn_14322
void CUIMonster::BindMonster(CUIMonsterInfo* item, const CMonsterData& data) {
  item->SetMonster(data.name);
  reinterpret_cast<CUITextViewer*>(item->valuesViewer)->Clear();
  for (const std::string& value : data.values) {
    reinterpret_cast<CUITextViewer*>(item->valuesViewer)->AddLine(value);
  }
}

// OFFSET: 0x009DFCB0
// FUNCTION: fn_14360
void CUIMonsterInfo::SetMonster(const std::string& title) {
  reinterpret_cast<CUITextViewer*>(this->detailsViewer)->Clear();

  std::string name;
  std::string spriteName;
  int x = 0;
  int size = 0;
  int element = 0;
  int elementLevel = 0;
  int race = 0;
  int type = 0;
  if (!g_navigationManager.GetMobInfo(title, &name, &spriteName, &x, &size, &element, &elementLevel, &race, &type)) {
    this->nameText->SetText(title);
    return;
  }

  this->nameText->SetText(name.c_str(), name.size());
  this->sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName + ".spr"));
  this->action = static_cast<CActRes*>(ResourceMgr::Instance().Load(spriteName + ".act"));

  if (element == 1) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY4), elementLevel));
  } else if (element == 2) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY5), elementLevel));
  } else if (element == 3) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY6), elementLevel));
  } else if (element == 4) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY7), elementLevel));
  } else if (element == 5) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY8), elementLevel));
  } else if (element == 6) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY9), elementLevel));
  } else if (element == 7) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY0), elementLevel));
  } else if (element == 8) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_SCALE1), elementLevel));
  } else if (element == 9) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_SCALE2), elementLevel));
  } else {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_SCALE0), elementLevel));
  }

  if (race == 1) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE4)));
  } else if (race == 2) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE5)));
  } else if (race == 3) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE6)));
  } else if (race == 4) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE7)));
  } else if (race == 5) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE8)));
  } else if (race == 6) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE9)));
  } else if (race == 7) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE0)));
  } else if (race == 8) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_MOVECLICK)));
  } else if (race == 9) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_NPCMOVE)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_NPCCLICK)));
  }

  if (size == 1) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE1)));
  } else if (size == 2) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE2)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE3)));
  }

  if (type == 0x12D) {
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->SetTextColor(0x6464FFu);
    reinterpret_cast<CUITextViewer*>(this->detailsViewer)->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_TARGET_D_D)));
  }
}

}

// OFFSET: 0x009DC910
// FUNCTION: fn_14327
void AdventureGuide::CUIMonsterInfo::Paint() {
  CUIListItem::Paint();
  this->DrawActionSprite(this->sprite, this->action, 16, 2, 124, 124, 0, true, nullptr);
}

namespace AdventureGuide {

// OFFSET: 0x009C1550
// FUNCTION: fn_13992
CUIListBox<CUISubTab>* CUITab::CreateSubTabList() {
  this->subTabs = new CUIListBox<CUISubTab>();
  this->subTabs->SetSize(98, 26);
  this->subTabs->Move(0, 40);
  this->subTabs->SetOnSelect([this](CUISubTab* subTab) { this->OnSelectSubTab(subTab); });

  CUIListBox<CUISubTab>* list = this->subTabs;
  {
    list->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\xC3', '\x9A', '\x59', '\x4F', '\x73', '\xB9', '\x0C', '\xA6', '\xC5', '\xD1', '\x59', '\x79', '\x0E', '\xCD', '\x95', '\x2A', '\x67', '\x43', '\xFC',
        '\xDB', '\xC7', '\x02', '\xBB', '\x0E', '\x44', '\x40', '\xF0', '\xD1', '\xD6', '\x2B', '\xBD', '\x0A', '\x61', '\x6A', '\xFB', '\xC1', '\xDD', '\x28',
        '\xBA', '\x1E', '\x61', '\x6A', '\xF7', '\xDA', '\xC1', '\x1A', '\xA8', '\x07', '\x2D', '\x57', '\xF4', '\xC5', '\xB3'};
      constexpr EncryptedSource<53> source(bytes);
      thread_local EncryptedName<53, 0x6BC977B3B5993503ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x33', '\xBA', '\xB3', '\xD9', '\xB9', '\x39', '\x36', '\x78', '\x35', '\xF1', '\xB3', '\xEF', '\xC4', '\x4D', '\xAF', '\xF4', '\x97', '\x63', '\x16',
        '\x4D', '\x0D', '\x82', '\x81', '\xD0', '\xB4', '\x60', '\x1A', '\x47', '\x1C', '\xAB', '\x87', '\xD4', '\x91', '\x4A', '\x11', '\x57', '\x17', '\xA8',
        '\x80', '\xC0', '\x91', '\x4A', '\x03', '\x51', '\x1C', '\x84', '\x80', '\x9B', '\x91', '\x78', '\x03', '\x23'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xB5F3F779237315F3ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x11', '\x76', '\xD9', '\xE3', '\x6B', '\x95', '\x18', '\xAA', '\x17', '\x3D', '\xD9', '\xD5', '\x16', '\xE1', '\x81', '\x26', '\xB5', '\xAF', '\x7C',
        '\x77', '\xDF', '\x2E', '\xAF', '\x02', '\x96', '\xAC', '\x70', '\x7D', '\xCE', '\x07', '\xA9', '\x06', '\xB3', '\x86', '\x7B', '\x6D', '\xC5', '\x04',
        '\xAE', '\x12', '\xB3', '\x86', '\x76', '\x6F', '\xCE', '\x29', '\xF3', '\x05', '\xBC', '\xA9', '\x19'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0x67DD5BAB1919D9D1ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x97', '\x50', '\x2D', '\xB5', '\x6F', '\x7B', '\x12', '\x78', '\x91', '\x1B', '\x2D', '\x83', '\x12', '\x0F', '\x8B', '\xF4', '\x33', '\x89', '\x88',
        '\x21', '\xDB', '\xC0', '\xA5', '\xD0', '\x10', '\x8A', '\x84', '\x2B', '\xCA', '\xE9', '\xA3', '\xD4', '\x35', '\xA0', '\x8F', '\x3B', '\xC1', '\xEA',
        '\xA4', '\xC0', '\x35', '\xA0', '\x83', '\x20', '\xDD', '\xD8', '\xB6', '\xD9', '\x79', '\x9D', '\x80', '\x3F', '\xAF'};
      constexpr EncryptedSource<53> source(bytes);
      thread_local EncryptedName<53, 0xB5D7B5AF4FEDFF57ULL> name(source);
      return name.Text();
    }()));
  }

  this->subTabs->itemWidth = 98;
  this->subTabs->rowHeight = 26;
  this->subTabs->itemX = 0;
  this->subTabs->itemY = 0;
  this->subTabs->scrollBar->SetSize(9, 26);
  this->subTabs->scrollBar->Move(89, 0);
  this->subTabs->scrollBar->step = 20;
  {
    this->subTabs->scrollBar->SetButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\xB9', '\x58', '\xD5', '\xCF', '\x6D', '\x9F', '\x58', '\x56', '\xBF', '\x13', '\xD5', '\xF9', '\x10', '\xEB', '\xC1', '\xDA', '\x1D', '\x81', '\x70',
        '\x5B', '\xD9', '\x24', '\xEF', '\xFE', '\x3E', '\x82', '\x7C', '\x51', '\xC8', '\x0D', '\xEE', '\xF8', '\x0B', '\x98', '\x79', '\x59', '\xF2', '\x30',
        '\xEF', '\xE9', '\x16', '\x80', '\x4A', '\x40', '\xDD', '\x7F', '\xFF', '\xF6', '\x09', '\xF7'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x9B9D51AD3515F779ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x77', '\x9C', '\xE5', '\xF3', '\x7B', '\xFB', '\x40', '\xCE', '\x71', '\xD7', '\xE5', '\xC5', '\x06', '\x8F', '\xD9', '\x42', '\xD3', '\x45', '\x40',
        '\x67', '\xCF', '\x40', '\xF7', '\x66', '\xF0', '\x46', '\x4C', '\x6D', '\xDE', '\x69', '\xF6', '\x60', '\xC5', '\x5C', '\x49', '\x65', '\xE4', '\x54',
        '\xF7', '\x71', '\xD8', '\x44', '\x7A', '\x6D', '\xD4', '\x42', '\xEB', '\x2D', '\xD5', '\x5E', '\x55', '\x09'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x038535BB092533B7ULL> name(source);
      return name.Text();
    }()));
  }
  {
    this->subTabs->scrollBar->SetBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x13', '\xD8', '\x27', '\x93', '\x97', '\x09', '\xEE', '\x36', '\x15', '\x93', '\x27', '\xA5', '\xEA', '\x7D', '\x77', '\xBA', '\xB7', '\x01', '\x82',
        '\x07', '\x23', '\xB2', '\x59', '\x9E', '\x94', '\x02', '\x8E', '\x0D', '\x32', '\x9B', '\x58', '\x98', '\xA1', '\x18', '\x8B', '\x05', '\x08', '\xA5',
        '\x4C', '\xA4', '\xBE', '\x1E', '\x83', '\x47', '\x35', '\xAA', '\x5B', '\xFB'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xFB2BC75769E777D3ULL> name(source);
      return name.Text();
    }()));
  }
  {
    this->subTabs->scrollBar->SetThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x51', '\x86', '\xB9', '\xB9', '\x1B', '\xC7', '\x60', '\xBC', '\x57', '\xCD', '\xB9', '\x8F', '\x66', '\xB3', '\xF9', '\x30', '\xF5', '\x5F', '\x1C',
        '\x2D', '\xAF', '\x7C', '\xD7', '\x14', '\xD6', '\x5C', '\x10', '\x27', '\xBE', '\x55', '\xD6', '\x12', '\xE3', '\x46', '\x15', '\x2F', '\x84', '\x7D',
        '\xCD', '\x04', '\xFC', '\x4B', '\x26', '\x36', '\xAB', '\x27', '\xC7', '\x1C', '\xE1', '\x29'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x71A509DB43792991ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xBD', '\xFC', '\x4F', '\x5B', '\x3F', '\x23', '\xAC', '\x10', '\xBB', '\xB7', '\x4F', '\x6D', '\x42', '\x57', '\x35', '\x9C', '\x19', '\x25', '\xEA',
        '\xCF', '\x8B', '\x98', '\x1B', '\xB8', '\x3A', '\x26', '\xE6', '\xC5', '\x9A', '\xB1', '\x1A', '\xBE', '\x0F', '\x3C', '\xE3', '\xCD', '\xA0', '\x99',
        '\x01', '\xA8', '\x10', '\x31', '\xD0', '\xCC', '\x96', '\x89', '\x47', '\xBF', '\x10', '\x23', '\x8F'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xDD69EDFFA18F537DULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xB3', '\x8E', '\x27', '\xBD', '\xEB', '\xB7', '\x7A', '\x46', '\xB5', '\xC5', '\x27', '\x8B', '\x96', '\xC3', '\xE3', '\xCA', '\x17', '\x57', '\x82',
        '\x29', '\x5F', '\x0C', '\xCD', '\xEE', '\x34', '\x54', '\x8E', '\x23', '\x4E', '\x25', '\xCC', '\xE8', '\x01', '\x4E', '\x8B', '\x2B', '\x74', '\x0D',
        '\xD7', '\xFE', '\x1E', '\x43', '\xB8', '\x23', '\x44', '\x0E', '\xD1', '\xA5', '\x11', '\x4C', '\x97', '\x47'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x8BBF792B47E72173ULL> name(source);
      return name.Text();
    }()));
  }

  this->AddChild(this->subTabs);
  return this->subTabs;
}

}

// OFFSET: 0x009C1230
// FUNCTION: fn_13986
CUIAdventureGuide* CreateAdventureGuide() {
  return new CUIAdventureGuide();
}

// OFFSET: 0x00487B10
// FUNCTION: fn_45546
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateAdventureGuide;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10002, factory);
  return true;
}();

// OFFSET: 0x009DB280
// FUNCTION: fn_14306
template <>
AdventureGuide::CUISubTab* CUIListBox<AdventureGuide::CUISubTab>::CreateItem() {
  AdventureGuide::CUISubTab* item = new AdventureGuide::CUISubTab();
  item->SetSize(this->itemWidth, this->rowHeight);
  item->Show(0);
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  item->SetImageResources(images[0], images[1], images[2], images[3]);
  item->onHover = [this](CUIListItem* listItem) {
    for (AdventureGuide::CUISubTab* other : this->items) {
      other->SetHighlighted(other == listItem);
    }
  };
  item->onPress = [this](CUIListItem* listItem) {
    this->SelectItem(listItem);
  };
  this->AddChild(item);
  this->items.push_back(item);
  this->UpdateScrollBar();
  this->Invalidate();
  return item;
}

// OFFSET: 0x009DB4B0
// FUNCTION: fn_14307
template <>
AdventureGuide::CUITab* CUIListBox<AdventureGuide::CUITab>::CreateItem() {
  AdventureGuide::CUITab* item = new AdventureGuide::CUITab();
  item->SetSize(this->itemWidth, this->rowHeight);
  item->Show(0);
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  item->SetImageResources(images[0], images[1], images[2], images[3]);
  item->onHover = [this](CUIListItem* listItem) {
    for (AdventureGuide::CUITab* other : this->items) {
      other->SetHighlighted(other == listItem);
    }
  };
  item->onPress = [this](CUIListItem* listItem) {
    this->SelectItem(listItem);
  };
  this->AddChild(item);
  this->items.push_back(item);
  this->UpdateScrollBar();
  this->Invalidate();
  return item;
}

namespace AdventureGuide {

// OFFSET: 0x009DB720
// FUNCTION: fn_14309
void CUIItem::RequestNavigation() {
  std::string name;
  int x = -1;
  int y = -1;
  std::vector<std::string> parts = Split(this->navigationTarget, ",");
  if (parts.size() > 2) {
    name = parts[0];
    x = std::atoi(parts[1].c_str());
    y = std::atoi(parts[2].c_str());
  }

  if (x == -1 || y == -1) {
    g_navigationManager.StartNavigation(name, 1, 5, false, -1, -1, 1002);
  } else {
    g_navigationManager.StartNavigation(name, 0, 5, false, x, y, 1002);
  }
}

// OFFSET: 0x009DB8F0
// FUNCTION: fn_14310
void CUIQuest::RequestNavigation() {
  std::string name;
  int x = -1;
  int y = -1;
  std::vector<std::string> parts = Split(this->navigationTarget, ",");
  if (parts.size() > 2) {
    name = parts[0];
    x = std::atoi(parts[1].c_str());
    y = std::atoi(parts[2].c_str());
  }

  if (x == -1 || y == -1) {
    g_navigationManager.StartNavigation(name, 1, 5, false, -1, -1, 1002);
  } else {
    g_navigationManager.StartNavigation(name, 0, 5, false, x, y, 1002);
  }
}

// OFFSET: 0x009DBB80
// FUNCTION: fn_14311
void CUIItem::SearchNavigation() {
  const std::size_t comma = this->navigationTarget.find(',');
  if (comma == std::string::npos) {
    return;
  }

  std::string name = this->navigationTarget.substr(0, comma);
  g_navigationManager.searchText = name;
  g_navigationManager.UpdateSearchResults();
  g_navigationManager.selectedPath = 0;
  g_navigationManager.selectedPathNode = 0;
  g_navigationManager.SelectNodeState();

  UIWindow* window = g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND);
  if (!window) {
    window = g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  }
  window->SendMsg(nullptr, UM_COMMAND, 499, 0, 0, 0);
}

// OFFSET: 0x009DBCB0
// FUNCTION: fn_14312
void CUIQuest::SearchNavigation() {
  const std::size_t comma = this->navigationTarget.find(',');
  if (comma == std::string::npos) {
    return;
  }

  std::string name = this->navigationTarget.substr(0, comma);
  g_navigationManager.searchText = name;
  g_navigationManager.UpdateSearchResults();
  g_navigationManager.selectedPath = 0;
  g_navigationManager.selectedPathNode = 0;
  if (!g_navigationManager.SelectNodeState()) {
    this->RequestNavigation();
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(4241)), 0, 0, 0, 0);
    return;
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND);
  if (!window) {
    window = g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  }
  window->Invalidate();
  window->SendMsg(nullptr, UM_COMMAND, 499, 0, 0, 0);
}

}

namespace AdventureGuide {

// OFFSET: 0x009C5BB0
// FUNCTION: fn_14001
void CUIIntro::OnCreate(int width, int height) {
  this->banner = new CUIStaticImage();
  this->banner->SetSize(630, 216);
  this->banner->Move(0, 0);
  {
    this->banner->SetImage(std::string([]() {
      constexpr char bytes[] = {
        '\xAD', '\x14', '\x55', '\xD7', '\x53', '\x35', '\xD2', '\x08', '\xAB', '\x5F', '\x55', '\xE1', '\x2E', '\x41', '\x4B', '\x84', '\x09', '\xCD', '\xF0',
        '\x43', '\xE7', '\x8E', '\x65', '\xA0', '\x2A', '\xCE', '\xFC', '\x49', '\xF6', '\xA7', '\x75', '\xA2', '\x32', '\xD9', '\xF4', '\x43', '\xFD', '\x9E',
        '\x65', '\xEB', '\x0F', '\xD6', '\xE5', '\x2D'};
      constexpr EncryptedSource<44> source(bytes);
      thread_local EncryptedName<44, 0xC517FB932D95BB6DULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->banner);

  this->image = new CUIStaticImage();
  this->image->SetSize(583, 162);
  this->image->Move(22, 27);
  this->image->SetImage(std::string(""));
  this->AddChild(this->image);

  CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents = new CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>();
  this->contents = contents;
  contents->itemWidth = 603;
  contents->rowHeight = 44;
  contents->SetSize(603, 166);
  this->contents->Move(10, 207);
  this->contents->SetOnSelect([this](CUITabInfo* item, const Guide_Intro::Contents& contents) { this->OnSelectContents(item, contents); });
  this->contents->SetBinder([this](CUITabInfo* item, const Guide_Intro::Contents& contents) { this->BindContents(item, contents); });
  {
    this->contents->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\x09', '\x5A', '\x9F', '\x71', '\x4F', '\x1B', '\x7E', '\xEE', '\x0F', '\x11', '\x9F', '\x47', '\x32', '\x6F', '\xE7', '\x62', '\xAD', '\x83', '\x3A',
        '\xE5', '\xFB', '\xA0', '\xC9', '\x46', '\x8E', '\x80', '\x36', '\xEF', '\xEA', '\x89', '\xD7', '\x4A', '\xBA', '\x81', '\x00', '\xBA', '\xA1', '\xB7',
        '\xD6', '\x53', '\xC9'};
      constexpr EncryptedSource<41> source(bytes);
      thread_local EncryptedName<41, 0x23BBD58F8B5FF5C9ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x47', '\xBC', '\xAB', '\x85', '\xF9', '\xC7', '\xB4', '\xC6', '\x41', '\xF7', '\xAB', '\xB3', '\x84', '\xB3', '\x2D', '\x4A', '\xE3', '\x65', '\x0E',
        '\x11', '\x4D', '\x7C', '\x03', '\x6E', '\xC0', '\x66', '\x02', '\x1B', '\x5C', '\x55', '\x1D', '\x62', '\xF4', '\x67', '\x34', '\x4E', '\x17', '\x6B',
        '\x1C', '\x7B', '\x87'};
      constexpr EncryptedSource<41> source(bytes);
      thread_local EncryptedName<41, 0x0B7109397F6B1387ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x65', '\x32', '\xAD', '\x03', '\xA3', '\xE9', '\xB2', '\xCE', '\x63', '\x79', '\xAD', '\x35', '\xDE', '\x9D', '\x2B', '\x42', '\xC1', '\xEB', '\x08',
        '\x97', '\x17', '\x52', '\x05', '\x66', '\xE2', '\xE8', '\x04', '\x9D', '\x06', '\x7B', '\x1B', '\x6A', '\xD6', '\xE9', '\x32', '\xC8', '\x4D', '\x45',
        '\x1A', '\x73', '\xA5'};
      constexpr EncryptedSource<41> source(bytes);
      thread_local EncryptedName<41, 0x03772763F96D9DA5ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x5B', '\xF8', '\xC3', '\x63', '\xA7', '\xCB', '\xC0', '\xC8', '\x5D', '\xB3', '\xC3', '\x55', '\xDA', '\xBF', '\x59', '\x44', '\xFF', '\x21', '\x66',
        '\xF7', '\x13', '\x70', '\x77', '\x60', '\xDC', '\x22', '\x6A', '\xFD', '\x02', '\x59', '\x69', '\x6C', '\xE8', '\x23', '\x5C', '\xA8', '\x49', '\x67',
        '\x68', '\x75', '\x9B'};
      constexpr EncryptedSource<41> source(bytes);
      thread_local EncryptedName<41, 0x050505679903579BULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents = this->contents;
    contents->itemX = 0;
    contents->itemY = 0;
  }
  this->contents->scrollBar->SetSize(8, 166);
  this->contents->scrollBar->Move(595, 0);
  this->contents->scrollBar->step = 44;
  {
    this->contents->scrollBar->SetButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\x21', '\xD4', '\x39', '\x2D', '\x15', '\xDD', '\xFC', '\x10', '\x27', '\x9F', '\x39', '\x1B', '\x68', '\xA9', '\x65', '\x9C', '\x85', '\x0D', '\x9C',
        '\xB9', '\xA1', '\x66', '\x4B', '\xB8', '\xA6', '\x0E', '\x90', '\xB3', '\xB0', '\x4F', '\x4A', '\xBE', '\x93', '\x14', '\x95', '\xBB', '\x8A', '\x72',
        '\x4B', '\xAF', '\x8E', '\x0C', '\xA6', '\xA2', '\xA5', '\x3D', '\x5B', '\xB0', '\x91', '\x7B'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xDD3913D5D7F97BE1ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x67', '\x5A', '\xBF', '\xC3', '\xF1', '\x85', '\xEE', '\x72', '\x61', '\x11', '\xBF', '\xF5', '\x8C', '\xF1', '\x77', '\xFE', '\xC3', '\x83', '\x1A',
        '\x57', '\x45', '\x3E', '\x59', '\xDA', '\xE0', '\x80', '\x16', '\x5D', '\x54', '\x17', '\x58', '\xDC', '\xD5', '\x9A', '\x13', '\x55', '\x6E', '\x2A',
        '\x59', '\xCD', '\xC8', '\x82', '\x20', '\x5D', '\x5E', '\x3C', '\x45', '\x91', '\xC5', '\x98', '\x0F', '\x39'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }()));
  }
  {
    this->contents->scrollBar->SetBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x7D', '\x12', '\x2D', '\xCD', '\x7B', '\x71', '\x2A', '\x4E', '\x7B', '\x59', '\x2D', '\xFB', '\x06', '\x05', '\xB3', '\xC2', '\xD9', '\xCB', '\x88',
        '\x59', '\xCF', '\xCA', '\x9D', '\xE6', '\xFA', '\xC8', '\x84', '\x53', '\xDE', '\xE3', '\x9C', '\xE0', '\xCF', '\xD2', '\x81', '\x5B', '\xE4', '\xDD',
        '\x88', '\xDC', '\xD0', '\xD4', '\x89', '\x19', '\xD9', '\xD2', '\x9F', '\x83'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }()));
  }
  {
    this->contents->scrollBar->SetThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\xBF', '\xBC', '\x1F', '\x2B', '\x45', '\xA9', '\xBC', '\x20', '\xB9', '\xF7', '\x1F', '\x1D', '\x38', '\xDD', '\x25', '\xAC', '\x1B', '\x65', '\xBA',
        '\xBF', '\xF1', '\x12', '\x0B', '\x88', '\x38', '\x66', '\xB6', '\xB5', '\xE0', '\x3B', '\x0A', '\x8E', '\x0D', '\x7C', '\xB3', '\xBD', '\xDA', '\x13',
        '\x11', '\x98', '\x12', '\x71', '\x80', '\xA4', '\xF5', '\x49', '\x1B', '\x80', '\x0F', '\x13'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0xED796785D1DF137FULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xC9', '\x88', '\xEF', '\x41', '\x7D', '\x93', '\xB8', '\x14', '\xCF', '\xC3', '\xEF', '\x77', '\x00', '\xE7', '\x21', '\x98', '\x6D', '\x51', '\x4A',
        '\xD5', '\xC9', '\x28', '\x0F', '\xBC', '\x4E', '\x52', '\x46', '\xDF', '\xD8', '\x01', '\x0E', '\xBA', '\x7B', '\x48', '\x43', '\xD7', '\xE2', '\x29',
        '\x15', '\xAC', '\x64', '\x45', '\x70', '\xD6', '\xD4', '\x39', '\x53', '\xBB', '\x64', '\x57', '\x2F'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xD97D5DBDBB2F2709ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xE4', '\xFB', '\xB3', '\x08',
        '\xC5', '\xA3', '\x5C', '\x8F', '\xC0', '\xD8', '\xB0', '\x04', '\xCF', '\xB2', '\x75', '\x8E', '\xC6', '\xED', '\xAA', '\x01', '\xC7', '\x88', '\x5D',
        '\x95', '\xD0', '\xF2', '\xA7', '\x32', '\xCF', '\xB8', '\x5E', '\x93', '\x8B', '\xFD', '\xA8', '\x1D', '\xAB'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }()));
  }
  this->contents->AddNewItem();
  this->AddChild(this->contents);
}


// OFFSET: 0x009C6F50
// FUNCTION: fn_14002
void CUIItem::OnCreate(int width, int height) {
  CUIListBoxEx<CUIItemInfo, CItemData>* itemList = new CUIListBoxEx<CUIItemInfo, CItemData>();
  this->itemList = itemList;
  itemList->itemWidth = 603;
  itemList->rowHeight = 128;
  itemList->SetSize(627, 355);
  this->itemList->Move(0, 0);
  this->itemList->SetOnSelect([this](CUIItemInfo* item, const CItemData& data) { this->OnSelectItem(item, data); });
  this->itemList->SetBinder([this](CUIItemInfo* item, const CItemData& data) { this->BindItem(item, data); });
  {
    const char listDisabledSource[] = {
        '\xD1', '\x4C', '\x93', '\x63', '\x71', '\x93', '\xB0', '\x86', '\xD7', '\x07', '\x93', '\x55', '\x0C', '\xE7', '\x29', '\x0A', '\x75', '\x95', '\x36',
        '\xF7', '\xC5', '\x28', '\x07', '\x2E', '\x56', '\x96', '\x3A', '\xFD', '\xD4', '\x01', '\x19', '\x22', '\x62', '\x97', '\x0C', '\xAA', '\xEE', '\x33',
        '\x1A', '\x39', '\x7C', '\x82', '\x3F', '\xB7', '\xD3', '\x30', '\x05', '\x4B'};
    thread_local EncryptedName<sizeof(listDisabledSource), 0x4B755DB19953E311ULL> listDisabledName(listDisabledSource);
    const std::string listDisabled(listDisabledName.Text());
    const char listOverSource[] = {
        '\xFB', '\x48', '\xC1', '\xE1', '\x01', '\x6F', '\xEE', '\x70', '\xFD', '\x03', '\xC1', '\xD7', '\x7C', '\x1B', '\x77', '\xFC', '\x5F', '\x91', '\x64',
        '\x75', '\xB5', '\xD4', '\x59', '\xD8', '\x7C', '\x92', '\x68', '\x7F', '\xA4', '\xFD', '\x47', '\xD4', '\x48', '\x93', '\x5E', '\x28', '\x9E', '\xCE',
        '\x5D', '\xD8', '\x49', '\xC9', '\x63', '\x76', '\xB1', '\xA1'};
    thread_local EncryptedName<sizeof(listOverSource), 0xBD2BA1C11B01E73BULL> listOverName(listOverSource);
    const std::string listOver(listOverName.Text());
    const char listPressSource[] = {
        '\x79', '\x82', '\xFD', '\xC9', '\x4D', '\x5D', '\xD0', '\x20', '\x7F', '\xC9', '\xFD', '\xFF', '\x30', '\x29', '\x49', '\xAC', '\xDD', '\x5B', '\x58',
        '\x5D', '\xF9', '\xE6', '\x67', '\x88', '\xFE', '\x58', '\x54', '\x57', '\xE8', '\xCF', '\x79', '\x84', '\xCA', '\x59', '\x62', '\x00', '\xD2', '\xE3',
        '\x67', '\x88', '\xCA', '\x5E', '\x13', '\x51', '\xE0', '\xE3', '\x15'};
    thread_local EncryptedName<sizeof(listPressSource), 0xED15938D333D2DB9ULL> listPressName(listPressSource);
    const std::string listPress(listPressName.Text());
    const char listNormalSource[] = {
        '\x83', '\x96', '\x9B', '\x91', '\xA7', '\xE3', '\xB4', '\x04', '\x85', '\xDD', '\x9B', '\xA7', '\xDA', '\x97', '\x2D', '\x88', '\x27', '\x4F', '\x3E',
        '\x05', '\x13', '\x58', '\x03', '\xAC', '\x04', '\x4C', '\x32', '\x0F', '\x02', '\x71', '\x1D', '\xA0', '\x30', '\x4D', '\x04', '\x58', '\x38', '\x43',
        '\x1E', '\xBB', '\x2E', '\x58', '\x37', '\x45', '\x05', '\x40', '\x01', '\xC9'};
    thread_local EncryptedName<sizeof(listNormalSource), 0xC9712D676B5B3943ULL> listNormalName(listNormalSource);
    const std::string listNormal(listNormalName.Text());
    this->itemList->SetImages(listNormal, listPress, listOver, listDisabled);
  }
  this->itemList->itemX = 0;
  this->itemList->itemY = 0;
  this->itemList->scrollBar->SetSize(8, 355);
  this->itemList->scrollBar->Move(619, 0);
  this->itemList->scrollBar->step = 20;
  {
    const char downSource[] = {
        '\xB3', '\x88', '\x8D', '\xB7', '\x39', '\xA1', '\xC0', '\x1C', '\xB5', '\xC3', '\x8D', '\x81', '\x44', '\xD5', '\x59', '\x90', '\x17', '\x51', '\x28',
        '\x23', '\x8D', '\x1A', '\x77', '\xB4', '\x34', '\x52', '\x24', '\x29', '\x9C', '\x33', '\x76', '\xB2', '\x01', '\x48', '\x21', '\x21', '\xA6', '\x0E',
        '\x77', '\xA3', '\x1C', '\x50', '\x12', '\x29', '\x96', '\x18', '\x6B', '\xFF', '\x11', '\x4A', '\x3D', '\x4D'};
    thread_local EncryptedName<sizeof(downSource), 0xD1056FF94D4D2773ULL> downName(downSource);
    const std::string down(downName.Text());
    const char upSource[] = {
        '\x2B', '\xF0', '\xC9', '\xC9', '\xBF', '\x6F', '\xF4', '\x8E', '\x2D', '\xBB', '\xC9', '\xFF', '\xC2', '\x1B', '\x6D', '\x02', '\x8F', '\x29', '\x6C',
        '\x5D', '\x0B', '\xD4', '\x43', '\x26', '\xAC', '\x2A', '\x60', '\x57', '\x1A', '\xFD', '\x42', '\x20', '\x99', '\x30', '\x65', '\x5F', '\x20', '\xC0',
        '\x43', '\x31', '\x84', '\x28', '\x56', '\x46', '\x0F', '\x8F', '\x53', '\x2E', '\x9B', '\x5F'};
    thread_local EncryptedName<sizeof(upSource), 0x4331A17F33095FEBULL> upName(upSource);
    const std::string up(upName.Text());
    this->itemList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char backgroundSource[] = {
        '\xD7', '\x76', '\xF3', '\x53', '\xB9', '\x29', '\x52', '\xC2', '\xD1', '\x3D', '\xF3', '\x65', '\xC4', '\x5D', '\xCB', '\x4E', '\x73', '\xAF', '\x56',
        '\xC7', '\x0D', '\x92', '\xE5', '\x6A', '\x50', '\xAC', '\x5A', '\xCD', '\x1C', '\xBB', '\xE4', '\x6C', '\x65', '\xB6', '\x5F', '\xC5', '\x26', '\x85',
        '\xF0', '\x50', '\x7A', '\xB0', '\x57', '\x87', '\x1B', '\x8A', '\xE7', '\x0F'};
    thread_local EncryptedName<sizeof(backgroundSource), 0x0F97E779A933D917ULL> backgroundName(backgroundSource);
    const std::string background(backgroundName.Text());
    this->itemList->scrollBar->SetBackground(background);
  }
  {
    const char thumbBottomSource[] = {
        '\xB5', '\x90', '\xA1', '\x51', '\xEF', '\xBF', '\x22', '\x90', '\xB3', '\xDB', '\xA1', '\x67', '\x92', '\xCB', '\xBB', '\x1C', '\x11', '\x49', '\x04',
        '\xC5', '\x5B', '\x04', '\x95', '\x38', '\x32', '\x4A', '\x08', '\xCF', '\x4A', '\x2D', '\x94', '\x3E', '\x07', '\x50', '\x0D', '\xC7', '\x70', '\x05',
        '\x8F', '\x28', '\x18', '\x5D', '\x3E', '\xCF', '\x40', '\x06', '\x89', '\x73', '\x17', '\x52', '\x11', '\xAB'};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x5DE7712FAB613F75ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const char thumbMiddleSource[] = {
        '\x51', '\xC4', '\xAB', '\xE5', '\x43', '\x05', '\x0A', '\x8C', '\x57', '\x8F', '\xAB', '\xD3', '\x3E', '\x71', '\x93', '\x00', '\xF5', '\x1D', '\x0E',
        '\x71', '\xF7', '\xBE', '\xBD', '\x24', '\xD6', '\x1E', '\x02', '\x7B', '\xE6', '\x97', '\xBC', '\x22', '\xE3', '\x04', '\x07', '\x73', '\xDC', '\xBF',
        '\xA7', '\x34', '\xFC', '\x09', '\x34', '\x72', '\xEA', '\xAF', '\xE1', '\x23', '\xFC', '\x1B', '\x6B'};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x41CFCB831F6B6B91ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const char thumbTopSource[] = {
        '\x81', '\x7E', '\x21', '\x19', '\xAB', '\x87', '\x4C', '\xFA', '\x87', '\x35', '\x21', '\x2F', '\xD6', '\xF3', '\xD5', '\x76', '\x25', '\xA7', '\x84',
        '\x8D', '\x1F', '\x3C', '\xFB', '\x52', '\x06', '\xA4', '\x88', '\x87', '\x0E', '\x15', '\xFA', '\x54', '\x33', '\xBE', '\x8D', '\x8F', '\x34', '\x3D',
        '\xE1', '\x42', '\x2C', '\xB3', '\xBE', '\x96', '\x1B', '\x67', '\xEB', '\x5A', '\x31', '\xD1'};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x3789496BE3E1D141ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->itemList->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->itemList->AddNewItem();
  this->AddChild(this->itemList);

  this->findButton = new CUIImageButton();
  this->findButton->SetSize(62, 20);
  this->findButton->Move(471, 363);
  this->findButton->SetOnClick([this]() { this->RequestNavigation(); });
  this->findButton->disabled = 0;
  this->findButton->Invalidate();
  {
    const char findDisableSource[] = {
        '\xE7', '\x0A', '\x53', '\x65', '\xC1', '\x9D', '\xA4', '\x60', '\xE1', '\x41', '\x53', '\x53', '\xBC', '\xE9', '\x3D', '\xEC', '\x43', '\xD3', '\xF6',
        '\xF1', '\x75', '\x26', '\x13', '\xC8', '\x60', '\xD0', '\xFA', '\xFB', '\x64', '\x0F', '\x03', '\xD9', '\x49', '\xFA', '\xF5', '\xF6', '\x6F', '\x37',
        '\x3E', '\xC9', '\x4E', '\xD6', '\xF2', '\xFD', '\x6D', '\x36', '\x4F', '\xCF', '\x4A', '\xD5', '\x93'};
    thread_local EncryptedName<sizeof(findDisableSource), 0xAD6153019F93A527ULL> findDisableName(findDisableSource);
    const std::string findDisable(findDisableName.Text());
    const char findOverSource[] = {
        '\xBF', '\xF4', '\xB5', '\x5F', '\x67', '\x3B', '\x24', '\x48', '\xB9', '\xBF', '\xB5', '\x69', '\x1A', '\x4F', '\xBD', '\xC4', '\x1B', '\x2D', '\x10',
        '\xCB', '\xD3', '\x80', '\x93', '\xE0', '\x38', '\x2E', '\x1C', '\xC1', '\xC2', '\xA9', '\x83', '\xF1', '\x11', '\x04', '\x13', '\xCC', '\xC9', '\x91',
        '\xBE', '\xEA', '\x09', '\x3E', '\x07', '\x8B', '\xC5', '\x98', '\x91', '\x85'};
    thread_local EncryptedName<sizeof(findOverSource), 0x85E1F5A7A5755B7FULL> findOverName(findOverSource);
    const std::string findOver(findOverName.Text());
    const char findPressSource[] = {
        '\x0B', '\x42', '\x73', '\xD1', '\x47', '\xFF', '\x88', '\x88', '\x0D', '\x09', '\x73', '\xE7', '\x3A', '\x8B', '\x11', '\x04', '\xAF', '\x9B', '\xD6',
        '\x45', '\xF3', '\x44', '\x3F', '\x20', '\x8C', '\x98', '\xDA', '\x4F', '\xE2', '\x6D', '\x2F', '\x31', '\xA5', '\xB2', '\xD5', '\x42', '\xE9', '\x55',
        '\x12', '\x35', '\xB9', '\x88', '\xC0', '\x58', '\xA9', '\x53', '\x20', '\x35', '\xCB'};
    thread_local EncryptedName<sizeof(findPressSource), 0x454D31872BB3EDCBULL> findPressName(findPressSource);
    const std::string findPress(findPressName.Text());
    const char findNormalSource[] = {
        '\xC1', '\xA2', '\x87', '\xA7', '\x2D', '\x43', '\x30', '\x4C', '\xC7', '\xE9', '\x87', '\x91', '\x50', '\x37', '\xA9', '\xC0', '\x65', '\x7B', '\x22',
        '\x33', '\x99', '\xF8', '\x87', '\xE4', '\x46', '\x78', '\x2E', '\x39', '\x88', '\xD1', '\x97', '\xF5', '\x6F', '\x52', '\x21', '\x34', '\x83', '\xE9',
        '\xAA', '\xEF', '\x6E', '\x7F', '\x2A', '\x3C', '\x81', '\xA3', '\x97', '\xEC', '\x71', '\x0D'};
    thread_local EncryptedName<sizeof(findNormalSource), 0x81F58DED5D470D01ULL> findNormalName(findNormalSource);
    const std::string findNormal(findNormalName.Text());
    this->findButton->SetImages(findNormal, findPress, findOver, findDisable);
  }
  this->AddChild(this->findButton);

  this->navigationButton = new CUIImageButton();
  this->navigationButton->SetSize(62, 20);
  this->navigationButton->Move(540, 363);
  this->navigationButton->SetOnClick([this]() { this->SearchNavigation(); });
  this->navigationButton->disabled = 0;
  this->navigationButton->Invalidate();
  {
    const char moveDisableSource[] = {
        '\x09', '\xAE', '\x2D', '\x15', '\x27', '\xA7', '\xDA', '\x6E', '\x0F', '\xE5', '\x2D', '\x23', '\x5A', '\xD3', '\x43', '\xE2', '\xAD', '\x77', '\x88',
        '\x81', '\x93', '\x1C', '\x6D', '\xC6', '\x8E', '\x74', '\x84', '\x8B', '\x82', '\x35', '\x7D', '\xD7', '\xA7', '\x5E', '\x80', '\x80', '\x91', '\x0C',
        '\x40', '\xC7', '\xA0', '\x72', '\x8C', '\x8D', '\x8B', '\x0C', '\x31', '\xC1', '\xA4', '\x71', '\xED'};
    thread_local EncryptedName<sizeof(moveDisableSource), 0xA31F69E7EFED01C9ULL> moveDisableName(moveDisableSource);
    const std::string moveDisable(moveDisableName.Text());
    const char moveOverSource[] = {
        '\xAB', '\xE4', '\x7B', '\xFB', '\x47', '\xA1', '\x6C', '\x7A', '\xAD', '\xAF', '\x7B', '\xCD', '\x3A', '\xD5', '\xF5', '\xF6', '\x0F', '\x3D', '\xDE',
        '\x6F', '\xF3', '\x1A', '\xDB', '\xD2', '\x2C', '\x3E', '\xD2', '\x65', '\xE2', '\x33', '\xCB', '\xC3', '\x05', '\x14', '\xD6', '\x6E', '\xF1', '\x0A',
        '\xF6', '\xD8', '\x1D', '\x2E', '\xC9', '\x2F', '\xE5', '\x02', '\xD9', '\xB7'};
    thread_local EncryptedName<sizeof(moveOverSource), 0xB7A96F8701BB4B6BULL> moveOverName(moveOverSource);
    const std::string moveOver(moveOverName.Text());
    const char movePressSource[] = {
        '\xFD', '\xD2', '\x95', '\xDB', '\x01', '\xBB', '\x5C', '\xA8', '\xFB', '\x99', '\x95', '\xED', '\x7C', '\xCF', '\xC5', '\x24', '\x59', '\x0B', '\x30',
        '\x4F', '\xB5', '\x00', '\xEB', '\x00', '\x7A', '\x08', '\x3C', '\x45', '\xA4', '\x29', '\xFB', '\x11', '\x53', '\x22', '\x38', '\x4E', '\xB7', '\x10',
        '\xC6', '\x15', '\x4F', '\x18', '\x26', '\x52', '\xEF', '\x17', '\xF4', '\x15', '\x3D'};
    thread_local EncryptedName<sizeof(movePressSource), 0x659975C121557D3DULL> movePressName(movePressSource);
    const std::string movePress(movePressName.Text());
    const char moveNormalSource[] = {
        '\x99', '\xD0', '\x67', '\x5B', '\xD7', '\x0B', '\x30', '\x72', '\x9F', '\x9B', '\x67', '\x6D', '\xAA', '\x7F', '\xA9', '\xFE', '\x3D', '\x09', '\xC2',
        '\xCF', '\x63', '\xB0', '\x87', '\xDA', '\x1E', '\x0A', '\xCE', '\xC5', '\x72', '\x99', '\x97', '\xCB', '\x37', '\x20', '\xCA', '\xCE', '\x61', '\xA0',
        '\xAA', '\xD1', '\x36', '\x0D', '\xCA', '\xC0', '\x7B', '\xEB', '\x97', '\xD2', '\x29', '\x7F'};
    thread_local EncryptedName<sizeof(moveNormalSource), 0xBFF5C517A1A77F59ULL> moveNormalName(moveNormalSource);
    const std::string moveNormal(moveNormalName.Text());
    this->navigationButton->SetImages(moveNormal, movePress, moveOver, moveDisable);
  }
  this->AddChild(this->navigationButton);
}


// OFFSET: 0x009CDD40
// FUNCTION: fn_14006
void CUIQuest::OnCreate(int width, int height) {
  CUIListBoxEx<CUIQuestInfo, CQuestInfo>* questList = new CUIListBoxEx<CUIQuestInfo, CQuestInfo>();
  this->questList = questList;
  questList->itemWidth = 603;
  questList->rowHeight = 97;
  questList->SetSize(627, 355);
  this->questList->Move(0, 0);
  this->questList->SetOnSelect([this](CUIQuestInfo* item, const CQuestInfo& data) { this->OnSelectQuest(item, data); });
  this->questList->SetBinder([this](CUIQuestInfo* item, const CQuestInfo& data) { BindQuestInfo(item, data); });
  {
    const char listDisabledSource[] = {
        '\xD7', '\xB4', '\xED', '\x59', '\x7D', '\x25', '\x84', '\xC2', '\xD1', '\xFF', '\xED', '\x6F', '\x00', '\x51', '\x1D', '\x4E', '\x73', '\x6D', '\x48',
        '\xCD', '\xC9', '\x9E', '\x33', '\x6A', '\x50', '\x6E', '\x44', '\xC7', '\xD8', '\xB7', '\x2D', '\x66', '\x64', '\x6F', '\x72', '\x91', '\xE2', '\x85',
        '\x2E', '\x7D', '\x7A', '\x7A', '\x41', '\x8D', '\xDF', '\x86', '\x31', '\x0F'};
    thread_local EncryptedName<sizeof(listDisabledSource), 0x0F41EBBDA32D1B17ULL> listDisabledName(listDisabledSource);
    const std::string listDisabled(listDisabledName.Text());
    const char listOverSource[] = {
        '\x01', '\xE8', '\x05', '\xED', '\x21', '\x57', '\x10', '\x36', '\x07', '\xA3', '\x05', '\xDB', '\x5C', '\x23', '\x89', '\xBA', '\xA5', '\x31', '\xA0',
        '\x79', '\x95', '\xEC', '\xA7', '\x9E', '\x86', '\x32', '\xAC', '\x73', '\x84', '\xC5', '\xB9', '\x92', '\xB2', '\x33', '\x9A', '\x25', '\xBE', '\xF6',
        '\xA3', '\x9E', '\xB3', '\x69', '\xA7', '\x7A', '\x91', '\x99'};
    thread_local EncryptedName<sizeof(listOverSource), 0xFBD599E117C547C1ULL> listOverName(listOverSource);
    const std::string listOver(listOverName.Text());
    const char listPressSource[] = {
        '\xA5', '\x5C', '\x17', '\xA7', '\x4D', '\xDD', '\xAA', '\xDA', '\xA3', '\x17', '\x17', '\x91', '\x30', '\xA9', '\x33', '\x56', '\x01', '\x85', '\xB2',
        '\x33', '\xF9', '\x66', '\x1D', '\x72', '\x22', '\x86', '\xBE', '\x39', '\xE8', '\x4F', '\x03', '\x7E', '\x16', '\x87', '\x88', '\x6F', '\xD2', '\x63',
        '\x1D', '\x72', '\x16', '\x80', '\xF9', '\x3F', '\xE0', '\x63', '\x6F'};
    thread_local EncryptedName<sizeof(listPressSource), 0x176F138D5DD7F365ULL> listPressName(listPressSource);
    const std::string listPress(listPressName.Text());
    const char listNormalSource[] = {
        '\x8F', '\xC8', '\x2D', '\x7F', '\xC9', '\xF9', '\xFA', '\xB4', '\x89', '\x83', '\x2D', '\x49', '\xB4', '\x8D', '\x63', '\x38', '\x2B', '\x11', '\x88',
        '\xEB', '\x7D', '\x42', '\x4D', '\x1C', '\x08', '\x12', '\x84', '\xE1', '\x6C', '\x6B', '\x53', '\x10', '\x3C', '\x13', '\xB2', '\xB7', '\x56', '\x59',
        '\x50', '\x0B', '\x22', '\x06', '\x81', '\xAB', '\x6B', '\x5A', '\x4F', '\x79'};
    thread_local EncryptedName<sizeof(listNormalSource), 0x793F370985ED674FULL> listNormalName(listNormalSource);
    const std::string listNormal(listNormalName.Text());
    this->questList->SetImages(listNormal, listPress, listOver, listDisabled);
  }
  this->questList->itemX = 0;
  this->questList->itemY = 0;
  this->questList->scrollBar->SetSize(8, 355);
  this->questList->scrollBar->Move(619, 0);
  this->questList->scrollBar->step = 20;
  {
    const char downSource[] = {
        '\xAD', '\xB4', '\x67', '\xEB', '\xB7', '\xA1', '\x0E', '\x3E', '\xAB', '\xFF', '\x67', '\xDD', '\xCA', '\xD5', '\x97', '\xB2', '\x09', '\x6D', '\xC2',
        '\x7F', '\x03', '\x1A', '\xB9', '\x96', '\x2A', '\x6E', '\xCE', '\x75', '\x12', '\x33', '\xB8', '\x90', '\x1F', '\x74', '\xCB', '\x7D', '\x28', '\x0E',
        '\xB9', '\x81', '\x02', '\x6C', '\xF8', '\x75', '\x18', '\x18', '\xA5', '\xDD', '\x0F', '\x76', '\xD7', '\x11'};
    thread_local EncryptedName<sizeof(downSource), 0xF3CB6F7711A71B6DULL> downName(downSource);
    const std::string down(downName.Text());
    const char upSource[] = {
        '\x2D', '\xDA', '\xA1', '\xD9', '\x77', '\x2D', '\x94', '\xE0', '\x2B', '\x91', '\xA1', '\xEF', '\x0A', '\x59', '\x0D', '\x6C', '\x89', '\x03', '\x04',
        '\x4D', '\xC3', '\x96', '\x23', '\x48', '\xAA', '\x00', '\x08', '\x47', '\xD2', '\xBF', '\x22', '\x4E', '\x9F', '\x1A', '\x0D', '\x4F', '\xE8', '\x82',
        '\x23', '\x5F', '\x82', '\x02', '\x3E', '\x56', '\xC7', '\xCD', '\x33', '\x40', '\x9D', '\x75'};
    thread_local EncryptedName<sizeof(upSource), 0x2D51E3B7236175EDULL> upName(upSource);
    const std::string up(upName.Text());
    this->questList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char backgroundSource[] = {
        '\x73', '\xC8', '\x99', '\x75', '\x1F', '\xE9', '\x5E', '\x4E', '\x75', '\x83', '\x99', '\x43', '\x62', '\x9D', '\xC7', '\xC2', '\xD7', '\x11', '\x3C',
        '\xE1', '\xAB', '\x52', '\xE9', '\xE6', '\xF4', '\x12', '\x30', '\xEB', '\xBA', '\x7B', '\xE8', '\xE0', '\xC1', '\x08', '\x35', '\xE3', '\x80', '\x45',
        '\xFC', '\xDC', '\xDE', '\x0E', '\x3D', '\xA1', '\xBD', '\x4A', '\xEB', '\x83'};
    thread_local EncryptedName<sizeof(backgroundSource), 0x839B27DF8F5967B3ULL> backgroundName(backgroundSource);
    const std::string background(backgroundName.Text());
    this->questList->scrollBar->SetBackground(background);
  }
  {
    const char thumbBottomSource[] = {
        '\x99', '\x18', '\xC7', '\xE9', '\x05', '\xB3', '\xA6', '\x82', '\x9F', '\x53', '\xC7', '\xDF', '\x78', '\xC7', '\x3F', '\x0E', '\x3D', '\xC1', '\x62',
        '\x7D', '\xB1', '\x08', '\x11', '\x2A', '\x1E', '\xC2', '\x6E', '\x77', '\xA0', '\x21', '\x10', '\x2C', '\x2B', '\xD8', '\x6B', '\x7F', '\x9A', '\x09',
        '\x0B', '\x3A', '\x34', '\xD5', '\x58', '\x77', '\xAA', '\x0A', '\x0D', '\x61', '\x3B', '\xDA', '\x77', '\x13'};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x4F637DC51307B759ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const char thumbMiddleSource[] = {
        '\xA5', '\xFA', '\x09', '\x85', '\xCD', '\x21', '\xEA', '\x88', '\xA3', '\xB1', '\x09', '\xB3', '\xB0', '\x55', '\x73', '\x04', '\x01', '\x23', '\xAC',
        '\x11', '\x79', '\x9A', '\x5D', '\x20', '\x22', '\x20', '\xA0', '\x1B', '\x68', '\xB3', '\x5C', '\x26', '\x17', '\x3A', '\xA5', '\x13', '\x52', '\x9B',
        '\x47', '\x30', '\x08', '\x37', '\x96', '\x12', '\x64', '\x8B', '\x01', '\x27', '\x08', '\x25', '\xC9'};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x452FEF0D7FC95565ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const char thumbTopSource[] = {
        '\x9D', '\xA2', '\x89', '\x73', '\x61', '\xB3', '\x28', '\x52', '\x9B', '\xE9', '\x89', '\x45', '\x1C', '\xC7', '\xB1', '\xDE', '\x39', '\x7B', '\x2C',
        '\xE7', '\xD5', '\x08', '\x9F', '\xFA', '\x1A', '\x78', '\x20', '\xED', '\xC4', '\x21', '\x9E', '\xFC', '\x2F', '\x62', '\x25', '\xE5', '\xFE', '\x09',
        '\x85', '\xEA', '\x30', '\x6F', '\x16', '\xFC', '\xD1', '\x53', '\x8F', '\xF2', '\x2D', '\x0D'};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x9FED7DA189490D5DULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->questList->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->questList->AddNewItem();
  this->AddChild(this->questList);

  this->findButton = new CUIImageButton();
  this->findButton->SetSize(62, 20);
  this->findButton->Move(471, 363);
  this->findButton->SetOnClick([this]() { this->RequestNavigation(); });
  this->findButton->disabled = 0;
  this->findButton->Invalidate();
  {
    const char findDisableSource[] = {
        '\x77', '\x9E', '\x2F', '\x39', '\xC3', '\x11', '\x5C', '\x82', '\x71', '\xD5', '\x2F', '\x0F', '\xBE', '\x65', '\xC5', '\x0E', '\xD3', '\x47', '\x8A',
        '\xAD', '\x77', '\xAA', '\xEB', '\x2A', '\xF0', '\x44', '\x86', '\xA7', '\x66', '\x83', '\xFB', '\x3B', '\xD9', '\x6E', '\x89', '\xAA', '\x6D', '\xBB',
        '\xC6', '\x2B', '\xDE', '\x42', '\x8E', '\xA1', '\x6F', '\xBA', '\xB7', '\x2D', '\xDA', '\x41', '\xEF'};
    thread_local EncryptedName<sizeof(findDisableSource), 0x4F99DF03C3EF31B7ULL> findDisableName(findDisableSource);
    const std::string findDisable(findDisableName.Text());
    const char findOverSource[] = {
        '\x37', '\xBC', '\xEB', '\x55', '\x31', '\xC1', '\xD8', '\x40', '\x31', '\xF7', '\xEB', '\x63', '\x4C', '\xB5', '\x41', '\xCC', '\x93', '\x65', '\x4E',
        '\xC1', '\x85', '\x7A', '\x6F', '\xE8', '\xB0', '\x66', '\x42', '\xCB', '\x94', '\x53', '\x7F', '\xF9', '\x99', '\x4C', '\x4D', '\xC6', '\x9F', '\x6B',
        '\x42', '\xE2', '\x81', '\x76', '\x59', '\x81', '\x93', '\x62', '\x6D', '\x8D'};
    thread_local EncryptedName<sizeof(findOverSource), 0x8D1D0FF1AF2B13F7ULL> findOverName(findOverSource);
    const std::string findOver(findOverName.Text());
    const char findPressSource[] = {
        '\x4D', '\x14', '\x0F', '\x81', '\x51', '\x69', '\x5A', '\x7E', '\x4B', '\x5F', '\x0F', '\xB7', '\x2C', '\x1D', '\xC3', '\xF2', '\xE9', '\xCD', '\xAA',
        '\x15', '\xE5', '\xD2', '\xED', '\xD6', '\xCA', '\xCE', '\xA6', '\x1F', '\xF4', '\xFB', '\xFD', '\xC7', '\xE3', '\xE4', '\xA9', '\x12', '\xFF', '\xC3',
        '\xC0', '\xC3', '\xFF', '\xDE', '\xBC', '\x08', '\xBF', '\xC5', '\xF2', '\xC3', '\x8D'};
    thread_local EncryptedName<sizeof(findPressSource), 0xB39FA7917BCFBB8DULL> findPressName(findPressSource);
    const std::string findPress(findPressName.Text());
    const char findNormalSource[] = {
        '\x6B', '\x6E', '\x8D', '\xC3', '\xA7', '\xBB', '\x0E', '\xE0', '\x6D', '\x25', '\x8D', '\xF5', '\xDA', '\xCF', '\x97', '\x6C', '\xCF', '\xB7', '\x28',
        '\x57', '\x13', '\x00', '\xB9', '\x48', '\xEC', '\xB4', '\x24', '\x5D', '\x02', '\x29', '\xA9', '\x59', '\xC5', '\x9E', '\x2B', '\x50', '\x09', '\x11',
        '\x94', '\x43', '\xC4', '\xB3', '\x20', '\x58', '\x0B', '\x5B', '\xA9', '\x40', '\xDB', '\xC1'};
    thread_local EncryptedName<sizeof(findNormalSource), 0x2DCB7567394DC1ABULL> findNormalName(findNormalSource);
    const std::string findNormal(findNormalName.Text());
    this->findButton->SetImages(findNormal, findPress, findOver, findDisable);
  }
  this->AddChild(this->findButton);

  this->navigationButton = new CUIImageButton();
  this->navigationButton->SetSize(62, 20);
  this->navigationButton->Move(540, 363);
  this->navigationButton->SetOnClick([this]() { this->SearchNavigation(); });
  this->navigationButton->disabled = 0;
  this->navigationButton->Invalidate();
  {
    const char moveDisableSource[] = {
        '\xDD', '\x68', '\x8B', '\x4F', '\x1D', '\xB9', '\x8C', '\xEE', '\xDB', '\x23', '\x8B', '\x79', '\x60', '\xCD', '\x15', '\x62', '\x79', '\xB1', '\x2E',
        '\xDB', '\xA9', '\x02', '\x3B', '\x46', '\x5A', '\xB2', '\x22', '\xD1', '\xB8', '\x2B', '\x2B', '\x57', '\x73', '\x98', '\x26', '\xDA', '\xAB', '\x12',
        '\x16', '\x47', '\x74', '\xB4', '\x2A', '\xD7', '\xB1', '\x12', '\x67', '\x41', '\x70', '\xB7', '\x4B'};
    thread_local EncryptedName<sizeof(moveDisableSource), 0x234977DDB54BC71DULL> moveDisableName(moveDisableSource);
    const std::string moveDisable(moveDisableName.Text());
    const char moveOverSource[] = {
        '\xE5', '\xDE', '\x6F', '\x31', '\xCD', '\x3D', '\xE8', '\x28', '\xE3', '\x95', '\x6F', '\x07', '\xB0', '\x49', '\x71', '\xA4', '\x41', '\x07', '\xCA',
        '\xA5', '\x79', '\x86', '\x5F', '\x80', '\x62', '\x04', '\xC6', '\xAF', '\x68', '\xAF', '\x4F', '\x91', '\x4B', '\x2E', '\xC2', '\xA4', '\x7B', '\x96',
        '\x72', '\x8A', '\x53', '\x14', '\xDD', '\xE5', '\x6F', '\x9E', '\x5D', '\xE5'};
    thread_local EncryptedName<sizeof(moveOverSource), 0xE52DF30DCBAF7125ULL> moveOverName(moveOverSource);
    const std::string moveOver(moveOverName.Text());
    const char movePressSource[] = {
        '\x9F', '\x9A', '\x6F', '\x6F', '\x5D', '\x47', '\x36', '\x02', '\x99', '\xD1', '\x6F', '\x59', '\x20', '\x33', '\xAF', '\x8E', '\x3B', '\x43', '\xCA',
        '\xFB', '\xE9', '\xFC', '\x81', '\xAA', '\x18', '\x40', '\xC6', '\xF1', '\xF8', '\xD5', '\x91', '\xBB', '\x31', '\x6A', '\xC2', '\xFA', '\xEB', '\xEC',
        '\xAC', '\xBF', '\x2D', '\x50', '\xDC', '\xE6', '\xB3', '\xEB', '\x9E', '\xBF', '\x5F'};
    thread_local EncryptedName<sizeof(movePressSource), 0xCFF3899D95AF355FULL> movePressName(movePressSource);
    const std::string movePress(movePressName.Text());
    const char moveNormalSource[] = {
        '\x37', '\x66', '\xA3', '\x93', '\xBD', '\x57', '\xB4', '\x5C', '\x31', '\x2D', '\xA3', '\xA5', '\xC0', '\x23', '\x2D', '\xD0', '\x93', '\xBF', '\x06',
        '\x07', '\x09', '\xEC', '\x03', '\xF4', '\xB0', '\xBC', '\x0A', '\x0D', '\x18', '\xC5', '\x13', '\xE5', '\x99', '\x96', '\x0E', '\x06', '\x0B', '\xFC',
        '\x2E', '\xFF', '\x98', '\xBB', '\x0E', '\x08', '\x11', '\xB7', '\x13', '\xFC', '\x87', '\xC9'};
    thread_local EncryptedName<sizeof(moveNormalSource), 0x9171997D6963C9F7ULL> moveNormalName(moveNormalSource);
    const std::string moveNormal(moveNormalName.Text());
    this->navigationButton->SetImages(moveNormal, movePress, moveOver, moveDisable);
  }
  this->AddChild(this->navigationButton);
}


// OFFSET: 0x009CAE30
// FUNCTION: fn_14004
void CUIMonster::OnCreate(int width, int height) {
  CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList = new CUIListBoxEx<CUIMonsterInfo, CMonsterData>();
  this->monsterList = monsterList;
  monsterList->itemWidth = 603;
  monsterList->rowHeight = 128;
  monsterList->SetSize(627, 355);
  this->monsterList->Move(0, 0);
  this->monsterList->SetOnSelect([this](CUIMonsterInfo* item, const CMonsterData& data) { this->OnSelectMonster(item, data); });
  this->monsterList->SetBinder([this](CUIMonsterInfo* item, const CMonsterData& data) { this->BindMonster(item, data); });
  {
    this->monsterList->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\x99', '\x2A', '\x47', '\x83', '\x37', '\xE1', '\xFA', '\x2E', '\x9F', '\x61', '\x47', '\xB5', '\x4A', '\x95', '\x63', '\xA2', '\x3D', '\xF3', '\xE2',
        '\x17', '\x83', '\x5A', '\x4D', '\x86', '\x1E', '\xF0', '\xEE', '\x1D', '\x92', '\x73', '\x53', '\x8A', '\x2A', '\xF1', '\xD8', '\x4A', '\xA8', '\x41',
        '\x50', '\x91', '\x34', '\xE4', '\xEB', '\x57', '\x95', '\x42', '\x4F', '\xE3'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xE33F2FF779878559ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x7F', '\xC6', '\x23', '\x37', '\x45', '\x49', '\x9A', '\x3C', '\x79', '\x8D', '\x23', '\x01', '\x38', '\x3D', '\x03', '\xB0', '\xDB', '\x1F', '\x86',
        '\xA3', '\xF1', '\xF2', '\x2D', '\x94', '\xF8', '\x1C', '\x8A', '\xA9', '\xE0', '\xDB', '\x33', '\x98', '\xCC', '\x1D', '\xBC', '\xFE', '\xDA', '\xF7',
        '\x2D', '\x94', '\xCC', '\x1A', '\xCD', '\xAF', '\xE8', '\xF7', '\x5F'};
      constexpr EncryptedSource<47> source(bytes);
      thread_local EncryptedName<47, 0xF15F8785CDE369BFULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x75', '\x20', '\x81', '\xDF', '\xD3', '\x33', '\xEA', '\xB4', '\x73', '\x6B', '\x81', '\xE9', '\xAE', '\x47', '\x73', '\x38', '\xD1', '\xF9', '\x24',
        '\x4B', '\x67', '\x88', '\x5D', '\x1C', '\xF2', '\xFA', '\x28', '\x41', '\x76', '\xA1', '\x43', '\x10', '\xC6', '\xFB', '\x1E', '\x16', '\x4C', '\x92',
        '\x59', '\x1C', '\xC7', '\xA1', '\x23', '\x48', '\x63', '\xFD'};
      constexpr EncryptedSource<46> source(bytes);
      thread_local EncryptedName<46, 0x792FFD1325418FB5ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xF3', '\xCC', '\x95', '\x0B', '\xE9', '\xFB', '\x16', '\x84', '\xF5', '\x87', '\x95', '\x3D', '\x94', '\x8F', '\x8F', '\x08', '\x57', '\x15', '\x30',
        '\x9F', '\x5D', '\x40', '\xA1', '\x2C', '\x74', '\x16', '\x3C', '\x95', '\x4C', '\x69', '\xBF', '\x20', '\x40', '\x17', '\x0A', '\xC2', '\x76', '\x5B',
        '\xBC', '\x3B', '\x5E', '\x02', '\x39', '\xDF', '\x4B', '\x58', '\xA3', '\x49'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x49D33529F1556333ULL> name(source);
      return name.Text();
    }()));
  }
  {
    CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList = this->monsterList;
    monsterList->itemX = 0;
    monsterList->itemY = 0;
  }
  this->monsterList->scrollBar->SetSize(8, 355);
  this->monsterList->scrollBar->Move(619, 0);
  this->monsterList->scrollBar->step = 20;
  {
    this->monsterList->scrollBar->SetButtonImages(std::string([]() {
      constexpr char bytes[] = {
        '\xF1', '\x56', '\x13', '\x03', '\xDD', '\x81', '\x0E', '\xCC', '\xF7', '\x1D', '\x13', '\x35', '\xA0', '\xF5', '\x97', '\x40', '\x55', '\x8F', '\xB6',
        '\x97', '\x69', '\x3A', '\xB9', '\x64', '\x76', '\x8C', '\xBA', '\x9D', '\x78', '\x13', '\xB8', '\x62', '\x43', '\x96', '\xBF', '\x95', '\x42', '\x2E',
        '\xB9', '\x73', '\x5E', '\x8E', '\x8C', '\x8C', '\x6D', '\x61', '\xA9', '\x6C', '\x41', '\xF9'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x01CB4F1DF9D3F931ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x57', '\x66', '\x09', '\x61', '\x0D', '\x75', '\x74', '\x08', '\x51', '\x2D', '\x09', '\x57', '\x70', '\x01', '\xED', '\x84', '\xF3', '\xBF', '\xAC',
        '\xF5', '\xB9', '\xCE', '\xC3', '\xA0', '\xD0', '\xBC', '\xA0', '\xFF', '\xA8', '\xE7', '\xC2', '\xA6', '\xE5', '\xA6', '\xA5', '\xF7', '\x92', '\xDA',
        '\xC3', '\xB7', '\xF8', '\xBE', '\x96', '\xFF', '\xA2', '\xCC', '\xDF', '\xEB', '\xF5', '\xA4', '\xB9', '\x9B'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0xC5B1BBCD9BC9C997ULL> name(source);
      return name.Text();
    }()));
  }
  {
    this->monsterList->scrollBar->SetBackground(std::string([]() {
      constexpr char bytes[] = {
        '\x21', '\x2C', '\x11', '\x17', '\xD1', '\x4F', '\x7E', '\x0C', '\x27', '\x67', '\x11', '\x21', '\xAC', '\x3B', '\xE7', '\x80', '\x85', '\xF5', '\xB4',
        '\x83', '\x65', '\xF4', '\xC9', '\xA4', '\xA6', '\xF6', '\xB8', '\x89', '\x74', '\xDD', '\xC8', '\xA2', '\x93', '\xEC', '\xBD', '\x81', '\x4E', '\xE3',
        '\xDC', '\x9E', '\x8C', '\xEA', '\xB5', '\xC3', '\x73', '\xEC', '\xCB', '\xC1'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0xC1BB8111EDD183E1ULL> name(source);
      return name.Text();
    }()));
  }
  {
    this->monsterList->scrollBar->SetThumbImages(std::string([]() {
      constexpr char bytes[] = {
        '\x4B', '\x32', '\xB7', '\x77', '\x21', '\x13', '\xA2', '\xDA', '\x4D', '\x79', '\xB7', '\x41', '\x5C', '\x67', '\x3B', '\x56', '\xEF', '\xEB', '\x12',
        '\xE3', '\x95', '\xA8', '\x15', '\x72', '\xCC', '\xE8', '\x1E', '\xE9', '\x84', '\x81', '\x14', '\x74', '\xF9', '\xF2', '\x1B', '\xE1', '\xBE', '\xA9',
        '\x0F', '\x62', '\xE6', '\xFF', '\x28', '\xF8', '\x91', '\xF3', '\x05', '\x7A', '\xFB', '\x9D'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x1767DDE18D779D8BULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xEB', '\x78', '\x89', '\xD5', '\xCF', '\x1F', '\xE6', '\xC6', '\xED', '\x33', '\x89', '\xE3', '\xB2', '\x6B', '\x7F', '\x4A', '\x4F', '\xA1', '\x2C',
        '\x41', '\x7B', '\xA4', '\x51', '\x6E', '\x6C', '\xA2', '\x20', '\x4B', '\x6A', '\x8D', '\x50', '\x68', '\x59', '\xB8', '\x25', '\x43', '\x50', '\xA5',
        '\x4B', '\x7E', '\x46', '\xB5', '\x16', '\x42', '\x66', '\xB5', '\x0D', '\x69', '\x46', '\xA7', '\x49'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0x0B23D10F2F49D72BULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x0F', '\x74', '\x11', '\xEF', '\x59', '\xE1', '\xAA', '\xDE', '\x09', '\x3F', '\x11', '\xD9', '\x24', '\x95', '\x33', '\x52', '\xAB', '\xAD', '\xB4',
        '\x7B', '\xED', '\x5A', '\x1D', '\x76', '\x88', '\xAE', '\xB8', '\x71', '\xFC', '\x73', '\x1C', '\x70', '\xBD', '\xB4', '\xBD', '\x79', '\xC6', '\x5B',
        '\x07', '\x66', '\xA2', '\xB9', '\x8E', '\x71', '\xF6', '\x58', '\x01', '\x3D', '\xAD', '\xB6', '\xA1', '\x15'};
      constexpr EncryptedSource<52> source(bytes);
      thread_local EncryptedName<52, 0x136F2F9915D1DBCFULL> name(source);
      return name.Text();
    }()));
  }
  this->monsterList->AddNewItem();
  this->AddChild(this->monsterList);

  this->navigationButton = new CUIImageButton();
  this->navigationButton->SetSize(62, 20);
  this->navigationButton->Move(540, 363);
  this->navigationButton->SetOnClick([this]() { this->SearchNavigation(); });
  {
    CUIImageButton* button = this->navigationButton;
    button->disabled = 0;
    button->Invalidate();
  }
  {
    this->navigationButton->SetImages(std::string([]() {
      constexpr char bytes[] = {
        '\x91', '\x02', '\x3D', '\xD1', '\x8F', '\xFD', '\x1A', '\xA6', '\x97', '\x49', '\x3D', '\xE7', '\xF2', '\x89', '\x83', '\x2A', '\x35', '\xDB', '\x98',
        '\x45', '\x3B', '\x46', '\xAD', '\x0E', '\x16', '\xD8', '\x94', '\x4F', '\x2A', '\x6F', '\xBD', '\x1F', '\x3F', '\xF2', '\x93', '\x4A', '\x39', '\x5A',
        '\x80', '\x05', '\x3E', '\xDF', '\x90', '\x4A', '\x23', '\x1D', '\xBD', '\x06', '\x21', '\xAD'};
      constexpr EncryptedSource<50> source(bytes);
      thread_local EncryptedName<50, 0x6BDF334F2BFDAD51ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x81', '\x92', '\x97', '\x8F', '\xF9', '\x51', '\x88', '\xAA', '\x87', '\xD9', '\x97', '\xB9', '\x84', '\x25', '\x11', '\x26', '\x25', '\x4B', '\x32',
        '\x1B', '\x4D', '\xEA', '\x3F', '\x02', '\x06', '\x48', '\x3E', '\x11', '\x5C', '\xC3', '\x2F', '\x13', '\x2F', '\x62', '\x39', '\x14', '\x4F', '\xF6',
        '\x12', '\x17', '\x33', '\x58', '\x24', '\x06', '\x17', '\xFD', '\x20', '\x17', '\x41'};
      constexpr EncryptedSource<49> source(bytes);
      thread_local EncryptedName<49, 0x674D9F3975573D41ULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\x1D', '\xFA', '\xAF', '\x55', '\x29', '\xDB', '\x2C', '\xB4', '\x1B', '\xB1', '\xAF', '\x63', '\x54', '\xAF', '\xB5', '\x38', '\xB9', '\x23', '\x0A',
        '\xC1', '\x9D', '\x60', '\x9B', '\x1C', '\x9A', '\x20', '\x06', '\xCB', '\x8C', '\x49', '\x8B', '\x0D', '\xB3', '\x0A', '\x01', '\xCE', '\x9F', '\x7C',
        '\xB6', '\x16', '\xAB', '\x30', '\x1D', '\x81', '\x8B', '\x78', '\x99', '\x79'};
      constexpr EncryptedSource<48> source(bytes);
      thread_local EncryptedName<48, 0x79E915E9AF6F55DDULL> name(source);
      return name.Text();
    }()), std::string([]() {
      constexpr char bytes[] = {
        '\xC1', '\xDA', '\x3D', '\x23', '\xEF', '\x73', '\x6A', '\x7A', '\xC7', '\x91', '\x3D', '\x15', '\x92', '\x07', '\xF3', '\xF6', '\x65', '\x03', '\x98',
        '\xB7', '\x5B', '\xC8', '\xDD', '\xD2', '\x46', '\x00', '\x94', '\xBD', '\x4A', '\xE1', '\xCD', '\xC3', '\x6F', '\x2A', '\x93', '\xB8', '\x59', '\xD4',
        '\xF0', '\xD3', '\x68', '\x06', '\x9F', '\xB8', '\x43', '\xD8', '\x81', '\xD5', '\x6C', '\x05', '\xFD'};
      constexpr EncryptedSource<51> source(bytes);
      thread_local EncryptedName<51, 0xB7AFBD2FD9FD7501ULL> name(source);
      return name.Text();
    }()));
  }
  this->AddChild(this->navigationButton);
}

// OFFSET: 0x009DBAC0
// FUNCTION: fn_45477
void CUIMonster::SearchNavigation() {
  g_navigationManager.searchText = this->navigationTarget;
  g_navigationManager.UpdateSearchResults();
  g_navigationManager.selectedPath = 0;
  g_navigationManager.selectedPathNode = 0;
  g_navigationManager.SelectNodeState();
  if (g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND)) {
    g_navigationManager.SetViewMode(1);
    g_navigationManager.ClearSavedTarget();
    g_windowMgr.CloseWindow(WID_NAVIGATIONRUIDEWND);
    static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->RefreshLayout();
    g_windowMgr.Promote(g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND));
  } else {
    g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  }
}

}
