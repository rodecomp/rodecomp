#include "Contents/EmblemDownload.h"
#include "Network/Connection.h"
#include "Utility/AsyncDiagnostic.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace {
#pragma pack(push, 1)
  struct EmblemPacket {
    std::uint16_t id;
    std::uint16_t bytes;
    std::uint16_t state;
    std::uint32_t guildId;
    std::uint32_t version;
    unsigned char data[1010];
  };
#pragma pack(pop)
  static_assert(sizeof(EmblemPacket) == 1024);

  struct EmblemPacketAllocation {
    std::uint32_t marker;
    std::uint32_t unused;
    EmblemPacket packet;
  };
  static_assert(sizeof(EmblemPacketAllocation) == 1032);
}

// OFFSET: 0x005EB800
// FUNCTION: fn_03975
void CompleteEmblemDownload(
  AsyncDiagnostic diagnostic,
  int responseCode,
  std::string response,
  std::uint32_t guildId,
  std::uint32_t version,
  CConnection& connection
) {
  if (!diagnostic.success || responseCode != 200) {
    return;
  }

  auto* storage = static_cast<EmblemPacketAllocation*>(std::malloc(sizeof(EmblemPacketAllocation)));

  if (!storage) {
    return;
  }

  storage->marker = 0xDDDD;
  EmblemPacket& packet = storage->packet;
  packet.id = 0x0B36;
  packet.bytes = 14;
  packet.state = 2;
  packet.guildId = guildId;
  packet.version = version;
  connection.Prepend(packet.bytes, &packet);
  int remaining = static_cast<int>(response.size());

  while (remaining > 0) {
    const int bytes = std::min(remaining, 1010);
    remaining -= bytes;
    packet.id = 0x0B36;
    packet.bytes = static_cast<std::uint16_t>(bytes + 14);
    packet.state = 1;
    packet.guildId = guildId;
    packet.version = version;
    // Original substr/copy includes bytes at remaining, not the consumed prefix.
    // Avoid its unnecessary temporary string while preserving the same bytes.
    std::memcpy(packet.data, response.data() + remaining, bytes);
    connection.Prepend(packet.bytes, &packet);
  }

  packet.id = 0x0B36;
  packet.bytes = 14;
  packet.state = 0;
  packet.guildId = guildId;
  packet.version = version;
  connection.Prepend(packet.bytes, &packet);

  if (storage->marker == 0xDDDD) {
    std::free(storage);
  }
}
