#include "Network/Packets/PacketLengths.h"

#include <algorithm>

// GLOBAL: gl_24489
PacketLengths g_packetLengths;

// OFFSET: 0x00B09E50
// FUNCTION: fn_18388
PacketLengthResult PacketLengths::Lookup(int id) const {
  const auto entry = this->m_values.find(id);
  if (entry == this->m_values.end()) {
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

  return std::max(g_packetLengths.Lookup(id).length, 2);
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
void PacketLengths::Initialize() {
  for (const PacketRegistration& value : registrations) {
    this->m_values.try_emplace(value.id, PacketLengthValue{value.length, value.variable, value.record});
  }
}

// OFFSET: 0x00B01E70
// FUNCTION: fn_18383
PacketLengths::PacketLengths() {
  this->Initialize();
}

// OFFSET: 0x00B01F40
// FUNCTION: fn_18384
PacketLengths::~PacketLengths() {
}

// OFFSET: 0x00B02100
// FUNCTION: fn_18385
void PacketLengths::Register(int id, int length, int variable, int record) {
  this->m_values.try_emplace(id, PacketLengthValue{length, variable, record});
}

// OFFSET: 0x00B02210
// FUNCTION: fn_18386
int PacketLengths::IsRecorded(int id) const {
  const auto entry = this->m_values.find(id);
  return entry == this->m_values.end() ? 0 : entry->second.record;
}
