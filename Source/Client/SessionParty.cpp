#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/World/World.h"

// OFFSET: 0x00E1DEC0
// FUNCTION: fn_23884
bool CSession::IsPartyMember(std::uint32_t aid, bool onlineOnly) {
  for (const MessengerMember& member : this->m_partyMembers) {
    if (member.accountId == aid) {
      if (onlineOnly && member.offline != 0) {
        return false;
      }

      return true;
    }
  }

  return false;
}

// OFFSET: 0x00E3D2D0
// FUNCTION: fn_24185
bool CSession::IsPvpMap() {
  if (g_modeMgr.GetGameMode()) {
    CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (scene) {
      return scene->unknown_00FD;
    }
  }

  return false;
}
