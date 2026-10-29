#include "Contents/Effects/AstralStrikeEffect.h"

// OFFSET: 0x006F4830
// FUNCTION: fn_06828
CAstralStrikeEffect::CAstralStrikeEffect() : CEffect(1768) {
}

// OFFSET: 0x006F54B0
// FUNCTION: fn_06834
bool CAstralStrikeEffect::Slot06() {
  return this->ActorExists();
}
