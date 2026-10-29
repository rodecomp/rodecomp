#include "Contents/Effects/WindEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00730870
// FUNCTION: fn_07543
bool CWindEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00731030
// FUNCTION: fn_07545
void CWindEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    if (texPart <= 0) {
      continue;
    }

    const ParticleQuad quad = {true, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f, static_cast<float>(texPart) / 21.0f, false, true};
    particle->Render(quad);
  }
}
