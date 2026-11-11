#ifndef RAGNAROK_SOURCE_UTILITY_CIPHER_CRYPTNAME_H
#define RAGNAROK_SOURCE_UTILITY_CIPHER_CRYPTNAME_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

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
  consteval char CryptChar(const char (&text)[Length], std::size_t index) {
    return static_cast<char>(static_cast<std::uint8_t>(text[index]) ^ static_cast<std::uint8_t>(Key >> (8 * (index & 7))));
  }

  // INLINED
  template <std::uint64_t Key, std::size_t Length, std::size_t... Index>
  consteval CryptedText<Length> CryptText(const char (&text)[Length], std::index_sequence<Index...>) {
    return CryptedText<Length>{{CryptChar<Key>(text, Index)...}};
  }

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  consteval CryptedText<Length> CryptText(const char (&text)[Length]) {
    return CryptText<Key>(text, std::make_index_sequence<Length>{});
  }

}

#define CRYPT_NAME(text, key) (::utility::CryptText<key>(text))

#define CRYPT_CHAR_(text, key, index) \
  ([]() consteval { return static_cast<char>(static_cast<unsigned char>((text)[index]) ^ static_cast<unsigned char>((key) >> (8 * ((index) & 7)))); }())
#define CRYPT_CHARS_1_(text, key) CRYPT_CHAR_(text, key, 0)
#define CRYPT_CHARS_2_(text, key) CRYPT_CHARS_1_(text, key), CRYPT_CHAR_(text, key, 1)
#define CRYPT_CHARS_3_(text, key) CRYPT_CHARS_2_(text, key), CRYPT_CHAR_(text, key, 2)
#define CRYPT_CHARS_4_(text, key) CRYPT_CHARS_3_(text, key), CRYPT_CHAR_(text, key, 3)
#define CRYPT_CHARS_5_(text, key) CRYPT_CHARS_4_(text, key), CRYPT_CHAR_(text, key, 4)
#define CRYPT_CHARS_6_(text, key) CRYPT_CHARS_5_(text, key), CRYPT_CHAR_(text, key, 5)
#define CRYPT_CHARS_7_(text, key) CRYPT_CHARS_6_(text, key), CRYPT_CHAR_(text, key, 6)
#define CRYPT_CHARS_8_(text, key) CRYPT_CHARS_7_(text, key), CRYPT_CHAR_(text, key, 7)
#define CRYPT_CHARS_9_(text, key) CRYPT_CHARS_8_(text, key), CRYPT_CHAR_(text, key, 8)
#define CRYPT_CHARS_10_(text, key) CRYPT_CHARS_9_(text, key), CRYPT_CHAR_(text, key, 9)
#define CRYPT_CHARS_11_(text, key) CRYPT_CHARS_10_(text, key), CRYPT_CHAR_(text, key, 10)
#define CRYPT_CHARS_12_(text, key) CRYPT_CHARS_11_(text, key), CRYPT_CHAR_(text, key, 11)
#define CRYPT_CHARS_13_(text, key) CRYPT_CHARS_12_(text, key), CRYPT_CHAR_(text, key, 12)
#define CRYPT_CHARS_14_(text, key) CRYPT_CHARS_13_(text, key), CRYPT_CHAR_(text, key, 13)
#define CRYPT_CHARS_15_(text, key) CRYPT_CHARS_14_(text, key), CRYPT_CHAR_(text, key, 14)
#define CRYPT_CHARS_16_(text, key) CRYPT_CHARS_15_(text, key), CRYPT_CHAR_(text, key, 15)
#define CRYPT_CHARS_17_(text, key) CRYPT_CHARS_16_(text, key), CRYPT_CHAR_(text, key, 16)
#define CRYPT_CHARS_18_(text, key) CRYPT_CHARS_17_(text, key), CRYPT_CHAR_(text, key, 17)
#define CRYPT_CHARS_19_(text, key) CRYPT_CHARS_18_(text, key), CRYPT_CHAR_(text, key, 18)
#define CRYPT_CHARS_20_(text, key) CRYPT_CHARS_19_(text, key), CRYPT_CHAR_(text, key, 19)
#define CRYPT_CHARS_21_(text, key) CRYPT_CHARS_20_(text, key), CRYPT_CHAR_(text, key, 20)
#define CRYPT_CHARS_22_(text, key) CRYPT_CHARS_21_(text, key), CRYPT_CHAR_(text, key, 21)
#define CRYPT_CHARS_23_(text, key) CRYPT_CHARS_22_(text, key), CRYPT_CHAR_(text, key, 22)
#define CRYPT_CHARS_24_(text, key) CRYPT_CHARS_23_(text, key), CRYPT_CHAR_(text, key, 23)
#define CRYPT_CHARS_25_(text, key) CRYPT_CHARS_24_(text, key), CRYPT_CHAR_(text, key, 24)
#define CRYPT_CHARS_26_(text, key) CRYPT_CHARS_25_(text, key), CRYPT_CHAR_(text, key, 25)
#define CRYPT_CHARS_27_(text, key) CRYPT_CHARS_26_(text, key), CRYPT_CHAR_(text, key, 26)
#define CRYPT_CHARS_28_(text, key) CRYPT_CHARS_27_(text, key), CRYPT_CHAR_(text, key, 27)
#define CRYPT_CHARS_29_(text, key) CRYPT_CHARS_28_(text, key), CRYPT_CHAR_(text, key, 28)
#define CRYPT_CHARS_30_(text, key) CRYPT_CHARS_29_(text, key), CRYPT_CHAR_(text, key, 29)
#define CRYPT_CHARS_31_(text, key) CRYPT_CHARS_30_(text, key), CRYPT_CHAR_(text, key, 30)
#define CRYPT_CHARS_32_(text, key) CRYPT_CHARS_31_(text, key), CRYPT_CHAR_(text, key, 31)
#define CRYPT_CHARS_33_(text, key) CRYPT_CHARS_32_(text, key), CRYPT_CHAR_(text, key, 32)
#define CRYPT_CHARS_34_(text, key) CRYPT_CHARS_33_(text, key), CRYPT_CHAR_(text, key, 33)
#define CRYPT_CHARS_35_(text, key) CRYPT_CHARS_34_(text, key), CRYPT_CHAR_(text, key, 34)
#define CRYPT_CHARS_36_(text, key) CRYPT_CHARS_35_(text, key), CRYPT_CHAR_(text, key, 35)
#define CRYPT_CHARS_37_(text, key) CRYPT_CHARS_36_(text, key), CRYPT_CHAR_(text, key, 36)
#define CRYPT_CHARS_38_(text, key) CRYPT_CHARS_37_(text, key), CRYPT_CHAR_(text, key, 37)
#define CRYPT_CHARS_39_(text, key) CRYPT_CHARS_38_(text, key), CRYPT_CHAR_(text, key, 38)
#define CRYPT_CHARS_40_(text, key) CRYPT_CHARS_39_(text, key), CRYPT_CHAR_(text, key, 39)
#define CRYPT_CHARS_41_(text, key) CRYPT_CHARS_40_(text, key), CRYPT_CHAR_(text, key, 40)
#define CRYPT_CHARS_42_(text, key) CRYPT_CHARS_41_(text, key), CRYPT_CHAR_(text, key, 41)
#define CRYPT_CHARS_43_(text, key) CRYPT_CHARS_42_(text, key), CRYPT_CHAR_(text, key, 42)
#define CRYPT_CHARS_44_(text, key) CRYPT_CHARS_43_(text, key), CRYPT_CHAR_(text, key, 43)
#define CRYPT_CHARS_45_(text, key) CRYPT_CHARS_44_(text, key), CRYPT_CHAR_(text, key, 44)
#define CRYPT_CHARS_46_(text, key) CRYPT_CHARS_45_(text, key), CRYPT_CHAR_(text, key, 45)
#define CRYPT_CHARS_47_(text, key) CRYPT_CHARS_46_(text, key), CRYPT_CHAR_(text, key, 46)
#define CRYPT_CHARS_48_(text, key) CRYPT_CHARS_47_(text, key), CRYPT_CHAR_(text, key, 47)
#define CRYPT_CHARS_49_(text, key) CRYPT_CHARS_48_(text, key), CRYPT_CHAR_(text, key, 48)
#define CRYPT_CHARS_50_(text, key) CRYPT_CHARS_49_(text, key), CRYPT_CHAR_(text, key, 49)
#define CRYPT_CHARS_51_(text, key) CRYPT_CHARS_50_(text, key), CRYPT_CHAR_(text, key, 50)
#define CRYPT_CHARS_52_(text, key) CRYPT_CHARS_51_(text, key), CRYPT_CHAR_(text, key, 51)
#define CRYPT_CHARS_53_(text, key) CRYPT_CHARS_52_(text, key), CRYPT_CHAR_(text, key, 52)
#define CRYPT_CHARS_54_(text, key) CRYPT_CHARS_53_(text, key), CRYPT_CHAR_(text, key, 53)
#define CRYPT_CHARS_55_(text, key) CRYPT_CHARS_54_(text, key), CRYPT_CHAR_(text, key, 54)
#define CRYPT_CHARS_56_(text, key) CRYPT_CHARS_55_(text, key), CRYPT_CHAR_(text, key, 55)
#define CRYPT_CHARS_57_(text, key) CRYPT_CHARS_56_(text, key), CRYPT_CHAR_(text, key, 56)
#define CRYPT_CHARS_58_(text, key) CRYPT_CHARS_57_(text, key), CRYPT_CHAR_(text, key, 57)
#define CRYPT_CHARS_59_(text, key) CRYPT_CHARS_58_(text, key), CRYPT_CHAR_(text, key, 58)
#define CRYPT_CHARS_60_(text, key) CRYPT_CHARS_59_(text, key), CRYPT_CHAR_(text, key, 59)
#define CRYPT_CHARS_61_(text, key) CRYPT_CHARS_60_(text, key), CRYPT_CHAR_(text, key, 60)
#define CRYPT_CHARS_62_(text, key) CRYPT_CHARS_61_(text, key), CRYPT_CHAR_(text, key, 61)
#define CRYPT_CHARS_63_(text, key) CRYPT_CHARS_62_(text, key), CRYPT_CHAR_(text, key, 62)
#define CRYPT_CHARS_64_(text, key) CRYPT_CHARS_63_(text, key), CRYPT_CHAR_(text, key, 63)
#define CRYPT_CHARS(length, text, key) {CRYPT_CHARS_##length##_(text, key)}

#define CRYPT_BYTE_(text, key, index) \
  ([]() consteval { return static_cast<std::uint8_t>(static_cast<unsigned char>((text)[index]) ^ static_cast<unsigned char>((key) >> (8 * ((index) & 7)))); }())
#define CRYPT_BYTES_1_(text, key) CRYPT_BYTE_(text, key, 0)
#define CRYPT_BYTES_2_(text, key) CRYPT_BYTES_1_(text, key), CRYPT_BYTE_(text, key, 1)
#define CRYPT_BYTES_3_(text, key) CRYPT_BYTES_2_(text, key), CRYPT_BYTE_(text, key, 2)
#define CRYPT_BYTES_4_(text, key) CRYPT_BYTES_3_(text, key), CRYPT_BYTE_(text, key, 3)
#define CRYPT_BYTES_5_(text, key) CRYPT_BYTES_4_(text, key), CRYPT_BYTE_(text, key, 4)
#define CRYPT_BYTES_6_(text, key) CRYPT_BYTES_5_(text, key), CRYPT_BYTE_(text, key, 5)
#define CRYPT_BYTES_7_(text, key) CRYPT_BYTES_6_(text, key), CRYPT_BYTE_(text, key, 6)
#define CRYPT_BYTES_8_(text, key) CRYPT_BYTES_7_(text, key), CRYPT_BYTE_(text, key, 7)
#define CRYPT_BYTES_9_(text, key) CRYPT_BYTES_8_(text, key), CRYPT_BYTE_(text, key, 8)
#define CRYPT_BYTES_10_(text, key) CRYPT_BYTES_9_(text, key), CRYPT_BYTE_(text, key, 9)
#define CRYPT_BYTES_11_(text, key) CRYPT_BYTES_10_(text, key), CRYPT_BYTE_(text, key, 10)
#define CRYPT_BYTES_12_(text, key) CRYPT_BYTES_11_(text, key), CRYPT_BYTE_(text, key, 11)
#define CRYPT_BYTES_13_(text, key) CRYPT_BYTES_12_(text, key), CRYPT_BYTE_(text, key, 12)
#define CRYPT_BYTES_14_(text, key) CRYPT_BYTES_13_(text, key), CRYPT_BYTE_(text, key, 13)
#define CRYPT_BYTES_15_(text, key) CRYPT_BYTES_14_(text, key), CRYPT_BYTE_(text, key, 14)
#define CRYPT_BYTES_16_(text, key) CRYPT_BYTES_15_(text, key), CRYPT_BYTE_(text, key, 15)
#define CRYPT_BYTES_17_(text, key) CRYPT_BYTES_16_(text, key), CRYPT_BYTE_(text, key, 16)
#define CRYPT_BYTES_18_(text, key) CRYPT_BYTES_17_(text, key), CRYPT_BYTE_(text, key, 17)
#define CRYPT_BYTES_19_(text, key) CRYPT_BYTES_18_(text, key), CRYPT_BYTE_(text, key, 18)
#define CRYPT_BYTES_20_(text, key) CRYPT_BYTES_19_(text, key), CRYPT_BYTE_(text, key, 19)
#define CRYPT_BYTES_21_(text, key) CRYPT_BYTES_20_(text, key), CRYPT_BYTE_(text, key, 20)
#define CRYPT_BYTES_22_(text, key) CRYPT_BYTES_21_(text, key), CRYPT_BYTE_(text, key, 21)
#define CRYPT_BYTES_23_(text, key) CRYPT_BYTES_22_(text, key), CRYPT_BYTE_(text, key, 22)
#define CRYPT_BYTES_24_(text, key) CRYPT_BYTES_23_(text, key), CRYPT_BYTE_(text, key, 23)
#define CRYPT_BYTES_25_(text, key) CRYPT_BYTES_24_(text, key), CRYPT_BYTE_(text, key, 24)
#define CRYPT_BYTES_26_(text, key) CRYPT_BYTES_25_(text, key), CRYPT_BYTE_(text, key, 25)
#define CRYPT_BYTES_27_(text, key) CRYPT_BYTES_26_(text, key), CRYPT_BYTE_(text, key, 26)
#define CRYPT_BYTES_28_(text, key) CRYPT_BYTES_27_(text, key), CRYPT_BYTE_(text, key, 27)
#define CRYPT_BYTES_29_(text, key) CRYPT_BYTES_28_(text, key), CRYPT_BYTE_(text, key, 28)
#define CRYPT_BYTES_30_(text, key) CRYPT_BYTES_29_(text, key), CRYPT_BYTE_(text, key, 29)
#define CRYPT_BYTES_31_(text, key) CRYPT_BYTES_30_(text, key), CRYPT_BYTE_(text, key, 30)
#define CRYPT_BYTES_32_(text, key) CRYPT_BYTES_31_(text, key), CRYPT_BYTE_(text, key, 31)
#define CRYPT_BYTES_33_(text, key) CRYPT_BYTES_32_(text, key), CRYPT_BYTE_(text, key, 32)
#define CRYPT_BYTES_34_(text, key) CRYPT_BYTES_33_(text, key), CRYPT_BYTE_(text, key, 33)
#define CRYPT_BYTES_35_(text, key) CRYPT_BYTES_34_(text, key), CRYPT_BYTE_(text, key, 34)
#define CRYPT_BYTES_36_(text, key) CRYPT_BYTES_35_(text, key), CRYPT_BYTE_(text, key, 35)
#define CRYPT_BYTES_37_(text, key) CRYPT_BYTES_36_(text, key), CRYPT_BYTE_(text, key, 36)
#define CRYPT_BYTES_38_(text, key) CRYPT_BYTES_37_(text, key), CRYPT_BYTE_(text, key, 37)
#define CRYPT_BYTES_39_(text, key) CRYPT_BYTES_38_(text, key), CRYPT_BYTE_(text, key, 38)
#define CRYPT_BYTES_40_(text, key) CRYPT_BYTES_39_(text, key), CRYPT_BYTE_(text, key, 39)
#define CRYPT_BYTES_41_(text, key) CRYPT_BYTES_40_(text, key), CRYPT_BYTE_(text, key, 40)
#define CRYPT_BYTES_42_(text, key) CRYPT_BYTES_41_(text, key), CRYPT_BYTE_(text, key, 41)
#define CRYPT_BYTES_43_(text, key) CRYPT_BYTES_42_(text, key), CRYPT_BYTE_(text, key, 42)
#define CRYPT_BYTES_44_(text, key) CRYPT_BYTES_43_(text, key), CRYPT_BYTE_(text, key, 43)
#define CRYPT_BYTES_45_(text, key) CRYPT_BYTES_44_(text, key), CRYPT_BYTE_(text, key, 44)
#define CRYPT_BYTES_46_(text, key) CRYPT_BYTES_45_(text, key), CRYPT_BYTE_(text, key, 45)
#define CRYPT_BYTES_47_(text, key) CRYPT_BYTES_46_(text, key), CRYPT_BYTE_(text, key, 46)
#define CRYPT_BYTES_48_(text, key) CRYPT_BYTES_47_(text, key), CRYPT_BYTE_(text, key, 47)
#define CRYPT_BYTES_49_(text, key) CRYPT_BYTES_48_(text, key), CRYPT_BYTE_(text, key, 48)
#define CRYPT_BYTES_50_(text, key) CRYPT_BYTES_49_(text, key), CRYPT_BYTE_(text, key, 49)
#define CRYPT_BYTES_51_(text, key) CRYPT_BYTES_50_(text, key), CRYPT_BYTE_(text, key, 50)
#define CRYPT_BYTES_52_(text, key) CRYPT_BYTES_51_(text, key), CRYPT_BYTE_(text, key, 51)
#define CRYPT_BYTES_53_(text, key) CRYPT_BYTES_52_(text, key), CRYPT_BYTE_(text, key, 52)
#define CRYPT_BYTES_54_(text, key) CRYPT_BYTES_53_(text, key), CRYPT_BYTE_(text, key, 53)
#define CRYPT_BYTES_55_(text, key) CRYPT_BYTES_54_(text, key), CRYPT_BYTE_(text, key, 54)
#define CRYPT_BYTES_56_(text, key) CRYPT_BYTES_55_(text, key), CRYPT_BYTE_(text, key, 55)
#define CRYPT_BYTES_57_(text, key) CRYPT_BYTES_56_(text, key), CRYPT_BYTE_(text, key, 56)
#define CRYPT_BYTES_58_(text, key) CRYPT_BYTES_57_(text, key), CRYPT_BYTE_(text, key, 57)
#define CRYPT_BYTES_59_(text, key) CRYPT_BYTES_58_(text, key), CRYPT_BYTE_(text, key, 58)
#define CRYPT_BYTES_60_(text, key) CRYPT_BYTES_59_(text, key), CRYPT_BYTE_(text, key, 59)
#define CRYPT_BYTES_61_(text, key) CRYPT_BYTES_60_(text, key), CRYPT_BYTE_(text, key, 60)
#define CRYPT_BYTES_62_(text, key) CRYPT_BYTES_61_(text, key), CRYPT_BYTE_(text, key, 61)
#define CRYPT_BYTES_63_(text, key) CRYPT_BYTES_62_(text, key), CRYPT_BYTE_(text, key, 62)
#define CRYPT_BYTES_64_(text, key) CRYPT_BYTES_63_(text, key), CRYPT_BYTE_(text, key, 63)
#define CRYPT_BYTES(length, text, key) {CRYPT_BYTES_##length##_(text, key)}

#endif
