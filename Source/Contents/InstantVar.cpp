#include "Contents/InstantVar.h"

#include "Contents/Actors/RenderObject.h"

// OFFSET: 0x00CA8EE0
// FUNCTION: fn_20997
CInstantVar::~CInstantVar() = default;

// OFFSET: 0x00D559B0
// FUNCTION: fn_21952
void CInstantVar::Set(int key, std::int64_t value) {
  this->m_values[key] = value;
}

std::int64_t CInstantVar::Get(int key) const {
  const auto entry = this->m_values.find(key);
  return entry == this->m_values.end() ? 0 : entry->second;
}

// OFFSET: 0x00612020
// FUNCTION: fn_04340
std::int64_t CRenderObject::GetInstant(int key) const {
  return this->render.instant.Get(key);
}
