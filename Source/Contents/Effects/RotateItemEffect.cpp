#include "Contents/Effects/RotateItemEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B73840
// FUNCTION: fn_19146
CRotateItemEffect::CRotateItemEffect() : CEffect(-1), alpha(0), angle(0) {
}

// OFFSET: 0x00B739B0

// OFFSET: 0x00B739F0

// OFFSET: 0x00B73AC0
// FUNCTION: fn_19150
bool CRotateItemEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B73AD0
// FUNCTION: fn_19151
void CRotateItemEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      if (this->frame >= 5) {
        float& particleAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
        if (particle->unknown_000C == 0) {
          particleAngle = static_cast<float>(static_cast<int>(particle->unknown_00C0 * 45.0f + particleAngle) % 360);
          if (this->id != 984) {
            if (CRenderObject* object = FindRenderObject(this->actor)) {
              object->render.angle = static_cast<float>(static_cast<int>(particle->unknown_00C0 * 45.0f + object->render.angle) % 360);
            }
          }
        } else {
          this->alpha -= particle->unknown_000C == 1 ? 150 : 25;
          particle->alpha = this->alpha;
          particleAngle = this->angle;
        }

        const int index = static_cast<int>(particleAngle);
        const float radius = particle->unknown_0054;
        particle->unknown_00D0 = radius * g_cosTable[index];
        particle->unknown_00D4 = -8.0f;
        particle->unknown_00D8 = radius * g_sinTable[index];
        this->angle = static_cast<float>(static_cast<int>(particleAngle - 5.0f) % 360);
        this->alpha = particle->alpha;
        if (particle->alpha > 0) {
          matrix rotation;
          std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
          CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
          rotation.MakeXRotation(mode->camera->camera.pitch / 180.0f * 3.141592f);
          rotation.AppendYRotation(static_cast<float>(static_cast<int>(particleAngle + 45.0f) % 360) / 180.0f * 3.141592f);
          matrix scale;
          std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
          scale.SetScale(particle->scale, particle->scale, 1.0f);
          matrix translate;
          std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
          translate.MakeTranslate(particle->unknown_00D0, particle->unknown_00D4, particle->unknown_00D8);
          matrix first;
          first.Compose(scale, rotation);
          matrix result;
          result.Compose(first, translate);
          particle->transform = result;
        }
      }
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B74110
// FUNCTION: fn_19152
void CRotateItemEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
