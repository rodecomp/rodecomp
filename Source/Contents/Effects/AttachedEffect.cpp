#include "Contents/Effects/AttachedEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Resource/Resource.h"

#include <cstring>

// INLINED
inline RagEffectState::RagEffectState()
    : parent(nullptr), unknown_0160(0), unknown_0164(0), id(0), unknown_016C(1), unknown_0170(0), unknown_0174(0), unknown_0178(0), offset{}, unknown_018C{},
      resource(nullptr), resourceData(nullptr), unknown_119F4(0), unknown_11C7C(0), unknown_11C80(0), unknown_11C84(nullptr), unknown_11C88{-1, -1, -1, -1},
      forceRender(0), tag(0), unknown_11CA0(0) {
  for (unsigned index = 0; index < 20; ++index) {
    auto* record = this->unknown_01AC + index * 80;
    const std::uint32_t zero = 0;

    for (unsigned field = 0; field < 6; ++field) {
      std::memcpy(record + field * 4, &zero, sizeof(zero));
    }
  }

  std::memset(this->unknown_11A7C, 0, sizeof(this->unknown_11A7C));

  std::memset(this->vertices, 0, sizeof(this->vertices));
}

// INLINED
inline RagEffectState::~RagEffectState() {
  for (EffectPrimitiveState* particle : this->related) {
    particle->master = nullptr;
  }

  this->related.clear();

  if (this->parent) {
    this->parent->state.Attachments().Unlink(this);
  }

  if (this->resource) {
    this->resource->Release();
  }
}

// OFFSET: 0x00BF84B0
// FUNCTION: fn_20007
AttachedEffect::AttachedEffect(ResourceMgr& resources, JobNames& names) : CRenderObject(resources, names) {
}

AttachedEffect::~AttachedEffect() = default;
