#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffect.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00BC5E00
// FUNCTION: fn_19849
void EffectPrimitiveState::ProcessAlpha() {
  if (this->changeFlags.z != 0) {
    int index = this->colorCycleIndex;

    if (index < 0 || static_cast<std::uint32_t>(index) > this->chPoint.size() - 1) {
      this->colorCycleIndex = 0;
      index = 0;
    }

    if (static_cast<float>(this->stateCnt) == this->chPoint[index]) {
      this->alpha = this->chVal1[index];
      this->maxAlpha = this->chVal2[index];
      this->colorCycleIndex = index + 1;
    }
  } else if (this->changeFlags.y != 0) {
    int index = this->colorCycleIndex;

    if (index < 0 || static_cast<std::uint32_t>(index) >= this->chPoint.size()) {
      this->colorCycleIndex = 0;
      index = 0;
    }

    if (static_cast<float>(this->stateCnt) == this->chPoint[index]) {
      this->alphaSpeed = this->chVal1[index];
      this->colorCycleIndex = index + 1;
    }
  }

  if ((this->pattern & 0x80u) != 0) {
    if (this->alphaSpeed < 0.0f ? this->alpha <= this->minAlpha : this->alpha >= this->maxAlpha) {
      this->alphaSpeed *= -1.0f;
    }
  }

  if (this->isDisappear == 0 && this->duration - this->stateCnt > this->master->state.duration - this->master->state.stateCnt + 1) {
    this->isDisappear = 1;
    this->pattern &= ~0x80u;
    this->alphaSpeed = -(this->alpha / static_cast<float>(this->master->state.duration - this->master->state.stateCnt));
  }

  if (this->stateCnt == this->fadeOutCnt) {
    this->pattern &= ~0x80u;
    this->alphaSpeed = -(this->alpha / static_cast<float>(this->duration - this->stateCnt)) * this->alphaDelta;
  }

  const float sum = this->alphaSpeed + this->alpha;
  const float minimum = this->minAlpha > sum ? this->minAlpha : sum;
  const float bounded = this->maxAlpha < minimum ? this->maxAlpha : minimum;
  this->alpha = bounded;
  const int alpha = static_cast<int>(bounded);
  const auto color =
    ((alpha * 256u + static_cast<unsigned char>(this->red)) * 256u + static_cast<unsigned char>(this->green)) * 256u + static_cast<unsigned char>(this->blue);
  this->argb = color;
}
