#ifndef RAGNAROK_SOURCE_UTILITY_ENCRYPTEDNAME_H
#define RAGNAROK_SOURCE_UTILITY_ENCRYPTEDNAME_H

#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  // INLINED
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
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
  bool Encrypted() const {
    return this->m_encrypted;
  }

  // INLINED
  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

template <>
const char* EncryptedName<14, 0x3375B1C7DBB5FD55ULL>::Text();
template <>
const char* EncryptedName<10, 0x4755C1355575FD37ULL>::Text();

#endif
