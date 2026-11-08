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
  this->effectLifetime = 9999;
  this->emitting = 1;
  this->particleLifetimeFrames = 10;
  this->spriteLifetimeFrames = 40;
}

// OFFSET: 0x006FC600
// FUNCTION: fn_06924
CCloudKillEffect::~CCloudKillEffect() {
}

// OFFSET: 0x006FC8F0
// FUNCTION: fn_06926
void CCloudKillEffect::AddSprites() {
  for (int z = -10; z <= 10; z += 4) {
    for (int x = -10; x <= 10; x += 4) {
      CParticle* created = CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1);
      CSpriteParticle* particle = static_cast<CSpriteParticle*>(created);
      matrix& rotation = *reinterpret_cast<matrix*>(particle->rotationStorage);
      rotation.IsIdentity();
      rotation.MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
      const float distance = static_cast<float>(std::max(rand() % 6, 0) + 2);
      const float* values = rotation.values;
      particle->originX = values[3] * 0.0f + values[0] * 0.0f + distance * values[6] + values[9] + static_cast<float>(x);
      particle->originY = values[4] * 0.0f + values[1] * 0.0f + distance * values[7] + values[10] + 0.0f;
      particle->originZ = values[5] * 0.0f + values[2] * 0.0f + distance * values[8] + values[11] + static_cast<float>(z);
      rotation.MakeXRotation(GetRadian(90));
      const float speed = static_cast<float>(std::max(rand() % 30, 0) + 30) / 100.0f;
      const float acceleration = -(speed / 40.0f) * g_depthAverageHalf;
      const int size = std::max(rand() % 50, 0);
      particle->frameDelay = speed;
      reinterpret_cast<float&>(particle->accelerationStorage) = acceleration;
      particle->scale = static_cast<float>(size + 40) / 170.0f;
      reinterpret_cast<float&>(particle->spinBits) = 3.0f;
      particle->alpha = 0;
      particle->animationParameter = 25.4f;
      reinterpret_cast<float&>(particle->animationParameters) = 32.0f;
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
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    matrix& rotation = *reinterpret_cast<matrix*>(particle->rotationStorage);
    rotation.IsIdentity();
    rotation.MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    rotation.MakeXRotation(GetRadian(90));
    const float speed = static_cast<float>(std::max(rand() % 20, 0) + 20) / 80.0f;
    particle->frameDelay = speed;
    reinterpret_cast<float&>(particle->accelerationStorage) = -(speed / 10.0f) * g_depthAverageHalf;
    particle->scale = 0.8f;
    reinterpret_cast<float&>(particle->spinBits) = 3.0f;
    particle->alpha = 70;
    particle->animationParameter = 25.4f;
    reinterpret_cast<float&>(particle->animationParameters) = 8.0f;
    particle->SetSprite("이팩트\\particle3.spr");
    particle->SetAction("이팩트\\particle3.act");
    particle->life = static_cast<float>(this->particleLifetimeFrames) / 60.0f;
    this->particles.push_back(static_cast<CParticle*>(particle));
  }

  for (auto it = this->particles.begin(); it != this->particles.end();) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(*it);
    if (particle->life == 0.0f || particle->life * 60.0f > static_cast<float>(particle->elapsedFrames)) {
      const float* values = reinterpret_cast<matrix*>(particle->rotationStorage)->values;
      float& angle = reinterpret_cast<float&>(particle->angleBits);
      const float speed = reinterpret_cast<float&>(particle->accelerationStorage) + particle->frameDelay;
      particle->frameDelay = speed;
      particle->velocity.x = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9] + particle->velocity.x;
      particle->velocity.y = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10] + particle->velocity.y;
      particle->velocity.z = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11] + particle->velocity.z;
      angle = reinterpret_cast<float&>(particle->spinBits) + angle;
      particle->position.z = particle->velocity.z + this->position[2];
      particle->position.x = particle->velocity.x + this->position[0];
      particle->position.y = particle->velocity.y + this->position[1];
      const int age = particle->elapsedFrames;
      float fade = particle->animationParameter;
      if (age == static_cast<int>(reinterpret_cast<float&>(particle->animationParameters))) {
        fade = -(static_cast<float>(particle->alpha) / static_cast<float>(this->particleLifetimeFrames - age));
      }
      particle->animationParameter = fade;
      const float level = fade + static_cast<float>(particle->alpha);
      const float positive = 0.0f <= level ? level : 0.0f;
      particle->alpha = static_cast<int>(positive <= 254.0f ? positive : 254.0f);
      particle->elapsedFrames = age + 1;
      ++it;
    } else {
      FreeParticle(particle);
      it = this->particles.erase(it);
    }
  }

  if (this->emitting && this->frame % 9 == 0) {
    this->AddSprites();
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end();) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(*it);
    if (particle->life == 0.0f || particle->life * 60.0f > static_cast<float>(particle->elapsedFrames)) {
      const float* values = reinterpret_cast<matrix*>(particle->rotationStorage)->values;
      float& angle = reinterpret_cast<float&>(particle->angleBits);
      const float speed = reinterpret_cast<float&>(particle->accelerationStorage) + particle->frameDelay;
      particle->frameDelay = speed;
      particle->velocity.x = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9] + particle->velocity.x;
      particle->velocity.y = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10] + particle->velocity.y;
      particle->velocity.z = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11] + particle->velocity.z;
      particle->position.y = particle->originY + this->position[1] + particle->velocity.y;
      particle->position.x = particle->originX + this->position[0] + particle->velocity.x;
      particle->position.z = this->position[2] + particle->originZ + particle->velocity.z;
      angle = reinterpret_cast<float&>(particle->spinBits) + angle;
      const int age = particle->elapsedFrames;
      float fade = particle->animationParameter;
      if (age == static_cast<int>(reinterpret_cast<float&>(particle->animationParameters))) {
        fade = -(static_cast<float>(particle->alpha) / static_cast<float>(this->spriteLifetimeFrames - age));
      }
      particle->animationParameter = fade;
      const float level = fade + static_cast<float>(particle->alpha);
      const float positive = 0.0f <= level ? level : 0.0f;
      particle->alpha = static_cast<int>(positive <= 254.0f ? positive : 254.0f);
      particle->elapsedFrames = age + 1;
      ++it;
    } else {
      FreeParticle(particle);
      it = this->sprites.erase(it);
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
