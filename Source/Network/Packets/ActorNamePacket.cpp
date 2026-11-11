#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Packets/ActorNamePacket.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Status/UIPetInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <string>

#include "Utility/StdStringCopy.h"

// OFFSET: 0x00D1DFB0
// FUNCTION: fn_21632
void CGameMode::ReceiveActorNameAll(const ActorNameAllPacket* packet) {
  this->nameCard.Set(packet->actorId, std::string(packet->name, packet->name + sizeof(packet->name)).c_str(),
    std::string(packet->partyName, packet->partyName + sizeof(packet->partyName)).c_str(),
    std::string(packet->guildName, packet->guildName + sizeof(packet->guildName)).c_str(),
    std::string(packet->positionName, packet->positionName + sizeof(packet->positionName)).c_str(), "", "", 0);
}

// OFFSET: 0x00D1E3D0
// FUNCTION: fn_21633
void CGameMode::ReceiveActorNameTitle(const ActorNameTitlePacket* packet) {
  this->nameCard.Set(packet->actorId, std::string(packet->name, packet->name + sizeof(packet->name)).c_str(), "", "", "", "",
    packet->title, packet->groupId);

  if (packet->actorId == g_session.petGid) {
    std::sprintf(g_session.petName, packet->name);
    if (UIPetInfoWnd* window = static_cast<UIPetInfoWnd*>(g_windowMgr.petInfoWnd)) {
      window->nameEdit->SetText(packet->name);
    }
  }
}

// OFFSET: 0x00D1DB50
// FUNCTION: fn_21631
void CGameMode::ReceivePlayerNameUpdate(const PlayerNameUpdatePacket* packet) {
  if (packet->type == 4 || packet->type == 3) {
    std::string name = packet->type == 4 ? std::string(LookupMessage(MSI_NAMELESS_USER))
                                         : std::string(std::string(packet->name, packet->name + sizeof(packet->name)).c_str());
    SetPlayerName(packet->characterId, name);
  }
}
