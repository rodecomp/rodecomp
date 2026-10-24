#include "Contents/ServiceOptions.h"

// OFFSET: 0x006C17A0
// FUNCTION: fn_06318
unsigned char ServiceOptions::Get(int id) const {
  const auto entry = this->values.find(static_cast<std::uint32_t>(id));
  return entry == this->values.end() ? 0 : entry->second;
}

// OFFSET: 0x006C1820
// FUNCTION: fn_06319
bool IsServiceOption(int id) {
  switch (id) {
  case 106:
  case 107:
  case 108:
  case 231:
  case 269:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x006C2A80
// FUNCTION: fn_06325
bool ServiceOptions::Set(int id, unsigned char value) {
  const auto entry = this->values.find(static_cast<std::uint32_t>(id));

  if (entry == this->values.end()) {
    return false;
  }

  entry->second = value;
  return true;
}
