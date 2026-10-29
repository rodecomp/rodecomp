#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/World/World.h"

#include <cctype>
#include <new>
#include <string>

// GLOBAL: gl_24485
template <>
CEZ2STREffectMgr* Singleton<CEZ2STREffectMgr>::s_instance = nullptr;

// OFFSET: 0x005A2090
// FUNCTION: fn_03082
void EffectResourceSwitcher::RetireEffects() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  if (!this->sourceName.empty()) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName));
  }

  if (!this->targetName.empty()) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->targetName));
  }
}

// OFFSET: 0x005A2110
// FUNCTION: fn_03083
void CEffect::Slot10(int) {
}

// OFFSET: 0x005A2120
// FUNCTION: fn_03084
void CEffect::Slot08(int, int, int) {
}

// OFFSET: 0x005A2130
// FUNCTION: fn_03085
void CEffect::Slot11(std::string) {
}

// OFFSET: 0x005A2170
// FUNCTION: fn_03089
vector3d CEffect::Slot25() {
  return vector3d{0.0f, 0.0f, 0.0f};
}

// OFFSET: 0x005A2190
// FUNCTION: fn_03090
int CEffect::Slot28() {
  return 0;
}

// OFFSET: 0x005A21A0
// FUNCTION: fn_03091
void EffectResourceSwitcher::Start() {
  if (this->effect) {
    this->active = 1;
    this->effect->EndLoop(1);
  }
}

// OFFSET: 0x005A21C0
// FUNCTION: fn_03092
void CEffect::Slot01() {
}

// OFFSET: 0x005A21D0
// FUNCTION: fn_03093
void EffectResourceSwitcher::SetNames(const std::string& source, const std::string& target) {
  if (source.empty() || target.empty()) {
    return;
  }

  this->sourceName = source;
  this->targetName = target;

  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  this->effect = static_cast<CEZ2STREffect*>(
      g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
  this->spawnPosition = actor->render.position;
}

// OFFSET: 0x005A22E0
// FUNCTION: fn_03094
void CEffect::Slot12() {
}

// OFFSET: 0x005A22F0
// FUNCTION: fn_03095
void CEffect::Slot07() {
}

// OFFSET: 0x005A2300
// FUNCTION: fn_03096
bool EffectResourceSwitcher::Slot06() {
  if (!this->effect) {
    return false;
  }

  if (!this->effect->retained) {
    return !this->effect->IsFinished();
  }

  return true;
}

// OFFSET: 0x005A2330
// FUNCTION: fn_03097
void EffectResourceSwitcher::Slot03() {
  if (!this->effect || !this->active || !this->effect->IsFinished()) {
    return;
  }

  CEZ2STREffect* previous = this->effect;
  this->sourceName = this->targetName;
  this->targetName = "";
  this->effect = static_cast<CEZ2STREffect*>(g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId(this->sourceName), this->spawnPosition.x,
                                                                this->spawnPosition.y, this->spawnPosition.z, 0, -1, 0, 0));
  this->active = 0;
  if (previous) {
    previous->EndLoop(0);
  }
}

// OFFSET: 0x005A2430
// FUNCTION: fn_03098
void CEffect::Slot04() {
}

// OFFSET: 0x005A2440
// FUNCTION: fn_03099
void CEffect::Slot00(int) {
}

// OFFSET: 0x005A2450
// FUNCTION: fn_03100
void CEffect::Slot23(float) {
}

// OFFSET: 0x005A2460
// FUNCTION: fn_03101
void CEffect::Slot24(int, int, int, int, int) {
}

// OFFSET: 0x005A2470
// FUNCTION: fn_03102
void CEffect::Slot26(int) {
}

// OFFSET: 0x005A2480
// FUNCTION: fn_03103
void CEffect::Slot27(int) {
}

// OFFSET: 0x005A2490
// FUNCTION: fn_03104
void CEffect::Slot21(int) {
}

// OFFSET: 0x005A24A0
// FUNCTION: fn_03105
void CEffect::Slot32() {
}

// OFFSET: 0x005A24B0
// FUNCTION: fn_03106
void CEffect::Slot15(int) {
}

// OFFSET: 0x005A24C0
// FUNCTION: fn_03107
void CEffect::Slot14(int) {
}

// OFFSET: 0x005A24D0
// FUNCTION: fn_03108
void CEffect::Slot17(float) {
}

// OFFSET: 0x005A24E0
// FUNCTION: fn_03109
void CEffect::Slot18(float) {
}

// OFFSET: 0x005A24F0
// FUNCTION: fn_03110
void CEffect::Slot19(float) {
}

// OFFSET: 0x005A2500
// FUNCTION: fn_03111
void CEffect::Slot22(float) {
}

// OFFSET: 0x005A2510
// FUNCTION: fn_03112
void CEffect::Slot13(int) {
}

// OFFSET: 0x005A2520
// FUNCTION: fn_03113
void EffectResourceSwitcher::Slot16(int argument) {
  if (this->effect) {
    this->effect->Slot16(argument);
  }
}

// OFFSET: 0x005A2540
// FUNCTION: fn_03114
void CEffect::Slot20(int) {
}

// OFFSET: 0x005A2630
// FUNCTION: fn_03115
template <>
CEZ2STREffectMgr* Singleton<CEZ2STREffectMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CEZ2STREffectMgr;
  }

  return s_instance;
}

// OFFSET: 0x006FFA00
// FUNCTION: fn_06967
bool CEZ2STREffect::IsFinished() {
  if (!this->emitter) {
    return false;
  }

  return this->emitter->currentFrame >= this->emitter->frameCount;
}

// OFFSET: 0x006FFBF0
// FUNCTION: fn_06969
bool CEZ2STREffect::Slot06() {
  EZ2STREmitter* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (this->timed == 1) {
    return emitter->remainingTime > 0.0f;
  }

  if (emitter->looping == 1) {
    return CEffect::Slot06();
  }

  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f;
}

// OFFSET: 0x006FFEF0
// FUNCTION: fn_06972
void CEZ2STREffect::Slot23(float argument) {
  if (this->emitter) {
    this->emitter->remainingTime = argument;
  }
}

// OFFSET: 0x006FFF10
// FUNCTION: fn_06973
void CEZ2STREffect::Slot26(int argument) {
  if (this->emitter) {
    this->emitter->unknown_3FE8 = static_cast<unsigned char>(argument);
  }
}

// OFFSET: 0x006FFF30

// OFFSET: 0x006FFF50

// OFFSET: 0x006FFF80
// FUNCTION: fn_06976
void CEZ2STREffect::Slot32() {
  this->alternateRender = 1;
}

// OFFSET: 0x006FFF90

// OFFSET: 0x006FFFB0

// OFFSET: 0x006FFFD0

// OFFSET: 0x006FFFF0

// OFFSET: 0x00700010

// OFFSET: 0x00700030

// OFFSET: 0x00700A10

// OFFSET: 0x00700A90
// FUNCTION: fn_06989
int CEZ2STREffectMgr::EffectId(const std::string& name) {
  std::string key = name;
  for (char& c : key) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }

  const auto found = this->effects.lower_bound(key);
  if (found != this->effects.end() && key.compare(found->first) >= 0) {
    return found->second.id + 10000;
  }

  return -1;
}
