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
// FUNCTION: fn_46729
// VIRTUALIZED: emulated 0x00B08F6A
void PacketLengths::InitializeProtected() {
  this->Register(0x0437, 7, 7, 0);
  this->Register(0x0438, 10, 10, 0);
  this->Register(0x035f, 5, 5, 0);
  this->Register(0x0360, 6, 6, 0);
  this->Register(0x0b80, 10, 10, 0);
  this->Register(0x0361, 5, 5, 0);
  this->Register(0x0362, 6, 6, 0);
  this->Register(0x0363, 6, 6, 0);
  this->Register(0x0364, 8, 8, 0);
  this->Register(0x0365, 8, 8, 0);
  this->Register(0x0366, 10, 10, 0);
  this->Register(0x0367, 31, 31, 0);
  this->Register(0x0368, 6, 6, 0);
  this->Register(0x0369, 6, 6, 0);
  this->Register(0x083c, 14, 14, 0);
  this->Register(0x0838, 2, 2, 0);
  this->Register(0x0835, -1, 15, 0);
  this->Register(0x0819, -1, 12, 0);
  this->Register(0x0817, 6, 6, 0);
  this->Register(0x0815, 2, 2, 0);
  this->Register(0x0811, -1, 89, 0);
  this->Register(0x0802, 18, 18, 0);
  this->Register(0x07ec, 8, 8, 0);
  this->Register(0x07e4, -1, 12, 0);
  this->Register(0x0436, 23, 23, 0);
  this->Register(0x26ac, 23, 23, 0);
  this->Register(0x0bb9, 27, 27, 0);
  this->Register(0x02c4, 26, 26, 0);
  this->Register(0x0281, 4, 4, 0);
  this->Register(0x0202, 26, 26, 0);
  this->Register(0x022d, 5, 5, 0);
  this->Register(0x023b, 36, 36, 0);
  this->Register(0x0c1f, 1000, 1000, 0);
  this->Register(0x085a, 2, 2, 0);
  this->Register(0x085b, 2, 2, 0);
  this->Register(0x085c, 2, 2, 0);
  this->Register(0x085d, 2, 2, 0);
  this->Register(0x085e, 2, 2, 0);
  this->Register(0x085f, 2, 2, 0);
  this->Register(0x0860, 2, 2, 0);
  this->Register(0x0861, 2, 2, 0);
  this->Register(0x0862, 2, 2, 0);
  this->Register(0x0863, 2, 2, 0);
  this->Register(0x0864, 2, 2, 0);
  this->Register(0x0865, 2, 2, 0);
  this->Register(0x0866, 2, 2, 0);
  this->Register(0x0867, 2, 2, 0);
  this->Register(0x0868, 2, 2, 0);
  this->Register(0x0869, 2, 2, 0);
  this->Register(0x086a, 2, 2, 0);
  this->Register(0x086b, 2, 2, 0);
  this->Register(0x086c, 2, 2, 0);
  this->Register(0x086d, 2, 2, 0);
  this->Register(0x086e, 2, 2, 0);
  this->Register(0x086f, 2, 2, 0);
  this->Register(0x0870, 2, 2, 0);
  this->Register(0x0871, 2, 2, 0);
  this->Register(0x0872, 2, 2, 0);
  this->Register(0x0873, 2, 2, 0);
  this->Register(0x0874, 2, 2, 0);
  this->Register(0x0875, 2, 2, 0);
  this->Register(0x0876, 2, 2, 0);
  this->Register(0x0877, 2, 2, 0);
  this->Register(0x0878, 2, 2, 0);
  this->Register(0x0879, 2, 2, 0);
  this->Register(0x087a, 2, 2, 0);
  this->Register(0x087b, 2, 2, 0);
  this->Register(0x087c, 2, 2, 0);
  this->Register(0x087d, 2, 2, 0);
  this->Register(0x087e, 2, 2, 0);
  this->Register(0x087f, 2, 2, 0);
  this->Register(0x0880, 2, 2, 0);
  this->Register(0x0881, 2, 2, 0);
  this->Register(0x0882, 2, 2, 0);
  this->Register(0x0883, 2, 2, 0);
  this->Register(0x0917, 2, 2, 0);
  this->Register(0x0918, 2, 2, 0);
  this->Register(0x0919, 2, 2, 0);
  this->Register(0x091a, 2, 2, 0);
  this->Register(0x091b, 2, 2, 0);
  this->Register(0x091c, 2, 2, 0);
  this->Register(0x091d, 2, 2, 0);
  this->Register(0x091e, 2, 2, 0);
  this->Register(0x091f, 2, 2, 0);
  this->Register(0x0920, 2, 2, 0);
  this->Register(0x0921, 2, 2, 0);
  this->Register(0x0922, 2, 2, 0);
  this->Register(0x0923, 2, 2, 0);
  this->Register(0x0924, 2, 2, 0);
  this->Register(0x0925, 2, 2, 0);
  this->Register(0x0926, 2, 2, 0);
  this->Register(0x0927, 2, 2, 0);
  this->Register(0x0928, 2, 2, 0);
  this->Register(0x0929, 2, 2, 0);
  this->Register(0x092a, 2, 2, 0);
  this->Register(0x092b, 2, 2, 0);
  this->Register(0x092c, 2, 2, 0);
  this->Register(0x092d, 2, 2, 0);
  this->Register(0x092e, 2, 2, 0);
  this->Register(0x092f, 2, 2, 0);
  this->Register(0x0930, 2, 2, 0);
  this->Register(0x0931, 2, 2, 0);
  this->Register(0x0932, 2, 2, 0);
  this->Register(0x0933, 2, 2, 0);
  this->Register(0x0934, 2, 2, 0);
  this->Register(0x0935, 2, 2, 0);
  this->Register(0x0936, 2, 2, 0);
  this->Register(0x0937, 2, 2, 0);
  this->Register(0x0938, 2, 2, 0);
  this->Register(0x0939, 2, 2, 0);
  this->Register(0x093a, 2, 2, 0);
  this->Register(0x093b, 2, 2, 0);
  this->Register(0x093c, 2, 2, 0);
  this->Register(0x093d, 2, 2, 0);
  this->Register(0x093e, 2, 2, 0);
  this->Register(0x093f, 2, 2, 0);
  this->Register(0x0940, 2, 2, 0);
  this->Register(0x0884, 2, 2, 0);
  this->Register(0x0885, 2, 2, 0);
  this->Register(0x0886, 2, 2, 0);
  this->Register(0x0887, 2, 2, 0);
  this->Register(0x0888, 2, 2, 0);
  this->Register(0x0889, 2, 2, 0);
  this->Register(0x088a, 2, 2, 0);
  this->Register(0x088b, 2, 2, 0);
  this->Register(0x088c, 2, 2, 0);
  this->Register(0x088d, 2, 2, 0);
  this->Register(0x088e, 2, 2, 0);
  this->Register(0x088f, 2, 2, 0);
  this->Register(0x0890, 2, 2, 0);
  this->Register(0x0891, 2, 2, 0);
  this->Register(0x0892, 2, 2, 0);
  this->Register(0x0893, 2, 2, 0);
  this->Register(0x0894, 2, 2, 0);
  this->Register(0x0895, 2, 2, 0);
  this->Register(0x0896, 2, 2, 0);
  this->Register(0x0897, 2, 2, 0);
  this->Register(0x0898, 2, 2, 0);
  this->Register(0x0899, 2, 2, 0);
  this->Register(0x089a, 2, 2, 0);
  this->Register(0x089b, 2, 2, 0);
  this->Register(0x089c, 2, 2, 0);
  this->Register(0x089d, 2, 2, 0);
  this->Register(0x089e, 2, 2, 0);
  this->Register(0x089f, 2, 2, 0);
  this->Register(0x08a0, 2, 2, 0);
  this->Register(0x08a1, 2, 2, 0);
  this->Register(0x08a2, 2, 2, 0);
  this->Register(0x08a3, 2, 2, 0);
  this->Register(0x08a4, 2, 2, 0);
  this->Register(0x08a5, 2, 2, 0);
  this->Register(0x08a6, 2, 2, 0);
  this->Register(0x08a7, 2, 2, 0);
  this->Register(0x08a8, 2, 2, 0);
  this->Register(0x08a9, 2, 2, 0);
  this->Register(0x08aa, 2, 2, 0);
  this->Register(0x08ab, 2, 2, 0);
  this->Register(0x08ac, 2, 2, 0);
  this->Register(0x08ad, 2, 2, 0);
  this->Register(0x0941, 2, 2, 0);
  this->Register(0x0942, 2, 2, 0);
  this->Register(0x0943, 2, 2, 0);
  this->Register(0x0944, 2, 2, 0);
  this->Register(0x0945, 2, 2, 0);
  this->Register(0x0946, 2, 2, 0);
  this->Register(0x0947, 2, 2, 0);
  this->Register(0x0948, 2, 2, 0);
  this->Register(0x0949, 2, 2, 0);
  this->Register(0x094a, 2, 2, 0);
  this->Register(0x094b, 2, 2, 0);
  this->Register(0x094c, 2, 2, 0);
  this->Register(0x094d, 2, 2, 0);
  this->Register(0x094e, 2, 2, 0);
  this->Register(0x094f, 2, 2, 0);
  this->Register(0x0950, 2, 2, 0);
  this->Register(0x0951, 2, 2, 0);
  this->Register(0x0952, 2, 2, 0);
  this->Register(0x0953, 2, 2, 0);
  this->Register(0x0954, 2, 2, 0);
  this->Register(0x0955, 2, 2, 0);
  this->Register(0x0956, 2, 2, 0);
  this->Register(0x0957, 2, 2, 0);
  this->Register(0x0958, 2, 2, 0);
  this->Register(0x0959, 2, 2, 0);
  this->Register(0x095a, 2, 2, 0);
  this->Register(0x095b, 2, 2, 0);
  this->Register(0x095c, 2, 2, 0);
  this->Register(0x095d, 2, 2, 0);
  this->Register(0x095e, 2, 2, 0);
  this->Register(0x095f, 2, 2, 0);
  this->Register(0x0960, 2, 2, 0);
  this->Register(0x0961, 2, 2, 0);
  this->Register(0x0962, 2, 2, 0);
  this->Register(0x0963, 2, 2, 0);
  this->Register(0x0964, 2, 2, 0);
  this->Register(0x0965, 2, 2, 0);
  this->Register(0x0966, 2, 2, 0);
  this->Register(0x0967, 2, 2, 0);
  this->Register(0x0968, 2, 2, 0);
  this->Register(0x0969, 2, 2, 0);
  this->Register(0x096a, 2, 2, 0);
  this->Register(0x0c1f, 1000, 1000, 0);
}

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
