#include "Utility/Random/RandomBytes.h"
#include "Security/ThemidaMgr.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Utility/Hash/SHA3.h"

#include <cstring>
#include <random>

namespace {
  struct RandomByteSource {
    /* 0000 */ std::mt19937 m_engine;
    /* 1388 */ std::uniform_int_distribution<int> m_distribution;

    // INLINED
    explicit RandomByteSource(unsigned int seed) : m_engine(seed), m_distribution(0, 255) {
    }

    // INLINED
    std::uint8_t Next() {
      return static_cast<std::uint8_t>(this->m_distribution(this->m_engine));
    }
  };
}

namespace utility {
  // OFFSET: 0x0061CBE0
  // FUNCTION: fn_04427
  void FillRandomBytes(std::span<std::uint8_t> bytes, unsigned int seed) {
    RandomByteSource source(seed);
    for (std::uint8_t& byte : bytes) {
      byte = source.Next();
    }
  }
}

// OFFSET: 0x0061D450
// FUNCTION: fn_46731
// VIRTUALIZED: emulated 0x027AABCA
void DeriveAuxiliaryKey(const std::uint8_t* digest, std::uint8_t* key) {
  std::uint32_t* words = reinterpret_cast<std::uint32_t*>(key);
  words[0] = 0x7E644C48;
  words[1] = 0x3869416E;

  EncryptedName<17, 0x73D1674B61DF7B0BULL>& salt = OBFUSCATE_STRING("%S4%X=HN`7]_bIvm", 0x73D1674B61DF7B0BULL);
  const std::uint8_t* text = reinterpret_cast<const std::uint8_t*>(salt.Text());

  constexpr std::uint8_t mask[8] = {0x1B, 0x51, 0x5E, 0x4F, 0x19, 0x42, 0x7E, 0x1B};
  for (std::size_t i = 0; i < 8; ++i) {
    key[i] = mask[i] ^ text[i] ^ text[i + 8] ^ digest[i] ^ digest[i + 8];
  }
}

// OFFSET: 0x0061D5B0
// FUNCTION: fn_04430
void ComputeLoginDigest(std::span<std::uint8_t, 16> digest, std::span<const std::uint8_t> data) {
  GenerateLoginDigest(digest, data);
}

// OFFSET: 0x0061D5D0
// FUNCTION: fn_04431
// VIRTUALIZED: emulated 0x02786CA5
__declspec(noinline) void GenerateLoginDigest(std::span<std::uint8_t, 16> digest, std::span<const std::uint8_t> data) {
  std::memcpy(digest.data(), "8{JBz#EcNb*<c2h{", digest.size());

  if (!CThemidaMgr::IsProtected()) {
    return;
  }

  constexpr std::uint8_t seed[8] = {0x31, 0x5C, 0x78, 0x46, 0x5D, 0x1C, 0x7C, 0x09};
  std::memcpy(digest.data(), seed, sizeof(seed));

  EncryptedName<17, 0x73D1674B61DF7B0BULL>& salt = OBFUSCATE_STRING("%S4%X=HN`7]_bIvm", 0x73D1674B61DF7B0BULL);
  const std::uint8_t* text = reinterpret_cast<const std::uint8_t*>(salt.Text());

  constexpr std::uint8_t mask[8] = {0x1B, 0x51, 0x5E, 0x4F, 0x19, 0x42, 0x7E, 0x1B};
  std::uint8_t prefix[8];
  for (std::size_t i = 0; i < 8; ++i) {
    prefix[i] = mask[i] ^ text[i] ^ text[i + 8];
  }

  SHA3 sha(256);
  sha.Add(prefix, sizeof(prefix));
  sha.Add(data.data(), static_cast<unsigned int>(data.size()));

  std::uint8_t hash[32];
  sha.GetHash(hash, sizeof(hash));

  std::uint8_t folded[8];
  for (std::size_t i = 0; i < 8; ++i) {
    folded[i] = hash[i] ^ hash[i + 8] ^ hash[i + 16] ^ hash[i + 24];
  }

  for (std::size_t i = 0; i < 8; ++i) {
    digest[i] ^= folded[i];
    digest[i + 8] = folded[i];
  }

  SecureZeroMemory(prefix, sizeof(prefix));
  SecureZeroMemory(hash, sizeof(hash));
  sha.Reset();
}
