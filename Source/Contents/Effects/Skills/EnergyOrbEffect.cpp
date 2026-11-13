#include "Contents/Effects/Skills/EnergyOrbEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
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
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B50EC0
// FUNCTION: fn_19025
void CEnergyOrbEffect::Open() {
  for (int i = 0; i < this->count; ++i) {
    const int angleRoll = rand() % 360;
    const int angle = angleRoll >= 0 ? angleRoll : 0;
    const int phaseRoll = rand() % 3;
    const int phase = phaseRoll >= 0 ? phaseRoll : 0;

    for (int j = 0; j < this->mode; ++j) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->rowIndex = i;
      particle->columnIndex = j;
      particle->groupIndex = 1;
      particle->angle = static_cast<float>(360 / this->count * i);
      particle->SetTexture("effect\\thunder_center.bmp");
      particle->alpha = 0;
      particle->scale = 0.8f;
      particle->speedParameter = 0.15f;
      particle->animationParameter = 8.0f;
      particle->phaseParameter = angle;
      particle->animationParameters[0] = static_cast<float>(phase);
      particle->animationParameters[1] = -12.0f;
      if (j == 0) {
        particle->destinationBlend = 6;
      }

      switch (this->id) {
      case EF_CHOOKGI:
        particle->SetColor(particle->columnIndex != 0 ? 255 : 0, particle->columnIndex != 0 ? 255 : 0, particle->columnIndex != 0 ? 0 : 255);
        break;
      case EF_CHOOKGI_FIRE:
        particle->SetTexture("effect\\FireOrb.bmp");
        particle->scale = 2.3f;
        break;
      case EF_CHOOKGI_WIND:
        particle->SetTexture("effect\\LightningOrb.bmp");
        particle->scale = 2.3f;
        break;
      case EF_CHOOKGI_WATER:
        particle->SetTexture("effect\\WaterOrb.bmp");
        particle->scale = 1.3f;
        break;
      case EF_CHOOKGI_GROUND:
        particle->SetTexture("effect\\StoneOrb.bmp");
        particle->scale = 2.3f;
        break;
      case EF_OB_GENSOU:
        particle->SetTexture("effect\\fog1.tga");
        particle->scale = 6.0f;
        particle->speedParameter = 0.05f;
        particle->animationParameter = 4.0f;
        particle->animationParameters[0] = 0.0f;
        particle->animationParameters[1] = -8.0f;
        particle->destinationBlend = 6;
        break;
      }

      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B51240
// FUNCTION: fn_19026
bool CEnergyOrbEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B51250
// FUNCTION: fn_19027
void CEnergyOrbEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    if (particle) {
      if (particle->elapsedFrames <= 200) {
        ++particle->alpha;
      }

      float angle = particle->speedParameter * 5.0f + particle->angle;
      if (angle > 360.0f) {
        angle -= 360.0f;
      }
      particle->angle = angle;

      if (particle->alpha > 0) {
        matrix rotation;
        std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
        rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
        rotation.AppendXRotation(3.141592f);

        const float scale = particle->scale;
        particle->phaseParameter = static_cast<int>(static_cast<float>(particle->phaseParameter) + particle->animationParameters[0]) % 360;
        float pulse = scale * 0.1f * g_sinTable[particle->phaseParameter];
        if (this->mode > 1 && particle->columnIndex == 0) {
          pulse = static_cast<float>(scale * 0.1 + pulse);
        }
        const float size = scale + pulse;
        matrix scaling;
        std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
        scaling.SetScale(size, size, size);

        const float wave = g_sinTable[particle->phaseParameter] * 0.6f;
        const int index = static_cast<int>(particle->angle);
        const float radius = wave + particle->animationParameter;
        matrix translation;
        std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
        translation.MakeTranslate(radius * g_cosTable[index], wave * particle->animationParameters[0] + particle->animationParameters[1], radius * g_sinTable[index]);

        matrix scaled;
        scaled.Compose(scaling, rotation);
        matrix result;
        result.Compose(scaled, translation);
        particle->transform = result;
      }
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B51750
// FUNCTION: fn_19028
void CEnergyOrbEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x00B517F0
// FUNCTION: fn_19029
void CEnergyOrbEffect::SetCount(int value) {
  if (this->count != value) {
    this->count = value;
    this->Open();
  }
}
