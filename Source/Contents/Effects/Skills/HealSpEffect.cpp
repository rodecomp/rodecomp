#include "Contents/Effects/Skills/HealSpEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <cstdlib>

namespace {
// INLINED
void MatrixMult(vector3d& result, const vector3d& v, const float* m) {
  result.x = v.x * m[0] + v.y * m[3] + v.z * m[6] + m[9];
  result.y = v.x * m[1] + v.y * m[4] + v.z * m[7] + m[10];
  result.z = v.x * m[2] + v.y * m[5] + v.z * m[8] + m[11];
}

// INLINED
__forceinline bool IsParticleActive(const CSpriteParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x00B45140
// FUNCTION: fn_45974
CHealSp::CHealSp() : CEffect(EF_HEALSP) {
  this->effectLifetime = 100;
  this->holdFrame = 80;
}

// OFFSET: 0x00B45200
// FUNCTION: fn_18896
__declspec(noinline) CHealSp::~CHealSp() {
}

// OFFSET: 0x00B45370
// FUNCTION: fn_18898
void CHealSp::AddRing(float inner, float outer, float height, int duration) {
  const float fadeFrame = static_cast<float>(duration) / 30.0f;
  const float holdFrame = static_cast<float>(this->holdFrame);
  for (int segment = 0; segment < 10; ++segment) {
    CParticle* particle = CreateParticle(&this->positionVector, 1);
    particle->effectId = this->id;
    particle->columnIndex = segment;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->speedParameter = 5.0f;
    particle->scale = height;
    particle->animationParameter = inner;
    particle->animationParameters[0] = outer;
    particle->animationParameters[1] = fadeFrame;
    particle->animationParameters[2] = holdFrame;
    particle->animationParameters[3] = static_cast<float>(duration);
    particle->animationParameters[4] = 0.0f;
    particle->animationParameters[5] = 0.0f;
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

// OFFSET: 0x00B457C0
// FUNCTION: fn_45976
__forceinline void CHealSp::UpdateRing(CParticle* particle) {
  float* params = particle->animationValues;
  float fade = params[2];
  const float spin = particle->speedParameter;
  const float endFrame = params[3];
  const float maxAlpha = params[4];
  const float alpha = params[6];
  const float angle = spin + params[5];
  matrix rotation;
  rotation.MakeYRotation(GetRadian(static_cast<int>(angle)));
  const float nextAngle = angle + spin;
  const int limit = static_cast<int>(maxAlpha);
  if (this->frame == static_cast<int>(endFrame)) {
    fade = -(alpha / static_cast<float>(this->effectLifetime - this->frame));
  }

  float level = alpha + fade;
  if (level < 0.0f) {
    level = 0.0f;
  }
  if (level > static_cast<float>(limit)) {
    level = static_cast<float>(limit);
  }
  this->alphaLevel = static_cast<int>(level);
  params[2] = fade;
  params[5] = nextAngle;
  params[6] = level;
  particle->alpha = this->alphaLevel;
  particle->transform = rotation;
}

// OFFSET: 0x00B45ED0
// FUNCTION: fn_45977
__forceinline void CHealSp::UpdateSprite(CSpriteParticle* sprite) {
  float* params = sprite->animationValues;
  const float radius = params[0];
  const float delay = params[4];
  float fade = params[5];
  const float alpha = static_cast<float>(sprite->alpha);
  vector3d velocity = sprite->velocity;
  const vector3d origin = this->positionVector;
  const float lift = params[3] + params[2];
  velocity.y += lift;
  matrix rotation;
  rotation.MakeYRotation(GetRadian(static_cast<int>(params[1])));
  const vector3d offset{0.0f, 0.0f, radius};
  vector3d point;
  MatrixMult(point, offset, rotation.values);
  const float x = point.x + (velocity.x + origin.x);
  const float y = point.y + (velocity.y + origin.y);
  const float z = point.z + (velocity.z + origin.z);
  const int elapsed = sprite->elapsedFrames;
  float level;
  if (this->frame < this->holdFrame) {
    if (elapsed == static_cast<int>(delay)) {
      fade = -(alpha / static_cast<float>(30 - elapsed));
    }

    level = alpha + fade;
    if (level < 0.0f) {
      level = 0.0f;
    }
    if (level > 255.0f) {
      level = 255.0f;
    }
  } else {
    level = static_cast<float>(this->alphaLevel);
  }

  params[5] = fade;
  params[2] = lift;
  sprite->velocity = velocity;
  sprite->position.x = x;
  sprite->position.y = y;
  sprite->position.z = z;
  sprite->alpha = static_cast<int>(level);
}

// OFFSET: 0x00B458E0
// FUNCTION: fn_18903
void CHealSp::OnProcess() {
  if (this->frame % 2 == 0) {
    matrix rotation;
    const int roll = rand() % 360;
    const float angle = static_cast<float>(roll >= 0 ? roll : 0);
    rotation.MakeYRotation(GetRadian(static_cast<int>(angle)));
    const vector3d offset{0.0f, 0.0f, 3.0f};
    vector3d point;
    MatrixMult(point, offset, rotation.values);
    CSpriteParticle* sprite = static_cast<CSpriteParticle*>(CreateSpriteParticle(&this->positionVector, 1));
    float* params = &sprite->animationParameter;
    sprite->life = 0.5f;
    sprite->scale = 0.55f;
    params[0] = 3.0f;
    params[1] = angle;
    params[2] = -1.2f;
    params[3] = 0.02666667f;
    params[4] = 20.0f;
    params[5] = 0.0f;
    sprite->velocity.x = point.x;
    sprite->velocity.y = point.y;
    sprite->velocity.z = point.z;
    sprite->SetSprite("이팩트\\particle2.spr");
    sprite->SetAction("이팩트\\particle2.act");
    sprite->alpha = 254;
    this->sprites.push_back(sprite);
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->UpdateRing(particle);
    ++particle->elapsedFrames;
  }

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end();) {
    CSpriteParticle* sprite = *it;
    if (IsParticleActive(sprite) == true) {
      this->UpdateSprite(sprite);
      ++sprite->elapsedFrames;
      ++it;
    } else {
      FreeParticle(sprite);
      this->sprites.erase(it++);
    }
  }
}

// OFFSET: 0x00B463D0
// FUNCTION: fn_45979
__forceinline void CHealSp::RenderSprite(CSpriteParticle* sprite) {
  sprite->Draw(1);
}

// OFFSET: 0x00B46230
// FUNCTION: fn_18905
void CHealSp::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const int segment = particle->columnIndex;
    const float height = particle->scale;
    const float outer = particle->animationParameters[0];
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
    this->RenderSprite(*it);
  }
}
