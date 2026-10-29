#include "Contents/Effects/MonsterSkillEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x0070C070

// OFFSET: 0x0070C3B0

// OFFSET: 0x0070C3D0
// FUNCTION: fn_07131
bool CRootTwistEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070C3E0

// OFFSET: 0x0070C7F0

// OFFSET: 0x0070D150
// FUNCTION: fn_07145
bool CSpriteMableEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070D660
// FUNCTION: fn_07153
bool CStemSpearEffect::Slot06() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed < this->unknown_002C) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0070D690

// OFFSET: 0x0070DB30
// FUNCTION: fn_07160
bool CSvgSpiritEffect::Slot06() {
  return this->emitter->elapsed < this->unknown_002C;
}

// OFFSET: 0x0070DB50

// OFFSET: 0x0070DF30
// FUNCTION: fn_07167
bool CTunaPartyEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070EB60
// FUNCTION: fn_07178
bool CCurseOfCubeEffect::Slot06() {
  if (!this->ActorExists() || this->emitters.empty()) {
    return false;
  }

  return true;
}

// OFFSET: 0x0070F350
// FUNCTION: fn_07185
void CWarningPlaneEffect::Slot01() {
}

// OFFSET: 0x0070F360

// OFFSET: 0x0070FC60

// OFFSET: 0x007100D0
CDancingKnifeEffect::CDancingKnifeEffect() : CEffect(1829) {
}

// OFFSET: 0x00710830
// FUNCTION: fn_07203
bool CDancingKnifeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00710AE0
CEternalSlashAttackEffect::CEternalSlashAttackEffect() : CEffect(-1) {
}

// OFFSET: 0x00710F50
bool CEternalSlashAttackEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00711120
CEternalSlashCountEffect::CEternalSlashCountEffect() : CEffect(1913), emitter(nullptr) {
}

// OFFSET: 0x00711340
bool CEternalSlashCountEffect::Slot06() {
  EZ2STREmitter* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f;
}

// OFFSET: 0x00711420
CFallenAngelEffect::CFallenAngelEffect() : CEffect(1140), emitter(nullptr), unknown_00D0(0), unknown_00D4(0), unknown_00D8(0) {
}

// OFFSET: 0x00711690
bool CFallenAngelEffect::Slot06() {
  if (this->emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007118C0
CFloralFlareRoad::CFloralFlareRoad() : CEffect(1804) {
}

// OFFSET: 0x00711DA0
// FUNCTION: fn_07232
bool CFloralFlareRoad::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00711F80
CFootprintEffect::CFootprintEffect() : CEffect(2411), stepDistance(0.0), unknown_0110(1), lastPosition{}, unknown_0120(-1000.0f) {
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
}

// OFFSET: 0x007123C0
bool CFootprintEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x007127A0
// FUNCTION: fn_07240
bool CFootprintEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00713370
bool CFootprintStrEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x00713830
// FUNCTION: fn_07250
bool CFootprintStrEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00715400
// FUNCTION: fn_07266
void CFromTheAbyssEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00715410
// FUNCTION: fn_07267
bool CFromTheAbyssEffect::Slot06() {
  return this->ActorExists();
}
