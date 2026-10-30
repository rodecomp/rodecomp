#include "Contents/Effects/MapMagicZoneEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B6E3B0
// FUNCTION: fn_19115
bool CMapMagicZoneEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B6E3C0
// FUNCTION: fn_19116
void CMapMagicZoneEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->spinners.begin(); it != this->spinners.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    ++particle->unknown_00E8;
    this->UpdateSpinner(particle);
  }

  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    ++particle->unknown_00E8;

    vector3d corners[4]{};
    const int angle = particle->unknown_000C * 23;
    particle->unknown_004C = (particle->unknown_004C + 3) % 360;
    const float radius = (g_sinTable[particle->unknown_004C] + 1.0f) * (particle->scale * 0.1f) + particle->scale * 0.8f;
    const float height = particle->unknown_0054;

    for (int corner = 0; corner < 4; ++corner) {
      const int index = (angle + corner * 90) % 360;
      corners[corner].x = radius * g_cosTable[index];
      corners[corner].y = height;
      corners[corner].z = radius * g_sinTable[index];
    }

    particle->vertices[0].x = corners[0].x;
    particle->vertices[0].y = corners[0].y;
    particle->vertices[0].z = corners[0].z;
    particle->vertices[1].x = corners[1].x;
    particle->vertices[1].y = corners[1].y;
    particle->vertices[1].z = corners[1].z;
    particle->vertices[3].x = corners[2].x;
    particle->vertices[3].y = corners[2].y;
    particle->vertices[3].z = corners[2].z;
    particle->vertices[2].x = corners[3].x;
    particle->vertices[2].y = corners[3].y;
    particle->vertices[2].z = corners[3].z;

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;
  }

  for (std::vector<CParticle*>::iterator it = this->ribbon.begin(); it != this->ribbon.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    ++particle->unknown_00E8;
    float& spinAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
    spinAngle = static_cast<float>(static_cast<int>(particle->unknown_0010 + spinAngle + 3.0f));

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;

    const int segment = particle->unknown_000C;
    const float wave = g_sinTable[segment * 9] * particle->unknown_00C0;
    const int step = static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f);
    const int angle = static_cast<int>(static_cast<float>(step * segment) + spinAngle) % 360;
    const float flat = wave * g_cosTable[particle->unknown_004C];
    const float rise = wave * g_sinTable[particle->unknown_004C];

    vector3d points[2]{};
    points[0].x = particle->scale * g_cosTable[angle];
    points[0].z = particle->scale * g_sinTable[angle];
    points[1].x = points[0].x + flat * g_cosTable[angle];
    points[1].y = points[0].y - rise;
    points[1].z = points[0].z + flat * g_sinTable[angle];

    particle->vertices[3].x = points[0].x;
    particle->vertices[3].y = points[0].y;
    particle->vertices[3].z = points[0].z;
    particle->vertices[1].x = points[1].x;
    particle->vertices[1].y = points[1].y;
    particle->vertices[1].z = points[1].z;

    if (segment != 0) {
      particle->vertices[0].x = this->previousOuter.x;
      particle->vertices[0].y = this->previousOuter.y;
      particle->vertices[0].z = this->previousOuter.z;
      particle->vertices[2].x = this->previousInner.x;
      particle->vertices[2].y = this->previousInner.y;
      particle->vertices[2].z = this->previousInner.z;
    }

    this->previousInner = points[0];
    this->previousOuter = points[1];
  }
}

// OFFSET: 0x00B6E8F0
// FUNCTION: fn_19117
void CMapMagicZoneEffect::UpdateSpinner(CParticle* particle) {
  if (particle) {
    vector3d corners[4]{};
    float& spinAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
    const int angle = static_cast<int>(spinAngle + 1.0f);
    spinAngle = static_cast<float>(angle % 360);
    const float radius = (g_sinTable[particle->unknown_004C] + 1.0f) * (particle->scale * 0.1f) + particle->scale * 0.8f;
    const float height = particle->unknown_0054;

    for (int corner = 0; corner < 4; ++corner) {
      const int index = (angle + corner * 90) % 360;
      corners[corner].x = radius * g_cosTable[index];
      corners[corner].y = height;
      corners[corner].z = radius * g_sinTable[index];
    }

    particle->vertices[0].x = corners[0].x;
    particle->vertices[0].y = corners[0].y;
    particle->vertices[0].z = corners[0].z;
    particle->vertices[1].x = corners[1].x;
    particle->vertices[1].y = corners[1].y;
    particle->vertices[1].z = corners[1].z;
    particle->vertices[3].x = corners[2].x;
    particle->vertices[3].y = corners[2].y;
    particle->vertices[3].z = corners[2].z;
    particle->vertices[2].x = corners[3].x;
    particle->vertices[2].y = corners[3].y;
    particle->vertices[2].z = corners[3].z;

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;
  }
}

// OFFSET: 0x00B6F010
// FUNCTION: fn_19118
void CMapMagicZoneEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->spinners.begin(); it != this->spinners.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->destinationBlend = 6;
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->ribbon.begin(); it != this->ribbon.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_000C > 0) {
      const int segment = particle->unknown_000C;
      particle->destinationBlend = 6;
      const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
