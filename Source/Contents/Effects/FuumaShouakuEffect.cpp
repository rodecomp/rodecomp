#include "Contents/Effects/FuumaShouakuEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x007028C0
// FUNCTION: fn_07007
CFuumaShouakuEffect::CFuumaShouakuEffect()
    : CEffect(0x930), first(nullptr), second(nullptr), unknown_00D4(0), unknown_00D8(0), unknown_00DC(0), unknown_00E0(0) {
}

// OFFSET: 0x00702CC0
// FUNCTION: fn_07011
bool CFuumaShouakuEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00702F30
void CFuumaShouakuEffect::Slot04() {
  if (this->first) {
    this->first->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  if (this->second) {
    this->second->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}
