#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Utility/Math/ScalarConversion.h"

// INLINED
static __forceinline void LimitAlpha(CEffectPrim& primitive) {
  if ((primitive.state.pattern & 0x80u) != 0) {
    if (primitive.state.alphaSpeed < 0.0f) {
      if (primitive.state.alpha <= primitive.state.minAlpha) {
        primitive.state.alphaSpeed *= -1.0f;
      }
    } else if (primitive.state.alpha >= primitive.state.maxAlpha) {
      primitive.state.alphaSpeed *= -1.0f;
    }
  }

  if (primitive.state.isDisappear == 0 && primitive.state.duration - primitive.state.stateCnt > primitive.state.master->state.duration - primitive.state.master->state.stateCnt + 1) {
    primitive.state.isDisappear = 1;
    primitive.state.pattern &= ~0x80u;
    primitive.state.alphaSpeed = -(primitive.state.alpha / static_cast<float>(primitive.state.master->state.duration - primitive.state.master->state.stateCnt));
  }
}

// OFFSET: 0x00BC5E00
// FUNCTION: fn_19849
void CEffectPrim::ProcessAlpha() {
  if (this->state.alphaCycle != 0) {
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

    LimitAlpha(*this);
  } else {
    if (this->state.alphaSpeedCycle != 0) {
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

    LimitAlpha(*this);
  }

  if (this->state.stateCnt == this->state.fadeOutCnt) {
    this->state.pattern &= ~0x80u;
    this->state.alphaSpeed = -(this->state.alpha / static_cast<float>(this->state.duration - this->state.stateCnt)) * this->state.alphaDelta;
  }

  const float sum = this->state.alphaSpeed + this->state.alpha;
  auto& minAlphaRef = this->state.minAlpha;
  const float minimum = minAlphaRef > sum ? minAlphaRef : sum;
  const float bounded = this->state.maxAlpha < minimum ? this->state.maxAlpha : minimum;
  this->state.alpha = bounded;
  const int alpha = static_cast<int>(bounded);
  const auto color =
    ((alpha * 256u + static_cast<unsigned char>(this->state.red)) * 256u + static_cast<unsigned char>(this->state.green)) * 256u +
    static_cast<unsigned char>(this->state.blue);
  this->state.argb = color;
}
