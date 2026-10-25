#include "Network/Packets/PacketLengths.h"

#include <algorithm>

// OFFSET: 0x00B09E50
// FUNCTION: fn_18388
PacketLengthResult PacketLengths::Lookup(int id) const {
  const auto entry = this->values.find(id);
  if (entry == this->values.end()) {
    return {-1, 4};
  }

  if (entry->second.length == -1) {
    return {0, entry->second.variable};
  }

  if (entry->second.length < 0) {
    return {-1, 4};
  }

  return {1, entry->second.length};
}

// OFFSET: 0x00C7C970
// FUNCTION: fn_20391
int PacketLengths::FixedLength(int id) const {
  if (id == 703) {
    return 12;
  }

  if (id == 704) {
    return 4;
  }

  return std::max(this->Lookup(id).length, 2);
}

namespace {
  struct PacketRegistration {
    int record;
    int variable;
    int length;
    int id;
  };
  // STATIC: 0x00B02294
  // GLOBAL: gl_00808
  constexpr PacketRegistration registrations[] = {
#include "Network/Packets/PacketRegistrations.inc"
};
}

// OFFSET: 0x00B02260
// FUNCTION: fn_18387
void PacketLengths::Initialize(std::map<int, PacketLengthValue>& values) {
  for (const PacketRegistration& value : registrations) {
    values.try_emplace(value.id, PacketLengthValue{value.length, value.variable, value.record});
  }
}

// OFFSET: 0x00B02210
// FUNCTION: fn_18386
int PacketLengths::IsRecorded(int id) const {
  const auto entry = this->values.find(id);
  return entry == this->values.end() ? 0 : entry->second.record;
}
