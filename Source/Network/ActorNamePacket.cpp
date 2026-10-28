#include "Client/Mode/CGameMode.h"
#include "Network/ActorNamePacket.h"

#include <string>

// OFFSET: 0x00D1DFB0
// FUNCTION: fn_21632
void CGameMode::ReceiveActorNameAll(const ActorNameAllPacket* packet) {
  this->nameCard.Set(packet->actorId, std::string(packet->name, packet->name + sizeof(packet->name)).c_str(),
    std::string(packet->partyName, packet->partyName + sizeof(packet->partyName)).c_str(),
    std::string(packet->guildName, packet->guildName + sizeof(packet->guildName)).c_str(),
    std::string(packet->positionName, packet->positionName + sizeof(packet->positionName)).c_str(), std::string(), std::string(), 0);
}
