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

class PacketLengths {
public:
  PacketLengths();
  ~PacketLengths();
  PacketLengthResult Lookup(int id) const;
  int FixedLength(int id) const;
  int IsRecorded(int id) const;

private:
  /* 0000 */ std::map<int, PacketLengthValue> m_values;
  void Initialize();
  void Register(int id, int length, int variable, int record);
};

extern PacketLengths g_packetLengths;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PacketLengthValue) == 0x0C);
static_assert(offsetof(PacketLengthValue, variable) == 0x04);
static_assert(offsetof(PacketLengthValue, record) == 0x08);
static_assert(sizeof(PacketLengthResult) == 0x08);
static_assert(offsetof(PacketLengthResult, length) == 0x04);
static_assert(sizeof(PacketLengths) == 0x08);
#endif

#endif
