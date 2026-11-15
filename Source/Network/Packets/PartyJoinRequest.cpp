#include "Network/Packets/PartyJoinPacket.h"
#include "UI/Party/UIRequestJoinPartyMgr.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

#pragma pack(push, 1)
struct PACKET_ZC_PARTY_REQ_MASTER_TO_JOIN {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t characterId;
  /* 0006 */ std::uint32_t accountId;
  /* 000A */ char name[24];
  /* 0022 */ std::int16_t job;
  /* 0024 */ std::int16_t level;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PACKET_ZC_PARTY_REQ_MASTER_TO_JOIN) == 0x26);
static_assert(offsetof(PACKET_ZC_PARTY_REQ_MASTER_TO_JOIN, name) == 0x0A);
static_assert(offsetof(PACKET_ZC_PARTY_REQ_MASTER_TO_JOIN, level) == 0x24);
#endif

// OFFSET: 0x00D3BCE0
// FUNCTION: fn_21789
void __stdcall ReceivePartyJoinRequest(const void* packet) {
  const auto* request = static_cast<const PACKET_ZC_PARTY_REQ_MASTER_TO_JOIN*>(packet);
  char name[25];
  std::strncpy(name, request->name, 24);
  name[24] = '\0';

  g_requestJoinPartyMgr->Add(request->accountId, request->characterId, name, request->job, request->level);
}
