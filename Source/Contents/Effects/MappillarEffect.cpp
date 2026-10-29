#include "Contents/Effects/MappillarEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B4C0B0
// FUNCTION: fn_18957
bool CMappillarEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B4C4C0
// FUNCTION: fn_18959
void CMappillarEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      const int texPart = particle->unknown_000C;
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(texPart - 1) / 21.0f;
      quad.right = static_cast<float>(texPart) / 21.0f;
      quad.unused = false;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
