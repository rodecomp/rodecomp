#include "Contents/Effects/Skills/SphereWind2Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>

namespace {
// INLINED
__forceinline int Random(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}
}

// OFFSET: 0x00B76DF0
// FUNCTION: fn_46041
CSphereWind2Effect::CSphereWind2Effect() : CEffect(EF_BABY) {
}

// OFFSET: 0x00B76E90
// FUNCTION: fn_46042
__declspec(noinline) CSphereWind2Effect::~CSphereWind2Effect() {
}

// OFFSET: 0x00B77340
// FUNCTION: fn_19182
void CSphereWind2Effect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B76F70
// FUNCTION: fn_46043
__forceinline void CSphereWind2Effect::CreateRings(int baseAngle) {
  for (int ring = 0; ring < 4; ++ring) {
    const int angle = ring * 45 + baseAngle;
    const int delay = -Random(20);
    const float radius = static_cast<float>(Random(11)) * 0.2f + 2.0f;
    const float rotation = static_cast<float>(Random(360));
    const int step = Random(91) + 180;
    const float speed = static_cast<float>(Random(4) + 3);

    for (int part = 0; part < 21; ++part) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        const int effectId = this->id;
        particle->columnIndex = part;
        particle->rowIndex = ring;
        particle->effectId = effectId;
        particle->groupIndex = 1;
        particle->elapsedFrames = delay;
        particle->speedParameter = radius;
        particle->scale = 5.0f;
        particle->angle = rotation;
        particle->phaseParameter = angle;
        particle->alpha = 0;
        particle->angleParameter = step;
        particle->animationParameter = -10.0f;
        particle->animationParameters[0] = speed;
        particle->SetTexture("effect\\fire_ivy.bmp");
        particle->SetColor(255, 89, 182);
        particle->InitParticleQuad(particle->vertices);
        this->particles.push_back(particle);
      }
    }

  }
}

// OFFSET: 0x00B77380
// FUNCTION: fn_19183
void CSphereWind2Effect::Open() {
  for (int base = 0; base < 45; base += 9) {
    this->CreateRings(base);
  }

  CWorld* world = g_modeMgr.GetGameMode()->scene;
  const vector3d& player = world->player->render.position;
  g_soundMgr->PlayWave("effect\\EF_Blessing.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);
  this->effectLifetime = 200;
}

// OFFSET: 0x00B775E0
// FUNCTION: fn_19184
bool CSphereWind2Effect::IsAlive() {
  return !this->IsFinished();
}

// INLINED
static __forceinline void BuildSegmentPoints(const CParticle* particle, EffectPoint (&points)[4], int angle, int pitch, float scale) {
  points[0].x = g_cosTable[angle] * scale * g_cosTable[pitch];
  points[0].y = g_sinTable[angle] * scale * g_cosTable[pitch];
  points[0].z = scale * g_sinTable[pitch];
  const float radius = particle->speedParameter;
  points[1].x = -g_sinTable[angle] * radius;
  points[1].y = g_cosTable[angle] * radius;
  points[1].z = 0.0f;
  const vector3d offset = {0.0f, particle->animationParameter, 0.0f};
  points[3].x = points[0].x + points[1].x + offset.x;
  points[3].y = points[0].y + points[1].y + offset.y;
  points[3].z = points[0].z + points[1].z + offset.z;
  points[2].x = points[0].x - points[1].x + offset.x;
  points[2].y = points[0].y - points[1].y + offset.y;
  points[2].z = points[0].z - points[1].z + offset.z;
}

// INLINED
static __forceinline void StoreSegment(EffectPoint (&lastPoints)[2], CParticle* particle, int texPart, const EffectPoint (&points)[4]) {
  particle->vertices[1].x = points[2].x;
  particle->vertices[1].y = points[2].y;
  particle->vertices[1].z = points[2].z;
  particle->vertices[3].x = points[3].x;
  particle->vertices[3].y = points[3].y;
  particle->vertices[3].z = points[3].z;

  if (texPart != 0) {
    particle->vertices[0].x = lastPoints[1].x;
    particle->vertices[0].y = lastPoints[1].y;
    particle->vertices[0].z = lastPoints[1].z;
    particle->vertices[2].x = lastPoints[0].x;
    particle->vertices[2].y = lastPoints[0].y;
    particle->vertices[2].z = lastPoints[0].z;
  }

  lastPoints[0].x = points[3].x;
  lastPoints[0].y = points[3].y;
  lastPoints[0].z = points[3].z;
  lastPoints[1].x = points[2].x;
  lastPoints[1].y = points[2].y;
  lastPoints[1].z = points[2].z;
}

// OFFSET: 0x00B77140
// FUNCTION: fn_46044
void CSphereWind2Effect::UpdateSegmentVertices(CParticle* particle) {
  if (particle && particle->elapsedFrames > 0) {
    const int texPart = particle->columnIndex;
    const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
    const int pitch = static_cast<int>(static_cast<float>(step * texPart) + particle->angle) % 360;

    EffectPoint points[4];
    const int angle = particle->phaseParameter;
    const float scale = particle->scale;
    BuildSegmentPoints(particle, points, angle, pitch, scale);
    StoreSegment(this->lastPoints, particle, texPart, points);
  }
}

// OFFSET: 0x00B778C0
// FUNCTION: fn_46045
__forceinline void CSphereWind2Effect::UpdateSegment(CParticle* particle) {
  if (particle) {
    if (particle->elapsedFrames > 0) {
      int alpha = particle->alpha;
      if (particle->elapsedFrames <= 25) {
        alpha += 2;
      } else if (particle->elapsedFrames > 70) {
        alpha -= 2;
      }

      particle->alpha = alpha >= 0 ? alpha : 0;
      const float scale = particle->scale * 1.01f;
      particle->scale = scale;

      const int oldAngle = particle->phaseParameter;
      particle->phaseParameter = oldAngle + (oldAngle + 1 < 360 ? 1 : -359);
      const int texPart = particle->columnIndex;

      float& spin = particle->angle;
      float rotation = particle->animationParameters[0] + spin;
      if (rotation >= 360.0f) {
        rotation -= 360.0f;
      }
      spin = rotation;

      const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
      const int pitch = static_cast<int>(static_cast<float>(step * texPart) + particle->angle) % 360;
      const int angle = particle->phaseParameter;
      EffectPoint points[4];
      const float radius = particle->speedParameter;
      points[0].z = scale * g_sinTable[pitch];
      points[0].x = scale * g_cosTable[angle] * g_cosTable[pitch];
      points[0].y = scale * g_sinTable[angle] * g_cosTable[pitch];
      points[1].x = -g_sinTable[angle] * radius;
      points[1].y = radius * g_cosTable[angle];
      points[1].z = 0.0f;
      const vector3d offset = {0.0f, particle->animationParameter, 0.0f};
      points[3].x = points[0].x + points[1].x + offset.x;
      points[3].y = points[0].y + points[1].y + offset.y;
      points[3].z = points[0].z + points[1].z + offset.z;
      points[2].x = points[0].x - points[1].x + offset.x;
      points[2].y = points[0].y - points[1].y + offset.y;
      points[2].z = points[0].z - points[1].z + offset.z;
      StoreSegment(this->lastPoints, particle, texPart, points);
    }
  }
}

// OFFSET: 0x00B775F0
// FUNCTION: fn_19185
void CSphereWind2Effect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    this->UpdateSegment(particle);
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B77BF0
// FUNCTION: fn_46046
__forceinline void CSphereWind2Effect::RenderSegment(CParticle* particle) {
  if (particle && particle->elapsedFrames > 0 && particle->columnIndex > 0) {
    const int texPart = particle->columnIndex;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.left = static_cast<float>(texPart - 1) / 21.0f;
    quad.right = static_cast<float>(texPart) / 21.0f;
    particle->Render(quad);
  }
}

// OFFSET: 0x00B77B40
// FUNCTION: fn_19186
void CSphereWind2Effect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->RenderSegment(particle);
  }
}
