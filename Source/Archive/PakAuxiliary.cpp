#include "Archive/GPak.h"
#include "Archive/PakAuxiliary.h"

#include <cstring>

namespace {
#pragma pack(push, 1)
  struct PakAuxiliaryFile {
    /* 0000 */ std::uint8_t version;
    /* 0001 */ std::uint8_t padding_0001[3];
    /* 0004 */ std::uint64_t identifier;
    /* 000C */ std::uint32_t padding_000C;
  };
#pragma pack(pop)

  static_assert(sizeof(PakAuxiliaryFile) == 16);
  static_assert(offsetof(PakAuxiliaryFile, identifier) == 4);
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(PakUniformRandom, m_distribution) == 0x1388);
#endif

// OFFSET: 0x006563E0
// FUNCTION: fn_05073
__declspec(noinline) void FillRandomBytes(char* first, char* last, PakUniformRandom& random) {
  for (; first != last; ++first) {
    *first = static_cast<char>(random.m_distribution(random.m_engine));
  }
}

// OFFSET: 0x00656C70
// FUNCTION: fn_05080
PakAuxiliary::PakAuxiliary() : m_archiveKeys(), m_headerKey(), m_archiveKeyBlockKey(), m_streamKey() {
  this->Initialize();
}

// OFFSET: 0x00657640
// FUNCTION: fn_05091
PakAuxiliary::~PakAuxiliary() {
}

// OFFSET: 0x00656B60
// FUNCTION: fn_05078
__declspec(noinline) PakUniformRandom::PakUniformRandom(int minimum, int maximum, unsigned int seed) : m_engine(seed), m_distribution(minimum, maximum) {
}

// OFFSET: 0x00658FC0
// FUNCTION: fn_05116
std::uint64_t ReadAuxiliaryIdentifier(std::span<const std::uint8_t> data) {
  if (data.size() != 0) {
    PakAuxiliaryFile header;
    std::memcpy(&header, data.data(), sizeof(header));
    if (header.version <= 1) {
      return header.identifier;
    }
  }

  return UINT64_MAX;
}
