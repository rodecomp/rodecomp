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

// OFFSET: 0x00654410
// FUNCTION: fn_04984
int sub_654410() {
  return 8;
}

// OFFSET: 0x00654420
// FUNCTION: fn_04985
int sub_654420() {
  return 12;
}

// OFFSET: 0x00654430
// FUNCTION: fn_04986
int sub_654430() {
  return 8;
}

// OFFSET: 0x00654440
// FUNCTION: fn_04987
int sub_654440() {
  return 8;
}

// OFFSET: 0x00654450
// FUNCTION: fn_04988
int sub_654450() {
  return 8;
}

// OFFSET: 0x00654460
// FUNCTION: fn_04989
int sub_654460() {
  volatile int v = 0xf093c3a0;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654490
// FUNCTION: fn_04990
int sub_654490() {
  volatile int v = 0xf093c3a3;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006544C0
// FUNCTION: fn_04991
int sub_6544C0() {
  volatile int v = 0xf093c3a2;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006544F0
// FUNCTION: fn_04992
int sub_6544F0() {
  volatile int v = 0xf093c3ad;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654520
// FUNCTION: fn_04993
int sub_654520() {
  volatile int v = 0xf093c3ac;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654550
// FUNCTION: fn_04994
int sub_654550() {
  volatile int v = 0xf093c3af;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654580
// FUNCTION: fn_04995
int sub_654580() {
  volatile int v = 0xf093c1a4;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006545B0
// FUNCTION: fn_04996
int sub_6545B0() {
  volatile int v = 0xf093c325;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006545E0
// FUNCTION: fn_04997
int sub_6545E0() {
  volatile int v = 0xf093c3ae;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654610
// FUNCTION: fn_04998
int sub_654610() {
  volatile int v = 0xf093c3a9;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654640
// FUNCTION: fn_04999
int sub_654640() {
  volatile int v = 0xf093c3a8;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654670
// FUNCTION: fn_05000
int sub_654670() {
  volatile int v = 0xf093c3ab;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006546A0
// FUNCTION: fn_05001
int sub_6546A0() {
  volatile int v = 0xf093c3aa;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006546D0
// FUNCTION: fn_05002
int sub_6546D0() {
  volatile int v = 0xfd8bf70b;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654700
// FUNCTION: fn_05003
int sub_654700() {
  volatile int v = 0xfd8bf70a;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654730
// FUNCTION: fn_05004
int sub_654730() {
  volatile int v = 0xfd8bf709;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654760
// FUNCTION: fn_05005
int sub_654760() {
  volatile int v = 0xfd8bf708;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654790
// FUNCTION: fn_05006
int sub_654790() {
  volatile int v = 0xfd8bf70f;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006547C0
// FUNCTION: fn_05007
int sub_6547C0() {
  volatile int v = 0xfd8bf70e;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006547F0
// FUNCTION: fn_05008
int sub_6547F0() {
  volatile int v = 0xfd8bf70d;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654820
// FUNCTION: fn_05009
int sub_654820() {
  volatile int v = 0xfd8bf70c;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654850
// FUNCTION: fn_05010
int sub_654850() {
  volatile int v = 0xfd8bf703;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654880
// FUNCTION: fn_05011
int sub_654880() {
  volatile int v = 0xfd8bf702;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006548B0
// FUNCTION: fn_05012
int sub_6548B0() {
  volatile int v = 0xfd8bf701;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006548E0
// FUNCTION: fn_05013
int sub_6548E0() {
  volatile int v = 0xfd8bf700;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654910
// FUNCTION: fn_05014
int sub_654910() {
  volatile int v = 0xfd8bf707;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654940
// FUNCTION: fn_05015
int sub_654940() {
  volatile int v = 0xfd8bf706;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654970
// FUNCTION: fn_05016
int sub_654970() {
  volatile int v = 0xfd8bf705;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006549A0
// FUNCTION: fn_05017
int sub_6549A0() {
  volatile int v = 0xfd8bf704;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006549D0
// FUNCTION: fn_05018
int sub_6549D0() {
  volatile int v = 0xfd8bf71b;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A00
// FUNCTION: fn_05019
int sub_654A00() {
  volatile int v = 0xfd8bf71a;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A30
// FUNCTION: fn_05020
int sub_654A30() {
  volatile int v = 0xfd8bf719;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A60
// FUNCTION: fn_05021
int sub_654A60() {
  volatile int v = 0xfd8bf718;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A90
// FUNCTION: fn_05022
int sub_654A90() {
  volatile int v = 0xfd8bf409;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654AC0
// FUNCTION: fn_05023
int sub_654AC0() {
  volatile int v = 0xfd8bf408;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654AF0
// FUNCTION: fn_05024
int sub_654AF0() {
  volatile int v = 0xfd8bf68a;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654B20
// FUNCTION: fn_05025
int sub_654B20() {
  volatile int v = 0xfd8bf689;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654B70
// FUNCTION: fn_05028
int sub_654B70() {
  return 12;
}

// OFFSET: 0x00654BB0
// FUNCTION: fn_05032
unsigned int sub_654BB0(unsigned int value) {
  value++;
  value ^= value >> 17;
  value *= 0xed5ad4bb;
  value ^= value >> 11;
  value *= 0xac4c1b51;
  value ^= value >> 15;
  value *= 0x31848bab;
  value ^= value >> 14;
  return value;
}

// OFFSET: 0x00654BF0
// FUNCTION: fn_05033
bool sub_654BF0() {
  return false;
}

// OFFSET: 0x00654C00
// FUNCTION: fn_05034
bool sub_654C00() {
  return true;
}

// OFFSET: 0x00654C10
// FUNCTION: fn_05035
bool sub_654C10() {
  return true;
}

// OFFSET: 0x00654C20
// FUNCTION: fn_05036
bool sub_654C20() {
  return true;
}

// OFFSET: 0x00654C30
// FUNCTION: fn_05037
bool sub_654C30() {
  return true;
}

// OFFSET: 0x00654C40
// FUNCTION: fn_05038
bool sub_654C40() {
  return true;
}

// OFFSET: 0x00654C50
// FUNCTION: fn_05039
bool sub_654C50() {
  return true;
}

// OFFSET: 0x00654C60
// FUNCTION: fn_05040
bool sub_654C60() {
  return false;
}

// OFFSET: 0x00654C70
// FUNCTION: fn_05041
bool sub_654C70() {
  return false;
}

// OFFSET: 0x00654C80
// FUNCTION: fn_05042
bool sub_654C80() {
  return false;
}

// OFFSET: 0x00654C90
// FUNCTION: fn_05043
bool sub_654C90() {
  return false;
}

// OFFSET: 0x00654CA0
// FUNCTION: fn_05044
bool sub_654CA0() {
  return false;
}

// OFFSET: 0x00654CB0
// FUNCTION: fn_05045
bool sub_654CB0() {
  return false;
}

// OFFSET: 0x00654CC0
// FUNCTION: fn_05046
bool sub_654CC0() {
  return false;
}

// OFFSET: 0x00654CD0
// FUNCTION: fn_05047
bool sub_654CD0() {
  return false;
}

// OFFSET: 0x00654CE0
// FUNCTION: fn_05048
bool sub_654CE0() {
  return false;
}

// OFFSET: 0x00654CF0
// FUNCTION: fn_05049
bool sub_654CF0() {
  return false;
}

// OFFSET: 0x00654D00
// FUNCTION: fn_05050
bool sub_654D00() {
  return true;
}

// OFFSET: 0x00654EF0

struct Span8_654FB0 { const void* ptr; unsigned int len; };
struct Span4_654FB0 { const void* ptr; };

struct FuncImpl_654FB0 {
  void* vtable;
  bool (*target)(Span8_654FB0, Span4_654FB0, Span4_654FB0, Span8_654FB0, Span8_654FB0);

  bool sub_654FB0(const Span8_654FB0* a1, const Span4_654FB0* a2, const Span4_654FB0* a3, const Span8_654FB0* a4, const Span8_654FB0* a5) const;
};

// OFFSET: 0x00654FB0

struct HashNode_655340 {
  HashNode_655340* prev;
  HashNode_655340* next;
};

struct HashBucket_655340 {
  HashNode_655340* first;
  HashNode_655340* last;
};

struct HashTable_655340 {
  void* unknown_00;
  HashNode_655340* head;
  unsigned int size;
  HashBucket_655340* buckets;
  unsigned int unknown_10;
  unsigned int unknown_14;
  unsigned int mask;

  HashNode_655340* sub_655340(unsigned int hash, HashNode_655340* prev, HashNode_655340* node);
};

// OFFSET: 0x00655340

// OFFSET: 0x00655330
// FUNCTION: fn_05059
int* __fastcall sub_655330(int* a) {
  return a + 1;
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

// OFFSET: 0x00E55940
myGPack::CGPakUpdate::~CGPakUpdate() {
  this->Close();
}

struct PakLinkNode {
  /* 0000 */ PakLinkNode* m_next;
  /* 0004 */ PakLinkNode* m_prev;
};

struct PakIntrusiveLink {
  /* 0000 */ unsigned int unknown_0000;
  /* 0004 */ PakLinkNode* m_prev;
  /* 0008 */ PakLinkNode* m_next;

  void Unlink657820();
};

// OFFSET: 0x00657820
// FUNCTION: fn_05092
void PakIntrusiveLink::Unlink657820() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}
