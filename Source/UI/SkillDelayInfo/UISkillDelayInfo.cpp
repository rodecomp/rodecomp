#include "UI/SkillDelayInfo/SkillDelayInfo.h"
#include "Contents/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/WindowBorder.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>

template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Paint();
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Slot35(int value);
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnScroll(int position);

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
