#include "Contents/ActorNameCard.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"

#include <algorithm>
#include <cstdint>
#include <windows.h>
#include <timeapi.h>

#pragma pack(push, 1)
struct PACKET_CZ_REQNAME2 {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint32_t gid;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PACKET_CZ_REQNAME2) == 6);
#endif

// OFFSET: 0x005BFD10
// FUNCTION: fn_03500
const CActorNameCard& CActorNameCardHolder::Lookup(std::uint32_t id) {
  const auto found = this->cards.find(id);
  if (found != this->cards.end() && found->second.received == 1) {
    return found->second;
  }

  if (std::find(this->pending.begin(), this->pending.end(), id) == this->pending.end()) {
    this->pending.push_back(id);
  }

  // GLOBAL: gl_24526
  static CActorNameCard empty;
  return empty;
}

// OFFSET: 0x005C01D0
// FUNCTION: fn_03504
void CActorNameCardHolder::ProcessRequests() {
  while (!this->requests.empty()) {
    const Request request = this->requests.front();
    if (timeGetTime() < request.tick + 10000) {
      break;
    }
    this->requests.pop_front();
  }

  while (!this->pending.empty()) {
    const std::uint32_t id = this->pending.front();
    const auto found = this->cards.find(id);
    if (found == this->cards.end() || found->second.received != 1) {
      const auto requested = std::find_if(this->requests.begin(), this->requests.end(), [id](const Request& request) { return request.id == id; });
      if (requested == this->requests.end()) {
        this->pending.pop_front();
        if (id != 0) {
          PACKET_CZ_REQNAME2 packet;
          packet.packetType = ID_CZ_REQNAME2;
          packet.gid = id;
          GetRagConnection().Send(sizeof(packet), &packet);
          this->requests.push_back({ id, timeGetTime() });
        }
        return;
      }
    }

    this->pending.pop_front();
  }
}
