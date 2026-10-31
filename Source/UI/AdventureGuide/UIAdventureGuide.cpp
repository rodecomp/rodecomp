#include "Client/Session.h"
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
#include "Contents/Items/ItemDescription.h"
#include <cstring>
#include <format>
#include <list>
#include <regex>
#include <utility>

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

template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();
template <> __declspec(noinline) void CUIListBox<AdventureGuide::CUISubTab>::SelectItem(CUIListItem* item);

// OFFSET: 0x009C28B0
// OFFSET: 0x009C2980
// FUNCTION: fn_13993
// FUNCTION: fn_13994
template <class T> void CUIListBox<T>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
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

template void CUIListBox<AdventureGuide::CUISubTab>::Slot15(int, int);
template void CUIListBox<AdventureGuide::CUITab>::Slot15(int, int);

// OFFSET: 0x009C2A50
// OFFSET: 0x009C2B20
// OFFSET: 0x009C2BF0
// OFFSET: 0x009C2CC0
// FUNCTION: fn_13995
// FUNCTION: fn_13996
// FUNCTION: fn_13997
// FUNCTION: fn_13998
template <class T, class D> void CUIListBoxEx<T, D>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::Slot15(int, int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::Slot15(int, int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::Slot15(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::Slot15(int, int);

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
void CUIAdventureGuide::Slot15(int width, int height) {
  this->background = new CUIStaticImage(*this->m_context);
  this->background->SetSize(750, 468);
  this->background->Move(0, 0);
  {
    CUIStaticImage* background = this->background;
    const std::uint8_t backgroundImageSource[] = {
        0x49, 0x74, 0xA3, 0x83, 0x2B, 0x55, 0x0A, 0x7E, 0x4F, 0x3F, 0xA3, 0xB5, 0x56, 0x21, 0x93, 0xF2, 0xED, 0xAD, 0x06, 0x17, 0x9F, 0xEE, 0xBD, 0xD6,
        0xCE, 0xAE, 0x0A, 0x1D, 0x8E, 0xC7, 0xAD, 0xD4, 0xD6, 0xBC, 0x16, 0x10, 0x8F, 0xFE, 0xE1, 0xD1, 0xE4, 0xAB, 0x63};
    thread_local EncryptedName<sizeof(backgroundImageSource), 0xB3CF9BEB7963DB89ULL> backgroundImageName(backgroundImageSource);
    const std::string backgroundImage(backgroundImageName.Text());
    background->SetImage(backgroundImage);
  }
  this->AddChild(this->background);

  this->titleBar = new CUITitleBar(*this->m_context);
  this->titleBar->SetSize(750, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    const std::uint8_t titleRightSource[] = {
        0x5F, 0x56, 0x81, 0x37, 0x39, 0xA5, 0x62, 0x7A, 0x59, 0x1D, 0x81, 0x01, 0x44, 0xD1, 0xFB, 0xD5, 0xFE, 0x8A, 0x28, 0xAE, 0xA6, 0x02, 0xC9, 0xC3,
        0xFA, 0x8B, 0x27, 0xAC, 0x9A, 0x0E, 0xFB, 0xC3, 0xF6, 0x8D, 0x2D, 0xA8, 0x9B, 0x0A, 0xD5, 0xE8, 0xED, 0x90, 0x26, 0xA5, 0x8D, 0x45, 0xC5, 0xDA,
        0xEF, 0xF9};
    thread_local EncryptedName<sizeof(titleRightSource), 0xB7A76BF9CD41F99FULL> titleRightName(titleRightSource);
    const std::string titleRight(titleRightName.Text());
    const std::uint8_t titleMiddleSource[] = {
        0x7F, 0x2A, 0xE7, 0x87, 0x93, 0xED, 0x3E, 0x2E, 0x79, 0x61, 0xE7, 0xB1, 0xEE, 0x99, 0xA7, 0x81, 0xDE, 0xF6, 0x4E, 0x1E, 0x0C, 0x4A, 0x95, 0x97,
        0xDA, 0xF7, 0x41, 0x1C, 0x30, 0x46, 0xA7, 0x97, 0xD6, 0xF1, 0x4B, 0x18, 0x31, 0x42, 0x89, 0xBC, 0xD2, 0xEC, 0x43, 0x53, 0x31, 0x4E, 0x8B, 0xE3};
    thread_local EncryptedName<sizeof(titleMiddleSource), 0xE3FB23537D2785BFULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const std::uint8_t titleLeftSource[] = {
        0x23, 0xF8, 0xD5, 0xA7, 0xD7, 0x4F, 0x1E, 0xD2, 0x25, 0xB3, 0xD5, 0x91, 0xAA, 0x3B, 0x87, 0x7D, 0x82, 0x24, 0x7C, 0x3E, 0x48, 0xE8, 0xB5, 0x6B,
        0x86, 0x25, 0x73, 0x3C, 0x74, 0xE4, 0x87, 0x6B, 0x8A, 0x23, 0x79, 0x38, 0x75, 0xE0, 0xA9, 0x40, 0x8F, 0x32, 0x73, 0x29, 0x39, 0xE3, 0xB6, 0x6F,
        0xE3};
    thread_local EncryptedName<sizeof(titleLeftSource), 0x1FDB81175D1557E3ULL> titleLeftName(titleLeftSource);
    const std::string titleLeft(titleLeftName.Text());
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText("", 0);
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const std::uint8_t titleFontSource[] = {
        0x90, 0x32, 0x91, 0x0A, 0x60, 0x39};
    thread_local EncryptedName<sizeof(titleFontSource), 0x732B390D63FD47D7ULL> titleFontName(titleFontSource);
    const std::string titleFont(titleFontName.Text());
    title->SetFont(titleFont);
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton(*this->m_context);
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    const std::uint8_t menuDisabledSource[] = {
        0x49, 0xC4, 0x29, 0x65, 0x8B, 0x91, 0x5C, 0x08, 0x4F, 0x8F, 0x29, 0x53, 0xF6, 0xE5, 0xC5, 0xA7, 0xE8, 0x18, 0x80, 0xFC, 0x14, 0x36, 0xF7, 0xB1,
        0xEC, 0x19, 0x8F, 0xFE, 0x28, 0x3A, 0xC5, 0xB6, 0xF0, 0x18, 0xB6, 0xFD, 0x2A, 0x2C, 0xFC, 0x9A, 0xE6, 0x0D, 0x8F, 0xB1, 0x29, 0x32, 0xE9, 0xC5};
    thread_local EncryptedName<sizeof(menuDisabledSource), 0xC5995F4B9FE96B89ULL> menuDisabledName(menuDisabledSource);
    const std::string menuDisabled(menuDisabledName.Text());
    const std::uint8_t menuPressedSource[] = {
        0xDB, 0x66, 0x01, 0x15, 0x11, 0xB3, 0xCA, 0xAC, 0xDD, 0x2D, 0x01, 0x23, 0x6C, 0xC7, 0x53, 0x03, 0x7A, 0xBA, 0xA8, 0x8C, 0x8E, 0x14, 0x61, 0x15,
        0x7E, 0xBB, 0xA7, 0x8E, 0xB2, 0x18, 0x53, 0x12, 0x62, 0xBA, 0x9E, 0x8D, 0xB0, 0x0E, 0x6A, 0x3E, 0x74, 0xA7, 0xEF, 0x8D, 0xBC, 0x0D, 0x0F};
    thread_local EncryptedName<sizeof(menuPressedSource), 0x610F7DD1EFC1C91BULL> menuPressedName(menuPressedSource);
    const std::string menuPressed(menuPressedName.Text());
    const std::uint8_t menuHoverSource[] = {
        0xF7, 0xDA, 0x6D, 0x5D, 0x03, 0x6F, 0x56, 0x92, 0xF1, 0x91, 0x6D, 0x6B, 0x7E, 0x1B, 0xCF, 0x3D, 0x56, 0x06, 0xC4, 0xC4, 0x9C, 0xC8, 0xFD, 0x2B,
        0x52, 0x07, 0xCB, 0xC6, 0xA0, 0xC4, 0xCF, 0x2C, 0x4E, 0x06, 0xF2, 0xC5, 0xA2, 0xD2, 0xF6, 0x00, 0x58, 0x1B, 0x83, 0xC5, 0xAE, 0xD1, 0x93};
    thread_local EncryptedName<sizeof(menuHoverSource), 0x5F93A1C3A7AD7537ULL> menuHoverName(menuHoverSource);
    const std::string menuHover(menuHoverName.Text());
    const std::uint8_t menuNormalSource[] = {
        0x47, 0x08, 0x85, 0x89, 0xD5, 0x47, 0xB6, 0xC4, 0x41, 0x43, 0x85, 0xBF, 0xA8, 0x33, 0x2F, 0x6B, 0xE6, 0xD4, 0x2C, 0x10, 0x4A, 0xE0, 0x1D, 0x7D,
        0xE2, 0xD5, 0x23, 0x12, 0x76, 0xEC, 0x2F, 0x7A, 0xFE, 0xD4, 0x1A, 0x11, 0x74, 0xFA, 0x16, 0x56, 0xE8, 0xC1, 0x23, 0x5D, 0x77, 0xE4, 0x03, 0x09};
    thread_local EncryptedName<sizeof(menuNormalSource), 0x097389157345A787ULL> menuNormalName(menuNormalSource);
    const std::string menuNormal(menuNormalName.Text());
    menuButton->SetImages(menuNormal, menuHover, menuPressed, menuDisabled);
  }
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton(*this->m_context);
  closeButton->SetSize(11, 11);
  closeButton->Move(736, 3);
  closeButton->SetOnClick([this]() { this->OnClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const std::uint8_t closeDisabledSource[] = {
        0x53, 0x34, 0xE5, 0x23, 0xE5, 0xE3, 0x68, 0x90, 0x55, 0x7F, 0xE5, 0x15, 0x98, 0x97, 0xF1, 0x3F, 0xF2, 0xE8, 0x4C, 0xBA, 0x7A, 0x44, 0xC3, 0x29,
        0xF6, 0xE9, 0x43, 0xB8, 0x46, 0x48, 0xF1, 0x2E, 0xEA, 0xE8, 0x7A, 0xBA, 0x49, 0x42, 0xDE, 0x38, 0xCC, 0xF4, 0x43, 0xBF, 0x0B, 0x4F, 0xC0, 0x2D,
        0x93};
    thread_local EncryptedName<sizeof(closeDisabledSource), 0x5DAD2D25D9259B93ULL> closeDisabledName(closeDisabledSource);
    const std::string closeDisabled(closeDisabledName.Text());
    const std::uint8_t closePressedSource[] = {
        0x3D, 0x6E, 0xB3, 0xB9, 0xE7, 0x9F, 0xD8, 0xD2, 0x3B, 0x25, 0xB3, 0x8F, 0x9A, 0xEB, 0x41, 0x7D, 0x9C, 0xB2, 0x1A, 0x20, 0x78, 0x38, 0x73, 0x6B,
        0x98, 0xB3, 0x15, 0x22, 0x44, 0x34, 0x41, 0x6C, 0x84, 0xB2, 0x2C, 0x20, 0x4B, 0x3E, 0x6E, 0x7A, 0xA2, 0xAE, 0x1D, 0x6D, 0x45, 0x3C, 0x6D, 0x1F};
    thread_local EncryptedName<sizeof(closePressedSource), 0x1F1D51274373C1FDULL> closePressedName(closePressedSource);
    const std::string closePressed(closePressedName.Text());
    const std::uint8_t closeHoverSource[] = {
        0xE7, 0x3C, 0xBF, 0xF5, 0x9B, 0xD1, 0xF2, 0xF0, 0xE1, 0x77, 0xBF, 0xC3, 0xE6, 0xA5, 0x6B, 0x5F, 0x46, 0xE0, 0x16, 0x6C, 0x04, 0x76, 0x59, 0x49,
        0x42, 0xE1, 0x19, 0x6E, 0x38, 0x7A, 0x6B, 0x4E, 0x5E, 0xE0, 0x20, 0x6C, 0x37, 0x70, 0x44, 0x58, 0x78, 0xFC, 0x11, 0x21, 0x39, 0x72, 0x47, 0x3D};
    thread_local EncryptedName<sizeof(closeHoverSource), 0x3D371F5B0F7F9327ULL> closeHoverName(closeHoverSource);
    const std::string closeHover(closeHoverName.Text());
    const std::uint8_t closeNormalSource[] = {
        0xE9, 0x60, 0xE9, 0xFD, 0x23, 0xE7, 0x32, 0xA6, 0xEF, 0x2B, 0xE9, 0xCB, 0x5E, 0x93, 0xAB, 0x09, 0x48, 0xBC, 0x40, 0x64, 0xBC, 0x40, 0x99, 0x1F,
        0x4C, 0xBD, 0x4F, 0x66, 0x80, 0x4C, 0xAB, 0x18, 0x50, 0xBC, 0x76, 0x64, 0x8F, 0x46, 0x84, 0x0E, 0x76, 0xA0, 0x4F, 0x61, 0xCD, 0x4B, 0x9A, 0x1B,
        0x29};
    thread_local EncryptedName<sizeof(closeNormalSource), 0x6BF729E30729CF29ULL> closeNormalName(closeNormalSource);
    const std::string closeNormal(closeNormalName.Text());
    closeButton->SetImages(closeNormal, closeHover, closePressed, closeDisabled);
  }
  this->titleBar->SetSecondButton(closeButton);

  this->characterInfo = new AdventureGuide::CUICharacterInfo(*this->m_context);
  this->characterInfo->SetSize(161, 41);
  this->characterInfo->Move(1, 19);
  this->AddChild(this->characterInfo);

  this->titleText = new CUIStaticText(*this->m_context);
  this->titleText->SetSize(104, 20);
  this->titleText->Move(362, 28);
  this->titleText->SetText("", 0);
  this->titleText->SetFitText(0);
  this->titleText->SetAlign(6, 6);
  {
    CUIStaticText* titleText = this->titleText;
    const std::uint8_t stageFontSource[] = {
        0xF0, 0x14, 0x91, 0xD6, 0x5A, 0x2D};
    thread_local EncryptedName<sizeof(stageFontSource), 0x9D8F2D37BFFD61B7ULL> stageFontName(stageFontSource);
    const std::string stageFont(stageFontName.Text());
    titleText->SetFont(stageFont);
  }
  this->titleText->SetFontHeight(14);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x89261B);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->intro = new AdventureGuide::CUIIntro(*this->m_context);
  this->intro->SetSize(637, 373);
  this->intro->Move(112, 63);
  this->AddChild(this->intro);

  this->quest = new AdventureGuide::CUIQuest(*this->m_context);
  this->quest->SetSize(627, 387);
  this->quest->Move(122, 81);
  this->AddChild(this->quest);

  this->monster = new AdventureGuide::CUIMonster(*this->m_context);
  this->monster->SetSize(627, 387);
  this->monster->Move(122, 81);
  this->AddChild(this->monster);

  this->item = new AdventureGuide::CUIItem(*this->m_context);
  this->item->SetSize(627, 387);
  this->item->Move(122, 81);
  this->AddChild(this->item);

  this->tabs = new CUIListBox<AdventureGuide::CUITab>(*this->m_context);
  this->tabs->SetSize(106, 372);
  this->tabs->Move(1, 63);
  this->tabs->SetOnSelect([this](AdventureGuide::CUITab* tab) { this->OnSelectTab(tab); });
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    const std::uint8_t tabDisabledSource[] = {
        0xB9, 0x02, 0x47, 0x89, 0xB3, 0x21, 0x26, 0x56, 0xBF, 0x49, 0x47, 0xBF, 0xCE, 0x55, 0xBF, 0xDA, 0x1D, 0xDB, 0xE2, 0x1D, 0x07, 0x9A, 0x91, 0xFE,
        0x3E, 0xD8, 0xEE, 0x17, 0x16, 0xB3, 0x97, 0xFA, 0x1B, 0xF2, 0xE5, 0x07, 0x1D, 0xB0, 0x8D, 0xF4, 0x0B, 0xC0, 0xE6, 0x1F, 0x5D, 0x8D, 0x8E, 0xEB,
        0x79};
    thread_local EncryptedName<sizeof(tabDisabledSource), 0x9BE3EF737387AD79ULL> tabDisabledName(tabDisabledSource);
    const std::string tabDisabled(tabDisabledName.Text());
    const std::uint8_t tabPressedSource[] = {
        0xF7, 0x52, 0xB5, 0xAF, 0xF5, 0x0F, 0x90, 0x8A, 0xF1, 0x19, 0xB5, 0x99, 0x88, 0x7B, 0x09, 0x06, 0x53, 0x8B, 0x10, 0x3B, 0x41, 0xB4, 0x27, 0x22,
        0x70, 0x88, 0x1C, 0x31, 0x50, 0x9D, 0x21, 0x26, 0x55, 0xA2, 0x17, 0x21, 0x5B, 0x9E, 0x3A, 0x31, 0x52, 0x8F, 0x5B, 0x37, 0x58, 0xB1, 0x55};
    thread_local EncryptedName<sizeof(tabPressedSource), 0x4755C1355575FD37ULL> tabPressedName(tabPressedSource);
    const std::string tabPressed(tabPressedName.Text());
    const std::uint8_t tabHoverSource[] = {
        0xC7, 0x72, 0xB7, 0x99, 0xA3, 0xFB, 0x0E, 0xDC, 0xC1, 0x39, 0xB7, 0xAF, 0xDE, 0x8F, 0x97, 0x50, 0x63, 0xAB, 0x12, 0x0D, 0x17, 0x40, 0xB9, 0x74,
        0x40, 0xA8, 0x1E, 0x07, 0x06, 0x69, 0xBF, 0x70, 0x65, 0x82, 0x15, 0x17, 0x0D, 0x6A, 0xBB, 0x63, 0x62, 0xAE, 0x04, 0x4D, 0x01, 0x58, 0xBB, 0x11};
    thread_local EncryptedName<sizeof(tabHoverSource), 0x11CB35636377DD07ULL> tabHoverName(tabHoverSource);
    const std::string tabHover(tabHoverName.Text());
    const std::uint8_t tabNormalSource[] = {
        0xB5, 0xCA, 0xA1, 0x23, 0x8D, 0xC7, 0x1A, 0xB4, 0xB3, 0x81, 0xA1, 0x15, 0xF0, 0xB3, 0x83, 0x38, 0x11, 0x13, 0x04, 0xB7, 0x39, 0x7C, 0xAD, 0x1C,
        0x32, 0x10, 0x08, 0xBD, 0x28, 0x55, 0xAB, 0x18, 0x17, 0x3A, 0x03, 0xAD, 0x23, 0x56, 0xB1, 0x16, 0x07, 0x08, 0x00, 0xB5, 0x63, 0x6B, 0xB2, 0x09,
        0x75};
    thread_local EncryptedName<sizeof(tabNormalSource), 0x79DF094DD9616575ULL> tabNormalName(tabNormalSource);
    const std::string tabNormal(tabNormalName.Text());
    tabs->SetImages(tabNormal, tabHover, tabPressed, tabDisabled);
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
    const std::uint8_t downImageSource[] = {
        0x9F, 0xD6, 0x61, 0x1D, 0x31, 0x03, 0x9A, 0x4A, 0x99, 0x9D, 0x61, 0x2B, 0x4C, 0x77, 0x03, 0xC6, 0x3B, 0x0F, 0xC4, 0x89, 0x85, 0xB8, 0x2D, 0xE2,
        0x18, 0x0C, 0xC8, 0x83, 0x94, 0x91, 0x2C, 0xE4, 0x2D, 0x16, 0xCD, 0x8B, 0xAE, 0xAC, 0x2D, 0xF5, 0x30, 0x0E, 0xFE, 0x83, 0x9E, 0xBA, 0x31, 0xA9,
        0x3D, 0x14, 0xD1, 0xE7};
    thread_local EncryptedName<sizeof(downImageSource), 0x875FCDF1E7A1795FULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0xA9, 0x2A, 0x9F, 0xDF, 0x77, 0xB9, 0xAE, 0x74, 0xAF, 0x61, 0x9F, 0xE9, 0x0A, 0xCD, 0x37, 0xF8, 0x0D, 0xF3, 0x3A, 0x4B, 0xC3, 0x02, 0x19, 0xDC,
        0x2E, 0xF0, 0x36, 0x41, 0xD2, 0x2B, 0x18, 0xDA, 0x1B, 0xEA, 0x33, 0x49, 0xE8, 0x16, 0x19, 0xCB, 0x06, 0xF2, 0x00, 0x50, 0xC7, 0x59, 0x09, 0xD4,
        0x19, 0x85};
    thread_local EncryptedName<sizeof(upImageSource), 0xB96B77B7255F8569ULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    tabs->scrollBar->SetButtonImages(upImage, downImage);
  }
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    const std::uint8_t scrollBackgroundSource[] = {
        0x9B, 0x32, 0xE5, 0xD7, 0x47, 0xE3, 0xE8, 0xCA, 0x9D, 0x79, 0xE5, 0xE1, 0x3A, 0x97, 0x71, 0x46, 0x3F, 0xEB, 0x40, 0x43, 0xF3, 0x58, 0x5F, 0x62,
        0x1C, 0xE8, 0x4C, 0x49, 0xE2, 0x71, 0x5E, 0x64, 0x29, 0xF2, 0x49, 0x41, 0xD8, 0x4F, 0x4A, 0x58, 0x36, 0xF4, 0x41, 0x03, 0xE5, 0x40, 0x5D, 0x07};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0x072D2D872D259D5BULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    tabs->scrollBar->SetBackground(scrollBackground);
  }
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    const std::uint8_t thumbBottomSource[] = {
        0x97, 0x40, 0x11, 0x4F, 0x6B, 0xB1, 0x38, 0x92, 0x91, 0x0B, 0x11, 0x79, 0x16, 0xC5, 0xA1, 0x1E, 0x33, 0x99, 0xB4, 0xDB, 0xDF, 0x0A, 0x8F, 0x3A,
        0x10, 0x9A, 0xB8, 0xD1, 0xCE, 0x23, 0x8E, 0x3C, 0x25, 0x80, 0xBD, 0xD9, 0xF4, 0x0B, 0x95, 0x2A, 0x3A, 0x8D, 0x8E, 0xD1, 0xC4, 0x08, 0x93, 0x71,
        0x35, 0x82, 0xA1, 0xB5};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x5FFD7FABB5D1EF57ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0x11, 0xCE, 0x31, 0x73, 0xAF, 0x9F, 0x20, 0x1E, 0x17, 0x85, 0x31, 0x45, 0xD2, 0xEB, 0xB9, 0x92, 0xB5, 0x17, 0x94, 0xE7, 0x1B, 0x24, 0x97, 0xB6,
        0x96, 0x14, 0x98, 0xED, 0x0A, 0x0D, 0x96, 0xB0, 0xA3, 0x0E, 0x9D, 0xE5, 0x30, 0x25, 0x8D, 0xA6, 0xBC, 0x03, 0xAE, 0xE4, 0x06, 0x35, 0xCB, 0xB1,
        0xBC, 0x11, 0xF1};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xD3E5516F89F161D1ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0x4D, 0xBC, 0xBF, 0xE9, 0xA1, 0x2B, 0xF0, 0x38, 0x4B, 0xF7, 0xBF, 0xDF, 0xDC, 0x5F, 0x69, 0xB4, 0xE9, 0x65, 0x1A, 0x7D, 0x15, 0x90, 0x47, 0x90,
        0xCA, 0x66, 0x16, 0x77, 0x04, 0xB9, 0x46, 0x96, 0xFF, 0x7C, 0x13, 0x7F, 0x3E, 0x91, 0x5D, 0x80, 0xE0, 0x71, 0x20, 0x66, 0x11, 0xCB, 0x57, 0x98,
        0xFD, 0x13};
    thread_local EncryptedName<sizeof(thumbTopSource), 0xF535E561137F138DULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    tabs->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(this->tabs);
}

// OFFSET: 0x009DBF20
// FUNCTION: fn_45467
void CUIAdventureGuide::OnClose() {
  g_windowMgr.CloseWindow(10002);
}

// OFFSET: 0x009C14F0
// FUNCTION: fn_13991
void CUIAdventureGuide::Slot53() {
  this->unknown_00B4 = 0;
  this->Resize(750, 468);
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
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
    this->page->Slot54(this->type, subTab->id);
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
    this->unknown_008C = item->unknown_0108;
  }
  const bool selected = !this->unknown_008C.empty();
  this->unknown_0084->Show(selected);
  this->unknown_0088->Show(selected);
}

// OFFSET: 0x009DC270
// FUNCTION: fn_14319
void CUIMonster::OnSelectMonster(CUIMonsterInfo* item, const CMonsterData&) {
  if (item) {
    this->unknown_0088 = item->unknown_00E8->unknown_0098;
  }
  this->unknown_0084->Show(!this->unknown_0088.empty());
}

// OFFSET: 0x009DC2C0
// FUNCTION: fn_14320
void CUIQuest::OnSelectQuest(CUIQuestInfo* item, const CQuestInfo&) {
  if (item) {
    this->unknown_008C = item->unknown_00F8;
  }
  const bool selected = !this->unknown_008C.empty();
  this->unknown_0084->Show(selected);
  this->unknown_0088->Show(selected);
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
  const std::string name(desc.identifiedName);
  item->nameText->SetText(name.c_str(), name.size());
  item->image->SetImage(image);
  reinterpret_cast<CUIQuestInfo*>(item)->SetStatusLines(data.first, data.second, data.third);
  item->SetDescription(data.type, ItemDescriptions{g_itemDescriptions}.Description(info));
  reinterpret_cast<CUITextViewer*>(item->unknown_00F8)->Clear();
  for (const std::string& value : data.values) {
    reinterpret_cast<CUITextViewer*>(item->unknown_00F8)->AddLine(value);
  }
  item->unknown_0108 = data.text;
}

// OFFSET: 0x009DC770
// FUNCTION: fn_14323
void __stdcall BindQuestInfo(CUIQuestInfo* owner, const CQuestInfo& data) {
  switch (data.type) {
  case 1:
    owner->SetTitle(data.id);
    owner->SetViewerLines(data.rewards);
    owner->unknown_00F8 = data.title;
    reinterpret_cast<CUITextViewer*>(owner->unknown_00F4)->Clear();
    reinterpret_cast<CUITextViewer*>(owner->unknown_00F4)->AddLine(data.text);
    owner->questId = data.questId;
    owner->unknown_00E8->Show(g_renewQuestMgr->IsNewQuest(data.questId));
    break;
  case 3:
    owner->SetTitle(data.id);
    owner->SetViewerLines(data.rewards);
    owner->unknown_00F8 = data.title;
    reinterpret_cast<CUITextViewer*>(owner->unknown_00F4)->Clear();
    reinterpret_cast<CUITextViewer*>(owner->unknown_00F4)->AddLine(data.text);
    owner->unknown_00E8->Show(0);
    return;
  case 4:
    owner->SetTip(data.id);
    owner->SetViewerLines(data.rewards);
    owner->unknown_00F8 = data.title;
    reinterpret_cast<CUITextViewer*>(owner->unknown_00F4)->Clear();
    reinterpret_cast<CUITextViewer*>(owner->unknown_00F4)->AddLine(data.text);
    owner->unknown_00E8->Show(0);
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
void CUIIntro::Slot16() {
  const std::string& title = g_adventureGuideMgr->unknown_0014;
  const std::map<int, AdventureGuideText>& texts = g_adventureGuideMgr->unknown_002C;
  if (texts.empty()) {
    return;
  }

  for (const auto& text : texts) {
    this->contents->data.emplace_back(text.second.first, text.second.second);
  }

  this->contents->UpdateScrollBar();
  this->image->SetImage(title);
  this->contents->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD1C0
// FUNCTION: fn_14333
void CUIItem::Slot16() {
}

// OFFSET: 0x009DD6B0
// FUNCTION: fn_14338
void CUIQuest::Slot16() {
}

// OFFSET: 0x009DD1D0
// FUNCTION: fn_14334
void CUIItemInfo::Slot16() {
  this->unknown_00F0->scrollBar->SetHidden(1);
  this->unknown_00F4->scrollBar->SetHidden(1);
  this->unknown_00F8->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DEF40
// FUNCTION: fn_14358
void CUIItemInfo::SetDescription(int type, const std::vector<const char*>& description) {
  CUITextViewer* viewer = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
  viewer->Clear();

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
    bool highlighted = false;
    for (std::size_t pos = text.find('^'); pos != std::string::npos; pos = text.find('^', pos + 1)) {
      if (pos + 6 < text.size() && text[pos + 1] == '7' && text[pos + 2] == '7' && text[pos + 3] == '7' &&
          text[pos + 4] == '7' && text[pos + 5] == '7' && text[pos + 6] == '7') {
        highlighted = true;
        break;
      }
    }
    if (!highlighted) {
      continue;
    }

    std::regex pattern("\\^000000");
    std::cmatch match;
    while (std::regex_search(text.c_str(), text.c_str() + text.size(), match, pattern)) {
      viewer->AddLine(match.prefix().str() + match[0].str());
      ++lines;
      text = match.suffix().str();
      text.erase(0, text.find_first_not_of(" \r\n\t\v\f"));
    }
  }
}

// OFFSET: 0x009DD210
// FUNCTION: fn_14335
void CUIMonster::Slot16() {
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
void CUIMonsterInfo::Slot16() {
  this->unknown_00EC->scrollBar->SetHidden(1);
  this->unknown_00F0->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD6C0
// FUNCTION: fn_14339
void CUIQuestInfo::Slot16() {
  this->unknown_00EC->scrollBar->SetHidden(1);
  CUIVScrollBar* first = this->unknown_00EC->scrollBar;
  first->dragging = 0;
  first->Invalidate();
  this->unknown_00F0->scrollBar->SetHidden(1);
  CUIVScrollBar* last = this->unknown_00F4->scrollBar;
  last->dragging = 1;
  last->Invalidate();
}

// OFFSET: 0x009DD720
// FUNCTION: fn_14340
void CUISubTab::Slot16() {
  this->unknown_00EC->Show(0);
}

// OFFSET: 0x009DD730
// FUNCTION: fn_14341
void CUITab::Slot16() {
}

// OFFSET: 0x009DD740
// FUNCTION: fn_14342
void CUITabInfo::Slot16() {
}

// OFFSET: 0x009DC0F0
// FUNCTION: fn_14316
void CUIIntro::BindContents(CUITabInfo* item, const Guide_Intro::Contents& contents) {
  item->unknown_00E8->SetText(contents.first.c_str(), contents.first.size());
  item->unknown_00EC->SetText(contents.second.c_str(), contents.second.size());
}

// OFFSET: 0x009E0350
// FUNCTION: fn_14361
void CUIQuestInfo::SetTitle(int id) {
  CUITextViewer* viewer = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
  viewer->Clear();

  const TipBoxEntry& entry = g_itemTagOwner->GetTip(id);
  if (entry.title.empty() || entry.paragraphs.empty()) {
    viewer->AddLine(std::to_string(id));
    return;
  }

  std::string text = entry.title;
  std::cmatch match;
  if (std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("Q(.+)\\(")) && match.size() > 1) {
    text = match[1].str();
  }

  text.erase(0, text.find_first_not_of(" \r\n\t\v\f"));
  text.erase(text.find_last_not_of(" \r\n\t\v\f") + 1);
  viewer->SetBold(1);
  viewer->AddLine(text);

  text = entry.paragraphs[0];
  if (std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("\n"))) {
    text = match.prefix().str();
  }

  text.erase(0, text.find_first_not_of(" \r\n\t\v\f"));
  text.erase(text.find_last_not_of(" \r\n\t\v\f") + 1);
  viewer->SetBold(0);
  viewer->AddLine(text);
}

// OFFSET: 0x009DFAD0
// FUNCTION: fn_14359
void CUIQuestInfo::SetStatusLines(bool first, bool second, bool third) {
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->Clear();
  if (first == true) {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0xC86464u);
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->AddLine(FormatOwnedString(LookupMessage(0xFC2)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0x6464C8u);
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->AddLine(FormatOwnedString(LookupMessage(0xFC3)));
  }
  if (second == true) {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0xC86464u);
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->AddLine(FormatOwnedString(LookupMessage(0xFC4)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0x6464C8u);
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->AddLine(FormatOwnedString(LookupMessage(0xFC5)));
  }
  if (third == true) {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0xC86464u);
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->AddLine(FormatOwnedString(LookupMessage(0xFC6)));
  } else {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0x6464C8u);
    reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->AddLine(FormatOwnedString(LookupMessage(0xFC7)));
  }
}

// OFFSET: 0x009E07B0
// FUNCTION: fn_14362
void CUIQuestInfo::SetViewerLines(const std::vector<std::string>& lines) {
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->Clear();
  for (const std::string& line : lines) {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->AddLine(line);
  }
}

// OFFSET: 0x009DDA40
// FUNCTION: fn_14347
void CUIIntro::Slot54(int, int) {
}

// OFFSET: 0x009DDA50
// FUNCTION: fn_14348
void CUIItem::Slot54(int type, int id) {
  this->itemList->ClearData();
  this->location = id;
  std::list<std::pair<int, AdventureGuideEntry>> entries;
  g_adventureGuideMgr->GetEntryList(entries, type, id);
  for (const auto& [key, entry] : entries) {
    CItemData data;
    data.id = key;
    data.first = entry.unknown_0000;
    data.second = entry.unknown_0001;
    data.third = entry.unknown_0002;
    data.unknown_0008 = entry.unknown_0004;
    data.unknown_000C = entry.unknown_0008;
    data.text = entry.text;
    data.values = entry.values;
    data.type = type;
    this->itemList->data.push_back(data);
  }
  this->itemList->UpdateScrollBar();
  this->unknown_0084->Show(0);
  this->unknown_0088->Show(0);
}

// OFFSET: 0x009DDE50
// FUNCTION: fn_14350
void CUIMonster::Slot54(int, int) {
  this->unknown_0084->Show(0);
}

// OFFSET: 0x009DDE60
// FUNCTION: fn_14351
void CUIQuest::Slot54(int type, int id) {
  this->questList->ClearData();
  std::list<std::pair<int, AdventureGuideContents>> contents;
  g_adventureGuideMgr->GetContentsList(contents, type, id);
  for (const auto& [key, content] : contents) {
    CQuestInfo data;
    data.id = key;
    data.unknown_0004 = content.unknown_0000;
    data.unknown_0008 = content.unknown_0004;
    data.unknown_000C = content.unknown_0008;
    data.questId = content.unknown_000C;
    data.title = content.first;
    data.text = content.second;
    data.rewards = content.values;
    data.type = type;
    this->questList->data.push_back(data);
    if (type == 1 && !g_renewQuestMgr->IsNewQuest(content.unknown_000C)) {
      g_renewQuestMgr->AddStatusRequest(content.unknown_000C);
    }
  }
  this->questList->UpdateScrollBar();
  if (type == 1) {
    g_renewQuestMgr->SendQuestStatusRequest();
  }
  this->unknown_0084->Show(0);
  this->unknown_0088->Show(0);
}

// OFFSET: 0x009DE410
// FUNCTION: fn_14353
void CUITab::SetSelected(bool selected) {
  CUIListItem::SetSelected(selected);
  const bool expand = selected && this->subTabs && !this->subTabs->geometry.visible;
  if (this->subTabs) {
    this->subTabs->Show(expand);
  }
  this->Resize(this->geometry.width, this->unknown_0110 + (expand ? this->subTabs->geometry.height : 0));
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
    if (entry != item) {
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
    if (entry != item) {
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
void CUIAdventureGuide::Slot16() {
  const char* title = LookupMessage(0xFD5);
  this->titleBar->SetTitle(title, std::strlen(title));

  std::map<int, AdventureGuide::CUIPage*> pages = {
      {0, this->intro},
      {1, this->quest},
      {2, this->monster},
      {3, this->quest},
      {4, this->quest},
      {5, this->item},
      {6, this->item},
  };

  for (const auto& [type, intro] : g_adventureGuideMgr->m_intros) {
    AdventureGuide::CUITab* tab = this->tabs->CreateItem();
    tab->type = type;
    tab->unknown_00E8->SetText(intro.text.c_str(), intro.text.size());
    tab->unknown_00F0 = intro.title;
    tab->unknown_0110 = this->tabs->rowHeight;

    auto page = pages.find(tab->type);
    if (page != pages.end() && page->first == tab->type) {
      tab->page = page->second;
    }

    if (!intro.details.empty()) {
      std::map<int, AdventureGuideDetail> details;
      for (const auto& [id, detail] : intro.details) {
        if (g_adventureGuideMgr->IsAvailable(tab->type, id)) {
          details[id] = detail;
        }
      }
      if (!details.empty()) {
        CUIListBox<AdventureGuide::CUISubTab>* subTabs = tab->CreateSubTabList();
        subTabs->Resize(subTabs->geometry.width, subTabs->rowHeight * static_cast<int>(details.size()));
        for (const auto& [id, detail] : details) {
          AdventureGuide::CUISubTab* subTab = subTabs->CreateItem();
          subTab->unknown_00E8->SetText(detail.text.c_str(), detail.text.size());
          subTab->id = id;
          subTab->unknown_00F4 = detail.unknown_0008;
          if (tab->type == 1) {
            const bool isNew = g_renewQuestMgr->IsNewQuest(detail.unknown_0008);
            subTab->unknown_00EC->Show(isNew);
            if (!isNew) {
              g_renewQuestMgr->AddStatusRequest(detail.unknown_0008);
            }
          }
        }
        tab->Resize(tab->geometry.width, this->tabs->rowHeight + subTabs->geometry.height);
      }
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
  CUITextViewer* viewer = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
  viewer->Clear();

  const TipBoxEntry& tip = g_itemTagOwner->GetTip(id);
  if (tip.title.empty() || tip.paragraphs.empty()) {
    viewer->AddLine(std::to_string(id));
    return;
  }

  std::string title = tip.title;
  std::match_results<const char*> match;
  std::regex_search(title.c_str(), title.c_str() + title.size(), match, std::regex("\\#"));
  if (!match.empty()) {
    title = match.prefix().str();
  }

  const std::string whitespace(" \r\n\t\v\f");
  title.erase(0, title.find_first_not_of(whitespace));
  title.erase(title.find_last_not_of(whitespace) + 1);
  viewer->SetBold(true);
  viewer->AddLine(title);

  title = tip.paragraphs.front();
  std::regex_search(title.c_str(), title.c_str() + title.size(), match, std::regex("\n"));
  if (!match.empty()) {
    title = match.prefix().str();
  }
  title.erase(0, title.find_first_not_of(whitespace));
  title.erase(title.find_last_not_of(whitespace) + 1);
  viewer->SetBold(false);
  viewer->AddLine(title);
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
        return subTab->unknown_00F4 == static_cast<unsigned int>(questId);
      });
      if (found != subTabs.end()) {
        (*found)->unknown_00EC->Show(1);
      }
    }

    AdventureGuide::CUIQuest* quest = dynamic_cast<AdventureGuide::CUIQuest*>(tab->page);
    if (quest) {
      std::vector<AdventureGuide::CUIQuestInfo*> items = quest->questList->items;
      auto found = std::find_if(items.begin(), items.end(), [questId](AdventureGuide::CUIQuestInfo* item) {
        return item->questId == questId;
      });
      if (found != items.end()) {
        (*found)->unknown_00E8->Show(1);
      }
    }
  }
}

// OFFSET: 0x009DC140
// FUNCTION: fn_14317
void CUIAdventureGuide::OnSelectTab(AdventureGuide::CUITab* tab) {
  this->titleText->SetText(tab->unknown_00F0.c_str(), tab->unknown_00F0.size());
  AdventureGuide::CUIPage* page = tab->page;
  if (!page) {
    return;
  }

  const int type = tab->type;
  CUIListBox<AdventureGuide::CUISubTab>* subTabs = tab->subTabs;
  if (!subTabs) {
    page->Slot54(type, 0);
    return;
  }

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
    page->Slot54(type, subTabs->selected->id);
  } else {
    page->Slot54(type, 0);
  }
}

namespace AdventureGuide {

// OFFSET: 0x009C5400
// FUNCTION: fn_14000
void CUICharacterInfo::Slot15(int width, int height) {
  this->jobIcon = new CUIStaticImage(*this->m_context);
  this->jobIcon->SetSize(25, 25);
  this->jobIcon->Move(10, 8);
  this->jobIcon->SetImage(std::string());
  this->AddChild(this->jobIcon);

  this->level = new CUIStaticText(*this->m_context);
  this->level->SetSize(50, 12);
  this->level->Move(46, 6);
  this->level->SetText("", 0);
  this->level->SetFitText(0);
  this->level->SetAlign(0, 0);
  const std::uint8_t levelFontSource[] = {0xFE, 0x70, 0x1B, 0x26, 0x66, 0x1D};
  // GLOBAL: gl_25126
  thread_local EncryptedName<sizeof(levelFontSource), 0x758F1D0B4F7705B9ULL> levelFont(levelFontSource);
  this->level->SetFont(std::string(levelFont.Text()));
  this->level->SetFontHeight(12);
  this->level->SetBold(0);
  this->level->SetColor(0);
  this->level->SetShadow(1, 0xFFFFFF);
  this->level->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->level);

  this->jobLevel = new CUIStaticText(*this->m_context);
  this->jobLevel->SetSize(50, 12);
  this->jobLevel->Move(97, 6);
  this->jobLevel->SetText("", 0);
  this->jobLevel->SetFitText(0);
  this->jobLevel->SetAlign(0, 0);
  const std::uint8_t jobLevelFontSource[] = {0x70, 0x32, 0xC1, 0x50, 0xD2, 0xE5};
  // GLOBAL: gl_25127
  thread_local EncryptedName<sizeof(jobLevelFontSource), 0x17A9E5BF39AD4737ULL> jobLevelFont(jobLevelFontSource);
  this->jobLevel->SetFont(std::string(jobLevelFont.Text()));
  this->jobLevel->SetFontHeight(12);
  this->jobLevel->SetBold(0);
  this->jobLevel->SetColor(0);
  this->jobLevel->SetShadow(1, 0xFFFFFF);
  this->jobLevel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->jobLevel);

  this->name = new CUIStaticText(*this->m_context);
  this->name->SetSize(100, 12);
  this->name->Move(45, 22);
  this->name->SetText("", 0);
  this->name->SetFitText(1);
  this->name->SetAlign(0, 0);
  const std::uint8_t nameFontSource[] = {0x20, 0xDE, 0xFD, 0xCC, 0xCC, 0x3F};
  // GLOBAL: gl_25128
  thread_local EncryptedName<sizeof(nameFontSource), 0x13E33FA1A591AB67ULL> nameFont(nameFontSource);
  this->name->SetFont(std::string(nameFont.Text()));
  this->name->SetFontHeight(12);
  this->name->SetBold(0);
  this->name->SetColor(0);
  this->name->SetShadow(1, 0xFFFFFF);
  this->name->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->name);
}

// OFFSET: 0x009C8D00
// FUNCTION: fn_14003
void CUIItemInfo::Slot15(int width, int height) {
  this->image = new CUIStaticImage(*this->m_context);
  this->image->SetSize(75, 100);
  this->image->Move(45, 14);
  this->image->SetImage(std::string());
  this->AddChild(this->image);

  this->nameText = new CUIStaticText(*this->m_context);
  this->nameText->SetSize(280, 12);
  this->nameText->Move(165, 13);
  this->nameText->SetText("", 0);
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 0);
  {
    CUIStaticText* name = this->nameText;
    const std::uint8_t nameFontSource[] = {
        0xA2, 0x0E, 0x5F, 0x40, 0x20, 0x65};
    thread_local EncryptedName<sizeof(nameFontSource), 0x192D654D29337BE5ULL> nameFontName(nameFontSource);
    const std::string nameFont(nameFontName.Text());
    name->SetFont(nameFont);
  }
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(1);
  this->nameText->SetColor(0xCE6B52);
  this->nameText->SetShadow(0, 0xFFFFFF);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);

  CUITextViewer* description = new CUITextViewer(*this->m_context);
  this->unknown_00F0 = reinterpret_cast<CUIStaticText*>(description);
  description->SetSize(173, 97);
  description->Move(165, 26);
  description->SetTextOffset(0, 6);
  {
    const std::uint8_t descriptionFontSource[] = {
        0xA4, 0xA0, 0x3F, 0x9E, 0xC8, 0x07};
    thread_local EncryptedName<sizeof(descriptionFontSource), 0xB3E307A5F753D5E3ULL> descriptionFontName(descriptionFontSource);
    const std::string descriptionFont(descriptionFontName.Text());
    description->SetFont(descriptionFont);
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
    const std::uint8_t downImageSource[] = {
        0x3F, 0x4C, 0x9D, 0xF9, 0x67, 0x61, 0x66, 0xF2, 0x39, 0x07, 0x9D, 0xCF, 0x1A, 0x15, 0xFF, 0x7E, 0x9B, 0x95, 0x38, 0x6D, 0xD3, 0xDA, 0xD1, 0x5A,
        0xB8, 0x96, 0x34, 0x67, 0xC2, 0xF3, 0xD0, 0x5C, 0x8D, 0x8C, 0x31, 0x6F, 0xF8, 0xCE, 0xD1, 0x4D, 0x90, 0x94, 0x02, 0x67, 0xC8, 0xD8, 0xCD, 0x11,
        0x9D, 0x8E, 0x2D, 0x03};
    thread_local EncryptedName<sizeof(downImageSource), 0x3FA3AFA7035DE3FFULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0x99, 0x28, 0xC3, 0x87, 0x95, 0x05, 0xE2, 0x16, 0x9F, 0x63, 0xC3, 0xB1, 0xE8, 0x71, 0x7B, 0x9A, 0x3D, 0xF1, 0x66, 0x13, 0x21, 0xBE, 0x55, 0xBE,
        0x1E, 0xF2, 0x6A, 0x19, 0x30, 0x97, 0x54, 0xB8, 0x2B, 0xE8, 0x6F, 0x11, 0x0A, 0xAA, 0x55, 0xA9, 0x36, 0xF0, 0x5C, 0x08, 0x25, 0xE5, 0x45, 0xB6,
        0x29, 0x87};
    thread_local EncryptedName<sizeof(upImageSource), 0xDB27CB557D038759ULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    description->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    const std::uint8_t scrollBackgroundSource[] = {
        0x2F, 0x82, 0x97, 0xF7, 0x05, 0x1F, 0x26, 0x4A, 0x29, 0xC9, 0x97, 0xC1, 0x78, 0x6B, 0xBF, 0xC6, 0x8B, 0x5B, 0x32, 0x63, 0xB1, 0xA4, 0x91, 0xE2,
        0xA8, 0x58, 0x3E, 0x69, 0xA0, 0x8D, 0x90, 0xE4, 0x9D, 0x42, 0x3B, 0x61, 0x9A, 0xB3, 0x84, 0xD8, 0x82, 0x44, 0x33, 0x23, 0xA7, 0xBC, 0x93, 0x87};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0x87E3D1C50D572DEFULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    description->SetScrollBarBackground(scrollBackground);
  }
  {
    const std::uint8_t thumbBottomSource[] = {
        0x91, 0x5A, 0x0F, 0xB3, 0x3F, 0xD9, 0x44, 0x12, 0x97, 0x11, 0x0F, 0x85, 0x42, 0xAD, 0xDD, 0x9E, 0x35, 0x83, 0xAA, 0x27, 0x8B, 0x62, 0xF3, 0xBA,
        0x16, 0x80, 0xA6, 0x2D, 0x9A, 0x4B, 0xF2, 0xBC, 0x23, 0x9A, 0xA3, 0x25, 0xA0, 0x63, 0xE9, 0xAA, 0x3C, 0x97, 0x90, 0x2D, 0x90, 0x60, 0xEF, 0xF1,
        0x33, 0x98, 0xBF, 0x49};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xDF8117FF49CFF551ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0xFD, 0xEE, 0x4B, 0x33, 0x39, 0xD5, 0x00, 0x60, 0xFB, 0xA5, 0x4B, 0x05, 0x44, 0xA1, 0x99, 0xEC, 0x59, 0x37, 0xEE, 0xA7, 0x8D, 0x6E, 0xB7, 0xC8,
        0x7A, 0x34, 0xE2, 0xAD, 0x9C, 0x47, 0xB6, 0xCE, 0x4F, 0x2E, 0xE7, 0xA5, 0xA6, 0x6F, 0xAD, 0xD8, 0x50, 0x23, 0xD4, 0xA4, 0x90, 0x7F, 0xEB, 0xCF,
        0x50, 0x31, 0x8B};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xADC51BF9C98B413DULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0x4D, 0x12, 0x7B, 0x99, 0x77, 0xF7, 0x7E, 0x9C, 0x4B, 0x59, 0x7B, 0xAF, 0x0A, 0x83, 0xE7, 0x10, 0xE9, 0xCB, 0xDE, 0x0D, 0xC3, 0x4C, 0xC9, 0x34,
        0xCA, 0xC8, 0xD2, 0x07, 0xD2, 0x65, 0xC8, 0x32, 0xFF, 0xD2, 0xD7, 0x0F, 0xE8, 0x4D, 0xD3, 0x24, 0xE0, 0xDF, 0xE4, 0x16, 0xC7, 0x17, 0xD9, 0x3C,
        0xFD, 0xBD};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x51BB39B763BBBD8DULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    description->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(description);

  CUITextViewer* options = new CUITextViewer(*this->m_context);
  this->unknown_00F4 = reinterpret_cast<CUIStaticText*>(options);
  options->SetSize(115, 90);
  options->Move(338, 26);
  options->SetTextOffset(0, 8);
  {
    const std::uint8_t optionFontSource[] = {
        0x0E, 0x18, 0x13, 0x8A, 0xAA, 0x01};
    thread_local EncryptedName<sizeof(optionFontSource), 0x114901C7E37F6D49ULL> optionFontName(optionFontSource);
    const std::string optionFont(optionFontName.Text());
    options->SetFont(optionFont);
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
    const std::uint8_t downImageSource[] = {
        0x9B, 0x18, 0xA1, 0xA3, 0x8F, 0x9F, 0xD0, 0x3A, 0x9D, 0x53, 0xA1, 0x95, 0xF2, 0xEB, 0x49, 0xB6, 0x3F, 0xC1, 0x04, 0x37, 0x3B, 0x24, 0x67, 0x92,
        0x1C, 0xC2, 0x08, 0x3D, 0x2A, 0x0D, 0x66, 0x94, 0x29, 0xD8, 0x0D, 0x35, 0x10, 0x30, 0x67, 0x85, 0x34, 0xC0, 0x3E, 0x3D, 0x20, 0x26, 0x7B, 0xD9,
        0x39, 0xDA, 0x11, 0x59};
    thread_local EncryptedName<sizeof(downImageSource), 0xF715514F5961B75BULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0xB1, 0x2C, 0xAB, 0xF1, 0xD1, 0xC1, 0x98, 0xB0, 0xB7, 0x67, 0xAB, 0xC7, 0xAC, 0xB5, 0x01, 0x3C, 0x15, 0xF5, 0x0E, 0x65, 0x65, 0x7A, 0x2F, 0x18,
        0x36, 0xF6, 0x02, 0x6F, 0x74, 0x53, 0x2E, 0x1E, 0x03, 0xEC, 0x07, 0x67, 0x4E, 0x6E, 0x2F, 0x0F, 0x1E, 0xF4, 0x34, 0x7E, 0x61, 0x21, 0x3F, 0x10,
        0x01, 0x83};
    thread_local EncryptedName<sizeof(upImageSource), 0x7D5D0F110B6B8371ULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    options->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    const std::uint8_t scrollBackgroundSource[] = {
        0x59, 0x96, 0x3D, 0xCB, 0x95, 0x75, 0x54, 0x8C, 0x5F, 0xDD, 0x3D, 0xFD, 0xE8, 0x01, 0xCD, 0x00, 0xFD, 0x4F, 0x98, 0x5F, 0x21, 0xCE, 0xE3, 0x24,
        0xDE, 0x4C, 0x94, 0x55, 0x30, 0xE7, 0xE2, 0x22, 0xEB, 0x56, 0x91, 0x5D, 0x0A, 0xD9, 0xF6, 0x1E, 0xF4, 0x50, 0x99, 0x1F, 0x37, 0xD6, 0xE1, 0x41};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0x4191BB5531FD3999ULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    options->SetScrollBarBackground(scrollBackground);
  }
  {
    const std::uint8_t thumbBottomSource[] = {
        0xAD, 0x10, 0x15, 0xDD, 0x2B, 0x87, 0x62, 0x1A, 0xAB, 0x5B, 0x15, 0xEB, 0x56, 0xF3, 0xFB, 0x96, 0x09, 0xC9, 0xB0, 0x49, 0x9F, 0x3C, 0xD5, 0xB2,
        0x2A, 0xCA, 0xBC, 0x43, 0x8E, 0x15, 0xD4, 0xB4, 0x1F, 0xD0, 0xB9, 0x4B, 0xB4, 0x3D, 0xCF, 0xA2, 0x00, 0xDD, 0x8A, 0x43, 0x84, 0x3E, 0xC9, 0xF9,
        0x0F, 0xD2, 0xA5, 0x27};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xD7A749EB27D5BF6DULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0x6F, 0xA6, 0xC9, 0x1D, 0x07, 0x07, 0xAC, 0x58, 0x69, 0xED, 0xC9, 0x2B, 0x7A, 0x73, 0x35, 0xD4, 0xCB, 0x7F, 0x6C, 0x89, 0xB3, 0xBC, 0x1B, 0xF0,
        0xE8, 0x7C, 0x60, 0x83, 0xA2, 0x95, 0x1A, 0xF6, 0xDD, 0x66, 0x65, 0x8B, 0x98, 0xBD, 0x01, 0xE0, 0xC2, 0x6B, 0x56, 0x8A, 0xAE, 0xAD, 0x47, 0xF7,
        0xC2, 0x79, 0x09};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x9569C9C7E70909AFULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0x13, 0x96, 0xAF, 0xEB, 0x69, 0xD1, 0x58, 0x7C, 0x15, 0xDD, 0xAF, 0xDD, 0x14, 0xA5, 0xC1, 0xF0, 0xB7, 0x4F, 0x0A, 0x7F, 0xDD, 0x6A, 0xEF, 0xD4,
        0x94, 0x4C, 0x06, 0x75, 0xCC, 0x43, 0xEE, 0xD2, 0xA1, 0x56, 0x03, 0x7D, 0xF6, 0x6B, 0xF5, 0xC4, 0xBE, 0x5B, 0x30, 0x64, 0xD9, 0x31, 0xFF, 0xDC,
        0xA3, 0x39};
    thread_local EncryptedName<sizeof(thumbTopSource), 0xB19D1FA9116F39D3ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    options->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(options);

  CUITextViewer* sources = new CUITextViewer(*this->m_context);
  this->unknown_00F8 = reinterpret_cast<CUIStaticText*>(sources);
  sources->SetSize(108, 118);
  sources->Move(480, 5);
  sources->SetTextOffset(0, 6);
  {
    const std::uint8_t sourceFontSource[] = {
        0x1E, 0x78, 0xA7, 0xC6, 0x10, 0x63};
    thread_local EncryptedName<sizeof(sourceFontSource), 0xB195637DAFCB0D59ULL> sourceFontName(sourceFontSource);
    const std::string sourceFont(sourceFontName.Text());
    sources->SetFont(sourceFont);
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
    const std::uint8_t downImageSource[] = {
        0x01, 0x36, 0x0D, 0x1F, 0x41, 0xE3, 0x80, 0x14, 0x07, 0x7D, 0x0D, 0x29, 0x3C, 0x97, 0x19, 0x98, 0xA5, 0xEF, 0xA8, 0x8B, 0xF5, 0x58, 0x37, 0xBC,
        0x86, 0xEC, 0xA4, 0x81, 0xE4, 0x71, 0x36, 0xBA, 0xB3, 0xF6, 0xA1, 0x89, 0xDE, 0x4C, 0x37, 0xAB, 0xAE, 0xEE, 0x92, 0x81, 0xEE, 0x5A, 0x2B, 0xF7,
        0xA3, 0xF4, 0xBD, 0xE5};
    thread_local EncryptedName<sizeof(downImageSource), 0xD9452D81E5CD99C1ULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0x95, 0x94, 0x41, 0x0D, 0x8D, 0x25, 0x68, 0x60, 0x93, 0xDF, 0x41, 0x3B, 0xF0, 0x51, 0xF1, 0xEC, 0x31, 0x4D, 0xE4, 0x99, 0x39, 0x9E, 0xDF, 0xC8,
        0x12, 0x4E, 0xE8, 0x93, 0x28, 0xB7, 0xDE, 0xCE, 0x27, 0x54, 0xED, 0x9B, 0x12, 0x8A, 0xDF, 0xDF, 0x3A, 0x4C, 0xDE, 0x82, 0x3D, 0xC5, 0xCF, 0xC0,
        0x25, 0x3B};
    thread_local EncryptedName<sizeof(upImageSource), 0xADADEB4DF7813B55ULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    sources->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    const std::uint8_t scrollBackgroundSource[] = {
        0x4D, 0x2E, 0xD1, 0x09, 0x73, 0x67, 0xAC, 0x30, 0x4B, 0x65, 0xD1, 0x3F, 0x0E, 0x13, 0x35, 0xBC, 0xE9, 0xF7, 0x74, 0x9D, 0xC7, 0xDC, 0x1B, 0x98,
        0xCA, 0xF4, 0x78, 0x97, 0xD6, 0xF5, 0x1A, 0x9E, 0xFF, 0xEE, 0x7D, 0x9F, 0xEC, 0xCB, 0x0E, 0xA2, 0xE0, 0xE8, 0x75, 0xDD, 0xD1, 0xC4, 0x19, 0xFD};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0xFD69A9B3F311818DULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    sources->SetScrollBarBackground(scrollBackground);
  }
  {
    const std::uint8_t thumbBottomSource[] = {
        0x53, 0xDA, 0x1F, 0x15, 0x8D, 0xED, 0xCA, 0x56, 0x55, 0x91, 0x1F, 0x23, 0xF0, 0x99, 0x53, 0xDA, 0xF7, 0x03, 0xBA, 0x81, 0x39, 0x56, 0x7D, 0xFE,
        0xD4, 0x00, 0xB6, 0x8B, 0x28, 0x7F, 0x7C, 0xF8, 0xE1, 0x1A, 0xB3, 0x83, 0x12, 0x57, 0x67, 0xEE, 0xFE, 0x17, 0x80, 0x8B, 0x22, 0x54, 0x61, 0xB5,
        0xF1, 0x18, 0xAF, 0xEF};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x9B0F234DEFDF7593ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0xCD, 0xCC, 0x4D, 0xF3, 0x85, 0x9D, 0x4A, 0x14, 0xCB, 0x87, 0x4D, 0xC5, 0xF8, 0xE9, 0xD3, 0x98, 0x69, 0x15, 0xE8, 0x67, 0x31, 0x26, 0xFD, 0xBC,
        0x4A, 0x16, 0xE4, 0x6D, 0x20, 0x0F, 0xFC, 0xBA, 0x7F, 0x0C, 0xE1, 0x65, 0x1A, 0x27, 0xE7, 0xAC, 0x60, 0x01, 0xD2, 0x64, 0x2C, 0x37, 0xA1, 0xBB,
        0x60, 0x13, 0x8D};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xD98F5345098D630DULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0xEB, 0xB0, 0x29, 0x51, 0x01, 0x31, 0xE8, 0xB2, 0xED, 0xFB, 0x29, 0x67, 0x7C, 0x45, 0x71, 0x3E, 0x4F, 0x69, 0x8C, 0xC5, 0xB5, 0x8A, 0x5F, 0x1A,
        0x6C, 0x6A, 0x80, 0xCF, 0xA4, 0xA3, 0x5E, 0x1C, 0x59, 0x70, 0x85, 0xC7, 0x9E, 0x8B, 0x45, 0x0A, 0x46, 0x7D, 0xB6, 0xDE, 0xB1, 0xD1, 0x4F, 0x12,
        0x5B, 0x1F};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x7F2DFFC1ABE91F2BULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    sources->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(sources);
}

// OFFSET: 0x009CC580
// FUNCTION: fn_14005
void CUIMonsterInfo::Slot15(int width, int height) {
  this->unknown_00E8 = new CUIStaticText(*this->m_context);
  this->unknown_00E8->SetSize(280, 12);
  this->unknown_00E8->Move(165, 19);
  this->unknown_00E8->SetText("", 0);
  this->unknown_00E8->SetFitText(0);
  this->unknown_00E8->SetAlign(0, 0);
  {
    CUIStaticText* name = this->unknown_00E8;
    const std::uint8_t nameFontSource[] = {
        0x82, 0x58, 0x47, 0x56, 0xE4, 0x1D};
    thread_local EncryptedName<sizeof(nameFontSource), 0xEF6D1D893F2B2DC5ULL> nameFontName(nameFontSource);
    const std::string nameFont(nameFontName.Text());
    name->SetFont(nameFont);
  }
  this->unknown_00E8->SetFontHeight(12);
  this->unknown_00E8->SetBold(0);
  this->unknown_00E8->SetColor(0xCE6B52);
  this->unknown_00E8->SetShadow(1, 0xDEDEDE);
  this->unknown_00E8->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = reinterpret_cast<CUIStaticText*>(new CUITextViewer(*this->m_context));
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetSize(288, 84);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->Move(165, 39);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetTextOffset(0, 6);
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t detailsFontSource[] = {
        0x1C, 0x26, 0x33, 0xB4, 0xEA, 0x9F};
    thread_local EncryptedName<sizeof(detailsFontSource), 0x39A39F87DD5F535BULL> detailsFontName(detailsFontSource);
    const std::string detailsFont(detailsFontName.Text());
    details->SetFont(detailsFont);
  }
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetPadding(0, 0, 0, 0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetScrollBarSize(8, 84);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetScrollStep(20);
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t downImageSource[] = {
        0x0D, 0xAC, 0x0D, 0x81, 0x57, 0xF5, 0xD6, 0xFE, 0x0B, 0xE7, 0x0D, 0xB7, 0x2A, 0x81, 0x4F, 0x72, 0xA9, 0x75, 0xA8, 0x15, 0xE3, 0x4E, 0x61, 0x56,
        0x8A, 0x76, 0xA4, 0x1F, 0xF2, 0x67, 0x60, 0x50, 0xBF, 0x6C, 0xA1, 0x17, 0xC8, 0x5A, 0x61, 0x41, 0xA2, 0x74, 0x92, 0x1F, 0xF8, 0x4C, 0x7D, 0x1D,
        0xAF, 0x6E, 0xBD, 0x7B};
    thread_local EncryptedName<sizeof(downImageSource), 0x33133B977BCD03CDULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0xB7, 0x38, 0x47, 0xE7, 0xE3, 0x9D, 0x24, 0xA2, 0xB1, 0x73, 0x47, 0xD1, 0x9E, 0xE9, 0xBD, 0x2E, 0x13, 0xE1, 0xE2, 0x73, 0x57, 0x26, 0x93, 0x0A,
        0x30, 0xE2, 0xEE, 0x79, 0x46, 0x0F, 0x92, 0x0C, 0x05, 0xF8, 0xEB, 0x71, 0x7C, 0x32, 0x93, 0x1D, 0x18, 0xE0, 0xD8, 0x68, 0x53, 0x7D, 0x83, 0x02,
        0x07, 0x97};
    thread_local EncryptedName<sizeof(upImageSource), 0x6FE153231D879777ULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    details->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t scrollBackgroundSource[] = {
        0x11, 0xF4, 0x57, 0x1F, 0xF9, 0x49, 0x2E, 0xE2, 0x17, 0xBF, 0x57, 0x29, 0x84, 0x3D, 0xB7, 0x6E, 0xB5, 0x2D, 0xF2, 0x8B, 0x4D, 0xF2, 0x99, 0x4A,
        0x96, 0x2E, 0xFE, 0x81, 0x5C, 0xDB, 0x98, 0x4C, 0xA3, 0x34, 0xFB, 0x89, 0x66, 0xE5, 0x8C, 0x70, 0xBC, 0x32, 0xF3, 0xCB, 0x5B, 0xEA, 0x9B, 0x2F};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0x2FEB8739E5975BD1ULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    details->SetScrollBarBackground(scrollBackground);
  }
  {
    CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t thumbBottomSource[] = {
        0xD7, 0xFE, 0x21, 0xBB, 0xF9, 0x0D, 0x46, 0x4A, 0xD1, 0xB5, 0x21, 0x8D, 0x84, 0x79, 0xDF, 0xC6, 0x73, 0x27, 0x84, 0x2F, 0x4D, 0xB6, 0xF1, 0xE2,
        0x50, 0x24, 0x88, 0x25, 0x5C, 0x9F, 0xF0, 0xE4, 0x65, 0x3E, 0x8D, 0x2D, 0x66, 0xB7, 0xEB, 0xF2, 0x7A, 0x33, 0xBE, 0x25, 0x56, 0xB4, 0xED, 0xA9,
        0x75, 0x3C, 0x91, 0x41};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x8783C33941E15117ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0xEF, 0x78, 0x85, 0xDF, 0xF9, 0x65, 0xE8, 0x6C, 0xE9, 0x33, 0x85, 0xE9, 0x84, 0x11, 0x71, 0xE0, 0x4B, 0xA1, 0x20, 0x4B, 0x4D, 0xDE, 0x5F, 0xC4,
        0x68, 0xA2, 0x2C, 0x41, 0x5C, 0xF7, 0x5E, 0xC2, 0x5D, 0xB8, 0x29, 0x49, 0x66, 0xDF, 0x45, 0xD4, 0x42, 0xB5, 0x1A, 0x48, 0x50, 0xCF, 0x03, 0xC3,
        0x42, 0xA7, 0x45};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xA12DAB392545D72FULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0xB5, 0x94, 0xC3, 0xD5, 0xD3, 0x0F, 0x64, 0xA6, 0xB3, 0xDF, 0xC3, 0xE3, 0xAE, 0x7B, 0xFD, 0x2A, 0x11, 0x4D, 0x66, 0x41, 0x67, 0xB4, 0xD3, 0x0E,
        0x32, 0x4E, 0x6A, 0x4B, 0x76, 0x9D, 0xD2, 0x08, 0x07, 0x54, 0x6F, 0x43, 0x4C, 0xB5, 0xC9, 0x1E, 0x18, 0x59, 0x5C, 0x5A, 0x63, 0xEF, 0xC3, 0x06,
        0x05, 0x3B};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x6BA1C1132F033B75ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    details->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(this->unknown_00EC);

  this->unknown_00F0 = reinterpret_cast<CUIStaticText*>(new CUITextViewer(*this->m_context));
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetSize(108, 118);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->Move(480, 5);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetTextOffset(0, 6);
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t valuesFontSource[] = {
        0xD8, 0xA8, 0xEF, 0x76, 0x1A, 0x45};
    thread_local EncryptedName<sizeof(valuesFontSource), 0x271B45771F83DD9FULL> valuesFontName(valuesFontSource);
    const std::string valuesFont(valuesFontName.Text());
    values->SetFont(valuesFont);
  }
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetPadding(0, 0, 0, 0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetScrollBarSize(8, 118);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetScrollStep(20);
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t downImageSource[] = {
        0x17, 0x3A, 0xDF, 0x61, 0x91, 0x8B, 0x42, 0xC6, 0x11, 0x71, 0xDF, 0x57, 0xEC, 0xFF, 0xDB, 0x4A, 0xB3, 0xE3, 0x7A, 0xF5, 0x25, 0x30, 0xF5, 0x6E,
        0x90, 0xE0, 0x76, 0xFF, 0x34, 0x19, 0xF4, 0x68, 0xA5, 0xFA, 0x73, 0xF7, 0x0E, 0x24, 0xF5, 0x79, 0xB8, 0xE2, 0x40, 0xFF, 0x3E, 0x32, 0xE9, 0x25,
        0xB5, 0xF8, 0x6F, 0x9B};
    thread_local EncryptedName<sizeof(downImageSource), 0x0B8745519B1F95D7ULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0x4F, 0xF6, 0xA1, 0x11, 0x75, 0xD7, 0xC6, 0x0A, 0x49, 0xBD, 0xA1, 0x27, 0x08, 0xA3, 0x5F, 0x86, 0xEB, 0x2F, 0x04, 0x85, 0xC1, 0x6C, 0x71, 0xA2,
        0xC8, 0x2C, 0x08, 0x8F, 0xD0, 0x45, 0x70, 0xA4, 0xFD, 0x36, 0x0D, 0x87, 0xEA, 0x78, 0x71, 0xB5, 0xE0, 0x2E, 0x3E, 0x9E, 0xC5, 0x37, 0x61, 0xAA,
        0xFF, 0x59};
    thread_local EncryptedName<sizeof(upImageSource), 0xC70319B5EB61598FULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    values->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t scrollBackgroundSource[] = {
        0x15, 0x96, 0xEB, 0xB3, 0x27, 0x69, 0x52, 0x68, 0x13, 0xDD, 0xEB, 0x85, 0x5A, 0x1D, 0xCB, 0xE4, 0xB1, 0x4F, 0x4E, 0x27, 0x93, 0xD2, 0xE5, 0xC0,
        0x92, 0x4C, 0x42, 0x2D, 0x82, 0xFB, 0xE4, 0xC6, 0xA7, 0x56, 0x47, 0x25, 0xB8, 0xC5, 0xF0, 0xFA, 0xB8, 0x50, 0x4F, 0x67, 0x85, 0xCA, 0xE7, 0xA5};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0xA597A7E7492B39D5ULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    values->SetScrollBarBackground(scrollBackground);
  }
  {
    CUITextViewer* values = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t thumbBottomSource[] = {
        0x6B, 0x00, 0x4D, 0x77, 0x73, 0x43, 0x62, 0x18, 0x6D, 0x4B, 0x4D, 0x41, 0x0E, 0x37, 0xFB, 0x94, 0xCF, 0xD9, 0xE8, 0xE3, 0xC7, 0xF8, 0xD5, 0xB0,
        0xEC, 0xDA, 0xE4, 0xE9, 0xD6, 0xD1, 0xD4, 0xB6, 0xD9, 0xC0, 0xE1, 0xE1, 0xEC, 0xF9, 0xCF, 0xA0, 0xC6, 0xCD, 0xD2, 0xE9, 0xDC, 0xFA, 0xC9, 0xFB,
        0xC9, 0xC2, 0xFD, 0x8D};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xD5A78DB38D8DAFABULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0xCF, 0xE0, 0x85, 0x55, 0x5D, 0x8B, 0x5C, 0xCE, 0xC9, 0xAB, 0x85, 0x63, 0x20, 0xFF, 0xC5, 0x42, 0x6B, 0x39, 0x20, 0xC1, 0xE9, 0x30, 0xEB, 0x66,
        0x48, 0x3A, 0x2C, 0xCB, 0xF8, 0x19, 0xEA, 0x60, 0x7D, 0x20, 0x29, 0xC3, 0xC2, 0x31, 0xF1, 0x76, 0x62, 0x2D, 0x1A, 0xC2, 0xF4, 0x21, 0xB7, 0x61,
        0x62, 0x3F, 0x45};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x0399459DAF454F0FULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0xB3, 0x40, 0x7B, 0x2B, 0x15, 0x2D, 0x6E, 0x32, 0xB5, 0x0B, 0x7B, 0x1D, 0x68, 0x59, 0xF7, 0xBE, 0x17, 0x99, 0xDE, 0xBF, 0xA1, 0x96, 0xD9, 0x9A,
        0x34, 0x9A, 0xD2, 0xB5, 0xB0, 0xBF, 0xD8, 0x9C, 0x01, 0x80, 0xD7, 0xBD, 0x8A, 0x97, 0xC3, 0x8A, 0x1E, 0x8D, 0xE4, 0xA4, 0xA5, 0xCD, 0xC9, 0x92,
        0x03, 0xEF};
    thread_local EncryptedName<sizeof(thumbTopSource), 0xFFABE3D5D1BBEF73ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    values->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(this->unknown_00F0);
}

// OFFSET: 0x009CFAF0
// FUNCTION: fn_14007
void CUIQuestInfo::Slot15(int width, int height) {
  this->unknown_00E8 = new CUIStaticImage(*this->m_context);
  this->unknown_00E8->SetSize(85, 70);
  this->unknown_00E8->Move(250, 10);
  {
    CUIStaticImage* stamp = static_cast<CUIStaticImage*>(this->unknown_00E8);
    const std::uint8_t stampImageSource[] = {
        0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0xB0, 0x95, 0xB1, 0x5C, 0x67, 0x63, 0x0C, 0x3D, 0x94,
        0xB6, 0xB2, 0x50, 0x6D, 0x72, 0x25, 0x3C, 0x85, 0x90, 0xAA, 0x49, 0x56, 0x74, 0x16, 0x22, 0x81, 0x9D, 0xA2, 0x4D, 0x6C, 0x73, 0x57, 0x2D, 0x9C,
        0x81, 0xC7};
    thread_local EncryptedName<sizeof(stampImageSource), 0xF14F79170939C7F1ULL> stampImageName(stampImageSource);
    const std::string stampImage(stampImageName.Text());
    stamp->SetImage(stampImage);
  }
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = reinterpret_cast<CUIStaticText*>(new CUITextViewer(*this->m_context));
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetSize(360, 57);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->Move(25, 13);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetTextOffset(0, 0);
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t titleFontSource[] = {
        0x32, 0xB8, 0x81, 0x34, 0x06, 0x65};
    thread_local EncryptedName<sizeof(titleFontSource), 0x35E5656B5DEDCD75ULL> titleFontName(titleFontSource);
    const std::string titleFont(titleFontName.Text());
    title->SetFont(titleFont);
  }
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetBold(1);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetPadding(15, 5, 5, 5);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetScrollBarSize(8, 87);
  reinterpret_cast<CUITextViewer*>(this->unknown_00EC)->SetScrollStep(20);
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t downImageSource[] = {
        0x3D, 0x78, 0x8D, 0x53, 0x75, 0x41, 0x52, 0x2A, 0x3B, 0x33, 0x8D, 0x65, 0x08, 0x35, 0xCB, 0xA6, 0x99, 0xA1, 0x28, 0xC7, 0xC1, 0xFA, 0xE5, 0x82,
        0xBA, 0xA2, 0x24, 0xCD, 0xD0, 0xD3, 0xE4, 0x84, 0x8F, 0xB8, 0x21, 0xC5, 0xEA, 0xEE, 0xE5, 0x95, 0x92, 0xA0, 0x12, 0xCD, 0xDA, 0xF8, 0xF9, 0xC9,
        0x9F, 0xBA, 0x3D, 0xA9};
    thread_local EncryptedName<sizeof(downImageSource), 0xE7978FB5A94DD7FDULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0x3D, 0x16, 0x7B, 0xDB, 0x5F, 0x33, 0x84, 0x84, 0x3B, 0x5D, 0x7B, 0xED, 0x22, 0x47, 0x1D, 0x08, 0x99, 0xCF, 0xDE, 0x4F, 0xEB, 0x88, 0x33, 0x2C,
        0xBA, 0xCC, 0xD2, 0x45, 0xFA, 0xA1, 0x32, 0x2A, 0x8F, 0xD6, 0xD7, 0x4D, 0xC0, 0x9C, 0x33, 0x3B, 0x92, 0xCE, 0xE4, 0x54, 0xEF, 0xD3, 0x23, 0x24,
        0x8D, 0xB9};
    thread_local EncryptedName<sizeof(upImageSource), 0x4941FD9F21BBB9FDULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    title->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t scrollBackgroundSource[] = {
        0xAB, 0x16, 0x07, 0xF5, 0xA7, 0x41, 0x38, 0x7A, 0xAD, 0x5D, 0x07, 0xC3, 0xDA, 0x35, 0xA1, 0xF6, 0x0F, 0xCF, 0xA2, 0x61, 0x13, 0xFA, 0x8F, 0xD2,
        0x2C, 0xCC, 0xAE, 0x6B, 0x02, 0xD3, 0x8E, 0xD4, 0x19, 0xD6, 0xAB, 0x63, 0x38, 0xED, 0x9A, 0xE8, 0x06, 0xD0, 0xA3, 0x21, 0x05, 0xE2, 0x8D, 0xB7};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0xB7FD8F670FC7B96BULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    title->SetScrollBarBackground(scrollBackground);
  }
  {
    CUITextViewer* title = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
    const std::uint8_t thumbBottomSource[] = {
        0x0D, 0x5E, 0x59, 0x4F, 0x4D, 0x95, 0xC2, 0x60, 0x0B, 0x15, 0x59, 0x79, 0x30, 0xE1, 0x5B, 0xEC, 0xA9, 0x87, 0xFC, 0xDB, 0xF9, 0x2E, 0x75, 0xC8,
        0x8A, 0x84, 0xF0, 0xD1, 0xE8, 0x07, 0x74, 0xCE, 0xBF, 0x9E, 0xF5, 0xD9, 0xD2, 0x2F, 0x6F, 0xD8, 0xA0, 0x93, 0xC6, 0xD1, 0xE2, 0x2C, 0x69, 0x83,
        0xAF, 0x9C, 0xE9, 0xB5};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xAD075B8DB599F1CDULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0x57, 0x5E, 0x21, 0x27, 0x11, 0x5D, 0x2C, 0xE2, 0x51, 0x15, 0x21, 0x11, 0x6C, 0x29, 0xB5, 0x6E, 0xF3, 0x87, 0x84, 0xB3, 0xA5, 0xE6, 0x9B, 0x4A,
        0xD0, 0x84, 0x88, 0xB9, 0xB4, 0xCF, 0x9A, 0x4C, 0xE5, 0x9E, 0x8D, 0xB1, 0x8E, 0xE7, 0x81, 0x5A, 0xFA, 0x93, 0xBE, 0xB0, 0xB8, 0xF7, 0xC7, 0x4D,
        0xFA, 0x81, 0xE1};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0x2FE993D1DDE1F197ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0x4F, 0x36, 0x2B, 0xF7, 0x75, 0xB1, 0x3A, 0xF4, 0x49, 0x7D, 0x2B, 0xC1, 0x08, 0xC5, 0xA3, 0x78, 0xEB, 0xEF, 0x8E, 0x63, 0xC1, 0x0A, 0x8D, 0x5C,
        0xC8, 0xEC, 0x82, 0x69, 0xD0, 0x23, 0x8C, 0x5A, 0xFD, 0xF6, 0x87, 0x61, 0xEA, 0x0B, 0x97, 0x4C, 0xE2, 0xFB, 0xB4, 0x78, 0xC5, 0x51, 0x9D, 0x54,
        0xFF, 0x99};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x39FF7FB50DEB998FULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    title->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(this->unknown_00EC);

  this->unknown_00F0 = reinterpret_cast<CUIStaticText*>(new CUITextViewer(*this->m_context));
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetSize(200, 87);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->Move(400, 5);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetTextOffset(0, 6);
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t linesFontSource[] = {
        0x38, 0x60, 0xE1, 0x78, 0x5A, 0x0F};
    thread_local EncryptedName<sizeof(linesFontSource), 0x2F8B0F37118D157FULL> linesFontName(linesFontSource);
    const std::string linesFont(linesFontName.Text());
    lines->SetFont(linesFont);
  }
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetBold(1);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetPadding(15, 5, 5, 5);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetScrollBarSize(8, 87);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->SetScrollStep(20);
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t downImageSource[] = {
        0xE3, 0x5E, 0x17, 0xED, 0x17, 0x87, 0x26, 0xB0, 0xE5, 0x15, 0x17, 0xDB, 0x6A, 0xF3, 0xBF, 0x3C, 0x47, 0x87, 0xB2, 0x79, 0xA3, 0x3C, 0x91, 0x18,
        0x64, 0x84, 0xBE, 0x73, 0xB2, 0x15, 0x90, 0x1E, 0x51, 0x9E, 0xBB, 0x7B, 0x88, 0x28, 0x91, 0x0F, 0x4C, 0x86, 0x88, 0x73, 0xB8, 0x3E, 0x8D, 0x53,
        0x41, 0x9C, 0xA7, 0x17};
    thread_local EncryptedName<sizeof(downImageSource), 0x7DE349D717D7F123ULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0xBF, 0x7A, 0x6F, 0xA1, 0xAB, 0x7F, 0xD0, 0xC2, 0xB9, 0x31, 0x6F, 0x97, 0xD6, 0x0B, 0x49, 0x4E, 0x1B, 0xA3, 0xCA, 0x35, 0x1F, 0xC4, 0x67, 0x6A,
        0x38, 0xA0, 0xC6, 0x3F, 0x0E, 0xED, 0x66, 0x6C, 0x0D, 0xBA, 0xC3, 0x37, 0x34, 0xD0, 0x67, 0x7D, 0x10, 0xA2, 0xF0, 0x2E, 0x1B, 0x9F, 0x77, 0x62,
        0x0F, 0xD5};
    thread_local EncryptedName<sizeof(upImageSource), 0x0F15B16B5BAFD57FULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    lines->SetScrollBarButtonImages(upImage, downImage);
  }
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t scrollBackgroundSource[] = {
        0x21, 0x36, 0x47, 0xB5, 0x41, 0x0D, 0xE4, 0xBE, 0x27, 0x7D, 0x47, 0x83, 0x3C, 0x79, 0x7D, 0x32, 0x85, 0xEF, 0xE2, 0x21, 0xF5, 0xB6, 0x53, 0x16,
        0xA6, 0xEC, 0xEE, 0x2B, 0xE4, 0x9F, 0x52, 0x10, 0x93, 0xF6, 0xEB, 0x23, 0xDE, 0xA1, 0x46, 0x2C, 0x8C, 0xF0, 0xE3, 0x61, 0xE3, 0xAE, 0x51, 0x73};
    thread_local EncryptedName<sizeof(scrollBackgroundSource), 0x7321C3814F8799E1ULL> scrollBackgroundName(scrollBackgroundSource);
    const std::string scrollBackground(scrollBackgroundName.Text());
    lines->SetScrollBarBackground(scrollBackground);
  }
  {
    CUITextViewer* lines = reinterpret_cast<CUITextViewer*>(this->unknown_00F0);
    const std::uint8_t thumbBottomSource[] = {
        0x09, 0xBC, 0x39, 0x89, 0x39, 0x17, 0xD0, 0xBC, 0x0F, 0xF7, 0x39, 0xBF, 0x44, 0x63, 0x49, 0x30, 0xAD, 0x65, 0x9C, 0x1D, 0x8D, 0xAC, 0x67, 0x14,
        0x8E, 0x66, 0x90, 0x17, 0x9C, 0x85, 0x66, 0x12, 0xBB, 0x7C, 0x95, 0x1F, 0xA6, 0xAD, 0x7D, 0x04, 0xA4, 0x71, 0xA6, 0x17, 0x96, 0xAE, 0x7B, 0x5F,
        0xAB, 0x7E, 0x89, 0x73};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x7115D9F973F913C9ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0xA3, 0x5A, 0x21, 0x85, 0xFB, 0xEB, 0xD8, 0x06, 0xA5, 0x11, 0x21, 0xB3, 0x86, 0x9F, 0x41, 0x8A, 0x07, 0x83, 0x84, 0x11, 0x4F, 0x50, 0x6F, 0xAE,
        0x24, 0x80, 0x88, 0x1B, 0x5E, 0x79, 0x6E, 0xA8, 0x11, 0x9A, 0x8D, 0x13, 0x64, 0x51, 0x75, 0xBE, 0x0E, 0x97, 0xBE, 0x12, 0x52, 0x41, 0x33, 0xA9,
        0x0E, 0x85, 0xE1};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xCB1D253B7FE1F563ULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0x8F, 0x0E, 0x0F, 0xCF, 0xA3, 0x19, 0x84, 0xB4, 0x89, 0x45, 0x0F, 0xF9, 0xDE, 0x6D, 0x1D, 0x38, 0x2B, 0xD7, 0xAA, 0x5B, 0x17, 0xA2, 0x33, 0x1C,
        0x08, 0xD4, 0xA6, 0x51, 0x06, 0x8B, 0x32, 0x1A, 0x3D, 0xCE, 0xA3, 0x59, 0x3C, 0xA3, 0x29, 0x0C, 0x22, 0xC3, 0x90, 0x40, 0x13, 0xF9, 0x23, 0x14,
        0x3F, 0xA1};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x7941D76335CFA14FULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    lines->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(this->unknown_00F0);

  this->unknown_00F4 = reinterpret_cast<CUIStaticText*>(new CUITextViewer(*this->m_context));
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetSize(360, 13);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->Move(25, 73);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextOffset(0, 0);
  {
    CUITextViewer* status = reinterpret_cast<CUITextViewer*>(this->unknown_00F4);
    const std::uint8_t statusFontSource[] = {
        0x5A, 0x70, 0xFF, 0x40, 0x1C, 0x23};
    thread_local EncryptedName<sizeof(statusFontSource), 0xE55723712993051DULL> statusFontName(statusFontSource);
    const std::string statusFont(statusFontName.Text());
    status->SetFont(statusFont);
  }
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetFontHeight(12);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetBold(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetTextColor(0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetOutline(0, 0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetLineSpacing(6);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetBackgroundColor(0xFFFF00FF);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetPadding(15, 0, 3, 0);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetScrollBarSize(8, 12);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetScrollStep(1);
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetScrollBarButtonImages("", "");
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetScrollBarBackground("");
  reinterpret_cast<CUITextViewer*>(this->unknown_00F4)->SetScrollBarThumbImages("", "", "");
  this->AddChild(this->unknown_00F4);
}

// OFFSET: 0x009DCE10
// FUNCTION: fn_14330
void CUICharacterInfo::Slot16() {
  this->jobIcon->SetImage(std::format("유저인터페이스\\renewalparty\\icon_jobs_{}.bmp", g_session.m_job));
  const std::string level = std::format("Lv. {}", g_session.m_level);
  this->level->SetText(level.c_str(), level.size());
  const std::string jobLevel = std::format("Jv. {}", g_session.m_jobLevel);
  this->jobLevel->SetText(jobLevel.c_str(), jobLevel.size());
  const char* name = g_session.DecodePlayerName();
  this->name->SetText(name, std::strlen(name));
}

// OFFSET: 0x009DD750
// FUNCTION: fn_14343
void CUIItemInfo::Slot32(int, int) {
  ITEM_INFO info;
  info.SetItemId(this->itemId);
  info.unknown_0000[0] = this->type;
  info.identified = 1;
  if (this->type == 5) {
    info.unknown_0000[2] = this->location > 9 ? 0x22 : 2;
  } else if (this->type == 4) {
    switch (this->location) {
    case 0:
      info.unknown_0000[2] = 0x100;
      break;
    case 1:
      info.unknown_0000[2] = 0x200;
      break;
    case 2:
      info.unknown_0000[2] = 1;
      break;
    case 3:
      info.unknown_0000[2] = 0x10;
      break;
    case 4:
      info.unknown_0000[2] = 4;
      break;
    case 5:
      info.unknown_0000[2] = 0x20;
      break;
    case 6:
      info.unknown_0000[2] = 0x40;
      break;
    case 7:
      info.unknown_0000[2] = 0x88;
      break;
    case 8:
      info.unknown_0000[2] = 8;
      break;
    case 9:
      info.unknown_0000[2] = 0x80;
      break;
    default:
      info.unknown_0000[2] = 0;
      break;
    }
  } else {
    info.unknown_0000[2] = 0;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(12);
  if (window) {
    window->SendMsg(nullptr, 0x18, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
  }
}

// OFFSET: 0x009DC720
// FUNCTION: fn_14322
void CUIMonster::BindMonster(CUIMonsterInfo* item, const CMonsterData& data) {
  item->SetMonster(data.name);
  reinterpret_cast<CUITextViewer*>(item->unknown_00F0)->Clear();
  for (const std::string& value : data.values) {
    reinterpret_cast<CUITextViewer*>(item->unknown_00F0)->AddLine(value);
  }
}

// OFFSET: 0x009DFCB0
// FUNCTION: fn_14360
void CUIMonsterInfo::SetMonster(const std::string& title) {
  CUITextViewer* details = reinterpret_cast<CUITextViewer*>(this->unknown_00EC);
  details->Clear();

  std::string name;
  std::string spriteName;
  int x = 0;
  int size = 0;
  int element = 0;
  int elementLevel = 0;
  int race = 0;
  int type = 0;
  if (!g_navigationManager.GetMobInfo(title, &name, &spriteName, &x, &size, &element, &elementLevel, &race, &type)) {
    this->unknown_00E8->SetText(title.c_str(), title.size());
    return;
  }

  this->unknown_00E8->SetText(name.c_str(), name.size());
  this->sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName + ".spr"));
  this->action = static_cast<CActRes*>(ResourceMgr::Instance().Load(spriteName + ".act"));

  if (element == 1) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8D7), elementLevel));
  } else if (element == 2) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8D8), elementLevel));
  } else if (element == 3) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8D9), elementLevel));
  } else if (element == 4) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8DA), elementLevel));
  } else if (element == 5) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8DB), elementLevel));
  } else if (element == 6) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8DC), elementLevel));
  } else if (element == 7) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8DD), elementLevel));
  } else if (element == 8) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8DE), elementLevel));
  } else if (element == 9) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8DF), elementLevel));
  } else {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E0), elementLevel));
  }

  if (race == 1) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E4)));
  } else if (race == 2) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E5)));
  } else if (race == 3) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E6)));
  } else if (race == 4) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E7)));
  } else if (race == 5) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E8)));
  } else if (race == 6) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E9)));
  } else if (race == 7) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8EA)));
  } else if (race == 8) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8EB)));
  } else if (race == 9) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8EC)));
  } else {
    details->AddLine(FormatOwnedString(LookupMessage(0x8ED)));
  }

  if (size == 1) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E1)));
  } else if (size == 2) {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E2)));
  } else {
    details->AddLine(FormatOwnedString(LookupMessage(0x8E3)));
  }

  if (type == 0x12D) {
    details->SetTextColor(0x6464FFu);
    details->AddLine(FormatOwnedString(LookupMessage(0x8B6)));
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
  this->subTabs = new CUIListBox<CUISubTab>(*this->m_context);
  this->subTabs->SetSize(98, 26);
  this->subTabs->Move(0, 40);
  this->subTabs->SetOnSelect([this](CUISubTab* subTab) { this->OnSelectSubTab(subTab); });

  CUIListBox<CUISubTab>* list = this->subTabs;
  {
    const std::uint8_t disabledImageSource[] = {
        0x97, 0x50, 0x2D, 0xB5, 0x6F, 0x7B, 0x12, 0x78, 0x91, 0x1B, 0x2D, 0x83, 0x12, 0x0F, 0x8B, 0xF4, 0x33, 0x89, 0x88, 0x21, 0xDB, 0xC0, 0xA5, 0xD0,
        0x10, 0x8A, 0x84, 0x2B, 0xCA, 0xE9, 0xA3, 0xD4, 0x35, 0xA0, 0x8F, 0x3B, 0xC1, 0xEA, 0xA4, 0xC0, 0x35, 0xA0, 0x83, 0x20, 0xDD, 0xD8, 0xB6, 0xD9,
        0x79, 0x9D, 0x80, 0x3F, 0xAF};
    thread_local EncryptedName<sizeof(disabledImageSource), 0xB5D7B5AF4FEDFF57ULL> disabledImageName(disabledImageSource);
    const std::string disabledImage(disabledImageName.Text());
    const std::uint8_t pressedImageSource[] = {
        0x11, 0x76, 0xD9, 0xE3, 0x6B, 0x95, 0x18, 0xAA, 0x17, 0x3D, 0xD9, 0xD5, 0x16, 0xE1, 0x81, 0x26, 0xB5, 0xAF, 0x7C, 0x77, 0xDF, 0x2E, 0xAF, 0x02,
        0x96, 0xAC, 0x70, 0x7D, 0xCE, 0x07, 0xA9, 0x06, 0xB3, 0x86, 0x7B, 0x6D, 0xC5, 0x04, 0xAE, 0x12, 0xB3, 0x86, 0x76, 0x6F, 0xCE, 0x29, 0xF3, 0x05,
        0xBC, 0xA9, 0x19};
    thread_local EncryptedName<sizeof(pressedImageSource), 0x67DD5BAB1919D9D1ULL> pressedImageName(pressedImageSource);
    const std::string pressedImage(pressedImageName.Text());
    const std::uint8_t hoverImageSource[] = {
        0x33, 0xBA, 0xB3, 0xD9, 0xB9, 0x39, 0x36, 0x78, 0x35, 0xF1, 0xB3, 0xEF, 0xC4, 0x4D, 0xAF, 0xF4, 0x97, 0x63, 0x16, 0x4D, 0x0D, 0x82, 0x81, 0xD0,
        0xB4, 0x60, 0x1A, 0x47, 0x1C, 0xAB, 0x87, 0xD4, 0x91, 0x4A, 0x11, 0x57, 0x17, 0xA8, 0x80, 0xC0, 0x91, 0x4A, 0x03, 0x51, 0x1C, 0x84, 0x80, 0x9B,
        0x91, 0x78, 0x03, 0x23};
    thread_local EncryptedName<sizeof(hoverImageSource), 0xB5F3F779237315F3ULL> hoverImageName(hoverImageSource);
    const std::string hoverImage(hoverImageName.Text());
    const std::uint8_t normalImageSource[] = {
        0xC3, 0x9A, 0x59, 0x4F, 0x73, 0xB9, 0x0C, 0xA6, 0xC5, 0xD1, 0x59, 0x79, 0x0E, 0xCD, 0x95, 0x2A, 0x67, 0x43, 0xFC, 0xDB, 0xC7, 0x02, 0xBB, 0x0E,
        0x44, 0x40, 0xF0, 0xD1, 0xD6, 0x2B, 0xBD, 0x0A, 0x61, 0x6A, 0xFB, 0xC1, 0xDD, 0x28, 0xBA, 0x1E, 0x61, 0x6A, 0xF7, 0xDA, 0xC1, 0x1A, 0xA8, 0x07,
        0x2D, 0x57, 0xF4, 0xC5, 0xB3};
    thread_local EncryptedName<sizeof(normalImageSource), 0x6BC977B3B5993503ULL> normalImageName(normalImageSource);
    const std::string normalImage(normalImageName.Text());
    list->SetImages(normalImage, hoverImage, pressedImage, disabledImage);
  }

  this->subTabs->itemWidth = 98;
  this->subTabs->rowHeight = 26;
  this->subTabs->itemX = 0;
  this->subTabs->itemY = 0;
  this->subTabs->scrollBar->SetSize(9, 26);
  this->subTabs->scrollBar->Move(89, 0);
  this->subTabs->scrollBar->step = 20;
  {
    const std::uint8_t downImageSource[] = {
        0x77, 0x9C, 0xE5, 0xF3, 0x7B, 0xFB, 0x40, 0xCE, 0x71, 0xD7, 0xE5, 0xC5, 0x06, 0x8F, 0xD9, 0x42, 0xD3, 0x45, 0x40, 0x67, 0xCF, 0x40, 0xF7, 0x66,
        0xF0, 0x46, 0x4C, 0x6D, 0xDE, 0x69, 0xF6, 0x60, 0xC5, 0x5C, 0x49, 0x65, 0xE4, 0x54, 0xF7, 0x71, 0xD8, 0x44, 0x7A, 0x6D, 0xD4, 0x42, 0xEB, 0x2D,
        0xD5, 0x5E, 0x55, 0x09};
    thread_local EncryptedName<sizeof(downImageSource), 0x038535BB092533B7ULL> downImageName(downImageSource);
    const std::string downImage(downImageName.Text());
    const std::uint8_t upImageSource[] = {
        0xB9, 0x58, 0xD5, 0xCF, 0x6D, 0x9F, 0x58, 0x56, 0xBF, 0x13, 0xD5, 0xF9, 0x10, 0xEB, 0xC1, 0xDA, 0x1D, 0x81, 0x70, 0x5B, 0xD9, 0x24, 0xEF, 0xFE,
        0x3E, 0x82, 0x7C, 0x51, 0xC8, 0x0D, 0xEE, 0xF8, 0x0B, 0x98, 0x79, 0x59, 0xF2, 0x30, 0xEF, 0xE9, 0x16, 0x80, 0x4A, 0x40, 0xDD, 0x7F, 0xFF, 0xF6,
        0x09, 0xF7};
    thread_local EncryptedName<sizeof(upImageSource), 0x9B9D51AD3515F779ULL> upImageName(upImageSource);
    const std::string upImage(upImageName.Text());
    this->subTabs->scrollBar->SetButtonImages(upImage, downImage);
  }
  {
    const std::uint8_t backgroundImageSource[] = {
        0x13, 0xD8, 0x27, 0x93, 0x97, 0x09, 0xEE, 0x36, 0x15, 0x93, 0x27, 0xA5, 0xEA, 0x7D, 0x77, 0xBA, 0xB7, 0x01, 0x82, 0x07, 0x23, 0xB2, 0x59, 0x9E,
        0x94, 0x02, 0x8E, 0x0D, 0x32, 0x9B, 0x58, 0x98, 0xA1, 0x18, 0x8B, 0x05, 0x08, 0xA5, 0x4C, 0xA4, 0xBE, 0x1E, 0x83, 0x47, 0x35, 0xAA, 0x5B, 0xFB};
    thread_local EncryptedName<sizeof(backgroundImageSource), 0xFB2BC75769E777D3ULL> backgroundImageName(backgroundImageSource);
    const std::string backgroundImage(backgroundImageName.Text());
    this->subTabs->scrollBar->SetBackground(backgroundImage);
  }
  {
    const std::uint8_t thumbBottomSource[] = {
        0xB3, 0x8E, 0x27, 0xBD, 0xEB, 0xB7, 0x7A, 0x46, 0xB5, 0xC5, 0x27, 0x8B, 0x96, 0xC3, 0xE3, 0xCA, 0x17, 0x57, 0x82, 0x29, 0x5F, 0x0C, 0xCD, 0xEE,
        0x34, 0x54, 0x8E, 0x23, 0x4E, 0x25, 0xCC, 0xE8, 0x01, 0x4E, 0x8B, 0x2B, 0x74, 0x0D, 0xD7, 0xFE, 0x1E, 0x43, 0xB8, 0x23, 0x44, 0x0E, 0xD1, 0xA5,
        0x11, 0x4C, 0x97, 0x47};
    thread_local EncryptedName<sizeof(thumbBottomSource), 0x8BBF792B47E72173ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
        0xBD, 0xFC, 0x4F, 0x5B, 0x3F, 0x23, 0xAC, 0x10, 0xBB, 0xB7, 0x4F, 0x6D, 0x42, 0x57, 0x35, 0x9C, 0x19, 0x25, 0xEA, 0xCF, 0x8B, 0x98, 0x1B, 0xB8,
        0x3A, 0x26, 0xE6, 0xC5, 0x9A, 0xB1, 0x1A, 0xBE, 0x0F, 0x3C, 0xE3, 0xCD, 0xA0, 0x99, 0x01, 0xA8, 0x10, 0x31, 0xD0, 0xCC, 0x96, 0x89, 0x47, 0xBF,
        0x10, 0x23, 0x8F};
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xDD69EDFFA18F537DULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
        0x51, 0x86, 0xB9, 0xB9, 0x1B, 0xC7, 0x60, 0xBC, 0x57, 0xCD, 0xB9, 0x8F, 0x66, 0xB3, 0xF9, 0x30, 0xF5, 0x5F, 0x1C, 0x2D, 0xAF, 0x7C, 0xD7, 0x14,
        0xD6, 0x5C, 0x10, 0x27, 0xBE, 0x55, 0xD6, 0x12, 0xE3, 0x46, 0x15, 0x2F, 0x84, 0x7D, 0xCD, 0x04, 0xFC, 0x4B, 0x26, 0x36, 0xAB, 0x27, 0xC7, 0x1C,
        0xE1, 0x29};
    thread_local EncryptedName<sizeof(thumbTopSource), 0x71A509DB43792991ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->subTabs->scrollBar->SetThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }

  this->AddChild(this->subTabs);
  return this->subTabs;
}

}

CUIAdventureGuide::CUIAdventureGuide()
    : background(nullptr), titleBar(nullptr), characterInfo(nullptr), titleText(nullptr), intro(nullptr), quest(nullptr), monster(nullptr), item(nullptr), tabs(nullptr) {
}

// OFFSET: 0x009C1230
// FUNCTION: fn_13986
CUIAdventureGuide* CreateAdventureGuide() {
  return new CUIAdventureGuide();
}
