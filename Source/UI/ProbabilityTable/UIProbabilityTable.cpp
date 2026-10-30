#include "UI/ProbabilityTable/UIProbabilityTable.h"

#include "UI/Window/WindowBorder.h"

// OFFSET: 0x00A4BFF0
// FUNCTION: fn_16120
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A4C000
// FUNCTION: fn_16121
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A4C580
// FUNCTION: fn_16125
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::OnScroll(int position) {
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
    ProbabilityTable::CUIItem* item = *it;
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

// OFFSET: 0x00A4C780
// FUNCTION: fn_16126
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (ProbabilityTable::CUIItem* other : this->items) {
    if (other != item) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  ProbabilityTable::CUIItem* selectedItem = dynamic_cast<ProbabilityTable::CUIItem*>(item);
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

// OFFSET: 0x00A4DC90
// FUNCTION: fn_16195
void CUIProbabilityTable::OnButtonClick() {
}

// OFFSET: 0x00A4E030
// FUNCTION: fn_16198
void CUIProbabilityTable::OnPrepareItem(ProbabilityTable::CUIItem*, const ProbabilityTable::CItemData&) {
}

namespace ProbabilityTable {

// OFFSET: 0x00A4E110
// FUNCTION: fn_16200
void CUIItem::Paint() {
  this->ClearSurface(this->backgroundColor);
  this->FillRegion(0, this->geometry.height - 1, this->geometry.width, 1, 0xFFCEADA5);
  this->FillRegion(0, 0, 1, this->geometry.height, 0xFFCEADA5);
  this->FillRegion(232, 0, 1, this->geometry.height, 0xFFCEADA5);
  this->FillRegion(this->geometry.width - 1, 0, 1, this->geometry.height, 0xFFCEADA5);
}

}

// OFFSET: 0x00A4E180
// FUNCTION: fn_16201
void CUIProbabilityTable::Paint() {
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFF808080);
  this->FillRegion(10, 60, 312, 25, 0xFF99E5FF);
  this->FillRegion(10, 60, 312, 1, 0xFFCEADA5);
  for (int y = 84; y <= 444; y += 30) {
    this->FillRegion(10, y, 312, 1, 0xFFCEADA5);
  }
  this->FillRegion(10, 60, 1, 385, 0xFFCEADA5);
  this->FillRegion(242, 60, 1, 385, 0xFFCEADA5);
  this->FillRegion(321, 60, 1, 385, 0xFFCEADA5);
}

namespace ProbabilityTable {

// OFFSET: 0x00A4E250
// FUNCTION: fn_16202
void CUIItem::Slot16() {
  this->nameText->scrollBar->SetHidden(1);
  this->probabilityText->scrollBar->SetHidden(1);
}

// OFFSET: 0x00A4E350
// FUNCTION: fn_16204
void CUIItem::Slot25(int, int) {
}

}

// OFFSET: 0x00A4E360
// FUNCTION: fn_16205
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::UpdateScrollBar() {
  this->scrollBar->SetTotal(this->data.size() * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}
