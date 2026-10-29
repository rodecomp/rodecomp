#include "Contents/Effects/MagicFloorEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B49760
// FUNCTION: fn_18936
void CMagicFloorEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->floors.begin(); it != this->floors.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_000C > 0) {
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(particle->unknown_000C - 1) / 21.0f;
      quad.right = static_cast<float>(particle->unknown_000C) / 21.0f;
      quad.unused = false;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
