#include "UI/Gauges/UIPcGage.h"

// OFFSET: 0x0086D2D0
// FUNCTION: fn_11044
UIGage::UIGage() : UIWindow(0) {
  this->colors[0] = 0x009B1914;
  this->colors[1] = 0x00444444;
  this->colors[2] = 0x0025EA16;
  this->colors[3] = 0x00DB651D;
  this->colors[4] = 0x001616EA;
  this->colors[5] = 0x00E1E4FF;
  this->colors[6] = 0x001616EA;
  this->colors[7] = 0x0060C0FF;
}

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
// FUNCTION: fn_11061
UIMonsterGage::UIMonsterGage() : m_threshold(0x19) {
  this->colors[0] = 0x009C1810;
  this->colors[1] = 0x00424142;
  this->colors[2] = 0x00E700FF;
  this->colors[4] = 0x0000FFFF;
  this->unknown_009C = 1;
}

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

// OFFSET: 0x0088CB50
// FUNCTION: fn_45488
void UIPlayerGage::Paint() {
  this->ClearSurface(this->colors[0]);

  const auto drawBar = [this](int y, std::function<std::int64_t()>& current, std::function<std::int64_t()>& maximum,
                              std::uint32_t color, std::uint32_t alternateColor, bool thresholdColor,
                              bool wideDenominator) {
    const std::int64_t maximumValue = (std::max)(std::int64_t(1), maximum());
    const std::int64_t denominator = (std::max)(current(), maximumValue);
    const std::int64_t numerator = current();
    const double divisor =
        wideDenominator ? static_cast<double>(denominator) : static_cast<double>(static_cast<int>(denominator));
    const float ratio = static_cast<float>(static_cast<double>(numerator) / divisor);

    this->FillRegion(1, y, this->geometry.width - 2, 3, this->colors[1]);
    if (thresholdColor && ratio < 0.25f) {
      color = alternateColor;
    }
    this->FillRegion(1, y, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3, color);
  };

  switch (this->unknown_009C) {
    case 7:
      if (this->m_thirdCurrent && this->m_thirdMaximum) {
        drawBar(1, this->m_thirdCurrent, this->m_thirdMaximum, this->colors[5], this->colors[6], true, true);
      }
      break;
    case 9:
    case 10:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum) {
        drawBar(1, this->m_firstCurrent, this->m_firstMaximum, this->colors[2], this->colors[4], true, true);
        drawBar(5, this->m_secondCurrent, this->m_secondMaximum, this->colors[3], this->colors[3], false, false);
      }
      break;
    case 13:
    case 14:
      if (this->m_firstCurrent && this->m_firstMaximum) {
        drawBar(1, this->m_firstCurrent, this->m_firstMaximum, this->colors[2], this->colors[4], true, true);
      }
      break;
    case 8:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum &&
          this->m_thirdCurrent && this->m_thirdMaximum) {
        drawBar(1, this->m_firstCurrent, this->m_firstMaximum, this->colors[2], this->colors[4], true, true);
        drawBar(5, this->m_secondCurrent, this->m_secondMaximum, this->colors[3], this->colors[3], false, true);
        drawBar(9, this->m_thirdCurrent, this->m_thirdMaximum, this->colors[5], this->colors[6], true, true);
      }
      break;
    default:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum) {
        drawBar(1, this->m_firstCurrent, this->m_firstMaximum, this->colors[2], this->colors[4], true, false);
        drawBar(5, this->m_secondCurrent, this->m_secondMaximum, this->colors[3], this->colors[3], false, false);
        if (this->m_fourthCurrent && this->m_fourthMaximum) {
          drawBar(9, this->m_fourthCurrent, this->m_fourthMaximum, this->colors[7], this->colors[7], false, false);
        }
      }
      break;
  }
}
