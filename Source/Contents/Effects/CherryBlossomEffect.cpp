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
CCherryBlossomEffect::CCherryBlossomEffect() : CEffect(1013) {
}

// OFFSET: 0x00B43B50
// FUNCTION: fn_18884
void CCherryBlossomEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    FreeParticle(*it);
  }

  this->petals.clear();
}

// OFFSET: 0x00B43B90
// FUNCTION: fn_18885
void CCherryBlossomEffect::Open() {
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->SetTexture("effect\\cherry_circle.bmp");
        particle->effectId = this->id;
        particle->columnIndex = column;
        particle->rowIndex = row;
        particle->groupIndex = 2;
        particle->speedParameter = 0.15f;
        particle->scale = 0.55f;
        particle->alpha = 0;

        const float height = static_cast<float>(std::max(rand() % 100, 0));
        EffectPoint& offset = this->offsets[row * 4 + column];
        offset.x = 0.0f;
        offset.y = height;
        offset.z = 0.0f;

        float* angles = &particle->animationParameter;
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
void CCherryBlossomEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
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
          offset.y = static_cast<float>(std::max(rand() % 100, 0));
          offset.z = 0.0f;

          float* angles = &particle->animationParameter;
          for (int i = 0; i < 4; ++i) {
            angles[i] = static_cast<float>(std::max(rand() % 360, 0));
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

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B44500
// FUNCTION: fn_18888
void CCherryBlossomEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->petals.begin(); it != this->petals.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      for (int i = 0; i < 2; ++i) {
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      }
    }
  }
}
