#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"

template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();

namespace AdventureGuide {

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

// OFFSET: 0x009DDA40
// FUNCTION: fn_14347
void CUIIntro::Slot54(int, int) {
}

// OFFSET: 0x009DDE50
// FUNCTION: fn_14350
void CUIMonster::Slot54(int, int) {
  this->unknown_0084->Show(0);
}

}
