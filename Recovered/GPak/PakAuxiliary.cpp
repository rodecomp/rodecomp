// PakAuxiliary methods whose bodies are virtualised in every known Ragexe build
// (02-19: Apply jumps out at 0x00659B94, Extract at 0x0065861E). Linked only into the
// standalone exe built by scripts/exe/build_exe.py; Clank never compiles this file.
//
// Extract is rebuilt from the open GRF Editor decoder (Tokeiburu/GRFEditor,
// ProtectedGrfDecoder) and the EVENT_HORIZON_FLAG80_CRYPTO notes, following the visible
// 02-19 prologue: header decode, size checks, key lookup, then the body cipher and
// decompression.
//
// Apply is not rebuilt: in 02-19 its only callers (0x00AEA1F3, 0x00AEC820) have no xref,
// call, jump, pointer or RVA in the image, so it only traps if ever reached.

#include "Archive/GPak.h"
#include "Utility/AES.h"
#include "Utility/Hash/XxHash32.h"
#include "Utility/Hash/XxHash3.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>
#include <zlib.h>
#include <intrin.h>

namespace {

  constexpr std::size_t kHeaderSize = 104;

  // AES-128 key of the 96 encrypted header bytes (header variants 1, 2 and 15).
  constexpr std::uint8_t kHeaderKey[16] = {0x56, 0xfd, 0x7e, 0x71, 0x2f, 0x6d, 0x36, 0xf5, 0x5c, 0xa5, 0x99, 0xe6, 0x7b, 0x18, 0x10, 0x7a};
  constexpr std::uint8_t kHeaderIv1[16] = {0x4c, 0xe8, 0xa1, 0xae, 0x45, 0xb3, 0x21, 0x60, 0x7e, 0x4f, 0x26, 0xa4, 0xbb, 0x14, 0x0c, 0x96};

  // 3-Way key and IV of the header variant 0x71.
  constexpr std::uint8_t kThreeWayKey[12] = {0xad, 0x61, 0xae, 0xda, 0xbf, 0x20, 0x3e, 0x1d, 0xfe, 0x1d, 0xb7, 0x2b};
  constexpr std::uint8_t kThreeWayIv[12] = {0xa0, 0x95, 0xdf, 0xcf, 0xd0, 0x4b, 0x74, 0xdb, 0x02, 0x7e, 0xde, 0x09};

  using Hash128 = std::array<std::uint8_t, 16>;

  std::uint32_t ReadLe32(const std::uint8_t* p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<std::uint32_t>(p[3]) << 24);
  }

  std::uint32_t ReadBe32(const std::uint8_t* p) {
    return (static_cast<std::uint32_t>(p[0]) << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
  }

  void WriteBe32(std::uint8_t* p, std::uint32_t v) {
    p[0] = static_cast<std::uint8_t>(v >> 24);
    p[1] = static_cast<std::uint8_t>(v >> 16);
    p[2] = static_cast<std::uint8_t>(v >> 8);
    p[3] = static_cast<std::uint8_t>(v);
  }

  std::uint32_t Rotl(std::uint32_t v, int n) {
    return (v << (n & 31)) | (v >> ((32 - n) & 31));
  }

  void ThreeWayTheta(std::uint32_t& a, std::uint32_t& b, std::uint32_t& c) {
    const std::uint32_t mix = Rotl(a ^ b ^ c, 16) ^ Rotl(a ^ b ^ c, 8);
    const std::uint32_t u = (a << 24) ^ (c >> 8) ^ (b << 8) ^ (a >> 24);
    const std::uint32_t v = (b << 24) ^ (a >> 8) ^ (c << 8) ^ (b >> 24);
    a ^= mix ^ u;
    b ^= mix ^ v;
    c ^= mix ^ (u >> 16) ^ (v << 16);
  }

  // 3-Way encryption of one 12-byte block (11 rounds, big-endian words).
  void ThreeWayEncrypt(const std::uint8_t* key, const std::uint8_t* in, std::uint8_t* out) {
    const std::uint32_t k0 = ReadBe32(key), k1 = ReadBe32(key + 4), k2 = ReadBe32(key + 8);
    std::uint32_t a = ReadBe32(in), b = ReadBe32(in + 4), c = ReadBe32(in + 8);
    std::uint32_t rc = 0x0b0b;
    for (int round = 0; round < 11; ++round) {
      a ^= k0 ^ (rc << 16);
      b ^= k1;
      c ^= k2 ^ rc;
      ThreeWayTheta(a, b, c);
      const std::uint32_t u = Rotl(a, 22), v = Rotl(c, 1);
      a = Rotl(u ^ (b | ~v), 1);
      c = Rotl(v ^ (u | ~b), 22);
      b ^= v | ~u;
      rc <<= 1;
      if (rc & 0x10000) {
        rc ^= 0x11011;
      }
    }
    a ^= k0 ^ (rc << 16);
    b ^= k1;
    c ^= k2 ^ rc;
    ThreeWayTheta(a, b, c);
    WriteBe32(out, a);
    WriteBe32(out + 4, b);
    WriteBe32(out + 8, c);
  }

  bool ThreeWayCfbDecrypt(std::uint8_t* data, std::size_t size, const std::uint8_t* key, const std::uint8_t* iv) {
    if (size % 12 != 0) {
      return false;
    }
    std::uint8_t previous[12];
    std::memcpy(previous, iv, sizeof(previous));
    for (std::size_t pos = 0; pos < size; pos += 12) {
      std::uint8_t cipher[12], stream[12];
      std::memcpy(cipher, data + pos, sizeof(cipher));
      ThreeWayEncrypt(key, previous, stream);
      for (int i = 0; i < 12; ++i) {
        data[pos + i] ^= stream[i];
      }
      std::memcpy(previous, cipher, sizeof(previous));
    }
    return true;
  }

  // XXH3-128 as stored by the GRF: low64 then high64, both little-endian.
  Hash128 NativeHash(const void* data, std::size_t size, std::uint64_t seed = 0) {
    const XXH128_hash_t hash = XXH3_128bits_withSeed(data, size, seed);
    Hash128 out;
    std::memcpy(out.data(), &hash.low64, 8);
    std::memcpy(out.data() + 8, &hash.high64, 8);
    return out;
  }

  bool AesCbcDecrypt(std::uint8_t* data, std::size_t size, const std::uint8_t* key, const std::uint8_t* iv) {
    if (size % 16 != 0) {
      return false;
    }
    if (size == 0) {
      return true;
    }
    AES aes;
    std::uint8_t secret[16];
    std::memcpy(secret, key, sizeof(secret));
    if (!aes.Initialize(secret, sizeof(secret), std::span<const UCHAR>(iv, 16), AES::CBC)) {
      return false;
    }
    return aes.Decrypt(data, static_cast<ULONG>(size), data, static_cast<ULONG>(size), 0) == size;
  }

  // Decrypts the 96 header bytes after the plaintext {version, variant, ?, ?, xxh32} prefix
  // and validates them against that XXH32.
  bool DecodeHeader(const std::uint8_t* payload, std::size_t size, const Hash128& nameHash, std::uint8_t* header) {
    if (size < kHeaderSize || payload[0] != 1) {
      return false;
    }
    std::memcpy(header, payload, kHeaderSize);

    switch (header[1]) {
    case 1:
      if (!AesCbcDecrypt(header + 8, 96, kHeaderKey, kHeaderIv1)) {
        return false;
      }
      break;
    case 2:
      if (!AesCbcDecrypt(header + 8, 96, kHeaderKey, nameHash.data())) {
        return false;
      }
      break;
    case 15: {
      const Hash128 iv = NativeHash(header + 4, 4);
      if (!AesCbcDecrypt(header + 8, 96, kHeaderKey, iv.data())) {
        return false;
      }
      break;
    }
    case 113:
      if (!ThreeWayCfbDecrypt(header + 8, 96, kThreeWayKey, kThreeWayIv)) {
        return false;
      }
      break;
    default:
      return false;
    }

    return XxHash32(header + 8, 96, 0) == ReadLe32(header + 4);
  }

  // kRO archive keys 1-4. In 02-19 PakAuxiliary::Apply (which would load the GRF's own key
  // block) is unreachable, so these are the only keys an entry can reference besides id 0.
  constexpr std::uint8_t kArchiveKeys[4][60] = {
    {0x2c, 0x94, 0x21, 0x96, 0x40, 0x67, 0x35, 0xfe, 0xd0, 0xc6, 0xfc, 0x9e, 0x15, 0xb9, 0x48, 0x06, 0x17, 0x19, 0x6b, 0x53,
     0x4e, 0xaf, 0x25, 0xac, 0x03, 0x50, 0x81, 0xf2, 0xb6, 0xd4, 0xa3, 0x71, 0x51, 0xcd, 0xaf, 0x68, 0x47, 0x78, 0x6d, 0xd3,
     0xe4, 0x04, 0x03, 0x2f, 0x86, 0x03, 0x38, 0x3e, 0xc7, 0x49, 0x20, 0x03, 0x8a, 0x9e, 0x63, 0x04, 0x07, 0x57, 0x7e, 0x0b},
    {0x2a, 0x08, 0xda, 0x55, 0x41, 0x0a, 0x5d, 0xd3, 0x4a, 0xb5, 0xc1, 0xa4, 0x04, 0xb1, 0x67, 0xd9, 0xcd, 0x6b, 0x60, 0xe3,
     0x49, 0xfa, 0x8a, 0x60, 0x25, 0xbd, 0x0d, 0x28, 0x2c, 0x05, 0x50, 0xbd, 0x4c, 0x60, 0x67, 0x59, 0xc3, 0xdc, 0x9e, 0xb2,
     0x56, 0xb1, 0x45, 0x5b, 0x74, 0x3e, 0x81, 0xf9, 0xbc, 0x8a, 0x06, 0x6e, 0x76, 0xb6, 0x05, 0x4f, 0x11, 0xc1, 0xa3, 0x01},
    {0x1e, 0x5a, 0xb5, 0x22, 0xd9, 0x73, 0x34, 0xc9, 0x9b, 0xe5, 0x41, 0x33, 0x36, 0x17, 0x7a, 0xbe, 0x59, 0xe6, 0x77, 0x8c,
     0xe6, 0xd5, 0x4a, 0xa3, 0xdf, 0x2b, 0x26, 0x56, 0xb1, 0x80, 0xde, 0xfa, 0xe6, 0x7c, 0x0a, 0x11, 0xf5, 0x54, 0xce, 0x76,
     0x15, 0xbf, 0xe6, 0xc4, 0x13, 0x02, 0xfe, 0x05, 0x9c, 0xe0, 0xec, 0x6f, 0x59, 0xec, 0x7e, 0xd3, 0x49, 0x3b, 0x3d, 0x93},
    {0x58, 0x15, 0x32, 0x06, 0x74, 0x9f, 0x6d, 0x2a, 0x33, 0xa2, 0x0d, 0x4e, 0x51, 0xc7, 0xf5, 0x65, 0xb3, 0xc8, 0x04, 0xc8,
     0xd7, 0x61, 0x71, 0x76, 0x64, 0xdb, 0x6b, 0x81, 0xdd, 0xbf, 0x54, 0xe1, 0x4b, 0x07, 0x7c, 0x72, 0x8b, 0xad, 0x59, 0x0a,
     0x69, 0xe7, 0x6e, 0xa0, 0xd3, 0x80, 0x5c, 0xcb, 0x17, 0x28, 0x35, 0xd2, 0x1a, 0xa6, 0xec, 0x46, 0xfd, 0x95, 0x29, 0x8c},
  };

  // Fields of the decrypted header.
  struct EntryHeader {
    std::uint16_t cipher;     // +8
    std::uint8_t compression; // +10: 0 stored, 1 zlib, 2 LZ4
    std::uint8_t parameters;  // +11: bit 0 = key material XORed with the output hash
    std::uint32_t paddedSize; // +12
    std::uint32_t packedSize; // +16
    std::uint32_t outputSize; // +20
    Hash128 outputHash;       // +24
    std::uint32_t keyId;      // +40
  };

  EntryHeader ReadEntryHeader(const std::uint8_t* header) {
    EntryHeader entry;
    entry.cipher = static_cast<std::uint16_t>(header[8] | (header[9] << 8));
    entry.compression = header[10];
    entry.parameters = header[11];
    entry.paddedSize = ReadLe32(header + 12);
    entry.packedSize = ReadLe32(header + 16);
    entry.outputSize = ReadLe32(header + 20);
    std::memcpy(entry.outputHash.data(), header + 24, 16);
    entry.keyId = ReadLe32(header + 40);
    return entry;
  }

  // 64-byte cipher parameters: the 60 bytes of key material (inline for id 0, else archive key),
  // optionally XORed with the output hash.
  bool ResolveParameters(const std::uint8_t* header, const EntryHeader& entry, std::uint8_t (&parameters)[64]) {
    const std::uint8_t* material;
    if (entry.keyId == 0) {
      material = header + 44;
    } else if (entry.keyId <= 4) {
      material = kArchiveKeys[entry.keyId - 1];
    } else {
      return false;
    }

    std::memset(parameters, 0, sizeof(parameters));
    for (int i = 0; i < 60; ++i) {
      parameters[i] = material[i] ^ ((entry.parameters & 1) ? entry.outputHash[i % 16] : 0);
    }
    return true;
  }

  // --- IDEA (encrypt direction only: the format uses it to derive a 16-byte XOR mask) ---

  std::uint16_t IdeaMultiply(std::uint16_t a, std::uint16_t b) {
    const std::uint64_t left = a == 0 ? 65536 : a;
    const std::uint64_t right = b == 0 ? 65536 : b;
    return static_cast<std::uint16_t>(left * right % 65537);
  }

  void IdeaKeys(const std::uint8_t* key, std::uint16_t (&keys)[52]) {
    std::uint8_t value[16];
    std::memcpy(value, key, sizeof(value));
    for (int i = 0; i < 52; ++i) {
      if (i > 0 && i % 8 == 0) {
        std::uint8_t rotated[16];
        for (int j = 0; j < 16; ++j) {
          rotated[j] = static_cast<std::uint8_t>((value[(j + 3) % 16] << 1) | (value[(j + 4) % 16] >> 7));
        }
        std::memcpy(value, rotated, sizeof(value));
      }
      const int offset = i % 8 * 2;
      keys[i] = static_cast<std::uint16_t>((value[offset] << 8) | value[offset + 1]);
    }
  }

  void IdeaEncrypt(const std::uint8_t* in, const std::uint16_t (&keys)[52], std::uint8_t* out) {
    std::uint16_t a = static_cast<std::uint16_t>((in[0] << 8) | in[1]);
    std::uint16_t b = static_cast<std::uint16_t>((in[2] << 8) | in[3]);
    std::uint16_t c = static_cast<std::uint16_t>((in[4] << 8) | in[5]);
    std::uint16_t d = static_cast<std::uint16_t>((in[6] << 8) | in[7]);
    for (int i = 0; i < 48; i += 6) {
      a = IdeaMultiply(a, keys[i]);
      b = static_cast<std::uint16_t>(b + keys[i + 1]);
      c = static_cast<std::uint16_t>(c + keys[i + 2]);
      d = IdeaMultiply(d, keys[i + 3]);
      const std::uint16_t e = IdeaMultiply(static_cast<std::uint16_t>(a ^ c), keys[i + 4]);
      const std::uint16_t f = IdeaMultiply(static_cast<std::uint16_t>((b ^ d) + e), keys[i + 5]);
      const std::uint16_t g = static_cast<std::uint16_t>(e + f);
      const std::uint16_t oldB = b;
      a ^= f;
      b = static_cast<std::uint16_t>(c ^ f);
      c = static_cast<std::uint16_t>(oldB ^ g);
      d ^= g;
    }
    const std::uint16_t words[4] = {IdeaMultiply(a, keys[48]), static_cast<std::uint16_t>(c + keys[49]), static_cast<std::uint16_t>(b + keys[50]), IdeaMultiply(d, keys[51])};
    for (int i = 0; i < 4; ++i) {
      out[i * 2] = static_cast<std::uint8_t>(words[i] >> 8);
      out[i * 2 + 1] = static_cast<std::uint8_t>(words[i]);
    }
  }

  // IDEA-CBC encryption of a 16-byte input: the period-16 XOR mask of the body tail.
  Hash128 IdeaMask(const std::uint8_t* key, const std::uint8_t* input, const std::uint8_t* iv) {
    std::uint16_t keys[52];
    IdeaKeys(key, keys);
    std::uint8_t block[8];
    Hash128 mask;
    for (int i = 0; i < 8; ++i) {
      block[i] = input[i] ^ iv[i];
    }
    IdeaEncrypt(block, keys, mask.data());
    for (int i = 0; i < 8; ++i) {
      block[i] = input[8 + i] ^ mask[i];
    }
    IdeaEncrypt(block, keys, mask.data() + 8);
    return mask;
  }

  // --- RC6-32/20 (encrypt direction only: used as a CFB keystream) ---

  void Rc6Keys(const std::uint8_t* key, std::uint32_t (&keys)[44]) {
    std::uint32_t words[4];
    for (int n = 0; n < 4; ++n) {
      words[n] = ReadLe32(key + n * 4);
    }
    for (int n = 0; n < 44; ++n) {
      keys[n] = 0xb7e15163u + static_cast<std::uint32_t>(n) * 0x9e3779b9u;
    }
    std::uint32_t a = 0, b = 0;
    int i = 0, j = 0;
    for (int n = 0; n < 132; ++n) {
      a = keys[i] = Rotl(keys[i] + a + b, 3);
      b = words[j] = Rotl(words[j] + a + b, static_cast<int>(a + b));
      i = (i + 1) % 44;
      j = (j + 1) % 4;
    }
  }

  void Rc6Encrypt(const std::uint8_t* in, const std::uint32_t (&keys)[44], std::uint8_t* out) {
    std::uint32_t a = ReadLe32(in), b = ReadLe32(in + 4) + keys[0];
    std::uint32_t c = ReadLe32(in + 8), d = ReadLe32(in + 12) + keys[1];
    for (int i = 1; i <= 20; ++i) {
      const std::uint32_t t = Rotl(b * (b * 2 + 1), 5);
      const std::uint32_t u = Rotl(d * (d * 2 + 1), 5);
      a = Rotl(a ^ t, static_cast<int>(u)) + keys[2 * i];
      c = Rotl(c ^ u, static_cast<int>(t)) + keys[2 * i + 1];
      const std::uint32_t oldA = a;
      a = b;
      b = c;
      c = d;
      d = oldA;
    }
    const std::uint32_t words[4] = {a + keys[42], b, c + keys[43], d};
    std::memcpy(out, words, sizeof(words));
  }

  // --- CHAM-128/128 (80 rounds, big-endian words), CBC decryption ---

  void ChamCbcDecrypt(std::uint8_t* data, std::size_t size, const std::uint8_t* key, const std::uint8_t* iv) {
    std::uint32_t keys[8];
    for (int i = 0; i < 4; ++i) {
      const std::uint32_t k = ReadBe32(key + i * 4);
      keys[i] = k ^ Rotl(k, 1) ^ Rotl(k, 8);
      keys[(i + 4) ^ 1] = k ^ Rotl(k, 1) ^ Rotl(k, 11);
    }
    std::uint8_t previous[16];
    std::memcpy(previous, iv, sizeof(previous));
    for (std::size_t pos = 0; pos + 16 <= size; pos += 16) {
      std::uint8_t cipher[16];
      std::memcpy(cipher, data + pos, sizeof(cipher));
      std::uint32_t w[4];
      for (int i = 0; i < 4; ++i) {
        w[i] = ReadBe32(cipher + i * 4);
      }
      for (int round = 79; round >= 0; --round) {
        const int i = round % 4;
        const bool odd = (round & 1) != 0;
        w[i] = (Rotl(w[i], odd ? 31 : 24) - (Rotl(w[(i + 1) % 4], odd ? 8 : 1) ^ keys[round % 8])) ^ static_cast<std::uint32_t>(round);
      }
      for (int i = 0; i < 4; ++i) {
        WriteBe32(data + pos + i * 4, w[i]);
      }
      for (int i = 0; i < 16; ++i) {
        data[pos + i] ^= previous[i];
      }
      std::memcpy(previous, cipher, sizeof(previous));
    }
  }

  // --- Body ciphers: the first `offset` (128) body bytes are plaintext ---

  // Ciphers 128 (AES window) and 133 (RC6-CFB on the first 1024 bytes), then the IDEA mask.
  bool DecryptBodyTail(std::uint16_t cipher, const std::uint8_t (&parameters)[64], const Hash128& nameHash, const Hash128& outputHash, std::uint8_t* data, std::size_t size, std::size_t offset) {
    const Hash128 mask = cipher == 128 ? IdeaMask(outputHash.data(), nameHash.data(), parameters + 49) : IdeaMask(nameHash.data(), outputHash.data(), parameters + 49);
    const std::size_t length = size - offset;
    std::uint8_t* tail = data + offset;

    if (cipher == 128) {
      const std::size_t start = length < 3072 ? 0 : 2048;
      const std::size_t window = length < 3072 ? length / 16 * 16 : 1024;
      if (!AesCbcDecrypt(tail + start, window, parameters + 44, parameters + 28)) {
        return false;
      }
    } else {
      std::uint32_t keys[44];
      Rc6Keys(parameters + 24, keys);
      std::uint8_t previous[16];
      std::memcpy(previous, parameters + 40, sizeof(previous));
      const std::size_t window = (std::min)(length / 16 * 16, static_cast<std::size_t>(1024));
      for (std::size_t pos = 0; pos < window; pos += 16) {
        std::uint8_t encrypted[16], stream[16];
        std::memcpy(encrypted, tail + pos, sizeof(encrypted));
        Rc6Encrypt(previous, keys, stream);
        for (int i = 0; i < 16; ++i) {
          tail[pos + i] ^= stream[i];
        }
        std::memcpy(previous, encrypted, sizeof(previous));
      }
    }

    for (std::size_t i = 0; i < length; ++i) {
      tail[i] ^= mask[i % 16];
    }
    return true;
  }

  // Ciphers 5 and 7: AES-CBC with PKCS#7 padding over min(1024, packed) bytes, then the IDEA
  // mask over the rest. Shrinks `size` once the padding is removed.
  bool DecryptLegacyTail(std::uint16_t cipher, const std::uint8_t (&parameters)[64], const Hash128& nameHash, const Hash128& outputHash, std::uint8_t* data, std::size_t& size, std::size_t offset, std::size_t packedLength) {
    const std::size_t length = (std::min)(packedLength, static_cast<std::size_t>(1024));
    const std::size_t encryptedLength = (length / 16 + 1) * 16;
    const std::size_t tailLength = size - offset;
    if (tailLength < encryptedLength || tailLength - encryptedLength != packedLength - length) {
      return false;
    }

    const std::uint8_t* key = cipher == 5 ? parameters : parameters + 28;
    const std::uint8_t* iv = cipher == 5 ? parameters + 16 : parameters + 44;
    const Hash128 mask = cipher == 5 ? IdeaMask(nameHash.data(), outputHash.data(), parameters + 48) : IdeaMask(outputHash.data(), nameHash.data(), parameters + 52);

    std::uint8_t* tail = data + offset;
    if (!AesCbcDecrypt(tail, encryptedLength, key, iv)) {
      return false;
    }
    const std::uint8_t padding = tail[encryptedLength - 1];
    if (padding < 1 || padding > 16 || encryptedLength - padding != length) {
      return false;
    }
    for (std::size_t i = length; i < encryptedLength; ++i) {
      if (tail[i] != padding) {
        return false;
      }
    }

    std::memmove(tail + length, tail + encryptedLength, tailLength - encryptedLength);
    for (std::size_t i = 0; i < packedLength - length; ++i) {
      tail[length + i] ^= mask[i % 16];
    }
    size = offset + packedLength;
    return true;
  }

  // Cipher 1: CHAM-CBC on the first 1024 bytes keyed from the parameters and a seeded name
  // hash, then a rotate/XOR chain over every full block and the trailing bytes.
  void DecryptChamTail(const std::uint8_t (&parameters)[64], const Hash128& seededNameHash, const Hash128& outputHash, std::uint8_t* data, std::size_t size, std::size_t offset) {
    std::uint64_t seed;
    std::memcpy(&seed, seededNameHash.data() + 8, sizeof(seed));
    const Hash128 key = NativeHash(parameters, 60, seed);

    std::uint8_t* tail = data + offset;
    const std::size_t length = size - offset;
    const std::size_t fullLength = length / 16 * 16;
    ChamCbcDecrypt(tail, (std::min)(fullLength, static_cast<std::size_t>(1024)), key.data(), outputHash.data());

    std::uint8_t previous[16];
    std::memcpy(previous, outputHash.data(), sizeof(previous));
    for (std::size_t pos = 0; pos < fullLength; pos += 16) {
      std::uint8_t original[16];
      std::memcpy(original, tail + pos, sizeof(original));
      for (int i = 0; i < 16; ++i) {
        const int rotated = (i + 12) % 16;
        tail[pos + i] = original[rotated] ^ seededNameHash[rotated] ^ previous[i];
      }
      std::memcpy(previous, original, sizeof(previous));
    }
    for (std::size_t i = 0; i < length - fullLength; ++i) {
      tail[fullLength + i] ^= seededNameHash[i] ^ previous[i];
    }
  }

  // --- Decompression ---

  // LZ4 block format (no frame). Fails unless the stream fills `output` exactly.
  bool Lz4DecompressBlock(const std::uint8_t* src, std::size_t srcSize, std::uint8_t* output, std::size_t outputSize) {
    std::size_t in = 0, out = 0;
    while (in < srcSize) {
      const std::uint8_t token = src[in++];
      std::size_t literals = token >> 4;
      if (literals == 15) {
        std::uint8_t extra;
        do {
          if (in >= srcSize) {
            return false;
          }
          extra = src[in++];
          literals += extra;
        } while (extra == 255);
      }
      if (literals > srcSize - in || literals > outputSize - out) {
        return false;
      }
      std::memcpy(output + out, src + in, literals);
      in += literals;
      out += literals;
      if (in == srcSize) {
        break;
      }

      if (srcSize - in < 2) {
        return false;
      }
      const std::size_t distance = src[in] | (src[in + 1] << 8);
      in += 2;
      if (distance == 0 || distance > out) {
        return false;
      }
      std::size_t length = (token & 15) + 4;
      if ((token & 15) == 15) {
        std::uint8_t extra;
        do {
          if (in >= srcSize) {
            return false;
          }
          extra = src[in++];
          length += extra;
        } while (extra == 255);
      }
      if (length > outputSize - out) {
        return false;
      }
      for (std::size_t i = 0; i < length; ++i, ++out) {
        output[out] = output[out - distance];
      }
    }
    return out == outputSize;
  }

  // zlib stream that must end exactly at `srcSize` and produce exactly `outputSize` bytes.
  bool ZlibDecompress(const std::uint8_t* src, std::size_t srcSize, std::uint8_t* output, std::size_t outputSize) {
    z_stream stream{};
    if (inflateInit(&stream) != Z_OK) {
      return false;
    }
    std::vector<std::uint8_t> buffer(outputSize + 1);
    stream.next_in = const_cast<Bytef*>(src);
    stream.avail_in = static_cast<uInt>(srcSize);
    stream.next_out = buffer.data();
    stream.avail_out = static_cast<uInt>(buffer.size());
    const int status = inflate(&stream, Z_FINISH);
    const bool ok = status == Z_STREAM_END && stream.total_in == srcSize && stream.total_out == outputSize;
    inflateEnd(&stream);
    if (ok) {
      std::memcpy(output, buffer.data(), outputSize);
    }
    return ok;
  }

} // namespace

bool PakAuxiliary::Extract(
  std::string_view name, const std::uint8_t* data, std::uint32_t size, std::span<const std::uint8_t>, void* output, std::uint32_t outputSize, unsigned
) {
  const Hash128 nameHash = NativeHash(name.data(), name.size());
  std::uint8_t header[kHeaderSize];
  if (!DecodeHeader(data, size, nameHash, header)) {
    return false;
  }

  const EntryHeader entry = ReadEntryHeader(header);
  if (entry.outputSize != outputSize || entry.compression > 2 || (entry.parameters & ~1) != 0) {
    return false;
  }
  if (entry.paddedSize > size - kHeaderSize || entry.packedSize > entry.paddedSize) {
    return false;
  }

  std::uint8_t parameters[64];
  if (!ResolveParameters(header, entry, parameters)) {
    return false;
  }

  std::vector<std::uint8_t> body(data + kHeaderSize, data + kHeaderSize + entry.paddedSize);
  std::size_t bodySize = body.size();
  if (entry.packedSize > 128) {
    switch (entry.cipher) {
    case 1:
      DecryptChamTail(parameters, NativeHash(name.data(), name.size(), outputSize), entry.outputHash, body.data(), bodySize, 128);
      break;
    case 5:
    case 7:
      if (!DecryptLegacyTail(entry.cipher, parameters, nameHash, entry.outputHash, body.data(), bodySize, 128, entry.packedSize - 128)) {
        return false;
      }
      break;
    case 128:
    case 133:
      if (!DecryptBodyTail(entry.cipher, parameters, nameHash, entry.outputHash, body.data(), bodySize, 128)) {
        return false;
      }
      break;
    default:
      return false;
    }
  } else if (entry.cipher != 1 && entry.cipher != 5 && entry.cipher != 7 && entry.cipher != 128 && entry.cipher != 133) {
    return false;
  }
  if (entry.packedSize > bodySize) {
    return false;
  }

  std::uint8_t* destination = static_cast<std::uint8_t*>(output);
  switch (entry.compression) {
  case 0:
    if (entry.packedSize != outputSize) {
      return false;
    }
    std::memcpy(destination, body.data(), outputSize);
    break;
  case 1:
    if (!ZlibDecompress(body.data(), entry.packedSize, destination, outputSize)) {
      return false;
    }
    break;
  default:
    if (outputSize == 0 ? entry.packedSize != 1 || body[0] != 0 : !Lz4DecompressBlock(body.data(), entry.packedSize, destination, outputSize)) {
      return false;
    }
    break;
  }

  return NativeHash(destination, outputSize) == entry.outputHash;
}

void PakAuxiliary::Apply(const void*, std::uint32_t, const void*, std::uint32_t) {
  __debugbreak();
}
