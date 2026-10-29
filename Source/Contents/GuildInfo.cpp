#include "Contents/GuildInfo.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "UI/Window/UIWindowMgr.h"

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
void GuildInfo::ClearUnknown0114() {
  this->unknown_0114.clear();
}

// OFFSET: 0x00AE4240
// FUNCTION: fn_18032
void GuildInfo::ClearMembers() {
  this->m_members.clear();
}

// OFFSET: 0x00AE4270
// FUNCTION: fn_18033
void GuildInfo::RemoveUnknown0114(int value) {
  for (auto it = this->unknown_0114.begin(); it != this->unknown_0114.end(); ++it) {
    if (*it == value) {
      this->unknown_0114.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00AE42C0
// FUNCTION: fn_18034
void GuildInfo::RemoveRelatedGuild(int unknown_0000, int unknown_0004) {
  for (auto it = this->unknown_00E4.begin(); it != this->unknown_00E4.end(); ++it) {
    if (it->unknown_0000 == unknown_0000 && it->unknown_0004 == unknown_0004) {
      this->unknown_00E4.erase(it);
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
int GuildInfo::GetUnknown0114(int index) {
  return this->unknown_0114[index];
}

// OFFSET: 0x00AE4690
// FUNCTION: fn_18037
int GuildInfo::CountRelatedGuilds(int unknown_0004) {
  int count = 0;
  for (auto it = this->unknown_00E4.begin(); it != this->unknown_00E4.end(); ++it) {
    if (it->unknown_0004 == unknown_0004) {
      ++count;
    }
  }
  return count;
}

// OFFSET: 0x00AE46C0
// FUNCTION: fn_18038
int GuildInfo::GetUnknown0114Count() {
  return static_cast<int>(this->unknown_0114.size());
}

// OFFSET: 0x00AE4740
// FUNCTION: fn_18039
void GuildInfo::Init() {
  this->m_guildId = 0;
  if (g_modeMgr.GetGameMode() &&
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene &&
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->Slot48(0);
  }

  this->unknown_00AC = 0;
  this->unknown_00B0 = 0;
  this->unknown_00B4 = 0;
  this->unknown_00D0 = 0;
  this->unknown_00B8 = 0;
  this->unknown_00A4 = -1;
  this->unknown_00BC = 0;
  this->unknown_00C0 = 0;
  this->unknown_00C4 = 0;
  this->unknown_00C8 = 0;
  this->unknown_00CC = 0;
  this->unknown_0000.clear();
  this->unknown_0018.clear();
  this->unknown_0030.clear();
  this->unknown_0048.clear();
  this->unknown_0060 = 0;
  this->unknown_0064 = 0;
  this->unknown_006C = 0;
  this->unknown_0070 = 0;
  this->unknown_0074.clear();
  this->unknown_008C = 0;
  this->unknown_0090 = 0;
  this->unknown_0094 = 0;
  this->unknown_0098 = 0;
  this->unknown_009C = 0;
  this->unknown_00A0 = 0;
  this->unknown_00D4.clear();
  this->m_members.clear();
  this->unknown_00E4.clear();
  this->unknown_00EC.clear();
  this->unknown_00F4.clear();
  this->m_positions.clear();
  this->unknown_0104.clear();
  this->unknown_010C.clear();
  this->unknown_0114.clear();
}
