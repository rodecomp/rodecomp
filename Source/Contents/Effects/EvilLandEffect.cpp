#include "Contents/Effects/EvilLandEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/NyanggrassEffect.h"

// OFFSET: 0x006FE300
// FUNCTION: fn_06944
CEvilLandEffect::CEvilLandEffect() : CEffect(1555), particles() {
}

// OFFSET: 0x006FE4F0
// FUNCTION: fn_06947
void CEvilLandEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x006FE830
// FUNCTION: fn_06949
bool CEvilLandEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FE840
// FUNCTION: fn_06950
void CEvilLandEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(*it)) {
      particle->offsetV += 0.001f;
    }
  }
}

// OFFSET: 0x006FE8B0
// FUNCTION: fn_06951
void CEvilLandEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(*it);
    if (particle) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (gameMode) {
        int column = 0;
        int row = 0;
        gameMode->WorldCell(this->position[0], this->position[2], column, row);
        const int half = this->unknown_00B0 / 2;
        particle->Render(particle->unknown_000C - half + column, particle->unknown_0010 - half + row);
      }
    }
  }
}
