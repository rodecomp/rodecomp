#include "UI/UIBarGraph.h"

// OFFSET: 0x0089B4D0
// FUNCTION: fn_11564
void UIBarGraph4::SetValues(const int* current, const int* maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

// OFFSET: 0x0089B4F0
// FUNCTION: fn_11565
void UIBarGraph::SetValue(float value) {
  this->m_value = value;
  this->Invalidate();
}

// OFFSET: 0x0089B510
// FUNCTION: fn_11566
void UIINT64BarGraph::SetValues(std::int64_t current, std::int64_t maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

// OFFSET: 0x0089B540
// FUNCTION: fn_11567
void UIPetInfoBarGraph::SetValue(int value) {
  this->unknown_007C = value;
  this->Invalidate();
}
