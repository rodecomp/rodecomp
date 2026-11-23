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

// OFFSET: 0x006FC7D0
// FUNCTION: fn_46119
__forceinline void CCloudKillEffect::AddParticle() {
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
      const vector3d origin = {values[3] * 0.0f + values[0] * 0.0f + distance * values[6] + values[9] + static_cast<float>(x),
                               values[4] * 0.0f + values[1] * 0.0f + distance * values[7] + values[10] + 0.0f,
                               values[5] * 0.0f + values[2] * 0.0f + distance * values[8] + values[11] + static_cast<float>(z)};
      particle->originX = origin.x;
      particle->originY = origin.y;
      particle->originZ = origin.z;
      rotation.MakeXRotation(GetRadian(90));
      const float speed = static_cast<float>(ClampedRandom(30) + 30) / 100.0f;
      const float acceleration = -(speed / 40.0f) * g_depthAverageHalf;
      const float scale = static_cast<float>(ClampedRandom(50) + 40) / 170.0f;
      particle->frameDelay = speed;
      particle->acceleration = acceleration;
      particle->scale = scale;
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

// INLINED
static __forceinline void AdvanceSpriteVelocity(CSpriteParticle* particle) {
  const float* values = particle->rotation.values;
  const float speed = particle->acceleration + particle->frameDelay;
  particle->frameDelay = speed;
  const vector3d velocity = {values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9] + particle->velocity.x,
                             values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10] + particle->velocity.y,
                             values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11] + particle->velocity.z};
  particle->velocity = velocity;
}

// OFFSET: 0x006FD440
// FUNCTION: fn_46273
void CCloudKillEffect::UpdateSprite(CSpriteParticle* particle) {
  if (!particle) {
    return;
  }

  const vector3d center = this->positionVector;
  AdvanceSpriteVelocity(particle);
  particle->position.y = center.y + particle->originY + particle->velocity.y;
  particle->position.x = center.x + particle->originX + particle->velocity.x;
  particle->position.z = center.z + particle->originZ + particle->velocity.z;
  particle->angle = particle->spin + particle->angle;
  const float alpha = static_cast<float>(particle->alpha);
  float fade = particle->animationParameter;
  if (particle->elapsedFrames == static_cast<int>(particle->animationParameters[0])) {
    fade = -(alpha / static_cast<float>(this->spriteLifetimeFrames - particle->elapsedFrames));
  }
  particle->animationParameter = fade;
  const float level = alpha + fade;
  const float positive = level < 0.0f ? 0.0f : level;
  particle->alpha = static_cast<int>(positive > 254.0f ? 254.0f : positive);
}

// OFFSET: 0x006FCCD0
// FUNCTION: fn_46120
void CCloudKillEffect::UpdateParticle(CSpriteParticle* particle) {
  if (!particle) {
    return;
  }

  const float* values = particle->rotation.values;
  float& angle = particle->angle;
  const float speed = particle->acceleration + particle->frameDelay;
  particle->frameDelay = speed;
  vector3d velocity = {values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9],
                             values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10],
                             values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11]};
  velocity += particle->velocity;
  particle->velocity = velocity;
  particle->position = vector3d{velocity.x + this->position[0], velocity.y + this->position[1], velocity.z + this->position[2]};
  angle = particle->spin + angle;
  const int age = particle->elapsedFrames;
  float fade = particle->animationParameter;
  const float alpha = static_cast<float>(particle->alpha);
  if (age == static_cast<int>(particle->animationParameters[0])) {
    fade = -(alpha / static_cast<float>(this->particleLifetimeFrames - age));
  }
  particle->animationParameter = fade;
  const float level = fade + alpha;
  const float positive = level < 0.0f ? 0.0f : level;
  particle->alpha = static_cast<int>(positive > 254.0f ? 254.0f : positive);
}

// OFFSET: 0x006FCE60
// FUNCTION: fn_06930
void CCloudKillEffect::OnProcess() {
  if (this->emitting && this->frame % 5 == 0) {
    this->AddParticle();
  }

  for (auto it = this->particles.begin(); it != this->particles.end();) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(*it);
    if (IsParticleActive(particle) != true) {
      FreeParticle(particle);
      it = this->particles.erase(it);
    } else {
      this->UpdateParticle(particle);
      ++particle->elapsedFrames;
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
      this->UpdateSprite(particle);
      ++particle->elapsedFrames;
      ++it;
    }
  }

  if (!this->ActorExists()) {
    this->emitting = 0;
    this->effectLifetime = this->frame + 50;
  }
}

// OFFSET: 0x006FD620
// FUNCTION: fn_46274
__forceinline void CCloudKillEffect::RenderParticle(CParticle* particle) {
  if (particle) {
    static_cast<CSpriteParticle*>(particle)->Draw(1);
  }
}

// OFFSET: 0x006FD690
// FUNCTION: fn_46275
__forceinline void CCloudKillEffect::RenderSprite(CParticle* particle) {
  this->RenderParticle(particle);
}

// OFFSET: 0x006FD640
// FUNCTION: fn_06931
void CCloudKillEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderParticle(*it);
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    this->RenderSprite(*it);
  }
}
