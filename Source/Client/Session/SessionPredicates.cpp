#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"

#include <algorithm>
#include <cstdint>
#include <map>

// OFFSET: 0x00E1D6B0
// FUNCTION: fn_23850
int CSession::IsAbracadabraSpecialSkill(int state) const {
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
  const auto it = std::find_if(this->trackedAchievements.begin(), this->trackedAchievements.end(), [id](SessionRecord6178 record) {
    return id == record.id;
  });
  return this->trackedAchievements.end() != it;
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
  if ((job > JT_MON_BEGIN && job < JT_MONSTER_LAST) || (job >= JT_EXTENDED_JOB_NAME_START && job <= 31999)) {
    return 1;
  }

  if (actor->GetEffectiveJob(-1) >= JT_EL_AGNI_S) {
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

  CPlayer* player = g_modeMgr.GetGameMode()->scene->player;
  if (player && aid == static_cast<std::uint32_t>(player->GetGdid())) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00E1DDF0
// FUNCTION: fn_23881
int CSession::IsHomunAid(std::uint32_t aid) const {
  if (aid == INVALID_AID) {
    return false;
  }
  return aid == this->m_homunAid;
}

// OFFSET: 0x00E1DE10
// FUNCTION: fn_23882
int CSession::IsMercenaryAid(std::uint32_t aid) const {
  if (aid == INVALID_AID) {
    return false;
  }
  return aid == this->m_mercenaryAid;
}

// OFFSET: 0x00E1DE30
// FUNCTION: fn_23883
int CSession::IsCompanionAid(std::uint32_t aid) const {
  if (aid == INVALID_AID) {
    return 0;
  }
  if (aid == this->m_homunAid ||
      aid == this->m_mercenaryAid || aid == this->petGid ||
      aid == static_cast<std::uint32_t>(this->elementalGid)) {
    return 1;
  }
  auto it = this->summonHp.find(aid);
  if (this->summonHp.end() == it || -1 == it->second) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x00E1DF00
// FUNCTION: fn_23885
int CSession::IsPetAid(std::uint32_t aid) const {
  if (aid == INVALID_AID) {
    return false;
  }
  return aid == this->petGid;
}
