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

// OFFSET: 0x00CAF4B0
// FUNCTION: fn_21083
void EffectActor::ResetEffectState() {
  this->state.unknown_0138 = -1;
  this->state.unknown_013C = 0;
  this->state.unknown_0140 = 0;
  this->state.first = nullptr;
  this->state.second = nullptr;
  this->render.unknown_0130[0] = 0;
}
