#include "Archive/PakName.h"

#include <cstdlib>
#include <cstring>

namespace myGPack {
  // OFFSET: 0x00AECE00
  // FUNCTION: fn_18145
  PakName::PakName() {
    this->Assign("");
  }

  // FOLDED: fn_18144
  PakName::PakName(const char* name) {
    this->Assign(name);
  }

  // OFFSET: 0x00AECF90
  // FUNCTION: fn_18149
  void PakName::Assign(const char* name) {
    std::strcpy(this->m_name, name);
    _strlwr(this->m_name);
    const std::size_t length = std::strlen(this->m_name);
    std::uint32_t hash = 5381;

    for (std::size_t i = 0; i < length; ++i) {
      hash = 33 * hash + static_cast<std::int8_t>(this->m_name[i]);
    }

    this->m_hash = hash;
  }

  // OFFSET: 0x00AECE20
  // FUNCTION: fn_18146
  bool PakName::operator==(const PakName& other) const {
    if (this->m_hash != other.m_hash) {
      return false;
    }

    return std::strcmp(this->m_name, other.m_name) == 0;
  }

  // OFFSET: 0x00AECE80
  // FUNCTION: fn_18147
  bool PakName::operator!=(const PakName& other) const {
    if (this->m_hash != other.m_hash) {
      return true;
    }

    return std::strcmp(this->m_name, other.m_name) != 0;
  }

  // OFFSET: 0x00AECEE0
  // FUNCTION: fn_18148
  bool PakName::operator<(const PakName& other) const {
    if (this->m_hash != other.m_hash) {
      return this->m_hash < other.m_hash;
    }

    return std::strcmp(this->m_name, other.m_name) < 0;
  }
}
