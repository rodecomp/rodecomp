#include "Contents/EffectPrimitiveState.h"
#include "Contents/RagEffectState.h"

#include <bit>
#include <emmintrin.h>

namespace {
  int Difference(std::uint32_t left, std::uint32_t right) {
    return std::bit_cast<int>(left - right);
  }
}

// OFFSET: 0x00BC5E00
// FUNCTION: fn_19849
void EffectPrimitiveState::UpdateColor() {
  const auto current = std::bit_cast<int>(this->unknown_01E0);
  const bool valuesAndLimit = this->unknown_0158 != 0;

  if (valuesAndLimit || this->unknown_014C.y != 0) {
    auto index = this->unknown_01D8;
    const auto count = static_cast<std::uint32_t>(this->unknown_0258[0].size());
    const bool outside = valuesAndLimit ? index > count - 1u : index >= count;

    if (std::bit_cast<int>(index) < 0 || outside) {
      index = 0;
      this->unknown_01D8 = 0;
    }

    const float key = std::bit_cast<float>(this->unknown_0258[0][index]);

    if (static_cast<float>(current) == key) {
      const float value = std::bit_cast<float>(this->unknown_0258[1][index]);

      if (valuesAndLimit) {
        this->unknown_0324 = value;
        this->unknown_02D4 = std::bit_cast<float>(this->unknown_0258[2][index]);
      } else {
        this->unknown_0328 = value;
      }

      this->unknown_01D8 = index + 1;
    }
  }

  const float lower = std::bit_cast<float>(this->unknown_02D8);

  if ((this->flags & 0x80u) != 0) {
    const bool below =
      this->unknown_0328 >= 0.0f ? this->unknown_0324 < this->unknown_02D4 : lower < this->unknown_0324;

    if (!below) {
      this->unknown_0328 *= -1.0f;
    }
  }

  if (this->unknown_015C == 0) {
    const auto parentRemaining = static_cast<std::uint32_t>(this->parent->lifetime) - this->parent->unknown_0178;
    const int remaining = Difference(static_cast<std::uint32_t>(this->lifetime), this->unknown_01E0);

    if (remaining > std::bit_cast<int>(parentRemaining + 1u)) {
      this->unknown_015C = 1;
      this->flags &= ~0x80u;
      this->unknown_0328 = -(this->unknown_0324 / static_cast<float>(std::bit_cast<int>(parentRemaining)));
    }
  }

  if (current == this->unknown_01F0) {
    this->flags &= ~0x80u;
    const int remaining = Difference(static_cast<std::uint32_t>(this->lifetime), this->unknown_01E0);
    this->unknown_0328 = -(this->unknown_0324 / static_cast<float>(remaining)) * this->unknown_032C;
  }

  // MAXSS/MINSS return their second operand for unordered/equal inputs.
  const __m128 sum = _mm_set_ss(this->unknown_0328 + this->unknown_0324);
  const __m128 bounded = _mm_min_ss(_mm_set_ss(this->unknown_02D4), _mm_max_ss(_mm_set_ss(lower), sum));
  this->unknown_0324 = _mm_cvtss_f32(bounded);
  const auto alpha = static_cast<std::uint32_t>(_mm_cvttss_si32(bounded));
  const auto color =
    ((alpha * 256u + static_cast<unsigned char>(this->red)) * 256u + static_cast<unsigned char>(this->green)) * 256u +
    static_cast<unsigned char>(this->blue);
  this->unknown_01C0 = std::bit_cast<int>(color);
}
