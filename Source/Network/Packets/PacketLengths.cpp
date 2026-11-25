#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"


// GLOBAL: gl_24489
PacketLengths g_packetLengths;

// OFFSET: 0x00B09E50
// FUNCTION: fn_18388
__declspec(noinline) PacketLengthResult PacketLengths::Lookup(int id) const {
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


// OFFSET: 0x00B08F60

// OFFSET: 0x00B02260
// FUNCTION: fn_18387
__declspec(noinline) void PacketLengths::Initialize() {
  this->InitializeProtected();
#define PACKET_LENGTH(id, length, variable, record) this->Register(id, length, variable, record);
#include "Network/Packets/PacketRegistrations.inc"
#undef PACKET_LENGTH
}

// OFFSET: 0x00B01E70
// FUNCTION: fn_18383
PacketLengths::PacketLengths() {
  this->Initialize();
}

// OFFSET: 0x00B01F40
// FUNCTION: fn_18384
PacketLengths::~PacketLengths() = default;

// OFFSET: 0x00B02100
// FUNCTION: fn_18385
__declspec(noinline) void PacketLengths::Register(int id, int length, int variable, int record) {
  this->m_values.insert(std::make_pair(id, PacketLengthValue{length, variable, record}));
}

// OFFSET: 0x00B02210
// FUNCTION: fn_18386
int PacketLengths::IsRecorded(int id) const {
  const auto entry = this->m_values.find(id);
  return entry == this->m_values.end() ? 0 : entry->second.record;
}
