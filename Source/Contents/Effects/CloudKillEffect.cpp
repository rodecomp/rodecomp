#include "Contents/Effects/CloudKillEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x006FC410
// FUNCTION: fn_06923
CCloudKillEffect::CCloudKillEffect() : CEffect(1080) {
  this->unknown_002C = 9999;
  this->unknown_00E4 = 1;
  this->unknown_00DC = 10;
  this->unknown_00E0 = 40;
}

// OFFSET: 0x006FC8F0
// FUNCTION: fn_06926
void CCloudKillEffect::AddSprites() {
  for (int z = -10; z <= 10; z += 4) {
    for (int x = -10; x <= 10; x += 4) {
      CParticle* created = CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1);
      CSpriteParticle* particle = static_cast<CSpriteParticle*>(created);
      matrix& rotation = *reinterpret_cast<matrix*>(particle->unknown_0198);
      rotation.IsIdentity();
      rotation.MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
      const float distance = static_cast<float>(std::max(rand() % 6, 0) + 2);
      const float* values = rotation.values;
      particle->unknown_00DC = values[3] * 0.0f + values[0] * 0.0f + distance * values[6] + values[9] + static_cast<float>(x);
      particle->unknown_00E0 = values[4] * 0.0f + values[1] * 0.0f + distance * values[7] + values[10] + 0.0f;
      particle->unknown_00E4 = values[5] * 0.0f + values[2] * 0.0f + distance * values[8] + values[11] + static_cast<float>(z);
      rotation.MakeXRotation(GetRadian(90));
      const float speed = static_cast<float>(std::max(rand() % 30, 0) + 30) / 100.0f;
      const float acceleration = -(speed / 40.0f) * g_depthAverageHalf;
      const int size = std::max(rand() % 50, 0);
      particle->frameDelay = speed;
      reinterpret_cast<float&>(particle->unknown_02DC) = acceleration;
      particle->scale = static_cast<float>(size + 40) / 170.0f;
      reinterpret_cast<float&>(particle->unknown_02D4) = 3.0f;
      particle->alpha = 0;
      particle->unknown_0054 = 25.4f;
      reinterpret_cast<float&>(particle->unknown_0058) = 32.0f;
      particle->SetSprite("이팩트\\particle3.spr");
      particle->SetAction("이팩트\\particle3.act");
      particle->life = static_cast<float>(this->unknown_00E0) / 60.0f;
      this->sprites.push_back(created);
    }
  }
}

// OFFSET: 0x006FCBE0
// FUNCTION: fn_06927
void CCloudKillEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }
  this->particles.clear();

  for (CParticle* sprite : this->sprites) {
    FreeParticle(sprite);
  }
  this->sprites.clear();
}

// OFFSET: 0x006FCCB0
// FUNCTION: fn_06928
void CCloudKillEffect::Slot01() {
}

// OFFSET: 0x006FCCC0
// FUNCTION: fn_06929
bool CCloudKillEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x006FCE60

// OFFSET: 0x006FD640
// FUNCTION: fn_06931
void CCloudKillEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      static_cast<CSpriteParticle*>(*it)->sub_B40F30(1);
    }
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    if (*it) {
      static_cast<CSpriteParticle*>(*it)->sub_B40F30(1);
    }
  }
}
