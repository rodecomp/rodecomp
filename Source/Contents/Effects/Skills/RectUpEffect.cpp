#include "Contents/Effects/Skills/RectUpEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace {
// INLINED
__forceinline int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}
}

// OFFSET: 0x00709F50
// FUNCTION: fn_07096
CRectUpEffect::CRectUpEffect() : CEffect(1088) {
}

// OFFSET: 0x0070A410
// FUNCTION: fn_07100
void CRectUpEffect::InitEvents() {
  if (this->id == 1088) {
    this->AddEvent(0, "effect\\EF_FireWall.wav", 0, 0);
  }
}

// OFFSET: 0x0070A430
// FUNCTION: fn_07101
bool CRectUpEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0070A440
// FUNCTION: fn_07102
void CRectUpEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0] + particle->offsetX;
    particle->position.y = this->position[1] + particle->offsetY;
    particle->position.z = this->position[2] + particle->offsetZ;

    int& phase = particle->angleParameter;
    phase += phase + 1 < 360 ? 1 : -359;
    if (particle->elapsedFrames > this->particleLifetime) {
      particle->alpha = std::max(particle->alpha - 2, 0);
    }

    float height = particle->speedParameter * g_sinTable[phase] * 0.35f + particle->speedParameter * 0.65f;
    if (particle->elapsedFrames < 90) {
      height *= g_sinTable[particle->elapsedFrames];
    }
    particle->animationParameter = height;
    this->ProcessParticle(particle);

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    ++particle->elapsedFrames;
    particle->transform = rotation;
  }
}

// OFFSET: 0x0070A700
// FUNCTION: fn_07103
void CRectUpEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      const int previous = particle->columnIndex;
      const int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->angleBits)) % 360;
      const float x = particle->scale * g_cosTable[angle];
      const float z = particle->scale * g_sinTable[angle];

      EffectPoint edge[2];
      edge[0].x = x;
      edge[0].y = 0.0f;
      edge[0].z = z;
      edge[1].x = x;
      edge[1].y = -particle->animationParameter;
      edge[1].z = z;

      particle->vertices[1].x = edge[1].x;
      particle->vertices[1].y = edge[1].y;
      particle->vertices[1].z = edge[1].z;
      particle->vertices[3].x = edge[0].x;
      particle->vertices[3].y = edge[0].y;
      particle->vertices[3].z = edge[0].z;
      if (previous) {
        particle->vertices[0].x = this->previousEdge[1].x;
        particle->vertices[0].y = this->previousEdge[1].y;
        particle->vertices[0].z = this->previousEdge[1].z;
        particle->vertices[2].x = this->previousEdge[0].x;
        particle->vertices[2].y = this->previousEdge[0].y;
        particle->vertices[2].z = this->previousEdge[0].z;
      }
      this->previousEdge[0].x = edge[0].x;
      this->previousEdge[0].y = edge[0].y;
      this->previousEdge[0].z = edge[0].z;
      this->previousEdge[1].x = edge[1].x;
      this->previousEdge[1].y = edge[1].y;
      this->previousEdge[1].z = edge[1].z;

      if (previous) {
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      }
    }
  }
}

// OFFSET: 0x0070AA30
// FUNCTION: fn_07104
void CRectUpEffect::ProcessParticle(CParticle* particle) {
  if (particle && this->id == 1088) {
    particle->phaseParameter += particle->phaseParameter + 1 < 360 ? 1 : -359;
    const int base = *reinterpret_cast<int*>(particle->effectParameters);
    particle->SetColor(255, 255, static_cast<short>(g_sinTable[particle->phaseParameter] * 100.0f + static_cast<float>(base)));
  }
}

// OFFSET: 0x0070AAA0
// FUNCTION: fn_07105
void CRectUpEffect::InitParticle(CParticle* particle) {
  if (particle && this->id == 1088) {
    particle->SetTexture("effect\\ring_yellow.tga");
  }
}

// OFFSET: 0x0070A220
// FUNCTION: fn_07098
void CRectUpEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0070A260
// FUNCTION: fn_07099
void CRectUpEffect::Open() {
  if (this->id != 1088) {
    return;
  }

  this->particleLifetime = 1880;
  for (int z = -5; z < 10; z += 5) {
    for (int x = -5; x < 10; x += 5) {
      int index = 0;
      for (int angle = 45; angle <= 405; angle += 90, ++index) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->offsetY = 0.0f;
        particle->offsetX = static_cast<float>(x);
        particle->offsetZ = static_cast<float>(z);
        particle->effectId = this->id;
        particle->columnIndex = index;
        particle->rowIndex = 0;
        particle->groupIndex = 1;
        particle->angleParameter = ClampedRandom(360);
        *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(angle);
        particle->animationParameter = 0.0f;
        if (this->id == 1088) {
          particle->speedParameter = 16.0f;
          particle->alpha = 120;
          particle->scale = 3.5f;
          const int shade = ClampedRandom(50) + 100;
          std::memcpy(particle->effectParameters, &shade, sizeof(shade));
          particle->phaseParameter = 0;
        }
        this->InitParticle(particle);
        InitParticleQuad(particle->vertices);
        this->particles.push_back(particle);
      }
    }
  }
}

