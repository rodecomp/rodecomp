#include "Contents/Effects/HealSpEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B45370
// FUNCTION: fn_18898
void CHealSp::AddRing(float inner, float outer, float height, int duration) {
  const float fadeFrame = static_cast<float>(duration) / 30.0f;
  const float holdFrame = static_cast<float>(this->unknown_00E0);
  for (int segment = 0; segment < 10; ++segment) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
    float* params = &particle->unknown_0054;
    particle->unknown_0004 = this->id;
    particle->unknown_000C = segment;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->unknown_00C0 = 5.0f;
    particle->scale = height;
    params[0] = inner;
    params[1] = outer;
    params[2] = fadeFrame;
    params[3] = holdFrame;
    params[4] = static_cast<float>(duration);
    params[5] = 0.0f;
    params[6] = 0.0f;
    particle->SetColor(32, 176, 232);
    particle->SetTexture("effect\\alpha_down.tga");
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00B456A0
// FUNCTION: fn_18900
void CHealSp::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();

  for (CSpriteParticle* sprite : this->sprites) {
    FreeParticle(sprite);
  }

  this->sprites.clear();
}

// OFFSET: 0x00B45740
// FUNCTION: fn_18901
void CHealSp::Slot01() {
  this->AddRing(2.5f, 2.5f, 15.0f, 16);
  this->AddRing(3.5f, 3.5f, 25.0f, 32);
  this->AddRing(4.5f, 4.5f, 35.0f, 64);
}

// OFFSET: 0x00B457B0
// FUNCTION: fn_18902
bool CHealSp::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B458E0
// FUNCTION: fn_18903
void CHealSp::Slot03() {
  matrix rotation;
  if (this->frame % 2 == 0) {
    const float angle = static_cast<float>(std::max(rand() % 360, 0));
    rotation.MakeYRotation(GetRadian(static_cast<int>(angle)));
    const float* values = rotation.values;
    const float x = values[3] * 0.0f + values[0] * 0.0f + values[6] * 3.0f + values[9];
    const float y = values[4] * 0.0f + values[1] * 0.0f + values[7] * 3.0f + values[10];
    const float z = values[5] * 0.0f + values[2] * 0.0f + values[8] * 3.0f + values[11];
    CSpriteParticle* sprite = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    float* params = &sprite->unknown_0054;
    sprite->life = 0.5f;
    sprite->scale = 0.55f;
    params[0] = 3.0f;
    params[1] = angle;
    params[2] = -1.2f;
    params[3] = 0.02666667f;
    params[4] = 20.0f;
    params[5] = 0.0f;
    sprite->velocity.x = x;
    sprite->velocity.y = y;
    sprite->velocity.z = z;
    sprite->SetSprite("이팩트\\particle2.spr");
    sprite->SetAction("이팩트\\particle2.act");
    sprite->alpha = 254;
    this->sprites.push_back(sprite);
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    float* params = &particle->unknown_0054;
    float fade = params[2];
    const float spin = particle->unknown_00C0;
    const float endFrame = params[3];
    const float maxAlpha = params[4];
    const float alpha = params[6];
    const float angle = spin + params[5];
    rotation.MakeYRotation(GetRadian(static_cast<int>(angle)));
    if (this->frame == static_cast<int>(endFrame)) {
      fade = -(alpha / static_cast<float>(this->unknown_002C - this->frame));
    }

    const float level = std::min(std::max(alpha + fade, 0.0f), static_cast<float>(static_cast<int>(maxAlpha)));
    this->unknown_00E4 = static_cast<int>(level);
    params[2] = fade;
    params[5] = spin + angle;
    params[6] = level;
    ++particle->unknown_00E8;
    particle->transform = rotation;
    particle->alpha = this->unknown_00E4;
  }

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end();) {
    CSpriteParticle* sprite = *it;
    if (sprite->life == 0.0f || static_cast<float>(sprite->unknown_00E8) < sprite->life * 60.0f) {
      float* params = &sprite->unknown_0054;
      const float radius = params[0];
      const float delay = params[4];
      float fade = params[5];
      const float alpha = static_cast<float>(sprite->alpha);
      const vector3d origin = *reinterpret_cast<const vector3d*>(this->position);
      vector3d velocity = sprite->velocity;
      const float lift = params[3] + params[2];
      velocity.y += lift;
      rotation.MakeYRotation(GetRadian(static_cast<int>(params[1])));
      const float* values = rotation.values;
      const float x = values[3] * 0.0f + values[0] * 0.0f + values[6] * radius + values[9] + (origin.x + velocity.x);
      const float y = values[4] * 0.0f + values[1] * 0.0f + values[7] * radius + values[10] + (origin.y + velocity.y);
      const float z = values[5] * 0.0f + values[2] * 0.0f + values[8] * radius + values[11] + (origin.z + velocity.z);
      float level;
      if (this->frame < this->unknown_00E0) {
        if (sprite->unknown_00E8 == static_cast<int>(delay)) {
          fade = -(alpha / static_cast<float>(30 - sprite->unknown_00E8));
        }

        level = std::min(std::max(alpha + fade, 0.0f), 255.0f);
      } else {
        level = static_cast<float>(this->unknown_00E4);
      }

      params[2] = lift;
      sprite->velocity = velocity;
      sprite->alpha = static_cast<int>(level);
      params[5] = fade;
      sprite->position.x = x;
      sprite->position.y = y;
      sprite->position.z = z;
      ++sprite->unknown_00E8;
      ++it;
    } else {
      FreeParticle(sprite);
      it = this->sprites.erase(it);
    }
  }
}

// OFFSET: 0x00B46230
// FUNCTION: fn_18905
void CHealSp::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const int segment = particle->unknown_000C;
    const float height = particle->scale;
    const float outer = *reinterpret_cast<const float*>(particle->unknown_0058);
    const float inner = particle->unknown_0054;
    const float startCos = g_cosTable[segment * 36];
    const float startSin = g_sinTable[segment * 36];
    const float endCos = g_cosTable[segment * 36 + 36];
    const float endSin = g_sinTable[segment * 36 + 36];

    particle->vertices[0].x = startCos * outer;
    particle->vertices[0].y = 0.0f;
    particle->vertices[0].z = startSin * outer;
    particle->vertices[1].x = endCos * outer;
    particle->vertices[1].y = 0.0f;
    particle->vertices[1].z = endSin * outer;
    particle->vertices[2].x = startCos * inner;
    particle->vertices[2].y = -height;
    particle->vertices[2].z = startSin * inner;
    particle->vertices[3].x = endCos * inner;
    particle->vertices[3].y = -height;
    particle->vertices[3].z = endSin * inner;

    const float left = static_cast<float>(particle->unknown_000C % 4) * 0.25f;
    const ParticleQuad quad = {false, 1.0f, 0.0f, left, left + 0.25f, false, true};
    particle->Render(quad);
  }

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    (*it)->sub_B40F30(1);
  }
}
