#include "Contents/InstantVar.h"

#include "Contents/Actors/RenderObject.h"

template __declspec(noinline) std::_Tree<std::_Tmap_traits<int, std::int64_t, std::less<int>, std::allocator<std::pair<const int, std::int64_t>>, false>>::~_Tree() noexcept;

// OFFSET: 0x00CA8EE0
// FUNCTION: fn_20997
CInstantVar::~CInstantVar() = default;

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
