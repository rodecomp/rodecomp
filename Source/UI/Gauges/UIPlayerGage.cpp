#include "UI/Gauges/UIPcGage.h"
#include "Network/Packets/NoticeEnums.h"
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
UIPlayerGage::UIPlayerGage() : actorKind(PC_TYPE) {
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

  std::int64_t maxVal;
  std::int64_t curVal;
  std::int64_t den;
  int denInt;
  double num;
  float ratio;

  switch (this->actorKind) {
    case NPC_PET_TYPE:
      if (this->m_thirdCurrent && this->m_thirdMaximum) {
        maxVal = MaxIntAndInt64(1, this->m_thirdMaximum());
        curVal = this->m_thirdCurrent();
        den = (std::max)(curVal, maxVal);
        num = static_cast<double>(this->m_thirdCurrent());
        ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[6] : this->colors[5]);
      }
      return;
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum) {
        maxVal = MaxIntAndInt64(1, this->m_firstMaximum());
        curVal = this->m_firstCurrent();
        den = (std::max)(curVal, maxVal);
        num = static_cast<double>(this->m_firstCurrent());
        ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[4] : this->colors[2]);

        maxVal = MaxIntAndInt64(1, this->m_secondMaximum());
        curVal = this->m_secondCurrent();
        denInt = static_cast<int>((std::max)(curVal, maxVal));
        num = static_cast<double>(this->m_secondCurrent());
        ratio = static_cast<float>(num / static_cast<double>(denInt));
        this->FillRegion(1, 5, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 5, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         this->colors[3]);
      }
      return;
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      if (this->m_firstCurrent && this->m_firstMaximum) {
        maxVal = MaxIntAndInt64(1, this->m_firstMaximum());
        curVal = this->m_firstCurrent();
        den = curVal > maxVal ? curVal : maxVal;
        num = static_cast<double>(this->m_firstCurrent());
        ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[4] : this->colors[2]);
      }
      return;
    case NPC_HO_TYPE:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum &&
          this->m_thirdCurrent && this->m_thirdMaximum) {
        maxVal = MaxIntAndInt64(1, this->m_firstMaximum());
        curVal = this->m_firstCurrent();
        den = (std::max)(curVal, maxVal);
        num = static_cast<double>(this->m_firstCurrent());
        ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[4] : this->colors[2]);

        maxVal = MaxIntAndInt64(1, this->m_secondMaximum());
        curVal = this->m_secondCurrent();
        den = (std::max)(curVal, maxVal);
        num = static_cast<double>(this->m_secondCurrent());
        ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 5, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 5, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         this->colors[3]);

        maxVal = MaxIntAndInt64(1, this->m_thirdMaximum());
        curVal = this->m_thirdCurrent();
        den = (std::max)(curVal, maxVal);
        num = static_cast<double>(this->m_thirdCurrent());
        ratio = static_cast<float>(num / static_cast<double>(den));
        this->FillRegion(1, 9, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 9, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[6] : this->colors[5]);
      }
      return;
    default:
      if (this->m_firstCurrent && this->m_firstMaximum && this->m_secondCurrent && this->m_secondMaximum) {
        maxVal = MaxIntAndInt64(1, this->m_firstMaximum());
        curVal = this->m_firstCurrent();
        den = curVal > maxVal ? curVal : maxVal;
        num = static_cast<double>(this->m_firstCurrent());
        ratio = static_cast<float>(num / static_cast<double>(static_cast<int>(den)));
        this->FillRegion(1, 1, this->geometry.width - 2, 3, this->colors[1]);
        this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         ratio < g_gageThreshold ? this->colors[4] : this->colors[2]);

        maxVal = MaxIntAndInt64(1, this->m_secondMaximum());
        curVal = this->m_secondCurrent();
        den = curVal > maxVal ? curVal : maxVal;
        num = static_cast<double>(this->m_secondCurrent());
        this->FillRegion(1, 5, this->geometry.width - 2, 3, this->colors[1]);
        ratio = static_cast<float>(num / static_cast<double>(static_cast<int>(den)));
        this->FillRegion(1, 5, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                         this->colors[3]);

        if (this->m_fourthCurrent && this->m_fourthMaximum) {
          maxVal = MaxIntAndInt64(1, this->m_fourthMaximum());
          curVal = this->m_fourthCurrent();
          den = (std::max)(curVal, maxVal);
          num = static_cast<double>(this->m_fourthCurrent());
          ratio = static_cast<float>(num / static_cast<double>(static_cast<int>(den)));
          this->FillRegion(1, 9, this->geometry.width - 2, 3, this->colors[1]);
          this->FillRegion(1, 9, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), 3,
                           this->colors[7]);
        }
      }
      return;
  }
}
