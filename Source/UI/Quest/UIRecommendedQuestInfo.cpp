#include "UI/Quest/UIRecommendedQuestInfo.h"

// OFFSET: 0x00A568E0
// FUNCTION: fn_16321
void CUIRecommendedQuestInfo::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(342, 412);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A58BA0
// FUNCTION: fn_16346
void CUIRecommendedQuestInfo::OnFirstAction() {
}

namespace RecommendedQuestInfo {

// OFFSET: 0x00A58BC0
// FUNCTION: fn_16347
void CUIQuestInfo::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A58BD0
// FUNCTION: fn_16348
void CUIQuestInfo::Slot16() {
}

}
