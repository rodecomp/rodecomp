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

// OFFSET: 0x00B781E0
// FUNCTION: fn_19188
void CSphereWindEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B78220
// FUNCTION: fn_19189
void CSphereWindEffect::Open() {
  for (int start = 0; start < 45; start += 9) {
    const int id = this->id;
    int tilt = start;
    for (int ring = 0; ring < 4; ++ring) {
      const int radiusRoll = rand() % 11;
      const float radius = static_cast<float>(radiusRoll >= 0 ? radiusRoll : 0) * 0.2f + 4.0f;
      const int rotationRoll = rand() % 360;
      const int rotation = rotationRoll >= 0 ? rotationRoll : 0;
      const int spreadRoll = rand() % 91;
      const int spread = (spreadRoll >= 0 ? spreadRoll : 0) + 180;
      const int spinRoll = rand() % 4;
      const int spin = (spinRoll >= 0 ? spinRoll : 0) + 3;

      for (int segment = 0; segment < 21; ++segment) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->columnIndex = segment;
        particle->rowIndex = ring;
        particle->effectId = this->id;
        particle->groupIndex = 1;
        particle->speedParameter = radius;
        particle->scale = 14.0f;
        particle->angle = static_cast<float>(rotation);
        particle->phaseParameter = tilt;
        particle->alpha = 100;
        particle->angleParameter = spread;
        particle->animationParameter = -14.0f;
        particle->animationParameters[0] = static_cast<float>(spin);
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

      tilt += 45;
    }
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
  transform.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
  particle->transform = transform;

  const int segment = particle->columnIndex;
  const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * segment) + rotation) % 360;
  const int tilt = particle->phaseParameter;
  const float scale = particle->scale;
  const float radius = particle->speedParameter;

  EffectPoint points[4];
  points[0].x = g_cosTable[tilt] * scale * g_cosTable[angle];
  points[0].y = g_sinTable[tilt] * scale * g_cosTable[angle];
  points[0].z = scale * g_sinTable[angle];
  points[1].x = -g_sinTable[tilt] * radius;
  points[1].y = g_cosTable[tilt] * radius;
  points[1].z = 0.0f;
  const vector3d offset = {0.0f, particle->animationParameter, 0.0f};
  points[2].x = points[0].x - points[1].x + offset.x;
  points[2].y = points[0].y - points[1].y + offset.y;
  points[2].z = points[0].z - points[1].z + offset.z;
  points[3].x = points[0].x + points[1].x + offset.x;
  points[3].y = points[0].y + points[1].y + offset.y;
  points[3].z = points[0].z + points[1].z + offset.z;

  particle->vertices[1].x = points[2].x;
  particle->vertices[1].y = points[2].y;
  particle->vertices[1].z = points[2].z;
  particle->vertices[3].x = points[3].x;
  particle->vertices[3].y = points[3].y;
  particle->vertices[3].z = points[3].z;

  if (segment != 0) {
    particle->vertices[0].x = this->lastOuter.x;
    particle->vertices[0].y = this->lastOuter.y;
    particle->vertices[0].z = this->lastOuter.z;
    particle->vertices[2].x = this->lastInner.x;
    particle->vertices[2].y = this->lastInner.y;
    particle->vertices[2].z = this->lastInner.z;
  }

  this->lastInner.x = points[3].x;
  this->lastInner.y = points[3].y;
  this->lastInner.z = points[3].z;
  this->lastOuter.x = points[2].x;
  this->lastOuter.y = points[2].y;
  this->lastOuter.z = points[2].z;
}

// OFFSET: 0x00B78710
// FUNCTION: fn_19193
void CSphereWindEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->columnIndex <= 0) {
      continue;
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
}
