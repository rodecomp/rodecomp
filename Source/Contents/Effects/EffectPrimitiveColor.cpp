#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/AttachedEffect.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00BC5E00
// FUNCTION: fn_19849
void EffectPrimitiveState::ProcessAlpha() {
  const int current = this->unknown_01E0;
  const bool valuesAndLimit = this->unknown_0158 != 0;

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
        this->unknown_02D4 = this->unknown_0258[2][index];
      } else {
        this->alphaSpeed = value;
      }

      this->unknown_01D8 = index + 1;
    }
  }

  const float lower = this->unknown_02D8;

  if ((this->flags & 0x80u) != 0) {
    const bool below = this->alphaSpeed >= 0.0f ? this->alpha < this->unknown_02D4 : lower < this->alpha;

    if (!below) {
      this->alphaSpeed *= -1.0f;
    }
  }

  if (this->unknown_015C == 0) {
    const int parentRemaining = this->master->state.lifetime - this->master->state.unknown_0178;
    const int remaining = this->lifetime - current;

    if (remaining > parentRemaining + 1) {
      this->unknown_015C = 1;
      this->flags &= ~0x80u;
      this->alphaSpeed = -(this->alpha / static_cast<float>(parentRemaining));
    }
  }

  if (current == this->unknown_01F0) {
    this->flags &= ~0x80u;
    const int remaining = this->lifetime - current;
    this->alphaSpeed = -(this->alpha / static_cast<float>(remaining)) * this->alphaDelta;
  }

  const float sum = this->alphaSpeed + this->alpha;
  const float minimum = lower > sum ? lower : sum;
  const float bounded = this->unknown_02D4 < minimum ? this->unknown_02D4 : minimum;
  this->alpha = bounded;
  const auto alpha = static_cast<std::uint32_t>(TruncateToInt(bounded));
  const auto color =
    ((alpha * 256u + static_cast<unsigned char>(this->red)) * 256u + static_cast<unsigned char>(this->green)) * 256u + static_cast<unsigned char>(this->blue);
  this->unknown_01C0 = color;
}
