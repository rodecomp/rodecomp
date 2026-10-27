#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_PACKETLENGTHS_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_PACKETLENGTHS_H

#include <cstddef>
#include <map>

struct PacketLengthValue {
  /* 0000 */ int length;
  /* 0004 */ int variable;
  /* 0008 */ int record;
};

struct PacketLengthResult {
  /* 0000 */ int kind;
  /* 0004 */ int length;
};

struct PacketLengths {
  const std::map<int, PacketLengthValue>& values;
  PacketLengthResult Lookup(int id) const;
  int FixedLength(int id) const;
  int IsRecorded(int id) const;
  static void Initialize(std::map<int, PacketLengthValue>& values);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PacketLengthValue) == 0x0C);
static_assert(offsetof(PacketLengthValue, variable) == 0x04);
static_assert(offsetof(PacketLengthValue, record) == 0x08);
static_assert(sizeof(PacketLengthResult) == 0x08);
static_assert(offsetof(PacketLengthResult, length) == 0x04);
#endif

#endif
