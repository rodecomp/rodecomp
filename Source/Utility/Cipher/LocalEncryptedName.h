#ifndef RAGNAROK_SOURCE_UTILITY_CIPHER_LOCALENCRYPTEDNAME_H
#define RAGNAROK_SOURCE_UTILITY_CIPHER_LOCALENCRYPTEDNAME_H

#include "Utility/Cipher/XorBytes.h"

#include <array>
#include <cstddef>
#include <cstdint>

template <std::size_t Length>
class EncryptedSource {
public:
  // INLINED
  template <typename Byte>
  constexpr explicit EncryptedSource(const Byte (&source)[Length]) : data{} {
    for (std::size_t index = 0; index < Length; ++index) {
      this->data[index] = static_cast<char>(source[index]);
    }
  }

  char data[Length];
};

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

    // INLINED
    template <typename Byte>
    explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    // INLINED
    template <typename Byte>
    explicit EncryptedName(const std::array<Byte, Length>& source) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    // INLINED
    explicit EncryptedName(const EncryptedSource<Length>& source) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = static_cast<std::uint8_t>(source.data[index]);
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

    /* 0000 */ std::uint8_t m_text[Length];
    bool m_encrypted;
  };
}

#endif
