#include "Contents/Effects/PowderingEffect.h"

// OFFSET: 0x00708E70
// FUNCTION: fn_07085
CPowderingEffect::CPowderingEffect() : CEffect(-1), unknown_00E4(0) {
}

// OFFSET: 0x00709690
// FUNCTION: fn_07091
void CPowderingEffect::Slot12() {
  if (this->id == 1104) {
    this->AddEvent(0, "effect\\su_cn_powdering.wav", 0, 0);
  }
}
