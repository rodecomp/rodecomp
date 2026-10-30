#include "Contents/Effects/RotateBgEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B734E0
// FUNCTION: fn_19145
void CRotateBgEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    if (CParticle* particle = *it) {
      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, 0.0f, 1.0f, (segment - 1) / 21.0f, segment / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
