#include "Contents/Emblems/EmblemDownload.h"
#include "Network/Connection/Connection.h"
#include "Utility/Async/AsyncDiagnostic.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace {
#pragma pack(push, 1)
  struct EmblemPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::int16_t bytes;
    /* 0004 */ std::uint16_t state;
    /* 0006 */ std::uint32_t guildId;
    /* 000A */ std::uint32_t version;
    /* 000E */ unsigned char data[1010];
  };
#pragma pack(pop)
  static_assert(sizeof(EmblemPacket) == 1024);
}

// OFFSET: 0x005EB800
// FUNCTION: fn_03975
void CEmblemDataMgr::CompleteDownload(AsyncDiagnostic diagnostic, int responseCode, std::string response, std::uint32_t guildId, std::uint32_t version) {
  if (!diagnostic.success || responseCode != 200) {
    return;
  }

  auto* packet = static_cast<EmblemPacket*>(
    _MallocaComputeSize(sizeof(EmblemPacket)) <= _ALLOCA_S_THRESHOLD
      ? _MarkAllocaS(_alloca(_MallocaComputeSize(sizeof(EmblemPacket))), _ALLOCA_S_STACK_MARKER)
      : _MarkAllocaS(std::malloc(_MallocaComputeSize(sizeof(EmblemPacket))), _ALLOCA_S_HEAP_MARKER)
  );
  if (!packet) {
    return;
  }

  packet->id = 0x0B36;
  packet->bytes = 14;
  packet->state = 2;
  packet->guildId = guildId;
  packet->version = version;
  GetRagConnection().Prepend(packet->bytes, packet);

  int remaining = static_cast<int>(response.size());
  while (remaining > 0) {
    const int bytes = std::min(1010, remaining);
    packet->id = 0x0B36;
    packet->state = 1;
    packet->bytes = static_cast<std::int16_t>(bytes + 14);
    packet->guildId = guildId;
    packet->version = version;
    memcpy_s(packet->data, packet->bytes - 14, response.substr(remaining - bytes, remaining).c_str(), bytes);
    GetRagConnection().Prepend(packet->bytes, packet);
    remaining -= bytes;
  }

  packet->id = 0x0B36;
  packet->bytes = 14;
  packet->state = 0;
  packet->guildId = guildId;
  packet->version = version;
  GetRagConnection().Prepend(packet->bytes, packet);

  _freea(packet);
}
