#include "Contents/Effects/RectUpEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00709F50
// FUNCTION: fn_07096
CRectUpEffect::CRectUpEffect() : CEffect(1088), unknown_00CC{} {
}

// OFFSET: 0x0070A410
// FUNCTION: fn_07100
void CRectUpEffect::Slot12() {
  if (this->id == 1088) {
    this->AddEvent(0, "effect\\EF_FireWall.wav", 0, 0);
  }
}

// OFFSET: 0x0070A430
// FUNCTION: fn_07101
bool CRectUpEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070A440
// FUNCTION: fn_07102
void CRectUpEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0] + particle->unknown_00D0;
    particle->position.y = this->position[1] + particle->unknown_00D4;
    particle->position.z = this->position[2] + particle->unknown_00D8;

    int& phase = *reinterpret_cast<int*>(particle->unknown_0050);
    phase += phase + 1 < 360 ? 1 : -359;
    if (particle->unknown_00E8 > this->unknown_00E4) {
      particle->alpha = std::max(particle->alpha - 2, 0);
    }

    float height = particle->unknown_00C0 * g_sinTable[phase] * 0.35f + particle->unknown_00C0 * 0.65f;
    if (particle->unknown_00E8 < 90) {
      height *= g_sinTable[particle->unknown_00E8];
    }
    particle->unknown_0054 = height;
    this->Slot34(particle);

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    ++particle->unknown_00E8;
    particle->transform = rotation;
  }
}

// OFFSET: 0x0070A700
// FUNCTION: fn_07103
void CRectUpEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      const int previous = particle->unknown_000C;
      const int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4)) % 360;
      const float x = particle->scale * g_cosTable[angle];
      const float z = particle->scale * g_sinTable[angle];

      vector3d edge[2] = {};
      edge[0].x = x;
      edge[0].y = 0.0f;
      edge[0].z = z;
      edge[1].x = x;
      edge[1].y = -particle->unknown_0054;
      edge[1].z = z;

      particle->vertices[1].x = edge[1].x;
      particle->vertices[1].y = edge[1].y;
      particle->vertices[1].z = edge[1].z;
      particle->vertices[3].x = edge[0].x;
      particle->vertices[3].y = edge[0].y;
      particle->vertices[3].z = edge[0].z;
      if (previous) {
        particle->vertices[0].x = this->unknown_00CC[1].x;
        particle->vertices[0].y = this->unknown_00CC[1].y;
        particle->vertices[0].z = this->unknown_00CC[1].z;
        particle->vertices[2].x = this->unknown_00CC[0].x;
        particle->vertices[2].y = this->unknown_00CC[0].y;
        particle->vertices[2].z = this->unknown_00CC[0].z;
      }
      this->unknown_00CC[0] = edge[0];
      this->unknown_00CC[1] = edge[1];

      if (previous) {
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      }
    }
  }
}

// OFFSET: 0x0070AA30
// FUNCTION: fn_07104
void CRectUpEffect::Slot34(CParticle* particle) {
  if (particle && this->id == 1088) {
    particle->unknown_004C += particle->unknown_004C + 1 < 360 ? 1 : -359;
    const int base = *reinterpret_cast<int*>(particle->unknown_0048);
    particle->SetColor(255, 255, static_cast<short>(g_sinTable[particle->unknown_004C] * 100.0f + static_cast<float>(base)));
  }
}

// OFFSET: 0x0070AAA0
// FUNCTION: fn_07105
void CRectUpEffect::Slot35(CParticle* particle) {
  if (particle && this->id == 1088) {
    particle->SetTexture("effect\\ring_yellow.tga");
  }
}

// OFFSET: 0x0070A220
// FUNCTION: fn_07098
void CRectUpEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }
  this->particles.clear();
}

// OFFSET: 0x0070A260
