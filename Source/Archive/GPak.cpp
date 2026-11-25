#include "Archive/GPak.h"
#include "Archive/GPakEx.h"
#include "Archive/PakCrypter.h"
#include "Archive/GPakUpdate.h"
#include "Archive/PakAuxiliary.h"
#include "Archive/PakCipher.h"
#include "Security/ThemidaMgr.h"
#include "Utility/AES.h"
#include "Utility/Cipher/AES.h"
#include "Utility/Hash/SHA3.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Compression.h"
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
#include <intrin.h>
#include <random>
#include <span>
#include <unordered_map>
#include <vector>
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


namespace {

  // INLINED
  inline std::uint32_t GetWordBigEndian(const std::uint8_t* bytes) {
    return (static_cast<std::uint32_t>(bytes[0]) << 24) | (static_cast<std::uint32_t>(bytes[1]) << 16) | (static_cast<std::uint32_t>(bytes[2]) << 8) | bytes[3];
  }

  // INLINED
  inline std::uint32_t ByteSwapWord(std::uint32_t value) {
    return _byteswap_ulong(value);
  }

  // INLINED
  inline std::uint16_t ByteSwapWord(std::uint16_t value) {
    return _byteswap_ushort(value);
  }

  template <class T, bool BigEndian> class BlockGetter {
  public:
    // INLINED
    explicit BlockGetter(const std::uint8_t* block) : m_block(block) {}

    // INLINED
    template <class U> BlockGetter& operator()(U& value) {
      T word = 0;
      if (this->m_block) {
        word = *reinterpret_cast<const T*>(this->m_block);
      }
      if (BigEndian) {
        word = ByteSwapWord(word);
      }
      value = word;
      this->m_block += sizeof(T);
      return *this;
    }

    const std::uint8_t* m_block;
  };

  template <class T, bool BigEndian> class BlockPutter {
  public:
    // INLINED
    BlockPutter(const std::uint8_t* xorBlock, std::uint8_t* outBlock) : m_xorBlock(xorBlock), m_outBlock(outBlock) {}

    // INLINED
    template <class U> BlockPutter& operator()(U value) {
      T word = static_cast<T>(value);
      if (BigEndian) {
        word = ByteSwapWord(word);
      }
      if (this->m_xorBlock) {
        word ^= *reinterpret_cast<const T*>(this->m_xorBlock);
      }
      if (this->m_outBlock) {
        *reinterpret_cast<T*>(this->m_outBlock) = word;
      }
      this->m_xorBlock = GPakEx::PtrAdd(this->m_xorBlock, sizeof(T));
      this->m_outBlock += sizeof(T);
      return *this;
    }

    const std::uint8_t* m_xorBlock;
    std::uint8_t* m_outBlock;
  };

  // INLINED
  inline std::uint32_t GostF(std::uint32_t value) {
    using GPakEx::GOST;
    return GOST::Base::s_sTable[3][value >> 24] ^ GOST::Base::s_sTable[2][(value >> 16) & 0xFF] ^ GOST::Base::s_sTable[1][(value >> 8) & 0xFF] ^ GOST::Base::s_sTable[0][value & 0xFF];
  }

  // INLINED
  inline void IdeaMul(std::uint32_t& a, std::uint32_t b) {
    std::uint32_t product = (a & 0xFFFF) * b;
    if (product) {
      product = (product & 0xFFFF) - (product >> 16);
      a = product - (product >> 16);
    } else {
      a = 1 - a - b;
    }
  }

  // INLINED
  inline std::uint32_t IdeaAddInv(std::uint32_t x) {
    return (0 - x) & 0xFFFF;
  }

  // INLINED
  __forceinline void ThreeWayTheta(std::uint32_t& a0, std::uint32_t& a1, std::uint32_t& a2) {
    std::uint32_t c = a0 ^ a1 ^ a2;
    c = _rotl(c, 16) ^ _rotl(c, 8);
    const std::uint32_t b0 = (a0 << 24) ^ (a2 >> 8) ^ (a1 << 8) ^ (a0 >> 24);
    const std::uint32_t b1 = (a1 << 24) ^ (a0 >> 8) ^ (a2 << 8) ^ (a1 >> 24);
    a0 ^= c ^ b0;
    a1 ^= c ^ b1;
    a2 ^= c ^ (b0 >> 16) ^ (b1 << 16);
  }

  // INLINED
  __forceinline void ThreeWayPiGammaPi(std::uint32_t& a0, std::uint32_t& a1, std::uint32_t& a2) {
    const std::uint32_t b2 = _rotl(a2, 1);
    const std::uint32_t b0 = _rotl(a0, 22);
    a0 = _rotl(b0 ^ (a1 | ~b2), 1);
    a2 = _rotl(b2 ^ (b0 | ~a1), 22);
    a1 ^= b2 | ~b0;
  }

  // INLINED
  __forceinline std::uint32_t ThreeWayReverseBits(std::uint32_t a) {
    a = ((a & 0xAAAAAAAA) >> 1) | ((a & 0x55555555) << 1);
    a = ((a & 0xCCCCCCCC) >> 2) | ((a & 0x33333333) << 2);
    return ((a & 0xF0F0F0F0) >> 4) | ((a & 0x0F0F0F0F) << 4);
  }

  // INLINED
  __forceinline void ThreeWayMu(std::uint32_t& a0, std::uint32_t& a1, std::uint32_t& a2) {
    a1 = ThreeWayReverseBits(a1);
    const std::uint32_t t = ThreeWayReverseBits(a0);
    a0 = ThreeWayReverseBits(a2);
    a2 = t;
  }

}

unsigned int IdeaMulInv(unsigned int x);

// GLOBAL: gl_25481
const std::uint8_t GPakEx::GOST::Base::s_sbox[8][16] = {
  {11, 4, 12, 7, 12, 0, 9, 13, 3, 14, 11, 11, 9, 8, 0, 4},
  {12, 1, 4, 5, 14, 3, 4, 12, 7, 4, 10, 14, 7, 15, 14, 0},
  {14, 11, 2, 10, 2, 11, 11, 7, 5, 10, 0, 15, 7, 0, 9, 0},
  {4, 8, 12, 7, 9, 3, 6, 4, 12, 14, 10, 15, 13, 0, 8, 1},
  {6, 15, 0, 4, 0, 11, 0, 3, 10, 14, 6, 12, 7, 3, 5, 3},
  {3, 4, 15, 13, 3, 3, 13, 4, 1, 6, 15, 10, 13, 6, 12, 15},
  {2, 10, 4, 12, 1, 0, 14, 0, 9, 12, 11, 11, 9, 5, 3, 9},
  {14, 9, 8, 7, 0, 15, 3, 7, 4, 7, 15, 13, 11, 10, 6, 2},
};

// GLOBAL: gl_25483
bool GPakEx::GOST::Base::s_sTableCalculated;

// GLOBAL: gl_25484
unsigned int GPakEx::GOST::Base::s_sTable[4][256];

// OFFSET: 0x006429B0
// FUNCTION: fn_04822
GPakEx::SecureByteBuffer::~SecureByteBuffer() {
  SecureZeroMemory(this->m_bytes.data(), this->m_bytes.size());
}

// OFFSET: 0x00642A60
// FUNCTION: fn_04823
GPakEx::BlockCipher::~BlockCipher() {
}

// OFFSET: 0x00642A70
// FUNCTION: fn_04824
GPakEx::CipherModeBase::~CipherModeBase() {
}

// OFFSET: 0x00642B80
// FUNCTION: fn_04827
unsigned int GPakEx::BlockCipher::AdvancedProcessBlocks(const std::uint8_t* inBlocks, const std::uint8_t* xorBlocks, std::uint8_t* outBlocks, unsigned int length, unsigned int flags) const {
  const unsigned int blockSize = this->BlockSize();
  std::ptrdiff_t increment = blockSize;
  if (flags & ReverseDirection) {
    if (!(flags & InBlockIsCounter)) {
      inBlocks = PtrAdd(inBlocks, length - blockSize);
    }
    xorBlocks = PtrAdd(xorBlocks, length - blockSize);
    outBlocks = PtrAdd(outBlocks, length - blockSize);
    increment = -increment;
  }
  const bool xorInput = xorBlocks && (flags & XorInput);
  while (length >= blockSize) {
    if (xorInput) {
      XorBuffers(outBlocks, xorBlocks, inBlocks, blockSize);
      this->ProcessAndXorBlock(outBlocks, nullptr, outBlocks);
    } else {
      this->ProcessAndXorBlock(inBlocks, xorBlocks, outBlocks);
    }
    if (flags & InBlockIsCounter) {
      const_cast<std::uint8_t*>(inBlocks)[blockSize - 1]++;
    } else {
      inBlocks = PtrAdd(inBlocks, increment);
    }
    length -= blockSize;
    xorBlocks = PtrAdd(xorBlocks, increment);
    outBlocks = PtrAdd(outBlocks, increment);
  }
  return length;
}

// OFFSET: 0x00642C80
// FUNCTION: fn_04828
__declspec(noinline) unsigned int GPakEx::CipherModeBase::CiphertextLength(unsigned int length, bool pad) const {
  const unsigned int blockSize = this->m_cipher->BlockSize();
  if (length == 0) {
    return pad ? blockSize : 0;
  }
  if (pad) {
    return length + blockSize - length % blockSize;
  }
  const unsigned int rounded = length + blockSize - 1;
  return rounded - rounded % blockSize;
}

// OFFSET: 0x00642D60
// FUNCTION: fn_04829
void GPakEx::CBC_Decryption::ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) {
  const unsigned int blockSize = this->m_cipher->BlockSize();
  if (length == 0) {
    return;
  }
  memcpy(this->m_temp.data(), PtrAdd(input, length - blockSize), blockSize);
  if (length > blockSize) {
    this->m_cipher->AdvancedProcessBlocks(PtrAdd(input, blockSize), input, PtrAdd(output, blockSize), length - blockSize, BlockCipher::ReverseDirection);
  }
  this->m_cipher->ProcessAndXorBlock(input, this->m_register.data(), output);
  this->m_register.swap(this->m_temp);
}

// OFFSET: 0x00642E20
// FUNCTION: fn_04830
void GPakEx::CBC_Encryption::ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) {
  const unsigned int blockSize = this->m_cipher->BlockSize();
  if (length == 0) {
    return;
  }
  this->m_cipher->AdvancedProcessBlocks(input, this->m_register.data(), output, blockSize, BlockCipher::XorInput);
  if (length > blockSize) {
    this->m_cipher->AdvancedProcessBlocks(PtrAdd(input, blockSize), output, PtrAdd(output, blockSize), length - blockSize, BlockCipher::XorInput);
  }
  memcpy(this->m_register.data(), PtrAdd(output, length - blockSize), blockSize);
}

// OFFSET: 0x00642EB0
// FUNCTION: fn_04831
void GPakEx::CFB_Decryption::ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) {
  const unsigned int blockSize = this->m_cipher->BlockSize();
  if (length == 0) {
    return;
  }
  memcpy(this->m_temp.data(), PtrAdd(input, length - blockSize), blockSize);
  if (blockSize < length) {
    this->m_cipher->AdvancedProcessBlocks(input, PtrAdd(input, blockSize), PtrAdd(output, blockSize), length - blockSize, BlockCipher::ReverseDirection);
  }
  this->m_cipher->ProcessAndXorBlock(this->m_register.data(), input, output);
  this->m_register.swap(this->m_temp);
}

// OFFSET: 0x006430F0
// FUNCTION: fn_04832
void GPakEx::OFB_Base::ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) {
  const unsigned int blockSize = this->m_cipher->BlockSize();
  while (length > 0) {
    this->m_cipher->ProcessAndXorBlock(this->m_register.data(), nullptr, this->m_register.data());
    XorBuffers(output, input, this->m_register.data(), blockSize);
    length -= blockSize;
    input = PtrAdd(input, blockSize);
    output = PtrAdd(output, blockSize);
  }
}

// OFFSET: 0x00643160
// FUNCTION: fn_04833
unsigned int GPakEx::CipherModeBase::Process(std::span<const std::uint8_t> input, std::span<std::uint8_t> output, bool pad) {
  const bool forward = this->IsForwardTransformation();
  const unsigned int blockSize = this->m_cipher->BlockSize();
  const unsigned int reserved = (!forward && pad) ? blockSize : 0;
  unsigned int processed = 0;
  if (input.size() >= reserved + blockSize) {
    unsigned int length = input.size() - reserved;
    length -= length % blockSize;
    if (output.size() < length) {
      return static_cast<unsigned int>(-1);
    }
    this->ProcessData(input.data(), output.data(), length);
    input = input.subspan(length);
    output = output.subspan(length);
    processed = length;
  }
  if (pad) {
    if (forward) {
      memcpy(this->m_buffer.data(), input.data(), input.size());
      const std::uint8_t padding = static_cast<std::uint8_t>(blockSize - input.size());
      memset(PtrAdd(this->m_buffer.data(), input.size()), padding, padding);
      if (output.size() < blockSize) {
        return static_cast<unsigned int>(-1);
      }
      this->ProcessData(this->m_buffer.data(), output.data(), blockSize);
      processed += blockSize;
    } else {
      if (input.size() != blockSize) {
        return static_cast<unsigned int>(-1);
      }
      this->ProcessData(input.data(), this->m_buffer.data(), blockSize);
      const std::uint8_t padding = this->m_buffer[blockSize - 1];
      const unsigned int length = blockSize - padding;
      if (padding < 1 || blockSize < padding || output.size() < length) {
        return static_cast<unsigned int>(-1);
      }
      memcpy(output.data(), this->m_buffer.data(), length);
      processed += length;
    }
  } else if (!input.empty()) {
    return static_cast<unsigned int>(-1);
  }
  return processed;
}

// OFFSET: 0x006432C0
// FUNCTION: fn_04834
void GPakEx::CBC_Decryption::ResizeBuffers() {
  CipherModeBase::ResizeBuffers();
  this->m_temp.resize(this->m_cipher->BlockSize());
}

// OFFSET: 0x00643330
// FUNCTION: fn_04835
void GPakEx::CFB_Decryption::ResizeBuffers() {
  CipherModeBase::ResizeBuffers();
  this->m_temp.resize(this->m_cipher->BlockSize());
}

// OFFSET: 0x006433A0
// FUNCTION: fn_04836
void GPakEx::CipherModeBase::ResizeBuffers() {
  const unsigned int blockSize = this->m_cipher->BlockSize();
  this->m_register.resize(blockSize);
  this->m_buffer.resize(blockSize);
}

// OFFSET: 0x00643460
// FUNCTION: fn_46735
void GPakEx::CipherModeBase::SetKey(std::span<const std::uint8_t> key) {
  this->m_cipher->SetKey(key);
}

// OFFSET: 0x00643480
// FUNCTION: fn_04837
__declspec(noinline) void GPakEx::CipherModeBase::SetKeyWithIV(std::span<const std::uint8_t> key, std::span<const std::uint8_t> iv) {
  this->m_cipher->SetKey(key);
  memcpy(this->m_register.data(), iv.data(), this->m_register.size());
}

// OFFSET: 0x006434B0
// FUNCTION: fn_04838
void GPakEx::XorBuffers(std::uint8_t* output, const std::uint8_t* input, const std::uint8_t* mask, unsigned int length) {
  while (length >= 16) {
    _mm_storeu_si128(reinterpret_cast<__m128i*>(output), _mm_xor_si128(_mm_loadu_si128(reinterpret_cast<const __m128i*>(input)), _mm_loadu_si128(reinterpret_cast<const __m128i*>(mask))));
    output += 16;
    input += 16;
    mask += 16;
    length -= 16;
  }
  for (unsigned int i = 0; i < length; ++i) {
    output[i] = input[i] ^ mask[i];
  }
}

// OFFSET: 0x00643600
// FUNCTION: fn_46736
void GPakEx::XorBuffers(std::uint8_t* buffer, const std::uint8_t* mask, unsigned int length) {
  while (length >= 16) {
    _mm_storeu_si128(reinterpret_cast<__m128i*>(buffer), _mm_xor_si128(_mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer)), _mm_loadu_si128(reinterpret_cast<const __m128i*>(mask))));
    buffer += 16;
    mask += 16;
    length -= 16;
  }
  for (unsigned int i = 0; i < length; ++i) {
    buffer[i] ^= mask[i];
  }
}

// OFFSET: 0x00643710
// FUNCTION: fn_46737
void GPakEx::ByteReverse(std::uint32_t* output, const std::uint32_t* input, unsigned int byteCount) {
  const unsigned int count = byteCount / sizeof(std::uint32_t);
  for (unsigned int i = 0; i < count; ++i) {
    output[i] = _byteswap_ulong(input[i]);
  }
}

// OFFSET: 0x006437F0
// FUNCTION: fn_46738
void GPakEx::GOST::Base::PrecalculateSTable() {
  if (!s_sTableCalculated) {
    for (unsigned int i = 0; i < 4; ++i) {
      for (unsigned int j = 0; j < 256; ++j) {
        const unsigned int temp = s_sbox[2 * i][j % 16] | (s_sbox[2 * i + 1][j / 16] << 4);
        s_sTable[i][j] = _rotl(temp, 11 + 8 * i);
      }
    }
    s_sTableCalculated = true;
  }
}

// OFFSET: 0x00643860
// FUNCTION: fn_04839
// VIRTUALIZED: 2025-12-02 0x005F20C0
void GPakEx::GOST::Dec::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  std::uint32_t n1;
  std::uint32_t n2;
  BlockGetter<std::uint32_t, false>(inBlock)(n1)(n2);
  n2 ^= GostF(n1 + this->m_key[0]);
  n1 ^= GostF(n2 + this->m_key[1]);
  n2 ^= GostF(n1 + this->m_key[2]);
  n1 ^= GostF(n2 + this->m_key[3]);
  n2 ^= GostF(n1 + this->m_key[4]);
  n1 ^= GostF(n2 + this->m_key[5]);
  n2 ^= GostF(n1 + this->m_key[6]);
  n1 ^= GostF(n2 + this->m_key[7]);
  for (unsigned int i = 0; i < 3; ++i) {
    n2 ^= GostF(n1 + this->m_key[7]);
    n1 ^= GostF(n2 + this->m_key[6]);
    n2 ^= GostF(n1 + this->m_key[5]);
    n1 ^= GostF(n2 + this->m_key[4]);
    n2 ^= GostF(n1 + this->m_key[3]);
    n1 ^= GostF(n2 + this->m_key[2]);
    n2 ^= GostF(n1 + this->m_key[1]);
    n1 ^= GostF(n2 + this->m_key[0]);
  }
  BlockPutter<std::uint32_t, false>(xorBlock, outBlock)(n2)(n1);
}

// OFFSET: 0x00643CC0
// FUNCTION: fn_04840
void GPakEx::GOST::Enc::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  std::uint32_t n1;
  std::uint32_t n2;
  BlockGetter<std::uint32_t, false>(inBlock)(n1)(n2);
  for (unsigned int i = 0; i < 3; ++i) {
    n2 ^= GostF(n1 + this->m_key[0]);
    n1 ^= GostF(n2 + this->m_key[1]);
    n2 ^= GostF(n1 + this->m_key[2]);
    n1 ^= GostF(n2 + this->m_key[3]);
    n2 ^= GostF(n1 + this->m_key[4]);
    n1 ^= GostF(n2 + this->m_key[5]);
    n2 ^= GostF(n1 + this->m_key[6]);
    n1 ^= GostF(n2 + this->m_key[7]);
  }
  n2 ^= GostF(n1 + this->m_key[7]);
  n1 ^= GostF(n2 + this->m_key[6]);
  n2 ^= GostF(n1 + this->m_key[5]);
  n1 ^= GostF(n2 + this->m_key[4]);
  n2 ^= GostF(n1 + this->m_key[3]);
  n1 ^= GostF(n2 + this->m_key[2]);
  n2 ^= GostF(n1 + this->m_key[1]);
  n1 ^= GostF(n2 + this->m_key[0]);
  BlockPutter<std::uint32_t, false>(xorBlock, outBlock)(n2)(n1);
}

// OFFSET: 0x00644130
// FUNCTION: fn_04841
// VIRTUALIZED: 2025-12-02 0x005F2950
void GPakEx::GOST::Base::SetKey(std::span<const std::uint8_t> key) {
  PrecalculateSTable();
  memcpy_s(this->m_key.m_data, sizeof(this->m_key.m_data), key.data(), key.size());
}

// OFFSET: 0x006469E0
// FUNCTION: fn_04885
// OFFSET: 0x00646A90
// FUNCTION: fn_04886
// OFFSET: 0x00646B40
// FUNCTION: fn_04887
// OFFSET: 0x00646BE0
// FUNCTION: fn_04888
// OFFSET: 0x00646C80
// FUNCTION: fn_04889
// OFFSET: 0x00646D90
// FUNCTION: fn_04890
// OFFSET: 0x00646E30
// FUNCTION: fn_04891
// OFFSET: 0x00646EE0
// FUNCTION: fn_04892
// OFFSET: 0x00646F90
// FUNCTION: fn_04893
// OFFSET: 0x00647030
// FUNCTION: fn_04894
// OFFSET: 0x006470D0
// FUNCTION: fn_04895
// OFFSET: 0x006568E0
// FUNCTION: fn_05074
// OFFSET: 0x00656980
// FUNCTION: fn_05075
// OFFSET: 0x00656A20
// FUNCTION: fn_05076
// OFFSET: 0x00656AC0
// FUNCTION: fn_05077
template <class CIPHER, GPakEx::CipherDir DIR, class BLOCKCIPHER, class MODE> GPakEx::CipherModeFinal<CIPHER, DIR, BLOCKCIPHER, MODE>::CipherModeFinal() {
  this->m_cipher = &this->m_object;
  this->ResizeBuffers();
}

// OFFSET: 0x006471C0
// FUNCTION: fn_04896
GPakEx::CFB_Decryption::CFB_Decryption() {
}

// OFFSET: 0x00647270
// FUNCTION: fn_04897
GPakEx::OFB_Base::OFB_Base() {
}

// OFFSET: 0x00649B50
// FUNCTION: fn_04899
// OFFSET: 0x00649C50
// FUNCTION: fn_04900
// OFFSET: 0x00649D80
// FUNCTION: fn_04901
// OFFSET: 0x00649EB0
// FUNCTION: fn_04902
// OFFSET: 0x00649FB0
// FUNCTION: fn_04903
template <class CIPHER, GPakEx::CipherDir DIR, class BLOCKCIPHER, class MODE> GPakEx::CipherModeFinal<CIPHER, DIR, BLOCKCIPHER, MODE>::~CipherModeFinal() {
}

// OFFSET: 0x0064A730
// FUNCTION: fn_04908
GPakEx::RC6::Base::~Base() {
}

// OFFSET: 0x0064A880
// FUNCTION: fn_04909
GPakEx::RC5::Base::~Base() {
}

// OFFSET: 0x00654400
// FUNCTION: fn_04983
unsigned int GPakEx::RC6::Base::BlockSize() const {
  return 16;
}

// OFFSET: 0x00654410
// FUNCTION: fn_04984
unsigned int GPakEx::TEA::Base::BlockSize() const {
  return 8;
}

// OFFSET: 0x00654420
// FUNCTION: fn_04985
unsigned int GPakEx::ThreeWay::Base::BlockSize() const {
  return 12;
}

// OFFSET: 0x00654430
// FUNCTION: fn_04986
unsigned int GPakEx::RC5::Base::BlockSize() const {
  return 8;
}

// OFFSET: 0x00654440
// FUNCTION: fn_04987
unsigned int GPakEx::XTEA::Base::BlockSize() const {
  return 8;
}

// OFFSET: 0x00654450
// FUNCTION: fn_04988
unsigned int GPakEx::IDEA::Base::BlockSize() const {
  return 8;
}

// OFFSET: 0x00654B50
// FUNCTION: fn_05026
unsigned int GPakEx::RC6::Base::KeyLength() const {
  return 16;
}

// OFFSET: 0x00654B60
// FUNCTION: fn_05027
unsigned int GPakEx::TEA::Base::KeyLength() const {
  return 16;
}

// OFFSET: 0x00654B70
// FUNCTION: fn_05028
unsigned int GPakEx::ThreeWay::Base::KeyLength() const {
  return 12;
}

// OFFSET: 0x00654B80
// FUNCTION: fn_05029
unsigned int GPakEx::RC5::Base::KeyLength() const {
  return 16;
}

// OFFSET: 0x00654B90
// FUNCTION: fn_05030
unsigned int GPakEx::XTEA::Base::KeyLength() const {
  return 16;
}

// OFFSET: 0x00654BA0
// FUNCTION: fn_05031
unsigned int GPakEx::IDEA::Base::KeyLength() const {
  return 16;
}

// OFFSET: 0x00654BF0
// FUNCTION: fn_05033
// OFFSET: 0x00654C00
// FUNCTION: fn_05034
// OFFSET: 0x00654C10
// FUNCTION: fn_05035
// OFFSET: 0x00654C20
// FUNCTION: fn_05036
// OFFSET: 0x00654C30
// FUNCTION: fn_05037
// OFFSET: 0x00654C40
// FUNCTION: fn_05038
// OFFSET: 0x00654C50
// FUNCTION: fn_05039
// OFFSET: 0x00659AF0
// FUNCTION: fn_45528
// OFFSET: 0x00659B00
// FUNCTION: fn_45529
// OFFSET: 0x00659B10
// FUNCTION: fn_45530
template <GPakEx::CipherDir DIR, class BASE> bool GPakEx::BlockCipherFinal<DIR, BASE>::IsForwardTransformation() const {
  return DIR == ENCRYPTION;
}

// OFFSET: 0x00654C60
// FUNCTION: fn_05040
// OFFSET: 0x00654C70
// FUNCTION: fn_05041
// OFFSET: 0x00654C80
// FUNCTION: fn_05042
// OFFSET: 0x00654C90
// FUNCTION: fn_05043
// OFFSET: 0x00654CA0
// FUNCTION: fn_05044
// OFFSET: 0x00654CB0
// FUNCTION: fn_05045
// OFFSET: 0x00654CC0
// FUNCTION: fn_05046
// OFFSET: 0x00654CD0
// FUNCTION: fn_05047
// OFFSET: 0x00654CE0
// FUNCTION: fn_05048
// OFFSET: 0x00654CF0
// FUNCTION: fn_05049
// OFFSET: 0x00654D00
// FUNCTION: fn_05050
// OFFSET: 0x00659B20
// FUNCTION: fn_45531
// OFFSET: 0x00659B30
// FUNCTION: fn_45532
// OFFSET: 0x00659B40
// FUNCTION: fn_45533
// OFFSET: 0x00659B50
// FUNCTION: fn_45534
template <class CIPHER, GPakEx::CipherDir DIR, class BLOCKCIPHER, class MODE> bool GPakEx::CipherModeFinal<CIPHER, DIR, BLOCKCIPHER, MODE>::IsForwardTransformation() const {
  return DIR == ENCRYPTION;
}

// OFFSET: 0x00656BC0
// FUNCTION: fn_05079
GPakEx::CBC_Decryption::CBC_Decryption() {
}




// OFFSET: 0x00658FA0
// FUNCTION: fn_05114
unsigned int GPakEx::GOST::Base::BlockSize() const {
  return 8;
}

// OFFSET: 0x00658FB0
// FUNCTION: fn_05115
unsigned int GPakEx::GOST::Base::KeyLength() const {
  return 32;
}

// OFFSET: 0x0065A760
// FUNCTION: fn_46752
void GPakEx::IDEA::Base::DeKey() {
  FixedSecureBlock<std::uint32_t, 6 * ROUNDS + 4> tempkey;
  unsigned int i;
  for (i = 0; i < ROUNDS; ++i) {
    tempkey[i * 6 + 0] = IdeaMulInv(this->m_key[(ROUNDS - i) * 6 + 0]);
    tempkey[i * 6 + 1] = IdeaAddInv(this->m_key[(ROUNDS - i) * 6 + 1 + (i > 0)]);
    tempkey[i * 6 + 2] = IdeaAddInv(this->m_key[(ROUNDS - i) * 6 + 2 - (i > 0)]);
    tempkey[i * 6 + 3] = IdeaMulInv(this->m_key[(ROUNDS - i) * 6 + 3]);
    tempkey[i * 6 + 4] = this->m_key[(ROUNDS - i) * 6 - 2];
    tempkey[i * 6 + 5] = this->m_key[(ROUNDS - i) * 6 - 1];
  }
  tempkey[i * 6 + 0] = IdeaMulInv(this->m_key[(ROUNDS - i) * 6 + 0]);
  tempkey[i * 6 + 1] = IdeaAddInv(this->m_key[(ROUNDS - i) * 6 + 1]);
  tempkey[i * 6 + 2] = IdeaAddInv(this->m_key[(ROUNDS - i) * 6 + 2]);
  tempkey[i * 6 + 3] = IdeaMulInv(this->m_key[(ROUNDS - i) * 6 + 3]);
  this->m_key = tempkey;
}

// OFFSET: 0x0065A920
// FUNCTION: fn_46753
void GPakEx::IDEA::Base::EnKey(const std::uint8_t* userKey) {
  unsigned int i;
  for (i = 0; i < 8; ++i) {
    this->m_key[i] = (static_cast<std::uint32_t>(userKey[2 * i]) << 8) | userKey[2 * i + 1];
  }
  for (; i < 6 * ROUNDS + 4; ++i) {
    const unsigned int j = (i & ~7u) - 8;
    this->m_key[i] = static_cast<std::uint16_t>(this->m_key[j + (i + 1) % 8] << 9) | (this->m_key[j + (i + 2) % 8] >> 7);
  }
}

// OFFSET: 0x0065AA00
// FUNCTION: fn_05133
unsigned int IdeaMulInv(unsigned int x) {
  std::uint32_t y = x;
  for (unsigned int i = 0; i < 15; ++i) {
    IdeaMul(y, y & 0xFFFF);
    IdeaMul(y, x);
  }
  return y & 0xFFFF;
}

// OFFSET: 0x0065AA70
// FUNCTION: fn_05134
// VIRTUALIZED: 2025-12-02 0x00603EF0
void GPakEx::IDEA::Base::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  const std::uint32_t* key = this->m_key.m_data;
  std::uint32_t x0;
  std::uint32_t x1;
  std::uint32_t x2;
  std::uint32_t x3;
  BlockGetter<std::uint16_t, true>(inBlock)(x0)(x1)(x2)(x3);
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    IdeaMul(x0, key[i * 6 + 0]);
    x1 += key[i * 6 + 1];
    x2 += key[i * 6 + 2];
    IdeaMul(x3, key[i * 6 + 3]);
    std::uint32_t t0 = x0 ^ x2;
    IdeaMul(t0, key[i * 6 + 4]);
    std::uint32_t t1 = t0 + (x1 ^ x3);
    IdeaMul(t1, key[i * 6 + 5]);
    t0 += t1;
    x0 ^= t1;
    x3 ^= t0;
    t0 ^= x1;
    x1 = x2 ^ t1;
    x2 = t0;
  }
  IdeaMul(x0, key[ROUNDS * 6 + 0]);
  x2 += key[ROUNDS * 6 + 1];
  x1 += key[ROUNDS * 6 + 2];
  IdeaMul(x3, key[ROUNDS * 6 + 3]);
  BlockPutter<std::uint16_t, true>(xorBlock, outBlock)(x0)(x2)(x1)(x3);
}

// OFFSET: 0x0065ACF0
// FUNCTION: fn_05135
// VIRTUALIZED: 2025-12-02 0x00604140
void GPakEx::IDEA::Base::SetKey(std::span<const std::uint8_t> key) {
  this->EnKey(key.data());
  if (!this->IsForwardTransformation()) {
    this->DeKey();
  }
}

// OFFSET: 0x0065B1B0
// FUNCTION: fn_05136
// VIRTUALIZED: 2025-12-02 0x006045F0
void GPakEx::RC6::Enc::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  const std::uint32_t* sptr = this->m_sTable.data();
  std::uint32_t a;
  std::uint32_t b;
  std::uint32_t c;
  std::uint32_t d;
  BlockGetter<std::uint32_t, false>(inBlock)(a)(b)(c)(d);
  b += sptr[0];
  d += sptr[1];
  sptr += 2;
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    std::uint32_t t = _rotl(b * (2 * b + 1), 5);
    const std::uint32_t u = _rotl(d * (2 * d + 1), 5);
    a = _rotl(a ^ t, u & 31) + sptr[0];
    c = _rotl(c ^ u, t & 31) + sptr[1];
    t = a;
    a = b;
    b = c;
    c = d;
    d = t;
    sptr += 2;
  }
  a += sptr[0];
  c += sptr[1];
  BlockPutter<std::uint32_t, false>(xorBlock, outBlock)(a)(b)(c)(d);
}

// OFFSET: 0x0065B2E0
// FUNCTION: fn_05137
// VIRTUALIZED: 2025-12-02 0x00604570
void GPakEx::RC5::Enc::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  const std::uint32_t* sptr = this->m_sTable.data();
  std::uint32_t a;
  std::uint32_t b;
  BlockGetter<std::uint32_t, false>(inBlock)(a)(b);
  a += sptr[0];
  b += sptr[1];
  sptr += 2;
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    a = _rotl(a ^ b, b & 31) + sptr[2 * i + 0];
    b = _rotl(a ^ b, a & 31) + sptr[2 * i + 1];
  }
  BlockPutter<std::uint32_t, false>(xorBlock, outBlock)(a)(b);
}

// OFFSET: 0x0065B380
// FUNCTION: fn_05138
// VIRTUALIZED: 2025-12-02 0x00604890
void GPakEx::RC6::Base::SetKey(std::span<const std::uint8_t> key) {
  const std::uint32_t MAGIC_P = 0xB7E15163;
  const std::uint32_t MAGIC_Q = 0x9E3779B9;
  const unsigned int c = 4;
  this->m_sTable.resize(2 * ROUNDS + 4);
  FixedSecureBlock<std::uint32_t, 4> l;
  memcpy_s(l.m_data, sizeof(l.m_data), key.data(), key.size());
  this->m_sTable[0] = MAGIC_P;
  for (unsigned int j = 1; j < this->m_sTable.size(); ++j) {
    this->m_sTable[j] = this->m_sTable[j - 1] + MAGIC_Q;
  }
  std::uint32_t a = 0;
  std::uint32_t b = 0;
  const unsigned int n = 3 * std::max(this->m_sTable.size(), c);
  for (unsigned int h = 0; h < n; ++h) {
    a = this->m_sTable[h % this->m_sTable.size()] = _rotl(this->m_sTable[h % this->m_sTable.size()] + a + b, 3);
    b = l[h % c] = _rotl(l[h % c] + a + b, (a + b) & 31);
  }
}

// OFFSET: 0x0065B540
// FUNCTION: fn_05139
// VIRTUALIZED: 2025-12-02 0x00604700
void GPakEx::RC5::Base::SetKey(std::span<const std::uint8_t> key) {
  const std::uint32_t MAGIC_P = 0xB7E15163;
  const std::uint32_t MAGIC_Q = 0x9E3779B9;
  const unsigned int c = 4;
  this->m_sTable.resize(2 * (ROUNDS + 1));
  FixedSecureBlock<std::uint32_t, 4> l;
  memcpy_s(l.m_data, sizeof(l.m_data), key.data(), key.size());
  this->m_sTable[0] = MAGIC_P;
  for (unsigned int j = 1; j < this->m_sTable.size(); ++j) {
    this->m_sTable[j] = this->m_sTable[j - 1] + MAGIC_Q;
  }
  std::uint32_t a = 0;
  std::uint32_t b = 0;
  const unsigned int n = 3 * std::max(this->m_sTable.size(), c);
  for (unsigned int h = 0; h < n; ++h) {
    a = this->m_sTable[h % this->m_sTable.size()] = _rotl(this->m_sTable[h % this->m_sTable.size()] + a + b, 3);
    b = l[h % c] = _rotl(l[h % c] + a + b, (a + b) & 31);
  }
}

// OFFSET: 0x0065B940
// FUNCTION: fn_05140
// VIRTUALIZED: 2025-12-02 0x00604C10
void GPakEx::TEA::Enc::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  const std::uint32_t DELTA = 0x9E3779B9;
  std::uint32_t y;
  std::uint32_t z;
  BlockGetter<std::uint32_t, true>(inBlock)(y)(z);
  std::uint32_t sum = 0;
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    sum += DELTA;
    y += ((z << 4) + this->m_k[0]) ^ (z + sum) ^ ((z >> 5) + this->m_k[1]);
    z += ((y << 4) + this->m_k[2]) ^ (y + sum) ^ ((y >> 5) + this->m_k[3]);
  }
  BlockPutter<std::uint32_t, true>(xorBlock, outBlock)(y)(z);
}

// OFFSET: 0x0065BA30
// FUNCTION: fn_05141
// VIRTUALIZED: 2025-12-02 0x00604CD0
void GPakEx::XTEA::Enc::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  const std::uint32_t DELTA = 0x9E3779B9;
  std::uint32_t y;
  std::uint32_t z;
  BlockGetter<std::uint32_t, true>(inBlock)(y)(z);
  std::uint32_t sum = 0;
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    y += (((z << 4) ^ (z >> 5)) + z) ^ (sum + this->m_k[sum & 3]);
    sum += DELTA;
    z += (((y << 4) ^ (y >> 5)) + y) ^ (sum + this->m_k[(sum >> 11) & 3]);
  }
  BlockPutter<std::uint32_t, true>(xorBlock, outBlock)(y)(z);
}

// OFFSET: 0x0065BB10
// FUNCTION: fn_05142
// VIRTUALIZED: 2025-12-02 0x00604D90
void GPakEx::TEA::Base::SetKey(std::span<const std::uint8_t> key) {
  memcpy_s(this->m_k.m_data, sizeof(this->m_k.m_data), key.data(), key.size());
  ByteReverse(this->m_k.m_data, this->m_k.m_data, sizeof(this->m_k.m_data));
}

// OFFSET: 0x0065BBD0
// FUNCTION: fn_05143
// VIRTUALIZED: 2025-12-02 0x00604E10
void GPakEx::XTEA::Base::SetKey(std::span<const std::uint8_t> key) {
  memcpy_s(this->m_k.m_data, sizeof(this->m_k.m_data), key.data(), key.size());
  ByteReverse(this->m_k.m_data, this->m_k.m_data, sizeof(this->m_k.m_data));
}

// OFFSET: 0x0065BC90
// FUNCTION: fn_05144
// VIRTUALIZED: 2025-12-02 0x00604E90
void GPakEx::ThreeWay::Dec::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  std::uint32_t a0;
  std::uint32_t a1;
  std::uint32_t a2;
  BlockGetter<std::uint32_t, false>(inBlock)(a0)(a1)(a2);
  std::uint32_t rc = 0xB1B1;
  ThreeWayMu(a0, a1, a2);
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    a0 ^= this->m_k[0] ^ (rc << 16);
    a1 ^= this->m_k[1];
    a2 ^= this->m_k[2] ^ rc;
    ThreeWayTheta(a0, a1, a2);
    ThreeWayPiGammaPi(a0, a1, a2);
    rc <<= 1;
    if (rc & 0x10000) {
      rc ^= 0x11011;
    }
  }
  a0 ^= this->m_k[0] ^ (rc << 16);
  a1 ^= this->m_k[1];
  a2 ^= this->m_k[2] ^ rc;
  ThreeWayTheta(a0, a1, a2);
  ThreeWayMu(a0, a1, a2);
  BlockPutter<std::uint32_t, false>(xorBlock, outBlock)(a0)(a1)(a2);
}

// OFFSET: 0x0065C030
// FUNCTION: fn_05145
// VIRTUALIZED: 2025-12-02 0x00605210
void GPakEx::ThreeWay::Enc::ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const {
  std::uint32_t a0;
  std::uint32_t a1;
  std::uint32_t a2;
  BlockGetter<std::uint32_t, true>(inBlock)(a0)(a1)(a2);
  std::uint32_t rc = 0x0B0B;
  for (unsigned int i = 0; i < ROUNDS; ++i) {
    a0 ^= this->m_k[0] ^ (rc << 16);
    a1 ^= this->m_k[1];
    a2 ^= this->m_k[2] ^ rc;
    ThreeWayTheta(a0, a1, a2);
    ThreeWayPiGammaPi(a0, a1, a2);
    rc <<= 1;
    if (rc & 0x10000) {
      rc ^= 0x11011;
    }
  }
  a0 ^= this->m_k[0] ^ (rc << 16);
  a1 ^= this->m_k[1];
  a2 ^= this->m_k[2] ^ rc;
  ThreeWayTheta(a0, a1, a2);
  BlockPutter<std::uint32_t, true>(xorBlock, outBlock)(a0)(a1)(a2);
}

// OFFSET: 0x0065C230
// FUNCTION: fn_05146
// VIRTUALIZED: 2025-12-02 0x006053E0
void GPakEx::ThreeWay::Base::SetKey(std::span<const std::uint8_t> key) {
  const std::uint8_t* userKey = key.data();
  for (unsigned int i = 0; i < 3; ++i) {
    this->m_k[i] = GetWordBigEndian(userKey + 4 * i);
  }
  if (!this->IsForwardTransformation()) {
    ThreeWayTheta(this->m_k[0], this->m_k[1], this->m_k[2]);
    ThreeWayMu(this->m_k[0], this->m_k[1], this->m_k[2]);
    this->m_k[0] = _byteswap_ulong(this->m_k[0]);
    this->m_k[1] = _byteswap_ulong(this->m_k[1]);
    this->m_k[2] = _byteswap_ulong(this->m_k[2]);
  }
}

template class GPakEx::CipherModeFinal<GPakEx::RC6, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::RC6::Enc>, GPakEx::CFB_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::RC6, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::RC6::Enc>, GPakEx::OFB_Base>;
template class GPakEx::CipherModeFinal<GPakEx::TEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::TEA::Enc>, GPakEx::CFB_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::TEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::TEA::Enc>, GPakEx::OFB_Base>;
template class GPakEx::CipherModeFinal<GPakEx::ThreeWay, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::ThreeWay::Dec>, GPakEx::CBC_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::ThreeWay, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::ThreeWay::Enc>, GPakEx::OFB_Base>;
template class GPakEx::CipherModeFinal<GPakEx::RC5, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::RC5::Enc>, GPakEx::CFB_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::RC5, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::RC5::Enc>, GPakEx::OFB_Base>;
template class GPakEx::CipherModeFinal<GPakEx::XTEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::XTEA::Enc>, GPakEx::CFB_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::XTEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::XTEA::Enc>, GPakEx::OFB_Base>;
template class GPakEx::CipherModeFinal<GPakEx::IDEA, GPakEx::ENCRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::IDEA::Base>, GPakEx::CBC_Encryption>;
template class GPakEx::CipherModeFinal<GPakEx::GOST, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::GOST::Dec>, GPakEx::CBC_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::GOST, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::GOST::Enc>, GPakEx::CFB_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::ThreeWay, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::ThreeWay::Enc>, GPakEx::CFB_Decryption>;
template class GPakEx::CipherModeFinal<GPakEx::IDEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::IDEA::Base>, GPakEx::CBC_Decryption>;

struct PakAuxiliaryHeader {
  /* 0000 */ std::uint8_t version;
  /* 0001 */ std::uint8_t headerCipher;
  /* 0002 */ std::uint8_t padding_0002[2];
  /* 0004 */ std::uint32_t checksum;
  /* 0008 */ std::uint16_t cipher;
  /* 000A */ std::uint8_t compression;
  /* 000B */ std::uint8_t flags;
  /* 000C */ std::uint32_t paddedSize;
  /* 0010 */ std::uint32_t compressedSize;
  /* 0014 */ std::uint32_t outputSize;
  /* 0018 */ std::uint8_t outputHash[16];
  /* 0028 */ std::uint16_t keyId;
  /* 002A */ std::uint16_t padding_002A;
  /* 002C */ std::uint8_t material[60];
};

static_assert(sizeof(PakAuxiliaryHeader) == 104);
static_assert(offsetof(PakAuxiliaryHeader, keyId) == 40);

#pragma pack(push, 1)

struct PakArchiveKeyBlock {
  /* 0000 */ std::uint8_t version;
  /* 0001 */ std::uint8_t encrypted;
  /* 0002 */ std::uint8_t padding_0002[2];
  /* 0004 */ std::uint64_t identifier;
  /* 000C */ std::uint32_t size;
  /* 0010 */ std::uint32_t checksum;
};

#pragma pack(pop)

static_assert(sizeof(PakArchiveKeyBlock) == 20);

using PakGostDecryption = GPakEx::CipherModeFinal<GPakEx::GOST, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::GOST::Dec>, GPakEx::CBC_Decryption>;
using PakGostEncryption = GPakEx::CipherModeFinal<GPakEx::GOST, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::GOST::Enc>, GPakEx::CFB_Decryption>;
using PakThreeWayEncryption =
  GPakEx::CipherModeFinal<GPakEx::ThreeWay, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::ThreeWay::Enc>, GPakEx::CFB_Decryption>;
using PakIdeaDecryption = GPakEx::CipherModeFinal<GPakEx::IDEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::IDEA::Base>, GPakEx::CBC_Decryption>;

// GLOBAL: gl_26227
PakDecryptFunctionMap g_pakDecryptFunctions;
// GLOBAL: gl_26226
PakCrypterMap g_pakCrypters;

// INLINED
template <std::uint32_t Value, std::uint32_t First, std::uint32_t Second>
__forceinline std::uint32_t RevealConstant() {
  volatile std::uint32_t value = Value ^ First ^ Second;
  value ^= First;
  value ^= Second;
  return value;
}

// OFFSET: 0x00654240
// FUNCTION: fn_46750
// VIRTUALIZED: emulated 0x00654257
__declspec(noinline) bool PakDecryptBody(
  std::uint16_t cipher, std::string_view name, std::span<const std::uint8_t, 60> key, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
  std::span<std::uint8_t> output
) {
  bool result = false;

  const PakCrypterMap::const_iterator crypter = g_pakCrypters.find(cipher);
  if (crypter != g_pakCrypters.end()) {
    std::uint8_t mask[16];
    crypter->second->Decrypt(
      std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(name.data()), name.size()), key, hash, std::span<const std::uint8_t>(), mask
    );
  }

  const PakDecryptFunctionMap::const_iterator found = g_pakDecryptFunctions.find(cipher);
  if (found != g_pakDecryptFunctions.end()) {
    result = found->second(name, key, hash, input, output);
  }

  return result;
}

// OFFSET: 0x006543B0
// FUNCTION: fn_46749
bool IsSupportedCipher(std::uint16_t cipher) {
  return g_pakDecryptFunctions.contains(cipher);
}

enum PakHeaderIvSource : int {
  PAK_HEADER_IV_KEY = 0,
  PAK_HEADER_IV_NAME_HASH = 1,
  PAK_HEADER_IV_CHECKSUM_HASH = 2,
};

enum PakHeaderCipher : std::uint8_t {
  PAK_HEADER_CIPHER_NONE = 0,
  PAK_HEADER_CIPHER_AES_CBC_KEY_IV = 1,
  PAK_HEADER_CIPHER_AES_CBC_NAME_IV = 2,
  PAK_HEADER_CIPHER_AES_CBC_CHECKSUM_IV = 15,
};

struct PakHeaderSource {
  /* 0000 */ const std::span<const std::uint8_t>& name;
  /* 0004 */ const PakAuxiliaryHeader* header;
};

struct PakHeaderDecryptor {
  /* 0000 */ const PakHeaderSource& source;
  /* 0004 */ std::span<std::uint8_t>& input;
  /* 0008 */ std::span<std::uint8_t>& output;

  bool Decrypt(std::shared_ptr<AES>& cipher, const PakObfuscated<PakKeyMaterial>& key, int mode, int ivSource) const;
};

// OFFSET: 0x00655BA0
// FUNCTION: fn_46751
static std::shared_ptr<AES> CreateHeaderCipher(const PakObfuscated<PakKeyMaterial>& key, std::shared_ptr<AES> cipher) {
  PakKeyMaterial material = key.Unmask();
  cipher->Initialize(std::span<UCHAR>(material.key, sizeof(material.key)), AES::CBC);
  SecureZeroMemory(&material, sizeof(material));
  return cipher;
}

// OFFSET: 0x00655A30
// FUNCTION: fn_05068
bool PakHeaderDecryptor::Decrypt(std::shared_ptr<AES>& cipher, const PakObfuscated<PakKeyMaterial>& key, int mode, int ivSource) const {
  alignas(16) std::uint8_t iv[16] = {};
  const PakHeaderSource& source = this->source;

  switch (ivSource) {
  case PAK_HEADER_IV_KEY: {
    PakKeyMaterial material = key.Unmask();
    std::memcpy(iv, material.iv, sizeof(iv));
    SecureZeroMemory(&material, sizeof(material));
    break;
  }
  case PAK_HEADER_IV_NAME_HASH: {
    XXH128_hash_t hash = XXH3_128bits(source.name.data(), source.name.size());
    std::memcpy(iv, &hash, sizeof(iv));
    SecureZeroMemory(&hash, sizeof(hash));
    break;
  }
  case PAK_HEADER_IV_CHECKSUM_HASH: {
    XXH128_hash_t hash = XXH3_128bits(&source.header->checksum, sizeof(source.header->checksum));
    std::memcpy(iv, &hash, sizeof(iv));
    SecureZeroMemory(&hash, sizeof(hash));
    break;
  }
  }

  cipher->SetIV(iv);
  cipher->SetMode(mode);
  return cipher->Decrypt(this->input, this->output, 0) == RevealConstant<0x60, 0xF63071D0, 0x13F3B990>();
}

// OFFSET: 0x00659000
// FUNCTION: fn_05117
void PakAuxiliary::Initialize() {
  PakKeyMaterial material;
  utility::FillRandomBytes(std::span<std::uint8_t>(reinterpret_cast<std::uint8_t*>(&material), sizeof(material)), 9999);

  this->m_headerKey = material;
  this->m_archiveKeyBlockKey = material;
  this->m_streamKey = material;

  this->m_headerCipher = CreateHeaderCipher(this->m_headerKey, std::make_shared<AES>());
  this->m_archiveKeyCipher = CreateHeaderCipher(this->m_archiveKeyBlockKey, std::make_shared<AES>());

  this->m_ideaDecryption = std::make_shared<PakIdeaDecryption>();
  {
    PakKeyMaterial key = this->m_headerKey.Unmask();
    this->m_ideaDecryption->SetKey(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(&key), 16));
    SecureZeroMemory(&key, sizeof(key));
  }

  this->m_gostDecryption = std::make_shared<PakGostDecryption>();
  {
    PakKeyMaterial key = this->m_headerKey.Unmask();
    this->m_gostDecryption->SetKey(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(&key), 32));
    SecureZeroMemory(&key, sizeof(key));
  }

  this->m_gostStreamDecryption = std::make_shared<PakGostEncryption>();
  {
    PakKeyMaterial key = this->m_streamKey.Unmask();
    this->m_gostStreamDecryption->SetKey(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(&key), 32));
    SecureZeroMemory(&key, sizeof(key));
  }

  this->m_threeWayStreamDecryption = std::make_shared<PakThreeWayEncryption>();
  {
    PakKeyMaterial key = this->m_archiveKeyBlockKey.Unmask();
    this->m_threeWayStreamDecryption->SetKey(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(&key), 12));
    SecureZeroMemory(&key, sizeof(key));
  }
}

// OFFSET: 0x006583D0
// FUNCTION: fn_05111
bool PakAuxiliary::Extract(
  std::string_view name, const std::uint8_t* data, std::uint32_t size, std::span<const std::uint8_t> scratch, void* output, std::uint32_t outputSize, bool verify
) {
  PakAuxiliaryHeader header;
  if (!this->DecodeHeader(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(name.data()), name.size()), const_cast<std::uint8_t*>(data), size, &header)) {
    return false;
  }

  if (outputSize < header.outputSize || scratch.size() < header.compressedSize) {
    return false;
  }

  std::uint8_t key[60];
  if (header.keyId == 0) {
    std::memcpy(key, header.material, sizeof(key));
  } else {
    const PakArchiveKeyMap::const_iterator found = this->m_archiveKeys.find(header.keyId);
    if (found == this->m_archiveKeys.end()) {
      return false;
    }

    PakArchiveKeyRecord record = found->second.Unmask();
    std::memcpy(key, record.material, sizeof(key));
    SecureZeroMemory(&record, sizeof(record));
  }

  if (!CThemidaMgr::IsProtected()) {
    return false;
  }

  if (!CThemidaMgr::IsProtectedThread()) {
    return false;
  }

  const std::span<const std::uint8_t> body = std::span<const std::uint8_t>(data, size).subspan(sizeof(PakAuxiliaryHeader), header.paddedSize);
  const std::span<std::uint8_t> work = std::span<std::uint8_t>(const_cast<std::uint8_t*>(scratch.data()), scratch.size()).subspan(0, header.compressedSize);
  if (body.size() <= RevealConstant<0x80, 0x212EAE39, 0xB1716DE6>()) {
    memcpy_s(work.data(), work.size(), body.data(), body.size());
  } else {
    memcpy_s(work.data(), work.size(), body.data(), RevealConstant<0x80, 0xA98CB49C, 0x08B4532B>());
    if (!PakDecryptBody(
          header.cipher, name, std::span<const std::uint8_t, 60>(key), std::span<const std::uint8_t, 16>(header.outputHash),
          body.subspan(RevealConstant<0x80, 0x582A846C, 0x332515EF>()), work.subspan(RevealConstant<0x80, 0x5413D7C2, 0xB6BC8006>())
        )) {
      return false;
    }
  }

  if (header.compression == static_cast<std::uint8_t>(RevealConstant<1, 0x817B6EC5, 0x077B8495>())) {
    if (utility::DecompressZlib(work, output, outputSize) < 0) {
      return false;
    }
  } else if (header.compression == static_cast<std::uint8_t>(RevealConstant<2, 0x568000F0, 0x7F8F35D2>())) {
    if (utility::DecompressLz4(work, output, outputSize) < 0) {
      return false;
    }
  } else {
    if (work.size() > outputSize) {
      return false;
    }

    std::memcpy(output, work.data(), work.size());
  }

  if (verify) {
    const XXH128_hash_t hash = XXH3_128bits(output, header.outputSize);
    if (std::memcmp(&hash, header.outputHash, sizeof(header.outputHash)) != 0) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00658B90
// FUNCTION: fn_05112
bool PakAuxiliary::DecodeHeader(std::span<const std::uint8_t> name, std::uint8_t* data, std::uint32_t size, PakAuxiliaryHeader* header) {
  if (size < sizeof(PakAuxiliaryHeader)) {
    return false;
  }

  std::memcpy(header, data, sizeof(PakAuxiliaryHeader));
  if (header->version > 1) {
    return false;
  }

  std::span<std::uint8_t> input =
    std::span<std::uint8_t>(data, size).subspan(RevealConstant<8, 0xEB082218, 0x9C2B76A5>(), RevealConstant<0x60, 0xEB082218, 0x9C2B76A5>());
  std::span<std::uint8_t> output = std::span<std::uint8_t, sizeof(PakAuxiliaryHeader)>(reinterpret_cast<std::uint8_t*>(header), sizeof(PakAuxiliaryHeader))
                                     .subspan(RevealConstant<8, 0xB575CB53, 0x7FD9BE32>(), RevealConstant<0x60, 0xB575CB53, 0x7FD9BE32>());
  const PakHeaderSource source = {name, header};
  const PakHeaderDecryptor decryptor = {source, input, output};

  switch (header->headerCipher) {
  case PAK_HEADER_CIPHER_NONE:
    break;
  case PAK_HEADER_CIPHER_AES_CBC_KEY_IV:
    if (!decryptor.Decrypt(this->m_headerCipher, this->m_headerKey, AES::CBC, PAK_HEADER_IV_KEY)) {
      return false;
    }
    break;
  case PAK_HEADER_CIPHER_AES_CBC_NAME_IV:
    if (!decryptor.Decrypt(this->m_headerCipher, this->m_headerKey, AES::CBC, PAK_HEADER_IV_NAME_HASH)) {
      return false;
    }
    break;
  case PAK_HEADER_CIPHER_AES_CBC_CHECKSUM_IV:
    if (!decryptor.Decrypt(this->m_headerCipher, this->m_headerKey, AES::CBC, PAK_HEADER_IV_CHECKSUM_HASH)) {
      return false;
    }
    break;
  default:
    return false;
  }

  return XxHash32(output.data(), output.size(), 0) == header->checksum;
}

// OFFSET: 0x00659B60
// FUNCTION: fn_05118
__declspec(noinline) bool PakAuxiliary::Apply(std::span<const std::uint8_t> data, std::span<const std::uint8_t> auxiliary) {
  this->m_archiveKeys.clear();
  if (auxiliary.size() < sizeof(PakArchiveKeyBlock)) {
    return false;
  }

  PakArchiveKeyBlock block;
  std::memcpy(&block, auxiliary.data(), sizeof(block));
  if (block.version > 1) {
    return false;
  }

  if (block.size == 0) {
    return true;
  }

  if (auxiliary.size() != sizeof(block) + block.size) {
    return false;
  }

  std::uint8_t key[8];
  DeriveAuxiliaryKey(data.data(), key);

  std::uint8_t hash[32];
  SHA3 sha(256);
  sha.Add(key, sizeof(key));
  sha.GetHash(hash, sizeof(hash));

  CAES aes(16, 16);
  aes.MakeKey(reinterpret_cast<const char*>(hash), reinterpret_cast<const char*>(hash + 16), 16, 16);

  std::vector<std::uint8_t> records(block.size);
  aes.Decrypt(reinterpret_cast<const char*>(auxiliary.data() + sizeof(block)), reinterpret_cast<char*>(records.data()), records.size(), CAES::CBC);
  sha.Reset();

  const bool valid = XxHash32(records.data(), static_cast<std::uint32_t>(records.size()), 0) == block.checksum;
  if (valid) {
    for (std::size_t offset = 0; offset + sizeof(PakArchiveKeyRecord) <= records.size(); offset += sizeof(PakArchiveKeyRecord)) {
      const PakArchiveKeyRecord& record = *reinterpret_cast<const PakArchiveKeyRecord*>(records.data() + offset);
      this->m_archiveKeys.try_emplace(static_cast<std::uint16_t>(record.id), record);
    }
  }

  return valid;
}

// OFFSET: 0x00AEC820
// FUNCTION: fn_18136
bool CGPak::ApplyAuxiliary(std::span<const std::uint8_t> data) {
  std::vector<std::uint8_t> auxiliary;
  this->ReadAuxiliary(auxiliary);
  const std::int64_t identifier = ReadAuxiliaryIdentifier(auxiliary);

  if (auxiliary.empty() || identifier <= 0) {
    return false;
  }

  return this->m_auxiliary.Apply(data, auxiliary);
}

// OFFSET: 0x00AEC9F0
// FUNCTION: fn_18137
void CGPak::ApplyAuxiliary(const void* data, std::uint32_t size, const void* auxData, std::uint32_t auxSize) {
  this->m_auxiliary.Apply(
    std::span<const std::uint8_t>(static_cast<const std::uint8_t*>(data), size), std::span<const std::uint8_t>(static_cast<const std::uint8_t*>(auxData), auxSize)
  );
}

// OFFSET: 0x00654BB0
// FUNCTION: fn_05032
std::uint32_t Triple32Hash(std::uint32_t value) {
  value += 1;
  value ^= value >> 17;
  value *= 0xED5AD4BB;
  value ^= value >> 11;
  value *= 0xAC4C1B51;
  value ^= value >> 15;
  value *= 0x31848BAB;
  value ^= value >> 14;
  return value;
}

namespace {

  // INLINED
  inline std::span<std::uint8_t> MutableBytes(std::span<const std::uint8_t> bytes) {
    return std::span<std::uint8_t>(const_cast<std::uint8_t*>(bytes.data()), bytes.size());
  }

}

// OFFSET: 0x00644260
// FUNCTION: fn_04842
void XorWithKey16(std::span<const std::uint8_t, 16> key, std::span<const std::uint8_t> input, std::span<std::uint8_t> output) {
  std::uint8_t* destination = output.data();
  const std::uint8_t* source = input.data();
  std::size_t blockCount = input.size();
  const __m128i mask = _mm_loadu_si128(reinterpret_cast<const __m128i*>(key.data()));
  if (blockCount >= 16) {
    blockCount >>= 4;
    do {
      _mm_storeu_si128(reinterpret_cast<__m128i*>(destination), _mm_xor_si128(_mm_loadu_si128(reinterpret_cast<const __m128i*>(source)), mask));
      source += 16;
      destination += 16;
    } while (--blockCount != 0);
  }
  const std::size_t remaining = input.size() % 16;
  for (std::size_t i = 0; i < remaining; ++i) {
    destination[i] = source[i] ^ key[i];
  }
}

// OFFSET: 0x006442D0
// FUNCTION: fn_04843
// OFFSET: 0x00644480
// FUNCTION: fn_04844
// OFFSET: 0x00644630
// FUNCTION: fn_04845
// OFFSET: 0x006447E0
// FUNCTION: fn_04846
// OFFSET: 0x00644990
// FUNCTION: fn_04847
template <class CIPHER, std::size_t IV_OFFSET>
void DataHashBlockTransform(
  std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<std::uint8_t, 16> output
) {
  const std::span<const std::uint8_t> iv = parameters.subspan(
    GPakEx::Obfuscated<0x53F9360C, 0x4CC36824, std::size_t>(IV_OFFSET), GPakEx::Obfuscated<0x53F9360C, 0x4CC36824, std::size_t>(GPakEx::CipherSizes<CIPHER>::BLOCKSIZE)
  );
  std::array<std::uint8_t, 16> key = std::bit_cast<std::array<std::uint8_t, 16>>(XXH3_128bits(name.data(), name.size()));
  GPakEx::CipherModeFinal<CIPHER, GPakEx::ENCRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, typename CIPHER::Base>, GPakEx::CBC_Encryption> cipher;
  cipher.SetKeyWithIV(key, iv);
  cipher.Process(hash, output, false);
  SecureZeroMemory(key.data(), key.size());
}

// OFFSET: 0x00644B40
// FUNCTION: fn_04848
// OFFSET: 0x00644CE0
// FUNCTION: fn_04849
// OFFSET: 0x00644E80
// FUNCTION: fn_04850
// OFFSET: 0x00645020
// FUNCTION: fn_04851
template <class CIPHER, std::size_t IV_OFFSET>
void DataHashKeyTransform(
  std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<std::uint8_t, 16> output
) {
  const std::span<const std::uint8_t> iv = parameters.subspan(
    GPakEx::Obfuscated<0x53F9360C, 0x4CC36824, std::size_t>(IV_OFFSET), GPakEx::Obfuscated<0x53F9360C, 0x4CC36824, std::size_t>(GPakEx::CipherSizes<CIPHER>::BLOCKSIZE)
  );
  std::array<std::uint8_t, 16> nameHash = std::bit_cast<std::array<std::uint8_t, 16>>(XXH3_128bits(name.data(), name.size()));
  GPakEx::CipherModeFinal<CIPHER, GPakEx::ENCRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, typename CIPHER::Base>, GPakEx::CBC_Encryption> cipher;
  cipher.SetKeyWithIV(hash, iv);
  cipher.Process(nameHash, output, false);
  SecureZeroMemory(nameHash.data(), nameHash.size());
}

namespace {

  // INLINED
  template <class CIPHER, bool BY_NAME, std::size_t IV_OFFSET>
  inline void DataHashTransform(
    std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::array<std::uint8_t, 16>& output
  ) {
    if constexpr (BY_NAME) {
      DataHashBlockTransform<CIPHER, IV_OFFSET>(name, parameters, hash, output);
    } else {
      DataHashKeyTransform<CIPHER, IV_OFFSET>(name, parameters, hash, output);
    }
  }

}

// OFFSET: 0x0064BCE0
// FUNCTION: fn_04946
// OFFSET: 0x0064BF20
// FUNCTION: fn_04947
// OFFSET: 0x0064C160
// FUNCTION: fn_04948
// OFFSET: 0x0064C3A0
// FUNCTION: fn_04949
// OFFSET: 0x0064C620
// FUNCTION: fn_04950
// OFFSET: 0x0064C8A0
// FUNCTION: fn_04951
// OFFSET: 0x0064CB20
// FUNCTION: fn_04952
// OFFSET: 0x0064CD60
// FUNCTION: fn_04953
// OFFSET: 0x0064CFE0
// FUNCTION: fn_04954
// OFFSET: 0x0064D260
// FUNCTION: fn_04955
// OFFSET: 0x0064D4E0
// FUNCTION: fn_04956
// OFFSET: 0x0064D760
// FUNCTION: fn_04957
// OFFSET: 0x0064D9E0
// FUNCTION: fn_04958
template <
  int ID, int AES_MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET, std::size_t PLAIN_SIZE,
  std::size_t BLOCK_SIZE>
bool GPakEx::Crypter_SUB_XOR_AES_A<ID, AES_MODE, KEY_OFFSET, IV_OFFSET, MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET, PLAIN_SIZE, BLOCK_SIZE>::Decrypt(
  std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
  std::span<std::uint8_t> output
) const {
  if (input.empty()) {
    return true;
  }

  std::array<std::uint8_t, 16> mask = {};
  DataHashTransform<MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET>(name, parameters, hash, mask);

  const std::span<const std::uint8_t> key =
    parameters.subspan(Obfuscated<0x508EF2F3, 0xBFDC0C82, std::size_t>(KEY_OFFSET), Obfuscated<0x508EF2F3, 0xBFDC0C82, std::size_t>(16));
  const std::span<const std::uint8_t> iv =
    parameters.subspan(Obfuscated<0xA101B199, 0x10F4BC62, std::size_t>(IV_OFFSET), Obfuscated<0xA101B199, 0x10F4BC62, std::size_t>(16));
  AES aes;
  if (!aes.Initialize(MutableBytes(key), MutableBytes(iv), AES_MODE)) {
    return false;
  }

  constexpr std::size_t encryptedSize = (BLOCK_SIZE / 16 + 1) * 16;
  if (input.size() <= PLAIN_SIZE) {
    XorWithKey16(mask, input, output);
  } else if (input.size() <= PLAIN_SIZE + encryptedSize) {
    XorWithKey16(mask, input.first(PLAIN_SIZE), output.first(PLAIN_SIZE));
    if (aes.Decrypt(MutableBytes(input.subspan(PLAIN_SIZE)), output.subspan(PLAIN_SIZE), BCRYPT_BLOCK_PADDING) != output.size() - PLAIN_SIZE) {
      return false;
    }
  } else {
    XorWithKey16(mask, input.first(PLAIN_SIZE), output.first(PLAIN_SIZE));
    if (aes.Decrypt(MutableBytes(input.subspan(PLAIN_SIZE, encryptedSize)), output.subspan(PLAIN_SIZE, BLOCK_SIZE), BCRYPT_BLOCK_PADDING) != BLOCK_SIZE) {
      return false;
    }
    XorWithKey16(mask, input.subspan(PLAIN_SIZE + encryptedSize), output.subspan(PLAIN_SIZE + BLOCK_SIZE));
  }
  return true;
}

// OFFSET: 0x0064F000
// FUNCTION: fn_04959
// OFFSET: 0x0064F2F0
// FUNCTION: fn_04960
// OFFSET: 0x0064F560
// FUNCTION: fn_04961
// OFFSET: 0x0064F7F0
// FUNCTION: fn_04962
// OFFSET: 0x0064FAE0
// FUNCTION: fn_04963
// OFFSET: 0x0064FDC0
// FUNCTION: fn_04964
// OFFSET: 0x006500A0
// FUNCTION: fn_04965
// OFFSET: 0x00650350
// FUNCTION: fn_04966
// OFFSET: 0x006505E0
// FUNCTION: fn_04967
// OFFSET: 0x00650840
// FUNCTION: fn_04968
// OFFSET: 0x00650AB0
// FUNCTION: fn_04969
// OFFSET: 0x00650DD0
// FUNCTION: fn_04970
// OFFSET: 0x00651080
// FUNCTION: fn_04971
// OFFSET: 0x00651310
// FUNCTION: fn_04972
// OFFSET: 0x006515C0
// FUNCTION: fn_04973
// OFFSET: 0x006518E0
// FUNCTION: fn_04974
// OFFSET: 0x00651BD0
// FUNCTION: fn_04975
// OFFSET: 0x00651E30
// FUNCTION: fn_04976
// OFFSET: 0x006520E0
// FUNCTION: fn_04977
// OFFSET: 0x00652350
// FUNCTION: fn_04978
// OFFSET: 0x006525C0
// FUNCTION: fn_04979
// OFFSET: 0x006528B0
// FUNCTION: fn_04980
// OFFSET: 0x00652BD0
// FUNCTION: fn_04981
// OFFSET: 0x00652EB0
// FUNCTION: fn_04982
template <
  int ID, class CIPHER, class MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET,
  std::size_t PLAIN_SIZE, std::size_t BLOCK_SIZE>
bool GPakEx::Crypter_SUB_XOR_BlockCipher_A<ID, CIPHER, MODE, KEY_OFFSET, IV_OFFSET, MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET, PLAIN_SIZE, BLOCK_SIZE>::Decrypt(
  std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
  std::span<std::uint8_t> output
) const {
  if (input.empty()) {
    return true;
  }

  std::array<std::uint8_t, 60> material;
  utility::FillRandomBytes(material, static_cast<unsigned int>(XXH3_64bits(parameters.data(), parameters.size())));
  std::array<std::uint8_t, 16> mask = {};
  DataHashTransform<MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET>(name, material, hash, mask);

  const std::span<const std::uint8_t> key = std::span<const std::uint8_t, 60>(material).subspan(
    Obfuscated<0xC0C9A189, 0x9DD8961C, std::size_t>(KEY_OFFSET), Obfuscated<0xC0C9A189, 0x9DD8961C, std::size_t>(CipherSizes<CIPHER>::DEFAULT_KEYLENGTH)
  );
  const std::span<const std::uint8_t> iv = std::span<const std::uint8_t, 60>(material).subspan(
    Obfuscated<0x3D1F34CF, 0x7D456D99, std::size_t>(IV_OFFSET), Obfuscated<0x3D1F34CF, 0x7D456D99, std::size_t>(CipherSizes<CIPHER>::BLOCKSIZE)
  );
  typename MODE::template Decryption<CIPHER> cipher;
  cipher.SetKeyWithIV(key, iv);

  const std::size_t encryptedSize = cipher.CiphertextLength(BLOCK_SIZE, true);
  if (input.size() <= PLAIN_SIZE) {
    XorWithKey16(mask, input, output);
  } else if (input.size() <= PLAIN_SIZE + encryptedSize) {
    XorWithKey16(mask, input.first(PLAIN_SIZE), output.first(PLAIN_SIZE));
    if (cipher.Process(input.subspan(PLAIN_SIZE), output.subspan(PLAIN_SIZE), true) != output.size() - PLAIN_SIZE) {
      return false;
    }
  } else {
    XorWithKey16(mask, input.first(PLAIN_SIZE), output.first(PLAIN_SIZE));
    if (cipher.Process(input.subspan(PLAIN_SIZE, encryptedSize), output.subspan(PLAIN_SIZE, BLOCK_SIZE), true) != BLOCK_SIZE) {
      return false;
    }
    XorWithKey16(mask, input.subspan(PLAIN_SIZE + encryptedSize), output.subspan(PLAIN_SIZE + BLOCK_SIZE));
  }
  return true;
}

// OFFSET: 0x00654460
// FUNCTION: fn_04989
// OFFSET: 0x00654490
// FUNCTION: fn_04990
// OFFSET: 0x006544C0
// FUNCTION: fn_04991
// OFFSET: 0x006544F0
// FUNCTION: fn_04992
// OFFSET: 0x00654520
// FUNCTION: fn_04993
// OFFSET: 0x00654550
// FUNCTION: fn_04994
// OFFSET: 0x00654580
// FUNCTION: fn_04995
// OFFSET: 0x006545B0
// FUNCTION: fn_04996
// OFFSET: 0x006545E0
// FUNCTION: fn_04997
// OFFSET: 0x00654610
// FUNCTION: fn_04998
// OFFSET: 0x00654640
// FUNCTION: fn_04999
// OFFSET: 0x00654670
// FUNCTION: fn_05000
// OFFSET: 0x006546A0
// FUNCTION: fn_05001
template <
  int ID, int AES_MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET, std::size_t PLAIN_SIZE,
  std::size_t BLOCK_SIZE>
int GPakEx::Crypter_SUB_XOR_AES_A<ID, AES_MODE, KEY_OFFSET, IV_OFFSET, MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET, PLAIN_SIZE, BLOCK_SIZE>::Id() const {
  return Obfuscated<0x3821881D, 0xC8B24BB8, int>(ID);
}

// OFFSET: 0x006546D0
// FUNCTION: fn_05002
// OFFSET: 0x00654700
// FUNCTION: fn_05003
// OFFSET: 0x00654730
// FUNCTION: fn_05004
// OFFSET: 0x00654760
// FUNCTION: fn_05005
// OFFSET: 0x00654790
// FUNCTION: fn_05006
// OFFSET: 0x006547C0
// FUNCTION: fn_05007
// OFFSET: 0x006547F0
// FUNCTION: fn_05008
// OFFSET: 0x00654820
// FUNCTION: fn_05009
// OFFSET: 0x00654850
// FUNCTION: fn_05010
// OFFSET: 0x00654880
// FUNCTION: fn_05011
// OFFSET: 0x006548B0
// FUNCTION: fn_05012
// OFFSET: 0x006548E0
// FUNCTION: fn_05013
// OFFSET: 0x00654910
// FUNCTION: fn_05014
// OFFSET: 0x00654940
// FUNCTION: fn_05015
// OFFSET: 0x00654970
// FUNCTION: fn_05016
// OFFSET: 0x006549A0
// FUNCTION: fn_05017
// OFFSET: 0x006549D0
// FUNCTION: fn_05018
// OFFSET: 0x00654A00
// FUNCTION: fn_05019
// OFFSET: 0x00654A30
// FUNCTION: fn_05020
// OFFSET: 0x00654A60
// FUNCTION: fn_05021
// OFFSET: 0x00654A90
// FUNCTION: fn_05022
// OFFSET: 0x00654AC0
// FUNCTION: fn_05023
// OFFSET: 0x00654AF0
// FUNCTION: fn_05024
// OFFSET: 0x00654B20
// FUNCTION: fn_05025
template <
  int ID, class CIPHER, class MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET,
  std::size_t PLAIN_SIZE, std::size_t BLOCK_SIZE>
int GPakEx::Crypter_SUB_XOR_BlockCipher_A<ID, CIPHER, MODE, KEY_OFFSET, IV_OFFSET, MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET, PLAIN_SIZE, BLOCK_SIZE>::Id() const {
  return Obfuscated<0x2B225215, 0xD6A9A41E, int>(ID);
}

template void DataHashBlockTransform<GPakEx::IDEA, 0x34>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashBlockTransform<GPakEx::IDEA, 0x33>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashBlockTransform<GPakEx::IDEA, 0x32>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashBlockTransform<GPakEx::IDEA, 0x31>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashBlockTransform<GPakEx::IDEA, 0x30>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashKeyTransform<GPakEx::IDEA, 0x34>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashKeyTransform<GPakEx::IDEA, 0x32>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashKeyTransform<GPakEx::IDEA, 0x31>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template void DataHashKeyTransform<GPakEx::IDEA, 0x30>(std::span<const std::uint8_t>, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<std::uint8_t, 16>);
template class GPakEx::Crypter_SUB_XOR_AES_A<0x00F, 2, 16, 0, GPakEx::IDEA, true, 0x31, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x00E, 2, 44, 0, GPakEx::IDEA, false, 0x34, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x00D, 2, 16, 32, GPakEx::IDEA, false, 0x30, 2048, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x00C, 1, 44, 16, GPakEx::IDEA, false, 0x31, 2048, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x00B, 2, 44, 24, GPakEx::IDEA, true, 0x30, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x080, 2, 20, 36, GPakEx::IDEA, false, 0x32, 1024, 2048>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x201, 2, 16, 32, GPakEx::IDEA, true, 0x33, 0, 2048>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x00A, 2, 44, 28, GPakEx::IDEA, true, 0x34, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x009, 1, 16, 32, GPakEx::IDEA, false, 0x30, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x008, 1, 0, 16, GPakEx::IDEA, true, 0x32, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x007, 1, 44, 28, GPakEx::IDEA, true, 0x34, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x006, 2, 16, 32, GPakEx::IDEA, true, 0x31, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_AES_A<0x005, 1, 0, 16, GPakEx::IDEA, true, 0x30, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x082, GPakEx::ThreeWay, GPakEx::CBC_Mode, 0, 44, GPakEx::IDEA, false, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x081, GPakEx::ThreeWay, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, true, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x203, GPakEx::ThreeWay, GPakEx::OFB_Mode, 24, 44, GPakEx::IDEA, true, 0x31, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x202, GPakEx::ThreeWay, GPakEx::CBC_Mode, 16, 44, GPakEx::IDEA, false, 0x30, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x113, GPakEx::RC6, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, false, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x112, GPakEx::RC6, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, true, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x111, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 0, GPakEx::IDEA, false, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x110, GPakEx::RC6, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, true, 0x30, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10F, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 16, GPakEx::IDEA, false, 0x30, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10E, GPakEx::XTEA, GPakEx::OFB_Mode, 44, 24, GPakEx::IDEA, true, 0x31, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10D, GPakEx::RC5, GPakEx::OFB_Mode, 0, 16, GPakEx::IDEA, true, 0x32, 2048, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10C, GPakEx::RC6, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, false, 0x32, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10B, GPakEx::TEA, GPakEx::CFB_Mode, 16, 32, GPakEx::IDEA, false, 0x34, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10A, GPakEx::TEA, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, true, 0x31, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x109, GPakEx::RC5, GPakEx::OFB_Mode, 24, 44, GPakEx::IDEA, true, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x108, GPakEx::RC5, GPakEx::CFB_Mode, 44, 0, GPakEx::IDEA, false, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x107, GPakEx::RC5, GPakEx::CFB_Mode, 44, 16, GPakEx::IDEA, true, 0x30, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x106, GPakEx::TEA, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, true, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x105, GPakEx::TEA, GPakEx::OFB_Mode, 16, 32, GPakEx::IDEA, true, 0x33, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x104, GPakEx::XTEA, GPakEx::OFB_Mode, 16, 32, GPakEx::IDEA, true, 0x32, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x103, GPakEx::TEA, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, false, 0x30, 2048, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x102, GPakEx::RC5, GPakEx::CFB_Mode, 44, 28, GPakEx::IDEA, false, 0x32, 1024, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x101, GPakEx::RC5, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, false, 0x31, 0, 1024>;
template class GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x100, GPakEx::TEA, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, false, 0x31, 1024, 1024>;

// OFFSET: 0x0064DC60
// FUNCTION: fn_46740
// OFFSET: 0x0064E0A0
// FUNCTION: fn_46741
// OFFSET: 0x0064E4E0
// FUNCTION: fn_46742
// OFFSET: 0x0064E920
// FUNCTION: fn_46743
template <int AES_MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET, std::size_t PLAIN_SIZE, std::size_t BLOCK_SIZE>
bool DecryptSubXorAesBody(
  std::string_view name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
  std::span<std::uint8_t> output
) {
  std::array<std::uint8_t, 16> mask = {};
  DataHashTransform<MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET>(
    std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(name.data()), name.size()), parameters, hash, mask
  );

  const std::span<const std::uint8_t> key =
    parameters.subspan(GPakEx::Obfuscated<0x4CBFFD22, 0x063C6555, std::size_t>(KEY_OFFSET), GPakEx::Obfuscated<0x4CBFFD22, 0x063C6555, std::size_t>(16));
  const std::span<const std::uint8_t> iv =
    parameters.subspan(GPakEx::Obfuscated<0xEDF22406, 0x73B94AD2, std::size_t>(IV_OFFSET), GPakEx::Obfuscated<0xEDF22406, 0x73B94AD2, std::size_t>(16));
  AES aes;
  if (!aes.Initialize(MutableBytes(key), MutableBytes(iv), AES_MODE)) {
    return false;
  }

  if (input.size() <= GPakEx::Obfuscated<0xA32C8BB1, 0x48052735, std::size_t>(PLAIN_SIZE)) {
    XorWithKey16(mask, input, output);
  } else if (input.size() <= GPakEx::Obfuscated<0x584C0F94, 0x3D9B43B4, std::size_t>(PLAIN_SIZE + (BLOCK_SIZE / 16 + 1) * 16)) {
    XorWithKey16(
      mask, input.subspan(0, GPakEx::Obfuscated<0x9E1F4D5F, 0xB469CC8C, std::size_t>(PLAIN_SIZE)),
      output.subspan(0, GPakEx::Obfuscated<0x5EC52F52, 0x1FECA855, std::size_t>(PLAIN_SIZE))
    );
    const std::span<std::uint8_t> tail = output.subspan(GPakEx::Obfuscated<0xCDE8F136, 0xA29C35F3, std::size_t>(PLAIN_SIZE));
    if (aes.Decrypt(MutableBytes(input.subspan(GPakEx::Obfuscated<0xC47071A4, 0xC080ABD2, std::size_t>(PLAIN_SIZE))), tail, BCRYPT_BLOCK_PADDING) != tail.size()) {
      return false;
    }
  } else {
    XorWithKey16(
      mask, input.subspan(0, GPakEx::Obfuscated<0xC8344285, 0x43344C73, std::size_t>(PLAIN_SIZE)),
      output.subspan(0, GPakEx::Obfuscated<0xA76A2507, 0x50DB30F7, std::size_t>(PLAIN_SIZE))
    );
    const std::span<std::uint8_t> block = output.subspan(
      GPakEx::Obfuscated<0xDB023E9F, 0x590434B6, std::size_t>(PLAIN_SIZE), GPakEx::Obfuscated<0xDB023E9F, 0x590434B6, std::size_t>(BLOCK_SIZE)
    );
    if (aes.Decrypt(
          MutableBytes(input.subspan(
            GPakEx::Obfuscated<0xDF06F6A9, 0xCA9C78CF, std::size_t>(PLAIN_SIZE),
            GPakEx::Obfuscated<0xDF06F6A9, 0xCA9C78CF, std::size_t>((BLOCK_SIZE / 16 + 1) * 16)
          )),
          block, BCRYPT_BLOCK_PADDING
        ) != block.size()) {
      return false;
    }
    XorWithKey16(
      mask, input.subspan(GPakEx::Obfuscated<0xBB5E26F6, 0xF5C5F83D, std::size_t>(PLAIN_SIZE + (BLOCK_SIZE / 16 + 1) * 16)),
      output.subspan(GPakEx::Obfuscated<0x767283BC, 0x2C63347A, std::size_t>(PLAIN_SIZE + BLOCK_SIZE))
    );
  }
  return true;
}

template bool DecryptSubXorAesBody<1, 0, 16, GPakEx::IDEA, true, 0x30, 0, 1024>(
  std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>
);
template bool DecryptSubXorAesBody<2, 16, 32, GPakEx::IDEA, true, 0x31, 0, 1024>(
  std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>
);
template bool DecryptSubXorAesBody<1, 28, 44, GPakEx::IDEA, false, 0x34, 0, 1024>(
  std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>
);
template bool DecryptSubXorAesBody<1, 0, 16, GPakEx::IDEA, true, 0x32, 1024, 1024>(
  std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>
);

// OFFSET: 0x0064ED60
// FUNCTION: fn_46744
bool DecryptXorAesBody080(
  std::string_view name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
  std::span<std::uint8_t> output
) {
  std::memcpy(output.data(), input.data(), input.size());

  std::array<std::uint8_t, 16> mask = {};
  DataHashKeyTransform<GPakEx::IDEA, 0x31>(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(name.data()), name.size()), parameters, hash, mask);

  const std::span<const std::uint8_t> key =
    parameters.subspan(GPakEx::Obfuscated<0xBA26A7FC, 0xC658A09A, std::size_t>(44), GPakEx::Obfuscated<0xBA26A7FC, 0xC658A09A, std::size_t>(16));
  const std::span<const std::uint8_t> iv =
    parameters.subspan(GPakEx::Obfuscated<0x646AC844, 0x2F74AD24, std::size_t>(28), GPakEx::Obfuscated<0x646AC844, 0x2F74AD24, std::size_t>(16));
  AES aes;
  if (!aes.Initialize(MutableBytes(key), MutableBytes(iv), AES::CBC)) {
    return false;
  }

  if (input.size() < GPakEx::Obfuscated<0x0BE08B9C, 0xBD4F0E9F, std::size_t>(0xC00)) {
    const std::size_t length = input.size() - input.size() % 16;
    const std::span<std::uint8_t> block = output.subspan(0, length);
    if (aes.Decrypt(block, block, 0) != length) {
      return false;
    }
  } else {
    const std::span<std::uint8_t> block =
      output.subspan(GPakEx::Obfuscated<0xA3C9E6EC, 0x02C46CAC, std::size_t>(0x800), GPakEx::Obfuscated<0xA3C9E6EC, 0x02C46CAC, std::size_t>(0x400));
    if (aes.Decrypt(block, block, 0) != GPakEx::Obfuscated<0x0DF26A1E, 0x0C1D6160, std::size_t>(0x400)) {
      return false;
    }
  }

  XorWithKey16(mask, output, output);
  return true;
}

// OFFSET: 0x00653160
// FUNCTION: fn_46745
// OFFSET: 0x00653590
// FUNCTION: fn_46746
// OFFSET: 0x006539D0
// FUNCTION: fn_46747
// OFFSET: 0x00653E00
// FUNCTION: fn_46748
template <
  class CIPHER, class MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET, std::size_t PLAIN_SIZE,
  std::size_t BLOCK_SIZE>
bool DecryptSubXorBlockCipherBody(
  std::string_view name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
  std::span<std::uint8_t> output
) {
  std::array<std::uint8_t, 16> mask = {};
  DataHashTransform<MASK_CIPHER, MASK_BY_NAME, MASK_IV_OFFSET>(
    std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(name.data()), name.size()), parameters, hash, mask
  );

  const std::span<const std::uint8_t> key = parameters.subspan(
    GPakEx::Obfuscated<0x96CFF05E, 0x5050D543, std::size_t>(KEY_OFFSET),
    GPakEx::Obfuscated<0x96CFF05E, 0x5050D543, std::size_t>(GPakEx::CipherSizes<CIPHER>::DEFAULT_KEYLENGTH)
  );
  const std::span<const std::uint8_t> iv = parameters.subspan(
    GPakEx::Obfuscated<0x16554CCA, 0x4F238189, std::size_t>(IV_OFFSET), GPakEx::Obfuscated<0x16554CCA, 0x4F238189, std::size_t>(GPakEx::CipherSizes<CIPHER>::BLOCKSIZE)
  );
  typename MODE::template Decryption<CIPHER> cipher;
  cipher.SetKeyWithIV(key, iv);

  const std::size_t encryptedSize = cipher.CiphertextLength(GPakEx::Obfuscated<0x588A4B2C, 0x8656ECC9, std::size_t>(BLOCK_SIZE), true);
  if (input.size() <= GPakEx::Obfuscated<0xF261E719, 0x6A9DC46A, std::size_t>(PLAIN_SIZE)) {
    XorWithKey16(mask, input, output);
  } else if (input.size() <= GPakEx::Obfuscated<0x31E5270B, 0xD384431F, std::size_t>(PLAIN_SIZE) + encryptedSize) {
    XorWithKey16(
      mask, input.subspan(0, GPakEx::Obfuscated<0xE9F8CC60, 0x6D0D3DD6, std::size_t>(PLAIN_SIZE)),
      output.subspan(0, GPakEx::Obfuscated<0x0BEAF6FA, 0xB4EC07A0, std::size_t>(PLAIN_SIZE))
    );
    const std::span<std::uint8_t> tail = output.subspan(GPakEx::Obfuscated<0x8A3747F3, 0x0CE217A4, std::size_t>(PLAIN_SIZE));
    if (cipher.Process(input.subspan(GPakEx::Obfuscated<0xC2A91B0E, 0x5A72F7BA, std::size_t>(PLAIN_SIZE)), tail, true) != tail.size()) {
      return false;
    }
  } else {
    XorWithKey16(
      mask, input.subspan(0, GPakEx::Obfuscated<0xCF2A3959, 0x9278DE31, std::size_t>(PLAIN_SIZE)),
      output.subspan(0, GPakEx::Obfuscated<0xD8D14CE1, 0x0393D53B, std::size_t>(PLAIN_SIZE))
    );
    const std::span<std::uint8_t> block = output.subspan(
      GPakEx::Obfuscated<0x277A64D7, 0xDC111EEA, std::size_t>(PLAIN_SIZE), GPakEx::Obfuscated<0x277A64D7, 0xDC111EEA, std::size_t>(BLOCK_SIZE)
    );
    if (cipher.Process(input.subspan(GPakEx::Obfuscated<0x4BDCE5E1, 0x530722FA, std::size_t>(PLAIN_SIZE), encryptedSize), block, true) != block.size()) {
      return false;
    }
    XorWithKey16(
      mask, input.subspan(GPakEx::Obfuscated<0x1FA9A208, 0x02182E64, std::size_t>(PLAIN_SIZE) + encryptedSize),
      output.subspan(GPakEx::Obfuscated<0x2C321ED5, 0x1F5FEFAB, std::size_t>(PLAIN_SIZE + BLOCK_SIZE))
    );
  }
  return true;
}

namespace {

  // GLOBAL: gl_26189
  GPakEx::Crypter_SUB_XOR_AES_A<0x005, 1, 0, 16, GPakEx::IDEA, true, 0x30, 0, 1024> s_crypter005;
  // GLOBAL: gl_26190
  GPakEx::Crypter_SUB_XOR_AES_A<0x006, 2, 16, 32, GPakEx::IDEA, true, 0x31, 0, 1024> s_crypter006;
  // GLOBAL: gl_26192
  GPakEx::Crypter_SUB_XOR_AES_A<0x007, 1, 44, 28, GPakEx::IDEA, true, 0x34, 0, 1024> s_crypter007;
  // GLOBAL: gl_26191
  GPakEx::Crypter_SUB_XOR_AES_A<0x008, 1, 0, 16, GPakEx::IDEA, true, 0x32, 1024, 1024> s_crypter008;
  // GLOBAL: gl_26193
  GPakEx::Crypter_SUB_XOR_AES_A<0x009, 1, 16, 32, GPakEx::IDEA, false, 0x30, 1024, 1024> s_crypter009;
  // GLOBAL: gl_26195
  GPakEx::Crypter_SUB_XOR_AES_A<0x00A, 2, 44, 28, GPakEx::IDEA, true, 0x34, 1024, 1024> s_crypter00A;
  // GLOBAL: gl_26194
  GPakEx::Crypter_SUB_XOR_AES_A<0x00B, 2, 44, 24, GPakEx::IDEA, true, 0x30, 1024, 1024> s_crypter00B;
  // GLOBAL: gl_26196
  GPakEx::Crypter_SUB_XOR_AES_A<0x00C, 1, 44, 16, GPakEx::IDEA, false, 0x31, 2048, 1024> s_crypter00C;
  // GLOBAL: gl_26197
  GPakEx::Crypter_SUB_XOR_AES_A<0x00D, 2, 16, 32, GPakEx::IDEA, false, 0x30, 2048, 1024> s_crypter00D;
  // GLOBAL: gl_26198
  GPakEx::Crypter_SUB_XOR_AES_A<0x00E, 2, 44, 0, GPakEx::IDEA, false, 0x34, 1024, 1024> s_crypter00E;
  // GLOBAL: gl_26199
  GPakEx::Crypter_SUB_XOR_AES_A<0x00F, 2, 16, 0, GPakEx::IDEA, true, 0x31, 1024, 1024> s_crypter00F;
  // GLOBAL: gl_26200
  GPakEx::Crypter_SUB_XOR_AES_A<0x080, 2, 20, 36, GPakEx::IDEA, false, 0x32, 1024, 2048> s_crypter080;
  // GLOBAL: gl_26201
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x081, GPakEx::ThreeWay, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, true, 0x32, 0, 1024> s_crypter081;
  // GLOBAL: gl_26202
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x082, GPakEx::ThreeWay, GPakEx::CBC_Mode, 0, 44, GPakEx::IDEA, false, 0x32, 0, 1024> s_crypter082;
  // GLOBAL: gl_26203
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x100, GPakEx::TEA, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, false, 0x31, 1024, 1024> s_crypter100;
  // GLOBAL: gl_26204
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x101, GPakEx::RC5, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, false, 0x31, 0, 1024> s_crypter101;
  // GLOBAL: gl_26206
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x102, GPakEx::RC5, GPakEx::CFB_Mode, 44, 28, GPakEx::IDEA, false, 0x32, 1024, 1024> s_crypter102;
  // GLOBAL: gl_26205
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x103, GPakEx::TEA, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, false, 0x30, 2048, 1024> s_crypter103;
  // GLOBAL: gl_26207
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x104, GPakEx::XTEA, GPakEx::OFB_Mode, 16, 32, GPakEx::IDEA, true, 0x32, 0, 1024> s_crypter104;
  // GLOBAL: gl_26208
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x105, GPakEx::TEA, GPakEx::OFB_Mode, 16, 32, GPakEx::IDEA, true, 0x33, 0, 1024> s_crypter105;
  // GLOBAL: gl_26210
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x106, GPakEx::TEA, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, true, 0x32, 0, 1024> s_crypter106;
  // GLOBAL: gl_26209
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x107, GPakEx::RC5, GPakEx::CFB_Mode, 44, 16, GPakEx::IDEA, true, 0x30, 1024, 1024> s_crypter107;
  // GLOBAL: gl_26211
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x108, GPakEx::RC5, GPakEx::CFB_Mode, 44, 0, GPakEx::IDEA, false, 0x32, 0, 1024> s_crypter108;
  // GLOBAL: gl_26212
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x109, GPakEx::RC5, GPakEx::OFB_Mode, 24, 44, GPakEx::IDEA, true, 0x32, 0, 1024> s_crypter109;
  // GLOBAL: gl_26213
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10A, GPakEx::TEA, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, true, 0x31, 1024, 1024> s_crypter10A;
  // GLOBAL: gl_26214
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10B, GPakEx::TEA, GPakEx::CFB_Mode, 16, 32, GPakEx::IDEA, false, 0x34, 0, 1024> s_crypter10B;
  // GLOBAL: gl_26215
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10C, GPakEx::RC6, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, false, 0x32, 1024, 1024> s_crypter10C;
  // GLOBAL: gl_26216
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10D, GPakEx::RC5, GPakEx::OFB_Mode, 0, 16, GPakEx::IDEA, true, 0x32, 2048, 1024> s_crypter10D;
  // GLOBAL: gl_26217
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10E, GPakEx::XTEA, GPakEx::OFB_Mode, 44, 24, GPakEx::IDEA, true, 0x31, 1024, 1024> s_crypter10E;
  // GLOBAL: gl_26219
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10F, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 16, GPakEx::IDEA, false, 0x30, 1024, 1024> s_crypter10F;
  // GLOBAL: gl_26218
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x110, GPakEx::RC6, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, true, 0x30, 0, 1024> s_crypter110;
  // GLOBAL: gl_26220
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x111, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 0, GPakEx::IDEA, false, 0x32, 0, 1024> s_crypter111;
  // GLOBAL: gl_26221
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x112, GPakEx::RC6, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, true, 0x32, 0, 1024> s_crypter112;
  // GLOBAL: gl_26223
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x113, GPakEx::RC6, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, false, 0x32, 0, 1024> s_crypter113;
  // GLOBAL: gl_26222
  GPakEx::Crypter_SUB_XOR_AES_A<0x201, 2, 16, 32, GPakEx::IDEA, true, 0x33, 0, 2048> s_crypter201;
  // GLOBAL: gl_26224
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x202, GPakEx::ThreeWay, GPakEx::CBC_Mode, 16, 44, GPakEx::IDEA, false, 0x30, 1024, 1024> s_crypter202;
  // GLOBAL: gl_26225
  GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x203, GPakEx::ThreeWay, GPakEx::OFB_Mode, 24, 44, GPakEx::IDEA, true, 0x31, 1024, 1024> s_crypter203;

  // INLINED
  template <class CRYPTER> inline void RegisterCrypter(CRYPTER& crypter) {
    CRYPTER* instance = &crypter;
    std::uint16_t id = static_cast<std::uint16_t>(crypter.Id());
    g_pakCrypters.emplace(id, instance);
  }

  class PakCrypterRegistrar {
  public:
    PakCrypterRegistrar();
  };

  // OFFSET: 0x00647310
  // FUNCTION: fn_46739
  PakCrypterRegistrar::PakCrypterRegistrar() {
    RegisterCrypter(s_crypter005);
    RegisterCrypter(s_crypter006);
    RegisterCrypter(s_crypter007);
    RegisterCrypter(s_crypter008);
    RegisterCrypter(s_crypter009);
    RegisterCrypter(s_crypter00A);
    RegisterCrypter(s_crypter00B);
    RegisterCrypter(s_crypter00C);
    RegisterCrypter(s_crypter00D);
    RegisterCrypter(s_crypter00E);
    RegisterCrypter(s_crypter00F);
    RegisterCrypter(s_crypter080);
    RegisterCrypter(s_crypter081);
    RegisterCrypter(s_crypter082);
    RegisterCrypter(s_crypter100);
    RegisterCrypter(s_crypter101);
    RegisterCrypter(s_crypter102);
    RegisterCrypter(s_crypter103);
    RegisterCrypter(s_crypter104);
    RegisterCrypter(s_crypter105);
    RegisterCrypter(s_crypter106);
    RegisterCrypter(s_crypter107);
    RegisterCrypter(s_crypter108);
    RegisterCrypter(s_crypter109);
    RegisterCrypter(s_crypter10A);
    RegisterCrypter(s_crypter10B);
    RegisterCrypter(s_crypter10C);
    RegisterCrypter(s_crypter10D);
    RegisterCrypter(s_crypter10E);
    RegisterCrypter(s_crypter10F);
    RegisterCrypter(s_crypter110);
    RegisterCrypter(s_crypter111);
    RegisterCrypter(s_crypter112);
    RegisterCrypter(s_crypter113);
    RegisterCrypter(s_crypter201);
    RegisterCrypter(s_crypter202);
    RegisterCrypter(s_crypter203);

    g_pakDecryptFunctions.emplace(0x005, DecryptSubXorAesBody<1, 0, 16, GPakEx::IDEA, true, 0x30, 0, 1024>);
    g_pakDecryptFunctions.emplace(0x006, DecryptSubXorAesBody<2, 16, 32, GPakEx::IDEA, true, 0x31, 0, 1024>);
    g_pakDecryptFunctions.emplace(0x007, DecryptSubXorAesBody<1, 28, 44, GPakEx::IDEA, false, 0x34, 0, 1024>);
    g_pakDecryptFunctions.emplace(0x008, DecryptSubXorAesBody<1, 0, 16, GPakEx::IDEA, true, 0x32, 1024, 1024>);
    g_pakDecryptFunctions.emplace(0x080, DecryptXorAesBody080);
    g_pakDecryptFunctions.emplace(0x100, DecryptSubXorBlockCipherBody<GPakEx::TEA, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, true, 0x31, 1024, 1024>);
    g_pakDecryptFunctions.emplace(0x101, DecryptSubXorBlockCipherBody<GPakEx::RC5, GPakEx::OFB_Mode, 16, 32, GPakEx::IDEA, true, 0x31, 0, 1024>);
    g_pakDecryptFunctions.emplace(0x106, DecryptSubXorBlockCipherBody<GPakEx::TEA, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, false, 0x32, 0, 1024>);
    g_pakDecryptFunctions.emplace(0x10D, DecryptSubXorBlockCipherBody<GPakEx::RC5, GPakEx::OFB_Mode, 16, 0, GPakEx::IDEA, true, 0x32, 2048, 1024>);
  }

  // GLOBAL: gl_26228
  PakCrypterRegistrar s_pakCrypterRegistrar;

}
