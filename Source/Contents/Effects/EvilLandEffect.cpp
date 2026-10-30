#include "Contents/Effects/EvilLandEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"

// OFFSET: 0x006FE300
// FUNCTION: fn_06944
CEvilLandEffect::CEvilLandEffect() : CEffect(1555), particles() {
}

// OFFSET: 0x006FE4F0
// FUNCTION: fn_06947
void CEvilLandEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x006FE830
// FUNCTION: fn_06949
bool CEvilLandEffect::Slot06() {
  return this->ActorExists();
}
