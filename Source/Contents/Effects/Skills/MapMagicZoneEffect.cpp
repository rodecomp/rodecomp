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

// INLINED
static inline void SetIdentity(CParticle* particle, int id, int group, int column, int row) {
  particle->effectId = id;
  particle->rowIndex = row;
  particle->columnIndex = column;
  particle->groupIndex = group;
}

// OFFSET: 0x00B6DAC0
// FUNCTION: fn_46117
CMapMagicZoneEffect::CMapMagicZoneEffect() : CEffect(0x28A), spinners(), petals(), ribbon() {
}

// OFFSET: 0x00B6DB90
// FUNCTION: fn_46118
__declspec(noinline) CMapMagicZoneEffect::~CMapMagicZoneEffect() {
}

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

// OFFSET: 0x00B6DDA0
// FUNCTION: fn_46225
__forceinline void CMapMagicZoneEffect::AddPetals() {
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 5);
    if (particle) {
      particle->effectId = this->id;
      int* indices = &particle->groupIndex;
      indices[1] = index;
      indices[2] = 0;
      indices[0] = 1;
      particle->SetTexture("effect\\pikapika2.bmp");
      particle->scale = 46.0f;
      particle->phaseParameter = index * 180;
      particle->angle = 0.0f;
      particle->alpha = 25;
      particle->animationParameter = -1.0f;
      particle->InitParticleQuad(particle->vertices);
      this->petals.push_back(particle);
    }
  }
}

// OFFSET: 0x00B6DE70
// FUNCTION: fn_46226
__forceinline void CMapMagicZoneEffect::AddRibbon() {
  for (int row = 0; row < 3; ++row) {
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->SetTexture("effect\\ring_blue.tga");
        SetIdentity(particle, this->id, 3, segment, row);
        particle->alpha = 50;
        particle->angle = static_cast<float>(row * 90);
        particle->angleParameter = 315;
        particle->speedParameter = 15.0f - (static_cast<float>(row) + static_cast<float>(row));
        particle->scale = static_cast<float>(row) * 0.2f + 12.9f;
        particle->phaseParameter = 55 - row * 5;
        particle->animationParameter = 0.0f;
        particle->InitParticleQuad(particle->vertices);
        this->ribbon.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B6E1D0
// FUNCTION: fn_19114
void CMapMagicZoneEffect::Open() {
  this->AddSpinner(0);
  this->AddSpinner(1);
  this->AddPetals();
  this->AddRibbon();
}

// OFFSET: 0x00B6E3B0
// FUNCTION: fn_19115
bool CMapMagicZoneEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B6EB70
// FUNCTION: fn_46228
__forceinline void CMapMagicZoneEffect::UpdatePetal(CParticle* particle) {
  if (particle) {
    ZoneVector3d corners[4];
    const int angle = particle->columnIndex * 23;
    particle->phaseParameter = (particle->phaseParameter + 3) % 360;

    for (int corner = 0; corner < 4; ++corner) {
      const int index = (angle + corner * 90) % 360;
      const float radius = (g_sinTable[particle->phaseParameter] + 1.0f) * (particle->scale * 0.1f) + particle->scale * 0.8f;
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
    rotation.MakeYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;
  }
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

    this->UpdatePetal(particle);
  }

  for (std::vector<CParticle*>::iterator it = this->ribbon.begin(); it != this->ribbon.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    ++particle->elapsedFrames;
    this->UpdateRibbon(particle);
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
    const float scale = particle->scale;
    const int phase = particle->phaseParameter;

    for (int corner = 0; corner < 4; ++corner) {
      const int index = (angle + corner * 90) % 360;
      const float radius = (g_sinTable[phase] + 1.0f) * (scale * 0.1f) + scale * 0.8f;
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
    rotation.MakeYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;
  }
}

// OFFSET: 0x00B6DF90
// FUNCTION: fn_46227
__forceinline void CMapMagicZoneEffect::UpdateRibbonVertices(CParticle* particle) {
  if (particle) {
    const int segment = particle->columnIndex;
    const float wave = g_sinTable[segment * 9] * particle->speedParameter;
    const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
    const int angle = static_cast<int>(static_cast<float>(step * segment) + particle->angle) % 360;
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

    points[0].x = innerX;
    points[0].y = innerY;
    points[0].z = innerZ;
    points[1].x = outerX;
    points[1].y = outerY;
    points[1].z = outerZ;

    particle->vertices[3].x = points[0].x;
    particle->vertices[3].y = points[0].y;
    particle->vertices[3].z = points[0].z;
    particle->vertices[1].x = points[1].x;
    particle->vertices[1].y = points[1].y;
    particle->vertices[1].z = points[1].z;

    if (segment != 0) {
      particle->vertices[0].x = this->previousPoints[1].x;
      particle->vertices[0].y = this->previousPoints[1].y;
      particle->vertices[0].z = this->previousPoints[1].z;
      particle->vertices[2].x = this->previousPoints[0].x;
      particle->vertices[2].y = this->previousPoints[0].y;
      particle->vertices[2].z = this->previousPoints[0].z;
    }

    this->previousPoints[0].x = points[0].x;
    this->previousPoints[0].y = points[0].y;
    this->previousPoints[0].z = points[0].z;
    this->previousPoints[1].x = points[1].x;
    this->previousPoints[1].y = points[1].y;
    this->previousPoints[1].z = points[1].z;
  }
}

// OFFSET: 0x00B6EDC0
// FUNCTION: fn_46082
void CMapMagicZoneEffect::UpdateRibbon(CParticle* particle) {
  if (particle) {
    float& spinAngle = particle->angle;
    spinAngle = static_cast<float>(static_cast<int>(particle->rowIndex + spinAngle + 3.0f));

    {
      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / -180.0f * 3.141592f);
      particle->transform = rotation;
    }

    this->UpdateRibbonVertices(particle);
  }
}

// OFFSET: 0x00B6F010
// FUNCTION: fn_19118
void CMapMagicZoneEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->spinners.begin(); it != this->spinners.end(); ++it) {
    this->RenderSpinner(*it);
  }

  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    this->RenderPetal(*it);
  }

  for (std::vector<CParticle*>::iterator it = this->ribbon.begin(); it != this->ribbon.end(); ++it) {
    this->RenderRibbon(*it);
  }
}

// OFFSET: 0x00B6F160
// FUNCTION: fn_46083
void CMapMagicZoneEffect::RenderSpinner(CParticle* particle) {
  if (particle) {
    particle->destinationBlend = 6;
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B6F1B0
// FUNCTION: fn_46084
void CMapMagicZoneEffect::RenderPetal(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B6F1F0
// FUNCTION: fn_46085
void CMapMagicZoneEffect::RenderRibbon(CParticle* particle) {
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
