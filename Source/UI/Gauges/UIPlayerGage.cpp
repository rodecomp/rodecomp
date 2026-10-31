#include "UI/Gauges/UIPcGage.h"

// OFFSET: 0x0086D2D0

// OFFSET: 0x0086E120
// FUNCTION: fn_11065
UIPcGage::UIPcGage() : unknown_009C(0), current(0), maximum(0), unknown_00A8(0), unknown_00AC(0) {
}

// OFFSET: 0x0086E3A0
// FUNCTION: fn_11069
UIRechargeGage::UIRechargeGage() : current(0), maximum(100) {
}

// OFFSET: 0x0086E210
// FUNCTION: fn_11067
UIPlayerGage::UIPlayerGage() : unknown_009C(0) {
}

// OFFSET: 0x0086DE90

// OFFSET: 0x00882450
// FUNCTION: fn_11304
bool UIPlayerGage::Slot03() {
  return true;
}

// OFFSET: 0x0089B560
// FUNCTION: fn_11568
void UIRechargeGage::SetValues(int current, int maximum) {
  this->current = current;
  this->maximum = maximum;
  this->Invalidate();
}

// OFFSET: 0x0089B590
// FUNCTION: fn_11569
void UIPlayerGage::SetFourthValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_fourthCurrent = current;
  this->m_fourthMaximum = maximum;
}

// OFFSET: 0x0089BCF0
// FUNCTION: fn_11578
void UIPlayerGage::SetFirstValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_firstCurrent = current;
  this->m_firstMaximum = maximum;
}

// OFFSET: 0x0089BDA0
// FUNCTION: fn_11579
void UIPlayerGage::SetThirdValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_thirdCurrent = current;
  this->m_thirdMaximum = maximum;
}

// OFFSET: 0x0089C070
// FUNCTION: fn_11580
void UIMonsterGage::SetMode(int mode) {
  this->unknown_009C = mode;
}

// OFFSET: 0x0089C390
// FUNCTION: fn_11587
void UIPlayerGage::SetSecondValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_secondCurrent = current;
  this->m_secondMaximum = maximum;
}
