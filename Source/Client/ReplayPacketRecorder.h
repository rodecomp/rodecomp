#ifndef RAGNAROK_SOURCE_CLIENT_REPLAYPACKETRECORDER_H
#define RAGNAROK_SOURCE_CLIENT_REPLAYPACKETRECORDER_H

#include <cstdint>
#include <cstdio>

struct PacketLengths;

// Borrowed fields of the original replay owner. File creation, header writing
// and mode transitions remain the enclosing owner's responsibility.
struct ReplayPacketRecorder {
  const PacketLengths& lengths;
  const std::uint32_t& baseFrame;        // +0x10
  const std::uint32_t& started;          // +0x14
  const std::uint32_t& frame;            // +0x1C
  const std::uint32_t& key0;             // +0x2C
  const std::uint32_t& key1;             // +0x30
  const unsigned char& recordRestricted; // +0xB6
  std::FILE*& file;                      // +0x1DC
  std::uint32_t& fileOffset;             // +0x1E0
  std::uint32_t& records;                // +0x1E4
  std::uint32_t& cipherIndex;            // +0x1E8
  std::uint32_t& cipherBase;             // +0x1EC
  std::uint32_t& cipherKey;              // +0x1F0

  bool Record(std::uint16_t bytes, const void* packet, int event, std::uint32_t delay);
};

// UIWindow response serialization, gated by replay mode +0x0C.
struct WindowResponseRecorder {
  const int& mode;
  ReplayPacketRecorder& packets;

  void Record(int windowId, int response, std::intptr_t b, std::intptr_t c, std::intptr_t d);
};

#endif
