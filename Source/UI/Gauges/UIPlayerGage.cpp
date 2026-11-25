#include "UI/Gauges/UIPcGage.h"
#include "Utility/Math/Int64Math.h"

#include <algorithm>

// GLOBAL: gl_26172
float g_gageThreshold = 0.25f;

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
UIPcGage::UIPcGage() : current(0), maximum(0), secondaryValue(0), secondaryMaximum(0) {
  this->displayMode = 0;
}

// OFFSET: 0x0086E3A0
// FUNCTION: fn_11069
UIRechargeGage::UIRechargeGage() : current(0), maximum(100) {
}

// OFFSET: 0x0086E210
// FUNCTION: fn_11067
UIPlayerGage::UIPlayerGage() : actorKind(0) {
}

// OFFSET: 0x0086DE90
// FUNCTION: fn_11061
UIMonsterGage::UIMonsterGage() {
  this->colors[0] = 0x009C1810;
  this->colors[1] = 0x00424142;
  this->colors[2] = 0x00E700FF;
  this->colors[4] = 0x0000FFFF;
  this->m_threshold = 0x19;
  this->displayMode = 1;
}

// OFFSET: 0x00882450
// FUNCTION: fn_11304
bool UIPlayerGage::NeedsFrameRefresh() {
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
  this->displayMode = mode;
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

  switch (this->actorKind) {
    case 7:
      if (this->m_thirdCurrent && this->m_thirdMaximum) {
        const std::int64_t maxVal = MaxIntAndInt64(1, this->m_thirdMaximum());
        const std::int64_t curVal = this->m_thirdCurrent();
        const std::int64_t den = (std::max)(curVal, maxVal);
        const double num = static_cast<double>(this->m_thirdCurrent());
        const float ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[6] : this->colors[5]);
      }
      return;
    case 9:
    case 10:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum) {
        const std::int64_t maxVal1 = MaxIntAndInt64(1, this->m_firstMaximum());
        const std::int64_t curVal1 = this->m_firstCurrent();
        const std::int64_t den1 = (std::max)(curVal1, maxVal1);
        const double num1 = static_cast<double>(this->m_firstCurrent());
        const float ratio1 = static_cast<float>(num1 / static_cast<double>(den1));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio1), 3,
                         ratio1 < g_gageThreshold ? this->colors[4] : this->colors[2]);

        const std::int64_t maxVal2 = MaxIntAndInt64(1, this->m_secondMaximum());
        const std::int64_t curVal2 = this->m_secondCurrent();
        const int den2 = static_cast<int>((std::max)(curVal2, maxVal2));
        const double num2 = static_cast<double>(this->m_secondCurrent());
        const float ratio2 = static_cast<float>(num2 / static_cast<double>(den2));
        this->FillRegion(1, 5, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 5, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio2), 3,
                         this->colors[3]);
      }
      return;
    case 13:
    case 14:
      if (this->m_firstCurrent && this->m_firstMaximum) {
        const std::int64_t maxVal = MaxIntAndInt64(1, this->m_firstMaximum());
        const std::int64_t curVal = this->m_firstCurrent();
        const std::int64_t den = curVal > maxVal ? curVal : maxVal;
        const double num = static_cast<double>(this->m_firstCurrent());
        const float ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[4] : this->colors[2]);
      }
      return;
    case 8:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum &&
          this->m_thirdCurrent && this->m_thirdMaximum) {
        const std::int64_t maxVal1 = MaxIntAndInt64(1, this->m_firstMaximum());
        const std::int64_t curVal1 = this->m_firstCurrent();
        const std::int64_t den1 = (std::max)(curVal1, maxVal1);
        const double num1 = static_cast<double>(this->m_firstCurrent());
        const float ratio1 = static_cast<float>(num1 / static_cast<double>(den1));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio1), 3,
                         ratio1 < g_gageThreshold ? this->colors[4] : this->colors[2]);

        const std::int64_t maxVal2 = MaxIntAndInt64(1, this->m_secondMaximum());
        const std::int64_t curVal2 = this->m_secondCurrent();
        const std::int64_t den2 = (std::max)(curVal2, maxVal2);
        const double num2 = static_cast<double>(this->m_secondCurrent());
        const float ratio2 = static_cast<float>(num2 / static_cast<double>(den2));
        this->FillRegion(1, 5, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 5, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio2), 3,
                         this->colors[3]);

        const std::int64_t maxVal3 = MaxIntAndInt64(1, this->m_thirdMaximum());
        const std::int64_t curVal3 = this->m_thirdCurrent();
        const std::int64_t den3 = (std::max)(curVal3, maxVal3);
        const double num3 = static_cast<double>(this->m_thirdCurrent());
        const float ratio3 = static_cast<float>(num3 / static_cast<double>(den3));
        this->FillRegion(1, 9, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 9, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio3), 3,
                         ratio3 < g_gageThreshold ? this->colors[6] : this->colors[5]);
      }
      return;
    default:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum) {
        const std::int64_t maxVal1 = MaxIntAndInt64(1, this->m_firstMaximum());
        const std::int64_t curVal1 = this->m_firstCurrent();
        const std::int64_t den1 = curVal1 > maxVal1 ? curVal1 : maxVal1;
        const double num1 = static_cast<double>(this->m_firstCurrent());
        const float ratio1 = static_cast<float>(num1 / static_cast<double>(static_cast<int>(den1)));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio1), 3,
                         ratio1 < g_gageThreshold ? this->colors[4] : this->colors[2]);

        const std::int64_t maxVal2 = MaxIntAndInt64(1, this->m_secondMaximum());
        const std::int64_t curVal2 = this->m_secondCurrent();
        const std::int64_t den2 = curVal2 > maxVal2 ? curVal2 : maxVal2;
        const double num2 = static_cast<double>(this->m_secondCurrent());
        this->FillRegion(1, 5, this->geometry.width - 2, 3, this->colors[1]);
        const float ratio2 = static_cast<float>(num2 / static_cast<double>(static_cast<int>(den2)));
        this->FillRegion(1, 5, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio2), 3,
                         this->colors[3]);

        if (this->m_fourthCurrent && this->m_fourthMaximum) {
          const std::int64_t maxVal4 = MaxIntAndInt64(1, this->m_fourthMaximum());
          const std::int64_t curVal4 = this->m_fourthCurrent();
          const std::int64_t den4 = (std::max)(curVal4, maxVal4);
          const double num4 = static_cast<double>(this->m_fourthCurrent());
          const float ratio4 = static_cast<float>(num4 / static_cast<double>(static_cast<int>(den4)));
          this->FillRegion(1, 9, this->geometry.width - 2, 3, this->colors[1]);
          this->FillRegion(1, 9, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio4), 3,
                           this->colors[7]);
        }
      }
      return;
  }
}
