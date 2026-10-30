#include "Contents/Effects/BindTrapEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x006FB2F0

// OFFSET: 0x006FB4E0

// OFFSET: 0x006FB5B0

// OFFSET: 0x006FB5F0
// FUNCTION: fn_06908
void CBindTrapEffect::Slot01() {
  switch (this->id) {
  case 1069:
    this->AddParticle(0);
    break;
  case 1070:
    this->AddParticle(1);
    break;
  case 1071:
    this->AddParticle(2);
    break;
  }
}

// OFFSET: 0x006FB620
// FUNCTION: fn_06909
bool CBindTrapEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FB630

// OFFSET: 0x006FB8B0
// FUNCTION: fn_06911
void CBindTrapEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
