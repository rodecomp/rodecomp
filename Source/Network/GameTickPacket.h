#ifndef RAGNAROK_SOURCE_NETWORK_GAMETICKPACKET_H
#define RAGNAROK_SOURCE_NETWORK_GAMETICKPACKET_H

#include <cstdint>
#include <list>

class CConnection;
class PacketLengths;

void SendGameTick(bool force, std::uint32_t& pendingTick, std::uint32_t& lastTick, const std::uint32_t& interval,
  CConnection& connection, const PacketLengths& lengths, const bool& recording, std::uint32_t& firstRecordedTick,
  std::list<std::uint32_t>& recordedTicks);

#endif
