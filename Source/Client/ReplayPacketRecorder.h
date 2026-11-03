#ifndef RAGNAROK_SOURCE_CLIENT_REPLAYPACKETRECORDER_H
#define RAGNAROK_SOURCE_CLIENT_REPLAYPACKETRECORDER_H
#include "Client/ReplayRecordOption.h"

#include <cstdint>
#include <cstdio>
#include <vector>

class PacketLengths;
struct ModeDragState;

struct ReplayPacketRecorder {
  const PacketLengths& lengths;
  const std::uint32_t& baseFrame;
  const std::uint32_t& started;
  const std::uint32_t& frame;
  const std::uint32_t& key0;
  const std::uint32_t& key1;
  const unsigned char& recordRestricted;
  std::FILE*& file;
  std::uint32_t& fileOffset;
  std::uint32_t& records;
  std::uint32_t& cipherIndex;
  std::uint32_t& cipherBase;
  std::uint32_t& cipherKey;

  bool Record(std::uint16_t bytes, const void* packet, int event, std::uint32_t delay);
};


#endif
