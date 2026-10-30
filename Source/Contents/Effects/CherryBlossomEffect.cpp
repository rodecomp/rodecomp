#include "Contents/Effects/CherryBlossomEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B439D0
// FUNCTION: fn_18882
CCherryBlossomEffect::CCherryBlossomEffect() : CEffect(1013), offsets() {
}

// OFFSET: 0x00B43B50
// FUNCTION: fn_18884
void CCherryBlossomEffect::Close() {
  for (CParticle* particle : this->petals) {
    FreeParticle(particle);
  }

  this->petals.clear();
}

// OFFSET: 0x00B43B90
// FUNCTION: fn_18885
void CCherryBlossomEffect::Slot01() {
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->SetTexture("effect\\cherry_circle.bmp");
        particle->unknown_0004 = this->id;
        particle->unknown_000C = column;
        particle->unknown_0010 = row;
        particle->unknown_0008 = 2;
        particle->unknown_00C0 = 0.15f;
        particle->scale = 0.55f;
        particle->alpha = 0;

        vector3d& offset = this->offsets[row * 4 + column];
        offset.x = 0.0f;
        offset.y = static_cast<float>(std::max(rand() % 100, 0));
        offset.z = 0.0f;

        float* angles = &particle->unknown_0054;
        for (int i = 0; i < 4; ++i) {
          angles[i] = static_cast<float>(std::max(rand() % 360, 0));
        }

        this->petals.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B43CE0
// FUNCTION: fn_18887
void CCherryBlossomEffect::Slot03() {
  for (CParticle* particle : this->petals) {
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      const int index = particle->unknown_000C + particle->unknown_0010 * 4;
      const float speed = particle->unknown_00C0;
      vector3d& offset = this->offsets[index];
      if (offset.y < 0.0f) {
        if (offset.y < -30.0f) {
          offset.x = 0.0f;
          offset.y = static_cast<float>(std::max(rand() % 100, 0));
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
            const int delta = static_cast<int>(angles[i + 2]) - current;
            angles[i] = static_cast<float>(current + (delta > 0 ? 3 : -2));
            if (std::abs(delta) < 3) {
              int target;
              if (delta > 0) {
                target = current ? std::max(rand() % current, 0) : 0;
              } else {
                target = 359 - (current ? std::max(rand() % current, 0) : 0);
              }

              angles[i + 2] = static_cast<float>(target);
            }
          }

          offset.x += speed * g_sinTable[static_cast<int>(angles[0])];
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
