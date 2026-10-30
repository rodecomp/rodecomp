#include "Contents/Effects/SkillEffects.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"

// OFFSET: 0x007077D0
// FUNCTION: fn_07055
void CMeteorEffect::Close() {
  for (CParticle* particle : this->unknown_00CC) {
    FreeParticle(particle);
  }

  this->unknown_00CC.clear();
}

// OFFSET: 0x00707B40
// FUNCTION: fn_07057
bool CMeteorEffect::Slot06() {
  return !this->unknown_00CC.empty();
}

// OFFSET: 0x00707CA0
// FUNCTION: fn_07059
void CMeteorEffect::Slot04() {
  for (CParticle* particle : this->unknown_00CC) {
    if ((particle->life == 0.0f || particle->life * 60.0f > static_cast<float>(particle->unknown_00E8)) && particle) {
      static_cast<CSpriteParticle*>(particle)->sub_B40F30(0);
    }
  }
}
