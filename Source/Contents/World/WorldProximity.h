#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDPROXIMITY_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDPROXIMITY_H

#include <cstdint>
#include <functional>

class CConnection;
struct PacketLengths;

void UpdateWorldProximityNotification(
  unsigned char& notified, std::function<bool()> nearby, CConnection& connection, const PacketLengths& lengths, const std::uint32_t& accountId
);

#endif
