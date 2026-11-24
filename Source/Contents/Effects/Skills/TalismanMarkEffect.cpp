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
void SetParticleIndices(CParticle& particle, int group, int column, int row) {
  particle.columnIndex = column;
  particle.rowIndex = row;
  particle.groupIndex = group;
}

// INLINED
__forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// INLINED
__forceinline vector3d TransformPoint(const matrix& transform, const vector3d point) {
  return vector3d{point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9],
                  point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10],
                  point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11]};
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

// OFFSET: 0x0072DEF0
// FUNCTION: fn_46033
__declspec(noinline) CTalismanMarkEffect::~CTalismanMarkEffect() {
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
    SetParticleIndices(*particle, 1, i, 0);
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

    switch (particle->animationState) {
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
  const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
  const vector3d viewPos = {center.x * view.values[0] + center.y * view.values[3] + center.z * view.values[6] + view.values[9],
                            center.x * view.values[1] + center.y * view.values[4] + center.z * view.values[7] + view.values[10],
                            center.x * view.values[2] + center.y * view.values[5] + center.z * view.values[8] + view.values[11]};

  const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;
  int angle = particle->phaseParameter;
  if (particle->animationState == 1) {
    angle = (angle + 1) % 360;
    particle->phaseParameter = angle;
  }

  const float radians = static_cast<float>(angle) / 180.0f * 3.141592f;
  const float offsetY = ClientSin(radians) * 8.5f - 20.0f;
  const float offsetX = ClientCos(radians) * 8.5f;
  const vector3d local = {offsetX + viewPos.x, offsetY + viewPos.y, viewPos.z + 0.0f};
  particle->position = TransformPoint(inverse, local);
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0072E640
// FUNCTION: fn_46282
__forceinline void __stdcall CTalismanMarkEffect::RenderParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x0072E5F0
// FUNCTION: fn_07507
void CTalismanMarkEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    RenderParticle(*it);
  }
}
