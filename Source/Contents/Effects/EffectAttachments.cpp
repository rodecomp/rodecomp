#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/RagEffectState.h"

// OFFSET: 0x00CADBE0
// FUNCTION: fn_21062
bool EffectActor::HasId(int id) const {
  for (const RagEffectState* effect : this->state.effects) {
    if (effect && effect->type == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CADC40
// FUNCTION: fn_21064
bool EffectActor::Contains(const RagEffectState* effect) const {
  for (const RagEffectState* current : this->state.effects) {
    if (current == effect) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CADC20
// FUNCTION: fn_21063
bool EffectActor::EffectVisible() const {
  return this->render.visible != 0 && this->render.enabled != 0;
}
