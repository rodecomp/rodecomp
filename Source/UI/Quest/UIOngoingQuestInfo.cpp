#include "UI/Quest/UIOngoingQuestInfo.h"

// OFFSET: 0x00A4FDE0
// FUNCTION: fn_16223
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::ClearItems() {
  for (OngoingQuestInfo::CUIRewardItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A4FE70
// FUNCTION: fn_16224
void CUIOngoingQuestInfo::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(342, 412);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A55650
// FUNCTION: fn_16300
void CUIOngoingQuestInfo::OnFirstAction() {
}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55710
// FUNCTION: fn_16301
void CUIRewards::OnPrepareItem(CUIRewardItem*, const CRewardItemData&) {
}

// OFFSET: 0x00A55860
// FUNCTION: fn_16303
void CUIContents::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A55870
// FUNCTION: fn_16304
void CUIRewardItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A55880
// FUNCTION: fn_16305
void CUIRewards::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A55890
// FUNCTION: fn_16306
void CUIScrollWindow::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A558A0
// FUNCTION: fn_16307
void CUIContents::Slot16() {
}

// OFFSET: 0x00A55940
// FUNCTION: fn_16309
void CUIRewardItem::Slot16() {
}

// OFFSET: 0x00A55B50
// FUNCTION: fn_16312
void CUIScrollWindow::Slot35(int value) {
  this->scrollBar->Slot35(value);
}

// OFFSET: 0x00A55F20
// FUNCTION: fn_16316
void CUIScrollWindow::OnScroll(int position) {
  int y = -position;
  for (UIWindow* child : this->m_children) {
    if (child != this->scrollBar) {
      child->Move(child->geometry.x, y);
      y = child->geometry.y + child->geometry.height;
    }
  }
}

}
