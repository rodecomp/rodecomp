#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/CharacterParticleBundle.h"
#include "Contents/Effects/GravityControlEffect.h"
#include "Contents/Effects/SkillEffects.h"

#include <new>

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

// OFFSET: 0x00B253A0
// FUNCTION: fn_18779
void EffectLists::Retire(CEffect* effect, unsigned char removeActive) {
  if (effect) {
    this->retired.push_back(effect);

    if (removeActive == 1 && this->active.find(static_cast<int>(effect->actor)) != this->active.end()) {
      this->active[static_cast<int>(effect->actor)].remove(effect);
    }
  }
}

// OFFSET: 0x00B25120
// FUNCTION: fn_18777
void EffectLists::Retire(int actor, int id) {
  if (CEffect* effect = this->Find(actor, id)) {
    this->retired.push_back(effect);

    if (this->active.find(static_cast<int>(effect->actor)) != this->active.end()) {
      this->active[static_cast<int>(effect->actor)].remove(effect);
    }
  }
}

// OFFSET: 0x00B25240
// FUNCTION: fn_18778
void EffectLists::Retire(int actor, int instance, int id) {
  if (actor > 0 && instance > 0) {
    if (CEffect* effect = this->Find(actor, instance, id)) {
      this->retired.push_back(effect);

      if (this->active.find(static_cast<int>(effect->actor)) != this->active.end()) {
        this->active[static_cast<int>(effect->actor)].remove(effect);
      }
    }
  }
}

// OFFSET: 0x00B24320
// FUNCTION: fn_18772
void EffectLists::RetireActor(int actor) {
  const auto found = this->active.find(actor);

  if (found == this->active.end()) {
    return;
  }

  for (auto effect = found->second.begin(); effect != found->second.end(); ++effect) {
    this->retired.push_back(*effect);
  }

  found->second.clear();
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
  case EF_EMITTER:
  case EF_MAGIC_FLOOR:
  case EF_EVILS_PAW:
  case EF_ANIMATED_EMITTER:
  case EF_NEW_RANGEDATTACK:
  case EF_UNK_2343:
    this->indexed[effect->id].push_back(effect);
    break;
  }
}

// OFFSET: 0x00B24A50
// FUNCTION: fn_18774
CEffect* EffectLists::Spawn(int actor, int id, float x, float y, float z, std::uint32_t sourceActor, int frame, unsigned char reuse, int instance) {
  CEffect* effect = nullptr;

  if (id < 0 || id > 60000) {
    switch (id) {
    case EF_READYPORTAL:
    case EF_PORTAL:
    case EF_LOCKON:
    case EF_BOTTOM:
    case EF_BOTTOM2:
    case EF_FIREPILLARON:
    case EF_REFINEOK:
    case EF_REFINEFAIL:
    case EF_TOPRANK:
    case EF_PARTY:
    case EF_STATUS_STATE:
    case EF_TALK_FROSTJOKE:
    case EF_TALK_SCREAM:
    case EF_BLIND:
    case EF_ANGEL2:
    case EF_MINI_TETRIS:
    case EF_ANGEL:
    case EF_REFLECTBODY:
    case EF_ASURABODY:
    case EF_QUAKEBODY:
    case EF_JUMPBODY:
    case EF_LANDBODY:
    case EF_TAE_READY:
    case EF_DEVIL1:
    case EF_DEVIL2:
    case EF_DEVIL3:
    case EF_DEVIL4:
    case EF_DEVIL5:
    case EF_DEVIL6:
    case EF_DEVIL7:
    case EF_DEVIL8:
    case EF_DEVIL9:
    case EF_DEVIL10:
    case EF_BLUEBODY:
    case EF_REDLIGHTBODY:
    case EF_RED_HIT:
    case EF_BLUE_HIT:
    case EF_ANGEL3:
    case EF_UNDEADBODY:
    case EF_UNDEADBODY_DEL:
    case EF_FIREPILLARON2:
    case EF_JUMPBODY1:
    case EF_UNK_1398:
      break;
    default:
      return nullptr;
    }
  } else if (id == EF_LEVEL150 || id == EF_LEVEL150_SUB) {
    if (this->Find(actor, EF_LEVEL99)) {
      return nullptr;
    }
  }

  if (actor) {
    effect = this->Find(actor, instance, id);
    if (effect) {
      if (instance > 0) {
        return nullptr;
      }
      if (effect->retained || reuse) {
        return effect;
      }
    }
  }

  const auto allocator = this->allocators.find(id);
  if (allocator == this->allocators.end()) {
    return nullptr;
  }

  effect = allocator->second->Create();
  if (effect) {
    effect->SetActor(actor);
    effect->SetSourceActor(sourceActor);
    effect->position[0] = x;
    effect->position[1] = y;
    effect->position[2] = z;
    effect->Slot29(id);
    if (frame != -1) {
      effect->unknown_00B0 = frame;
    }
    if (instance > 0) {
      effect->unknown_00C4 = instance;
      effect->retained = 1;
    }
    effect->Slot01();
    effect->Slot12();
    this->Add(actor, effect);
  }

  return effect;
}

// OFFSET: 0x00B254C0
// FUNCTION: fn_18780
void EffectLists::Clear() {
  for (auto entry = this->active.begin(); entry != this->active.end(); ++entry) {
    this->RetireActor(entry->first);
  }

  this->active.clear();
  this->RetireActor(0);
  this->indexed.clear();

  for (auto effect = this->retired.begin(); effect != this->retired.end(); ++effect) {
    (*effect)->Close();
    delete *effect;
  }

  this->retired.clear();
}

// OFFSET: 0x00B255B0
// FUNCTION: fn_18781
void EffectLists::Release() {
  for (auto entry = this->allocators.begin(); entry != this->allocators.end(); ++entry) {
    delete entry->second;
  }

  this->allocators.clear();
  this->Clear();
}

// OFFSET: 0x00B1D530
// FUNCTION: fn_45453
EffectLists::~EffectLists() {
  this->Release();
}

// OFFSET: 0x00B25730
// FUNCTION: fn_18782
void DestroyEffectMgr() {
  if (g_effectMgr) {
    delete g_effectMgr;
    g_effectMgr = nullptr;
  }
}

namespace {

template <class T>
struct CEffectAllocator : CAllocator {
  CEffect* Create() override;
};

// OFFSET: 0x00B1F010
// FUNCTION: fn_18604
template <>
CEffect* CEffectAllocator<CGravityControlEffect>::Create() {
  return new (std::nothrow) CGravityControlEffect();
}

// OFFSET: 0x00B1F740
// FUNCTION: fn_18618
template <>
CEffect* CEffectAllocator<CMeteorEffect>::Create() {
  return new (std::nothrow) CMeteorEffect();
}

// OFFSET: 0x00B20110
// FUNCTION: fn_18637
template <>
CEffect* CEffectAllocator<CShadowExceedNew>::Create() {
  return new (std::nothrow) CShadowExceedNew();
}

// OFFSET: 0x00B20400
// FUNCTION: fn_18642
template <>
CEffect* CEffectAllocator<CSprArrowEffect>::Create() {
  return new (std::nothrow) CSprArrowEffect();
}

// OFFSET: 0x00B1E1D0
// FUNCTION: fn_18576
template <>
CEffect* CEffectAllocator<CChopChopClawMarks>::Create() {
  return new (std::nothrow) CChopChopClawMarks();
}

template struct CEffectAllocator<CChopChopClawMarks>;
template struct CEffectAllocator<CGravityControlEffect>;
template struct CEffectAllocator<CMeteorEffect>;
template struct CEffectAllocator<CShadowExceedNew>;
template struct CEffectAllocator<CSprArrowEffect>;

}

CGravityControlEffect::~CGravityControlEffect() {
}

CMeteorEffect::~CMeteorEffect() {
}

CShadowExceedNew::~CShadowExceedNew() {
}

CSprArrowEffect::~CSprArrowEffect() {
}

CChopChopClawMarks::~CChopChopClawMarks() {
}
