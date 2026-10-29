#include "Contents/Effects/AMachineEffect.h"

// OFFSET: 0x006F6510
// FUNCTION: fn_06846
CAMachineEffect::CAMachineEffect() : CEffect(-1), unknown_00D4(0) {
}

// OFFSET: 0x006F6DA0
// FUNCTION: fn_06852
bool CAMachineEffect::Slot06() {
  return this->ActorExists();
}
