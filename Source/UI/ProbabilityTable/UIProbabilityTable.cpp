#include "UI/ProbabilityTable/UIProbabilityTable.h"

#include "UI/Window/WindowBorder.h"

// OFFSET: 0x00A4BFF0
// FUNCTION: fn_16120
template <>
void CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A4C000

// OFFSET: 0x00A4C580

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
