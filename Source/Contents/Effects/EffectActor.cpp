#include "Contents/Effects/EffectActor.h"

// INLINED
EffectActorState::EffectActorState() : unknown_0138(-1), unknown_013C(0), unknown_0140(0), first(nullptr), second(nullptr) {
}

// INLINED
EffectActorState::~EffectActorState() {
  this->Attachments().DetachAll();
}

// INLINED
EffectAttachments EffectActorState::Attachments() {
  return {this->effects, this->first, this->second};
}

// OFFSET: 0x00CA82E0
// FUNCTION: fn_20987
EffectActor::EffectActor() {
  this->render.unknown_0130[0] = 0;
}
