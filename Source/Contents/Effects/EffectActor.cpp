#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/RagEffect.h"

// INLINED
EffectActorState::EffectActorState() : unknown_0138(-1), unknown_013C(0), unknown_0140(0), first(nullptr), second(nullptr) {
}

// INLINED
EffectActorState::~EffectActorState() {
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

struct CAbleToMakeEffect : EffectActor {
  ~CAbleToMakeEffect() override;
};

// OFFSET: 0x00CA8C40
// FUNCTION: fn_20993
CAbleToMakeEffect::~CAbleToMakeEffect() {
  if (!this->state.effects.empty()) {
    for (std::list<RagEffectState*>::iterator it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
      if (*it) {
        CRagEffect* effect = reinterpret_cast<CRagEffect*>(*it);
        effect->SendMsg(this, 0x18, 0, 0, 0, 0, 0, 0);
      }
    }
    this->state.effects.erase(this->state.effects.begin(), this->state.effects.end());
  }
  this->state.first = nullptr;
  this->state.second = nullptr;
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
