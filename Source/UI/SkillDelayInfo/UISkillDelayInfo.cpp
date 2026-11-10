#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/SkillDelayInfo/SkillDelayInfo.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Script/LuaScript.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include "Resource/MSGSTRINGID.h"

template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Paint();
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnWheel(int value);
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnScroll(int position);
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::AddNewItem();
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Resize(int width, int height);

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  template <typename Byte>
  explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
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

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

struct SkillDelayInfoFreezeCallback {
  virtual ~SkillDelayInfoFreezeCallback() = default;

  /* 0004 */ CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list;

  void operator()(CUIListItem* const& item);
};

struct SkillDelayInfoSelectCallback {
  virtual ~SkillDelayInfoSelectCallback() = default;

  /* 0004 */ CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list;

  void operator()(CUIListItem* const& item);
};

// OFFSET: 0x00A71140
// FUNCTION: fn_16799
template <>
CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A71300
// FUNCTION: fn_16800
template <>
CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::~CUIListBoxEx() {
}

// OFFSET: 0x00A718D0
// FUNCTION: fn_16805
CUISkillDelayInfo* CreateSkillDelayInfo() {
  return new CUISkillDelayInfo();
}

// OFFSET: 0x0049C2D0
// FUNCTION: fn_45556
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateSkillDelayInfo;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10030, factory);
  return true;
}();

// OFFSET: 0x00A71950
// FUNCTION: fn_16806
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::ClearItems() {
  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A719E0
// FUNCTION: fn_16807
void CUISkillDelayInfo::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = true;
  this->Resize(345, 379);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
  this->SetSkinColors(0, 0, 0, 0);
}

// OFFSET: 0x00A71B50
// FUNCTION: fn_16808
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  {
    const std::function<void(int)> onChange = [this](int position) { this->OnScroll(position); };
    this->scrollBar->onChange = onChange;
  }
  this->AddChild(this->scrollBar);
}

namespace SkillDelayInfo {

// OFFSET: 0x00A71D60
// FUNCTION: fn_16809
void CUIDelayItem::OnCreate(int width, int height) {
  this->level = new CUIStaticText();
  this->level->SetSize(44, 21);
  this->level->Move(0, 0);
  this->level->SetText("");
  this->level->SetFitText(0);
  this->level->SetAlign(6, 6);
  const std::uint8_t levelFontSource[] = {0x1E, 0xF0, 0xEB, 0x10, 0x9A, 0x2F};
  // GLOBAL: gl_25165
  thread_local EncryptedName<sizeof(levelFontSource), 0xE33F2FF779878559ULL> levelFont(levelFontSource);
  this->level->SetFont(std::string(levelFont.Text()));
  this->level->SetFontHeight(12);
  this->level->SetBold(0);
  this->level->SetColor(0);
  this->level->SetShadow(0, 0xFFFFFF);
  this->level->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->level);

  for (int index = 4; index != 0; --index) {
    CUITime* time = new CUITime();
    time->SetSize(70, 21);
    this->AddChild(time);
    this->times.push_back(time);
  }

  this->times[0]->Move(44, 0);
  this->times[1]->Move(114, 0);
  this->times[2]->Move(184, 0);
  this->times[3]->Move(254, 0);
}

// OFFSET: 0x00A75A00
// FUNCTION: fn_16811
void CUITime::OnCreate(int width, int height) {
  this->text = new CUIStaticText();
  this->text->SetSize(60, 21);
  this->text->Move(5, 0);
  this->text->SetText("");
  this->text->SetFitText(0);
  this->text->SetAlign(6, 6);
  const std::uint8_t textFontSource[] = {0xEC, 0xDA, 0xE1, 0xE4, 0xDE, 0x8D};
  // GLOBAL: gl_25166
  thread_local EncryptedName<sizeof(textFontSource), 0xD5A78DB38D8DAFABULL> textFont(textFontSource);
  this->text->SetFont(std::string(textFont.Text()));
  this->text->SetFontHeight(12);
  this->text->SetBold(0);
  this->text->SetColor(0);
  this->text->SetShadow(0, 0xFFFFFF);
  this->text->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->text);
}

}

// OFFSET: 0x00A75C80
// FUNCTION: fn_16812
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A75C90
// FUNCTION: fn_16813
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A75D20
// FUNCTION: fn_16814
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
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
    for (SkillDelayInfo::CUIDelayItem*& slot : this->items) {
      SkillDelayInfo::CUIDelayItem* item = new SkillDelayInfo::CUIDelayItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (SkillDelayInfo::CUIDelayItem* row : this->items) {
          row->SetHighlighted(listItem == row);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->layoutFlag) {
          return;
        }
        for (SkillDelayInfo::CUIDelayItem* row : this->items) {
          if (listItem != row) {
            row->SetSelected(false);
          }
        }

        listItem->SetSelected(true);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
          return;
        }
        SkillDelayInfo::CUIDelayItem* delayItem = dynamic_cast<SkillDelayInfo::CUIDelayItem*>(listItem);
        if (!delayItem) {
          return;
        }
        this->selected = delayItem->index;
        if (this->onSelect && delayItem->index >= 0 && static_cast<unsigned int>(delayItem->index) < this->data.size()) {
          auto node = this->data.begin();
          for (int index = delayItem->index; index > 0; --index) {
            ++node;
          }
          this->onSelect(delayItem, *node);
        }
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A75F80
// FUNCTION: fn_16815
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::LoadSkin(bool resolveSkin) {
  this->normalImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
    item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x00A76190
// FUNCTION: fn_16816
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A76210
// FUNCTION: fn_16817
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnScroll(int position) {
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
    SkillDelayInfo::CUIDelayItem* item = *it;
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

// OFFSET: 0x00A768F0
// FUNCTION: fn_16833
void SkillDelayInfoFreezeCallback::operator()(CUIListItem* const& item) {
  CUIListItem* selectedItem = item;
  for (SkillDelayInfo::CUIDelayItem* row : this->list->items) {
    row->SetHighlighted(selectedItem == row);
  }
}

// OFFSET: 0x00A76960
// FUNCTION: fn_16835
void SkillDelayInfoSelectCallback::operator()(CUIListItem* const& item) {
  CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list = this->list;
  CUIListItem* selectedItem = item;
  if (list->layoutFlag) {
    return;
  }
  for (SkillDelayInfo::CUIDelayItem* row : list->items) {
    if (selectedItem != row) {
      row->SetSelected(false);
    }
  }

  selectedItem->SetSelected(true);
  if (selectedItem->index == static_cast<int>(list->selected) && list->selectionFlag) {
    return;
  }
  SkillDelayInfo::CUIDelayItem* delayItem = dynamic_cast<SkillDelayInfo::CUIDelayItem*>(selectedItem);
  if (!delayItem) {
    return;
  }
  const int selectedIndex = delayItem->index;
  list->selected = selectedIndex;
  if (list->onSelect && selectedIndex >= 0 && static_cast<unsigned int>(selectedIndex) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = selectedIndex; index > 0; --index) {
      ++node;
    }
    list->onSelect(delayItem, *node);
  }
}

// OFFSET: 0x00A77200
// FUNCTION: fn_16872
CUISkillDelayInfo::CUISkillDelayInfo()
    : titleBar(nullptr), skillNameText(nullptr), labels{nullptr, nullptr, nullptr}, columns{nullptr, nullptr, nullptr, nullptr}, delayList(nullptr), description(nullptr), backgroundColor(0), borderColor(0), skillId(7999) {
}

namespace SkillDelayInfo {
// OFFSET: 0x00A77160
// FUNCTION: fn_16871
CUIDelayItem::CUIDelayItem() : CUIListItem(), level(nullptr), times(), backgroundColor(0), borderColor(0) {
}
}

namespace SkillDelayInfo {
// OFFSET: 0x00A77350
// FUNCTION: fn_16873
CUITime::CUITime() : CUIControl(), text(nullptr) {
}
}

namespace SkillDelayInfo {
// OFFSET: 0x00A778C0
// FUNCTION: fn_16877
CUITime::~CUITime() = default;
}

// OFFSET: 0x00A77A70
// FUNCTION: fn_16879
void CUISkillDelayInfo::OnButtonClick() {
}

// OFFSET: 0x00A77B00
// FUNCTION: fn_45472
void CUISkillDelayInfo::OnClose() {
  g_windowMgr.CloseWindow(WID_SKILLDELAYINFO);
}

// OFFSET: 0x00A77B10
// FUNCTION: fn_16880
void CUISkillDelayInfo::OnSelectItem(SkillDelayInfo::CUIDelayItem*, const SkillDelayInfo::CDelayItemData&) {
}

// OFFSET: 0x00A77B20
// FUNCTION: fn_16881
void CUISkillDelayInfo::OnUpdateItem(SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) {
  item->level->SetText(std::to_string(data.levelIndex + 1));

  for (int index = 0; index < 4; ++index) {
    const auto found = data.delayTimes.find(index);
    item->times[index]->SetTime(found != data.delayTimes.end() ? found->second : 0);
  }
}

namespace SkillDelayInfo {

// OFFSET: 0x00A77CE0
// FUNCTION: fn_16882
void CUIDelayItem::Paint() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
  DrawRectangleBorder(*this, 0, 0, 325, 22, this->borderColor);
  this->FillRegion(1, 0, 43, 21, this->backgroundColor);
  this->FillRegion(44, 0, 280, 21, 0xFFFFFF);
  DrawRectangleBorder(*this, 44, 0, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 114, 0, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 184, 0, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 254, 0, 1, 21, this->borderColor);
}

// OFFSET: 0x00A77F00
// FUNCTION: fn_16884
void CUITime::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A77F10
// FUNCTION: fn_16885
void CUIDelayItem::OnInit() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
}

// OFFSET: 0x00A78180
// FUNCTION: fn_16887
void CUITime::OnInit() {
}

// OFFSET: 0x00A78A10
// FUNCTION: fn_16890
void CUITime::SetTime(int milliseconds) {
  const std::string text = FormatDecimal(static_cast<float>(milliseconds) / 1000.0f, 3, true) + LookupMessage(MSI_SECOND);
  this->text->SetText(text.c_str(), text.size());
}
}

// OFFSET: 0x00A78190
// FUNCTION: fn_16888
int CUISkillDelayInfo::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_REFRESH) {
    return CUIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (this->skillId != 7999) {
    this->SetSkill(this->skillId);
  }
  return 0;
}

// OFFSET: 0x00A781C0
// FUNCTION: fn_16889
void CUISkillDelayInfo::SetSkill(int skillId) {
  this->skillId = skillId;
  this->delayList->data.erase(this->delayList->data.begin(), this->delayList->data.end());
  this->delayList->selected = static_cast<unsigned int>(-1);
  this->delayList->UpdateScrollBar();

  const char* skillDescription = nullptr;
  g_session.m_hotkeyScript->Call("GetSkillDescript", "ddd>s", g_session.m_job, skillId, 0, &skillDescription);
  if (!IsNullOrEmpty(skillDescription)) {
    std::vector<std::string> lines = Split(skillDescription, "\r\n");
    if (!lines.empty()) {
      this->skillNameText->SetText(lines[0].c_str(), lines[0].size());
    }
  }

  int skillFlag = 0;
  std::vector<std::map<int, int>> delays;
  g_session.LoadSkillDelayList(skillId, delays, skillFlag);
  for (unsigned int index = 0; index < delays.size(); ++index) {
    this->delayList->data.push_back(SkillDelayInfo::CDelayItemData{static_cast<int>(index), delays[index]});
  }
  this->delayList->UpdateScrollBar();

  this->description->Clear();
  if (skillFlag & 0x1000) {
    this->description->AddLine(LookupMessage(4329));
  }
  if (skillFlag & 4) {
    if (!this->description->lines.empty()) {
      this->description->AddLine("\n");
    }
    this->description->AddLine(LookupMessage(4330));
  }
  if (skillId == 2461) {
    if (const char* message = LookupMessage(4335)) {
      this->description->AddLine(message);
    }
  }

  this->delayList->Resize(this->delayList->geometry.width, this->delayList->data.size() * this->delayList->rowHeight + this->delayList->itemY);
  if (this->description->lines.empty()) {
    this->description->Show(0);
    this->Resize(this->geometry.width, this->delayList->geometry.y + this->delayList->geometry.height + 10);
  } else {
    this->description->Show(1);
    this->description->Resize(this->description->geometry.width, this->description->contentHeight + this->description->paddingBottom + this->description->paddingTop);
    this->description->Move(this->description->geometry.x, this->delayList->geometry.y + this->delayList->geometry.height + 10);
    this->Resize(this->geometry.width, this->description->geometry.y + this->description->geometry.height + 10);
  }
}

// OFFSET: 0x00A77DD0
// FUNCTION: fn_16883
void CUISkillDelayInfo::Paint() {
  CUIFrameWnd::Paint();
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
  this->FillRegion(11, 28, 323, 23, this->backgroundColor);
  DrawRectangleBorder(*this, 10, 27, 325, 25, this->borderColor);
  DrawRectangleBorder(*this, 10, 58, 325, 46, this->borderColor);
  this->FillRegion(11, 59, 323, 44, this->backgroundColor);
  DrawRectangleBorder(*this, 11, 81, 323, 1, this->borderColor);
  DrawRectangleBorder(*this, 54, 59, 1, 44, this->borderColor);
  DrawRectangleBorder(*this, 194, 59, 1, 44, this->borderColor);
  DrawRectangleBorder(*this, 124, 83, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 264, 83, 1, 21, this->borderColor);
}

// OFFSET: 0x00A77F70
// FUNCTION: fn_16886
void CUISkillDelayInfo::OnInit() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);

  this->titleBar->SetTitle(LookupMessage(4254));
  this->columns[0]->SetText(LookupMessage(4255));
  this->columns[1]->SetText(LookupMessage(4256));
  this->columns[2]->SetText(LookupMessage(4257));
  this->columns[3]->SetText(LookupMessage(4258));
  this->labels[0]->SetText(LookupMessage(4303));
  this->labels[1]->SetText(LookupMessage(4304));
  this->labels[2]->SetText(LookupMessage(4305));

  this->delayList->layoutFlag = 1;
  this->delayList->Invalidate();
  this->description->scrollBar->SetHidden(1);
}

#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A720A0
// FUNCTION: fn_16810
void CUISkillDelayInfo::OnCreate(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(345, 17);
  this->titleBar->Move(0, 0);
  this->titleBar->SetImages(
    std::string(DecryptName<0xF15F8785CDE369BFULL>({
      '\x7F', '\xC6', '\x23', '\x37', '\x45', '\x49', '\x9A', '\x3C', '\x79', '\x8D', '\x23', '\x01', '\x38', '\x3D', '\x03', '\x93', '\xDE', '\x1A', '\x8A', '\xAE', '\xDA', '\xEE', '\x31', '\x85',
      '\xDA', '\x1B', '\x85', '\xAC', '\xE6', '\xE2', '\x03', '\x85', '\xD6', '\x1D', '\x8F', '\xA8', '\xE7', '\xE6', '\x2D', '\xAE', '\xD3', '\x0C', '\x85', '\xB9', '\xAB', '\xE5', '\x32', '\x81',
      '\xBF'})),
    std::string(DecryptName<0x792FFD1325418FB5ULL>({
      '\x75', '\x20', '\x81', '\xDF', '\xD3', '\x33', '\xEA', '\xB4', '\x73', '\x6B', '\x81', '\xE9', '\xAE', '\x47', '\x73', '\x1B', '\xD4', '\xFC', '\x28', '\x46', '\x4C', '\x94', '\x41', '\x0D',
      '\xD0', '\xFD', '\x27', '\x44', '\x70', '\x98', '\x73', '\x0D', '\xDC', '\xFB', '\x2D', '\x40', '\x71', '\x9C', '\x5D', '\x26', '\xD8', '\xE6', '\x25', '\x0B', '\x71', '\x90', '\x5F', '\x79'})),
    std::string(DecryptName<0x49D33529F1556333ULL>({
      '\xF3', '\xCC', '\x95', '\x0B', '\xE9', '\xFB', '\x16', '\x84', '\xF5', '\x87', '\x95', '\x3D', '\x94', '\x8F', '\x8F', '\x2B', '\x52', '\x10', '\x3C', '\x92', '\x76', '\x5C', '\xBD', '\x3D',
      '\x56', '\x11', '\x33', '\x90', '\x4A', '\x50', '\x8F', '\x3D', '\x5A', '\x17', '\x39', '\x94', '\x4B', '\x54', '\xA1', '\x16', '\x41', '\x0A', '\x32', '\x99', '\x5D', '\x1B', '\xB1', '\x24',
      '\x43', '\x63'}))
  );
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText(DecryptName<0x01CB4F1DF9D3F931ULL>({
      '\xF5', '\x4A', '\x6E', '\x43', '\xDB', '\x8C', '\xEB', '\xC0', '\x95', '\x43', '\x6B', '\xF9'}));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont(DecryptName<0xC5B1BBCD9BC9C997ULL>({
      '\xD0', '\xBC', '\xA5', '\xF2', '\xA0', '\xBB'}));
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  CUITitleBar* titleBar = this->titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  menuButton->SetImages(
    std::string(DecryptName<0xC1BB8111EDD183E1ULL>({
      '\x21', '\x2C', '\x11', '\x17', '\xD1', '\x4F', '\x7E', '\x0C', '\x27', '\x67', '\x11', '\x21', '\xAC', '\x3B', '\xE7', '\xA3', '\x80', '\xF0', '\xB8', '\x8E', '\x4E', '\xE8', '\xD5', '\xB5',
      '\x84', '\xF1', '\xB7', '\x8C', '\x72', '\xE4', '\xE7', '\xB2', '\x98', '\xF0', '\x8E', '\x8F', '\x70', '\xF2', '\xDE', '\x9E', '\x8E', '\xE5', '\xB7', '\xC3', '\x73', '\xEC', '\xCB', '\xC1'})),
    std::string(DecryptName<0x1767DDE18D779D8BULL>({
      '\x4B', '\x32', '\xB7', '\x77', '\x21', '\x13', '\xA2', '\xDA', '\x4D', '\x79', '\xB7', '\x41', '\x5C', '\x67', '\x3B', '\x75', '\xEA', '\xEE', '\x1E', '\xEE', '\xBE', '\xB4', '\x09', '\x63',
      '\xEE', '\xEF', '\x11', '\xEC', '\x82', '\xB8', '\x3B', '\x64', '\xF2', '\xEE', '\x28', '\xEF', '\x80', '\xAE', '\x02', '\x48', '\xE4', '\xF3', '\x59', '\xEF', '\x8C', '\xAD', '\x67'})),
    std::string(DecryptName<0x0B23D10F2F49D72BULL>({
      '\xEB', '\x78', '\x89', '\xD5', '\xCF', '\x1F', '\xE6', '\xC6', '\xED', '\x33', '\x89', '\xE3', '\xB2', '\x6B', '\x7F', '\x69', '\x4A', '\xA4', '\x20', '\x4C', '\x50', '\xB8', '\x4D', '\x7F',
      '\x4E', '\xA5', '\x2F', '\x4E', '\x6C', '\xB4', '\x7F', '\x78', '\x52', '\xA4', '\x16', '\x4D', '\x6E', '\xA2', '\x46', '\x54', '\x44', '\xB9', '\x67', '\x4D', '\x62', '\xA1', '\x23'})),
    std::string(DecryptName<0x136F2F9915D1DBCFULL>({
      '\x0F', '\x74', '\x11', '\xEF', '\x59', '\xE1', '\xAA', '\xDE', '\x09', '\x3F', '\x11', '\xD9', '\x24', '\x95', '\x33', '\x71', '\xAE', '\xA8', '\xB8', '\x76', '\xC6', '\x46', '\x01', '\x67',
      '\xAA', '\xA9', '\xB7', '\x74', '\xFA', '\x4A', '\x33', '\x60', '\xB6', '\xA8', '\x8E', '\x77', '\xF8', '\x5C', '\x0A', '\x4C', '\xA0', '\xBD', '\xB7', '\x3B', '\xFB', '\x42', '\x1F', '\x13'}))
  );
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(331, 3);
  closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(WID_SKILLDELAYINFO); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  closeButton->SetImages(
    std::string(DecryptName<0x6BDF334F2BFDAD51ULL>({
      '\x91', '\x02', '\x3D', '\xD1', '\x8F', '\xFD', '\x1A', '\xA6', '\x97', '\x49', '\x3D', '\xE7', '\xF2', '\x89', '\x83', '\x09', '\x30', '\xDE', '\x94', '\x48', '\x10', '\x5A', '\xB1', '\x1F',
      '\x34', '\xDF', '\x9B', '\x4A', '\x2C', '\x56', '\x83', '\x18', '\x28', '\xDE', '\xA2', '\x48', '\x23', '\x5C', '\xAC', '\x0E', '\x0E', '\xC2', '\x9B', '\x4D', '\x61', '\x51', '\xB2', '\x1B',
      '\x51'})),
    std::string(DecryptName<0x674D9F3975573D41ULL>({
      '\x81', '\x92', '\x97', '\x8F', '\xF9', '\x51', '\x88', '\xAA', '\x87', '\xD9', '\x97', '\xB9', '\x84', '\x25', '\x11', '\x05', '\x20', '\x4E', '\x3E', '\x16', '\x66', '\xF6', '\x23', '\x13',
      '\x24', '\x4F', '\x31', '\x14', '\x5A', '\xFA', '\x11', '\x14', '\x38', '\x4E', '\x08', '\x16', '\x55', '\xF0', '\x3E', '\x02', '\x1E', '\x52', '\x39', '\x5B', '\x5B', '\xF2', '\x3D', '\x67'})),
    std::string(DecryptName<0x79E915E9AF6F55DDULL>({
      '\x1D', '\xFA', '\xAF', '\x55', '\x29', '\xDB', '\x2C', '\xB4', '\x1B', '\xB1', '\xAF', '\x63', '\x54', '\xAF', '\xB5', '\x1B', '\xBC', '\x26', '\x06', '\xCC', '\xB6', '\x7C', '\x87', '\x0D',
      '\xB8', '\x27', '\x09', '\xCE', '\x8A', '\x70', '\xB5', '\x0A', '\xA4', '\x26', '\x30', '\xCC', '\x85', '\x7A', '\x9A', '\x1C', '\x82', '\x3A', '\x01', '\x81', '\x8B', '\x78', '\x99', '\x79'})),
    std::string(DecryptName<0xB7AFBD2FD9FD7501ULL>({
      '\xC1', '\xDA', '\x3D', '\x23', '\xEF', '\x73', '\x6A', '\x7A', '\xC7', '\x91', '\x3D', '\x15', '\x92', '\x07', '\xF3', '\xD5', '\x60', '\x06', '\x94', '\xBA', '\x70', '\xD4', '\xC1', '\xC3',
      '\x64', '\x07', '\x9B', '\xB8', '\x4C', '\xD8', '\xF3', '\xC4', '\x78', '\x06', '\xA2', '\xBA', '\x43', '\xD2', '\xDC', '\xD2', '\x5E', '\x1A', '\x9B', '\xBF', '\x01', '\xDF', '\xC2', '\xC7',
      '\x01'}))
  );
  this->titleBar->SetSecondButton(closeButton);

  this->skillNameText = new CUIStaticText();
  this->skillNameText->SetSize(325, 25);
  this->skillNameText->Move(10, 27);
  this->skillNameText->SetText("");
  this->skillNameText->SetFitText(1);
  this->skillNameText->SetAlign(6, 6);
  this->skillNameText->SetFont(DecryptName<0x192D654D29337BE5ULL>({
      '\xA2', '\x0E', '\x5F', '\x40', '\x20', '\x65'}));
  this->skillNameText->SetFontHeight(13);
  this->skillNameText->SetBold(1);
  this->skillNameText->SetColor(0);
  this->skillNameText->SetShadow(0, 0xFFFFFF);
  this->skillNameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->skillNameText);

  this->labels[0] = new CUIStaticText();
  this->labels[0]->SetSize(140, 22);
  this->labels[0]->Move(54, 58);
  this->labels[0]->SetText(DecryptName<0xB3E307A5F753D5E3ULL>({
      '\x27', '\x66', '\xEE', '\x4D', '\x63', '\xC4', '\xE3'}));
  this->labels[0]->SetFitText(0);
  this->labels[0]->SetAlign(6, 6);
  this->labels[0]->SetFont(DecryptName<0xDB27CB557D038759ULL>({
      '\x1E', '\xF2', '\x6F', '\x14', '\x38', '\xCB'}));
  this->labels[0]->SetFontHeight(13);
  this->labels[0]->SetBold(1);
  this->labels[0]->SetColor(0);
  this->labels[0]->SetShadow(0, 0xFFFFFF);
  this->labels[0]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[0]);

  this->labels[1] = new CUIStaticText();
  this->labels[1]->SetSize(140, 22);
  this->labels[1]->Move(194, 58);
  this->labels[1]->SetText(DecryptName<0x3FA3AFA7035DE3FFULL>({
      '\x3B', '\x13', '\x98', '\xBB', '\x67', '\x7C', '\xA3'}));
  this->labels[1]->SetFitText(0);
  this->labels[1]->SetAlign(6, 6);
  this->labels[1]->SetFont(DecryptName<0x87E3D1C50D572DEFULL>({
      '\xA8', '\x58', '\x3B', '\x64', '\xA8', '\xD1'}));
  this->labels[1]->SetFontHeight(13);
  this->labels[1]->SetBold(1);
  this->labels[1]->SetColor(0);
  this->labels[1]->SetShadow(0, 0xFFFFFF);
  this->labels[1]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[1]);

  this->labels[2] = new CUIStaticText();
  this->labels[2]->SetSize(44, 22);
  this->labels[2]->Move(10, 82);
  this->labels[2]->SetText(DecryptName<0x51BB39B763BBBD8DULL>({
      '\x3A', '\x04', '\x01', '\xC4', '\xB7'}));
  this->labels[2]->SetFitText(1);
  this->labels[2]->SetAlign(6, 6);
  this->labels[2]->SetFont(DecryptName<0xADC51BF9C98B413DULL>({
      '\x7A', '\x34', '\xE7', '\xA0', '\x94', '\x1B'}));
  this->labels[2]->SetFontHeight(12);
  this->labels[2]->SetBold(1);
  this->labels[2]->SetColor(0);
  this->labels[2]->SetShadow(0, 0xFFFFFF);
  this->labels[2]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[2]);

  this->columns[0] = new CUIStaticText();
  this->columns[0]->SetSize(70, 22);
  this->columns[0]->Move(54, 82);
  this->columns[0]->SetText(DecryptName<0xDF8117FF49CFF551ULL>({
      '\xE1', '\x18', '\x0E', '\xED', '\xFF'}));
  this->columns[0]->SetFitText(1);
  this->columns[0]->SetAlign(6, 6);
  this->columns[0]->SetFont(DecryptName<0x114901C7E37F6D49ULL>({
      '\x0E', '\x18', '\x13', '\x8A', '\xAA', '\x01'}));
  this->columns[0]->SetFontHeight(12);
  this->columns[0]->SetBold(1);
  this->columns[0]->SetColor(0);
  this->columns[0]->SetShadow(0, 0xFFFFFF);
  this->columns[0]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[0]);

  this->columns[1] = new CUIStaticText();
  this->columns[1]->SetSize(70, 22);
  this->columns[1]->Move(124, 82);
  this->columns[1]->SetText(DecryptName<0x7D5D0F110B6B8371ULL>({
      '\xCB', '\x2C', '\xDE', '\xB4', '\x11'}));
  this->columns[1]->SetFitText(1);
  this->columns[1]->SetAlign(6, 6);
  this->columns[1]->SetFont(DecryptName<0xF715514F5961B75BULL>({
      '\x1C', '\xC2', '\x0D', '\x30', '\x22', '\x51'}));
  this->columns[1]->SetFontHeight(12);
  this->columns[1]->SetBold(1);
  this->columns[1]->SetColor(0);
  this->columns[1]->SetShadow(0, 0xFFFFFF);
  this->columns[1]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[1]);

  this->columns[2] = new CUIStaticText();
  this->columns[2]->SetSize(70, 22);
  this->columns[2]->Move(194, 82);
  this->columns[2]->SetText(DecryptName<0x4191BB5531FD3999ULL>({
      '\x28', '\xE2', '\x4A', '\xFF', '\xEC', '\x41', '\x91'}));
  this->columns[2]->SetFitText(1);
  this->columns[2]->SetAlign(6, 6);
  this->columns[2]->SetFont(DecryptName<0xB19D1FA9116F39D3ULL>({
      '\x94', '\x4C', '\x03', '\x78', '\xC4', '\x1F'}));
  this->columns[2]->SetFontHeight(12);
  this->columns[2]->SetBold(1);
  this->columns[2]->SetColor(0);
  this->columns[2]->SetShadow(0, 0xFFFFFF);
  this->columns[2]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[2]);

  this->columns[3] = new CUIStaticText();
  this->columns[3]->SetSize(70, 22);
  this->columns[3]->Move(264, 82);
  this->columns[3]->SetText(DecryptName<0x9569C9C7E70909AFULL>({
      '\x12', '\xB3', '\xCC', '\x54', '\xC7'}));
  this->columns[3]->SetFitText(1);
  this->columns[3]->SetAlign(6, 6);
  this->columns[3]->SetFont(DecryptName<0xD7A749EB27D5BF6DULL>({
      '\x2A', '\xCA', '\xB9', '\x4E', '\x86', '\x49'}));
  this->columns[3]->SetFontHeight(12);
  this->columns[3]->SetBold(1);
  this->columns[3]->SetColor(0);
  this->columns[3]->SetShadow(0, 0xFFFFFF);
  this->columns[3]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[3]);

  CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* listBox = new CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>();
  this->delayList = listBox;
  listBox->itemWidth = 325;
  listBox->rowHeight = 22;
  listBox->SetSize(325, 220);
  this->delayList->Move(10, 104);
  this->delayList->SetOnSelect([this](SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) { this->OnSelectItem(item, data); });
  this->delayList->SetBinder([this](SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) { this->OnUpdateItem(item, data); });
  this->delayList->SetImages("", "", "", "");
  this->delayList->itemX = 0;
  this->delayList->itemY = 0;
  this->delayList->scrollBar->SetSize(13, 100);
  this->delayList->scrollBar->Move(422, 0);
  this->delayList->scrollBar->step = 10;
  this->delayList->scrollBar->SetButtonImages(
    std::string(DecryptName<0xB195637DAFCB0D59ULL>({
      '\x99', '\xA2', '\x0B', '\x55', '\xBD', '\xAD', '\x50', '\x7C', '\x9F', '\xE9', '\x0B', '\x63', '\xC0', '\xD9', '\xC9', '\xC2', '\x3A', '\x7F', '\xA4', '\xC3', '\x11', '\x53', '\xE0', '\xC1',
      '\x77', '\x6F', '\xA6', '\xDF', '\x7D'})),
    std::string(DecryptName<0xADADEB4DF7813B55ULL>({
      '\x95', '\x94', '\x41', '\x0D', '\x8D', '\x25', '\x68', '\x60', '\x93', '\xDF', '\x41', '\x3B', '\xF0', '\x51', '\xF1', '\xDE', '\x36', '\x49', '\xEE', '\x9B', '\x21', '\xDB', '\xC9', '\xC2',
      '\x22', '\x55', '\xAF', '\x95', '\x20', '\x9B', '\xAD'}))
  );
  this->delayList->scrollBar->SetBackground(std::string(DecryptName<0xD9452D81E5CD99C1ULL>({
      '\x01', '\x36', '\x0D', '\x1F', '\x41', '\xE3', '\x80', '\x14', '\x07', '\x7D', '\x0D', '\x29', '\x3C', '\x97', '\x19', '\xAA', '\xA2', '\xEB', '\xA2', '\x89', '\xED', '\x1D', '\x28', '\xB0',
      '\xA5', '\xB7', '\xAF', '\x88', '\xF1', '\x2D'})));
  this->delayList->scrollBar->SetThumbImages(
    std::string(DecryptName<0xFD69A9B3F311818DULL>({
      '\x4D', '\x2E', '\xD1', '\x09', '\x73', '\x67', '\xAC', '\x30', '\x4B', '\x65', '\xD1', '\x3F', '\x0E', '\x13', '\x35', '\x8E', '\xEE', '\xF3', '\x7E', '\x9F', '\xDF', '\x99', '\x0B', '\x9C',
      '\xFF', '\xDE', '\x64', '\x83', '\x9D', '\xCB', '\x04', '\x8D', '\x8D'})),
    std::string(DecryptName<0x7F2DFFC1ABE91F2BULL>({
      '\xEB', '\xB0', '\x29', '\x51', '\x01', '\x31', '\xE8', '\xB2', '\xED', '\xFB', '\x29', '\x67', '\x7C', '\x45', '\x71', '\x0C', '\x48', '\x6D', '\x86', '\xC7', '\xAD', '\xCF', '\x4F', '\x1E',
      '\x59', '\x40', '\x84', '\xC2', '\xA5', '\xD1', '\x4F', '\x12', '\x5B', '\x1F'})),
    std::string(DecryptName<0xD98F5345098D630DULL>({
      '\xCD', '\xCC', '\x4D', '\xF3', '\x85', '\x9D', '\x4A', '\x14', '\xCB', '\x87', '\x4D', '\xC5', '\xF8', '\xE9', '\xD3', '\xAA', '\x6E', '\x11', '\xE2', '\x65', '\x29', '\x63', '\xED', '\xB8',
      '\x7F', '\x3C', '\xE9', '\x66', '\x32', '\x3D', '\xA1', '\xBB', '\x60', '\x13', '\x8D'}))
  );
  this->delayList->AddNewItem();
  this->AddChild(this->delayList);

  this->description = new CUITextViewer();
  this->description->SetSize(330, 35);
  this->description->Move(11, 334);
  this->description->SetTextOffset(0, 0);
  this->description->SetFont(DecryptName<0x9B0F234DEFDF7593ULL>({
      '\xD4', '\x00', '\xB3', '\x86', '\x20', '\x23'}));
  this->description->SetFontHeight(11);
  this->description->SetBold(false);
  this->description->SetTextColor(0xFF);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(6);
  this->description->SetBackgroundColor(0xFFFFFF);
  this->description->SetPadding(3, 3, 3, 3);
  this->description->SetScrollBarSize(13, 35);
  this->description->SetScrollStep(1);
  this->description->SetScrollBarButtonImages(
    std::string(DecryptName<0xC9712D676B5B3943ULL>({
      '\x83', '\x96', '\x9B', '\x91', '\xA7', '\xE3', '\xB4', '\x04', '\x85', '\xDD', '\x9B', '\xA7', '\xDA', '\x97', '\x2D', '\xBA', '\x20', '\x4B', '\x34', '\x07', '\x0B', '\x1D', '\x04', '\xB9',
      '\x6D', '\x5B', '\x36', '\x1B', '\x67'})),
    std::string(DecryptName<0xED15938D333D2DB9ULL>({
      '\x79', '\x82', '\xFD', '\xC9', '\x4D', '\x5D', '\xD0', '\x20', '\x7F', '\xC9', '\xFD', '\xFF', '\x30', '\x29', '\x49', '\x9E', '\xDA', '\x5F', '\x52', '\x5F', '\xE1', '\xA3', '\x71', '\x82',
      '\xCE', '\x43', '\x13', '\x51', '\xE0', '\xE3', '\x15'}))
  );
  this->description->SetScrollBarBackground(std::string(DecryptName<0xBD2BA1C11B01E73BULL>({
      '\xFB', '\x48', '\xC1', '\xE1', '\x01', '\x6F', '\xEE', '\x70', '\xFD', '\x03', '\xC1', '\xD7', '\x7C', '\x1B', '\x77', '\xCE', '\x58', '\x95', '\x6E', '\x77', '\xAD', '\x91', '\x46', '\xD4',
      '\x5F', '\xC9', '\x63', '\x76', '\xB1', '\xA1'})));
  this->description->SetScrollBarThumbImages(
    std::string(DecryptName<0x4B755DB19953E311ULL>({
      '\xD1', '\x4C', '\x93', '\x63', '\x71', '\x93', '\xB0', '\x86', '\xD7', '\x07', '\x93', '\x55', '\x0C', '\xE7', '\x29', '\x38', '\x72', '\x91', '\x3C', '\xF5', '\xDD', '\x6D', '\x17', '\x2A',
      '\x63', '\xBC', '\x26', '\xE9', '\x9F', '\x3F', '\x18', '\x3B', '\x11'})),
    std::string(DecryptName<0x4331A17F33095FEBULL>({
      '\x2B', '\xF0', '\xC9', '\xC9', '\xBF', '\x6F', '\xF4', '\x8E', '\x2D', '\xBB', '\xC9', '\xFF', '\xC2', '\x1B', '\x6D', '\x30', '\x88', '\x2D', '\x66', '\x5F', '\x13', '\x91', '\x53', '\x22',
      '\x99', '\x00', '\x64', '\x5A', '\x1B', '\x8F', '\x53', '\x2E', '\x9B', '\x5F'})),
    std::string(DecryptName<0xD1056FF94D4D2773ULL>({
      '\xB3', '\x88', '\x8D', '\xB7', '\x39', '\xA1', '\xC0', '\x1C', '\xB5', '\xC3', '\x8D', '\x81', '\x44', '\xD5', '\x59', '\xA2', '\x10', '\x55', '\x22', '\x21', '\x95', '\x5F', '\x67', '\xB0',
      '\x01', '\x78', '\x29', '\x22', '\x8E', '\x01', '\x2B', '\xB3', '\x1E', '\x57', '\x4D'}))
  );
  this->AddChild(this->description);
}
