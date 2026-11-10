#include "Client/Mode/CGameMode.h"
#include "Contents/Skills/SkillId.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"

#include <cstdint>

// OFFSET: 0x00E1D6B0
// FUNCTION: fn_23850
int CSession::IsState291To303(int state) const {
  switch (state) {
  case SA_MONOCELL:
  case SA_CLASSCHANGE:
  case SA_SUMMONMONSTER:
  case SA_REVERSEORCISH:
  case SA_DEATH:
  case SA_TAMINGMONSTER:
  case SA_QUESTION:
  case SA_GRAVITY:
  case SA_LEVELUP:
  case SA_INSTANTDEATH:
  case SA_FULLRECOVERY:
  case SA_COMA:
    return 1;
  default:
    return 0;
  }
}

// OFFSET: 0x00E1D700
// FUNCTION: fn_23851
bool CSession::HasTrackedAchievement(int id) const {
  auto it = this->trackedAchievements.begin();
  for (; it != this->trackedAchievements.end(); ++it) {
    const SessionRecord6178 record = *it;
    if (id == record.id) {
      break;
    }
  }
  return it != this->trackedAchievements.end();
}

// OFFSET: 0x00E1D850
// FUNCTION: fn_23852
int CSession::IsType0Or4(int type) const {
  if (type == 0 || type == 4) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00E1DD50
// FUNCTION: fn_23879
int CSession::IsMonsterActor(CGameActor* actor) const {
  if (!actor) {
    return 0;
  }

  const int job = actor->GetEffectiveJob(-1);
  if ((job > 1000 && job < 3999) || (job >= 20021 && job <= 31999)) {
    return 1;
  }

  if (actor->GetEffectiveJob(-1) >= 2114) {
    actor->GetEffectiveJob(-1);
  }
  return 0;
}

// OFFSET: 0x00E1DDB0
// FUNCTION: fn_23880
int CSession::IsLocalPlayerAid(std::uint32_t aid) const {
  if (aid == 0) {
    return 0;
  }

  CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  if (player && aid == static_cast<std::uint32_t>(player->GetGdid())) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00E1DDF0
// FUNCTION: fn_23881
bool CSession::IsHomunAid(std::uint32_t aid) const {
  if (aid == 0xFFFFFFFF) {
    return false;
  }
  return aid == *reinterpret_cast<const std::uint32_t*>(&this->spAndHomunculusStats[8]);
}

// OFFSET: 0x00E1DE10
// FUNCTION: fn_23882
bool CSession::IsMercenaryAid(std::uint32_t aid) const {
  if (aid == 0xFFFFFFFF) {
    return false;
  }
  return aid == *reinterpret_cast<const std::uint32_t*>(&this->companionTargetState[8]);
}

// OFFSET: 0x00E1DE30
// FUNCTION: fn_23883
int CSession::IsCompanionAid(std::uint32_t aid) const {
  if (aid == 0xFFFFFFFF) {
    return 0;
  }
  if (aid == *reinterpret_cast<const std::uint32_t*>(&this->spAndHomunculusStats[8]) ||
      aid == *reinterpret_cast<const std::uint32_t*>(&this->companionTargetState[8]) || aid == this->petGid ||
      aid == static_cast<std::uint32_t>(this->elementalGid)) {
    return 1;
  }
  auto it = this->summonHp.find(aid);
  if (it == this->summonHp.end() || it->second == -1) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x00E1DF00
// FUNCTION: fn_23885
bool CSession::IsPetAid(std::uint32_t aid) const {
  if (aid == 0xFFFFFFFF) {
    return false;
  }
  return aid == this->petGid;
}
