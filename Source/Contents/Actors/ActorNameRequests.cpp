#include "Contents/Actors/ActorNameCard.h"
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
  const auto found = std::as_const(this->cards).find(id);
  if (this->cards.end() == found || found->second.received != 1) {
    this->QueueRequest(id);

    // GLOBAL: gl_24526
    static CActorNameCard empty;
    return empty;
  }

  return found->second;
}

// OFFSET: 0x005BF810
// FUNCTION: fn_45909
void CActorNameCardHolder::ExpireRequests() {
  while (!this->requests.empty()) {
    const Request request = this->requests.front();
    if (timeGetTime() < request.tick + 10000) {
      break;
    }
    this->requests.pop_front();
  }
}

// OFFSET: 0x005BFF60
// FUNCTION: fn_45910
std::uint32_t CActorNameCardHolder::PopNextRequest() {
  while (!this->pending.empty()) {
    const int id = this->pending.front();
    const std::map<std::uint32_t, CActorNameCard>::const_iterator found = this->cards.find(id);
    if (found == this->cards.end() || found->second.received != 1) {
      const auto end = this->requests.end();
      const auto requested = std::find_if(this->requests.begin(), end, [id](const Request& request) { return id == request.id; });
      if (end == requested) {
        this->pending.pop_front();
        return id;
      }
    }

    this->pending.pop_front();
  }
  return 0;
}

// OFFSET: 0x005C0090
// FUNCTION: fn_45911
void CActorNameCardHolder::QueueRequest(std::uint32_t id) {
  const auto end = this->pending.cend();
  const auto found = std::find(this->pending.cbegin(), end, id);
  if (end == found) {
    this->pending.push_back(id);
  }
}

// OFFSET: 0x005C01D0
// FUNCTION: fn_03504
void CActorNameCardHolder::ProcessRequests() {
  this->ExpireRequests();
  const std::uint32_t id = this->PopNextRequest();
  if (id != 0) {
    PACKET_CZ_REQNAME2 packet;
    packet.packetType = ID_CZ_REQNAME2;
    packet.gid = id;
    GetRagConnection().Send(sizeof(packet), &packet);
    this->requests.push_back({ id, timeGetTime() });
  }
}
