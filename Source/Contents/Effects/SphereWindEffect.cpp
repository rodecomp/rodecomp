#include "Contents/Effects/SphereWindEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B781E0
// FUNCTION: fn_19188
void CSphereWindEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B78220
// FUNCTION: fn_19189
void CSphereWindEffect::Slot01() {
  for (int start = 0; start < 45; start += 9) {
    const int id = this->id;
    int tilt = start;
    for (int ring = 0; ring < 4; ++ring) {
      const float radius = static_cast<float>(std::max(rand() % 11, 0)) * 0.2f + 4.0f;
      const int rotation = std::max(rand() % 360, 0);
      const int spread = std::max(rand() % 91, 0) + 180;
      const int spin = std::max(rand() % 4, 0) + 3;

      for (int segment = 0; segment < 21; ++segment) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->unknown_000C = segment;
        particle->unknown_0010 = ring;
        particle->unknown_0004 = this->id;
        particle->unknown_0008 = 1;
        particle->unknown_00C0 = radius;
        particle->scale = 14.0f;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(rotation);
        particle->unknown_004C = tilt;
        particle->alpha = 100;
        particle->unknown_0050 = spread;
        particle->unknown_0054 = -14.0f;
        *reinterpret_cast<float*>(particle->unknown_0058) = static_cast<float>(spin);
        InitParticleQuad(particle->vertices);
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
bool CSphereWindEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B78430
// FUNCTION: fn_19191
void CSphereWindEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    this->UpdateParticle(particle);
    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B78480
// FUNCTION: fn_19192
void CSphereWindEffect::UpdateParticle(CParticle* particle) {
  float& rotation = *reinterpret_cast<float*>(&particle->unknown_00C4);
  if (particle) {
    float value = *reinterpret_cast<const float*>(particle->unknown_0058) + rotation;
    if (value >= 360.0f) {
      value -= 360.0f;
    }
    rotation = value;
  }

  matrix transform;
  std::memcpy(transform.values, g_identityMatrix, sizeof(transform.values));
  transform.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
  particle->transform = transform;

  const int segment = particle->unknown_000C;
  const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * segment) + rotation) % 360;
  const int tilt = particle->unknown_004C;
  const float scale = particle->scale;
  const float radius = particle->unknown_00C0;

  const float ringX = g_cosTable[tilt] * scale * g_cosTable[angle];
  const float ringY = g_sinTable[tilt] * scale * g_cosTable[angle];
  const float ringZ = scale * g_sinTable[angle];
  const float spreadX = g_sinTable[tilt] * radius;
  const float spreadY = g_cosTable[tilt] * radius;

  const vector3d inner = {ringX - spreadX, ringY + spreadY + particle->unknown_0054, ringZ};
  const vector3d outer = {ringX + spreadX, ringY - spreadY + particle->unknown_0054, ringZ};

  particle->vertices[1].x = outer.x;
  particle->vertices[1].y = outer.y;
  particle->vertices[1].z = outer.z;
  particle->vertices[3].x = inner.x;
  particle->vertices[3].y = inner.y;
  particle->vertices[3].z = inner.z;

  if (segment != 0) {
    particle->vertices[0].x = this->lastOuter.x;
    particle->vertices[0].y = this->lastOuter.y;
    particle->vertices[0].z = this->lastOuter.z;
    particle->vertices[2].x = this->lastInner.x;
    particle->vertices[2].y = this->lastInner.y;
    particle->vertices[2].z = this->lastInner.z;
  }

  this->lastInner = inner;
  this->lastOuter = outer;
}

// OFFSET: 0x00B78710
// FUNCTION: fn_19193
void CSphereWindEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->unknown_000C <= 0) {
      continue;
    }

    const int segment = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, false, true};
    particle->Render(quad);
  }
}
