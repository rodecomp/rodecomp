#ifndef RAGNAROK_SOURCE_UTILITY_CIPHER_OBFUSCATE_H
#define RAGNAROK_SOURCE_UTILITY_CIPHER_OBFUSCATE_H

#include <array>
#include <cstddef>
#include <cstdint>

namespace utility {

  template <std::size_t Length, std::uint64_t Key>
  class Obfuscator {
  public:
    // INLINED
    consteval explicit Obfuscator(const char (&text)[Length]) : m_data{} {
      for (std::size_t index = 0; index < Length; ++index) {
        this->m_data[index] = static_cast<char>(text[index] ^ static_cast<char>(Key >> ((index % 8) * 8)));
      }
    }

    // INLINED
    template <typename Element>
    consteval std::array<Element, Length> Array() const {
      std::array<Element, Length> out{};
      for (std::size_t index = 0; index < Length; ++index) {
        out[index] = static_cast<Element>(this->m_data[index]);
      }
      return out;
    }

    char m_data[Length];
  };

}

#define OBFUSCATE_NAME_(text, key, variable) \
  constexpr ::utility::Obfuscator<sizeof(text), key> obfuscator(text); \
  thread_local EncryptedName<sizeof(text), key> variable(obfuscator.m_data)

#define OBFUSCATE_NAME_SOURCE_(text, key, variable) \
  constexpr ::utility::Obfuscator<sizeof(text), key> obfuscator(text); \
  constexpr EncryptedSource<sizeof(text)> source(obfuscator.m_data); \
  thread_local EncryptedName<sizeof(text), key> variable(source)

#define OBFUSCATE_NAME_ARRAY_(text, key, element, variable) \
  constexpr std::array<element, sizeof(text)> source = ::utility::Obfuscator<sizeof(text), key>(text).Array<element>(); \
  thread_local EncryptedName<sizeof(text), key> variable(source)

#define OBFUSCATE_DECLARE(text, key, variable) \
  constexpr ::utility::Obfuscator<sizeof(text), key> variable##Obfuscator(text); \
  thread_local EncryptedName<sizeof(text), key> variable(variable##Obfuscator.m_data)

#define OBFUSCATE_DECLARE_BYTES(text, key, variable) \
  constexpr std::array<std::uint8_t, sizeof(text)> variable##Source = ::utility::Obfuscator<sizeof(text), key>(text).Array<std::uint8_t>(); \
  thread_local EncryptedName<sizeof(text), key> variable(variable##Source._Elems)

#define OBFUSCATE_DECLARE_ARRAY(text, key, variable) \
  constexpr std::array<char, sizeof(text)> variable##Source = ::utility::Obfuscator<sizeof(text), key>(text).Array<char>(); \
  thread_local EncryptedName<sizeof(text), key> variable(variable##Source)

#define OBFUSCATE_DECLARE_BYTES_ARRAY(text, key, variable) \
  constexpr std::array<std::uint8_t, sizeof(text)> variable##Source = ::utility::Obfuscator<sizeof(text), key>(text).Array<std::uint8_t>(); \
  thread_local EncryptedName<sizeof(text), key> variable(variable##Source)

#define OBFUSCATE_SOURCE(text, key) CRYPT_NAME(text, key)
#define OBFUSCATE_SOURCE_CHARS(length, text, key) CRYPT_CHARS(length, text, key)
#define OBFUSCATE_SOURCE_BYTES(length, text, key) CRYPT_BYTES(length, text, key)

#define OBFUSCATE_DECRYPT(key, source) \
  [](const auto& bytes) { \
    thread_local EncryptedName<sizeof(bytes), key> name(bytes); \
    return name.Text(); \
  }(source)

#define OBFUSCATE_DECRYPT_NAME(key, source) \
  [](const auto& bytes) -> EncryptedName<sizeof(bytes), key>& { \
    thread_local EncryptedName<sizeof(bytes), key> name(bytes); \
    return name; \
  }(source)

#define OBFUSCATE_DECRYPT_CHARS(key, length, ...) \
  [](const char (&bytes)[length]) { \
    thread_local EncryptedName<length, key> name(bytes); \
    return name.Text(); \
  }(__VA_ARGS__)

#define OBFUSCATE_DECRYPT_BYTES(key, length, ...) \
  [](const std::uint8_t (&bytes)[length]) { \
    thread_local EncryptedName<length, key> name(bytes); \
    return name.Text(); \
  }(__VA_ARGS__)

#define OBFUSCATE_ARRAY(text, key, element) \
  []() -> EncryptedName<sizeof(text), key>& { \
    OBFUSCATE_NAME_ARRAY_(text, key, element, name); \
    return name; \
  }()

#define OBFUSCATE_ARRAY_TEXT(text, key, element) \
  []() { \
    OBFUSCATE_NAME_ARRAY_(text, key, element, name); \
    return name.Text(); \
  }()

#define OBFUSCATE_BYTES_TEXT(text, key) OBFUSCATE_ARRAY_TEXT(text, key, std::uint8_t)

#define OBFUSCATE_ARRAY_BODY(text, key, element) \
  OBFUSCATE_NAME_ARRAY_(text, key, element, name); \
  return name

#define OBFUSCATE_STRING_BODY(text, key) \
  OBFUSCATE_NAME_(text, key, name); \
  return name

#define OBFUSCATE_STRING(text, key) \
  []() -> EncryptedName<sizeof(text), key>& { \
    OBFUSCATE_NAME_(text, key, name); \
    return name; \
  }()

#define OBFUSCATE_TEXT_NAMED(text, key, variable) \
  []() { \
    OBFUSCATE_NAME_(text, key, variable); \
    return variable.Text(); \
  }()

#define OBFUSCATE_SOURCE_TEXT_NAMED(text, key, variable) \
  []() { \
    OBFUSCATE_NAME_SOURCE_(text, key, variable); \
    return variable.Text(); \
  }()

#define OBFUSCATE_TEXT(text, key) OBFUSCATE_TEXT_NAMED(text, key, name)
#define OBFUSCATE_SOURCE_TEXT(text, key) OBFUSCATE_SOURCE_TEXT_NAMED(text, key, name)

#endif
