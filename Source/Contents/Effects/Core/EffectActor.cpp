#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/RagEffect.h"

// INLINED
EffectActorState::EffectActorState() : padding_0138(-1), padding_013C(0), effectLaunched(0), first(nullptr), second(nullptr) {
}

// INLINED
EffectActorState::~EffectActorState() {
}

// OFFSET: 0x00CA82E0
// FUNCTION: fn_20987
EffectActor::EffectActor() {
  this->render.translucentOther[0] = 0;
}

// OFFSET: 0x00CA8C40
// FUNCTION: fn_20993
EffectActor::~EffectActor() {
  if (!this->state.effects.empty()) {
    for (std::list<CRagEffect*>::iterator it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
      if (*it) {
        CRagEffect* effect = *it;
        effect->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
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
  this->state.padding_0138 = -1;
  this->state.padding_013C = 0;
  this->state.effectLaunched = 0;
  this->state.first = nullptr;
  this->state.second = nullptr;
  this->render.translucentOther[0] = 0;
}
