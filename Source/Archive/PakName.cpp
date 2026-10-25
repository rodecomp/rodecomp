#include "Archive/PakName.h"

#include <cstdlib>
#include <cstring>

namespace myGPack {
  // OFFSET: 0x00AECE00
  // FUNCTION: fn_18145
  PakName::PakName() {
    this->Assign("");
  }

  // OFFSET: 0x00AECF90
  // FUNCTION: fn_18149
  PakName& PakName::Assign(const char* name) {
    AssignNameKey(this->m_hash, this->m_name, name);
    return *this;
  }

  void AssignNameKey(std::uint32_t& result, char* destination, const char* source) {
    std::strcpy(destination, source);
    _strlwr(destination);
    const std::size_t length = std::strlen(destination);
    std::uint32_t hash = 5381;

    for (std::size_t i = 0; i < length; ++i) {
      hash = 33 * hash + static_cast<std::int8_t>(destination[i]);
    }

    result = hash;
  }

  // OFFSET: 0x00AECE20
  // FUNCTION: fn_18146
  bool PakName::operator==(const PakName& other) const {
    return this->m_hash == other.m_hash && std::strcmp(this->m_name, other.m_name) == 0;
  }

  // OFFSET: 0x00AECE80
  // FUNCTION: fn_18147
  bool PakName::operator!=(const PakName& other) const {
    return this->m_hash != other.m_hash || std::strcmp(this->m_name, other.m_name) != 0;
  }

  // OFFSET: 0x00AECEE0
  // FUNCTION: fn_18148
  bool PakName::operator<(const PakName& other) const {
    if (this->m_hash == other.m_hash) {
      return std::strcmp(this->m_name, other.m_name) < 0;
    }

    return this->m_hash < other.m_hash;
  }
}
