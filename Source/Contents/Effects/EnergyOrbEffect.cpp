#include "Contents/Effects/EnergyOrbEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B50D10
// FUNCTION: fn_19022
CEnergyOrbEffect::CEnergyOrbEffect() : CEffect(-1), count(0), mode(2), particles() {
}

// OFFSET: 0x00B50E00
// FUNCTION: fn_19023
CEnergyOrbEffect::~CEnergyOrbEffect() {
}

// OFFSET: 0x00B50E80
// FUNCTION: fn_19024
void CEnergyOrbEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B50EC0
// FUNCTION: fn_19025
void CEnergyOrbEffect::Slot01() {
  for (int i = 0; i < this->count; ++i) {
    const int angle = std::max(rand() % 360, 0);
    const int phase = std::max(rand() % 3, 0);

    for (int j = 0; j < this->mode; ++j) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_0010 = i;
      particle->unknown_000C = j;
      particle->unknown_0008 = 1;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(360 / this->count * i);
      particle->SetTexture("effect\\thunder_center.bmp");
      particle->alpha = 0;
      particle->scale = 0.8f;
      particle->unknown_00C0 = 0.15f;
      particle->unknown_0054 = 8.0f;
      particle->unknown_004C = angle;
      *reinterpret_cast<float*>(&particle->unknown_0058[0]) = static_cast<float>(phase);
      *reinterpret_cast<float*>(&particle->unknown_0058[4]) = -12.0f;
      if (j == 0) {
        particle->destinationBlend = 6;
      }

      switch (this->id) {
      case 228:
        particle->SetColor(particle->unknown_000C != 0 ? -1 : 0, particle->unknown_000C != 0 ? -1 : 0, particle->unknown_000C != 0 ? 0 : 255);
        break;
      case 735:
        particle->SetTexture("effect\\FireOrb.bmp");
        particle->scale = 2.3f;
        break;
      case 736:
        particle->SetTexture("effect\\LightningOrb.bmp");
        particle->scale = 2.3f;
        break;
      case 737:
        particle->SetTexture("effect\\WaterOrb.bmp");
        particle->scale = 1.3f;
        break;
      case 738:
        particle->SetTexture("effect\\StoneOrb.bmp");
        particle->scale = 2.3f;
        break;
      case 1005:
        particle->SetTexture("effect\\fog1.tga");
        particle->scale = 6.0f;
        particle->unknown_00C0 = 0.05f;
        particle->unknown_0054 = 4.0f;
        *reinterpret_cast<float*>(&particle->unknown_0058[0]) = 0.0f;
        *reinterpret_cast<float*>(&particle->unknown_0058[4]) = -8.0f;
        particle->destinationBlend = 6;
        break;
      }

      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B51240
// FUNCTION: fn_19026
bool CEnergyOrbEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B51250
// FUNCTION: fn_19027
void CEnergyOrbEffect::Slot03() {
  for (CParticle* particle : this->particles) {
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      if (particle->unknown_00E8 <= 200) {
        ++particle->alpha;
      }

      float angle = particle->unknown_00C0 * 5.0f + *reinterpret_cast<float*>(&particle->unknown_00C4);
      if (angle > 360.0f) {
        angle -= 360.0f;
      }
      *reinterpret_cast<float*>(&particle->unknown_00C4) = angle;

      if (particle->alpha > 0) {
        matrix rotation;
        std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
        rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
        rotation.AppendXRotation(3.141592f);

        const float scale = particle->scale;
        particle->unknown_004C = static_cast<int>(static_cast<float>(particle->unknown_004C) + *reinterpret_cast<float*>(&particle->unknown_0058[0])) % 360;
        float pulse = scale * 0.1f * g_sinTable[particle->unknown_004C];
        if (this->mode > 1 && particle->unknown_000C == 0) {
          pulse = static_cast<float>(scale * 0.1 + pulse);
        }
        const float size = scale + pulse;
        matrix scaling;
        std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
        scaling.SetScale(size, size, size);

        const float wave = g_sinTable[particle->unknown_004C] * 0.6f;
        const int index = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4));
        const float radius = wave + particle->unknown_0054;
        matrix translation;
        std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
        translation.MakeTranslate(radius * g_cosTable[index], wave * *reinterpret_cast<float*>(&particle->unknown_0058[0]) + *reinterpret_cast<float*>(&particle->unknown_0058[4]), radius * g_sinTable[index]);

        matrix scaled;
        scaled.Compose(scaling, rotation);
        matrix result;
        result.Compose(scaled, translation);
        particle->transform = result;
      }
    }
    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B51750
// FUNCTION: fn_19028
void CEnergyOrbEffect::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x00B517F0
// FUNCTION: fn_19029
void CEnergyOrbEffect::SetCount(int value) {
  if (this->count != value) {
    this->count = value;
    this->Slot01();
  }
}
