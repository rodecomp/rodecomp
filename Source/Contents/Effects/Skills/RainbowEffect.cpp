#include "Contents/Effects/Skills/RainbowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Render/Projection/RenderProjection.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B707B0
// FUNCTION: fn_46054
CRainbowEffect::CRainbowEffect() : CEffect(EF_RAINBOW) {
}

// OFFSET: 0x00B70850
// FUNCTION: fn_46055
__declspec(noinline) CRainbowEffect::~CRainbowEffect() {
}

// OFFSET: 0x00B70930
// FUNCTION: fn_46056
__forceinline void CRainbowEffect::CreateBands() {
  CGameMode* mode = g_modeMgr.GetGameMode();
  const int angle = (360 - static_cast<int>(mode->camera->camera.yaw)) % 360;
  for (int row = 0; row < 7; ++row) {
    for (int column = 0; column < 21; ++column) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->effectId = this->id;
        int* indices = &particle->groupIndex;
        indices[1] = column;
        indices[2] = row;
        indices[0] = 1;
        particle->SetTexture("effect\\alpha_center.tga");
        particle->alpha = 0;
        particle->scale = 50.0f;
        particle->angleParameter = 180;
        particle->angle = 0;
        particle->phaseParameter = angle;
        particle->InitParticleQuad(particle->vertices);
        this->bands.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B70A30
// FUNCTION: fn_46057
void CRainbowEffect::UpdateBandVertices(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int band = particle->rowIndex;
  const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
  const int angle = static_cast<int>(static_cast<float>(step * particle->columnIndex) + particle->angle) % 360;

  EffectPoint points[4];

  float ratio = 1.0f - static_cast<float>(band) * 0.05f;
  float flat = particle->scale * g_cosTable[angle] * ratio;
  float rise = particle->scale * g_sinTable[angle] * ratio;
  points[0].x = g_cosTable[particle->phaseParameter] * flat;
  points[0].y = rise * 2.0f;
  points[0].z = g_sinTable[particle->phaseParameter] * flat;

  ratio = 1.0f - static_cast<float>(band + 1) * 0.05f;
  flat = particle->scale * g_cosTable[angle] * ratio;
  rise = particle->scale * g_sinTable[angle] * ratio;
  points[1].x = g_cosTable[particle->phaseParameter] * flat;
  points[1].y = rise * 2.0f;
  points[1].z = g_sinTable[particle->phaseParameter] * flat;

  if (particle->columnIndex != 0) {
    particle->vertices[0].x = points[1].x;
    particle->vertices[0].y = points[1].y;
    particle->vertices[0].z = points[1].z;
    particle->vertices[1].x = points[0].x;
    particle->vertices[1].y = points[0].y;
    particle->vertices[1].z = points[0].z;
    particle->vertices[2].x = this->previousPoints[0].x;
    particle->vertices[2].y = this->previousPoints[0].y;
    particle->vertices[2].z = this->previousPoints[0].z;
    particle->vertices[3].x = this->previousPoints[1].x;
    particle->vertices[3].y = this->previousPoints[1].y;
    particle->vertices[3].z = this->previousPoints[1].z;
  }

  this->previousPoints[0].x = points[1].x;
  this->previousPoints[0].y = points[1].y;
  this->previousPoints[0].z = points[1].z;
  this->previousPoints[1].x = points[0].x;
  this->previousPoints[1].y = points[0].y;
  this->previousPoints[1].z = points[0].z;

  switch (band) {
  case 0:
    particle->SetColor(255, 0, 0);
    break;
  case 1:
    particle->SetColor(255, 126, 0);
    break;
  case 2:
    particle->SetColor(255, 255, 0);
    break;
  case 3:
    particle->SetColor(0, 255, 0);
    break;
  case 4:
    particle->SetColor(0, 0, 255);
    break;
  case 5:
    particle->SetColor(115, 50, 200);
    break;
  case 6:
    particle->SetColor(255, 0, 255);
    break;
  }
}

// OFFSET: 0x00B70CB0
// FUNCTION: fn_19128
void CRainbowEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    FreeParticle(*it);
  }

  this->bands.clear();
}

// OFFSET: 0x00B70CF0
// FUNCTION: fn_19129
void CRainbowEffect::Open() {
  this->CreateBands();
  this->effectLifetime = 300;
}

// OFFSET: 0x00B70DF0
// FUNCTION: fn_19130
bool CRainbowEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B70E00
// FUNCTION: fn_19131
void CRainbowEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      int alpha = particle->alpha;

      if (particle->elapsedFrames <= 180) {
        ++alpha;
      } else if (particle->elapsedFrames <= 360) {
        --alpha;
      }

      float& spinAngle = particle->angle;
      spinAngle += 1.0f;

      if (alpha < 0) {
        alpha = 0;
      }

      particle->alpha = alpha >= 250 ? 250 : alpha;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / -180.0f * 3.141592f);
      particle->transform = rotation;

      const int band = particle->rowIndex;
      const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
      const int angle = static_cast<int>(static_cast<float>(step * particle->columnIndex) + spinAngle) % 360;

      EffectPoint points[4];

      float ratio = 1.0f - static_cast<float>(band) * 0.05f;
      float flat = particle->scale * g_cosTable[angle] * ratio;
      float rise = particle->scale * g_sinTable[angle] * ratio;
      points[0].x = g_cosTable[particle->phaseParameter] * flat;
      points[0].y = rise * 2.0f;
      points[0].z = g_sinTable[particle->phaseParameter] * flat;

      ratio = 1.0f - static_cast<float>(band + 1) * 0.05f;
      flat = particle->scale * g_cosTable[angle] * ratio;
      rise = particle->scale * g_sinTable[angle] * ratio;
      points[1].x = g_cosTable[particle->phaseParameter] * flat;
      points[1].y = rise * 2.0f;
      points[1].z = g_sinTable[particle->phaseParameter] * flat;

      if (particle->columnIndex != 0) {
        particle->vertices[0].x = points[1].x;
        particle->vertices[0].y = points[1].y;
        particle->vertices[0].z = points[1].z;
        particle->vertices[1].x = points[0].x;
        particle->vertices[1].y = points[0].y;
        particle->vertices[1].z = points[0].z;
        particle->vertices[2].x = this->previousPoints[0].x;
        particle->vertices[2].y = this->previousPoints[0].y;
        particle->vertices[2].z = this->previousPoints[0].z;
        particle->vertices[3].x = this->previousPoints[1].x;
        particle->vertices[3].y = this->previousPoints[1].y;
        particle->vertices[3].z = this->previousPoints[1].z;
      }

      this->previousPoints[0].x = points[1].x;
      this->previousPoints[0].y = points[1].y;
      this->previousPoints[0].z = points[1].z;
      this->previousPoints[1].x = points[0].x;
      this->previousPoints[1].y = points[0].y;
      this->previousPoints[1].z = points[0].z;

      switch (band) {
      case 0:
        particle->SetColor(255, 0, 0);
        break;
      case 1:
        particle->SetColor(255, 126, 0);
        break;
      case 2:
        particle->SetColor(255, 255, 0);
        break;
      case 3:
        particle->SetColor(0, 255, 0);
        break;
      case 4:
        particle->SetColor(0, 0, 255);
        break;
      case 5:
        particle->SetColor(115, 50, 200);
        break;
      case 6:
        particle->SetColor(255, 0, 255);
        break;
      }
    }

    ++particle->elapsedFrames;
  }
}

// INLINED
static matrix CameraYawRotation() {
  matrix rotation;
  std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
  rotation.MakeYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / -180.0f * 3.141592f);
  return rotation;
}

// OFFSET: 0x00B71180
// FUNCTION: fn_46058
void CRainbowEffect::UpdateBand(CParticle* particle) {
  if (!particle) {
    return;
  }

  int alpha = particle->alpha;
  if (particle->elapsedFrames <= 180) {
    ++alpha;
  } else if (particle->elapsedFrames <= 360) {
    --alpha;
  }

  particle->angle += 1.0f;
  if (alpha < 0) {
    alpha = 0;
  }

  particle->alpha = alpha >= 250 ? 250 : alpha;

  particle->transform = CameraYawRotation();

  const int band = particle->rowIndex;
  const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
  const int angle = static_cast<int>(static_cast<float>(step * particle->columnIndex) + particle->angle) % 360;

  {
    EffectPoint points[4];

    float ratio = 1.0f - static_cast<float>(band) * 0.05f;
    float flat = particle->scale * g_cosTable[angle] * ratio;
    float rise = particle->scale * g_sinTable[angle] * ratio;
    points[0].x = g_cosTable[particle->phaseParameter] * flat;
    points[0].y = rise * 2.0f;
    points[0].z = g_sinTable[particle->phaseParameter] * flat;

    ratio = 1.0f - static_cast<float>(band + 1) * 0.05f;
    flat = particle->scale * g_cosTable[angle] * ratio;
    rise = particle->scale * g_sinTable[angle] * ratio;
    points[1].x = g_cosTable[particle->phaseParameter] * flat;
    points[1].y = rise * 2.0f;
    points[1].z = g_sinTable[particle->phaseParameter] * flat;

    if (particle->columnIndex != 0) {
      particle->vertices[0].x = points[1].x;
      particle->vertices[0].y = points[1].y;
      particle->vertices[0].z = points[1].z;
      particle->vertices[1].x = points[0].x;
      particle->vertices[1].y = points[0].y;
      particle->vertices[1].z = points[0].z;
      particle->vertices[2].x = this->previousPoints[0].x;
      particle->vertices[2].y = this->previousPoints[0].y;
      particle->vertices[2].z = this->previousPoints[0].z;
      particle->vertices[3].x = this->previousPoints[1].x;
      particle->vertices[3].y = this->previousPoints[1].y;
      particle->vertices[3].z = this->previousPoints[1].z;
    }

    this->previousPoints[0].x = points[1].x;
    this->previousPoints[0].y = points[1].y;
    this->previousPoints[0].z = points[1].z;
    this->previousPoints[1].x = points[0].x;
    this->previousPoints[1].y = points[0].y;
    this->previousPoints[1].z = points[0].z;

  switch (band) {
  case 0:
    particle->SetColor(255, 0, 0);
    break;
  case 1:
    particle->SetColor(255, 126, 0);
    break;
  case 2:
    particle->SetColor(255, 255, 0);
    break;
  case 3:
    particle->SetColor(0, 255, 0);
    break;
  case 4:
    particle->SetColor(0, 0, 255);
    break;
  case 5:
    particle->SetColor(115, 50, 200);
    break;
  case 6:
    particle->SetColor(255, 0, 255);
    break;
  }
  }
}

// OFFSET: 0x00B71570
// FUNCTION: fn_46059
__forceinline void CRainbowEffect::RenderBand(CParticle* particle) {
  if (particle && particle->columnIndex > 0) {
    const int segment = particle->columnIndex;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = static_cast<float>(segment - 1) / 21.0f;
    quad.bottom = static_cast<float>(segment) / 21.0f;
    quad.left = 0.0f;
    quad.right = 1.0f;
    particle->Render(quad);
  }
}

// OFFSET: 0x00B714C0
// FUNCTION: fn_19132
void CRainbowEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    CParticle* particle = *it;

    this->RenderBand(particle);
  }
}
