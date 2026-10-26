#include "Contents/Effects/EffectAttachments.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/RagEffectState.h"

// OFFSET: 0x00CADBE0
// FUNCTION: fn_21062
bool EffectAttachments::HasId(int id) const {
  for (const RagEffectState* effect : this->effects) {
    if (effect && effect->id == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00CADC40
// FUNCTION: fn_21064
bool EffectAttachments::Contains(const RagEffectState* effect) const {
  for (const RagEffectState* current : this->effects) {
    if (current == effect) {
      return true;
    }
  }

  return false;
}

// INLINED
void EffectAttachments::Unlink(RagEffectState* effect) {
  if (!effect) {
    return;
  }

  for (auto entry = this->effects.begin(); entry != this->effects.end(); ++entry) {
    if (*entry == effect) {
      this->effects.erase(entry);

      if (this->first == effect) {
        this->first = nullptr;
      } else if (this->second == effect) {
        this->second = nullptr;
      }

      return;
    }
  }
}

// INLINED
void EffectAttachments::DetachAll() {
  for (RagEffectState* effect : this->effects) {
    if (effect) {
      effect->parent = nullptr;
    }
  }

  this->effects.clear();
  this->first = nullptr;
  this->second = nullptr;
}

// OFFSET: 0x00CADC20
// FUNCTION: fn_21063
bool EffectActor::EffectVisible() const {
  return this->render.visible != 0 && this->render.enabled != 0;
}
