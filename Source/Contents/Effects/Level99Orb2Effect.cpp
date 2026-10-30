#include "Contents/Effects/Level99Orb2Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B6BE00
// FUNCTION: fn_19096
CLevel99Orb2Effect::CLevel99Orb2Effect() : CEffect(977) {
}

// OFFSET: 0x00B6BF00
// FUNCTION: fn_19097
CLevel99Orb2Effect::~CLevel99Orb2Effect() {
}

// OFFSET: 0x00B6BF80
// FUNCTION: fn_19098
void CLevel99Orb2Effect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
}

// OFFSET: 0x00B6BFB0
// FUNCTION: fn_19099
void CLevel99Orb2Effect::Slot01() {
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\freezing_circle.bmp");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = column;
      particle->unknown_0010 = row;
      particle->unknown_0008 = 2;
      particle->unknown_00C0 = 0.15f;
      particle->scale = 0.55f;
      particle->alpha = 0;

      const float height = static_cast<float>(std::max(rand() % 100, 0));
      EffectPoint& offset = this->offsets[row][column];
      offset.x = 0.0f;
      offset.y = height;
      offset.z = 0.0f;

      float* angles = &particle->unknown_0054;
      for (int i = 0; i < 4; ++i) {
        angles[i] = static_cast<float>(std::max(rand() % 360, 0));
      }

      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B6C0F0
// FUNCTION: fn_19100
bool CLevel99Orb2Effect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B6C100
// FUNCTION: fn_19101
void CLevel99Orb2Effect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      const float speed = particle->unknown_00C0;
      EffectPoint& offset = this->offsets[particle->unknown_0010][particle->unknown_000C];
      if (offset.y < 0.0f) {
        if (offset.y < -30.0f) {
          const float height = static_cast<float>(std::max(rand() % 100, 0));
          offset.x = 0.0f;
          offset.y = height;
          offset.z = 0.0f;

          float* angles = &particle->unknown_0054;
          for (int i = 0; i < 4; ++i) {
            angles[i] = static_cast<float>(std::max(rand() % 360, 0));
          }

          particle->alpha = 0;
        } else {
          float* angles = &particle->unknown_0054;
          for (int i = 0; i < 2; ++i) {
            const int current = static_cast<int>(angles[i]);
            const int difference = static_cast<int>(angles[i + 2]) - current;
            angles[i] = static_cast<float>(current + (difference > 0 ? 3 : -2));
            if (std::abs(difference) < 3) {
              if (difference > 0) {
                angles[i + 2] = static_cast<float>(current ? std::max(rand() % current, 0) : 0);
              } else {
                angles[i + 2] = static_cast<float>(359 - (current ? std::max(rand() % current, 0) : 0));
              }
            }
          }

          offset.x += speed * g_sinTable[static_cast<int>(particle->unknown_0054)];
          offset.z += speed * g_sinTable[static_cast<int>(angles[1])];
          if (offset.y < -20.0f) {
            particle->alpha = std::max(particle->alpha - 4, 0);
          } else if (particle->alpha != 255) {
            particle->alpha = 255;
          }
        }
      }

      offset.y -= speed;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(3.141592f);
      rotation.AppendXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);

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
    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B6C920
// FUNCTION: fn_19102
void CLevel99Orb2Effect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      for (int pass = 0; pass < 2; ++pass) {
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      }
    }
  }
}
