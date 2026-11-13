#include "Contents/Effects/Skills/TalismanMarkEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <array>
#include <string>

namespace {
// INLINED
__forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// GLOBAL: gl_24871
static const std::array<std::string, 4> s_talismanStartEffects = {
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\blue_dragon_start.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\white_tiger_start.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\red_phoenix_start.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\black_tortoise_start.str",
};

// GLOBAL: gl_24873
static const std::array<std::string, 4> s_talismanEndEffects = {
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\blue_dragon_end.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\white_tiger_end.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\red_phoenix_end.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\black_tortoise_end.str",
};

// GLOBAL: gl_24872
static const std::array<std::string, 4> s_talismanLoopEffects = {
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\blue_dragon_loop.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\white_tiger_loop.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\red_phoenix_loop.str",
  "SOUL_ASCETIC\\EFST_T_XXXX_GOD\\black_tortoise_loop.str",
};

// OFFSET: 0x0072DE70
// FUNCTION: fn_07499
CTalismanMarkEffect::CTalismanMarkEffect() : CEffect(-1) {
}

// OFFSET: 0x0072DFD0
// FUNCTION: fn_07501
void CTalismanMarkEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->particles.clear();
}

// OFFSET: 0x0072E020
// FUNCTION: fn_07502
void CTalismanMarkEffect::PlayEnd() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CEZ2STRParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int index = particle->columnIndex;
    const int angle = particle->phaseParameter;
    FreeParticle(particle);

    particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
    if (!particle) {
      *it = nullptr;
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = index;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->InitParticleQuad(particle->vertices);
    particle->m_actorId = this->actor;
    particle->Load(s_talismanEndEffects.at(index));
    particle->phaseParameter = angle;
    particle->animationState = 2;
    *it = particle;
  }
}

// OFFSET: 0x0072E0F0
// FUNCTION: fn_07503
void CTalismanMarkEffect::Open() {
  this->retained = 1;

  for (int i = 0; i < std::min<unsigned int>(this->effectLevel, 4); ++i) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->InitParticleQuad(particle->vertices);
    particle->m_actorId = this->actor;
    particle->Load(s_talismanStartEffects.at(i));
    particle->phaseParameter = (i / 2 + i % 2 * 2) * 90;
    particle->animationState = 0;
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x0072E240
// FUNCTION: fn_07505
void CTalismanMarkEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end();) {
    CEZ2STRParticle* particle = *it;
    if (!particle) {
      continue;
    }

    switch (static_cast<signed char>(particle->animationState)) {
    case 0:
      if (IsParticleActive(particle)) {
        break;
      }

      {
        const int index = particle->columnIndex;
        const int angle = particle->phaseParameter;
        FreeParticle(particle);

        particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
        if (!particle) {
          *it = nullptr;
          continue;
        }

        particle->effectId = this->id;
        particle->columnIndex = index;
        particle->rowIndex = 0;
        particle->groupIndex = 1;
        particle->InitParticleQuad(particle->vertices);
        particle->m_actorId = this->actor;
        particle->Load(s_talismanLoopEffects.at(index));
        particle->phaseParameter = angle;
        particle->animationState = 1;
        particle->m_loop = true;
        *it = particle;
      }
      break;
    case 1:
      break;
    case 2:
      if (IsParticleActive(particle)) {
        break;
      }

      FreeParticle(particle);
      it = this->particles.erase(it);
      continue;
    default:
      continue;
    }

    this->UpdateParticle(particle);
    ++it;
  }
}

// OFFSET: 0x0072E210
// FUNCTION: fn_07504
bool CTalismanMarkEffect::IsAlive() {
  if (this->ActorExists() == true && !this->particles.empty()) {
    return true;
  }

  return false;
}

// OFFSET: 0x0072E3F0
// FUNCTION: fn_07506
void CTalismanMarkEffect::UpdateParticle(CEZ2STRParticle* particle) {
  if (!particle) {
    return;
  }

  const vector3d center = this->positionVector;
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  const float viewX = center.x * view[0] + center.y * view[3] + center.z * view[6] + view[9];
  const float viewY = center.x * view[1] + center.y * view[4] + center.z * view[7] + view[10];
  const float viewZ = center.x * view[2] + center.y * view[5] + center.z * view[8] + view[11];

  const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
  int angle = particle->phaseParameter;
  if (particle->animationState == 1) {
    angle = (angle + 1) % 360;
    particle->phaseParameter = angle;
  }

  const float radians = static_cast<float>(angle) / 180.0f * 3.141592f;
  const float offsetY = ClientSin(radians) * 8.5f - 20.0f;
  const float offsetX = ClientCos(radians) * 8.5f;
  const float x = offsetX + viewX;
  const float y = offsetY + viewY;
  const float z = viewZ + 0.0f;
  const float worldX = y * inverse[3] + x * inverse[0] + z * inverse[6] + inverse[9];
  const float worldY = y * inverse[4] + x * inverse[1] + z * inverse[7] + inverse[10];
  const float worldZ = y * inverse[5] + x * inverse[2] + z * inverse[8] + inverse[11];
  particle->position.x = worldX;
  particle->position.y = worldY;
  particle->position.z = worldZ;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0072E5F0
// FUNCTION: fn_07507
void CTalismanMarkEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CEZ2STRParticle* particle = *it;
    if (particle) {
      particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
