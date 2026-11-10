#include "Contents/GuildInfo.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/MessageTable.h"
#include "Contents/World/World.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
// GLOBAL: gl_25465
GuildInfo g_guildInfo;

// OFFSET: 0x00AE38C0
// FUNCTION: fn_18026
GuildInfo::GuildInfo() {
  this->Init();
}

// OFFSET: 0x00AE40C0
// FUNCTION: fn_18029
void GuildInfo::AddBannedMember(const Unknown00ECEntry& entry) {
  this->bannedMembers.push_back(entry);
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00AE41C0
// FUNCTION: fn_18030
void GuildInfo::ChangeMemberPosition(int aid, int cid, int position) {
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->accountId == aid && it->characterId == cid) {
      it->positionId = position;
      it->positionName = this->m_positions[it->positionId];
      return;
    }
  }
}

// OFFSET: 0x00AE4230
// FUNCTION: fn_18031
void GuildInfo::ClearAgitIds() {
  this->agitIds.clear();
}

// OFFSET: 0x00AE4240
// FUNCTION: fn_18032
void GuildInfo::ClearMembers() {
  this->m_members.clear();
}

// OFFSET: 0x00AE4270
// FUNCTION: fn_18033
void GuildInfo::RemoveAgitId(int value) {
  auto it = std::find(this->agitIds.begin(), this->agitIds.end(), value);
  if (it != this->agitIds.end()) {
    this->agitIds.erase(it);
  }
}

// OFFSET: 0x00AE42C0
// FUNCTION: fn_18034
void GuildInfo::RemoveRelatedGuild(int unknown_0000, int unknown_0004) {
  for (auto it = this->relatedGuilds.begin(); it != this->relatedGuilds.end(); ++it) {
    if (it->guildId == unknown_0000 && it->relation == unknown_0004) {
      this->relatedGuilds.erase(it);
      g_windowMgr.RefreshGuildWindows();
      return;
    }
  }
}

// OFFSET: 0x00AE4380
// FUNCTION: fn_18035
int GuildInfo::GetMemberAccountId(const char* name) {
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->name == name) {
      return it->accountId;
    }
  }
  return -1;
}

// OFFSET: 0x00AE43E0
// FUNCTION: fn_18036
int GuildInfo::GetAgitId(int index) {
  return this->agitIds[index];
}

// OFFSET: 0x00AE4690
// FUNCTION: fn_18037
int GuildInfo::CountRelatedGuilds(int unknown_0004) {
  int count = 0;
  for (auto it = this->relatedGuilds.begin(); it != this->relatedGuilds.end(); ++it) {
    if (it->relation == unknown_0004) {
      ++count;
    }
  }
  return count;
}

// OFFSET: 0x00AE46C0
// FUNCTION: fn_18038
int GuildInfo::GetAgitCount() {
  return static_cast<int>(this->agitIds.size());
}

// OFFSET: 0x00AE4740
// FUNCTION: fn_18039
void GuildInfo::Init() {
  this->m_guildId = 0;
  if (g_modeMgr.GetGameMode() &&
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene &&
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SetGdid(0);
  }

  this->canInvite = 0;
  this->canExpel = 0;
  this->isMaster = 0;
  this->masterCharacterId = 0;
  this->requestState = 0;
  this->guildTabBitmask = -1;
  this->infoRequested = 0;
  this->membersRequested = 0;
  this->positionsRequested = 0;
  this->skillsRequested = 0;
  this->bansRequested = 0;
  this->guildName = "";
  this->masterName = "";
  this->subject = "";
  this->notice = "";
  this->guildLevel = 0;
  this->memberCount = 0;
  this->maxMemberCount = 0;
  this->memberAverageLevel = 0;
  this->manageLand = "";
  this->exp = 0;
  this->nextExp = 0;
  this->emblem = 0;
  this->points = 0;
  this->honor = 0;
  this->virtue = 0;
  this->positions.clear();
  this->m_members.clear();
  this->relatedGuilds.clear();
  this->bannedMembers.clear();
  this->guildList.clear();
  this->m_positions.clear();
  this->pendingMemberPositions.clear();
  this->pendingPositionChanges.clear();
  this->agitIds.clear();
}

// OFFSET: 0x00AE49F0
// FUNCTION: fn_18040
bool GuildInfo::HasRelatedGuild(int unknown_0000) {
  for (auto it = this->relatedGuilds.begin(); it != this->relatedGuilds.end(); ++it) {
    if (it->guildId == unknown_0000 && it->relation == 0) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00AE4A20
// FUNCTION: fn_18041
void GuildInfo::UpdateMemberExpRatios() {
  double total = 0.0;
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    total += it->exp;
  }

  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (total == 0.0) {
      it->expRatio = 0.0f;
    } else {
      it->expRatio = static_cast<float>(it->exp) * 100.0f / static_cast<float>(total);
    }
    it->positionName = this->m_positions[it->positionId];
  }
}

// OFFSET: 0x00AE4C00
// FUNCTION: fn_18042
int GuildInfo::DeleteMemberByCharacterId(int characterId) {
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->characterId == characterId) {
      if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene) {
        CRenderObject* actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetActorByGID(it->characterId);
        if (actor != nullptr) {
          static_cast<CGameActor*>(actor)->SetGdid(0);
        }
      }
      this->m_members.erase(it);
      g_windowMgr.RefreshGuildWindows();
      return 1;
    }
  }
  return 0;
}

// OFFSET: 0x00AE4CA0
// FUNCTION: fn_18043
int GuildInfo::DeleteMember(const char* name) {
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->name == name) {
      this->m_members.erase(it);
      g_windowMgr.RefreshGuildWindows();
      return 1;
    }
  }
  return 0;
}

// OFFSET: 0x00AE4D30
// FUNCTION: fn_18044
void GuildInfo::UpdateCharacterName(int characterId, const char* name) {
  if (this->masterCharacterId == characterId) {
    this->masterName = name;
  }

  std::string unknownName = LookupMessage(MSI_NAMELESS_USER);
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->characterId == characterId) {
      if (it->name.empty() || it->name == unknownName) {
        it->name = name;
      }
      break;
    }
  }

  for (auto it = this->bannedMembers.begin(); it != this->bannedMembers.end(); ++it) {
    if (it->characterId == characterId) {
      if (it->name.empty() || it->name == unknownName) {
        it->name = name;
      }
      break;
    }
  }

  if (g_windowMgr.GetGuildWindow()) {
    g_windowMgr.GetGuildWindow()->Invalidate();
  }
}

// OFFSET: 0x00AE4F50
// FUNCTION: fn_18045
void GuildInfo::UpdateMemberStatus(int accountId, int online, int characterId, short unknown_002C, short unknown_0020, short unknown_0024) {
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (characterId == it->characterId) {
      it->sex = unknown_002C;
      it->head = unknown_0020;
      it->headPalette = unknown_0024;
      it->online = online;
      break;
    }
  }

  int count = 0;
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->online) {
      ++count;
    }
  }
  this->onlineMemberCount = count;
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00AE4FD0
// FUNCTION: fn_18046
void GuildInfo::SetMemberOnline(int accountId, int online) {
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (accountId == it->accountId) {
      it->online = online;
      break;
    }
  }

  int count = 0;
  for (auto it = this->m_members.begin(); it != this->m_members.end(); ++it) {
    if (it->online) {
      ++count;
    }
  }
  this->onlineMemberCount = count;
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00AE5030
// FUNCTION: fn_18047
void GuildInfo::SetMasterCharacterId(int characterId) {
  this->masterCharacterId = characterId;
  this->isMaster = characterId == static_cast<int>(g_session.m_gid);
}

// OFFSET: 0x00AE5090
// FUNCTION: fn_18048
void GuildInfo::UpdatePosition(Unknown00D4Entry entry) {
  for (auto it = this->positions.begin(); it != this->positions.end(); ++it) {
    if (it->positionId == entry.positionId) {
      *it = entry;
      return;
    }
  }
}

// OFFSET: 0x00AE3F40
// FUNCTION: fn_18027
void GuildInfo::AddAgitId(int value) {
  const auto found = std::find_if(this->agitIds.begin(), this->agitIds.end(),
                                  [value](int entry) { return value == entry; });
  if (found != this->agitIds.end()) {
    return;
  }

  this->agitIds.push_back(value);
}

// OFFSET: 0x00AE3FA0
// FUNCTION: fn_18028
void GuildInfo::AddMember(const Member& member) {
  this->m_members.push_back(member);
  if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene) {
    CRenderObject* actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetActorByGID(member.characterId);
    if (actor != nullptr) {
      static_cast<CGameActor*>(actor)->SetGdid(this->m_guildId);
    }
  }
}
