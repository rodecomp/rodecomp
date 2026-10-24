#include "Network/PartyJoinPacket.h"
#include "UI/UIRequestJoinPartyMgr.h"

#include <cstring>

// OFFSET: 0x00D3BCE0
// FUNCTION: fn_21789
void ReceivePartyJoinRequest(const void* packet) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  char name[25];
  std::strncpy(name, reinterpret_cast<const char*>(bytes + 10), 24);
  name[24] = '\0';
  std::uint32_t account;
  std::uint32_t character;
  std::int16_t job;
  std::int16_t level;
  std::memcpy(&level, bytes + 36, sizeof(level));
  std::memcpy(&job, bytes + 34, sizeof(job));
  std::memcpy(&account, bytes + 2, sizeof(account));
  std::memcpy(&character, bytes + 6, sizeof(character));
  // The original loads the manager pointer straight from its global and
  // dereferences it without a null check; the dispatch already registered it.
  g_requestJoinPartyMgr->Add(character, account, name, job, level);
}
