#include "Contents/Effects/Skills/HealSpEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B45200
// FUNCTION: fn_18896
CHealSp::~CHealSp() {
}

// OFFSET: 0x00B45370
// FUNCTION: fn_18898
void CHealSp::AddRing(float inner, float outer, float height, int duration) {
  const float fadeFrame = static_cast<float>(duration) / 30.0f;
  const float holdFrame = static_cast<float>(this->holdFrame);
  for (int segment = 0; segment < 10; ++segment) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
    float* params = &particle->animationParameter;
    particle->effectId = this->id;
    particle->columnIndex = segment;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->speedParameter = 5.0f;
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
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    FreeParticle(*it);
  }

  this->sprites.clear();
}

// OFFSET: 0x00B45740
// FUNCTION: fn_18901
void CHealSp::Open() {
  this->AddRing(2.5f, 2.5f, 15.0f, 16);
  this->AddRing(3.5f, 3.5f, 25.0f, 32);
  this->AddRing(4.5f, 4.5f, 35.0f, 64);
}

// OFFSET: 0x00B457B0
// FUNCTION: fn_18902
bool CHealSp::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B458E0
// FUNCTION: fn_18903
void CHealSp::OnProcess() {
  matrix rotation;
  if (this->frame % 2 == 0) {
    const float angle = static_cast<float>(std::max(rand() % 360, 0));
    rotation.MakeYRotation(GetRadian(static_cast<int>(angle)));
    const float* values = rotation.values;
    const float x = values[3] * 0.0f + values[0] * 0.0f + values[6] * 3.0f + values[9];
    const float y = values[4] * 0.0f + values[1] * 0.0f + values[7] * 3.0f + values[10];
    const float z = values[5] * 0.0f + values[2] * 0.0f + values[8] * 3.0f + values[11];
    CSpriteParticle* sprite = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    float* params = &sprite->animationParameter;
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
    float* params = &particle->animationParameter;
    float fade = params[2];
    const float spin = particle->speedParameter;
    const float endFrame = params[3];
    const float maxAlpha = params[4];
    const float alpha = params[6];
    const float angle = spin + params[5];
    rotation.MakeYRotation(GetRadian(static_cast<int>(angle)));
    if (this->frame == static_cast<int>(endFrame)) {
      fade = -(alpha / static_cast<float>(this->effectLifetime - this->frame));
    }

    const float level = std::min(std::max(alpha + fade, 0.0f), static_cast<float>(static_cast<int>(maxAlpha)));
    this->alphaLevel = static_cast<int>(level);
    params[2] = fade;
    params[5] = spin + angle;
    params[6] = level;
    ++particle->elapsedFrames;
    particle->transform = rotation;
    particle->alpha = this->alphaLevel;
  }

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end();) {
    CSpriteParticle* sprite = *it;
    if (sprite->life == 0.0f || static_cast<float>(sprite->elapsedFrames) < sprite->life * 60.0f) {
      float* params = &sprite->animationParameter;
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
      if (this->frame < this->holdFrame) {
        if (sprite->elapsedFrames == static_cast<int>(delay)) {
          fade = -(alpha / static_cast<float>(30 - sprite->elapsedFrames));
        }

        level = std::min(std::max(alpha + fade, 0.0f), 255.0f);
      } else {
        level = static_cast<float>(this->alphaLevel);
      }

      params[2] = lift;
      sprite->velocity = velocity;
      sprite->alpha = static_cast<int>(level);
      params[5] = fade;
      sprite->position.x = x;
      sprite->position.y = y;
      sprite->position.z = z;
      ++sprite->elapsedFrames;
      ++it;
    } else {
      FreeParticle(sprite);
      it = this->sprites.erase(it);
    }
  }
}

// OFFSET: 0x00B46230
// FUNCTION: fn_18905
void CHealSp::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const int segment = particle->columnIndex;
    const float height = particle->scale;
    const float outer = *reinterpret_cast<const float*>(particle->animationParameters);
    const float inner = particle->animationParameter;
    const float startCos = g_cosTable[segment * 36];
    const float startSin = g_sinTable[segment * 36];
    const float endCos = g_cosTable[segment * 36 + 36];
    const float endSin = g_sinTable[segment * 36 + 36];

    RenderVertex* vertices = particle->vertices;
    const vector3d outerStart = {startCos * outer, 0.0f, startSin * outer};
    vertices[0].x = outerStart.x;
    vertices[0].y = outerStart.y;
    vertices[0].z = outerStart.z;
    const vector3d outerEnd = {endCos * outer, 0.0f, endSin * outer};
    vertices[1].x = outerEnd.x;
    vertices[1].y = outerEnd.y;
    vertices[1].z = outerEnd.z;
    const vector3d innerStart = {startCos * inner, -height, startSin * inner};
    vertices[2].x = innerStart.x;
    vertices[2].y = innerStart.y;
    vertices[2].z = innerStart.z;
    const vector3d innerEnd = {endCos * inner, -height, endSin * inner};
    vertices[3].x = innerEnd.x;
    vertices[3].y = innerEnd.y;
    vertices[3].z = innerEnd.z;

    const float left = static_cast<float>(particle->columnIndex % 4) * 0.25f;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 1.0f;
    quad.bottom = 0.0f;
    quad.left = left;
    quad.right = left + 0.25f;
    particle->Render(quad);
  }

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    (*it)->Draw(1);
  }
}
