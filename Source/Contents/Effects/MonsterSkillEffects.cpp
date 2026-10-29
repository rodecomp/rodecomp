#include "Contents/Effects/MonsterSkillEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/SkillEffects.h"
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
