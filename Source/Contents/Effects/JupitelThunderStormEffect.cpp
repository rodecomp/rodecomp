#include "Contents/Effects/JupitelThunderStormEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

// INLINED
static bool ParticleAlive(const CParticle* particle) {
  return particle->life == 0.0f || static_cast<float>(particle->unknown_00E8) < particle->life * 60.0f;
}

// OFFSET: 0x00705F80
// FUNCTION: fn_07040
CJupitelThunderStormEffect::CJupitelThunderStormEffect() : CEffect(2366), particle(nullptr), step{}, duration(10) {
}

// OFFSET: 0x00706110
// FUNCTION: fn_07042
void CJupitelThunderStormEffect::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->particles.clear();
}

// OFFSET: 0x007065F0

// OFFSET: 0x00706690

// OFFSET: 0x007067C0
