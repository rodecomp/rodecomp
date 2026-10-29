#include "Contents/Effects/CaneOfEvilEyeEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x006F86B0
// FUNCTION: fn_06869
CCaneOfEvilEyeEffect::CCaneOfEvilEyeEffect() : CEffect(-1) {
  this->startTime = timeGetTime();
}

// OFFSET: 0x006F8E70
// FUNCTION: fn_06876
void CCaneOfEvilEyeEffect::RemoveNullEmitters() {
  for (auto it = this->unknown_00D0.begin(); it != this->unknown_00D0.end();) {
    if (*it == nullptr) {
      it = this->unknown_00D0.erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end();) {
    if (*it == nullptr) {
      it = this->unknown_00DC.erase(it);
    } else {
      ++it;
    }
  }
}

// OFFSET: 0x006F9340
// FUNCTION: fn_06879
bool CCaneOfEvilEyeEffect::Slot06() {
  if (this->ActorExists() && (!this->unknown_00D0.empty() || !this->unknown_00DC.empty())) {
    return true;
  }

  return false;
}

// OFFSET: 0x006F9D40
// FUNCTION: fn_06882
void CCaneOfEvilEyeEffect::Slot04() {
  for (auto it = this->unknown_00D0.begin(); it != this->unknown_00D0.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }

  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
