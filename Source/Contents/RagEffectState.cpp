#include "Contents/EffectActor.h"
#include "Contents/EffectPrimitiveState.h"
#include "Contents/RagEffectState.h"
#include "Resource/Resource.h"

#include <cstring>

// OFFSET: 0x00BF84B0
// FUNCTION: fn_20007
// Render-base resources are initialized first by the enclosing object, not duplicated here.
RagEffectState::RagEffectState()
    : parent(nullptr), unknown_0160(0), unknown_0164(0), id(0), unknown_016C(1), unknown_0170(0), unknown_0174(0),
      unknown_0178(0), offset{}, unknown_018C{}, resource(nullptr), resourceData(nullptr), unknown_119F4(0),
      unknown_11C7C(0), unknown_11C80(0), unknown_11C84(nullptr), unknown_11C88{-1, -1, -1, -1}, forceRender(0), tag(0),
      unknown_11CA0(0) {
  for (unsigned index = 0; index < 20; ++index) {
    auto* record = this->unknown_01AC + index * 80;
    const std::uint32_t zero = 0;

    for (unsigned field = 0; field < 6; ++field) {
      std::memcpy(record + field * 4, &zero, sizeof(zero));
    }
  }

  std::memset(this->unknown_11A7C, 0, sizeof(this->unknown_11A7C));
  // Original constructs the three vertices in each record, then clears the
  // entire array, overriding those initial vertex colors as well.
  std::memset(this->vertices, 0, sizeof(this->vertices));
}

// OFFSET: 0x00C0AED0
// FUNCTION: fn_20015
RagEffectState::~RagEffectState() {
  for (EffectPrimitiveState* particle : this->related) {
    particle->parent = nullptr;
  }

  this->related.clear();

  if (this->parent) {
    this->parent->state.Attachments().Unlink(this);
  }

  if (this->resource) {
    this->resource->Release();
  }
}
