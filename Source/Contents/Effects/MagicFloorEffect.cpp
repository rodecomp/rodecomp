#include "Contents/Effects/MagicFloorEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

// OFFSET: 0x00B48220

// OFFSET: 0x00B48400

// OFFSET: 0x00B48880

// OFFSET: 0x00B48D50

// OFFSET: 0x00B48DC0
// FUNCTION: fn_18932
void CMagicFloorEffect::Slot01() {
  this->AddFloors();
  this->AddClouds(0);
  this->AddClouds(1);
}

// OFFSET: 0x00B48E70
// FUNCTION: fn_18933
void CMagicFloorEffect::Slot07() {
  for (std::vector<CParticle*>::iterator it = this->floors.begin(); it != this->floors.end(); ++it) {
    CParticle* particle = *it;
    particle->flags = 5;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    particle->scale = *reinterpret_cast<const float*>(&this->unknown_0048);
    *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(*reinterpret_cast<const int*>(this->unknown_004C));
    particle->unknown_004C = static_cast<int>(static_cast<float>(particle->unknown_0010) * *reinterpret_cast<const float*>(&this->unknown_0050));
    particle->alpha = *reinterpret_cast<const int*>(this->unknown_0054);
    particle->sourceBlend = 5;
    particle->destinationBlend = 7;
    InitParticleQuad(particle->vertices);
    particle->unknown_00E8 = 0;
  }
}

// OFFSET: 0x00B48F20
// FUNCTION: fn_18934
bool CMagicFloorEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B48F30

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
