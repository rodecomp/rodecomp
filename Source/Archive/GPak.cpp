#include "Archive/GPak.h"
#include "Archive/GPakUpdate.h"
#include "Archive/PakCipher.h"
#include "Utility/AES.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Hash/XxHash32.h"
#include "Utility/Random/RandomBytes.h"
#include "Utility/XxHash3.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <emmintrin.h>
#include <random>
#include <span>
#include <xhash>
#include <zlib.h>

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    // INLINED
    explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    // INLINED
    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };

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
    /* 000C */ std::uint32_t unknown_000C;
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
  static_assert(sizeof(PakAuxiliaryFile) == 16);
  static_assert(offsetof(PakAuxiliaryFile, identifier) == 4);

  // INLINED
  std::uint8_t SwapNibbles(std::uint8_t byte) {
    return static_cast<std::uint8_t>((byte << 4) | (byte >> 4));
  }

  __declspec(noinline) std::uint64_t ReadAuxiliaryIdentifier(const void* data, std::size_t size);

  // OFFSET: 0x00658FC0
  // FUNCTION: fn_05116
  std::uint64_t ReadAuxiliaryIdentifier(const void* data, std::size_t size) {
    if (size != 0) {
      PakAuxiliaryFile header;
      std::memcpy(&header, data, sizeof(header));
      if (header.version <= 1) {
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

// OFFSET: 0x00644260
// FUNCTION: fn_04842
void XorWithKey16(std::span<const std::uint8_t, 16> key, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) {
  const __m128i mask = _mm_loadu_si128(reinterpret_cast<const __m128i*>(key.data()));
  const std::uint8_t* source = input.data();
  std::uint8_t* destination = output.data();
  if (input.size() >= 16) {
    std::size_t blocks = input.size() >> 4;
    do {
      _mm_storeu_si128(reinterpret_cast<__m128i*>(destination), _mm_xor_si128(_mm_loadu_si128(reinterpret_cast<const __m128i*>(source)), mask));
      source += 16;
      destination += 16;
    } while (--blocks != 0);
  }

  const std::size_t remainder = input.size() & 0xF;
  for (std::size_t i = 0; i < remainder; ++i) {
    destination[i] = source[i] ^ key[i];
  }
}

// OFFSET: 0x00654460
// FUNCTION: fn_04989
int CrypterId005() {
  volatile int v = 0xf093c3a0;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654490
// FUNCTION: fn_04990
int CrypterId006() {
  volatile int v = 0xf093c3a3;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006544C0
// FUNCTION: fn_04991
int CrypterId007() {
  volatile int v = 0xf093c3a2;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006544F0
// FUNCTION: fn_04992
int CrypterId008() {
  volatile int v = 0xf093c3ad;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654520
// FUNCTION: fn_04993
int CrypterId009() {
  volatile int v = 0xf093c3ac;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654550
// FUNCTION: fn_04994
int CrypterId00A() {
  volatile int v = 0xf093c3af;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654580
// FUNCTION: fn_04995
int CrypterId201() {
  volatile int v = 0xf093c1a4;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006545B0
// FUNCTION: fn_04996
int CrypterId080() {
  volatile int v = 0xf093c325;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006545E0
// FUNCTION: fn_04997
int CrypterId00B() {
  volatile int v = 0xf093c3ae;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654610
// FUNCTION: fn_04998
int CrypterId00C() {
  volatile int v = 0xf093c3a9;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654640
// FUNCTION: fn_04999
int CrypterId00D() {
  volatile int v = 0xf093c3a8;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x00654670
// FUNCTION: fn_05000
int CrypterId00E() {
  volatile int v = 0xf093c3ab;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006546A0
// FUNCTION: fn_05001
int CrypterId00F() {
  volatile int v = 0xf093c3aa;
  v ^= 0x3821881d;
  v ^= 0xc8b24bb8;
  return v;
}

// OFFSET: 0x006546D0
// FUNCTION: fn_05002
int CrypterId100() {
  volatile int v = 0xfd8bf70b;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654700
// FUNCTION: fn_05003
int CrypterId101() {
  volatile int v = 0xfd8bf70a;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654730
// FUNCTION: fn_05004
int CrypterId102() {
  volatile int v = 0xfd8bf709;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654760
// FUNCTION: fn_05005
int CrypterId103() {
  volatile int v = 0xfd8bf708;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654790
// FUNCTION: fn_05006
int CrypterId104() {
  volatile int v = 0xfd8bf70f;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006547C0
// FUNCTION: fn_05007
int CrypterId105() {
  volatile int v = 0xfd8bf70e;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006547F0
// FUNCTION: fn_05008
int CrypterId106() {
  volatile int v = 0xfd8bf70d;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654820
// FUNCTION: fn_05009
int CrypterId107() {
  volatile int v = 0xfd8bf70c;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654850
// FUNCTION: fn_05010
int CrypterId108() {
  volatile int v = 0xfd8bf703;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654880
// FUNCTION: fn_05011
int CrypterId109() {
  volatile int v = 0xfd8bf702;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006548B0
// FUNCTION: fn_05012
int CrypterId10A() {
  volatile int v = 0xfd8bf701;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006548E0
// FUNCTION: fn_05013
int CrypterId10B() {
  volatile int v = 0xfd8bf700;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654910
// FUNCTION: fn_05014
int CrypterId10C() {
  volatile int v = 0xfd8bf707;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654940
// FUNCTION: fn_05015
int CrypterId10D() {
  volatile int v = 0xfd8bf706;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654970
// FUNCTION: fn_05016
int CrypterId10E() {
  volatile int v = 0xfd8bf705;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006549A0
// FUNCTION: fn_05017
int CrypterId10F() {
  volatile int v = 0xfd8bf704;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x006549D0
// FUNCTION: fn_05018
int CrypterId110() {
  volatile int v = 0xfd8bf71b;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A00
// FUNCTION: fn_05019
int CrypterId111() {
  volatile int v = 0xfd8bf71a;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A30
// FUNCTION: fn_05020
int CrypterId112() {
  volatile int v = 0xfd8bf719;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A60
// FUNCTION: fn_05021
int CrypterId113() {
  volatile int v = 0xfd8bf718;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654A90
// FUNCTION: fn_05022
int CrypterId202() {
  volatile int v = 0xfd8bf409;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654AC0
// FUNCTION: fn_05023
int CrypterId203() {
  volatile int v = 0xfd8bf408;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654AF0
// FUNCTION: fn_05024
int CrypterId081() {
  volatile int v = 0xfd8bf68a;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654B20
// FUNCTION: fn_05025
int CrypterId082() {
  volatile int v = 0xfd8bf689;
  v ^= 0x2b225215;
  v ^= 0xd6a9a41e;
  return v;
}

// OFFSET: 0x00654BB0
// FUNCTION: fn_05032
unsigned int Triple32Hash(unsigned int value) {
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


struct VectorUint32_654D10 {
  unsigned int* first;
  unsigned int* last;
  unsigned int* end;

  __declspec(noinline) void AssignFill(unsigned int count, unsigned int val);
};

// OFFSET: 0x00654D10
// FUNCTION: fn_05051
__declspec(noinline) void VectorUint32_654D10::AssignFill(unsigned int count, unsigned int val) {
  unsigned int old_size = static_cast<unsigned int>(this->last - this->first);
  if (count <= old_size) {
    std::fill(this->first, this->last, val);
    return;
  }
  if (count > 0x3fffffff) {
    std::_Throw_bad_array_new_length();
  }
  unsigned int bytes = count * sizeof(unsigned int);
  unsigned int* new_ptr;
  if (bytes < 0x1000) {
    if (bytes == 0) {
      new_ptr = nullptr;
    } else {
      new_ptr = static_cast<unsigned int*>(::operator new(bytes));
    }
  } else {
    if (bytes + 0x23 <= bytes) {
      std::_Throw_bad_array_new_length();
    }
    void* raw = ::operator new(bytes + 0x23);
    if (!raw) {
      _invalid_parameter_noinfo_noreturn();
    }
    new_ptr = reinterpret_cast<unsigned int*>((reinterpret_cast<uintptr_t>(raw) + 0x23) & ~static_cast<uintptr_t>(0x1f));
    reinterpret_cast<void**>(new_ptr)[-1] = raw;
  }
  if (old_size != 0) {
    unsigned int old_bytes = old_size * sizeof(unsigned int);
    void* old_raw = this->first;
    if (old_bytes >= 0x1000) {
      old_raw = reinterpret_cast<void**>(this->first)[-1];
      if ((reinterpret_cast<uintptr_t>(this->first) - reinterpret_cast<uintptr_t>(old_raw) - 4) > 0x1f) {
        _invalid_parameter_noinfo_noreturn();
      }
      old_bytes += 0x23;
    }
    ::operator delete(old_raw, old_bytes);
  }
  this->first = new_ptr;
  this->last = new_ptr + count;
  this->end = new_ptr + count;
  for (unsigned int* p = new_ptr; p != this->last; ++p) {
    *p = val;
  }
}

struct VectorInt32_654E00 {
  int* first;
  int* last;
  int* end;

  __declspec(noinline) void AssignFill(unsigned int count, int val);
};

// OFFSET: 0x00654E00
// FUNCTION: fn_05052
__declspec(noinline) void VectorInt32_654E00::AssignFill(unsigned int count, int val) {
  unsigned int old_size = static_cast<unsigned int>(this->last - this->first);
  if (count <= old_size) {
    std::fill(this->first, this->last, val);
    return;
  }
  if (count > 0x3fffffff) {
    std::_Throw_bad_array_new_length();
  }
  unsigned int bytes = count * sizeof(int);
  int* new_ptr;
  if (bytes < 0x1000) {
    if (bytes == 0) {
      new_ptr = nullptr;
    } else {
      new_ptr = static_cast<int*>(::operator new(bytes));
    }
  } else {
    if (bytes + 0x23 <= bytes) {
      std::_Throw_bad_array_new_length();
    }
    void* raw = ::operator new(bytes + 0x23);
    if (!raw) {
      _invalid_parameter_noinfo_noreturn();
    }
    new_ptr = reinterpret_cast<int*>((reinterpret_cast<uintptr_t>(raw) + 0x23) & ~static_cast<uintptr_t>(0x1f));
    reinterpret_cast<void**>(new_ptr)[-1] = raw;
  }
  if (old_size != 0) {
    unsigned int old_bytes = old_size * sizeof(int);
    void* old_raw = this->first;
    if (old_bytes >= 0x1000) {
      old_raw = reinterpret_cast<void**>(this->first)[-1];
      if ((reinterpret_cast<uintptr_t>(this->first) - reinterpret_cast<uintptr_t>(old_raw) - 4) > 0x1f) {
        _invalid_parameter_noinfo_noreturn();
      }
      old_bytes += 0x23;
    }
    ::operator delete(old_raw, old_bytes);
  }
  this->first = new_ptr;
  this->last = new_ptr + count;
  this->end = new_ptr + count;
  for (int* p = new_ptr; p != this->last; ++p) {
    *p = val;
  }
}

// OFFSET: 0x00654EF0
// FUNCTION: fn_05053
bool __fastcall CheckRehashRequired(const void* self) {
  const float max_load_factor = *(const float*)self;
  const unsigned int size = *(const unsigned int*)((const char*)self + 8);
  const unsigned int bucket_count = *(const unsigned int*)((const char*)self + 0x1c);
  return (float)(size + 1) / (float)bucket_count > max_load_factor;
}

struct Span8_654FB0 { const void* ptr; unsigned int len; };
struct Span4_654FB0 { const void* ptr; };

struct FuncImpl_654FB0 {
  void* vtable;
  bool (*target)(Span8_654FB0, Span4_654FB0, Span4_654FB0, Span8_654FB0, Span8_654FB0);

  bool DoCall(const Span8_654FB0* a1, const Span4_654FB0* a2, const Span4_654FB0* a3, const Span8_654FB0* a4, const Span8_654FB0* a5) const;
};

// OFFSET: 0x00654FB0
// FUNCTION: fn_05056
bool FuncImpl_654FB0::DoCall(const Span8_654FB0* a1, const Span4_654FB0* a2, const Span4_654FB0* a3, const Span8_654FB0* a4, const Span8_654FB0* a5) const {
  return this->target(*a1, *a2, *a3, *a4, *a5);
}

struct HashNode_655340 {
  HashNode_655340* prev;
  HashNode_655340* next;
  short key;
};

struct HashBucket_655340 {
  HashNode_655340* first;
  HashNode_655340* last;
};

struct HashSearchResult_655D70 {
  HashNode_655340* first;
  HashNode_655340* second;

  HashSearchResult_655D70() {}
  HashSearchResult_655D70(HashNode_655340* f, HashNode_655340* s) : first(f), second(s) {}
};

struct HashTable_655340 {
  float max_load_factor;
  HashNode_655340* head;
  unsigned int size;
  HashBucket_655340* buckets;
  unsigned int unknown_10;
  unsigned int unknown_14;
  unsigned int mask;
  unsigned int bucket_count;

  void ForcedRehash(unsigned int buckets);
  void ForcedRehashSigned(unsigned int buckets);
  HashNode_655340* EraseRange(HashNode_655340* first, HashNode_655340* last);
  HashNode_655340* InsertNewNodeBefore(unsigned int hash, HashNode_655340* prev, HashNode_655340* node);
  void RehashForInsert();
  HashSearchResult_655D70 FindLast(const short* key, unsigned int hash);
};

// OFFSET: 0x00654FF0
// FUNCTION: fn_05057
void HashTable_655340::ForcedRehash(unsigned int bucket_count) {
  const unsigned long max_buckets_log2 = std::_Floor_of_log_2(0x1fffffff);
  const unsigned int max_buckets = 1u << max_buckets_log2;
  if (bucket_count > max_buckets) {
    std::_Xlength_error("invalid hash bucket count");
  }
  unsigned int new_bucket_count = 1u << std::_Ceiling_of_log_2(bucket_count);
  reinterpret_cast<VectorUint32_654D10*>(&this->buckets)->AssignFill(new_bucket_count * 2, reinterpret_cast<uintptr_t>(this->head));
  this->bucket_count = new_bucket_count;
  this->mask = new_bucket_count - 1;

  HashNode_655340* head = this->head;
  HashNode_655340* node = head->prev;
  while (node != head) {
    HashNode_655340* next_node = node->prev;
    const unsigned char* key_bytes = reinterpret_cast<const unsigned char*>(&node->key);
    unsigned int hash = (key_bytes[0] ^ 0x811c9dc5) * 0x1000193;
    hash = (hash ^ key_bytes[1]) * 0x1000193;
    unsigned int bucket_idx = hash & this->mask;
    HashBucket_655340* bucket = &this->buckets[bucket_idx];
    if (bucket->first == head) {
      bucket->first = node;
      bucket->last = node;
    } else {
      HashNode_655340* last = bucket->last;
      if (node->key == last->key) {
        if (last->prev != node) {
          node->next->prev = next_node;
          next_node->next = node->next;
          node->prev = last->prev;
          last->prev->next = node;
          last->prev = node;
          node->next = last;
        }
        bucket->last = node;
      } else {
        HashNode_655340* first = bucket->first;
        HashNode_655340* match = nullptr;
        HashNode_655340* it = last;
        while (it != first) {
          it = it->next;
          if (it->key == node->key) {
            match = it;
            break;
          }
        }
        if (match) {
          node->next->prev = next_node;
          next_node->next = node->next;
          node->prev = match->prev;
          match->prev->next = node;
          match->prev = node;
          node->next = match;
        } else {
          node->next->prev = next_node;
          next_node->next = node->next;
          node->prev = first;
          first->next->prev = node;
          node->next = first->next;
          first->next = node;
          bucket->first = node;
        }
      }
    }
    node = next_node;
  }
}

// OFFSET: 0x00655190
// FUNCTION: fn_05058
void HashTable_655340::ForcedRehashSigned(unsigned int bucket_count) {
  const unsigned long max_buckets_log2 = std::_Floor_of_log_2(0x1fffffff);
  const unsigned int max_buckets = 1u << max_buckets_log2;
  if (bucket_count > max_buckets) {
    std::_Xlength_error("invalid hash bucket count");
  }
  unsigned int new_bucket_count = 1u << std::_Ceiling_of_log_2(bucket_count);
  reinterpret_cast<VectorInt32_654E00*>(&this->buckets)->AssignFill(new_bucket_count * 2, reinterpret_cast<uintptr_t>(this->head));
  this->bucket_count = new_bucket_count;
  this->mask = new_bucket_count - 1;

  HashNode_655340* head = this->head;
  HashNode_655340* node = head->prev;
  while (node != head) {
    HashNode_655340* next_node = node->prev;
    const unsigned char* key_bytes = reinterpret_cast<const unsigned char*>(&node->key);
    unsigned int hash = (key_bytes[0] ^ 0x811c9dc5) * 0x1000193;
    hash = (hash ^ key_bytes[1]) * 0x1000193;
    unsigned int bucket_idx = hash & this->mask;
    HashBucket_655340* bucket = &this->buckets[bucket_idx];
    if (bucket->first == head) {
      bucket->first = node;
      bucket->last = node;
    } else {
      HashNode_655340* last = bucket->last;
      if (node->key == last->key) {
        if (last->prev != node) {
          node->next->prev = next_node;
          next_node->next = node->next;
          node->prev = last->prev;
          last->prev->next = node;
          last->prev = node;
          node->next = last;
        }
        bucket->last = node;
      } else {
        HashNode_655340* first = bucket->first;
        HashNode_655340* match = nullptr;
        HashNode_655340* it = last;
        while (it != first) {
          it = it->next;
          if (it->key == node->key) {
            match = it;
            break;
          }
        }
        if (match) {
          node->next->prev = next_node;
          next_node->next = node->next;
          node->prev = match->prev;
          match->prev->next = node;
          match->prev = node;
          node->next = match;
        } else {
          node->next->prev = next_node;
          next_node->next = node->next;
          node->prev = first;
          first->next->prev = node;
          node->next = first->next;
          first->next = node;
          bucket->first = node;
        }
      }
    }
    node = next_node;
  }
}

// OFFSET: 0x00655340
// FUNCTION: fn_05060
HashNode_655340* HashTable_655340::InsertNewNodeBefore(unsigned int hash, HashNode_655340* prev, HashNode_655340* node) {
  HashNode_655340* next = prev->next;
  this->size++;
  node->prev = prev;
  node->next = next;
  next->prev = node;
  prev->next = node;

  HashBucket_655340* bucket = &this->buckets[hash & this->mask];
  if (bucket->first == this->head) {
    bucket->first = node;
    bucket->last = node;
    return node;
  }

  if (bucket->first == prev) {
    bucket->first = node;
    return node;
  }

  if (bucket->last == next) {
    bucket->last = node;
  }

  return node;
}

// OFFSET: 0x00655330
// FUNCTION: fn_05059
int* __fastcall GetCallable(int* a) {
  return a + 1;
}

// OFFSET: 0x006553C0
// FUNCTION: fn_05062
void HashTable_655340::RehashForInsert() {
  const unsigned int old_buckets = this->bucket_count;
  const unsigned int required = (std::max)(8u, static_cast<unsigned int>(std::ceil(static_cast<float>(this->size + 1) / this->max_load_factor)));
  unsigned int buckets = old_buckets;
  if (old_buckets < required) {
    if (old_buckets < 512 && old_buckets * 8 >= required) {
      buckets = old_buckets * 8;
    } else {
      buckets = required;
    }
  }
  this->ForcedRehash(buckets);
}

// OFFSET: 0x00655470
// FUNCTION: fn_05065
HashNode_655340* HashTable_655340::EraseRange(HashNode_655340* first, HashNode_655340* last) {
  if (first == last) {
    return last;
  }
  HashNode_655340* head = this->head;
  HashBucket_655340* bucket_base = this->buckets;
  HashNode_655340* prev_node = first->next;
  HashNode_655340* cur = first;

  const unsigned char* key_bytes = reinterpret_cast<const unsigned char*>(&cur->key);
  unsigned int hash = (key_bytes[0] ^ 0x811c9dc5) * 0x1000193;
  hash = (hash ^ key_bytes[1]) * 0x1000193;
  HashBucket_655340* cur_bucket = &bucket_base[hash & this->mask];

  HashNode_655340* bucket_first = cur_bucket->first;
  HashNode_655340* bucket_last = cur_bucket->last;

  while (true) {
    HashNode_655340* next_node = cur->prev;
    bool is_bucket_last = (cur == bucket_last);

    char* cur_bytes = reinterpret_cast<char*>(cur);
    void** p_impl = reinterpret_cast<void**>(cur_bytes + 0x34);
    if (*p_impl != nullptr) {
      void* impl = *p_impl;
      void** vtbl = *reinterpret_cast<void***>(impl);
      typedef void (__thiscall *DeleteThisFn)(void*, bool);
      DeleteThisFn delete_this = reinterpret_cast<DeleteThisFn>(vtbl[4]);
      bool not_local = (impl != cur_bytes + 0x10);
      delete_this(impl, not_local);
      *p_impl = nullptr;
    }
    ::operator delete(cur, 0x38);
    --this->size;

    if (is_bucket_last) {
      if (cur_bucket->first == first) {
        cur_bucket->first = head;
        cur_bucket->last = head;
      } else {
        cur_bucket->last = prev_node;
      }

      if (next_node != last) {
        cur = next_node;
        while (true) {
          const unsigned char* k = reinterpret_cast<const unsigned char*>(&cur->key);
          unsigned int h = (k[0] ^ 0x811c9dc5) * 0x1000193;
          h = (h ^ k[1]) * 0x1000193;
          cur_bucket = &bucket_base[h & this->mask];
          bucket_last = cur_bucket->last;
          HashNode_655340* nxt = cur->prev;
          bool is_last = (cur == bucket_last);

          char* cur_b = reinterpret_cast<char*>(cur);
          void** pi = reinterpret_cast<void**>(cur_b + 0x34);
          if (*pi != nullptr) {
            void* imp = *pi;
            void** vt = *reinterpret_cast<void***>(imp);
            typedef void (__thiscall *DeleteThisFn)(void*, bool);
            DeleteThisFn delete_this = reinterpret_cast<DeleteThisFn>(vt[4]);
            bool not_local = (imp != cur_b + 0x10);
            delete_this(imp, not_local);
            *pi = nullptr;
          }
          ::operator delete(cur, 0x38);
          --this->size;

          if (is_last) {
            cur_bucket->first = head;
            cur_bucket->last = head;
            if (nxt == last) {
              break;
            }
          } else {
            if (nxt == last) {
              cur_bucket->first = nxt;
              break;
            }
          }
          cur = nxt;
        }
      }
      break;
    } else {
      if (next_node == last) {
        if (cur_bucket->first == first) {
          cur_bucket->first = next_node;
        }
        break;
      }
      cur = next_node;
    }
  }

  prev_node->prev = last;
  last->next = prev_node;
  return last;
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

struct PakKeyBlock48 {
  /* 0000 */ std::uint8_t bytes[48];
};

struct PakMaskedKey48 {
  /* 0000 */ std::uint8_t mask[5];
  /* 0005 */ std::uint8_t data[48];

  __declspec(noinline) PakKeyBlock48 Unmask() const;
};

struct PakCipherBlock_655900 {
  short* Encode(const short* in, const int* src);
};

// OFFSET: 0x00655900
// FUNCTION: fn_05067
short* PakCipherBlock_655900::Encode(const short* in, const int* src) {
  short* out = reinterpret_cast<short*>(this);
  *out = *in;
  int* key_ptr = reinterpret_cast<int*>(out + 1);
  *key_ptr = 0;
  reinterpret_cast<char*>(out)[6] = 0;
  char* payload = reinterpret_cast<char*>(out) + 7;
  std::memset(payload, 0, 0x40);

  unsigned int seed = std::_Random_device();
  PakUniformRandom rng(1, 0xfe, seed);

  FillRandomBytes(reinterpret_cast<char*>(key_ptr), payload, rng);

  const unsigned char* in_bytes = reinterpret_cast<const unsigned char*>(src);
  unsigned char* out_bytes = reinterpret_cast<unsigned char*>(out);
  const unsigned char* key_bytes = reinterpret_cast<const unsigned char*>(key_ptr);
  for (int i = 0; i < 0x40; i += 4) {
    int k = i & 3;
    out_bytes[i + 7] = in_bytes[i] ^ key_bytes[k + 1] ^ key_bytes[k];
    out_bytes[i + 8] = in_bytes[i + 1] ^ key_bytes[k + 2] ^ key_bytes[k + 1];
    out_bytes[i + 9] = in_bytes[i + 2] ^ key_bytes[k + 3] ^ key_bytes[k + 2];
    out_bytes[i + 10] = in_bytes[i + 3] ^ key_bytes[k + 4] ^ key_bytes[k + 3];
  }
  return out;
}

struct PakSpan_655A30 {
  void* data;
  unsigned int size;
};

struct PakVerifier_655A30 {
  PakSpan_655A30** span_objs;
  PakSpan_655A30* span_a;
  PakSpan_655A30* span_b;

  __declspec(noinline) bool Verify(AES** cipher_ptr, const PakMaskedKey48* masked_key, int mode, int keySource);
};

// OFFSET: 0x00655A30
// FUNCTION: fn_05068
__declspec(noinline) bool PakVerifier_655A30::Verify(AES** cipher_ptr, const PakMaskedKey48* masked_key, int mode, int keySource) {
  alignas(16) unsigned char key[16]{};

  switch (keySource) {
  case 0: {
    PakKeyBlock48 block = masked_key->Unmask();
    std::memcpy(key, block.bytes + 16, 16);
    SecureZeroMemory(&block, sizeof(block));
    break;
  }
  case 1: {
    PakSpan_655A30* name = *this->span_objs;
    alignas(16) XXH128_hash_t hash = XXH3_128bits(name->data, name->size);
    std::memcpy(key, &hash, 16);
    SecureZeroMemory(&hash, sizeof(hash));
    break;
  }
  case 2: {
    const void* checksum = reinterpret_cast<char*>(reinterpret_cast<void**>(this->span_objs)[1]) + 4;
    alignas(16) XXH128_hash_t hash = XXH3_128bits(checksum, 4);
    std::memcpy(key, &hash, 16);
    SecureZeroMemory(&hash, sizeof(hash));
    break;
  }
  }

  AES* cipher = *cipher_ptr;
  cipher->SetIV(key, sizeof(key));
  cipher->SetMode(mode);

  const ULONG res = cipher->Decrypt(static_cast<PUCHAR>(this->span_a->data), this->span_a->size, static_cast<PUCHAR>(this->span_b->data), this->span_b->size, 0);

  volatile int check = 0xe5c3c820;
  check ^= 0xf63071d0;
  check ^= 0x13f3b990;
  return res == check;
}

// OFFSET: 0x00655D70
// FUNCTION: fn_05069
HashSearchResult_655D70 HashTable_655340::FindLast(const short* key, unsigned int hash) {
  HashSearchResult_655D70 result;
  HashBucket_655340* bucket = &this->buckets[this->mask & hash];
  HashNode_655340* node = bucket->last;
  if (node == this->head) {
    result.first = this->head;
    result.second = nullptr;
    return result;
  }
  short target = *key;
  if (target == node->key) {
    result.first = node->prev;
    result.second = node;
    return result;
  }
  while (node != bucket->first) {
    node = node->next;
    if (target == node->key) {
      result.first = node->prev;
      result.second = node;
      return result;
    }
  }
  result.first = node;
  result.second = nullptr;
  return result;
}

struct SeedGen_657C30 {
  unsigned int seed;
  __declspec(noinline) unsigned int NextSeed();
};

// OFFSET: 0x00657C30
// FUNCTION: fn_05095
__declspec(noinline) unsigned int SeedGen_657C30::NextSeed() {
  this->seed = static_cast<unsigned int>(static_cast<unsigned __int64>(this->seed) * 40014u % 0x7fffffabu);
  return this->seed;
}

struct RanluxEngine_655F00 {
  void Seed(const unsigned int* seed_ptr, char flag, int unused);
};

// OFFSET: 0x00655F00
// FUNCTION: fn_05070
void RanluxEngine_655F00::Seed(const unsigned int* seed_ptr, char flag, int unused) {
  unsigned int* state = reinterpret_cast<unsigned int*>(this);
  unsigned int seed = *seed_ptr;
  if (seed == 0) {
    seed = 0x12dd397;
  } else {
    seed %= 0x7fffffab;
    if (seed == 0) {
      seed = 1;
    }
  }

  for (int i = 1; i <= 24; ++i) {
    unsigned __int64 product = static_cast<unsigned __int64>(seed) * 40014u;
    seed = static_cast<unsigned int>(product % 0x7fffffabu);
    state[i] = seed;
  }

  for (int i = 1; i <= 16; ++i) {
    state[i] &= 0x00ffffffu;
  }

  unsigned char* p = reinterpret_cast<unsigned char*>(this) + 0x47;
  for (int i = 0; i < 8; ++i) {
    *p = 0;
    p += 4;
  }

  unsigned int carry;
  if (flag == 0) {
    carry = (state[24] == 0) ? 1 : 0;
  } else {
    SeedGen_657C30 gen{seed};
    carry = gen.NextSeed();
  }

  state[0] = 24;
  state[49] = carry;
}

// OFFSET: 0x00656370
// FUNCTION: fn_05072
void FillUint32(unsigned int* first, unsigned int* last, const unsigned int* val) {
  for (; first != last; ++first) {
    *first = *val;
  }
}

// OFFSET: 0x00656C70
// FUNCTION: fn_05080
PakAuxiliary::PakAuxiliary() {
}

struct PakState_656DF0 {
  PakState_656DF0();

  /* 0000 */ std::uint32_t unknown_0000;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint32_t unknown_0010;
  /* 0014 */ std::uint32_t unknown_0014;
  /* 0018 */ std::uint32_t unknown_0018;
  /* 001C */ std::uint32_t unknown_001C;
  /* 0020 */ std::uint32_t unknown_0020;
};

static_assert(sizeof(PakState_656DF0) == 0x24);
static_assert(offsetof(PakState_656DF0, unknown_0020) == 0x20);

// OFFSET: 0x00656DF0
// FUNCTION: fn_05081
PakState_656DF0::PakState_656DF0()
    : unknown_0000(0), unknown_0004(0), unknown_0008(0), unknown_000C(0), unknown_0010(0), unknown_0014(0), unknown_0018(0), unknown_001C(0), unknown_0020(1) {
}

// OFFSET: 0x00657640
// FUNCTION: fn_05091
PakAuxiliary::~PakAuxiliary() {
}

// OFFSET: 0x00AE97D0
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
  EncryptedName<14, 0x050505679903579BULL>& eventHorizon = []() -> EncryptedName<14, 0x050505679903579BULL>& {
    const char source[] = {'\xDE', '\x21', '\x66', '\xF7', '\x13', '\x25', '\x4D', '\x6A', '\xE9', '\x3E', '\x79', '\xF6', '\x09', '\x05'};
    // GLOBAL: gl_25504
    thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> name(source);
    return name;
  }();

  if (std::memcmp(bytes, eventHorizon.Text(), 14) == 0) {
    const PakHeaderFile64* header = reinterpret_cast<const PakHeaderFile64*>(bytes);
    this->m_version = header->version;
    this->m_entryCount = header->entryCount;
    this->m_tableOffset = header->tableOffset + sizeof(PakHeaderFile64);
  } else {
    EncryptedName<16, 0x83EFBFBB37EDBDBDULL>& masterOfMagic = []() -> EncryptedName<16, 0x83EFBFBB37EDBDBDULL>& {
      const char source[] = {'\xF0', '\xDC', '\x9E', '\x43', '\xDE', '\xCD', '\xCF', '\xEC', '\xDB', '\x9D', '\xA0', '\x56', '\xDC', '\xD6', '\x8C', '\x83'};
      // GLOBAL: gl_25505
      thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
      return name;
    }();

    if (std::memcmp(bytes, masterOfMagic.Text(), 16) != 0) {
      return false;
    }

    const PakHeaderFile* header = reinterpret_cast<const PakHeaderFile*>(bytes);
    this->m_version = header->version;
    this->m_entryCount = header->entryCount - header->seed - 7;
    this->m_tableOffset = static_cast<std::uint64_t>(header->tableOffset) + sizeof(PakHeaderFile);
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
    const PakEntryFile* fileEntry = reinterpret_cast<const PakEntryFile*>(metadata);
    cursor = metadata + sizeof(PakEntryFile);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry->compressedSize - fileEntry->originalSize - 715;
    entry.alignedSize = fileEntry->alignedSize - 37579;
    entry.originalSize = fileEntry->originalSize;
    entry.flags = fileEntry->flags;
    entry.dataOffset = fileEntry->dataOffset;
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
    const PakEntryFile* fileEntry = reinterpret_cast<const PakEntryFile*>(metadata);
    cursor = metadata + sizeof(PakEntryFile);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry->compressedSize;
    entry.alignedSize = fileEntry->alignedSize;
    entry.originalSize = fileEntry->originalSize;
    entry.flags = fileEntry->flags;
    entry.dataOffset = fileEntry->dataOffset;
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
    const PakEntryFileV3* fileEntry = reinterpret_cast<const PakEntryFileV3*>(metadata);
    cursor = metadata + sizeof(PakEntryFileV3);

    myGPack::PakEntry& entry = this->m_entries[i];
    entry.compressedSize = fileEntry->compressedSize;
    entry.alignedSize = fileEntry->alignedSize;
    entry.originalSize = fileEntry->originalSize;
    entry.flags = fileEntry->flags;
    entry.dataOffset = fileEntry->dataOffset;
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
// FUNCTION: fn_24308
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

  void Unlink64AE10();
  void Unlink64AE30();
  void Unlink657820();
};

// OFFSET: 0x0064AE10
// FUNCTION: fn_04910
void PakIntrusiveLink::Unlink64AE10() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x0064AE30
// FUNCTION: fn_04911
void PakIntrusiveLink::Unlink64AE30() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x00657820
// FUNCTION: fn_05092
void PakIntrusiveLink::Unlink657820() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x00656B60
// FUNCTION: fn_05078
__declspec(noinline) PakUniformRandom::PakUniformRandom(int minimum, int maximum, unsigned int seed) : m_engine(seed), m_distribution(minimum, maximum) {
}

struct PakAuxiliaryHeader {
  /* 0000 */ std::uint8_t version;
  /* 0001 */ std::uint8_t keySource;
  /* 0002 */ std::uint8_t unknown_0002[2];
  /* 0004 */ std::uint32_t checksum;
  /* 0008 */ std::uint8_t payload[96];
};

static_assert(sizeof(PakAuxiliaryHeader) == 0x68);
static_assert(offsetof(PakAuxiliaryHeader, checksum) == 4);
static_assert(offsetof(PakAuxiliaryHeader, payload) == 8);

struct PakAuxiliaryContext {
  /* 0000 */ PakSpan_655A30* name;
  /* 0004 */ PakAuxiliaryHeader* header;
};

struct PakAuxiliaryState {
  /* 0000 */ HashTable_655340 m_handlers;
  /* 0020 */ PakMaskedKey48 m_key;
  /* 0055 */ std::uint8_t unknown_0055[107];
  /* 00C0 */ AES* m_cipher;

  bool DecodeHeader(PakSpan_655A30 name, std::uint8_t* data, std::uint32_t size, PakAuxiliaryHeader* header);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(PakAuxiliaryState, m_key) == 0x20);
static_assert(offsetof(PakAuxiliaryState, m_cipher) == 0xC0);
#endif

// OFFSET: 0x00658B90
// FUNCTION: fn_05112
bool PakAuxiliaryState::DecodeHeader(PakSpan_655A30 name, std::uint8_t* data, std::uint32_t size, PakAuxiliaryHeader* header) {
  if (size < sizeof(PakAuxiliaryHeader)) {
    return false;
  }

  std::memcpy(header, data, sizeof(PakAuxiliaryHeader));
  if (header->version > 1) {
    return false;
  }

  volatile int sourceCount = 0x772354dd;
  sourceCount ^= 0xeb082218;
  sourceCount ^= 0x9c2b76a5;
  const int encryptedCount = sourceCount;
  volatile int sourceOffset = 0x772354b5;
  sourceOffset ^= 0xeb082218;
  sourceOffset ^= 0x9c2b76a5;
  const std::span<std::uint8_t> encryptedSpan = std::span<std::uint8_t>(data, size).subspan(sourceOffset, encryptedCount);

  volatile int targetCount = 0xcaac7501;
  targetCount ^= 0xb575cb53;
  targetCount ^= 0x7fd9be32;
  const int decryptedCount = targetCount;
  volatile int targetOffset = 0xcaac7569;
  targetOffset ^= 0xb575cb53;
  targetOffset ^= 0x7fd9be32;
  const std::span<std::uint8_t> decryptedSpan =
      std::span<std::uint8_t, sizeof(PakAuxiliaryHeader)>(reinterpret_cast<std::uint8_t*>(header), sizeof(PakAuxiliaryHeader)).subspan(targetOffset, decryptedCount);

  PakSpan_655A30 encrypted;
  encrypted.data = encryptedSpan.data();
  encrypted.size = encryptedSpan.size();
  PakSpan_655A30 decrypted;
  decrypted.data = decryptedSpan.data();
  decrypted.size = decryptedSpan.size();

  PakAuxiliaryContext context;
  context.name = &name;
  context.header = header;

  PakVerifier_655A30 verifier;
  verifier.span_objs = &context.name;
  verifier.span_a = &encrypted;
  verifier.span_b = &decrypted;

  switch (header->keySource) {
  case 0:
    break;
  case 1:
    if (!verifier.Verify(&this->m_cipher, &this->m_key, AES::CBC, 0)) {
      return false;
    }
    break;
  case 2: {
    XXH128_hash_t hash = XXH3_128bits(name.data, name.size);
    XXH128_hash_t key = hash;
    for (std::size_t i = 0; i < sizeof(hash); ++i) {
      reinterpret_cast<volatile std::uint8_t*>(&hash)[i] = 0;
    }
    this->m_cipher->SetIV(reinterpret_cast<const UCHAR*>(&key), sizeof(key));
    this->m_cipher->SetMode(AES::CBC);
    const ULONG written = this->m_cipher->Decrypt(static_cast<PUCHAR>(encrypted.data), encrypted.size, static_cast<PUCHAR>(decrypted.data), decrypted.size, 0);
    volatile int expected = 0xe5c3c820;
    expected ^= 0xf63071d0;
    expected ^= 0x13f3b990;
    if (written != expected) {
      return false;
    }
    break;
  }
  case 15: {
    XXH128_hash_t hash = XXH3_128bits(&header->checksum, sizeof(header->checksum));
    XXH128_hash_t key = hash;
    for (std::size_t i = 0; i < sizeof(hash); ++i) {
      reinterpret_cast<volatile std::uint8_t*>(&hash)[i] = 0;
    }
    this->m_cipher->SetIV(reinterpret_cast<const UCHAR*>(&key), sizeof(key));
    this->m_cipher->SetMode(AES::CBC);
    const ULONG written = this->m_cipher->Decrypt(static_cast<PUCHAR>(encrypted.data), encrypted.size, static_cast<PUCHAR>(decrypted.data), decrypted.size, 0);
    volatile int expected = 0xe5c3c820;
    expected ^= 0xf63071d0;
    expected ^= 0x13f3b990;
    if (written != expected) {
      return false;
    }
    break;
  }
  default:
    return false;
  }

  return XxHash32(decrypted.data, decrypted.size, 0) == header->checksum;
}

// OFFSET: 0x00658E90
// FUNCTION: fn_05113
__declspec(noinline) PakKeyBlock48 PakMaskedKey48::Unmask() const {
  PakKeyBlock48 result;
  for (std::size_t i = 0; i < sizeof(result.bytes); ++i) {
    result.bytes[i] = this->data[i] ^ this->mask[i & 3] ^ this->mask[(i & 3) + 1];
  }
  return result;
}

// OFFSET: 0x0065AA00
// FUNCTION: fn_05133
std::uint32_t IdeaMulInv(std::uint32_t x) {
  std::uint32_t y = x;
  for (unsigned int i = 0; i < 15; i++) {
    std::uint32_t low = y & 0xffff;
    std::uint32_t p = low * low;
    if (p) {
      p = (p & 0xffff) - (p >> 16);
      y = p - (p >> 16);
    } else {
      y = 1 - y - low;
    }

    p = (y & 0xffff) * x;
    if (p) {
      p = (p & 0xffff) - (p >> 16);
      y = p - (p >> 16);
    } else {
      y = 1 - y - x;
    }
  }
  return y & 0xffff;
}

// INLINED
inline void SecureWipe(void* buffer, std::size_t size) {
  volatile std::uint8_t* bytes = static_cast<std::uint8_t*>(buffer);
  while (size--) {
    *bytes++ = 0;
  }
}

// INLINED
template<std::uint32_t Key1, std::uint32_t Key2>
std::uint32_t Deobfuscate(std::uint32_t value) {
  volatile std::uint32_t encoded = value ^ Key1 ^ Key2;
  encoded = encoded ^ Key1;
  encoded = encoded ^ Key2;
  return encoded;
}

namespace GPakEx {

  struct SecureByteBuffer {
    /* 0000 */ std::uint8_t* first;
    /* 0004 */ std::uint8_t* last;
    /* 0008 */ std::uint8_t* end;

    ~SecureByteBuffer();
  };

  // OFFSET: 0x006429B0
  // FUNCTION: fn_04822
  SecureByteBuffer::~SecureByteBuffer() {
    for (std::uint8_t* current = this->first; current != this->last; ++current) {
      *current = 0;
    }

    if (this->first) {
      std::size_t allocationSize = static_cast<std::size_t>(this->end - this->first);
      void* allocation = this->first;
      if (allocationSize > 0xFFF) {
        allocation = reinterpret_cast<void**>(this->first)[-1];
        if (reinterpret_cast<std::uintptr_t>(this->first) - reinterpret_cast<std::uintptr_t>(allocation) - 4 > 0x1F) {
          _invalid_parameter_noinfo_noreturn();
        }
        allocationSize += 0x23;
      }
      ::operator delete(allocation, static_cast<unsigned int>(allocationSize));
      this->first = nullptr;
      this->last = nullptr;
      this->end = nullptr;
    }
  }

  // VTABLE: vt_00294
  class BlockCipher {
  public:
    virtual ~BlockCipher();
    void* ScalarDeletingDestructor(std::uint32_t flags);

    virtual bool IsForwardTransformation() const = 0;
    virtual std::size_t BlockSize() const = 0;
    virtual std::size_t OptimalDataAlignment() const = 0;
    virtual void SetKey(std::span<const std::uint8_t> key) = 0;
    virtual void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const = 0;
    virtual std::size_t AdvancedProcessBlocks(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output, std::size_t length, std::uint32_t flags) const;

    enum Flags : std::uint32_t { InPlace = 1, Reverse = 2, Increment = 4 };
  };

  // OFFSET: 0x00642A60
  // FUNCTION: fn_04823
  __declspec(noinline) BlockCipher::~BlockCipher() {
  }

  // OFFSET: 0x00642AD0
  // FUNCTION: fn_04825
  void* BlockCipher::ScalarDeletingDestructor(std::uint32_t flags) {
    this->~BlockCipher();
    if (flags & 1) {
      ::operator delete(this, 4);
    }
    return this;
  }

  // OFFSET: 0x006434B0
  // FUNCTION: fn_04838
  __declspec(noinline) void XorBuffers(std::uint8_t* output, const std::uint8_t* input, const std::uint8_t* mask, std::size_t length) {
    while (length >= 16) {
      const __m128i block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(input));
      const __m128i key = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mask));
      _mm_storeu_si128(reinterpret_cast<__m128i*>(output), _mm_xor_si128(block, key));
      output += 16;
      input += 16;
      mask += 16;
      length -= 16;
    }

    if (length == 0) {
      return;
    }

    for (std::size_t i = 0; i < length; ++i) {
      output[i] = input[i] ^ mask[i];
    }
  }

  // OFFSET: 0x00642B80
  // OFFSET: 0x00642B80
  // FUNCTION: fn_04827
  std::size_t BlockCipher::AdvancedProcessBlocks(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output, std::size_t length, std::uint32_t flags) const {
    const std::size_t blockSize = this->BlockSize();
    std::ptrdiff_t step = static_cast<std::ptrdiff_t>(blockSize);
    if (flags & Reverse) {
      if (!(flags & Increment)) {
        input = input ? input + length - blockSize : nullptr;
      }
      xorBlock = xorBlock ? xorBlock + length - blockSize : nullptr;
      output = output ? output + length - blockSize : nullptr;
      step = -step;
    }

    const bool xorFirst = xorBlock && (flags & InPlace);
    while (length >= blockSize) {
      if (xorFirst) {
        XorBuffers(output, xorBlock, input, blockSize);
        this->ProcessAndXorBlock(output, nullptr, output);
      } else {
        this->ProcessAndXorBlock(input, xorBlock, output);
      }

      if (flags & Increment) {
        ++const_cast<std::uint8_t*>(input)[blockSize - 1];
      } else {
        input = input ? input + step : nullptr;
      }
      length -= blockSize;
      xorBlock = xorBlock ? xorBlock + step : nullptr;
      output = output ? output + step : nullptr;
    }
    return length;
  }

  class TEA {
  public:
    class Base : public BlockCipher {
    public:
      ~Base() override {
        SecureWipe(this->m_key, sizeof(this->m_key));
      }

      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;

      /* 0004 */ std::uint32_t m_key[4]{};
    };

    class Enc : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };
  };

  // OFFSET: 0x00654410
  // FUNCTION: fn_04984
  std::size_t TEA::Base::BlockSize() const {
    return 8;
  }

  // OFFSET: 0x00654B60
  // FUNCTION: fn_05027
  std::size_t TEA::Base::OptimalDataAlignment() const {
    return 16;
  }

  // OFFSET: 0x0065BB10
  // FUNCTION: fn_05142
  // VIRTUALIZED: 2026-01-07 0x0065AAA0
  void TEA::Base::SetKey(std::span<const std::uint8_t> key) {
    memcpy_s(this->m_key, sizeof(this->m_key), key.data(), key.size());
    for (std::uint32_t& word : this->m_key) {
      word = _byteswap_ulong(word);
    }
  }

  // OFFSET: 0x0065B940
  // FUNCTION: fn_05140
  // VIRTUALIZED: 2026-01-07 0x0065A920
  void TEA::Enc::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    std::uint32_t y = _byteswap_ulong(in[0]);
    std::uint32_t z = _byteswap_ulong(in[1]);

    std::uint32_t sum = 0;
    for (int round = 0; round < 16; ++round) {
      sum += 0x9E3779B9;
      y += ((z << 4) + this->m_key[0]) ^ (z + sum) ^ ((z >> 5) + this->m_key[1]);
      z += ((y << 4) + this->m_key[2]) ^ (y + sum) ^ ((y >> 5) + this->m_key[3]);
    }

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = _byteswap_ulong(y) ^ (mask ? mask[0] : 0);
    out[1] = _byteswap_ulong(z) ^ (mask ? mask[1] : 0);
  }

  class XTEA {
  public:
    class Base : public BlockCipher {
    public:
      ~Base() override {
        SecureWipe(this->m_key, sizeof(this->m_key));
      }

      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;

      /* 0004 */ std::uint32_t m_key[4]{};
    };

    class Enc : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };
  };

  // OFFSET: 0x00654440
  // FUNCTION: fn_04987
  std::size_t XTEA::Base::BlockSize() const {
    return 8;
  }

  // OFFSET: 0x00654B90
  // FUNCTION: fn_05030
  std::size_t XTEA::Base::OptimalDataAlignment() const {
    return 16;
  }

  // OFFSET: 0x0065BBD0
  // FUNCTION: fn_05143
  // VIRTUALIZED: 2026-01-07 0x0065AB20
  void XTEA::Base::SetKey(std::span<const std::uint8_t> key) {
    memcpy_s(this->m_key, sizeof(this->m_key), key.data(), key.size());
    for (std::uint32_t& word : this->m_key) {
      word = _byteswap_ulong(word);
    }
  }

  // OFFSET: 0x0065BA30
  // FUNCTION: fn_05141
  // VIRTUALIZED: 2026-01-07 0x0065A9E0
  void XTEA::Enc::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    std::uint32_t y = _byteswap_ulong(in[0]);
    std::uint32_t z = _byteswap_ulong(in[1]);

    std::uint32_t sum = 0;
    for (int round = 0; round < 16; ++round) {
      y += (((z << 4) ^ (z >> 5)) + z) ^ (sum + this->m_key[sum & 3]);
      sum += 0x9E3779B9;
      z += (((y << 4) ^ (y >> 5)) + y) ^ (sum + this->m_key[(sum >> 11) & 3]);
    }

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = _byteswap_ulong(y) ^ (mask ? mask[0] : 0);
    out[1] = _byteswap_ulong(z) ^ (mask ? mask[1] : 0);
  }

  // INLINED
  inline std::uint32_t RotateLeft(std::uint32_t value, std::uint32_t count) {
    return _rotl(value, static_cast<int>(count & 31));
  }

  // INLINED
  __forceinline void ExpandRcKey(std::vector<std::uint32_t>& table, std::size_t rounds, std::span<const std::uint8_t> key) {
    table.resize(rounds);
    std::uint32_t words[4];
    memcpy_s(words, sizeof(words), key.data(), key.size());

    table[0] = 0xB7E15163;
    for (std::size_t i = 1; i < table.size(); ++i) {
      table[i] = table[i - 1] + 0x9E3779B9;
    }

    std::uint32_t a = 0;
    std::uint32_t b = 0;
    const std::size_t count = 3 * (std::max)(table.size(), std::size_t{4});
    for (std::size_t h = 0; h < count; ++h) {
      a = table[h % table.size()] = RotateLeft(table[h % table.size()] + a + b, 3);
      b = words[h & 3] = RotateLeft(words[h & 3] + a + b, a + b);
    }
    SecureWipe(words, sizeof(words));
  }

  class RC5 {
  public:
    class Base : public BlockCipher {
    public:
      ~Base() override;

      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;

      /* 0004 */ std::vector<std::uint32_t> m_table;
    };

    class Enc : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };
  };

  // OFFSET: 0x0064A880
  // FUNCTION: fn_04909
  __declspec(noinline) RC5::Base::~Base() {
    SecureWipe(this->m_table.data(), this->m_table.size() * sizeof(std::uint32_t));
  }

  // OFFSET: 0x00654430
  // FUNCTION: fn_04986
  std::size_t RC5::Base::BlockSize() const {
    return 8;
  }

  // OFFSET: 0x00654B80
  // FUNCTION: fn_05029
  std::size_t RC5::Base::OptimalDataAlignment() const {
    return 16;
  }

  // OFFSET: 0x0065B540
  // FUNCTION: fn_05139
  // VIRTUALIZED: 2026-01-07 0x0065A410
  void RC5::Base::SetKey(std::span<const std::uint8_t> key) {
    ExpandRcKey(this->m_table, 42, key);
  }

  // OFFSET: 0x0065B2E0
  // FUNCTION: fn_05137
  // VIRTUALIZED: 2026-01-07 0x0065A280
  void RC5::Enc::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    const std::uint32_t* s = this->m_table.data();
    std::uint32_t a = in[0] + s[0];
    std::uint32_t b = in[1] + s[1];
    for (std::size_t i = 1; i <= 20; ++i) {
      a = RotateLeft(a ^ b, b) + s[2 * i];
      b = RotateLeft(b ^ a, a) + s[2 * i + 1];
    }

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = a ^ (mask ? mask[0] : 0);
    out[1] = b ^ (mask ? mask[1] : 0);
  }

  class RC6 {
  public:
    class Base : public BlockCipher {
    public:
      ~Base() override;
      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;

      /* 0004 */ std::vector<std::uint32_t> m_table;
    };

    class Enc : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };
  };

  // OFFSET: 0x00654400
  // FUNCTION: fn_04983
  std::size_t RC6::Base::BlockSize() const {
    return 16;
  }

  // OFFSET: 0x00654B50
  // FUNCTION: fn_05026
  std::size_t RC6::Base::OptimalDataAlignment() const {
    return 16;
  }

  // OFFSET: 0x0064A730
  // FUNCTION: fn_04908
  RC6::Base::~Base() {
    SecureWipe(this->m_table.data(), this->m_table.size() * sizeof(std::uint32_t));
  }

  // OFFSET: 0x0065B380
  // FUNCTION: fn_05138
  // VIRTUALIZED: 2026-01-07 0x0065A5A0
  void RC6::Base::SetKey(std::span<const std::uint8_t> key) {
    ExpandRcKey(this->m_table, 44, key);
  }

  // OFFSET: 0x0065B1B0
  // FUNCTION: fn_05136
  // VIRTUALIZED: 2026-01-07 0x0065A300
  void RC6::Enc::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    const std::uint32_t* s = this->m_table.data();
    std::uint32_t a = in[0];
    std::uint32_t b = in[1] + s[0];
    std::uint32_t c = in[2];
    std::uint32_t d = in[3] + s[1];
    for (std::size_t i = 1; i <= 20; ++i) {
      const std::uint32_t t = RotateLeft(b * (2 * b + 1), 5);
      const std::uint32_t u = RotateLeft(d * (2 * d + 1), 5);
      a = RotateLeft(a ^ t, u) + s[2 * i];
      c = RotateLeft(c ^ u, t) + s[2 * i + 1];
      const std::uint32_t first = a;
      a = b;
      b = c;
      c = d;
      d = first;
    }
    a += s[42];
    c += s[43];

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = a ^ (mask ? mask[0] : 0);
    out[1] = b ^ (mask ? mask[1] : 0);
    out[2] = c ^ (mask ? mask[2] : 0);
    out[3] = d ^ (mask ? mask[3] : 0);
  }

  // INLINED
  inline std::uint32_t IdeaMultiply(std::uint32_t a, std::uint32_t b) {
    const std::uint32_t p = (a & 0xffff) * (b & 0xffff);
    if (p) {
      const std::uint32_t r = (p & 0xffff) - (p >> 16);
      return r - (r >> 16);
    }
    return 1 - a - b;
  }

  class IDEA {
  public:
    class Base : public BlockCipher {
    public:
      Base() : m_key{} {
      }

      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;

      /* 0004 */ std::uint32_t m_key[52];
    };
  };

  // OFFSET: 0x00654450
  // FUNCTION: fn_04988
  std::size_t IDEA::Base::BlockSize() const {
    return 8;
  }

  // OFFSET: 0x00654BA0
  // FUNCTION: fn_05031
  std::size_t IDEA::Base::OptimalDataAlignment() const {
    return 16;
  }

  // OFFSET: 0x0065ACF0
  // FUNCTION: fn_05135
  // VIRTUALIZED: 2026-01-07 0x00659E50
  void IDEA::Base::SetKey(std::span<const std::uint8_t> key) {
    const std::uint8_t* bytes = key.data();
    for (std::size_t i = 0; i < 8; ++i) {
      this->m_key[i] = (bytes[2 * i] << 8) | bytes[2 * i + 1];
    }
    for (std::size_t i = 8; i < 52; ++i) {
      const std::size_t base = (i & ~7) - 8;
      this->m_key[i] = ((this->m_key[base + ((i + 1) & 7)] << 9) | (this->m_key[base + ((i + 2) & 7)] >> 7)) & 0xffff;
    }

    if (this->IsForwardTransformation()) {
      return;
    }

    std::uint32_t inverted[52];
    for (std::size_t round = 0; round < 8; ++round) {
      const std::uint32_t* k = &this->m_key[48 - 6 * round];
      std::uint32_t* out = &inverted[6 * round];
      out[0] = IdeaMulInv(k[0]);
      out[1] = (0 - k[round == 0 ? 1 : 2]) & 0xffff;
      out[2] = (0 - k[round == 0 ? 2 : 1]) & 0xffff;
      out[3] = IdeaMulInv(k[3]);
      out[4] = k[-2];
      out[5] = k[-1];
    }
    inverted[48] = IdeaMulInv(this->m_key[0]);
    inverted[49] = (0 - this->m_key[1]) & 0xffff;
    inverted[50] = (0 - this->m_key[2]) & 0xffff;
    inverted[51] = IdeaMulInv(this->m_key[3]);
    std::memcpy(this->m_key, inverted, sizeof(inverted));
    SecureWipe(inverted, sizeof(inverted));
  }

  // OFFSET: 0x0065AA70
  // FUNCTION: fn_05134
  // VIRTUALIZED: 2026-01-07 0x00659C00
  void IDEA::Base::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint16_t* in = reinterpret_cast<const std::uint16_t*>(input);
    std::uint32_t x0 = _byteswap_ushort(in[0]);
    std::uint32_t x1 = _byteswap_ushort(in[1]);
    std::uint32_t x2 = _byteswap_ushort(in[2]);
    std::uint32_t x3 = _byteswap_ushort(in[3]);

    const std::uint32_t* k = this->m_key;
    for (int round = 0; round < 8; ++round, k += 6) {
      x0 = IdeaMultiply(x0, k[0]);
      x1 += k[1];
      x2 += k[2];
      x3 = IdeaMultiply(x3, k[3]);
      std::uint32_t t0 = x0 ^ x2;
      t0 = IdeaMultiply(t0, k[4]);
      std::uint32_t t1 = t0 + (x1 ^ x3);
      t1 = IdeaMultiply(t1, k[5]);
      t0 += t1;
      x0 ^= t1;
      x3 ^= t0;
      t0 ^= x1;
      x1 = x2 ^ t1;
      x2 = t0;
    }

    const std::uint16_t result[4] = {
      _byteswap_ushort(static_cast<std::uint16_t>(IdeaMultiply(x0, k[0]))),
      _byteswap_ushort(static_cast<std::uint16_t>(x2 + k[1])),
      _byteswap_ushort(static_cast<std::uint16_t>(x1 + k[2])),
      _byteswap_ushort(static_cast<std::uint16_t>(IdeaMultiply(x3, k[3]))),
    };
    const std::uint16_t* mask = reinterpret_cast<const std::uint16_t*>(xorBlock);
    std::uint16_t* out = reinterpret_cast<std::uint16_t*>(output);
    for (int i = 0; i < 4; ++i) {
      out[i] = result[i] ^ (mask ? mask[i] : 0);
    }
  }

  // INLINED
  inline std::uint32_t ReverseBits(std::uint32_t value) {
    value = ((value & 0xAAAAAAAA) >> 1) | ((value & 0x55555555) << 1);
    value = ((value & 0xCCCCCCCC) >> 2) | ((value & 0x33333333) << 2);
    value = ((value & 0xF0F0F0F0) >> 4) | ((value & 0x0F0F0F0F) << 4);
    return value;
  }

  // INLINED
  inline void ThreeWayMu(std::uint32_t& a0, std::uint32_t& a1, std::uint32_t& a2) {
    a1 = ReverseBits(a1);
    const std::uint32_t t = ReverseBits(a0);
    a0 = ReverseBits(a2);
    a2 = t;
  }

  // INLINED
  inline void ThreeWayTheta(std::uint32_t& a0, std::uint32_t& a1, std::uint32_t& a2) {
    const std::uint32_t c = a0 ^ a1 ^ a2;
    const std::uint32_t mix = _rotl(c, 16) ^ _rotl(c, 8);
    const std::uint32_t b0 = ((a1 ^ (a0 << 16)) << 8) ^ ((a2 ^ (a0 >> 16)) >> 8);
    const std::uint32_t b1 = ((a0 ^ (a1 >> 16)) >> 8) ^ ((a2 ^ (a1 << 16)) << 8);
    a0 ^= mix ^ b0;
    a1 ^= mix ^ b1;
    a2 ^= mix ^ (b0 >> 16) ^ (b1 << 16);
  }

  // INLINED
  inline void ThreeWayRho(std::uint32_t& a0, std::uint32_t& a1, std::uint32_t& a2) {
    ThreeWayTheta(a0, a1, a2);
    a0 = _rotr(a0, 10);
    a2 = _rotl(a2, 1);
    const std::uint32_t g0 = a0 ^ (a1 | ~a2);
    const std::uint32_t g1 = a1 ^ (a2 | ~a0);
    const std::uint32_t g2 = a2 ^ (a0 | ~a1);
    a0 = _rotl(g0, 1);
    a1 = g1;
    a2 = _rotr(g2, 10);
  }

  // INLINED
  inline std::uint32_t ThreeWayNextConstant(std::uint32_t rc) {
    rc <<= 1;
    return (rc & 0x10000) ? rc ^ 0x11011 : rc;
  }

  class ThreeWay {
  public:
    class Base : public BlockCipher {
    public:
      ~Base() override {
        SecureWipe(this->m_key, sizeof(this->m_key));
      }

      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;

      /* 0004 */ std::uint32_t m_key[3]{};
    };

    class Enc : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };

    class Dec : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };
  };

  // OFFSET: 0x00654420
  // FUNCTION: fn_04985
  std::size_t ThreeWay::Base::BlockSize() const {
    return 12;
  }

  // OFFSET: 0x00654B70
  // FUNCTION: fn_05028
  std::size_t ThreeWay::Base::OptimalDataAlignment() const {
    return 12;
  }

  // OFFSET: 0x0065C230
  // FUNCTION: fn_05146
  // VIRTUALIZED: 2026-01-07 0x0065B0F0
  void ThreeWay::Base::SetKey(std::span<const std::uint8_t> key) {
    const std::uint8_t* bytes = key.data();
    for (std::size_t i = 0; i < 3; ++i) {
      this->m_key[i] = (bytes[4 * i] << 24) | (bytes[4 * i + 1] << 16) | (bytes[4 * i + 2] << 8) | bytes[4 * i + 3];
    }

    if (this->IsForwardTransformation()) {
      return;
    }

    ThreeWayTheta(this->m_key[0], this->m_key[1], this->m_key[2]);
    ThreeWayMu(this->m_key[0], this->m_key[1], this->m_key[2]);
    for (std::uint32_t& word : this->m_key) {
      word = _byteswap_ulong(word);
    }
  }

  // OFFSET: 0x0065C030
  // FUNCTION: fn_05145
  // VIRTUALIZED: 2026-01-07 0x0065AF20
  void ThreeWay::Enc::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    std::uint32_t a0 = _byteswap_ulong(in[0]);
    std::uint32_t a1 = _byteswap_ulong(in[1]);
    std::uint32_t a2 = _byteswap_ulong(in[2]);

    std::uint32_t rc = 0x0B0B;
    for (int round = 0; round < 11; ++round) {
      a0 ^= this->m_key[0] ^ (rc << 16);
      a1 ^= this->m_key[1];
      a2 ^= this->m_key[2] ^ rc;
      ThreeWayRho(a0, a1, a2);
      rc = ThreeWayNextConstant(rc);
    }
    a0 ^= this->m_key[0] ^ (rc << 16);
    a1 ^= this->m_key[1];
    a2 ^= this->m_key[2] ^ rc;
    ThreeWayTheta(a0, a1, a2);

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = _byteswap_ulong(a0) ^ (mask ? mask[0] : 0);
    out[1] = _byteswap_ulong(a1) ^ (mask ? mask[1] : 0);
    out[2] = _byteswap_ulong(a2) ^ (mask ? mask[2] : 0);
  }

  // OFFSET: 0x0065BC90
  // FUNCTION: fn_05144
  // VIRTUALIZED: 2026-01-07 0x0065ABA0
  void ThreeWay::Dec::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    std::uint32_t a0 = in[0];
    std::uint32_t a1 = in[1];
    std::uint32_t a2 = in[2];
    ThreeWayMu(a0, a1, a2);

    std::uint32_t rc = 0xB1B1;
    for (int round = 0; round < 11; ++round) {
      a0 ^= this->m_key[0] ^ (rc << 16);
      a1 ^= this->m_key[1];
      a2 ^= this->m_key[2] ^ rc;
      ThreeWayRho(a0, a1, a2);
      rc = ThreeWayNextConstant(rc);
    }
    a0 ^= this->m_key[0] ^ (rc << 16);
    a1 ^= this->m_key[1];
    a2 ^= this->m_key[2] ^ rc;
    ThreeWayTheta(a0, a1, a2);
    ThreeWayMu(a0, a1, a2);

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = a0 ^ (mask ? mask[0] : 0);
    out[1] = a1 ^ (mask ? mask[1] : 0);
    out[2] = a2 ^ (mask ? mask[2] : 0);
  }

  class GOST {
  public:
    class Base : public BlockCipher {
    public:
      ~Base() override {
        SecureWipe(this->m_key, sizeof(this->m_key));
      }

      std::size_t BlockSize() const override;
      std::size_t OptimalDataAlignment() const override;
      void SetKey(std::span<const std::uint8_t> key) override;

      static void PrecalculateSTable();

      static const std::uint8_t s_sbox[8][16];
      static bool s_sTableCalculated;
      static std::uint32_t s_sTable[4][256];

      /* 0004 */ std::uint32_t m_key[8]{};
    };

    class Enc : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };

    class Dec : public Base {
    public:
      void ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const override;
    };
  };

  // OFFSET: 0x00658FA0
  // FUNCTION: fn_05114
  std::size_t GOST::Base::BlockSize() const {
    return 8;
  }

  // OFFSET: 0x00658FB0
  // FUNCTION: fn_05115
  std::size_t GOST::Base::OptimalDataAlignment() const {
    return 32;
  }

  // GLOBAL: gl_25481
  const std::uint8_t GOST::Base::s_sbox[8][16] = {
    {0x0b, 0x04, 0x0c, 0x07, 0x0c, 0x00, 0x09, 0x0d, 0x03, 0x0e, 0x0b, 0x0b, 0x09, 0x08, 0x00, 0x04},
    {0x0c, 0x01, 0x04, 0x05, 0x0e, 0x03, 0x04, 0x0c, 0x07, 0x04, 0x0a, 0x0e, 0x07, 0x0f, 0x0e, 0x00},
    {0x0e, 0x0b, 0x02, 0x0a, 0x02, 0x0b, 0x0b, 0x07, 0x05, 0x0a, 0x00, 0x0f, 0x07, 0x00, 0x09, 0x00},
    {0x04, 0x08, 0x0c, 0x07, 0x09, 0x03, 0x06, 0x04, 0x0c, 0x0e, 0x0a, 0x0f, 0x0d, 0x00, 0x08, 0x01},
    {0x06, 0x0f, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x03, 0x0a, 0x0e, 0x06, 0x0c, 0x07, 0x03, 0x05, 0x03},
    {0x03, 0x04, 0x0f, 0x0d, 0x03, 0x03, 0x0d, 0x04, 0x01, 0x06, 0x0f, 0x0a, 0x0d, 0x06, 0x0c, 0x0f},
    {0x02, 0x0a, 0x04, 0x0c, 0x01, 0x00, 0x0e, 0x00, 0x09, 0x0c, 0x0b, 0x0b, 0x09, 0x05, 0x03, 0x09},
    {0x0e, 0x09, 0x08, 0x07, 0x00, 0x0f, 0x03, 0x07, 0x04, 0x07, 0x0f, 0x0d, 0x0b, 0x0a, 0x06, 0x02},
  };

  // GLOBAL: gl_25483
  bool GOST::Base::s_sTableCalculated;

  // GLOBAL: gl_25484
  std::uint32_t GOST::Base::s_sTable[4][256];

  // INLINED
  void GOST::Base::PrecalculateSTable() {
    if (s_sTableCalculated) {
      return;
    }
    for (std::size_t i = 0; i < 4; ++i) {
      for (std::uint32_t j = 0; j < 256; ++j) {
        const std::uint32_t value = (s_sbox[2 * i + 1][j >> 4] << 4) | s_sbox[2 * i][j & 15];
        s_sTable[i][j] = _rotl(value, static_cast<int>(11 + 8 * i));
      }
    }
    s_sTableCalculated = true;
  }

  // OFFSET: 0x00644130
  // FUNCTION: fn_04841
  // VIRTUALIZED: 2026-01-07 0x00643180
  void GOST::Base::SetKey(std::span<const std::uint8_t> key) {
    PrecalculateSTable();
    memcpy_s(this->m_key, sizeof(this->m_key), key.data(), key.size());
  }

  // INLINED
  inline std::uint32_t GostRound(std::uint32_t value) {
    return GOST::Base::s_sTable[0][value & 0xff] ^ GOST::Base::s_sTable[1][(value >> 8) & 0xff] ^ GOST::Base::s_sTable[2][(value >> 16) & 0xff] ^
           GOST::Base::s_sTable[3][value >> 24];
  }

  // OFFSET: 0x00643CC0
  // FUNCTION: fn_04840
  // VIRTUALIZED: 2026-01-07 0x00642D30
  void GOST::Enc::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    std::uint32_t n1 = in[0];
    std::uint32_t n2 = in[1];

    for (int pass = 0; pass < 3; ++pass) {
      for (std::size_t i = 0; i < 8; i += 2) {
        n2 ^= GostRound(n1 + this->m_key[i]);
        n1 ^= GostRound(n2 + this->m_key[i + 1]);
      }
    }
    for (std::size_t i = 8; i > 0; i -= 2) {
      n2 ^= GostRound(n1 + this->m_key[i - 1]);
      n1 ^= GostRound(n2 + this->m_key[i - 2]);
    }

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = n2 ^ (mask ? mask[0] : 0);
    out[1] = n1 ^ (mask ? mask[1] : 0);
  }

  // OFFSET: 0x00643860
  // FUNCTION: fn_04839
  // VIRTUALIZED: 2026-01-07 0x006428F0
  void GOST::Dec::ProcessAndXorBlock(const std::uint8_t* input, const std::uint8_t* xorBlock, std::uint8_t* output) const {
    const std::uint32_t* in = reinterpret_cast<const std::uint32_t*>(input);
    std::uint32_t n1 = in[0];
    std::uint32_t n2 = in[1];

    for (std::size_t i = 0; i < 8; i += 2) {
      n2 ^= GostRound(n1 + this->m_key[i]);
      n1 ^= GostRound(n2 + this->m_key[i + 1]);
    }
    for (int pass = 0; pass < 3; ++pass) {
      for (std::size_t i = 8; i > 0; i -= 2) {
        n2 ^= GostRound(n1 + this->m_key[i - 1]);
        n1 ^= GostRound(n2 + this->m_key[i - 2]);
      }
    }

    const std::uint32_t* mask = reinterpret_cast<const std::uint32_t*>(xorBlock);
    std::uint32_t* out = reinterpret_cast<std::uint32_t*>(output);
    out[0] = n2 ^ (mask ? mask[0] : 0);
    out[1] = n1 ^ (mask ? mask[1] : 0);
  }

  class CipherModeBase {
  public:
    virtual ~CipherModeBase();

    virtual bool IsForwardTransformation() const = 0;
    virtual void ResizeBuffers();
    virtual void ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) = 0;

    std::size_t CiphertextLength(std::size_t length, bool padding) const;
    std::size_t Process(std::span<const std::uint8_t> input, std::span<std::uint8_t> output, bool final);
    void SetKeyWithIV(std::span<const std::uint8_t> key, std::span<const std::uint8_t> iv);

    /* 0004 */ BlockCipher* m_cipher;
    /* 0008 */ std::vector<std::uint8_t> m_register;
    /* 0014 */ std::vector<std::uint8_t> m_buffer;
  };

  static_assert(sizeof(CipherModeBase) == 0x20);

  // OFFSET: 0x00642A70
  // FUNCTION: fn_04824
  __declspec(noinline) CipherModeBase::~CipherModeBase() {
  }

  // OFFSET: 0x006433A0
  // FUNCTION: fn_04836
  void CipherModeBase::ResizeBuffers() {
    const std::size_t blockSize = this->m_cipher->BlockSize();
    this->m_register.resize(blockSize);
    this->m_buffer.resize(blockSize);
  }

  // OFFSET: 0x00642C80
  // FUNCTION: fn_04828
  __declspec(noinline) std::size_t CipherModeBase::CiphertextLength(std::size_t length, bool padding) const {
    const std::size_t blockSize = this->m_cipher->BlockSize();
    if (length == 0) {
      return padding ? blockSize : 0;
    }
    if (!padding) {
      return (length + blockSize - 1) - (length + blockSize - 1) % blockSize;
    }
    return length + (blockSize - length % blockSize);
  }

  // OFFSET: 0x00643160
  // FUNCTION: fn_04833
  __declspec(noinline) std::size_t CipherModeBase::Process(std::span<const std::uint8_t> input, std::span<std::uint8_t> output, bool final) {
    const bool forward = this->IsForwardTransformation();
    const std::size_t blockSize = this->m_cipher->BlockSize();
    const std::size_t reserved = (!forward && final) ? blockSize : 0;
    const std::uint8_t* in = input.data();
    std::size_t inLength = input.size();
    std::uint8_t* out = output.data();
    std::size_t outLength = output.size();

    std::size_t done = 0;
    if (inLength >= reserved + blockSize) {
      done = (inLength - reserved) - (inLength - reserved) % blockSize;
      if (outLength < done) {
        return static_cast<std::size_t>(-1);
      }
      this->ProcessData(in, out, done);
      in += done;
      out += done;
      inLength -= done;
      outLength -= done;
    }

    if (!final) {
      if (inLength == 0) {
        return done;
      }
    } else if (!forward) {
      if (inLength == blockSize) {
        this->ProcessData(in, this->m_buffer.data(), blockSize);
        const std::uint8_t pad = this->m_buffer[blockSize - 1];
        const std::size_t size = blockSize - pad;
        if (pad != 0 && pad <= blockSize && size <= outLength) {
          std::memcpy(out, this->m_buffer.data(), size);
          return done + size;
        }
      }
    } else {
      std::memcpy(this->m_buffer.data(), in, inLength);
      const std::uint8_t pad = static_cast<std::uint8_t>(blockSize - inLength);
      std::memset(this->m_buffer.data() + inLength, pad, pad);
      if (blockSize <= outLength) {
        this->ProcessData(this->m_buffer.data(), out, blockSize);
        return done + blockSize;
      }
    }
    return static_cast<std::size_t>(-1);
  }

  // OFFSET: 0x00643480
  // FUNCTION: fn_04837
  __declspec(noinline) void CipherModeBase::SetKeyWithIV(std::span<const std::uint8_t> key, std::span<const std::uint8_t> iv) {
    this->m_cipher->SetKey(key);
    std::memcpy(this->m_register.data(), iv.data(), this->m_register.size());
  }

  class CBC_Encryption : public CipherModeBase {
  public:
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) override;
  };

  // OFFSET: 0x00642E20
  // FUNCTION: fn_04830
  void CBC_Encryption::ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) {
    const std::size_t blockSize = this->m_cipher->BlockSize();
    if (length == 0) {
      return;
    }
    this->m_cipher->AdvancedProcessBlocks(input, this->m_register.data(), output, blockSize, BlockCipher::InPlace);
    if (length > blockSize) {
      this->m_cipher->AdvancedProcessBlocks(input ? input + blockSize : nullptr, output, output ? output + blockSize : nullptr, length - blockSize, BlockCipher::InPlace);
    }
    std::memcpy(this->m_register.data(), output ? output + length - blockSize : nullptr, blockSize);
  }

  class CBC_Decryption : public CipherModeBase {
  public:
    CBC_Decryption();

    void ResizeBuffers() override;
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) override;

    /* 0020 */ std::vector<std::uint8_t> m_temp;
  };

  // OFFSET: 0x00656BC0
  // FUNCTION: fn_05079
  __declspec(noinline) CBC_Decryption::CBC_Decryption() {
  }

  // OFFSET: 0x006432C0
  // FUNCTION: fn_04834
  void CBC_Decryption::ResizeBuffers() {
    this->CipherModeBase::ResizeBuffers();
    this->m_temp.resize(this->m_cipher->BlockSize());
  }

  // OFFSET: 0x00642D60
  // FUNCTION: fn_04829
  void CBC_Decryption::ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) {
    const std::size_t blockSize = this->m_cipher->BlockSize();
    if (length == 0) {
      return;
    }
    std::memcpy(this->m_temp.data(), input ? input + length - blockSize : nullptr, blockSize);
    if (length > blockSize) {
      this->m_cipher->AdvancedProcessBlocks(input ? input + blockSize : nullptr, input, output ? output + blockSize : nullptr, length - blockSize, BlockCipher::Reverse);
    }
    this->m_cipher->ProcessAndXorBlock(input, this->m_register.data(), output);
    this->m_register.swap(this->m_temp);
  }

  class CFB_Decryption : public CipherModeBase {
  public:
    CFB_Decryption();

    void ResizeBuffers() override;
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) override;

    /* 0020 */ std::vector<std::uint8_t> m_temp;
  };

  // OFFSET: 0x006471C0
  // FUNCTION: fn_04896
  __declspec(noinline) CFB_Decryption::CFB_Decryption() {
  }

  // OFFSET: 0x00643330
  // FUNCTION: fn_04835
  void CFB_Decryption::ResizeBuffers() {
    this->CipherModeBase::ResizeBuffers();
    this->m_temp.resize(this->m_cipher->BlockSize());
  }

  // OFFSET: 0x00642EB0
  // FUNCTION: fn_04831
  void CFB_Decryption::ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) {
    const std::size_t blockSize = this->m_cipher->BlockSize();
    if (length == 0) {
      return;
    }
    std::memcpy(this->m_temp.data(), input ? input + length - blockSize : nullptr, blockSize);
    if (blockSize < length) {
      this->m_cipher->AdvancedProcessBlocks(input, input ? input + blockSize : nullptr, output ? output + blockSize : nullptr, length - blockSize, BlockCipher::Reverse);
    }
    this->m_cipher->ProcessAndXorBlock(this->m_register.data(), input, output);
    this->m_register.swap(this->m_temp);
  }

  class OFB_Base : public CipherModeBase {
  public:
    OFB_Base();

    void ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) override;
  };

  // OFFSET: 0x00647270
  // FUNCTION: fn_04897
  __declspec(noinline) OFB_Base::OFB_Base() {
  }

  // OFFSET: 0x006430F0
  // FUNCTION: fn_04832
  void OFB_Base::ProcessData(const std::uint8_t* input, std::uint8_t* output, std::size_t length) {
    const std::size_t blockSize = this->m_cipher->BlockSize();
    while (length > 0) {
      this->m_cipher->ProcessAndXorBlock(this->m_register.data(), nullptr, this->m_register.data());
      XorBuffers(output, input, this->m_register.data(), blockSize);
      length -= blockSize;
      input = input ? input + blockSize : nullptr;
      output = output ? output + blockSize : nullptr;
    }
  }

  template<bool Forward, class Cipher>
  class BlockCipherFinal : public Cipher {
  public:
    bool IsForwardTransformation() const override;
  };

  template<class Algorithm, bool Forward, class Cipher, class Mode>
  class CipherModeFinal : public Mode {
  public:
    CipherModeFinal();
    ~CipherModeFinal() override;

    bool IsForwardTransformation() const override;

    Cipher m_blockCipher;
  };

  using TeaCfbDecryption = CipherModeFinal<TEA, true, BlockCipherFinal<false, TEA::Enc>, CFB_Decryption>;

  // OFFSET: 0x00654C20
  // FUNCTION: fn_05036
  template<>
  bool BlockCipherFinal<false, TEA::Enc>::IsForwardTransformation() const {
    return true;
  }

  // OFFSET: 0x00654C80
  // FUNCTION: fn_05042
  template<>
  bool TeaCfbDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646B40
  // FUNCTION: fn_04887
  template<>
  __declspec(noinline) TeaCfbDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // OFFSET: 0x00649C50
  // FUNCTION: fn_04900
  template<>
  __declspec(noinline) TeaCfbDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(TeaCfbDecryption, m_blockCipher) == 0x2C);

  using TeaOfb = CipherModeFinal<TEA, true, BlockCipherFinal<false, TEA::Enc>, OFB_Base>;

  // OFFSET: 0x00654C90
  // FUNCTION: fn_05043
  template<>
  bool TeaOfb::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646BE0
  // FUNCTION: fn_04888
  template<>
  __declspec(noinline) TeaOfb::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline TeaOfb::~CipherModeFinal() {
  }

  static_assert(offsetof(TeaOfb, m_blockCipher) == 0x20);

  using XteaCfbDecryption = CipherModeFinal<XTEA, true, BlockCipherFinal<false, XTEA::Enc>, CFB_Decryption>;

  // OFFSET: 0x00654C50
  // FUNCTION: fn_05039
  template<>
  bool BlockCipherFinal<false, XTEA::Enc>::IsForwardTransformation() const {
    return true;
  }

  // OFFSET: 0x00654CE0
  // FUNCTION: fn_05048
  template<>
  bool XteaCfbDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646F90
  // FUNCTION: fn_04893
  template<>
  __declspec(noinline) XteaCfbDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // OFFSET: 0x00649FB0
  // FUNCTION: fn_04903
  template<>
  __declspec(noinline) XteaCfbDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(XteaCfbDecryption, m_blockCipher) == 0x2C);

  using XteaOfb = CipherModeFinal<XTEA, true, BlockCipherFinal<false, XTEA::Enc>, OFB_Base>;

  // OFFSET: 0x00654CF0
  // FUNCTION: fn_05049
  template<>
  bool XteaOfb::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00647030
  // FUNCTION: fn_04894
  template<>
  __declspec(noinline) XteaOfb::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline XteaOfb::~CipherModeFinal() {
  }

  static_assert(offsetof(XteaOfb, m_blockCipher) == 0x20);

  // OFFSET: 0x00654C10
  // FUNCTION: fn_05035
  template<>
  bool BlockCipherFinal<false, RC6::Enc>::IsForwardTransformation() const {
    return true;
  }

  // OFFSET: 0x00654C40
  // FUNCTION: fn_05038
  template<>
  bool BlockCipherFinal<false, RC5::Enc>::IsForwardTransformation() const {
    return true;
  }

  using Rc6CfbDecryption = CipherModeFinal<RC6, true, BlockCipherFinal<false, RC6::Enc>, CFB_Decryption>;

  // OFFSET: 0x00654C60
  // FUNCTION: fn_05040
  template<>
  bool Rc6CfbDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x006469E0
  // FUNCTION: fn_04885
  template<>
  __declspec(noinline) Rc6CfbDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // OFFSET: 0x00649B50
  // FUNCTION: fn_04899
  template<>
  __declspec(noinline) Rc6CfbDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(Rc6CfbDecryption, m_blockCipher) == 0x2C);

  using Rc6Ofb = CipherModeFinal<RC6, true, BlockCipherFinal<false, RC6::Enc>, OFB_Base>;

  // OFFSET: 0x00654C70
  // FUNCTION: fn_05041
  template<>
  bool Rc6Ofb::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646A90
  // FUNCTION: fn_04886
  template<>
  __declspec(noinline) Rc6Ofb::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline Rc6Ofb::~CipherModeFinal() {
  }

  static_assert(offsetof(Rc6Ofb, m_blockCipher) == 0x20);

  using Rc5CfbDecryption = CipherModeFinal<RC5, true, BlockCipherFinal<false, RC5::Enc>, CFB_Decryption>;

  // OFFSET: 0x00654CC0
  // FUNCTION: fn_05046
  template<>
  bool Rc5CfbDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646E30
  // FUNCTION: fn_04891
  template<>
  __declspec(noinline) Rc5CfbDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // OFFSET: 0x00649EB0
  // FUNCTION: fn_04902
  template<>
  __declspec(noinline) Rc5CfbDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(Rc5CfbDecryption, m_blockCipher) == 0x2C);

  using Rc5Ofb = CipherModeFinal<RC5, true, BlockCipherFinal<false, RC5::Enc>, OFB_Base>;

  // OFFSET: 0x00654CD0
  // FUNCTION: fn_05047
  template<>
  bool Rc5Ofb::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646EE0
  // FUNCTION: fn_04892
  template<>
  __declspec(noinline) Rc5Ofb::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline Rc5Ofb::~CipherModeFinal() {
  }

  static_assert(offsetof(Rc5Ofb, m_blockCipher) == 0x20);

  // OFFSET: 0x00654BF0
  // FUNCTION: fn_05033
  template<>
  bool BlockCipherFinal<true, ThreeWay::Dec>::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00654C30
  // FUNCTION: fn_05037
  template<>
  bool BlockCipherFinal<false, ThreeWay::Enc>::IsForwardTransformation() const {
    return true;
  }

  using ThreeWayCbcDecryption = CipherModeFinal<ThreeWay, true, BlockCipherFinal<true, ThreeWay::Dec>, CBC_Decryption>;

  // OFFSET: 0x00654CA0
  // FUNCTION: fn_05044
  template<>
  bool ThreeWayCbcDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646C80
  // FUNCTION: fn_04889
  template<>
  __declspec(noinline) ThreeWayCbcDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // OFFSET: 0x00649D80
  // FUNCTION: fn_04901
  template<>
  __declspec(noinline) ThreeWayCbcDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(ThreeWayCbcDecryption, m_blockCipher) == 0x2C);

  using ThreeWayOfb = CipherModeFinal<ThreeWay, true, BlockCipherFinal<false, ThreeWay::Enc>, OFB_Base>;

  // OFFSET: 0x00654CB0
  // FUNCTION: fn_05045
  template<>
  bool ThreeWayOfb::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00646D90
  // FUNCTION: fn_04890
  template<>
  __declspec(noinline) ThreeWayOfb::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline ThreeWayOfb::~CipherModeFinal() {
  }

  static_assert(offsetof(ThreeWayOfb, m_blockCipher) == 0x20);

  // OFFSET: 0x00654C00
  // FUNCTION: fn_05034
  template<>
  bool BlockCipherFinal<false, IDEA::Base>::IsForwardTransformation() const {
    return true;
  }

  using IdeaCbcEncryption = CipherModeFinal<IDEA, false, BlockCipherFinal<false, IDEA::Base>, CBC_Encryption>;

  // OFFSET: 0x00654D00
  // FUNCTION: fn_05050
  template<>
  bool IdeaCbcEncryption::IsForwardTransformation() const {
    return true;
  }

  // OFFSET: 0x006470D0
  // FUNCTION: fn_04895
  template<>
  __declspec(noinline) IdeaCbcEncryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline IdeaCbcEncryption::~CipherModeFinal() {
    SecureWipe(this->m_blockCipher.m_key, sizeof(this->m_blockCipher.m_key));
  }

  static_assert(offsetof(IdeaCbcEncryption, m_blockCipher) == 0x20);

  // OFFSET: 0x00659AF0
  // FUNCTION: fn_45528
  template<>
  bool BlockCipherFinal<true, IDEA::Base>::IsForwardTransformation() const {
    return false;
  }

  using IdeaCbcDecryption = CipherModeFinal<IDEA, true, BlockCipherFinal<true, IDEA::Base>, CBC_Decryption>;

  // OFFSET: 0x00659B50
  // FUNCTION: fn_45534
  template<>
  bool IdeaCbcDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00656AC0
  // FUNCTION: fn_05077
  template<>
  __declspec(noinline) IdeaCbcDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline IdeaCbcDecryption::~CipherModeFinal() {
    SecureWipe(this->m_blockCipher.m_key, sizeof(this->m_blockCipher.m_key));
  }

  static_assert(offsetof(IdeaCbcDecryption, m_blockCipher) == 0x2C);

  using ThreeWayCfbDecryption = CipherModeFinal<ThreeWay, true, BlockCipherFinal<false, ThreeWay::Enc>, CFB_Decryption>;

  // OFFSET: 0x00659B40
  // FUNCTION: fn_45533
  template<>
  bool ThreeWayCfbDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00656A20
  // FUNCTION: fn_05076
  template<>
  __declspec(noinline) ThreeWayCfbDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline ThreeWayCfbDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(ThreeWayCfbDecryption, m_blockCipher) == 0x2C);

  // OFFSET: 0x00659B00
  // FUNCTION: fn_45529
  template<>
  bool BlockCipherFinal<true, GOST::Dec>::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00659B10
  // FUNCTION: fn_45530
  template<>
  bool BlockCipherFinal<false, GOST::Enc>::IsForwardTransformation() const {
    return true;
  }

  using GostCbcDecryption = CipherModeFinal<GOST, true, BlockCipherFinal<true, GOST::Dec>, CBC_Decryption>;

  // OFFSET: 0x00659B20
  // FUNCTION: fn_45531
  template<>
  bool GostCbcDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x006568E0
  // FUNCTION: fn_05074
  template<>
  __declspec(noinline) GostCbcDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline GostCbcDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(GostCbcDecryption, m_blockCipher) == 0x2C);

  using GostCfbDecryption = CipherModeFinal<GOST, true, BlockCipherFinal<false, GOST::Enc>, CFB_Decryption>;

  // OFFSET: 0x00659B30
  // FUNCTION: fn_45532
  template<>
  bool GostCfbDecryption::IsForwardTransformation() const {
    return false;
  }

  // OFFSET: 0x00656980
  // FUNCTION: fn_05075
  template<>
  __declspec(noinline) GostCfbDecryption::CipherModeFinal() {
    this->m_cipher = &this->m_blockCipher;
    this->ResizeBuffers();
  }

  // INLINED
  template<>
  __forceinline GostCfbDecryption::~CipherModeFinal() {
  }

  static_assert(offsetof(GostCfbDecryption, m_blockCipher) == 0x2C);

}

using PakBlockTransform = void (*)(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);

// INLINED
__forceinline void EncryptBlockWithDataHash(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::uint32_t ivOffset,
                                            std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  const auto iv = keyData.subspan(Deobfuscate<0x53F9360C, 0x4CC36824>(ivOffset), Deobfuscate<0x53F9360C, 0x4CC36824>(8));
  XXH128_hash_t key = XXH3_128bits(data.data(), data.size());
  GPakEx::IdeaCbcEncryption cipher;
  cipher.SetKeyWithIV(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(&key), sizeof(key)), iv);
  cipher.Process(input, output, false);
  SecureWipe(&key, sizeof(key));
}

// INLINED
__forceinline void EncryptDataHash(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::uint32_t ivOffset,
                                   std::span<const std::uint8_t, 16> key, std::span<std::uint8_t, 16> output) {
  const auto iv = keyData.subspan(Deobfuscate<0x53F9360C, 0x4CC36824>(ivOffset), Deobfuscate<0x53F9360C, 0x4CC36824>(8));
  XXH128_hash_t hash = XXH3_128bits(data.data(), data.size());
  GPakEx::IdeaCbcEncryption cipher;
  cipher.SetKeyWithIV(key, iv);
  cipher.Process(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(&hash), sizeof(hash)), output, false);
  SecureWipe(&hash, sizeof(hash));
}

// OFFSET: 0x006442D0
// FUNCTION: fn_04843
void DataHashBlockTransform30(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptBlockWithDataHash(data, keyData, 0x30, input, output);
}

// OFFSET: 0x00644480
// FUNCTION: fn_04844
void DataHashBlockTransform31(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptBlockWithDataHash(data, keyData, 0x31, input, output);
}

// OFFSET: 0x00644630
// FUNCTION: fn_04845
void DataHashBlockTransform32(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptBlockWithDataHash(data, keyData, 0x32, input, output);
}

// OFFSET: 0x006447E0
// FUNCTION: fn_04846
void DataHashBlockTransform33(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptBlockWithDataHash(data, keyData, 0x33, input, output);
}

// OFFSET: 0x00644990
// FUNCTION: fn_04847
void DataHashBlockTransform34(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptBlockWithDataHash(data, keyData, 0x34, input, output);
}

// OFFSET: 0x00644B40
// FUNCTION: fn_04848
void DataHashKeyTransform30(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptDataHash(data, keyData, 0x30, input, output);
}

// OFFSET: 0x00644CE0
// FUNCTION: fn_04849
void DataHashKeyTransform31(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptDataHash(data, keyData, 0x31, input, output);
}

// OFFSET: 0x00644E80
// FUNCTION: fn_04850
void DataHashKeyTransform32(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptDataHash(data, keyData, 0x32, input, output);
}

// OFFSET: 0x00645020
// FUNCTION: fn_04851
void DataHashKeyTransform34(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> input, std::span<std::uint8_t, 16> output) {
  EncryptDataHash(data, keyData, 0x34, input, output);
}

struct PakDecryptor {

  bool Decrypt005(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt006(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt007(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt008(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt009(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt00A(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt201(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt080(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt00B(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt00C(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt00D(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt00E(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt00F(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt100(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt101(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt102(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt103(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt104(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt105(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt106(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt107(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt108(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt109(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt10A(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt10B(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt10C(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt10D(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt10E(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt10F(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt110(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt111(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt112(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt113(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt202(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt203(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt081(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
  bool Decrypt082(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const;
};

// INLINED
__forceinline bool DecryptWithAes(PakBlockTransform transform, std::uint32_t keyOffset, std::uint32_t ivOffset, AES::Mode mode, std::size_t prefixSize,
                                  std::size_t chunkSize, std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData,
                                  std::span<const std::uint8_t, 16> block, const std::span<const std::uint8_t>& input, const std::span<std::uint8_t>& output) {
  if (input.empty()) {
    return true;
  }

  std::uint8_t mask[16]{};
  transform(data, keyData, block, mask);

  const auto key = keyData.subspan(Deobfuscate<0x508EF2F3, 0xBFDC0C82>(keyOffset), Deobfuscate<0x508EF2F3, 0xBFDC0C82>(16));
  const auto iv = keyData.subspan(Deobfuscate<0xA101B199, 0x10F4BC62>(ivOffset), Deobfuscate<0xA101B199, 0x10F4BC62>(16));
  AES cipher;
  bool result = false;
  if (cipher.Initialize(const_cast<PUCHAR>(key.data()), key.size(), std::span<const UCHAR>(iv.data(), iv.size()), mode)) {
    if (input.size() <= prefixSize) {
      XorWithKey16(mask, input, output);
      result = true;
    } else {
      XorWithKey16(mask, input.first(prefixSize), output.first(prefixSize));
      const std::size_t encryptedSize = chunkSize + 16;
      if (input.size() <= prefixSize + encryptedSize) {
        const std::span<std::uint8_t> remaining = output.subspan(prefixSize);
        if (cipher.Decrypt(const_cast<PUCHAR>(input.data()) + prefixSize, input.size() - prefixSize, remaining.data(), remaining.size(), 1) == remaining.size()) {
          result = true;
        }
      } else if (cipher.Decrypt(const_cast<PUCHAR>(input.data()) + prefixSize, encryptedSize, output.data() + prefixSize, chunkSize, 1) == chunkSize) {
        XorWithKey16(mask, input.subspan(prefixSize + encryptedSize), output.subspan(prefixSize + chunkSize));
        result = true;
      }
    }
  }

  return result;
}

// OFFSET: 0x0064BCE0
// FUNCTION: fn_04946
bool PakDecryptor::Decrypt005(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform30, 0, 0x10, AES::CBC, 0, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064BF20
// FUNCTION: fn_04947
bool PakDecryptor::Decrypt006(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform31, 0x10, 0x20, AES::CFB, 0, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064C160
// FUNCTION: fn_04948
bool PakDecryptor::Decrypt007(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform34, 0x2C, 0x1C, AES::CBC, 0, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064C3A0
// FUNCTION: fn_04949
bool PakDecryptor::Decrypt008(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform32, 0, 0x10, AES::CBC, 0x400, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064C620
// FUNCTION: fn_04950
bool PakDecryptor::Decrypt009(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashKeyTransform30, 0x10, 0x20, AES::CBC, 0x400, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064C8A0
// FUNCTION: fn_04951
bool PakDecryptor::Decrypt00A(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform34, 0x2C, 0x1C, AES::CFB, 0x400, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064CB20
// FUNCTION: fn_04952
bool PakDecryptor::Decrypt201(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform33, 0x10, 0x20, AES::CFB, 0, 0x800, data, keyData, block, input, output);
}

// OFFSET: 0x0064CD60
// FUNCTION: fn_04953
bool PakDecryptor::Decrypt080(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashKeyTransform32, 0x14, 0x24, AES::CFB, 0x400, 0x800, data, keyData, block, input, output);
}

// OFFSET: 0x0064CFE0
// FUNCTION: fn_04954
bool PakDecryptor::Decrypt00B(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform30, 0x2C, 0x18, AES::CFB, 0x400, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064D260
// FUNCTION: fn_04955
bool PakDecryptor::Decrypt00C(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashKeyTransform31, 0x2C, 0x10, AES::CBC, 0x800, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064D4E0
// FUNCTION: fn_04956
bool PakDecryptor::Decrypt00D(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashKeyTransform30, 0x10, 0x20, AES::CFB, 0x800, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064D760
// FUNCTION: fn_04957
bool PakDecryptor::Decrypt00E(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashKeyTransform34, 0x2C, 0, AES::CFB, 0x400, 0x400, data, keyData, block, input, output);
}

// OFFSET: 0x0064D9E0
// FUNCTION: fn_04958
bool PakDecryptor::Decrypt00F(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> keyData, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithAes(DataHashBlockTransform31, 0x10, 0, AES::CFB, 0x400, 0x400, data, keyData, block, input, output);
}

// INLINED
template<class CipherMode>
__forceinline bool DecryptWithCipherMode(PakBlockTransform transform, std::uint32_t keyOffset, std::uint32_t keySize, std::uint32_t ivOffset, std::uint32_t ivSize,
                                         std::size_t prefixSize, std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed,
                                         std::span<const std::uint8_t, 16> block, const std::span<const std::uint8_t>& input, std::span<std::uint8_t> output) {
  if (input.empty()) {
    return true;
  }

  std::uint8_t keyData[60];
  utility::FillRandomBytes(keyData, static_cast<unsigned int>(XXH3_64bits(seed.data(), seed.size())));
  std::uint8_t mask[16]{};
  transform(data, keyData, block, mask);

  const std::span<const std::uint8_t, 60> keys = keyData;
  const auto key = keys.subspan(Deobfuscate<0xC0C9A189, 0x9DD8961C>(keyOffset), Deobfuscate<0xC0C9A189, 0x9DD8961C>(keySize));
  const auto iv = keys.subspan(Deobfuscate<0x3D1F34CF, 0x7D456D99>(ivOffset), Deobfuscate<0x3D1F34CF, 0x7D456D99>(ivSize));
  CipherMode cipher;
  cipher.SetKeyWithIV(key, iv);
  const std::size_t encryptedSize = cipher.CiphertextLength(0x400, true);

  bool succeeded = false;
  if (input.size() <= prefixSize) {
    XorWithKey16(mask, input, output);
    succeeded = true;
  } else {
    XorWithKey16(mask, input.first(prefixSize), output.first(prefixSize));
    if (input.size() <= prefixSize + encryptedSize) {
      const std::span<std::uint8_t> remaining = output.subspan(prefixSize);
      succeeded = cipher.Process(input.subspan(prefixSize), remaining, true) == remaining.size();
    } else if (cipher.Process(input.subspan(prefixSize, encryptedSize), output.subspan(prefixSize, 0x400), true) == 0x400) {
      XorWithKey16(mask, input.subspan(prefixSize + encryptedSize), output.subspan(prefixSize + 0x400));
      succeeded = true;
    }
  }
  return succeeded;
}

// OFFSET: 0x0064F000
// FUNCTION: fn_04959
bool PakDecryptor::Decrypt100(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::TeaCfbDecryption>(DataHashKeyTransform31, 0x1C, 0x10, 0x2C, 8, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x0064F2F0
// FUNCTION: fn_04960
bool PakDecryptor::Decrypt101(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc5Ofb>(DataHashKeyTransform31, 0x20, 0x10, 0x10, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x0064F560
// FUNCTION: fn_04961
bool PakDecryptor::Decrypt102(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc5CfbDecryption>(DataHashKeyTransform32, 0x2C, 0x10, 0x1C, 8, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x0064F7F0
// FUNCTION: fn_04962
bool PakDecryptor::Decrypt103(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::TeaCfbDecryption>(DataHashKeyTransform30, 0, 0x10, 0x20, 8, 0x800, data, seed, block, input, output);
}

// OFFSET: 0x0064FAE0
// FUNCTION: fn_04963
bool PakDecryptor::Decrypt104(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::XteaOfb>(DataHashBlockTransform32, 0x10, 0x10, 0x20, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x0064FDC0
// FUNCTION: fn_04964
bool PakDecryptor::Decrypt105(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::TeaOfb>(DataHashBlockTransform33, 0x10, 0x10, 0x20, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x006500A0
// FUNCTION: fn_04965
bool PakDecryptor::Decrypt106(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::TeaCfbDecryption>(DataHashBlockTransform32, 0, 0x10, 0x20, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x00650350
// FUNCTION: fn_04966
bool PakDecryptor::Decrypt107(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc5CfbDecryption>(DataHashBlockTransform30, 0x2C, 0x10, 0x10, 8, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x006505E0
// FUNCTION: fn_04967
bool PakDecryptor::Decrypt108(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc5CfbDecryption>(DataHashKeyTransform32, 0x2C, 0x10, 0, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x00650840
// FUNCTION: fn_04968
bool PakDecryptor::Decrypt109(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc5Ofb>(DataHashBlockTransform32, 0x18, 0x10, 0x2C, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x00650AB0
// FUNCTION: fn_04969
bool PakDecryptor::Decrypt10A(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::TeaOfb>(DataHashBlockTransform31, 0x18, 0x10, 0x20, 8, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x00650DD0
// FUNCTION: fn_04970
bool PakDecryptor::Decrypt10B(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::TeaCfbDecryption>(DataHashKeyTransform34, 0x10, 0x10, 0x20, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x00651080
// FUNCTION: fn_04971
bool PakDecryptor::Decrypt10C(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc6CfbDecryption>(DataHashKeyTransform32, 0x1C, 0x10, 0x2C, 0x10, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x00651310
// FUNCTION: fn_04972
bool PakDecryptor::Decrypt10D(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc5Ofb>(DataHashBlockTransform32, 0, 0x10, 0x10, 8, 0x800, data, seed, block, input, output);
}

// OFFSET: 0x006515C0
// FUNCTION: fn_04973
bool PakDecryptor::Decrypt10E(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::XteaOfb>(DataHashBlockTransform31, 0x2C, 0x10, 0x18, 8, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x006518E0
// FUNCTION: fn_04974
bool PakDecryptor::Decrypt10F(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::XteaCfbDecryption>(DataHashKeyTransform30, 0x2C, 0x10, 0x10, 8, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x00651BD0
// FUNCTION: fn_04975
bool PakDecryptor::Decrypt110(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc6CfbDecryption>(DataHashBlockTransform30, 0, 0x10, 0x20, 0x10, 0, data, seed, block, input, output);
}

// OFFSET: 0x00651E30
// FUNCTION: fn_04976
bool PakDecryptor::Decrypt111(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::XteaCfbDecryption>(DataHashKeyTransform32, 0x2C, 0x10, 0, 8, 0, data, seed, block, input, output);
}

// OFFSET: 0x006520E0
// FUNCTION: fn_04977
bool PakDecryptor::Decrypt112(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc6Ofb>(DataHashBlockTransform32, 0x20, 0x10, 0x10, 0x10, 0, data, seed, block, input, output);
}

// OFFSET: 0x00652350
// FUNCTION: fn_04978
bool PakDecryptor::Decrypt113(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::Rc6Ofb>(DataHashKeyTransform32, 0x18, 0x10, 0x20, 0x10, 0, data, seed, block, input, output);
}

// OFFSET: 0x006525C0
// FUNCTION: fn_04979
bool PakDecryptor::Decrypt202(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::ThreeWayCbcDecryption>(DataHashKeyTransform30, 0x10, 0xC, 0x2C, 0xC, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x006528B0
// FUNCTION: fn_04980
bool PakDecryptor::Decrypt203(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::ThreeWayOfb>(DataHashBlockTransform31, 0x18, 0xC, 0x2C, 0xC, 0x400, data, seed, block, input, output);
}

// OFFSET: 0x00652BD0
// FUNCTION: fn_04981
bool PakDecryptor::Decrypt081(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::ThreeWayOfb>(DataHashBlockTransform32, 0x20, 0xC, 0x10, 0xC, 0, data, seed, block, input, output);
}

// OFFSET: 0x00652EB0
// FUNCTION: fn_04982
bool PakDecryptor::Decrypt082(std::span<const std::uint8_t> data, std::span<const std::uint8_t, 60> seed, std::span<const std::uint8_t, 16> block,
                                std::span<const std::uint8_t> input, std::span<std::uint8_t> output) const {
  return DecryptWithCipherMode<GPakEx::ThreeWayCbcDecryption>(DataHashKeyTransform32, 0, 0xC, 0x2C, 0xC, 0, data, seed, block, input, output);
}
