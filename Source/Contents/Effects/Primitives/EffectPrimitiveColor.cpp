#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00BC5E00
// FUNCTION: fn_19849
void CEffectPrim::ProcessAlpha() {
  if (this->state.changeFlags.z != 0) {
    int index = this->state.colorCycleIndex;

    if (index < 0 || static_cast<std::uint32_t>(index) > this->state.chPoint.size() - 1) {
      this->state.colorCycleIndex = 0;
      index = 0;
    }

    if (static_cast<float>(this->state.stateCnt) == this->state.chPoint[index]) {
      this->state.alpha = this->state.chVal1[index];
      this->state.maxAlpha = this->state.chVal2[index];
      this->state.colorCycleIndex = index + 1;
    }
  } else if (this->state.changeFlags.y != 0) {
    int index = this->state.colorCycleIndex;

    if (index < 0 || static_cast<std::uint32_t>(index) >= this->state.chPoint.size()) {
      this->state.colorCycleIndex = 0;
      index = 0;
    }

    if (static_cast<float>(this->state.stateCnt) == this->state.chPoint[index]) {
      this->state.alphaSpeed = this->state.chVal1[index];
      this->state.colorCycleIndex = index + 1;
    }
  }

  if ((this->state.pattern & 0x80u) != 0) {
    if (this->state.alphaSpeed < 0.0f ? this->state.alpha <= this->state.minAlpha : this->state.alpha >= this->state.maxAlpha) {
      this->state.alphaSpeed *= -1.0f;
    }
  }

  if (this->state.isDisappear == 0 && this->state.duration - this->state.stateCnt > this->state.master->state.duration - this->state.master->state.stateCnt + 1) {
    this->state.isDisappear = 1;
    this->state.pattern &= ~0x80u;
    this->state.alphaSpeed = -(this->state.alpha / static_cast<float>(this->state.master->state.duration - this->state.master->state.stateCnt));
  }

  if (this->state.stateCnt == this->state.fadeOutCnt) {
    this->state.pattern &= ~0x80u;
    this->state.alphaSpeed = -(this->state.alpha / static_cast<float>(this->state.duration - this->state.stateCnt)) * this->state.alphaDelta;
  }

  const float sum = this->state.alphaSpeed + this->state.alpha;
  const float minimum = this->state.minAlpha > sum ? this->state.minAlpha : sum;
  const float bounded = this->state.maxAlpha < minimum ? this->state.maxAlpha : minimum;
  this->state.alpha = bounded;
  const int alpha = static_cast<int>(bounded);
  const auto color =
    ((alpha * 256u + static_cast<unsigned char>(this->state.red)) * 256u + static_cast<unsigned char>(this->state.green)) * 256u +
    static_cast<unsigned char>(this->state.blue);
  this->state.argb = color;
}
