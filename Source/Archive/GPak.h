#ifndef RAGNAROK_SOURCE_ARCHIVE_GPAK_H
#define RAGNAROK_SOURCE_ARCHIVE_GPAK_H

#include "Archive/PakFile.h"
#include "Archive/PakName.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <span>
#include <string_view>
#include <random>
#include <unordered_map>
#include <vector>

class AES;

namespace GPakEx {
  class CipherModeBase;
  class CrypterBase;
}

using PakCrypterMap = std::unordered_map<std::uint16_t, GPakEx::CrypterBase*>;
using PakDecryptFunction = std::function<
  bool(std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>)>;
using PakDecryptFunctionMap = std::unordered_map<std::uint16_t, PakDecryptFunction>;

struct PakUniformRandom {
  /* 0000 */ std::mt19937 m_engine;
  /* 1388 */ std::uniform_int_distribution<int> m_distribution;

  // INLINED
  explicit PakUniformRandom(unsigned int seed) : m_engine(seed), m_distribution(1, 254) {
  }

  PakUniformRandom(int minimum, int maximum, unsigned int seed);
};

void FillRandomBytes(char* first, char* last, PakUniformRandom& random);

namespace myGPack {

  struct PakEntry {
    /* 0000 */ std::uint32_t alignedSize;
    /* 0004 */ std::uint32_t compressedSize;
    /* 0008 */ std::uint32_t originalSize;
    /* 000C */ std::uint32_t padding_000C;
    /* 0010 */ std::uint64_t dataOffset;
    /* 0018 */ std::uint8_t flags;
    /* 0019 */ std::uint8_t padding_0019[3];
    /* 001C */ PakName name;
  };

  static_assert(sizeof(PakEntry) == 288);
  static_assert(offsetof(PakEntry, dataOffset) == 16);
  static_assert(offsetof(PakEntry, flags) == 24);
  static_assert(offsetof(PakEntry, name) == 28);

}

template <class T>
struct PakObfuscated {
  /* 0000 */ std::uint8_t m_mask[5];
  /* 0005 */ std::uint8_t m_data[sizeof(T)];

  PakObfuscated() = default;

  // INLINED
  explicit PakObfuscated(const T& value) : m_mask{}, m_data{} {
    *this = value;
  }

  // OFFSET: 0x00657840
  // FUNCTION: fn_46759
  PakObfuscated& operator=(const T& value) {
    PakUniformRandom random(1, 0xfe, std::random_device{}());
    FillRandomBytes(reinterpret_cast<char*>(this->m_mask), reinterpret_cast<char*>(this->m_mask) + sizeof(this->m_mask), random);

    const std::uint8_t* bytes = reinterpret_cast<const std::uint8_t*>(&value);
    for (std::size_t i = 0; i < 48; ++i) {
      this->m_data[i] = bytes[i] ^ this->m_mask[i % 4 + 1] ^ this->m_mask[i % 4];
    }

    return *this;
  }

  // INLINED
  T Unmask() const {
    std::uint8_t bytes[sizeof(T)];
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      bytes[i] = this->m_data[i] ^ this->m_mask[i % 4] ^ this->m_mask[i % 4 + 1];
    }

    return std::bit_cast<T>(bytes);
  }
};

struct PakKeyMaterial {
  /* 0000 */ std::uint8_t key[16];
  /* 0010 */ std::uint8_t iv[16];
  /* 0020 */ std::uint8_t reserved[16];
};

static_assert(sizeof(PakObfuscated<PakKeyMaterial>) == 53);

struct PakArchiveKeyRecord {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint8_t material[60];
};

static_assert(sizeof(PakObfuscated<PakArchiveKeyRecord>) == 69);

using PakArchiveKeyMap = std::unordered_map<std::uint16_t, PakObfuscated<PakArchiveKeyRecord>>;

struct PakAuxiliaryHeader;

struct PakAuxiliary {
  PakAuxiliary();
  ~PakAuxiliary();

  bool Apply(std::span<const std::uint8_t> data, std::span<const std::uint8_t> auxiliary);
  bool Extract(std::string_view name, const std::uint8_t* data, std::uint32_t size, std::span<const std::uint8_t> scratch, void* output, std::uint32_t outputSize, bool verify);
  bool DecodeHeader(std::span<const std::uint8_t> name, std::uint8_t* data, std::uint32_t size, PakAuxiliaryHeader* header);
  void Initialize();

  /* 0000 */ PakArchiveKeyMap m_archiveKeys;
  /* 0020 */ PakObfuscated<PakKeyMaterial> m_headerKey;
  /* 0055 */ PakObfuscated<PakKeyMaterial> m_archiveKeyBlockKey;
  /* 008A */ PakObfuscated<PakKeyMaterial> m_streamKey;
  /* 00C0 */ std::shared_ptr<AES> m_headerCipher;
  /* 00C8 */ std::shared_ptr<AES> m_archiveKeyCipher;
  /* 00D0 */ std::shared_ptr<GPakEx::CipherModeBase> m_ideaDecryption;
  /* 00D8 */ std::shared_ptr<GPakEx::CipherModeBase> m_gostDecryption;
  /* 00E0 */ std::shared_ptr<GPakEx::CipherModeBase> m_gostStreamDecryption;
  /* 00E8 */ std::shared_ptr<GPakEx::CipherModeBase> m_threeWayStreamDecryption;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(PakAuxiliary, m_headerKey) == 0x20);
static_assert(offsetof(PakAuxiliary, m_headerCipher) == 0xC0);
static_assert(sizeof(PakAuxiliary) == 240);
#endif

class CGPak {
public:
  CGPak();
  virtual ~CGPak();

  bool Open(CPakFile* file);
  __declspec(noinline) void Reset();
  bool Find(const myGPack::PakName& name, myGPack::PakEntry* entry) const;
  bool Extract(const myGPack::PakEntry* entry, void* buffer);
  void ReadAuxiliary(std::vector<std::uint8_t>& output, std::uint64_t* identifier = nullptr);
  bool ApplyAuxiliary(std::span<const std::uint8_t> data);
  void ApplyAuxiliary(const void* data, std::uint32_t size, const void* auxData, std::uint32_t auxSize);

protected:
  void Clear();
  bool ReadTableV1();
  bool ReadTableV2();
  bool ReadTableV3();

  /* 0008 */ CPakFile* m_file;
  /* 000C */ std::uint32_t m_version;
  /* 0010 */ std::uint32_t m_entryCount;
  /* 0014 */ std::uint32_t padding_0014;
  /* 0018 */ std::uint64_t m_tableOffset;
  /* 0020 */ std::uint32_t m_tableSize;
  /* 0024 */ std::vector<myGPack::PakEntry> m_entries;
  /* 0030 */ std::vector<std::uint8_t> m_scratch;
  /* 003C */ CRITICAL_SECTION m_criticalSection;
  /* 0054 */ PakAuxiliary m_auxiliary;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGPak) == 328);
#endif

// INLINED
inline CGPak::CGPak() : m_file(nullptr), m_version(0), m_entryCount(0), m_tableOffset(0), m_tableSize(0) {
  this->Reset();
  InitializeCriticalSection(&this->m_criticalSection);
}

#endif
