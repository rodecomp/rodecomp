#include "Utility/Async/NObject.h"

namespace {
  // STATIC: 0x0138D3F0
  // GLOBAL: gl_24362
  std::uint32_t s_nextObjectId;
}

// OFFSET: 0x007369B0
// FUNCTION: fn_07628
NObject::NObject() : m_objectId(++s_nextObjectId) {
}

// OFFSET: 0x007369D0
// OFFSET: 0x007369E0
// FUNCTION: fn_07629
NObject::~NObject() = default;
