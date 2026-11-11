#ifndef RAGNAROK_SOURCE_UTILITY_CIPHER_CRYPTNAME_H
#define RAGNAROK_SOURCE_UTILITY_CIPHER_CRYPTNAME_H

#include <array>
#include <cstddef>
#include <cstdint>

namespace utility {

  template <std::size_t Length>
  struct CryptedText {
    char data[Length];

    // INLINED
    template <typename Element>
    constexpr operator std::array<Element, Length>() const {
      std::array<Element, Length> out{};
      for (std::size_t index = 0; index < Length; ++index) {
        out[index] = static_cast<Element>(this->data[index]);
      }
      return out;
    }
  };

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  constexpr CryptedText<Length> CryptText(const char (&text)[Length]) {
    CryptedText<Length> out{};
    for (std::size_t index = 0; index < Length; ++index) {
      out.data[index] = static_cast<char>(static_cast<std::uint8_t>(text[index]) ^ static_cast<std::uint8_t>(Key >> (8 * (index & 7))));
    }
    return out;
  }

}

#define CRYPT_NAME(text, key) (::utility::CryptText<key>(text))

#endif
