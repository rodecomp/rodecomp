#include "Contents/Effects/TalismanMarkEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <algorithm>
#include <array>
#include <string>

CParticle* sub_B82ED0(const vector3d* position, int flags);
void sub_B82FD0(CParticle* particle);

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

// OFFSET: 0x0072E020
// FUNCTION: fn_07502
void CTalismanMarkEffect::PlayEnd() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CEZ2STRParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int index = particle->unknown_000C;
    const int angle = particle->unknown_004C;
    sub_B82FD0(particle);

    particle = static_cast<CEZ2STRParticle*>(sub_B82ED0(reinterpret_cast<const vector3d*>(this->position), 13));
    if (!particle) {
      *it = nullptr;
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = index;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    InitParticleQuad(particle->vertices);
    particle->m_actorId = this->actor;
    particle->Load(s_talismanEndEffects.at(index));
    particle->unknown_004C = angle;
    particle->unknown_00A8 = 2;
    *it = particle;
  }
}

// OFFSET: 0x0072E0F0
// FUNCTION: fn_07503
void CTalismanMarkEffect::Slot01() {
  this->retained = 1;

  for (int i = 0; i < std::min<unsigned int>(this->unknown_00B0, 4); ++i) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(sub_B82ED0(reinterpret_cast<const vector3d*>(this->position), 13));
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    InitParticleQuad(particle->vertices);
    particle->m_actorId = this->actor;
    particle->Load(s_talismanStartEffects.at(i));
    particle->unknown_004C = (i / 2 + i % 2 * 2) * 90;
    particle->unknown_00A8 = 0;
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x0072E240
// FUNCTION: fn_07505
void CTalismanMarkEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end();) {
    CEZ2STRParticle* particle = *it;
    if (!particle) {
      continue;
    }

    switch (particle->unknown_00A8) {
    case 0:
      if (particle->life == 0.0f || particle->life * 60.0f > particle->unknown_00E8) {
        break;
      }

      {
        const int index = particle->unknown_000C;
        const int angle = particle->unknown_004C;
        sub_B82FD0(particle);

        particle = static_cast<CEZ2STRParticle*>(sub_B82ED0(reinterpret_cast<const vector3d*>(this->position), 13));
        if (!particle) {
          *it = nullptr;
          continue;
        }

        particle->unknown_0004 = this->id;
        particle->unknown_000C = index;
        particle->unknown_0010 = 0;
        particle->unknown_0008 = 1;
        InitParticleQuad(particle->vertices);
        particle->m_actorId = this->actor;
        particle->Load(s_talismanLoopEffects.at(index));
        particle->unknown_004C = angle;
        particle->unknown_00A8 = 1;
        particle->m_loop = true;
        *it = particle;
      }
      break;
    case 1:
      break;
    case 2:
      if (particle->life == 0.0f || particle->life * 60.0f > particle->unknown_00E8) {
        break;
      }

      sub_B82FD0(particle);
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
bool CTalismanMarkEffect::Slot06() {
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

  const vector3d center = {this->position[0], this->position[1], this->position[2]};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  const float viewX = view[0] * center.x + view[3] * center.y + view[6] * center.z + view[9];
  const float viewY = view[1] * center.x + view[4] * center.y + view[7] * center.z + view[10];
  const float viewZ = view[2] * center.x + view[5] * center.y + view[8] * center.z + view[11];

  const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
  int angle = particle->unknown_004C;
  if (particle->unknown_00A8 == 1) {
    angle = (angle + 1) % 360;
    particle->unknown_004C = angle;
  }

  const float radians = static_cast<float>(angle) / 180.0f * 3.141592f;
  const float offsetY = ClientSin(radians) * 8.5f - 20.0f;
  const float offsetX = ClientCos(radians) * 8.5f;
  const float x = offsetX + viewX;
  const float y = offsetY + viewY;
  const float z = viewZ + 0.0f;
  const float worldX = x * inverse[0] + y * inverse[3] + z * inverse[6] + inverse[9];
  const float worldY = x * inverse[1] + y * inverse[4] + z * inverse[7] + inverse[10];
  const float worldZ = x * inverse[2] + y * inverse[5] + z * inverse[8] + inverse[11];
  particle->position.x = worldX;
  particle->position.y = worldY;
  particle->position.z = worldZ;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0072E5F0
// FUNCTION: fn_07507
void CTalismanMarkEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CEZ2STRParticle* particle = *it;
    if (particle) {
      particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
