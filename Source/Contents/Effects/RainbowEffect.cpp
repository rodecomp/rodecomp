#include "Contents/Effects/RainbowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B70E00

// OFFSET: 0x00B714C0
// FUNCTION: fn_19132
void CRainbowEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    CParticle* particle = *it;

    if (particle && particle->unknown_000C > 0) {
      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, 0.0f, 1.0f, false, true};
      particle->Render(quad);
    }
  }
}
