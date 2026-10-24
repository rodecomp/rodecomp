#include "Contents/InstantVar.h"

// OFFSET: 0x00CA8EE0
// FUNCTION: fn_20997
// OFFSET: 0x00CA9260
// FUNCTION: fn_21003
CInstantVar::~CInstantVar() = default;

// OFFSET: 0x00D559B0
// FUNCTION: fn_21952
void CInstantVar::Set(int key, std::int64_t value) {
  this->m_values[key] = value;
}

// OFFSET: 0x00612020
// FUNCTION: fn_04340
// OFFSET: 0x0091BE20
// FUNCTION: fn_12610
std::int64_t CInstantVar::Get(int key) const {
  const auto entry = this->m_values.find(key);
  return entry == this->m_values.end() ? 0 : entry->second;
}

// ORIGINAL_SITE: 0x00CAFB5E in fn_21086 (fragment)
// ORIGINAL_SITE: 0x00DC4701 in fn_22763 (fragment)
void CInstantVar::Clear() {
  this->m_values.clear();
}
