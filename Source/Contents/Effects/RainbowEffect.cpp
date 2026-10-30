#include "Contents/Effects/RainbowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

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
void CRainbowEffect::Slot01() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const int angle = (360 - static_cast<int>(mode->camera->camera.yaw)) % 360;
  for (int row = 0; row < 7; ++row) {
    for (int column = 0; column < 21; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->unknown_0004 = this->id;
        particle->unknown_000C = column;
        particle->unknown_0010 = row;
        particle->unknown_0008 = 1;
        particle->SetTexture("effect\\alpha_center.tga");
        particle->alpha = 0;
        particle->scale = 50.0f;
        particle->unknown_0050 = 180;
        particle->unknown_00C4 = 0;
        particle->unknown_004C = angle;
        InitParticleQuad(particle->vertices);
        this->bands.push_back(particle);
      }
    }
  }

  this->unknown_002C = 300;
}

// OFFSET: 0x00B70DF0
// FUNCTION: fn_19130
bool CRainbowEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B70E00
// FUNCTION: fn_19131
void CRainbowEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      int alpha = particle->alpha;

      if (particle->unknown_00E8 <= 180) {
        ++alpha;
      } else if (particle->unknown_00E8 <= 360) {
        --alpha;
      }

      float& spinAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      spinAngle += 1.0f;

      if (alpha < 0) {
        alpha = 0;
      }

      particle->alpha = alpha >= 250 ? 250 : alpha;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
      particle->transform = rotation;

      const int band = particle->unknown_0010;
      const int step = static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f);
      const int angle = static_cast<int>(static_cast<float>(step * particle->unknown_000C) + spinAngle) % 360;

      vector3d points[4]{};
      const float width = particle->scale * g_cosTable[angle];
      const float height = particle->scale * g_sinTable[angle];

      float ratio = 1.0f - static_cast<float>(band) * 0.05f;
      float flat = width * ratio;
      float rise = height * ratio;
      points[0].x = g_cosTable[particle->unknown_004C] * flat;
      points[0].y = rise + rise;
      points[0].z = g_sinTable[particle->unknown_004C] * flat;

      ratio = 1.0f - static_cast<float>(band + 1) * 0.05f;
      flat = width * ratio;
      rise = height * ratio;
      points[1].x = g_cosTable[particle->unknown_004C] * flat;
      points[1].y = rise + rise;
      points[1].z = g_sinTable[particle->unknown_004C] * flat;

      if (particle->unknown_000C != 0) {
        particle->vertices[0].x = points[1].x;
        particle->vertices[0].y = points[1].y;
        particle->vertices[0].z = points[1].z;
        particle->vertices[1].x = points[0].x;
        particle->vertices[1].y = points[0].y;
        particle->vertices[1].z = points[0].z;
        particle->vertices[2].x = this->previousOuter.x;
        particle->vertices[2].y = this->previousOuter.y;
        particle->vertices[2].z = this->previousOuter.z;
        particle->vertices[3].x = this->previousInner.x;
        particle->vertices[3].y = this->previousInner.y;
        particle->vertices[3].z = this->previousInner.z;
      }

      this->previousOuter = points[1];
      this->previousInner = points[0];

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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B714C0
// FUNCTION: fn_19132
void CRainbowEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    CParticle* particle = *it;

    if (particle && particle->unknown_000C > 0) {
      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, 0.0f, 1.0f, false, true};
      particle->Render(quad);
    }
  }
}
