#include "Contents/Effects/CloudKillEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x006FC410
// FUNCTION: fn_06923
CCloudKillEffect::CCloudKillEffect() : CEffect(1080) {
  this->unknown_002C = 9999;
  this->unknown_00E4 = 1;
  this->unknown_00DC = 10;
  this->unknown_00E0 = 40;
}

// OFFSET: 0x006FCBE0
// FUNCTION: fn_06927
void CCloudKillEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }
  this->particles.clear();

  for (CParticle* sprite : this->sprites) {
    FreeParticle(sprite);
  }
  this->sprites.clear();
}

// OFFSET: 0x006FCCB0
// FUNCTION: fn_06928
void CCloudKillEffect::Slot01() {
}
