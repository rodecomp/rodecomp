#ifndef RAGNAROK_SOURCE_NETWORK_PACKETLENGTHS_H
#define RAGNAROK_SOURCE_NETWORK_PACKETLENGTHS_H

#include <map>

struct PacketLengthValue {
  int length;
  int variable;
  int record;
};

struct PacketLengthResult {
  int kind;
  int length;
};

// Source view of packet metadata map.
struct PacketLengths {
  const std::map<int, PacketLengthValue>& values;
  PacketLengthResult Lookup(int id) const;
  int FixedLength(int id) const;
  int IsRecorded(int id) const;
  static void Initialize(std::map<int, PacketLengthValue>& values);
};

#endif
