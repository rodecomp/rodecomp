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
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <map>
#include <string>
#include <vector>

template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Paint();
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Slot35(int value);
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnScroll(int position);
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::AddNewItem();
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Slot53(int resolveSkin);
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
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
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
void CUISkillDelayInfo::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(345, 379);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
  this->SetSkinColors(0, 0, 0, 0);
}

// OFFSET: 0x00A71B50
// FUNCTION: fn_16808
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

namespace SkillDelayInfo {

// OFFSET: 0x00A71D60
// FUNCTION: fn_16809
void CUIDelayItem::Slot15(int width, int height) {
  this->level = new CUIStaticText();
  this->level->SetSize(44, 21);
  this->level->Move(0, 0);
  this->level->SetText("", 0);
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
void CUITime::Slot15(int width, int height) {
  this->text = new CUIStaticText();
  this->text->SetSize(60, 21);
  this->text->Move(5, 0);
  this->text->SetText("", 0);
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
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
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

  if (this->rowHeight <= this->geometry.height) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count, nullptr);
    for (SkillDelayInfo::CUIDelayItem*& slot : this->items) {
      SkillDelayInfo::CUIDelayItem* item = new SkillDelayInfo::CUIDelayItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (SkillDelayInfo::CUIDelayItem* row : this->items) {
          row->SetHighlighted(listItem == row);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->unknown_00C4) {
          return;
        }
        for (SkillDelayInfo::CUIDelayItem* row : this->items) {
          if (listItem != row) {
            row->SetSelected(false);
          }
        }

        listItem->SetSelected(true);
        if (listItem->index == static_cast<int>(this->selected) && this->unknown_00E0) {
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
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Slot53(int resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
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
  if (list->unknown_00C4) {
    return;
  }
  for (SkillDelayInfo::CUIDelayItem* row : list->items) {
    if (selectedItem != row) {
      row->SetSelected(false);
    }
  }

  selectedItem->SetSelected(true);
  if (selectedItem->index == static_cast<int>(list->selected) && list->unknown_00E0) {
    return;
  }
  SkillDelayInfo::CUIDelayItem* delayItem = dynamic_cast<SkillDelayInfo::CUIDelayItem*>(selectedItem);
  if (!delayItem) {
    return;
  }
  list->selected = delayItem->index;
  if (list->onSelect && delayItem->index >= 0 && static_cast<unsigned int>(delayItem->index) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = delayItem->index; index > 0; --index) {
      ++node;
    }
    list->onSelect(delayItem, *node);
  }
}

// OFFSET: 0x00A77200
// FUNCTION: fn_16872
CUISkillDelayInfo::CUISkillDelayInfo()
    : titleBar(nullptr), unknown_00DC(nullptr), labels{}, columns{}, delayList(nullptr), description(nullptr), backgroundColor(0), borderColor(0), skillId(7999) {
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
  g_windowMgr.CloseWindow(0x272E);
}

// OFFSET: 0x00A77B10
// FUNCTION: fn_16880
void CUISkillDelayInfo::OnSelectItem(SkillDelayInfo::CUIDelayItem*, const SkillDelayInfo::CDelayItemData&) {
}

// OFFSET: 0x00A77B20
// FUNCTION: fn_16881
void CUISkillDelayInfo::OnUpdateItem(SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) {
  const std::string text = std::to_string(data.unknown_0000 + 1);
  item->level->SetText(text.c_str(), text.size());

  for (int index = 0; index < 4; ++index) {
    const auto found = data.unknown_0004.find(index);
    item->times[index]->SetTime(found != data.unknown_0004.end() ? found->second : 0);
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
void CUIDelayItem::Slot16() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
}

// OFFSET: 0x00A78180
// FUNCTION: fn_16887
void CUITime::Slot16() {
}

// OFFSET: 0x00A78A10
// FUNCTION: fn_16890
void CUITime::SetTime(int milliseconds) {
  const std::string text = FormatDecimal(static_cast<float>(milliseconds) / 1000.0f, 3, true) + LookupMessage(1808);
  this->text->SetText(text.c_str(), text.size());
}
}

// OFFSET: 0x00A78190
// FUNCTION: fn_16888
int CUISkillDelayInfo::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 60) {
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
      this->unknown_00DC->SetText(lines[0].c_str(), lines[0].size());
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
void CUISkillDelayInfo::Slot16() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);

  const char* text = LookupMessage(4254);
  this->titleBar->SetTitle(text, std::strlen(text));
  text = LookupMessage(4255);
  this->columns[0]->SetText(text, std::strlen(text));
  text = LookupMessage(4256);
  this->columns[1]->SetText(text, std::strlen(text));
  text = LookupMessage(4257);
  this->columns[2]->SetText(text, std::strlen(text));
  text = LookupMessage(4258);
  this->columns[3]->SetText(text, std::strlen(text));
  text = LookupMessage(4303);
  this->labels[0]->SetText(text, std::strlen(text));
  text = LookupMessage(4304);
  this->labels[1]->SetText(text, std::strlen(text));
  text = LookupMessage(4305);
  this->labels[2]->SetText(text, std::strlen(text));

  this->delayList->unknown_00C4 = 1;
  this->delayList->Invalidate();
  this->description->scrollBar->SetHidden(1);
}

#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A720A0
// FUNCTION: fn_16810
void CUISkillDelayInfo::Slot15(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(345, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    const char titleRightSource[] = {
        '\xF3', '\xCC', '\x95', '\x0B', '\xE9', '\xFB', '\x16', '\x84', '\xF5', '\x87', '\x95', '\x3D', '\x94', '\x8F', '\x8F', '\x2B', '\x52', '\x10', '\x3C', '\x92', '\x76', '\x5C', '\xBD', '\x3D',
        '\x56', '\x11', '\x33', '\x90', '\x4A', '\x50', '\x8F', '\x3D', '\x5A', '\x17', '\x39', '\x94', '\x4B', '\x54', '\xA1', '\x16', '\x41', '\x0A', '\x32', '\x99', '\x5D', '\x1B', '\xB1', '\x24',
        '\x43', '\x63'};
    thread_local EncryptedName<sizeof(titleRightSource), 0x49D33529F1556333ULL> titleRightName(titleRightSource);
    const std::string titleRight(titleRightName.Text());
    const char titleMiddleSource[] = {
        '\x75', '\x20', '\x81', '\xDF', '\xD3', '\x33', '\xEA', '\xB4', '\x73', '\x6B', '\x81', '\xE9', '\xAE', '\x47', '\x73', '\x1B', '\xD4', '\xFC', '\x28', '\x46', '\x4C', '\x94', '\x41', '\x0D',
        '\xD0', '\xFD', '\x27', '\x44', '\x70', '\x98', '\x73', '\x0D', '\xDC', '\xFB', '\x2D', '\x40', '\x71', '\x9C', '\x5D', '\x26', '\xD8', '\xE6', '\x25', '\x0B', '\x71', '\x90', '\x5F', '\x79'};
    thread_local EncryptedName<sizeof(titleMiddleSource), 0x792FFD1325418FB5ULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const char titleLeftSource[] = {
        '\x7F', '\xC6', '\x23', '\x37', '\x45', '\x49', '\x9A', '\x3C', '\x79', '\x8D', '\x23', '\x01', '\x38', '\x3D', '\x03', '\x93', '\xDE', '\x1A', '\x8A', '\xAE', '\xDA', '\xEE', '\x31', '\x85',
        '\xDA', '\x1B', '\x85', '\xAC', '\xE6', '\xE2', '\x03', '\x85', '\xD6', '\x1D', '\x8F', '\xA8', '\xE7', '\xE6', '\x2D', '\xAE', '\xD3', '\x0C', '\x85', '\xB9', '\xAB', '\xE5', '\x32', '\x81',
        '\xBF'};
    thread_local EncryptedName<sizeof(titleLeftSource), 0xF15F8785CDE369BFULL> titleLeftName(titleLeftSource);
    const std::string titleLeft(titleLeftName.Text());
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    const char titleTextSource[] = {
        '\xF5', '\x4A', '\x6E', '\x43', '\xDB', '\x8C', '\xEB', '\xC0', '\x95', '\x43', '\x6B', '\xF9'};
    thread_local EncryptedName<sizeof(titleTextSource), 0x01CB4F1DF9D3F931ULL> titleTextName(titleTextSource);
    const char* titleText = titleTextName.Text();
    title->SetText(titleText, std::strlen(titleText));
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const char titleFontSource[] = {
        '\xD0', '\xBC', '\xA5', '\xF2', '\xA0', '\xBB'};
    thread_local EncryptedName<sizeof(titleFontSource), 0xC5B1BBCD9BC9C997ULL> titleFontName(titleFontSource);
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

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    const char menuDisabledSource[] = {
        '\x0F', '\x74', '\x11', '\xEF', '\x59', '\xE1', '\xAA', '\xDE', '\x09', '\x3F', '\x11', '\xD9', '\x24', '\x95', '\x33', '\x71', '\xAE', '\xA8', '\xB8', '\x76', '\xC6', '\x46', '\x01', '\x67',
        '\xAA', '\xA9', '\xB7', '\x74', '\xFA', '\x4A', '\x33', '\x60', '\xB6', '\xA8', '\x8E', '\x77', '\xF8', '\x5C', '\x0A', '\x4C', '\xA0', '\xBD', '\xB7', '\x3B', '\xFB', '\x42', '\x1F', '\x13'};
    thread_local EncryptedName<sizeof(menuDisabledSource), 0x136F2F9915D1DBCFULL> menuDisabledName(menuDisabledSource);
    const std::string menuDisabled(menuDisabledName.Text());
    const char menuPressedSource[] = {
        '\xEB', '\x78', '\x89', '\xD5', '\xCF', '\x1F', '\xE6', '\xC6', '\xED', '\x33', '\x89', '\xE3', '\xB2', '\x6B', '\x7F', '\x69', '\x4A', '\xA4', '\x20', '\x4C', '\x50', '\xB8', '\x4D', '\x7F',
        '\x4E', '\xA5', '\x2F', '\x4E', '\x6C', '\xB4', '\x7F', '\x78', '\x52', '\xA4', '\x16', '\x4D', '\x6E', '\xA2', '\x46', '\x54', '\x44', '\xB9', '\x67', '\x4D', '\x62', '\xA1', '\x23'};
    thread_local EncryptedName<sizeof(menuPressedSource), 0x0B23D10F2F49D72BULL> menuPressedName(menuPressedSource);
    const std::string menuPressed(menuPressedName.Text());
    const char menuHoverSource[] = {
        '\x4B', '\x32', '\xB7', '\x77', '\x21', '\x13', '\xA2', '\xDA', '\x4D', '\x79', '\xB7', '\x41', '\x5C', '\x67', '\x3B', '\x75', '\xEA', '\xEE', '\x1E', '\xEE', '\xBE', '\xB4', '\x09', '\x63',
        '\xEE', '\xEF', '\x11', '\xEC', '\x82', '\xB8', '\x3B', '\x64', '\xF2', '\xEE', '\x28', '\xEF', '\x80', '\xAE', '\x02', '\x48', '\xE4', '\xF3', '\x59', '\xEF', '\x8C', '\xAD', '\x67'};
    thread_local EncryptedName<sizeof(menuHoverSource), 0x1767DDE18D779D8BULL> menuHoverName(menuHoverSource);
    const std::string menuHover(menuHoverName.Text());
    const char menuNormalSource[] = {
        '\x21', '\x2C', '\x11', '\x17', '\xD1', '\x4F', '\x7E', '\x0C', '\x27', '\x67', '\x11', '\x21', '\xAC', '\x3B', '\xE7', '\xA3', '\x80', '\xF0', '\xB8', '\x8E', '\x4E', '\xE8', '\xD5', '\xB5',
        '\x84', '\xF1', '\xB7', '\x8C', '\x72', '\xE4', '\xE7', '\xB2', '\x98', '\xF0', '\x8E', '\x8F', '\x70', '\xF2', '\xDE', '\x9E', '\x8E', '\xE5', '\xB7', '\xC3', '\x73', '\xEC', '\xCB', '\xC1'};
    thread_local EncryptedName<sizeof(menuNormalSource), 0xC1BB8111EDD183E1ULL> menuNormalName(menuNormalSource);
    const std::string menuNormal(menuNormalName.Text());
    menuButton->SetImages(menuNormal, menuHover, menuPressed, menuDisabled);
  }
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(331, 3);
  closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(10030); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const char closeDisabledSource[] = {
        '\xC1', '\xDA', '\x3D', '\x23', '\xEF', '\x73', '\x6A', '\x7A', '\xC7', '\x91', '\x3D', '\x15', '\x92', '\x07', '\xF3', '\xD5', '\x60', '\x06', '\x94', '\xBA', '\x70', '\xD4', '\xC1', '\xC3',
        '\x64', '\x07', '\x9B', '\xB8', '\x4C', '\xD8', '\xF3', '\xC4', '\x78', '\x06', '\xA2', '\xBA', '\x43', '\xD2', '\xDC', '\xD2', '\x5E', '\x1A', '\x9B', '\xBF', '\x01', '\xDF', '\xC2', '\xC7',
        '\x01'};
    thread_local EncryptedName<sizeof(closeDisabledSource), 0xB7AFBD2FD9FD7501ULL> closeDisabledName(closeDisabledSource);
    const std::string closeDisabled(closeDisabledName.Text());
    const char closePressedSource[] = {
        '\x1D', '\xFA', '\xAF', '\x55', '\x29', '\xDB', '\x2C', '\xB4', '\x1B', '\xB1', '\xAF', '\x63', '\x54', '\xAF', '\xB5', '\x1B', '\xBC', '\x26', '\x06', '\xCC', '\xB6', '\x7C', '\x87', '\x0D',
        '\xB8', '\x27', '\x09', '\xCE', '\x8A', '\x70', '\xB5', '\x0A', '\xA4', '\x26', '\x30', '\xCC', '\x85', '\x7A', '\x9A', '\x1C', '\x82', '\x3A', '\x01', '\x81', '\x8B', '\x78', '\x99', '\x79'};
    thread_local EncryptedName<sizeof(closePressedSource), 0x79E915E9AF6F55DDULL> closePressedName(closePressedSource);
    const std::string closePressed(closePressedName.Text());
    const char closeHoverSource[] = {
        '\x81', '\x92', '\x97', '\x8F', '\xF9', '\x51', '\x88', '\xAA', '\x87', '\xD9', '\x97', '\xB9', '\x84', '\x25', '\x11', '\x05', '\x20', '\x4E', '\x3E', '\x16', '\x66', '\xF6', '\x23', '\x13',
        '\x24', '\x4F', '\x31', '\x14', '\x5A', '\xFA', '\x11', '\x14', '\x38', '\x4E', '\x08', '\x16', '\x55', '\xF0', '\x3E', '\x02', '\x1E', '\x52', '\x39', '\x5B', '\x5B', '\xF2', '\x3D', '\x67'};
    thread_local EncryptedName<sizeof(closeHoverSource), 0x674D9F3975573D41ULL> closeHoverName(closeHoverSource);
    const std::string closeHover(closeHoverName.Text());
    const char closeNormalSource[] = {
        '\x91', '\x02', '\x3D', '\xD1', '\x8F', '\xFD', '\x1A', '\xA6', '\x97', '\x49', '\x3D', '\xE7', '\xF2', '\x89', '\x83', '\x09', '\x30', '\xDE', '\x94', '\x48', '\x10', '\x5A', '\xB1', '\x1F',
        '\x34', '\xDF', '\x9B', '\x4A', '\x2C', '\x56', '\x83', '\x18', '\x28', '\xDE', '\xA2', '\x48', '\x23', '\x5C', '\xAC', '\x0E', '\x0E', '\xC2', '\x9B', '\x4D', '\x61', '\x51', '\xB2', '\x1B',
        '\x51'};
    thread_local EncryptedName<sizeof(closeNormalSource), 0x6BDF334F2BFDAD51ULL> closeNormalName(closeNormalSource);
    const std::string closeNormal(closeNormalName.Text());
    closeButton->SetImages(closeNormal, closeHover, closePressed, closeDisabled);
  }
  this->titleBar->SetSecondButton(closeButton);

  this->unknown_00DC = new CUIStaticText();
  this->unknown_00DC->SetSize(325, 25);
  this->unknown_00DC->Move(10, 27);
  this->unknown_00DC->SetText("", 0);
  this->unknown_00DC->SetFitText(1);
  this->unknown_00DC->SetAlign(6, 6);
  {
    const char noticeFontSource[] = {
        '\xA2', '\x0E', '\x5F', '\x40', '\x20', '\x65'};
    thread_local EncryptedName<sizeof(noticeFontSource), 0x192D654D29337BE5ULL> noticeFontName(noticeFontSource);
    const std::string noticeFont(noticeFontName.Text());
    this->unknown_00DC->SetFont(noticeFont);
  }
  this->unknown_00DC->SetFontHeight(13);
  this->unknown_00DC->SetBold(1);
  this->unknown_00DC->SetColor(0);
  this->unknown_00DC->SetShadow(0, 0xFFFFFF);
  this->unknown_00DC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00DC);

  this->labels[0] = new CUIStaticText();
  this->labels[0]->SetSize(140, 22);
  this->labels[0]->Move(54, 58);
  {
    const char castingTextSource[] = {
        '\x27', '\x66', '\xEE', '\x4D', '\x63', '\xC4', '\xE3'};
    thread_local EncryptedName<sizeof(castingTextSource), 0xB3E307A5F753D5E3ULL> castingTextName(castingTextSource);
    const char* castingText = castingTextName.Text();
    this->labels[0]->SetText(castingText, std::strlen(castingText));
  }
  this->labels[0]->SetFitText(0);
  this->labels[0]->SetAlign(6, 6);
  {
    const char castingFontSource[] = {
        '\x1E', '\xF2', '\x6F', '\x14', '\x38', '\xCB'};
    thread_local EncryptedName<sizeof(castingFontSource), 0xDB27CB557D038759ULL> castingFontName(castingFontSource);
    const std::string castingFont(castingFontName.Text());
    this->labels[0]->SetFont(castingFont);
  }
  this->labels[0]->SetFontHeight(13);
  this->labels[0]->SetBold(1);
  this->labels[0]->SetColor(0);
  this->labels[0]->SetShadow(0, 0xFFFFFF);
  this->labels[0]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[0]);

  this->labels[1] = new CUIStaticText();
  this->labels[1]->SetSize(140, 22);
  this->labels[1]->Move(194, 58);
  {
    const char cooldownTextSource[] = {
        '\x3B', '\x13', '\x98', '\xBB', '\x67', '\x7C', '\xA3'};
    thread_local EncryptedName<sizeof(cooldownTextSource), 0x3FA3AFA7035DE3FFULL> cooldownTextName(cooldownTextSource);
    const char* cooldownText = cooldownTextName.Text();
    this->labels[1]->SetText(cooldownText, std::strlen(cooldownText));
  }
  this->labels[1]->SetFitText(0);
  this->labels[1]->SetAlign(6, 6);
  {
    const char cooldownFontSource[] = {
        '\xA8', '\x58', '\x3B', '\x64', '\xA8', '\xD1'};
    thread_local EncryptedName<sizeof(cooldownFontSource), 0x87E3D1C50D572DEFULL> cooldownFontName(cooldownFontSource);
    const char* cooldownFont = cooldownFontName.Text();
    this->labels[1]->SetFont(std::string(cooldownFont));
  }
  this->labels[1]->SetFontHeight(13);
  this->labels[1]->SetBold(1);
  this->labels[1]->SetColor(0);
  this->labels[1]->SetShadow(0, 0xFFFFFF);
  this->labels[1]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[1]);

  this->labels[2] = new CUIStaticText();
  this->labels[2]->SetSize(44, 22);
  this->labels[2]->Move(10, 82);
  {
    const char levelTextSource[] = {
        '\x3A', '\x04', '\x01', '\xC4', '\xB7'};
    thread_local EncryptedName<sizeof(levelTextSource), 0x51BB39B763BBBD8DULL> levelTextName(levelTextSource);
    const char* levelText = levelTextName.Text();
    this->labels[2]->SetText(levelText, std::strlen(levelText));
  }
  this->labels[2]->SetFitText(1);
  this->labels[2]->SetAlign(6, 6);
  {
    const char levelFontSource[] = {
        '\x7A', '\x34', '\xE7', '\xA0', '\x94', '\x1B'};
    thread_local EncryptedName<sizeof(levelFontSource), 0xADC51BF9C98B413DULL> levelFontName(levelFontSource);
    const char* levelFont = levelFontName.Text();
    this->labels[2]->SetFont(std::string(levelFont));
  }
  this->labels[2]->SetFontHeight(12);
  this->labels[2]->SetBold(1);
  this->labels[2]->SetColor(0);
  this->labels[2]->SetShadow(0, 0xFFFFFF);
  this->labels[2]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[2]);

  this->columns[0] = new CUIStaticText();
  this->columns[0]->SetSize(70, 22);
  this->columns[0]->Move(54, 82);
  {
    const char fixedTextSource[] = {
        '\xE1', '\x18', '\x0E', '\xED', '\xFF'};
    thread_local EncryptedName<sizeof(fixedTextSource), 0xDF8117FF49CFF551ULL> fixedTextName(fixedTextSource);
    const char* fixedText = fixedTextName.Text();
    this->columns[0]->SetText(fixedText, std::strlen(fixedText));
  }
  this->columns[0]->SetFitText(1);
  this->columns[0]->SetAlign(6, 6);
  {
    const char fixedFontSource[] = {
        '\x0E', '\x18', '\x13', '\x8A', '\xAA', '\x01'};
    thread_local EncryptedName<sizeof(fixedFontSource), 0x114901C7E37F6D49ULL> fixedFontName(fixedFontSource);
    const char* fixedFont = fixedFontName.Text();
    this->columns[0]->SetFont(std::string(fixedFont));
  }
  this->columns[0]->SetFontHeight(12);
  this->columns[0]->SetBold(1);
  this->columns[0]->SetColor(0);
  this->columns[0]->SetShadow(0, 0xFFFFFF);
  this->columns[0]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[0]);

  this->columns[1] = new CUIStaticText();
  this->columns[1]->SetSize(70, 22);
  this->columns[1]->Move(124, 82);
  {
    const char variableTextSource[] = {
        '\xCB', '\x2C', '\xDE', '\xB4', '\x11'};
    thread_local EncryptedName<sizeof(variableTextSource), 0x7D5D0F110B6B8371ULL> variableTextName(variableTextSource);
    const char* variableText = variableTextName.Text();
    this->columns[1]->SetText(variableText, std::strlen(variableText));
  }
  this->columns[1]->SetFitText(1);
  this->columns[1]->SetAlign(6, 6);
  {
    const char variableFontSource[] = {
        '\x1C', '\xC2', '\x0D', '\x30', '\x22', '\x51'};
    thread_local EncryptedName<sizeof(variableFontSource), 0xF715514F5961B75BULL> variableFontName(variableFontSource);
    const char* variableFont = variableFontName.Text();
    this->columns[1]->SetFont(std::string(variableFont));
  }
  this->columns[1]->SetFontHeight(12);
  this->columns[1]->SetBold(1);
  this->columns[1]->SetColor(0);
  this->columns[1]->SetShadow(0, 0xFFFFFF);
  this->columns[1]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[1]);

  this->columns[2] = new CUIStaticText();
  this->columns[2]->SetSize(70, 22);
  this->columns[2]->Move(194, 82);
  {
    const char globalTextSource[] = {
        '\x28', '\xE2', '\x4A', '\xFF', '\xEC', '\x41', '\x91'};
    thread_local EncryptedName<sizeof(globalTextSource), 0x4191BB5531FD3999ULL> globalTextName(globalTextSource);
    const char* globalText = globalTextName.Text();
    this->columns[2]->SetText(globalText, std::strlen(globalText));
  }
  this->columns[2]->SetFitText(1);
  this->columns[2]->SetAlign(6, 6);
  {
    const char globalFontSource[] = {
        '\x94', '\x4C', '\x03', '\x78', '\xC4', '\x1F'};
    thread_local EncryptedName<sizeof(globalFontSource), 0xB19D1FA9116F39D3ULL> globalFontName(globalFontSource);
    const char* globalFont = globalFontName.Text();
    this->columns[2]->SetFont(std::string(globalFont));
  }
  this->columns[2]->SetFontHeight(12);
  this->columns[2]->SetBold(1);
  this->columns[2]->SetColor(0);
  this->columns[2]->SetShadow(0, 0xFFFFFF);
  this->columns[2]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[2]);

  this->columns[3] = new CUIStaticText();
  this->columns[3]->SetSize(70, 22);
  this->columns[3]->Move(264, 82);
  {
    const char skillTextSource[] = {
        '\x12', '\xB3', '\xCC', '\x54', '\xC7'};
    thread_local EncryptedName<sizeof(skillTextSource), 0x9569C9C7E70909AFULL> skillTextName(skillTextSource);
    const char* skillText = skillTextName.Text();
    this->columns[3]->SetText(skillText, std::strlen(skillText));
  }
  this->columns[3]->SetFitText(1);
  this->columns[3]->SetAlign(6, 6);
  {
    const char skillFontSource[] = {
        '\x2A', '\xCA', '\xB9', '\x4E', '\x86', '\x49'};
    thread_local EncryptedName<sizeof(skillFontSource), 0xD7A749EB27D5BF6DULL> skillFontName(skillFontSource);
    const char* skillFont = skillFontName.Text();
    this->columns[3]->SetFont(std::string(skillFont));
  }
  this->columns[3]->SetFontHeight(12);
  this->columns[3]->SetBold(1);
  this->columns[3]->SetColor(0);
  this->columns[3]->SetShadow(0, 0xFFFFFF);
  this->columns[3]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[3]);

  this->delayList = new CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>();
  this->delayList->itemWidth = 325;
  this->delayList->rowHeight = 22;
  this->delayList->SetSize(325, 220);
  this->delayList->Move(10, 104);
  this->delayList->SetOnSelect([this](SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) { this->OnSelectItem(item, data); });
  this->delayList->SetBinder([this](SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) { this->OnUpdateItem(item, data); });
  this->delayList->SetImages("", "", "", "");
  this->delayList->itemX = 0;
  this->delayList->itemY = 0;
  this->delayList->scrollBar->SetSize(13, 100);
  this->delayList->scrollBar->Move(422, 0);
  this->delayList->scrollBar->step = 10;
  {
    CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* delayList = this->delayList;
    const char listDownSource[] = {
        '\x95', '\x94', '\x41', '\x0D', '\x8D', '\x25', '\x68', '\x60', '\x93', '\xDF', '\x41', '\x3B', '\xF0', '\x51', '\xF1', '\xDE', '\x36', '\x49', '\xEE', '\x9B', '\x21', '\xDB', '\xC9', '\xC2',
        '\x22', '\x55', '\xAF', '\x95', '\x20', '\x9B', '\xAD'};
    thread_local EncryptedName<sizeof(listDownSource), 0xADADEB4DF7813B55ULL> listDownName(listDownSource);
    const char* listDown = listDownName.Text();
    const std::string down(listDown);
    const char listUpSource[] = {
        '\x99', '\xA2', '\x0B', '\x55', '\xBD', '\xAD', '\x50', '\x7C', '\x9F', '\xE9', '\x0B', '\x63', '\xC0', '\xD9', '\xC9', '\xC2', '\x3A', '\x7F', '\xA4', '\xC3', '\x11', '\x53', '\xE0', '\xC1',
        '\x77', '\x6F', '\xA6', '\xDF', '\x7D'};
    thread_local EncryptedName<sizeof(listUpSource), 0xB195637DAFCB0D59ULL> listUpName(listUpSource);
    const char* listUp = listUpName.Text();
    const std::string up(listUp);
    delayList->scrollBar->SetButtonImages(up, down);
  }
  {
    const char listBackgroundSource[] = {
        '\x01', '\x36', '\x0D', '\x1F', '\x41', '\xE3', '\x80', '\x14', '\x07', '\x7D', '\x0D', '\x29', '\x3C', '\x97', '\x19', '\xAA', '\xA2', '\xEB', '\xA2', '\x89', '\xED', '\x1D', '\x28', '\xB0',
        '\xA5', '\xB7', '\xAF', '\x88', '\xF1', '\x2D'};
    thread_local EncryptedName<sizeof(listBackgroundSource), 0xD9452D81E5CD99C1ULL> listBackgroundName(listBackgroundSource);
    const char* listBackground = listBackgroundName.Text();
    this->delayList->scrollBar->SetBackground(std::string(listBackground));
  }
  {
    CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* delayList = this->delayList;
    const char listThumbBottomSource[] = {
        '\xCD', '\xCC', '\x4D', '\xF3', '\x85', '\x9D', '\x4A', '\x14', '\xCB', '\x87', '\x4D', '\xC5', '\xF8', '\xE9', '\xD3', '\xAA', '\x6E', '\x11', '\xE2', '\x65', '\x29', '\x63', '\xED', '\xB8',
        '\x7F', '\x3C', '\xE9', '\x66', '\x32', '\x3D', '\xA1', '\xBB', '\x60', '\x13', '\x8D'};
    thread_local EncryptedName<sizeof(listThumbBottomSource), 0xD98F5345098D630DULL> listThumbBottomName(listThumbBottomSource);
    const char* listThumbBottom = listThumbBottomName.Text();
    const std::string bottom(listThumbBottom);
    const char listThumbMiddleSource[] = {
        '\xEB', '\xB0', '\x29', '\x51', '\x01', '\x31', '\xE8', '\xB2', '\xED', '\xFB', '\x29', '\x67', '\x7C', '\x45', '\x71', '\x0C', '\x48', '\x6D', '\x86', '\xC7', '\xAD', '\xCF', '\x4F', '\x1E',
        '\x59', '\x40', '\x84', '\xC2', '\xA5', '\xD1', '\x4F', '\x12', '\x5B', '\x1F'};
    thread_local EncryptedName<sizeof(listThumbMiddleSource), 0x7F2DFFC1ABE91F2BULL> listThumbMiddleName(listThumbMiddleSource);
    const char* listThumbMiddle = listThumbMiddleName.Text();
    const std::string middle(listThumbMiddle);
    const char listThumbTopSource[] = {
        '\x4D', '\x2E', '\xD1', '\x09', '\x73', '\x67', '\xAC', '\x30', '\x4B', '\x65', '\xD1', '\x3F', '\x0E', '\x13', '\x35', '\x8E', '\xEE', '\xF3', '\x7E', '\x9F', '\xDF', '\x99', '\x0B', '\x9C',
        '\xFF', '\xDE', '\x64', '\x83', '\x9D', '\xCB', '\x04', '\x8D', '\x8D'};
    thread_local EncryptedName<sizeof(listThumbTopSource), 0xFD69A9B3F311818DULL> listThumbTopName(listThumbTopSource);
    const char* listThumbTop = listThumbTopName.Text();
    const std::string top(listThumbTop);
    delayList->scrollBar->SetThumbImages(top, middle, bottom);
  }
  this->delayList->AddNewItem();
  this->AddChild(this->delayList);

  this->description = new CUITextViewer();
  this->description->SetSize(330, 35);
  this->description->Move(11, 334);
  this->description->SetTextOffset(0, 0);
  {
    const char descriptionFontSource[] = {
        '\xD4', '\x00', '\xB3', '\x86', '\x20', '\x23'};
    thread_local EncryptedName<sizeof(descriptionFontSource), 0x9B0F234DEFDF7593ULL> descriptionFontName(descriptionFontSource);
    const char* descriptionFont = descriptionFontName.Text();
    this->description->SetFont(std::string(descriptionFont));
  }
  this->description->SetFontHeight(11);
  this->description->SetBold(false);
  this->description->SetTextColor(0xFF);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(6);
  this->description->SetBackgroundColor(0xFFFFFF);
  this->description->SetPadding(3, 3, 3, 3);
  this->description->SetScrollBarSize(13, 35);
  this->description->SetScrollStep(1);
  {
    const char descriptionDownSource[] = {
        '\x79', '\x82', '\xFD', '\xC9', '\x4D', '\x5D', '\xD0', '\x20', '\x7F', '\xC9', '\xFD', '\xFF', '\x30', '\x29', '\x49', '\x9E', '\xDA', '\x5F', '\x52', '\x5F', '\xE1', '\xA3', '\x71', '\x82',
        '\xCE', '\x43', '\x13', '\x51', '\xE0', '\xE3', '\x15'};
    thread_local EncryptedName<sizeof(descriptionDownSource), 0xED15938D333D2DB9ULL> descriptionDownName(descriptionDownSource);
    const char* descriptionDown = descriptionDownName.Text();
    const std::string down(descriptionDown);
    const char descriptionUpSource[] = {
        '\x83', '\x96', '\x9B', '\x91', '\xA7', '\xE3', '\xB4', '\x04', '\x85', '\xDD', '\x9B', '\xA7', '\xDA', '\x97', '\x2D', '\xBA', '\x20', '\x4B', '\x34', '\x07', '\x0B', '\x1D', '\x04', '\xB9',
        '\x6D', '\x5B', '\x36', '\x1B', '\x67'};
    thread_local EncryptedName<sizeof(descriptionUpSource), 0xC9712D676B5B3943ULL> descriptionUpName(descriptionUpSource);
    const char* descriptionUp = descriptionUpName.Text();
    const std::string up(descriptionUp);
    this->description->SetScrollBarButtonImages(up, down);
  }
  {
    const char descriptionBackgroundSource[] = {
        '\xFB', '\x48', '\xC1', '\xE1', '\x01', '\x6F', '\xEE', '\x70', '\xFD', '\x03', '\xC1', '\xD7', '\x7C', '\x1B', '\x77', '\xCE', '\x58', '\x95', '\x6E', '\x77', '\xAD', '\x91', '\x46', '\xD4',
        '\x5F', '\xC9', '\x63', '\x76', '\xB1', '\xA1'};
    thread_local EncryptedName<sizeof(descriptionBackgroundSource), 0xBD2BA1C11B01E73BULL> descriptionBackgroundName(descriptionBackgroundSource);
    const char* descriptionBackground = descriptionBackgroundName.Text();
    this->description->SetScrollBarBackground(std::string(descriptionBackground));
  }
  {
    const char descriptionThumbBottomSource[] = {
        '\xB3', '\x88', '\x8D', '\xB7', '\x39', '\xA1', '\xC0', '\x1C', '\xB5', '\xC3', '\x8D', '\x81', '\x44', '\xD5', '\x59', '\xA2', '\x10', '\x55', '\x22', '\x21', '\x95', '\x5F', '\x67', '\xB0',
        '\x01', '\x78', '\x29', '\x22', '\x8E', '\x01', '\x2B', '\xB3', '\x1E', '\x57', '\x4D'};
    thread_local EncryptedName<sizeof(descriptionThumbBottomSource), 0xD1056FF94D4D2773ULL> descriptionThumbBottomName(descriptionThumbBottomSource);
    const char* descriptionThumbBottom = descriptionThumbBottomName.Text();
    const std::string bottom(descriptionThumbBottom);
    const char descriptionThumbMiddleSource[] = {
        '\x2B', '\xF0', '\xC9', '\xC9', '\xBF', '\x6F', '\xF4', '\x8E', '\x2D', '\xBB', '\xC9', '\xFF', '\xC2', '\x1B', '\x6D', '\x30', '\x88', '\x2D', '\x66', '\x5F', '\x13', '\x91', '\x53', '\x22',
        '\x99', '\x00', '\x64', '\x5A', '\x1B', '\x8F', '\x53', '\x2E', '\x9B', '\x5F'};
    thread_local EncryptedName<sizeof(descriptionThumbMiddleSource), 0x4331A17F33095FEBULL> descriptionThumbMiddleName(descriptionThumbMiddleSource);
    const char* descriptionThumbMiddle = descriptionThumbMiddleName.Text();
    const std::string middle(descriptionThumbMiddle);
    const char descriptionThumbTopSource[] = {
        '\xD1', '\x4C', '\x93', '\x63', '\x71', '\x93', '\xB0', '\x86', '\xD7', '\x07', '\x93', '\x55', '\x0C', '\xE7', '\x29', '\x38', '\x72', '\x91', '\x3C', '\xF5', '\xDD', '\x6D', '\x17', '\x2A',
        '\x63', '\xBC', '\x26', '\xE9', '\x9F', '\x3F', '\x18', '\x3B', '\x11'};
    thread_local EncryptedName<sizeof(descriptionThumbTopSource), 0x4B755DB19953E311ULL> descriptionThumbTopName(descriptionThumbTopSource);
    const char* descriptionThumbTop = descriptionThumbTopName.Text();
    const std::string top(descriptionThumbTop);
    this->description->SetScrollBarThumbImages(top, middle, bottom);
  }
  this->AddChild(this->description);
}
