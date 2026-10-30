#include "Contents/Effects/RichManKim.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x0070B160
// FUNCTION: fn_07114
void CRichManKim::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0070B1A0
// FUNCTION: fn_07115
void CRichManKim::Slot01() {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\pocket.bmp");
  particle->unknown_0004 = this->id;
  particle->unknown_000C = 0;
  particle->unknown_0010 = 0;
  particle->unknown_0008 = 1;
  particle->alpha = 200;
  particle->unknown_00C4 = 0;
  particle->unknown_004C = 0;
  particle->scale = 1.0f;
  *reinterpret_cast<float*>(particle->unknown_0058) = 0.0f;
  particle->unknown_0054 = -5.0f - static_cast<float>(rand() % 5);
  particle->SetColor(200, 200, 200);
  InitParticleQuad(particle->vertices);
  this->angleIndex = rand() % 359;
  this->particles.push_back(particle);
}

// OFFSET: 0x0070B370
// FUNCTION: fn_07116
bool CRichManKim::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070B380

// OFFSET: 0x0070B780
// FUNCTION: fn_07118
void CRichManKim::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
