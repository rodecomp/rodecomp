#include "Contents/GuildInfo.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"

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

// OFFSET: 0x00AE4740
