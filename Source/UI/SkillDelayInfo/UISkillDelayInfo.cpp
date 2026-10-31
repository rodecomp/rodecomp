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

#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A720A0
// FUNCTION: fn_16810
void CUISkillDelayInfo::Slot15(int width, int height) {
  this->titleBar = new CUITitleBar(*this->m_context);
  this->titleBar->SetSize(345, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    const std::uint8_t titleRightSource[] = {
        0xF3, 0xCC, 0x95, 0x0B, 0xE9, 0xFB, 0x16, 0x84, 0xF5, 0x87, 0x95, 0x3D, 0x94, 0x8F, 0x8F, 0x2B, 0x52, 0x10, 0x3C, 0x92, 0x76, 0x5C, 0xBD, 0x3D,
        0x56, 0x11, 0x33, 0x90, 0x4A, 0x50, 0x8F, 0x3D, 0x5A, 0x17, 0x39, 0x94, 0x4B, 0x54, 0xA1, 0x16, 0x41, 0x0A, 0x32, 0x99, 0x5D, 0x1B, 0xB1, 0x24,
        0x43, 0x63};
    thread_local EncryptedName<sizeof(titleRightSource), 0x49D33529F1556333ULL> titleRightName(titleRightSource);
    const std::string titleRight(titleRightName.Text());
    const std::uint8_t titleMiddleSource[] = {
        0x75, 0x20, 0x81, 0xDF, 0xD3, 0x33, 0xEA, 0xB4, 0x73, 0x6B, 0x81, 0xE9, 0xAE, 0x47, 0x73, 0x1B, 0xD4, 0xFC, 0x28, 0x46, 0x4C, 0x94, 0x41, 0x0D,
        0xD0, 0xFD, 0x27, 0x44, 0x70, 0x98, 0x73, 0x0D, 0xDC, 0xFB, 0x2D, 0x40, 0x71, 0x9C, 0x5D, 0x26, 0xD8, 0xE6, 0x25, 0x0B, 0x71, 0x90, 0x5F, 0x79};
    thread_local EncryptedName<sizeof(titleMiddleSource), 0x792FFD1325418FB5ULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const std::uint8_t titleLeftSource[] = {
        0x7F, 0xC6, 0x23, 0x37, 0x45, 0x49, 0x9A, 0x3C, 0x79, 0x8D, 0x23, 0x01, 0x38, 0x3D, 0x03, 0x93, 0xDE, 0x1A, 0x8A, 0xAE, 0xDA, 0xEE, 0x31, 0x85,
        0xDA, 0x1B, 0x85, 0xAC, 0xE6, 0xE2, 0x03, 0x85, 0xD6, 0x1D, 0x8F, 0xA8, 0xE7, 0xE6, 0x2D, 0xAE, 0xD3, 0x0C, 0x85, 0xB9, 0xAB, 0xE5, 0x32, 0x81,
        0xBF};
    thread_local EncryptedName<sizeof(titleLeftSource), 0xF15F8785CDE369BFULL> titleLeftName(titleLeftSource);
    const std::string titleLeft(titleLeftName.Text());
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    const std::uint8_t titleTextSource[] = {
        0xF5, 0x4A, 0x6E, 0x43, 0xDB, 0x8C, 0xEB, 0xC0, 0x95, 0x43, 0x6B, 0xF9};
    thread_local EncryptedName<sizeof(titleTextSource), 0x01CB4F1DF9D3F931ULL> titleTextName(titleTextSource);
    const char* titleText = titleTextName.Text();
    title->SetText(titleText, std::strlen(titleText));
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const std::uint8_t titleFontSource[] = {
        0xD0, 0xBC, 0xA5, 0xF2, 0xA0, 0xBB};
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

  CUIImageButton* menuButton = new CUIImageButton(*this->m_context);
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    const std::uint8_t menuDisabledSource[] = {
        0x0F, 0x74, 0x11, 0xEF, 0x59, 0xE1, 0xAA, 0xDE, 0x09, 0x3F, 0x11, 0xD9, 0x24, 0x95, 0x33, 0x71, 0xAE, 0xA8, 0xB8, 0x76, 0xC6, 0x46, 0x01, 0x67,
        0xAA, 0xA9, 0xB7, 0x74, 0xFA, 0x4A, 0x33, 0x60, 0xB6, 0xA8, 0x8E, 0x77, 0xF8, 0x5C, 0x0A, 0x4C, 0xA0, 0xBD, 0xB7, 0x3B, 0xFB, 0x42, 0x1F, 0x13};
    thread_local EncryptedName<sizeof(menuDisabledSource), 0x136F2F9915D1DBCFULL> menuDisabledName(menuDisabledSource);
    const std::string menuDisabled(menuDisabledName.Text());
    const std::uint8_t menuPressedSource[] = {
        0xEB, 0x78, 0x89, 0xD5, 0xCF, 0x1F, 0xE6, 0xC6, 0xED, 0x33, 0x89, 0xE3, 0xB2, 0x6B, 0x7F, 0x69, 0x4A, 0xA4, 0x20, 0x4C, 0x50, 0xB8, 0x4D, 0x7F,
        0x4E, 0xA5, 0x2F, 0x4E, 0x6C, 0xB4, 0x7F, 0x78, 0x52, 0xA4, 0x16, 0x4D, 0x6E, 0xA2, 0x46, 0x54, 0x44, 0xB9, 0x67, 0x4D, 0x62, 0xA1, 0x23};
    thread_local EncryptedName<sizeof(menuPressedSource), 0x0B23D10F2F49D72BULL> menuPressedName(menuPressedSource);
    const std::string menuPressed(menuPressedName.Text());
    const std::uint8_t menuHoverSource[] = {
        0x4B, 0x32, 0xB7, 0x77, 0x21, 0x13, 0xA2, 0xDA, 0x4D, 0x79, 0xB7, 0x41, 0x5C, 0x67, 0x3B, 0x75, 0xEA, 0xEE, 0x1E, 0xEE, 0xBE, 0xB4, 0x09, 0x63,
        0xEE, 0xEF, 0x11, 0xEC, 0x82, 0xB8, 0x3B, 0x64, 0xF2, 0xEE, 0x28, 0xEF, 0x80, 0xAE, 0x02, 0x48, 0xE4, 0xF3, 0x59, 0xEF, 0x8C, 0xAD, 0x67};
    thread_local EncryptedName<sizeof(menuHoverSource), 0x1767DDE18D779D8BULL> menuHoverName(menuHoverSource);
    const std::string menuHover(menuHoverName.Text());
    const std::uint8_t menuNormalSource[] = {
        0x21, 0x2C, 0x11, 0x17, 0xD1, 0x4F, 0x7E, 0x0C, 0x27, 0x67, 0x11, 0x21, 0xAC, 0x3B, 0xE7, 0xA3, 0x80, 0xF0, 0xB8, 0x8E, 0x4E, 0xE8, 0xD5, 0xB5,
        0x84, 0xF1, 0xB7, 0x8C, 0x72, 0xE4, 0xE7, 0xB2, 0x98, 0xF0, 0x8E, 0x8F, 0x70, 0xF2, 0xDE, 0x9E, 0x8E, 0xE5, 0xB7, 0xC3, 0x73, 0xEC, 0xCB, 0xC1};
    thread_local EncryptedName<sizeof(menuNormalSource), 0xC1BB8111EDD183E1ULL> menuNormalName(menuNormalSource);
    const std::string menuNormal(menuNormalName.Text());
    menuButton->SetImages(menuNormal, menuHover, menuPressed, menuDisabled);
  }
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton(*this->m_context);
  closeButton->SetSize(11, 11);
  closeButton->Move(331, 3);
  closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(10030); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const std::uint8_t closeDisabledSource[] = {
        0xC1, 0xDA, 0x3D, 0x23, 0xEF, 0x73, 0x6A, 0x7A, 0xC7, 0x91, 0x3D, 0x15, 0x92, 0x07, 0xF3, 0xD5, 0x60, 0x06, 0x94, 0xBA, 0x70, 0xD4, 0xC1, 0xC3,
        0x64, 0x07, 0x9B, 0xB8, 0x4C, 0xD8, 0xF3, 0xC4, 0x78, 0x06, 0xA2, 0xBA, 0x43, 0xD2, 0xDC, 0xD2, 0x5E, 0x1A, 0x9B, 0xBF, 0x01, 0xDF, 0xC2, 0xC7,
        0x01};
    thread_local EncryptedName<sizeof(closeDisabledSource), 0xB7AFBD2FD9FD7501ULL> closeDisabledName(closeDisabledSource);
    const std::string closeDisabled(closeDisabledName.Text());
    const std::uint8_t closePressedSource[] = {
        0x1D, 0xFA, 0xAF, 0x55, 0x29, 0xDB, 0x2C, 0xB4, 0x1B, 0xB1, 0xAF, 0x63, 0x54, 0xAF, 0xB5, 0x1B, 0xBC, 0x26, 0x06, 0xCC, 0xB6, 0x7C, 0x87, 0x0D,
        0xB8, 0x27, 0x09, 0xCE, 0x8A, 0x70, 0xB5, 0x0A, 0xA4, 0x26, 0x30, 0xCC, 0x85, 0x7A, 0x9A, 0x1C, 0x82, 0x3A, 0x01, 0x81, 0x8B, 0x78, 0x99, 0x79};
    thread_local EncryptedName<sizeof(closePressedSource), 0x79E915E9AF6F55DDULL> closePressedName(closePressedSource);
    const std::string closePressed(closePressedName.Text());
    const std::uint8_t closeHoverSource[] = {
        0x81, 0x92, 0x97, 0x8F, 0xF9, 0x51, 0x88, 0xAA, 0x87, 0xD9, 0x97, 0xB9, 0x84, 0x25, 0x11, 0x05, 0x20, 0x4E, 0x3E, 0x16, 0x66, 0xF6, 0x23, 0x13,
        0x24, 0x4F, 0x31, 0x14, 0x5A, 0xFA, 0x11, 0x14, 0x38, 0x4E, 0x08, 0x16, 0x55, 0xF0, 0x3E, 0x02, 0x1E, 0x52, 0x39, 0x5B, 0x5B, 0xF2, 0x3D, 0x67};
    thread_local EncryptedName<sizeof(closeHoverSource), 0x674D9F3975573D41ULL> closeHoverName(closeHoverSource);
    const std::string closeHover(closeHoverName.Text());
    const std::uint8_t closeNormalSource[] = {
        0x91, 0x02, 0x3D, 0xD1, 0x8F, 0xFD, 0x1A, 0xA6, 0x97, 0x49, 0x3D, 0xE7, 0xF2, 0x89, 0x83, 0x09, 0x30, 0xDE, 0x94, 0x48, 0x10, 0x5A, 0xB1, 0x1F,
        0x34, 0xDF, 0x9B, 0x4A, 0x2C, 0x56, 0x83, 0x18, 0x28, 0xDE, 0xA2, 0x48, 0x23, 0x5C, 0xAC, 0x0E, 0x0E, 0xC2, 0x9B, 0x4D, 0x61, 0x51, 0xB2, 0x1B,
        0x51};
    thread_local EncryptedName<sizeof(closeNormalSource), 0x6BDF334F2BFDAD51ULL> closeNormalName(closeNormalSource);
    const std::string closeNormal(closeNormalName.Text());
    closeButton->SetImages(closeNormal, closeHover, closePressed, closeDisabled);
  }
  this->titleBar->SetSecondButton(closeButton);

  this->unknown_00DC = new CUIStaticText(*this->m_context);
  this->unknown_00DC->SetSize(325, 25);
  this->unknown_00DC->Move(10, 27);
  this->unknown_00DC->SetText("", 0);
  this->unknown_00DC->SetFitText(1);
  this->unknown_00DC->SetAlign(6, 6);
  {
    const std::uint8_t noticeFontSource[] = {
        0xA2, 0x0E, 0x5F, 0x40, 0x20, 0x65};
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

  this->labels[0] = new CUIStaticText(*this->m_context);
  this->labels[0]->SetSize(140, 22);
  this->labels[0]->Move(54, 58);
  {
    const std::uint8_t castingTextSource[] = {
        0x27, 0x66, 0xEE, 0x4D, 0x63, 0xC4, 0xE3};
    thread_local EncryptedName<sizeof(castingTextSource), 0xB3E307A5F753D5E3ULL> castingTextName(castingTextSource);
    const char* castingText = castingTextName.Text();
    this->labels[0]->SetText(castingText, std::strlen(castingText));
  }
  this->labels[0]->SetFitText(0);
  this->labels[0]->SetAlign(6, 6);
  {
    const std::uint8_t castingFontSource[] = {
        0x1E, 0xF2, 0x6F, 0x14, 0x38, 0xCB};
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

  this->labels[1] = new CUIStaticText(*this->m_context);
  this->labels[1]->SetSize(140, 22);
  this->labels[1]->Move(194, 58);
  {
    const std::uint8_t cooldownTextSource[] = {
        0x3B, 0x13, 0x98, 0xBB, 0x67, 0x7C, 0xA3};
    thread_local EncryptedName<sizeof(cooldownTextSource), 0x3FA3AFA7035DE3FFULL> cooldownTextName(cooldownTextSource);
    const char* cooldownText = cooldownTextName.Text();
    this->labels[1]->SetText(cooldownText, std::strlen(cooldownText));
  }
  this->labels[1]->SetFitText(0);
  this->labels[1]->SetAlign(6, 6);
  {
    const std::uint8_t cooldownFontSource[] = {
        0xA8, 0x58, 0x3B, 0x64, 0xA8, 0xD1};
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

  this->labels[2] = new CUIStaticText(*this->m_context);
  this->labels[2]->SetSize(44, 22);
  this->labels[2]->Move(10, 82);
  {
    const std::uint8_t levelTextSource[] = {
        0x3A, 0x04, 0x01, 0xC4, 0xB7};
    thread_local EncryptedName<sizeof(levelTextSource), 0x51BB39B763BBBD8DULL> levelTextName(levelTextSource);
    const char* levelText = levelTextName.Text();
    this->labels[2]->SetText(levelText, std::strlen(levelText));
  }
  this->labels[2]->SetFitText(1);
  this->labels[2]->SetAlign(6, 6);
  {
    const std::uint8_t levelFontSource[] = {
        0x7A, 0x34, 0xE7, 0xA0, 0x94, 0x1B};
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

  this->columns[0] = new CUIStaticText(*this->m_context);
  this->columns[0]->SetSize(70, 22);
  this->columns[0]->Move(54, 82);
  {
    const std::uint8_t fixedTextSource[] = {
        0xE1, 0x18, 0x0E, 0xED, 0xFF};
    thread_local EncryptedName<sizeof(fixedTextSource), 0xDF8117FF49CFF551ULL> fixedTextName(fixedTextSource);
    const char* fixedText = fixedTextName.Text();
    this->columns[0]->SetText(fixedText, std::strlen(fixedText));
  }
  this->columns[0]->SetFitText(1);
  this->columns[0]->SetAlign(6, 6);
  {
    const std::uint8_t fixedFontSource[] = {
        0x0E, 0x18, 0x13, 0x8A, 0xAA, 0x01};
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

  this->columns[1] = new CUIStaticText(*this->m_context);
  this->columns[1]->SetSize(70, 22);
  this->columns[1]->Move(124, 82);
  {
    const std::uint8_t variableTextSource[] = {
        0xCB, 0x2C, 0xDE, 0xB4, 0x11};
    thread_local EncryptedName<sizeof(variableTextSource), 0x7D5D0F110B6B8371ULL> variableTextName(variableTextSource);
    const char* variableText = variableTextName.Text();
    this->columns[1]->SetText(variableText, std::strlen(variableText));
  }
  this->columns[1]->SetFitText(1);
  this->columns[1]->SetAlign(6, 6);
  {
    const std::uint8_t variableFontSource[] = {
        0x1C, 0xC2, 0x0D, 0x30, 0x22, 0x51};
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

  this->columns[2] = new CUIStaticText(*this->m_context);
  this->columns[2]->SetSize(70, 22);
  this->columns[2]->Move(194, 82);
  {
    const std::uint8_t globalTextSource[] = {
        0x28, 0xE2, 0x4A, 0xFF, 0xEC, 0x41, 0x91};
    thread_local EncryptedName<sizeof(globalTextSource), 0x4191BB5531FD3999ULL> globalTextName(globalTextSource);
    const char* globalText = globalTextName.Text();
    this->columns[2]->SetText(globalText, std::strlen(globalText));
  }
  this->columns[2]->SetFitText(1);
  this->columns[2]->SetAlign(6, 6);
  {
    const std::uint8_t globalFontSource[] = {
        0x94, 0x4C, 0x03, 0x78, 0xC4, 0x1F};
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

  this->columns[3] = new CUIStaticText(*this->m_context);
  this->columns[3]->SetSize(70, 22);
  this->columns[3]->Move(264, 82);
  {
    const std::uint8_t skillTextSource[] = {
        0x12, 0xB3, 0xCC, 0x54, 0xC7};
    thread_local EncryptedName<sizeof(skillTextSource), 0x9569C9C7E70909AFULL> skillTextName(skillTextSource);
    const char* skillText = skillTextName.Text();
    this->columns[3]->SetText(skillText, std::strlen(skillText));
  }
  this->columns[3]->SetFitText(1);
  this->columns[3]->SetAlign(6, 6);
  {
    const std::uint8_t skillFontSource[] = {
        0x2A, 0xCA, 0xB9, 0x4E, 0x86, 0x49};
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

  this->delayList = new CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>(*this->m_context);
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
    const std::uint8_t listDownSource[] = {
        0x95, 0x94, 0x41, 0x0D, 0x8D, 0x25, 0x68, 0x60, 0x93, 0xDF, 0x41, 0x3B, 0xF0, 0x51, 0xF1, 0xDE, 0x36, 0x49, 0xEE, 0x9B, 0x21, 0xDB, 0xC9, 0xC2,
        0x22, 0x55, 0xAF, 0x95, 0x20, 0x9B, 0xAD};
    thread_local EncryptedName<sizeof(listDownSource), 0xADADEB4DF7813B55ULL> listDownName(listDownSource);
    const char* listDown = listDownName.Text();
    const std::string down(listDown);
    const std::uint8_t listUpSource[] = {
        0x99, 0xA2, 0x0B, 0x55, 0xBD, 0xAD, 0x50, 0x7C, 0x9F, 0xE9, 0x0B, 0x63, 0xC0, 0xD9, 0xC9, 0xC2, 0x3A, 0x7F, 0xA4, 0xC3, 0x11, 0x53, 0xE0, 0xC1,
        0x77, 0x6F, 0xA6, 0xDF, 0x7D};
    thread_local EncryptedName<sizeof(listUpSource), 0xB195637DAFCB0D59ULL> listUpName(listUpSource);
    const char* listUp = listUpName.Text();
    const std::string up(listUp);
    delayList->scrollBar->SetButtonImages(up, down);
  }
  {
    const std::uint8_t listBackgroundSource[] = {
        0x01, 0x36, 0x0D, 0x1F, 0x41, 0xE3, 0x80, 0x14, 0x07, 0x7D, 0x0D, 0x29, 0x3C, 0x97, 0x19, 0xAA, 0xA2, 0xEB, 0xA2, 0x89, 0xED, 0x1D, 0x28, 0xB0,
        0xA5, 0xB7, 0xAF, 0x88, 0xF1, 0x2D};
    thread_local EncryptedName<sizeof(listBackgroundSource), 0xD9452D81E5CD99C1ULL> listBackgroundName(listBackgroundSource);
    const char* listBackground = listBackgroundName.Text();
    this->delayList->scrollBar->SetBackground(std::string(listBackground));
  }
  {
    CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* delayList = this->delayList;
    const std::uint8_t listThumbBottomSource[] = {
        0xCD, 0xCC, 0x4D, 0xF3, 0x85, 0x9D, 0x4A, 0x14, 0xCB, 0x87, 0x4D, 0xC5, 0xF8, 0xE9, 0xD3, 0xAA, 0x6E, 0x11, 0xE2, 0x65, 0x29, 0x63, 0xED, 0xB8,
        0x7F, 0x3C, 0xE9, 0x66, 0x32, 0x3D, 0xA1, 0xBB, 0x60, 0x13, 0x8D};
    thread_local EncryptedName<sizeof(listThumbBottomSource), 0xD98F5345098D630DULL> listThumbBottomName(listThumbBottomSource);
    const char* listThumbBottom = listThumbBottomName.Text();
    const std::string bottom(listThumbBottom);
    const std::uint8_t listThumbMiddleSource[] = {
        0xEB, 0xB0, 0x29, 0x51, 0x01, 0x31, 0xE8, 0xB2, 0xED, 0xFB, 0x29, 0x67, 0x7C, 0x45, 0x71, 0x0C, 0x48, 0x6D, 0x86, 0xC7, 0xAD, 0xCF, 0x4F, 0x1E,
        0x59, 0x40, 0x84, 0xC2, 0xA5, 0xD1, 0x4F, 0x12, 0x5B, 0x1F};
    thread_local EncryptedName<sizeof(listThumbMiddleSource), 0x7F2DFFC1ABE91F2BULL> listThumbMiddleName(listThumbMiddleSource);
    const char* listThumbMiddle = listThumbMiddleName.Text();
    const std::string middle(listThumbMiddle);
    const std::uint8_t listThumbTopSource[] = {
        0x4D, 0x2E, 0xD1, 0x09, 0x73, 0x67, 0xAC, 0x30, 0x4B, 0x65, 0xD1, 0x3F, 0x0E, 0x13, 0x35, 0x8E, 0xEE, 0xF3, 0x7E, 0x9F, 0xDF, 0x99, 0x0B, 0x9C,
        0xFF, 0xDE, 0x64, 0x83, 0x9D, 0xCB, 0x04, 0x8D, 0x8D};
    thread_local EncryptedName<sizeof(listThumbTopSource), 0xFD69A9B3F311818DULL> listThumbTopName(listThumbTopSource);
    const char* listThumbTop = listThumbTopName.Text();
    const std::string top(listThumbTop);
    delayList->scrollBar->SetThumbImages(top, middle, bottom);
  }
  this->delayList->AddNewItem();
  this->AddChild(this->delayList);

  this->description = new CUITextViewer(*this->m_context);
  this->description->SetSize(330, 35);
  this->description->Move(11, 334);
  this->description->SetTextOffset(0, 0);
  {
    const std::uint8_t descriptionFontSource[] = {
        0xD4, 0x00, 0xB3, 0x86, 0x20, 0x23};
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
    const std::uint8_t descriptionDownSource[] = {
        0x79, 0x82, 0xFD, 0xC9, 0x4D, 0x5D, 0xD0, 0x20, 0x7F, 0xC9, 0xFD, 0xFF, 0x30, 0x29, 0x49, 0x9E, 0xDA, 0x5F, 0x52, 0x5F, 0xE1, 0xA3, 0x71, 0x82,
        0xCE, 0x43, 0x13, 0x51, 0xE0, 0xE3, 0x15};
    thread_local EncryptedName<sizeof(descriptionDownSource), 0xED15938D333D2DB9ULL> descriptionDownName(descriptionDownSource);
    const char* descriptionDown = descriptionDownName.Text();
    const std::string down(descriptionDown);
    const std::uint8_t descriptionUpSource[] = {
        0x83, 0x96, 0x9B, 0x91, 0xA7, 0xE3, 0xB4, 0x04, 0x85, 0xDD, 0x9B, 0xA7, 0xDA, 0x97, 0x2D, 0xBA, 0x20, 0x4B, 0x34, 0x07, 0x0B, 0x1D, 0x04, 0xB9,
        0x6D, 0x5B, 0x36, 0x1B, 0x67};
    thread_local EncryptedName<sizeof(descriptionUpSource), 0xC9712D676B5B3943ULL> descriptionUpName(descriptionUpSource);
    const char* descriptionUp = descriptionUpName.Text();
    const std::string up(descriptionUp);
    this->description->SetScrollBarButtonImages(up, down);
  }
  {
    const std::uint8_t descriptionBackgroundSource[] = {
        0xFB, 0x48, 0xC1, 0xE1, 0x01, 0x6F, 0xEE, 0x70, 0xFD, 0x03, 0xC1, 0xD7, 0x7C, 0x1B, 0x77, 0xCE, 0x58, 0x95, 0x6E, 0x77, 0xAD, 0x91, 0x46, 0xD4,
        0x5F, 0xC9, 0x63, 0x76, 0xB1, 0xA1};
    thread_local EncryptedName<sizeof(descriptionBackgroundSource), 0xBD2BA1C11B01E73BULL> descriptionBackgroundName(descriptionBackgroundSource);
    const char* descriptionBackground = descriptionBackgroundName.Text();
    this->description->SetScrollBarBackground(std::string(descriptionBackground));
  }
  {
    const std::uint8_t descriptionThumbBottomSource[] = {
        0xB3, 0x88, 0x8D, 0xB7, 0x39, 0xA1, 0xC0, 0x1C, 0xB5, 0xC3, 0x8D, 0x81, 0x44, 0xD5, 0x59, 0xA2, 0x10, 0x55, 0x22, 0x21, 0x95, 0x5F, 0x67, 0xB0,
        0x01, 0x78, 0x29, 0x22, 0x8E, 0x01, 0x2B, 0xB3, 0x1E, 0x57, 0x4D};
    thread_local EncryptedName<sizeof(descriptionThumbBottomSource), 0xD1056FF94D4D2773ULL> descriptionThumbBottomName(descriptionThumbBottomSource);
    const char* descriptionThumbBottom = descriptionThumbBottomName.Text();
    const std::string bottom(descriptionThumbBottom);
    const std::uint8_t descriptionThumbMiddleSource[] = {
        0x2B, 0xF0, 0xC9, 0xC9, 0xBF, 0x6F, 0xF4, 0x8E, 0x2D, 0xBB, 0xC9, 0xFF, 0xC2, 0x1B, 0x6D, 0x30, 0x88, 0x2D, 0x66, 0x5F, 0x13, 0x91, 0x53, 0x22,
        0x99, 0x00, 0x64, 0x5A, 0x1B, 0x8F, 0x53, 0x2E, 0x9B, 0x5F};
    thread_local EncryptedName<sizeof(descriptionThumbMiddleSource), 0x4331A17F33095FEBULL> descriptionThumbMiddleName(descriptionThumbMiddleSource);
    const char* descriptionThumbMiddle = descriptionThumbMiddleName.Text();
    const std::string middle(descriptionThumbMiddle);
    const std::uint8_t descriptionThumbTopSource[] = {
        0xD1, 0x4C, 0x93, 0x63, 0x71, 0x93, 0xB0, 0x86, 0xD7, 0x07, 0x93, 0x55, 0x0C, 0xE7, 0x29, 0x38, 0x72, 0x91, 0x3C, 0xF5, 0xDD, 0x6D, 0x17, 0x2A,
        0x63, 0xBC, 0x26, 0xE9, 0x9F, 0x3F, 0x18, 0x3B, 0x11};
    thread_local EncryptedName<sizeof(descriptionThumbTopSource), 0x4B755DB19953E311ULL> descriptionThumbTopName(descriptionThumbTopSource);
    const char* descriptionThumbTop = descriptionThumbTopName.Text();
    const std::string top(descriptionThumbTop);
    this->description->SetScrollBarThumbImages(top, middle, bottom);
  }
  this->AddChild(this->description);
}
