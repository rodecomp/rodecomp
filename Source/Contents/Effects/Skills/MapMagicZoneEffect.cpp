#include "Contents/Effects/Skills/MapMagicZoneEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

struct ZoneVector3d : vector3d {
  // INLINED
  ZoneVector3d() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  }
};

// OFFSET: 0x00B6DCB0
// FUNCTION: fn_19112
void CMapMagicZoneEffect::AddSpinner(int kind) {
  CParticle* particle = CreateParticle(&this->positionVector, 5);
  if (particle) {
    particle->effectId = this->id;
    particle->columnIndex = 0;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    if (kind == 0) {
      particle->SetTexture("effect\\mjin2.tga");
      particle->angle = 0.0f;
      particle->alpha = 200;
      particle->animationParameter = -0.5f;
    } else if (kind == 1) {
      particle->SetTexture("effect\\mjin.tga");
      particle->angle = 45.0f;
      particle->alpha = 50;
      particle->animationParameter = -0.4f;
    }

    particle->scale = 30.0f;
    particle->phaseParameter = 0;
    particle->InitParticleQuad(particle->vertices);
    this->spinners.push_back(particle);
  }
}

// OFFSET: 0x00B6E130
// FUNCTION: fn_19113
void CMapMagicZoneEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->spinners.begin(); it != this->spinners.end(); ++it) {
    FreeParticle(*it);
  }
  this->spinners.clear();

  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    FreeParticle(*it);
  }
  this->petals.clear();

  for (std::vector<CParticle*>::iterator it = this->ribbon.begin(); it != this->ribbon.end(); ++it) {
    FreeParticle(*it);
  }
  this->ribbon.clear();
}

// OFFSET: 0x00B6E1D0
// FUNCTION: fn_19114
void CMapMagicZoneEffect::Open() {
  this->AddSpinner(0);
  this->AddSpinner(1);

  int index = 0;
  for (int angle = 0; angle < 360; angle += 180) {
    CParticle* particle = CreateParticle(&this->positionVector, 5);
    if (particle) {
      particle->effectId = this->id;
      int* indices = &particle->groupIndex;
      indices[1] = index;
      indices[2] = 0;
      indices[0] = 1;
      particle->SetTexture("effect\\pikapika2.bmp");
      particle->scale = 46.0f;
      particle->phaseParameter = angle;
      particle->angle = 0.0f;
      particle->alpha = 25;
      particle->animationParameter = -1.0f;
      particle->InitParticleQuad(particle->vertices);
      this->petals.push_back(particle);
    }

    ++index;
  }

  int row = 0;
  int angle = 0;
  for (int height = 55; height > 40; height -= 5) {
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->SetTexture("effect\\ring_blue.tga");
        const int id = this->id;
        particle->rowIndex = row;
        particle->effectId = id;
        particle->columnIndex = segment;
        particle->groupIndex = 3;
        particle->alpha = 50;
        particle->angle = static_cast<float>(angle);
        particle->angleParameter = 315;
        particle->speedParameter = 15.0f - (static_cast<float>(row) + static_cast<float>(row));
        particle->scale = static_cast<float>(row) * 0.2f + 12.9f;
        particle->phaseParameter = height;
        particle->animationParameter = 0.0f;
        particle->InitParticleQuad(particle->vertices);
        this->ribbon.push_back(particle);
      }
    }

    ++row;
    angle += 90;
  }
}

// OFFSET: 0x00B6E3B0
// FUNCTION: fn_19115
bool CMapMagicZoneEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B6E3C0
// FUNCTION: fn_19116
void CMapMagicZoneEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->spinners.begin(); it != this->spinners.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    ++particle->elapsedFrames;
    this->UpdateSpinner(particle);
  }

  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    ++particle->elapsedFrames;

    ZoneVector3d corners[4];
    const int angle = particle->columnIndex * 23;
    particle->phaseParameter = (particle->phaseParameter + 3) % 360;
    const float radius = (g_sinTable[particle->phaseParameter] + 1.0f) * (particle->scale * 0.1f) + particle->scale * 0.8f;
    const float height = particle->animationParameter;

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
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    ++particle->elapsedFrames;
    float& spinAngle = particle->angle;
    spinAngle = static_cast<float>(static_cast<int>(particle->rowIndex + spinAngle + 3.0f));

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;

    const int segment = particle->columnIndex;
    const float wave = g_sinTable[segment * 9] * particle->speedParameter;
    const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
    const int angle = static_cast<int>(static_cast<float>(step * segment) + spinAngle) % 360;
    const float flat = wave * g_cosTable[particle->phaseParameter];
    const float rise = wave * g_sinTable[particle->phaseParameter];

    const float offsetX = flat * g_cosTable[angle];
    const float offsetZ = flat * g_sinTable[angle];
    ZoneVector3d points[2];
    const float innerX = particle->scale * g_cosTable[angle];
    const float innerY = 0.0f;
    const float innerZ = particle->scale * g_sinTable[angle];
    const float outerX = innerX + offsetX;
    const float outerY = innerY - rise;
    const float outerZ = innerZ + offsetZ;

    particle->vertices[3].x = innerX;
    particle->vertices[3].y = innerY;
    particle->vertices[3].z = innerZ;
    particle->vertices[1].x = outerX;
    particle->vertices[1].y = outerY;
    particle->vertices[1].z = outerZ;

    if (segment != 0) {
      particle->vertices[0].x = this->previousOuter.x;
      particle->vertices[0].y = this->previousOuter.y;
      particle->vertices[0].z = this->previousOuter.z;
      particle->vertices[2].x = this->previousInner.x;
      particle->vertices[2].y = this->previousInner.y;
      particle->vertices[2].z = this->previousInner.z;
    }

    this->previousInner.x = innerX;
    this->previousInner.y = innerY;
    this->previousInner.z = innerZ;
    this->previousOuter.x = outerX;
    this->previousOuter.y = outerY;
    this->previousOuter.z = outerZ;
  }
}

// OFFSET: 0x00B6E8F0
// FUNCTION: fn_19117
void CMapMagicZoneEffect::UpdateSpinner(CParticle* particle) {
  if (particle) {
    ZoneVector3d corners[4];
    float& spinAngle = particle->angle;
    const int angle = static_cast<int>(spinAngle + 1.0f);
    spinAngle = static_cast<float>(angle % 360);
    const float radius = (g_sinTable[particle->phaseParameter] + 1.0f) * (particle->scale * 0.1f) + particle->scale * 0.8f;

    for (int corner = 0; corner < 4; ++corner) {
      const int index = (angle + corner * 90) % 360;
      corners[corner].x = radius * g_cosTable[index];
      corners[corner].y = particle->animationParameter;
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
void CMapMagicZoneEffect::Render() {
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
    if (particle && particle->columnIndex > 0) {
      const int segment = particle->columnIndex;
      particle->destinationBlend = 6;
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
}
