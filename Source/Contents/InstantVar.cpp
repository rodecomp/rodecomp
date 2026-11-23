#include "Contents/InstantVar.h"

#include "Contents/Actors/RenderObject.h"

// OFFSET: 0x00D559B0
// FUNCTION: fn_21952
void CInstantVar::Set(int key, std::int64_t value) {
  this->m_values[key] = value;
}

// OFFSET: 0x00612020
// FUNCTION: fn_04340
int CRenderObject::GetInstant(int key) const {
  return static_cast<int>(this->render.instant.Get(key));
}
