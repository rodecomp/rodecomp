#include "Contents/Effects/LightSphereEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B6C9F0
// FUNCTION: fn_19103
CLightSphereEffect::CLightSphereEffect() : CEffect(-1), spheres(), rays() {
}

// OFFSET: 0x00B6CB90
// FUNCTION: fn_19105
void CLightSphereEffect::sub_B6CB90(int param) {
  for (int c = 0; c < 4; ++c) {
    for (int i = 0; i < 80; ++i) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(&this->position), 5);
      if (particle != nullptr) {
        particle->SetTexture("effect\\white02.bmp");
        particle->unknown_0004 = this->id;
        particle->unknown_000C = i;
        particle->unknown_0010 = c;
        particle->unknown_0008 = 1;
        particle->alpha = 40;
        particle->scale = 1.0f;

        int r1 = std::rand() % 360;
        particle->unknown_004C = r1 < 0 ? 0 : r1;

        int r2 = std::rand() % 360;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(r2 < 0 ? 0 : r2);

        int r3 = std::rand() % 360;
        particle->unknown_0050 = r3 < 0 ? 0 : r3;

        particle->unknown_0054 = 10.0f;
        *reinterpret_cast<float*>(particle->unknown_0058) = static_cast<float>(param);

        if (this->id == 0x4AD) {
          int randVal = std::rand() % 2;
          if (randVal < 0) {
            randVal = 0;
          }
          if (randVal == 0) {
            particle->SetColor(0xFF, 0x7C, 0x80);
          } else {
            particle->SetColor(0xFF, 0x4A, 0x4E);
          }
        } else if (this->id == 0x4AE) {
          int randVal = std::rand() % 2;
          if (randVal < 0) {
            randVal = 0;
          }
          if (randVal == 0) {
            particle->SetColor(0xFF, 0xFF, 0x9B);
          } else {
            particle->SetColor(0xFF, 0xFF, 0x37);
          }
        } else if (this->id == 0x4AF) {
          int randVal = std::rand() % 2;
          if (randVal < 0) {
            randVal = 0;
          }
          if (randVal == 0) {
            particle->SetColor(0x3B, 0xCC, 0xFF);
          } else {
            particle->SetColor(0x6D, 0x9A, 0xFF);
          }
        } else {
          int randVal = std::rand() % 2;
          if (randVal < 0) {
            randVal = 0;
          }
          if (randVal == 0) {
            particle->SetColor(0x69, 0x69, 0xFF);
          } else {
            particle->SetColor(0x37, 0x37, 0xFF);
          }
        }

        InitParticleQuad(particle->vertices);
        this->rays.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B6CFF0
// FUNCTION: fn_19106
void CLightSphereEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->spheres.begin(); it != this->spheres.end(); ++it) {
    FreeParticle(*it);
  }
  this->spheres.clear();

  for (std::vector<CParticle*>::iterator it = this->rays.begin(); it != this->rays.end(); ++it) {
    FreeParticle(*it);
  }
  this->rays.clear();
}

// OFFSET: 0x00B6D060
// FUNCTION: fn_19107
void CLightSphereEffect::Slot01() {
  for (int i = 0; i < 4; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(&this->position), 5);
    if (particle != nullptr) {
      particle->SetTexture("effect\\thunder_center.bmp");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = i;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->scale = 3.5f;
      particle->alpha = 1;
      particle->unknown_0054 = -10.0f;
      *reinterpret_cast<float*>(particle->unknown_0058) = 0.0f;

      if (this->id == 0x4AD) {
        particle->SetColor(0xFF, 0x7C, 0x80);
      } else if (this->id == 0x4AE) {
        particle->SetColor(0xFF, 0xFF, 0x37);
      } else if (this->id == 0x4AF) {
        particle->SetColor(0x3B, 0xCC, 0xFF);
      }

      InitParticleQuad(particle->vertices);
      this->spheres.push_back(particle);
    }
  }

  this->sub_B6CB90(0);
  this->sub_B6CB90(1);
}

// OFFSET: 0x00B6D160
// FUNCTION: fn_19108
bool CLightSphereEffect::Slot06() {
  std::vector<CParticle*>::iterator sphere = this->spheres.begin();
  for (; sphere != this->spheres.end(); ++sphere) {
    if ((*sphere)->alpha > 0) {
      break;
    }
  }

  std::vector<CParticle*>::iterator ray = this->rays.begin();
  for (; ray != this->rays.end(); ++ray) {
    if ((*ray)->alpha > 0) {
      break;
    }
  }

  return sphere != this->spheres.end() || ray != this->rays.end();
}

// OFFSET: 0x00B6D1C0
// FUNCTION: fn_19109
void CLightSphereEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->spheres.begin(); it != this->spheres.end(); ++it) {
    CParticle* particle = *it;

    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      if (particle->unknown_00E8 < 300) {
        const int alpha = particle->alpha + 1;
        particle->alpha = alpha >= 255 ? 255 : alpha;
      } else {
        const int alpha = particle->alpha - 1;
        particle->alpha = alpha < 0 ? 0 : alpha;
      }

      float& phase = *reinterpret_cast<float*>(particle->unknown_0058);
      const int spin = (static_cast<int>(phase) + 2) % 360;
      phase = static_cast<float>(spin);
      const float scale = particle->scale * g_sinTable[spin] * 0.05f + particle->scale;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);

      matrix scaling;
      std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
      scaling.SetScale(scale, scale, 1.0f);

      matrix billboard;
      billboard.Compose(scaling, rotation);

      matrix translation;
      std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
      translation.MakeTranslate(0.0f, particle->unknown_0054, 0.0f);

      matrix transform;
      transform.Compose(billboard, translation);
      particle->transform = transform;
    }

    ++particle->unknown_00E8;
  }

  for (std::vector<CParticle*>::iterator it = this->rays.begin(); it != this->rays.end(); ++it) {
    CParticle* particle = *it;

    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      float scale = particle->scale;
      if (scale < 10.0f) {
        scale *= 1.02f;
      }
      particle->scale = scale;

      int alpha = particle->alpha;
      if (particle->unknown_00E8 > 500) {
        --alpha;
        if (alpha < 0) {
          alpha = 0;
        }
      }
      particle->alpha = alpha;

      const int previous = particle->unknown_0050;
      const int roll = std::rand() % 2;
      const int phase = (previous + 1 + (roll < 0 ? 0 : roll)) % 360;
      particle->unknown_0050 = phase;

      vector3d points[3]{};
      const float radius = particle->scale * g_sinTable[phase] + particle->scale;
      const int pitch = particle->unknown_004C;
      const int yaw = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4));
      const float flat = radius * g_cosTable[pitch];
      points[0].x = flat * g_cosTable[yaw];
      points[0].y = radius * g_sinTable[pitch] - particle->unknown_0054;
      points[0].z = flat * g_sinTable[yaw];

      if (*reinterpret_cast<float*>(particle->unknown_0058) == 0.0f) {
        points[1].x = -0.5f;
        points[1].y = -particle->unknown_0054;
        points[1].z = 0.0f;
        points[2].x = 0.5f;
        points[2].y = -particle->unknown_0054;
        points[2].z = 0.0f;
      } else {
        points[1].x = 0.0f;
        points[1].y = -particle->unknown_0054;
        points[1].z = -0.5f;
        points[2].x = 0.0f;
        points[2].y = -particle->unknown_0054;
        points[2].z = 0.5f;
      }

      particle->vertices[0].x = points[0].x;
      particle->vertices[0].y = points[0].y;
      particle->vertices[0].z = points[0].z;
      particle->vertices[1].x = points[0].x;
      particle->vertices[1].y = points[0].y;
      particle->vertices[1].z = points[0].z;
      particle->vertices[2].x = points[1].x;
      particle->vertices[2].y = points[1].y;
      particle->vertices[2].z = points[1].z;
      particle->vertices[3].x = points[2].x;
      particle->vertices[3].y = points[2].y;
      particle->vertices[3].z = points[2].z;
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B6D990
// FUNCTION: fn_19110
void CLightSphereEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->spheres.begin(); it != this->spheres.end(); ++it) {
    CParticle* particle = *it;

    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->rays.begin(); it != this->rays.end(); ++it) {
    CParticle* particle = *it;

    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
