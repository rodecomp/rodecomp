#include "Archive/GPak.h"
#include "Archive/GPakUpdate.h"
#include "Archive/PakCipher.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <zlib.h>

namespace {
#pragma pack(push, 1)

  struct PakHeaderFile {
    /* 0000 */ char signature[16];
    /* 0010 */ std::uint8_t unknown_0010[14];
    /* 001E */ std::uint32_t tableOffset;
    /* 0022 */ std::uint32_t seed;
    /* 0026 */ std::uint32_t entryCount;
    /* 002A */ std::uint32_t version;
  };

  struct PakHeaderFile64 {
    /* 0000 */ char signature[16];
    /* 0010 */ std::uint8_t unknown_0010[14];
    /* 001E */ std::uint64_t tableOffset;
    /* 0026 */ std::uint32_t entryCount;
    /* 002A */ std::uint32_t version;
  };

  struct PakTableFile {
    /* 0000 */ std::uint32_t compressedSize;
    /* 0004 */ std::uint32_t originalSize;
  };

  struct PakTableFileV3 {
    /* 0000 */ std::uint32_t auxiliarySize;
    /* 0004 */ std::uint32_t compressedSize;
    /* 0008 */ std::uint32_t originalSize;
  };

  struct PakEntryFile {
    /* 0000 */ std::uint32_t compressedSize;
    /* 0004 */ std::uint32_t alignedSize;
    /* 0008 */ std::uint32_t originalSize;
    /* 000C */ std::uint8_t flags;
    /* 000D */ std::uint32_t dataOffset;
  };

  struct PakEntryFileV3 {
    /* 0000 */ std::uint32_t compressedSize;
    /* 0004 */ std::uint32_t alignedSize;
    /* 0008 */ std::uint32_t originalSize;
    /* 000C */ std::uint8_t flags;
    /* 000D */ std::uint64_t dataOffset;
  };

  struct PakAuxiliaryFile {
    /* 0000 */ std::uint8_t version;
    /* 0001 */ std::uint8_t unknown_0001[3];
    /* 0004 */ std::uint64_t identifier;
  };

#pragma pack(pop)

  static_assert(sizeof(PakHeaderFile) == 46);
  static_assert(offsetof(PakHeaderFile, tableOffset) == 30);
  static_assert(offsetof(PakHeaderFile, seed) == 34);
  static_assert(offsetof(PakHeaderFile, entryCount) == 38);
  static_assert(offsetof(PakHeaderFile, version) == 42);
  static_assert(sizeof(PakHeaderFile64) == 46);
  static_assert(offsetof(PakHeaderFile64, tableOffset) == 30);
  static_assert(offsetof(PakHeaderFile64, entryCount) == 38);
  static_assert(offsetof(PakHeaderFile64, version) == 42);
  static_assert(sizeof(PakTableFile) == 8);
  static_assert(offsetof(PakTableFile, originalSize) == 4);
  static_assert(sizeof(PakTableFileV3) == 12);
  static_assert(offsetof(PakTableFileV3, compressedSize) == 4);
  static_assert(offsetof(PakTableFileV3, originalSize) == 8);
  static_assert(sizeof(PakEntryFile) == 17);
  static_assert(offsetof(PakEntryFile, flags) == 12);
  static_assert(offsetof(PakEntryFile, dataOffset) == 13);
  static_assert(sizeof(PakEntryFileV3) == 21);
  static_assert(offsetof(PakEntryFileV3, flags) == 12);
  static_assert(offsetof(PakEntryFileV3, dataOffset) == 13);
  static_assert(sizeof(PakAuxiliaryFile) == 12);
  static_assert(offsetof(PakAuxiliaryFile, identifier) == 4);

  // INLINED
  std::uint8_t SwapNibbles(std::uint8_t byte) {
    return static_cast<std::uint8_t>((byte << 4) | (byte >> 4));
  }

  // OFFSET: 0x00658FC0
  // FUNCTION: fn_05116
  std::uint64_t ReadAuxiliaryIdentifier(const void* data, std::size_t size) {
    if (size != 0) {
      const auto* bytes = static_cast<const std::uint8_t*>(data);
      if (bytes[0] <= 1) {
        PakAuxiliaryFile header;
        std::memcpy(&header, data, sizeof(header));
        return header.identifier;
      }
    }

    return UINT64_MAX;
  }

  // INLINED
  void SortEntries(std::vector<myGPack::PakEntry>& entries) {
    std::sort(entries.begin(), entries.end(), [](const auto& first, const auto& second) { return first.name < second.name; });
  }
}

// INLINED
CGPak::CGPak() : m_file(nullptr), m_version(0), m_entryCount(0), m_tableOffset(0), m_tableSize(0) {
  this->Reset();
  InitializeCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x00AE97D0
// OFFSET: 0x00AE98C0
// FUNCTION: fn_18107
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
  const std::uint8_t* bytes = file->Read(0, sizeof(PakHeaderFile));
  if (std::memcmp(bytes, "Event Horizon", 14) == 0) {
    PakHeaderFile64 header;
    std::memcpy(&header, bytes, sizeof(header));
    this->m_version = header.version;
    this->m_entryCount = header.entryCount;
    this->m_tableOffset = header.tableOffset + sizeof(header);
  } else if (std::memcmp(bytes, "Master of Magic", 16) == 0) {
    PakHeaderFile header;
    std::memcpy(&header, bytes, sizeof(header));
    this->m_version = header.version;
    this->m_entryCount = header.entryCount - header.seed - 7;
    this->m_tableOffset = static_cast<std::uint64_t>(header.tableOffset) + sizeof(header);
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
  this->m_tableSize = static_cast<std::uint32_t>(this->m_file->GetSize()) - static_cast<std::uint32_t>(this->m_tableOffset);
  const std::uint8_t* cursor = this->m_file->Read(this->m_tableOffset, this->m_tableSize);
  this->m_entries.resize(this->m_entryCount);

  std::uint32_t maximum = 0;
  std::uint32_t sequence = 1;
  std::int32_t legacyKey = 95001;

  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    std::uint32_t nameSize;
    std::memcpy(&nameSize, cursor, sizeof(nameSize));
    cursor += sizeof(nameSize);
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
    PakEntryFile fileEntry;
    std::memcpy(&fileEntry, metadata, sizeof(fileEntry));
    cursor = metadata + sizeof(fileEntry);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry.compressedSize - fileEntry.originalSize - 715;
    entry.alignedSize = fileEntry.alignedSize - 37579;
    entry.originalSize = fileEntry.originalSize;
    entry.flags = fileEntry.flags;
    entry.dataOffset = fileEntry.dataOffset;
    entry.name.Assign(name);

    const char* extension = std::strrchr(name, '.');
    bool special = extension && (_stricmp(extension, ".gnd") == 0 || _stricmp(extension, ".gat") == 0 || _stricmp(extension, ".act") == 0 ||
                                 _stricmp(extension, ".str") == 0);
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
  PakTableFile table;
  std::memcpy(&table, this->m_file->Read(this->m_tableOffset, sizeof(table)), sizeof(table));
  this->m_tableSize = table.compressedSize + sizeof(table);
  const std::uint8_t* source = this->m_file->Read(this->m_tableOffset + sizeof(table), table.compressedSize);
  uLongf length = table.originalSize + 8;
  std::vector<std::uint8_t> decoded(length);

  if (uncompress(decoded.data(), &length, source, table.compressedSize) != Z_OK) {
    return false;
  }

  this->m_entries.resize(this->m_entryCount);
  const std::uint8_t* cursor = decoded.data();
  std::uint32_t maximum = 0;

  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    const char* name = reinterpret_cast<const char*>(cursor);
    const std::uint8_t* metadata = cursor + std::strlen(name) + 1;
    PakEntryFile fileEntry;
    std::memcpy(&fileEntry, metadata, sizeof(fileEntry));
    cursor = metadata + sizeof(fileEntry);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry.compressedSize;
    entry.alignedSize = fileEntry.alignedSize;
    entry.originalSize = fileEntry.originalSize;
    entry.flags = fileEntry.flags;
    entry.dataOffset = fileEntry.dataOffset;
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
  PakTableFileV3 table;
  std::memcpy(&table, this->m_file->Read(this->m_tableOffset, sizeof(table)), sizeof(table));
  this->m_tableSize = table.auxiliarySize + table.compressedSize + sizeof(table);
  const std::uint8_t* source = this->m_file->Read(this->m_tableOffset + sizeof(table), table.compressedSize);
  uLongf length = table.originalSize + 8;
  std::vector<std::uint8_t> decoded(length);

  if (uncompress(decoded.data(), &length, source, table.compressedSize) != Z_OK) {
    return false;
  }

  this->m_entries.resize(this->m_entryCount);
  const std::uint8_t* cursor = decoded.data();
  std::uint32_t maximum = 0;

  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    const char* name = reinterpret_cast<const char*>(cursor);
    const std::uint8_t* metadata = cursor + std::strlen(name) + 1;
    PakEntryFileV3 fileEntry;
    std::memcpy(&fileEntry, metadata, sizeof(fileEntry));
    cursor = metadata + sizeof(fileEntry);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry.compressedSize;
    entry.alignedSize = fileEntry.alignedSize;
    entry.originalSize = fileEntry.originalSize;
    entry.flags = fileEntry.flags;
    entry.dataOffset = fileEntry.dataOffset;
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
  auto found = std::lower_bound(this->m_entries.begin(), this->m_entries.end(), name, [](const myGPack::PakEntry& candidate, const myGPack::PakName& key) {
    return candidate.name < key;
  });
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

    std::memcpy(&size, this->m_file->Read(this->m_tableOffset - sizeof(size), sizeof(size)), sizeof(size));
    offset = this->m_tableOffset - size - sizeof(size);
    break;
  case 0x300: {
    PakTableFileV3 table;
    std::memcpy(&table, this->m_file->Read(this->m_tableOffset, sizeof(table)), sizeof(table));
    size = table.auxiliarySize;
    offset = this->m_tableOffset + table.compressedSize + sizeof(table);
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

// OFFSET: 0x00AEC9F0
// FUNCTION: fn_18137
void CGPak::ApplyAuxiliary(const void* data, std::uint32_t size, const void* auxData, std::uint32_t auxSize) {
  this->m_auxiliary.Apply(data, size, auxData, auxSize);
}

// OFFSET: 0x00E555A0
// FUNCTION: fn_24306
myGPack::CGPakUpdate::CGPakUpdate() {
}
