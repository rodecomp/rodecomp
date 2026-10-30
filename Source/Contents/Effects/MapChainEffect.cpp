#include "Contents/Effects/MapChainEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B4A9E0
// FUNCTION: fn_18945
CMapChainEffect::CMapChainEffect()
    : CEffect(1024), particles(), unknown_00F0(2), unknown_00F4(2), unknown_00F8(2), unknown_00FC(2), unknown_0100(2) {
}

// OFFSET: 0x00B4B130
// FUNCTION: fn_18951
bool CMapChainEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B4B740
// FUNCTION: fn_18953
void CMapChainEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_000C > 0) {
      if (particle->unknown_0010 < 6) {
        particle->destinationBlend = 6;
      }

      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(particle->unknown_000C - 1) / 21.0f;
      quad.right = static_cast<float>(particle->unknown_000C) / 21.0f;
      quad.unused = true;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
