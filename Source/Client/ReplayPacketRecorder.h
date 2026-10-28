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

struct WindowResponseRecorder {
  const int& mode;
  ReplayPacketRecorder& packets;

  void Record(int windowId, int response, std::intptr_t b, std::intptr_t c, std::intptr_t d);
};

struct WindowReplayRecorder {
  const int& windowId;
  const int* policy;
  const bool& restricted;
  ReplayPacketRecorder& packets;

  void RecordDrop(int x, int y, const ModeDragState& drag, int target);
  int RecordData(std::uint16_t type, std::vector<unsigned char> data);
  void RecordValues(int value, int argument1, int argument2, int argument3, int argument4);
  void RecordText(int event, const char* text);
};

#endif
