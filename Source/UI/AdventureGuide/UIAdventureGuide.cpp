#include "UI/AdventureGuide/UIAdventureGuide.h"

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
