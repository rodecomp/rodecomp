#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Network/ActorNamePacket.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIPetInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <string>

// OFFSET: 0x00D1DFB0
// FUNCTION: fn_21632
void CGameMode::ReceiveActorNameAll(const ActorNameAllPacket* packet) {
  this->nameCard.Set(packet->actorId, std::string(packet->name, packet->name + sizeof(packet->name)).c_str(),
    std::string(packet->partyName, packet->partyName + sizeof(packet->partyName)).c_str(),
    std::string(packet->guildName, packet->guildName + sizeof(packet->guildName)).c_str(),
    std::string(packet->positionName, packet->positionName + sizeof(packet->positionName)).c_str(), std::string(), std::string(), 0);
}

// OFFSET: 0x00D1E3D0
// FUNCTION: fn_21633
void CGameMode::ReceiveActorNameTitle(const ActorNameTitlePacket* packet) {
  this->nameCard.Set(packet->actorId, std::string(packet->name, packet->name + sizeof(packet->name)).c_str(), std::string(), std::string(), std::string(),
    std::string(), packet->title, packet->groupId);

  if (packet->actorId == g_session.unknown_1010) {
    std::sprintf(g_session.unknown_1018, packet->name);
    if (UIPetInfoWnd* window = static_cast<UIPetInfoWnd*>(g_windowMgr.window_038C)) {
      window->nameEdit->SetText(packet->name);
    }
  }
}
