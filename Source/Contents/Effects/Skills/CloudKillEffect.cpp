#include "Contents/Effects/Skills/CloudKillEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

namespace {
// INLINED
int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}

// INLINED
__forceinline bool IsParticleActive(const CSpriteParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x006FC410
// FUNCTION: fn_06923
CCloudKillEffect::CCloudKillEffect() : CEffect(1080) {
  this->effectLifetime = 9999;
  this->emitting = 1;
  this->particleLifetimeFrames = 10;
  this->spriteLifetimeFrames = 40;
}

// OFFSET: 0x006FC600
// FUNCTION: fn_06924
__declspec(noinline) CCloudKillEffect::~CCloudKillEffect() {
}

// OFFSET: 0x006FC8F0
// FUNCTION: fn_06926
void CCloudKillEffect::AddSprites() {
  for (int z = -10; z <= 10; z += 4) {
    for (int x = -10; x <= 10; x += 4) {
      CParticle* created = CreateSpriteParticle(&this->positionVector, 1);
      CSpriteParticle* particle = static_cast<CSpriteParticle*>(created);
      matrix& rotation = particle->rotation;
      rotation.IsIdentity();
      rotation.MakeYRotation(GetRadian(ClampedRandom(360)));
      const float distance = static_cast<float>(ClampedRandom(6) + 2);
      const float* values = rotation.values;
      particle->originX = values[3] * 0.0f + values[0] * 0.0f + distance * values[6] + values[9] + static_cast<float>(x);
      particle->originY = values[4] * 0.0f + values[1] * 0.0f + distance * values[7] + values[10] + 0.0f;
      particle->originZ = values[5] * 0.0f + values[2] * 0.0f + distance * values[8] + values[11] + static_cast<float>(z);
      rotation.MakeXRotation(GetRadian(90));
      const float speed = static_cast<float>(ClampedRandom(30) + 30) / 100.0f;
      const float acceleration = -(speed / 40.0f) * g_depthAverageHalf;
      const int size = ClampedRandom(50);
      particle->frameDelay = speed;
      particle->acceleration = acceleration;
      particle->scale = static_cast<float>(size + 40) / 170.0f;
      particle->spin = 3.0f;
      particle->alpha = 0;
      particle->animationParameter = 25.4f;
      particle->animationParameters[0] = 32.0f;
      particle->SetSprite("이팩트\\particle3.spr");
      particle->SetAction("이팩트\\particle3.act");
      particle->life = static_cast<float>(this->spriteLifetimeFrames) / 60.0f;
      this->sprites.push_back(created);
    }
  }
}

// OFFSET: 0x006FCBE0
// FUNCTION: fn_06927
void CCloudKillEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    FreeParticle(*it);
  }
  this->sprites.clear();
}

// OFFSET: 0x006FCCB0
// FUNCTION: fn_06928
void CCloudKillEffect::Open() {
}

// OFFSET: 0x006FCCC0
// FUNCTION: fn_06929
bool CCloudKillEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x006FCE60
// FUNCTION: fn_06930
void CCloudKillEffect::OnProcess() {
  if (this->emitting && this->frame % 5 == 0) {
    CParticle* created = CreateSpriteParticle(&this->positionVector, 1);
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(created);
    matrix& rotation = particle->rotation;
    rotation.IsIdentity();
    rotation.MakeYRotation(GetRadian(ClampedRandom(360)));
    rotation.MakeXRotation(GetRadian(90));
    const float speed = static_cast<float>(ClampedRandom(20) + 20) / 80.0f;
    particle->frameDelay = speed;
    particle->acceleration = -(speed / 10.0f) * g_depthAverageHalf;
    particle->scale = 0.8f;
    particle->spin = 3.0f;
    particle->alpha = 70;
    particle->animationParameter = 25.4f;
    particle->animationParameters[0] = 8.0f;
    particle->SetSprite("이팩트\\particle3.spr");
    particle->SetAction("이팩트\\particle3.act");
    particle->life = static_cast<float>(this->particleLifetimeFrames) / 60.0f;
    this->particles.push_back(created);
  }

  for (auto it = this->particles.begin(); it != this->particles.end();) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(*it);
    if (IsParticleActive(particle) != true) {
      FreeParticle(particle);
      it = this->particles.erase(it);
    } else {
      const float* values = particle->rotation.values;
      float& angle = particle->angle;
      const float speed = particle->acceleration + particle->frameDelay;
      particle->frameDelay = speed;
      const float x = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9] + particle->velocity.x;
      const float y = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10] + particle->velocity.y;
      const float z = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11] + particle->velocity.z;
      particle->velocity.x = x;
      particle->velocity.y = y;
      particle->velocity.z = z;
      angle = particle->spin + angle;
      particle->position.z = particle->velocity.z + this->position[2];
      particle->position.x = particle->velocity.x + this->position[0];
      particle->position.y = particle->velocity.y + this->position[1];
      const int age = particle->elapsedFrames;
      float fade = particle->animationParameter;
      if (age == static_cast<int>(particle->animationParameters[0])) {
        fade = -(static_cast<float>(particle->alpha) / static_cast<float>(this->particleLifetimeFrames - age));
      }
      particle->animationParameter = fade;
      const float level = fade + static_cast<float>(particle->alpha);
      const float positive = 0.0f <= level ? level : 0.0f;
      particle->alpha = static_cast<int>(positive <= 254.0f ? positive : 254.0f);
      particle->elapsedFrames = age + 1;
      ++it;
    }
  }

  if (this->emitting && this->frame % 9 == 0) {
    this->AddSprites();
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end();) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(*it);
    if (IsParticleActive(particle) != true) {
      FreeParticle(particle);
      it = this->sprites.erase(it);
    } else {
      const float* values = particle->rotation.values;
      float& angle = particle->angle;
      const float speed = particle->acceleration + particle->frameDelay;
      particle->frameDelay = speed;
      const float x = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9] + particle->velocity.x;
      const float y = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10] + particle->velocity.y;
      const float z = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11] + particle->velocity.z;
      particle->velocity.x = x;
      particle->velocity.y = y;
      particle->velocity.z = z;
      particle->position.y = particle->originY + this->position[1] + particle->velocity.y;
      particle->position.x = particle->originX + this->position[0] + particle->velocity.x;
      particle->position.z = this->position[2] + particle->originZ + particle->velocity.z;
      angle = particle->spin + angle;
      const int age = particle->elapsedFrames;
      float fade = particle->animationParameter;
      if (age == static_cast<int>(particle->animationParameters[0])) {
        fade = -(static_cast<float>(particle->alpha) / static_cast<float>(this->spriteLifetimeFrames - age));
      }
      particle->animationParameter = fade;
      const float level = fade + static_cast<float>(particle->alpha);
      const float positive = 0.0f <= level ? level : 0.0f;
      particle->alpha = static_cast<int>(positive <= 254.0f ? positive : 254.0f);
      particle->elapsedFrames = age + 1;
      ++it;
    }
  }

  if (!this->ActorExists()) {
    this->emitting = 0;
    this->effectLifetime = this->frame + 50;
  }
}

// OFFSET: 0x006FD640
// FUNCTION: fn_06931
void CCloudKillEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      static_cast<CSpriteParticle*>(*it)->Draw(1);
    }
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    if (*it) {
      static_cast<CSpriteParticle*>(*it)->Draw(1);
    }
  }
}
