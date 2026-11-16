#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/RagEffect.h"

// OFFSET: 0x00CADBE0
// FUNCTION: fn_21062
bool EffectActor::HasId(int id) const {
  for (auto it = this->effectList.begin(); it != this->effectList.end(); ++it) {
    if (*it && (*it)->state.type == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CADC40
// FUNCTION: fn_21064
bool EffectActor::Contains(const CRagEffect* effect) const {
  if (this->effectList.size() != 0) {
    for (const CRagEffect* current : this->effectList) {
      if (current == effect) {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x00CADC20
// FUNCTION: fn_21063
bool EffectActor::EffectVisible() const {
  return this->render.visible != 0 && this->render.enabled != 0;
}
