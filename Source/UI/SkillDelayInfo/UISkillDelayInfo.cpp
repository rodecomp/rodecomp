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
CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
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
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

namespace SkillDelayInfo {

// OFFSET: 0x00A71D60
// FUNCTION: fn_16809
void CUIDelayItem::Slot15(int width, int height) {
  this->level = new CUIStaticText(*this->m_context);
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
    CUITime* time = new CUITime(*this->m_context);
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
  this->text = new CUIStaticText(*this->m_context);
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
CUITime::CUITime(UIWindowContext& context) : CUIControl(context), text(nullptr) {
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
  if (message == 60) {
    if (this->skillId != 7999) {
      this->SetSkill(this->skillId);
    }
    return 0;
  }
  return CUIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x00A781C0
// FUNCTION: fn_16889
void CUISkillDelayInfo::SetSkill(int skillId) {
  this->skillId = skillId;
  this->delayList->data.erase(this->delayList->data.begin(), this->delayList->data.end());
  this->delayList->selected = static_cast<unsigned int>(-1);
  this->delayList->UpdateScrollBar();

  const char* skillDescription = nullptr;
  g_luaScript->Call("GetSkillDescript", "ddd>s", g_session.m_job, skillId, 0, &skillDescription);
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
