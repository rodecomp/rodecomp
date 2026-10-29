#include "Network/ExecutableDigestPacket.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Utility/Md5State.h"

#include <cstdio>
#include <cstdint>
#include <cstring>

namespace {
#pragma pack(push, 1)
  struct ExecutableDigestPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ unsigned char digest[16];
  };
#pragma pack(pop)
  static_assert(sizeof(ExecutableDigestPacket) == 18);
}

// OFFSET: 0x00D8D790
// FUNCTION: fn_22267
void SendExecutableDigest(const int& serviceType, const int& serviceMode, HINSTANCE instance,
  CConnection& connection, const PacketLengths& lengths) {
  if (serviceType != 6 && (serviceType != 0 || serviceMode != 1)) {
    return;
  }

  ExecutableDigestPacket packet;
  packet.id = 0x0204;
  MD5State md5;
  md5.Init();
  char filename[MAX_PATH];
  GetModuleFileNameA(instance, filename, MAX_PATH);
  FILE* file = std::fopen(filename, "rb");
  if (!file) {
    return;
  }

  unsigned char buffer[1024];
  while (!std::feof(file)) {
    const int bytes = std::fread(buffer, 1, sizeof(buffer), file);
    md5.Append(buffer, bytes);
  }
  md5.Finish(packet.digest);

  if (serviceType == 0) {
    if (serviceMode == 1) {
      const std::uint32_t digest[4] = {0x912CD182, 0x8FD45A4F, 0x7ECF6FD9, 0x2D49CCF4};
      std::memcpy(packet.digest, digest, sizeof(digest));
    } else if (serviceMode == 0) {
      const std::uint32_t digest[4] = {0x3164E365, 0x11031C03, 0x136D501E, 0x7A2D6939};
      std::memcpy(packet.digest, digest, sizeof(digest));
    } else {
      std::memset(packet.digest, 0, sizeof(packet.digest));
    }
  } else if (serviceType == 6 && serviceMode == 1) {
    const std::uint32_t digest[4] = {0xC2940AC7, 0x9A38CC7A, 0x3954F547, 0x39D0A47C};
    std::memcpy(packet.digest, digest, sizeof(digest));
  }

  connection.Send(lengths.FixedLength(packet.id), &packet);
}
