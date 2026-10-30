#include "Contents/Effects/LightSphereEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B6D990
// FUNCTION: fn_19110
void CLightSphereEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->spheres.begin(); it != this->spheres.end(); ++it) {
    CParticle* particle = *it;

    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->rays.begin(); it != this->rays.end(); ++it) {
    CParticle* particle = *it;

    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
