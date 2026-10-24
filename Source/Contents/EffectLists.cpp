#include "Contents/EffectLists.h"

#include <bit>

// OFFSET: 0x00B25750
// FUNCTION: fn_18783
CEffect* EffectLists::Find(int actor, int id) const {
  const auto found = this->active.find(actor);

  if (found != this->active.end()) {
    for (CEffect* effect : found->second) {
      if (effect->id == id) {
        return effect;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00B257C0
// FUNCTION: fn_18784
CEffect* EffectLists::Find(int actor, int instance, int id) const {
  const auto found = this->active.find(actor);

  if (found != this->active.end()) {
    for (CEffect* effect : found->second) {
      if (effect->id == id && effect->unknown_00C4 == instance) {
        return effect;
      }
    }
  }

  return nullptr;
}

// INLINE
void EffectLists::RemoveActive(CEffect* effect) {
  const auto found = this->active.find(std::bit_cast<int>(effect->actor));

  if (found != this->active.end()) {
    found->second.remove(effect);
  }
}

// OFFSET: 0x00B253A0
// FUNCTION: fn_18779
void EffectLists::Retire(CEffect* effect, unsigned char removeActive) {
  if (effect) {
    this->retirement.effects.push_back(effect);

    if (removeActive == 1) {
      this->RemoveActive(effect);
    }
  }
}

// OFFSET: 0x00B25120
// FUNCTION: fn_18777
void EffectLists::Retire(int actor, int id) {
  if (CEffect* effect = this->Find(actor, id)) {
    this->retirement.effects.push_back(effect);
    this->RemoveActive(effect);
  }
}

// OFFSET: 0x00B25240
// FUNCTION: fn_18778
void EffectLists::Retire(int actor, int instance, int id) {
  if (actor > 0 && instance > 0) {
    if (CEffect* effect = this->Find(actor, instance, id)) {
      this->retirement.effects.push_back(effect);
      this->RemoveActive(effect);
    }
  }
}

// OFFSET: 0x00B24320
// FUNCTION: fn_18772
void EffectLists::RetireActor(int actor) {
  const auto found = this->active.find(actor);

  if (found != this->active.end()) {
    for (CEffect* effect : found->second) {
      this->retirement.effects.push_back(effect);
    }

    found->second.clear();
  }
}

// OFFSET: 0x00B25830
// FUNCTION: fn_18785
CEffect* EffectLists::At(int id, unsigned index) const {
  const auto found = this->indexed.find(id);

  if (found == this->indexed.end() || index >= found->second.size()) {
    return nullptr;
  }

  return found->second[index];
}

// OFFSET: 0x00B24190
// FUNCTION: fn_18771
void EffectLists::Add(int actor, CEffect* effect) {
  this->active[actor].push_back(effect);

  switch (effect->id) {
  case 974:
  case 1025:
  case 1039:
  case 1073:
  case 1412:
  case 2343:
    this->indexed[effect->id].push_back(effect);
    break;
  }
}

// OFFSET: 0x00B254C0
// FUNCTION: fn_18780
void EffectLists::Clear() {
  for (auto& [actor, effects] : this->active) {
    this->RetireActor(actor);
  }

  this->active.clear();
  this->RetireActor(0);
  this->indexed.clear();
  this->retirement.Clear();
}
