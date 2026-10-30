#include "Contents/Effects/Level99Orb2Effect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B6C920
// FUNCTION: fn_19102
void CLevel99Orb2Effect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      for (int pass = 0; pass < 2; ++pass) {
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      }
    }
  }
}
