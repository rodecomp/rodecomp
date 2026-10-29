#include "Contents/Effects/ThrowEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00B7D8B0
// FUNCTION: fn_19234
void CThrowItemEffect::Slot12() {
  switch (this->id) {
  case 613:
  case 614:
  case 615:
  case 616:
    this->AddEvent(0, "effect\\닌자_던지기.wav", 0, 0);
    break;
  case 298:
    this->AddEvent(0, "_heal_effect.wav", 0, 0);
    break;
  case 914:
    this->AddEvent(0, "effect\\G스포어익스플로젼.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B7DE70
// FUNCTION: fn_19236
void CThrowItemEffect::Slot03() {
  for (auto particle = this->particles.begin(); particle != this->particles.end(); ++particle) {
    CParticle* target = *particle;

    target->position.x = this->position[0];
    target->position.y = this->position[1];
    target->position.z = this->position[2];

    if (target != nullptr && this->unknown_00D0 <= this->frame) {
      float& angle = target->unknown_00C0;
      float& rotation = *reinterpret_cast<float*>(&target->unknown_00C4);
      float& offsetX = *reinterpret_cast<float*>(&target->unknown_00D0);
      float& offsetY = *reinterpret_cast<float*>(&target->unknown_00D4);
      float& offsetZ = *reinterpret_cast<float*>(&target->unknown_00D8);

      if (target->unknown_000C == 0) {
        rotation = static_cast<float>(static_cast<int>(rotation + 45.0f) % 360);
      } else {
        this->unknown_00E4 -= target->unknown_000C == 1 ? 150 : 25;
        target->alpha = this->unknown_00E4;
        offsetX = this->unknown_00EC;
        offsetY = this->unknown_00F0;
        offsetZ = this->unknown_00F4;
        rotation = this->unknown_00E8;
      }

      const float progress = static_cast<float>(this->frame) / static_cast<float>(this->unknown_00CC);
      float height = 0.0f;

      if (progress > 1.05f) {
        const int alpha = target->alpha - 20;
        target->alpha = alpha > 0 ? alpha : 0;
      } else {
        const int index = *reinterpret_cast<const int*>(&target->unknown_0048[4]);
        const float radius = angle / *reinterpret_cast<const float*>(&target->unknown_0048[0xC]);
        offsetX = angle * g_cosTable[index] + offsetX;
        offsetY = radius * this->unknown_00DC + offsetY;
        offsetZ = angle * g_sinTable[index] + offsetZ;
        height = g_sinTable[-static_cast<int>(progress * -180.0f)] * this->unknown_00E0;
      }

      this->unknown_00EC = offsetX;
      this->unknown_00F0 = offsetY;
      this->unknown_00F4 = offsetZ;
      this->unknown_00E8 = rotation;
      this->unknown_00E4 = target->alpha;

      if (target->alpha > 0) {
        matrix view;
        std::memcpy(view.values, g_identityMatrix, sizeof(view.values));
        view.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
        view.AppendYRotation(rotation / 180.0f * 3.141592f);

        matrix scale;
        std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
        scale.SetScale(target->scale, target->scale, 1.0f);

        matrix translate;
        std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
        translate.MakeTranslate(offsetX, offsetY - height, offsetZ);

        matrix rotationMatrix;
        rotationMatrix.Compose(scale, view);

        matrix transform;
        transform.Compose(rotationMatrix, translate);
        target->transform = transform;

        if (this->id == 0x3D5 && target->unknown_0010 == 0 && target->unknown_000C == 0) {
          CRenderObject* object = FindRenderObject(this->actor);

          if (object != nullptr) {
            float& objectAngle = *reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(object) + 0x4C);
            objectAngle = static_cast<float>(static_cast<int>(objectAngle + 45.0f) % 360);
          }
        }
      }
    }

    ++target->unknown_00E8;
  }
}


// OFFSET: 0x00B7F190
// FUNCTION: fn_19243
bool CVolcanoEffect::Slot06() {
  for (auto particle = this->particles.begin(); particle != this->particles.end(); ++particle) {
    if ((*particle)->alpha > 0) {
      return true;
    }
  }

  return false;
}
