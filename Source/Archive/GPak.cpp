#include "Archive/GPak.h"
#include "Archive/PakCipher.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <emmintrin.h>
#include <zlib.h>

namespace {

  std::uint32_t Read32(const std::uint8_t* bytes) {
    std::uint32_t value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
  }

  std::uint64_t Read64(const std::uint8_t* bytes) {
    std::uint64_t value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
  }

  std::uint8_t SwapNibbles(std::uint8_t byte) {
    return static_cast<std::uint8_t>((byte << 4) | (byte >> 4));
  }

  // OFFSET: 0x00658FC0
  // FUNCTION: fn_05116
  std::uint64_t ReadAuxiliaryIdentifier(const void* data, std::size_t size) {
    if (size != 0) {
      const __m128i header = _mm_loadu_si128(static_cast<const __m128i*>(data));
      if (static_cast<std::uint8_t>(_mm_cvtsi128_si32(header)) <= 1) {
        std::uint64_t identifier;
        _mm_storel_epi64(reinterpret_cast<__m128i*>(&identifier), _mm_srli_si128(header, 4));
        return identifier;
      }
    }
    return UINT64_MAX;
  }

  void SortEntries(std::vector<myGPack::PakEntry>& entries) {
    std::sort(entries.begin(), entries.end(), [](const auto& first, const auto& second) {
      return first.name < second.name;
    });
  }

}

// OFFSET: 0x00E555A0
// INLINED
CGPak::CGPak() : m_file(nullptr), m_version(0), m_entryCount(0), m_tableOffset(0), m_tableSize(0) {
  this->Reset();
  InitializeCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x00AE97D0
// FUNCTION: fn_18107
// INLINED
// OFFSET: 0x00AE98C0
CGPak::~CGPak() {
  DeleteCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x00AEBC00
// FUNCTION: fn_18130
void CGPak::Reset() {
  this->m_entries.clear();
  this->m_file = nullptr;
  this->m_version = 0;
  this->m_entryCount = 0;
  this->m_tableOffset = 0;
  this->m_tableSize = 0;
  this->m_scratch.clear();
}

// OFFSET: 0x00AEBDB0
// FUNCTION: fn_18132
bool CGPak::Open(CPakFile* file) {
  if (!file) {
    return false;
  }
  this->m_file = file;
  const std::uint8_t* header = file->Read(0, 46);
  if (std::memcmp(header, "Event Horizon", 14) == 0) {
    this->m_version = Read32(header + 42);
    this->m_entryCount = Read32(header + 38);
    this->m_tableOffset = Read64(header + 30) + 46;
  } else if (std::memcmp(header, "Master of Magic", 16) == 0) {
    this->m_version = Read32(header + 42);
    this->m_entryCount = Read32(header + 38) - Read32(header + 34) - 7;
    this->m_tableOffset = static_cast<std::uint64_t>(Read32(header + 30)) + 46;
  } else {
    return false;
  }

  bool loaded;
  switch (this->m_version & 0xFF00) {
  case 0x100:
    loaded = this->ReadTableV1();
    break;
  case 0x200:
    loaded = this->ReadTableV2();
    break;
  case 0x300:
    loaded = this->ReadTableV3();
    break;
  default:
    loaded = false;
    break;
  }
  if (loaded) {
    return true;
  }
  this->Reset();
  return false;
}

// OFFSET: 0x00AEC050
// FUNCTION: fn_18133
bool CGPak::ReadTableV1() {
  this->m_tableSize =
    static_cast<std::uint32_t>(this->m_file->GetSize()) - static_cast<std::uint32_t>(this->m_tableOffset);
  const std::uint8_t* cursor = this->m_file->Read(this->m_tableOffset, this->m_tableSize);
  this->m_entries.resize(this->m_entryCount);

  std::uint32_t maximum = 0;
  std::uint32_t sequence = 1;
  std::int32_t legacyKey = 95001;
  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    std::uint32_t nameSize = Read32(cursor);
    cursor += 4;
    char name[256];
    if (this->m_version < 0x101) {
      std::int32_t length = static_cast<std::int32_t>(std::strlen(reinterpret_cast<const char*>(cursor)) + 1);
      for (std::int32_t j = 0; j < length; ++j) {
        name[j] = static_cast<char>(SwapNibbles(cursor[j]));
      }
    } else {
      if (this->m_version > 0x103) {
        return false;
      }
      cursor += 2;
      std::int32_t length = static_cast<std::int32_t>(nameSize - 6);
      for (std::int32_t j = 0; j < length; ++j) {
        name[j] = static_cast<char>(SwapNibbles(cursor[j]));
      }
      std::int32_t number = legacyKey;
      if (this->m_version != 0x101) {
        number = static_cast<std::int32_t>(3 * (31669u - (sequence >> 1)));
      }
      if (number < 10000) {
        number += 85000;
      }
      char digits[20];
      _itoa(number, digits, 10);
      std::uint8_t key[8];
      key[0] = 'P';
      key[1] = 'r';
      std::memcpy(key + 2, digits, 5);
      key[7] = 'e';
      myGPack::PakCipher cipher(key);
      std::uint8_t decoded[256];
      int decodedLength;
      cipher.Decode(reinterpret_cast<const std::uint8_t*>(name), length, decoded, &decodedLength, 1);
      std::strcpy(name, reinterpret_cast<const char*>(decoded));
      nameSize -= 2;
    }

    const std::uint8_t* metadata = cursor + nameSize;
    cursor = metadata + 17;
    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = Read32(metadata) - Read32(metadata + 8) - 715;
    entry.alignedSize = Read32(metadata + 4) - 37579;
    entry.originalSize = Read32(metadata + 8);
    entry.flags = metadata[12];
    entry.dataOffset = Read32(metadata + 13);
    entry.name.Assign(name);

    const char* extension = std::strrchr(name, '.');
    bool special = extension && (_stricmp(extension, ".gnd") == 0 || _stricmp(extension, ".gat") == 0 ||
                                 _stricmp(extension, ".act") == 0 || _stricmp(extension, ".str") == 0);
    entry.flags |= special ? 4 : 2;
    maximum = std::max(maximum, entry.alignedSize);
    sequence += 5;
    legacyKey -= 2;
  }
  SortEntries(this->m_entries);
  this->m_scratch.resize(maximum);
  return true;
}

// OFFSET: 0x00AEC420
// FUNCTION: fn_18134
bool CGPak::ReadTableV2() {
  const std::uint8_t* prefix = this->m_file->Read(this->m_tableOffset, 8);
  std::uint32_t compressed = Read32(prefix);
  std::uint32_t original = Read32(prefix + 4);
  this->m_tableSize = compressed + 8;
  const std::uint8_t* source = this->m_file->Read(this->m_tableOffset + 8, compressed);
  uLongf length = original + 8;
  std::vector<std::uint8_t> decoded(length);
  if (uncompress(decoded.data(), &length, source, compressed) != Z_OK) {
    return false;
  }

  this->m_entries.resize(this->m_entryCount);
  const std::uint8_t* cursor = decoded.data();
  std::uint32_t maximum = 0;
  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    const char* name = reinterpret_cast<const char*>(cursor);
    const std::uint8_t* metadata = cursor + std::strlen(name) + 1;
    cursor = metadata + 17;
    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = Read32(metadata);
    entry.alignedSize = Read32(metadata + 4);
    entry.originalSize = Read32(metadata + 8);
    entry.flags = metadata[12];
    entry.dataOffset = Read32(metadata + 13);
    entry.name.Assign(name);
    maximum = std::max(maximum, entry.alignedSize);
  }
  SortEntries(this->m_entries);
  this->m_scratch.resize(maximum);
  return true;
}

// OFFSET: 0x00AEC620
// FUNCTION: fn_18135
bool CGPak::ReadTableV3() {
  const std::uint8_t* prefix = this->m_file->Read(this->m_tableOffset, 12);
  std::uint32_t extra = Read32(prefix);
  std::uint32_t compressed = Read32(prefix + 4);
  std::uint32_t original = Read32(prefix + 8);
  this->m_tableSize = extra + compressed + 12;
  const std::uint8_t* source = this->m_file->Read(this->m_tableOffset + 12, compressed);
  uLongf length = original + 8;
  std::vector<std::uint8_t> decoded(length);
  if (uncompress(decoded.data(), &length, source, compressed) != Z_OK) {
    return false;
  }

  this->m_entries.resize(this->m_entryCount);
  const std::uint8_t* cursor = decoded.data();
  std::uint32_t maximum = 0;
  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    const char* name = reinterpret_cast<const char*>(cursor);
    const std::uint8_t* metadata = cursor + std::strlen(name) + 1;
    cursor = metadata + 21;
    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = Read32(metadata);
    entry.alignedSize = Read32(metadata + 4);
    entry.originalSize = Read32(metadata + 8);
    entry.flags = metadata[12];
    const std::uint32_t offsetLow = Read32(metadata + 13);
    reinterpret_cast<std::uint32_t*>(&entry.dataOffset)[1] = Read32(metadata + 17);
    reinterpret_cast<std::uint32_t*>(&entry.dataOffset)[0] = offsetLow;
    entry.name.Assign(name);
    maximum = std::max(maximum, entry.alignedSize);
  }
  SortEntries(this->m_entries);
  this->m_scratch.resize(maximum);
  return true;
}

// OFFSET: 0x00AEBA30
// FUNCTION: fn_18128
bool CGPak::Find(const myGPack::PakName& name, myGPack::PakEntry* entry) const {
  auto found = std::lower_bound(
    this->m_entries.begin(), this->m_entries.end(), name,
    [](const myGPack::PakEntry& candidate, const myGPack::PakName& key) { return candidate.name < key; }
  );
  if (found == this->m_entries.end() || found->name != name) {
    return false;
  }
  if (entry) {
    std::memcpy(entry, &*found, sizeof(*entry));
  }
  return true;
}

// OFFSET: 0x00AEBAE0
// FUNCTION: fn_18129
void CGPak::ReadAuxiliary(std::vector<std::uint8_t>& output, std::uint64_t* identifier) {
  if (identifier) {
    *identifier = UINT64_MAX;
  }

  output.clear();

  std::uint32_t size;
  std::uint64_t offset;
  switch (this->m_version & 0xFF00) {
  case 0x200:
    if (this->m_version < 0x201) {
      return;
    }

    size = Read32(this->m_file->Read(this->m_tableOffset - 4, 4));
    offset = this->m_tableOffset - size - 4;
    break;
  case 0x300: {
    const std::uint8_t* prefix = this->m_file->Read(this->m_tableOffset, 12);
    size = Read32(prefix);
    offset = this->m_tableOffset + Read32(prefix + 4) + 12;
    break;
  }
  default:
    return;
  }

  if (!size) {
    return;
  }

  const std::uint8_t* data = this->m_file->Read(offset, size);
  output.resize(size);
  std::memcpy(output.data(), data, size);

  if (identifier) {
    *identifier = ReadAuxiliaryIdentifier(output.data(), output.size());
  }
}
