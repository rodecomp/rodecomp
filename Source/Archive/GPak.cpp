#include "Archive/GPak.h"
#include "Archive/GPakUpdate.h"
#include "Archive/PakAuxiliary.h"
#include "Archive/PakCipher.h"
#include "Utility/AES.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Hash/XxHash32.h"
#include "Utility/Random/RandomBytes.h"
#include "Utility/Hash/XxHash3.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <emmintrin.h>
#include <functional>
#include <random>
#include <span>
#include <unordered_map>
#include <xhash>
#include <zlib.h>

namespace {

#pragma pack(push, 1)

  struct PakHeaderFile {
    /* 0000 */ char signature[16];
    /* 0010 */ std::uint8_t padding_0010[14];
    /* 001E */ std::uint32_t tableOffset;
    /* 0022 */ std::uint32_t seed;
    /* 0026 */ std::uint32_t entryCount;
    /* 002A */ std::uint32_t version;
  };

  struct PakFileHeader {
    /* 0000 */ char signature[16];
    /* 0010 */ std::uint8_t padding_0010[14];
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

#pragma pack(pop)

  static_assert(sizeof(PakHeaderFile) == 46);
  static_assert(offsetof(PakHeaderFile, tableOffset) == 30);
  static_assert(offsetof(PakHeaderFile, seed) == 34);
  static_assert(offsetof(PakHeaderFile, entryCount) == 38);
  static_assert(offsetof(PakHeaderFile, version) == 42);
  static_assert(sizeof(PakFileHeader) == 46);
  static_assert(offsetof(PakFileHeader, tableOffset) == 30);
  static_assert(offsetof(PakFileHeader, entryCount) == 38);
  static_assert(offsetof(PakFileHeader, version) == 42);
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

  // INLINED
  char SwapNibbles(char c) {
    return static_cast<char>((c << 4) | ((c >> 4) & 0x0F));
  }

  // INLINED
  void ModifyString(char* dest, const char* src, std::int32_t size) {
    for (std::int32_t i = 0; i < size; ++i) {
      dest[i] = SwapNibbles(src[i]);
    }
  }
}

// OFFSET: 0x00AE97D0
// FUNCTION: fn_18107
CGPak::~CGPak() {
  DeleteCriticalSection(&this->m_criticalSection);
}

// OFFSET: 0x00AEBC00
// FUNCTION: fn_18130
void CGPak::Reset() {
  this->Clear();
}

// INLINED
inline void CGPak::Clear() {
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
  // GLOBAL: gl_25504
  EncryptedName<14, 0x050505679903579BULL>& eventHorizon = OBFUSCATE_STRING("Event Horizon", 0x050505679903579BULL);

  if (std::memcmp(bytes, eventHorizon.Text(), 14) == 0) {
    const PakFileHeader* header = reinterpret_cast<const PakFileHeader*>(bytes);
    this->m_version = header->version;
    this->m_entryCount = header->entryCount;
    this->m_tableOffset = header->tableOffset + sizeof(PakFileHeader);
  } else {
    // GLOBAL: gl_25505
    EncryptedName<16, 0x83EFBFBB37EDBDBDULL>& masterOfMagic = OBFUSCATE_STRING("Master of Magic", 0x83EFBFBB37EDBDBDULL);

    if (std::memcmp(bytes, masterOfMagic.Text(), 16) != 0) {
      return false;
    }

    const PakHeaderFile* header = reinterpret_cast<const PakHeaderFile*>(bytes);
    this->m_version = header->version;
    this->m_entryCount = header->entryCount - header->seed - 7;
    this->m_tableOffset = static_cast<std::uint64_t>(header->tableOffset) + sizeof(PakHeaderFile);
  }

  bool loaded = false;
  const std::uint32_t version = this->m_version & 0xFF00;
  if (version == 0x100) {
    loaded = this->ReadTableV1();
  } else if (version == 0x200) {
    loaded = this->ReadTableV2();
  } else if (version == 0x300) {
    loaded = this->ReadTableV3();
  }

  if (loaded == true) {
    return true;
  }

  this->Clear();
  return false;
}

// STRING: st_14036
char g_specialPakExtensions[4][5] = {".gnd", ".gat", ".act", ".str"};

// INLINED
static bool IsNeverEncrypt(const char* fileName) {
  fileName = std::strrchr(fileName, '.');

  if (fileName) {
    for (const char* extension : g_specialPakExtensions) {
      if (_stricmp(fileName, extension) == 0) {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x00AEC050
// FUNCTION: fn_18133
bool CGPak::ReadTableV1() {
  this->m_tableSize = static_cast<std::uint32_t>(this->m_file->GetSize()) - static_cast<std::uint32_t>(this->m_tableOffset);
  const std::uint8_t* cursor = this->m_file->Read(this->m_tableOffset, this->m_tableSize);
  this->m_entries.resize(this->m_entryCount);

  std::uint32_t maximum = 0;

  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    std::int32_t length = *reinterpret_cast<const std::uint32_t*>(cursor);
    cursor += sizeof(std::uint32_t);

    char name[256];
    if (this->m_version < 0x101) {
      ModifyString(name, reinterpret_cast<const char*>(cursor), static_cast<std::int32_t>(std::strlen(reinterpret_cast<const char*>(cursor)) + 1));
    } else if (this->m_version <= 0x103) {
      std::int32_t number;
      int decodedLength;
      char decoded[256];
      std::uint8_t key[8];
      char digits[20];
      cursor += 2;
      ModifyString(name, reinterpret_cast<const char*>(cursor), length - 6);

      if (this->m_version == 0x101) {
        number = 95000 - i * 2 + 1;
      } else {
        number = 95000 - ((i * 5 + 1) / 2) * 3 + 7;
      }

      if (number < 10000) {
        number += 85000;
      }

      _itoa(number, digits, 10);
      std::memcpy(key + 2, digits, 5);
      key[0] = 'P';
      key[1] = 'r';
      key[7] = 'e';

      myGPack::PakCipher cipher(key);
      cipher.Decode(reinterpret_cast<const std::uint8_t*>(name), length - 6, reinterpret_cast<std::uint8_t*>(decoded), &decodedLength, 1);
      std::strcpy(name, decoded);
      length -= 2;
    } else {
      return false;
    }

    cursor += length;

    const PakEntryFile* fileEntry = reinterpret_cast<const PakEntryFile*>(cursor);
    cursor += sizeof(PakEntryFile);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry->compressedSize - (fileEntry->originalSize + 715);
    entry.alignedSize = fileEntry->alignedSize - 37579;
    entry.originalSize = fileEntry->originalSize;
    entry.flags = fileEntry->flags;
    entry.dataOffset = fileEntry->dataOffset;
    entry.name.Assign(name);

    if (IsNeverEncrypt(name)) {
      entry.flags |= 4;
    } else {
      entry.flags |= 2;
    }

    maximum = std::max(maximum, entry.alignedSize);
  }

  std::sort(this->m_entries.begin(), this->m_entries.end(), [](const auto& first, const auto& second) { return first.name < second.name; });
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

  if (uncompress(decoded.empty() ? nullptr : &decoded[0], &length, source, table.compressedSize) != Z_OK) {
    return false;
  }

  const std::uint8_t* cursor = decoded.empty() ? nullptr : &decoded[0];
  this->m_entries.resize(this->m_entryCount);
  std::uint32_t maximum = 0;

  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    const char* name = reinterpret_cast<const char*>(cursor);
    cursor += std::strlen(name);
    const PakEntryFile* fileEntry = reinterpret_cast<const PakEntryFile*>(cursor + 1);
    cursor += sizeof(PakEntryFile) + 1;

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry->compressedSize;
    entry.alignedSize = fileEntry->alignedSize;
    entry.originalSize = fileEntry->originalSize;
    entry.flags = fileEntry->flags;
    entry.dataOffset = fileEntry->dataOffset;
    entry.name.Assign(name);
    maximum = std::max(maximum, entry.alignedSize);
  }

  std::sort(this->m_entries.begin(), this->m_entries.end(), [](const auto& first, const auto& second) { return first.name < second.name; });
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

  const std::uint8_t* cursor = decoded.data();
  this->m_entries.resize(this->m_entryCount);
  std::uint32_t maximum = 0;

  for (std::uint32_t i = 0; i < this->m_entryCount; ++i) {
    const char* name = reinterpret_cast<const char*>(cursor);
    cursor += std::strlen(name) + 1;

    const PakEntryFileV3* fileEntry = reinterpret_cast<const PakEntryFileV3*>(cursor);
    cursor += sizeof(PakEntryFileV3);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry->compressedSize;
    entry.alignedSize = fileEntry->alignedSize;
    entry.originalSize = fileEntry->originalSize;
    entry.flags = fileEntry->flags;
    entry.dataOffset = fileEntry->dataOffset;
    entry.name.Assign(name);
    maximum = std::max(maximum, entry.alignedSize);
  }

  std::sort(this->m_entries.begin(), this->m_entries.end(), [](const auto& first, const auto& second) { return first.name < second.name; });
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

  output.resize(0);

  std::uint32_t size;
  std::uint64_t offset;
  const std::uint32_t format = this->m_version & 0xFF00;

  if (format == 0x100) {
    return;
  } else if (format == 0x200) {
    if (this->m_version < 0x201) {
      return;
    }

    std::memcpy(&size, this->m_file->Read(this->m_tableOffset - sizeof(size), sizeof(size)), sizeof(size));
    offset = this->m_tableOffset - size - sizeof(size);
  } else if (format == 0x300) {
    const PakTableFileV3* table = reinterpret_cast<const PakTableFileV3*>(this->m_file->Read(this->m_tableOffset, sizeof(PakTableFileV3)));
    size = table->auxiliarySize;
    offset = this->m_tableOffset + table->compressedSize + sizeof(PakTableFileV3);
  } else {
    return;
  }

  if (!size) {
    return;
  }

  const std::uint8_t* data = this->m_file->Read(offset, size);
  output.resize(size);
  std::memcpy(output.data(), data, size);

  if (identifier) {
    *identifier = ReadAuxiliaryIdentifier(output);
  }
}

// OFFSET: 0x00E555A0
// FUNCTION: fn_24306
myGPack::CGPakUpdate::CGPakUpdate() {
}

// OFFSET: 0x00E55940
// FUNCTION: fn_24308
__declspec(noinline) myGPack::CGPakUpdate::~CGPakUpdate() {
  this->Close();
}

