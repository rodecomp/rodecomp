#include "Contents/Effects/Skills/SphereWindEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B77C80
// FUNCTION: fn_46047
CSphereWindEffect::CSphereWindEffect()
    : CEffect(-1), particles() {
}

// OFFSET: 0x00B77D20
// FUNCTION: fn_46048
__declspec(noinline) CSphereWindEffect::~CSphereWindEffect() {
}

// OFFSET: 0x00B781E0
// FUNCTION: fn_19188
void CSphereWindEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// INLINED
static void SetSegment(CParticle* particle, int effectId, int column, int row) {
  particle->columnIndex = column;
  particle->rowIndex = row;
  particle->effectId = effectId;
}

// OFFSET: 0x00B77E00
// FUNCTION: fn_46242
__forceinline void CSphereWindEffect::CreateRings(int start) {
  const int id = this->id;
  for (int ring = 0; ring < 4; ++ring) {
    const int tilt = ring * 45 + start;
    const int radiusRoll = rand() % 11;
    const float radius = static_cast<float>(radiusRoll >= 0 ? radiusRoll : 0) * 0.2f + 4.0f;
    const int rotationRoll = rand() % 360;
    const float rotation = static_cast<float>(rotationRoll >= 0 ? rotationRoll : 0);
    const int spreadRoll = rand() % 91;
    const int spread = (spreadRoll >= 0 ? spreadRoll : 0) + 180;
    const int spinRoll = rand() % 4;
    const float spin = static_cast<float>((spinRoll >= 0 ? spinRoll : 0) + 3);

    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      SetSegment(particle, this->id, segment, ring);
      particle->groupIndex = 1;
      particle->speedParameter = radius;
      particle->scale = 14.0f;
      particle->angle = rotation;
      particle->phaseParameter = tilt;
      particle->alpha = 100;
      particle->angleParameter = spread;
      particle->animationParameter = -14.0f;
      particle->animationParameters[0] = spin;
      particle->InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);

      if (id == 346) {
        particle->SetTexture("effect\\blue_ivy.bmp");
        particle->SetColor(100, 100, 255);
      } else if (id == 394) {
        particle->SetTexture("effect\\fire_ivy.bmp");
        particle->SetColor(255, 100, 100);
      }
    }
  }
}

// OFFSET: 0x00B78220
// FUNCTION: fn_19189
void CSphereWindEffect::Open() {
  for (int start = 0; start < 45; start += 9) {
    this->CreateRings(start);
  }
}

// OFFSET: 0x00B78420
// FUNCTION: fn_19190
bool CSphereWindEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B78430
// FUNCTION: fn_19191
void CSphereWindEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    this->UpdateParticle(particle);
    ++particle->elapsedFrames;
  }
}

// INLINED
static __forceinline void StoreSegment(CSphereWindEffect& effect, CParticle* particle, int segment, const EffectPoint (&points)[4]) {
  particle->vertices[1].x = points[2].x;
  particle->vertices[1].y = points[2].y;
  particle->vertices[1].z = points[2].z;
  particle->vertices[3].x = points[3].x;
  particle->vertices[3].y = points[3].y;
  particle->vertices[3].z = points[3].z;

  if (segment != 0) {
    particle->vertices[0].x = effect.lastEdges[1].x;
    particle->vertices[0].y = effect.lastEdges[1].y;
    particle->vertices[0].z = effect.lastEdges[1].z;
    particle->vertices[2].x = effect.lastEdges[0].x;
    particle->vertices[2].y = effect.lastEdges[0].y;
    particle->vertices[2].z = effect.lastEdges[0].z;
  }

  effect.lastEdges[0].x = points[3].x;
  effect.lastEdges[0].y = points[3].y;
  effect.lastEdges[0].z = points[3].z;
  effect.lastEdges[1].x = points[2].x;
  effect.lastEdges[1].y = points[2].y;
  effect.lastEdges[1].z = points[2].z;
}

// OFFSET: 0x00B77FE0
// FUNCTION: fn_46243
__forceinline void CSphereWindEffect::UpdateVertices(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int segment = particle->columnIndex;
  const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * segment) + particle->angle) % 360;
  EffectPoint points[4];
  const int tilt = particle->phaseParameter;
  const float scale = particle->scale;

  points[0].x = g_cosTable[tilt] * scale * g_cosTable[angle];
  points[0].y = g_sinTable[tilt] * scale * g_cosTable[angle];
  points[0].z = scale * g_sinTable[angle];
  const float radius = particle->speedParameter;
  points[1].x = -g_sinTable[tilt] * radius;
  points[1].y = g_cosTable[tilt] * radius;
  points[1].z = 0.0f;
  const vector3d offset = {0.0f, particle->animationParameter, 0.0f};
  points[3].x = points[0].x + points[1].x + offset.x;
  points[3].y = points[0].y + points[1].y + offset.y;
  points[3].z = points[0].z + points[1].z + offset.z;
  points[2].x = points[0].x - points[1].x + offset.x;
  points[2].y = points[0].y - points[1].y + offset.y;
  points[2].z = points[0].z - points[1].z + offset.z;

  StoreSegment(*this, particle, segment, points);
}

// OFFSET: 0x00B78480
// FUNCTION: fn_19192
__declspec(noinline) void CSphereWindEffect::UpdateParticle(CParticle* particle) {
  float& rotation = particle->angle;
  if (particle) {
    float value = *static_cast<const float*>(particle->animationParameters) + rotation;
    if (value >= 360.0f) {
      value -= 360.0f;
    }
    rotation = value;
  }

  matrix transform;
  std::memcpy(transform.values, g_identityMatrix, sizeof(transform.values));
  transform.MakeYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / -180.0f * 3.141592f);
  particle->transform = transform;

  this->UpdateVertices(particle);
}

// OFFSET: 0x00B787C0
// FUNCTION: fn_46244
__forceinline void CSphereWindEffect::RenderSegment(CParticle* particle) {
  if (!particle || particle->columnIndex <= 0) {
    return;
  }

  const int segment = particle->columnIndex;
  ParticleQuad quad;
  quad.billboard = false;
  quad.unused = false;
  quad.depth = true;
  quad.top = 0.0f;
  quad.bottom = 1.0f;
  quad.left = static_cast<float>(segment - 1) / 21.0f;
  quad.right = static_cast<float>(segment) / 21.0f;
  particle->Render(quad);
}

// OFFSET: 0x00B78710
// FUNCTION: fn_19193
void CSphereWindEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderSegment(*it);
  }
}
