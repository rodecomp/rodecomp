#include "Contents/EffectActor.h"

// OFFSET: 0x00CA82E0
EffectActorState::EffectActorState()
    : unknown_0138(-1), unknown_013C(0), unknown_0140(0), first(nullptr), second(nullptr) {
}

// OFFSET: 0x00CA8C40
EffectActorState::~EffectActorState() {
  this->Attachments().DetachAll();
}

EffectAttachments EffectActorState::Attachments() {
  return {this->effects, this->first, this->second};
}

// OFFSET: 0x00CA82E0
EffectActor::EffectActor(ResourceMgr& resources, JobNames& names) : CRenderObject(resources, names) {
  // Original full-object byte +0x130, inside the render-base tail.
  this->render.unknown_0130[0] = 0;
}
