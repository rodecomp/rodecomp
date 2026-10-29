#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffect.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00BC5E00
// FUNCTION: fn_19849
void EffectPrimitiveState::ProcessAlpha() {
  const int current = this->stateCnt;
  const bool valuesAndLimit = this->chRadiSpeed != 0;

  if (valuesAndLimit || this->unknown_014C.y != 0) {
    auto index = this->unknown_01D8;
    const auto count = static_cast<std::uint32_t>(this->unknown_0258[0].size());
    const bool outside = valuesAndLimit ? static_cast<unsigned>(index) > count - 1u : static_cast<unsigned>(index) >= count;

    if (index < 0 || outside) {
      index = 0;
      this->unknown_01D8 = 0;
    }

    const float key = this->unknown_0258[0][index];

    if (static_cast<float>(current) == key) {
      const float value = this->unknown_0258[1][index];

      if (valuesAndLimit) {
        this->alpha = value;
        this->maxAlpha = this->unknown_0258[2][index];
      } else {
        this->alphaSpeed = value;
      }

      this->unknown_01D8 = index + 1;
    }
  }

  const float lower = this->minAlpha;

  if ((this->pattern & 0x80u) != 0) {
    const bool below = this->alphaSpeed >= 0.0f ? this->alpha < this->maxAlpha : lower < this->alpha;

    if (!below) {
      this->alphaSpeed *= -1.0f;
    }
  }

  if (this->isDisappear == 0) {
    const int parentRemaining = this->master->state.lifetime - this->master->state.unknown_0178;
    const int remaining = this->duration - current;

    if (remaining > parentRemaining + 1) {
      this->isDisappear = 1;
      this->pattern &= ~0x80u;
      this->alphaSpeed = -(this->alpha / static_cast<float>(parentRemaining));
    }
  }

  if (current == this->fadeOutCnt) {
    this->pattern &= ~0x80u;
    const int remaining = this->duration - current;
    this->alphaSpeed = -(this->alpha / static_cast<float>(remaining)) * this->alphaDelta;
  }

  const float sum = this->alphaSpeed + this->alpha;
  const float minimum = lower > sum ? lower : sum;
  const float bounded = this->maxAlpha < minimum ? this->maxAlpha : minimum;
  this->alpha = bounded;
  const auto alpha = static_cast<std::uint32_t>(TruncateToInt(bounded));
  const auto color =
    ((alpha * 256u + static_cast<unsigned char>(this->red)) * 256u + static_cast<unsigned char>(this->green)) * 256u + static_cast<unsigned char>(this->blue);
  this->argb = color;
}
