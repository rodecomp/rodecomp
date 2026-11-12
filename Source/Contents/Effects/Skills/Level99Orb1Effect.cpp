#include "Contents/Effects/Skills/Level99Orb1Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace {
  // INLINED
  __forceinline int Random(int range) {
    if (range == 0) {
      return 0;
    }

    const int value = rand() % range;
    return value >= 0 ? value : 0;
  }
}

// OFFSET: 0x00B6B210
// FUNCTION: fn_19089
CLevel99Orb1Effect::CLevel99Orb1Effect() : CEffect(976) {
}

// OFFSET: 0x00B6B390
// FUNCTION: fn_19091
void CLevel99Orb1Effect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B6B3D0
// FUNCTION: fn_19092
void CLevel99Orb1Effect::Open() {
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->SetTexture("effect\\whitelight.tga");
        particle->effectId = this->id;
        particle->columnIndex = column;
        particle->rowIndex = row;
        particle->groupIndex = 2;
        particle->speedParameter = 0.15f;
        particle->scale = 1.8f;
        particle->SetColor(80, 80, 255);
        particle->alpha = 0;

        const float height = static_cast<float>(Random(100));
        EffectPoint& offset = this->offsets[row * 4 + column];
        offset.x = 0.0f;
        offset.y = height;
        offset.z = 0.0f;

        float* angles = &particle->animationParameter;
        for (int i = 0; i < 4; ++i) {
          angles[i] = static_cast<float>(Random(360));
        }

        this->particles.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B6B520
// FUNCTION: fn_19093
bool CLevel99Orb1Effect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B6B530
// FUNCTION: fn_19094
void CLevel99Orb1Effect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    CParticle* particle = *it;
    if (particle) {
      const int index = particle->columnIndex + particle->rowIndex * 4;
      const float speed = particle->speedParameter;
      EffectPoint& offset = this->offsets[index];
      if (offset.y < 0.0f) {
        if (offset.y < -30.0f) {
          offset.x = 0.0f;
          offset.y = static_cast<float>(Random(100));
          offset.z = 0.0f;

          float* angles = &particle->animationParameter;
          for (int i = 0; i < 4; ++i) {
            angles[i] = static_cast<float>(Random(360));
          }

          particle->alpha = 0;
        } else {
          float* angles = &particle->animationParameter;
          for (int i = 0; i < 2; ++i) {
            const int current = static_cast<int>(angles[i]);
            const int delta = static_cast<int>(angles[i + 2]) - current;
            angles[i] = static_cast<float>(current + (delta > 0 ? 3 : -2));
            if (std::abs(delta) < 3) {
              int target;
              if (delta > 0) {
                target = Random(current);
              } else {
                target = 359 - Random(current);
              }

              angles[i + 2] = static_cast<float>(target);
            }
          }

          offset.x += speed * g_sinTable[static_cast<int>(angles[0])];
          offset.z += speed * g_sinTable[static_cast<int>(angles[1])];
          if (offset.y < -20.0f) {
            const int alpha = particle->alpha - 4;
            particle->alpha = alpha >= 0 ? alpha : 0;
          } else if (particle->alpha != 255) {
            particle->alpha = 255;
          }
        }
      }

      offset.y -= speed;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);

      matrix translation;
      std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
      translation.MakeTranslate(offset.x, offset.y, offset.z);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix scaled;
      scaled.Compose(scale, rotation);
      matrix result;
      result.Compose(scaled, translation);
      particle->transform = result;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B6BD30
// FUNCTION: fn_19095
void CLevel99Orb1Effect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      for (int pass = 0; pass < 2; ++pass) {
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      }
    }
  }
}
