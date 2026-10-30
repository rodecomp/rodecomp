#include "Contents/Effects/ClimaxEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"

// OFFSET: 0x00706840
// FUNCTION: fn_07047
CClimaxEffect::CClimaxEffect() : CEffect(-1) {
  this->particles[0] = nullptr;
  this->particles[1] = nullptr;
}

// OFFSET: 0x00706AD0
// FUNCTION: fn_07049
void CClimaxEffect::Close() {
  for (CParticle*& particle : this->particles) {
    if (particle) {
      FreeParticle(particle);
      particle = nullptr;
    }
  }
}

// OFFSET: 0x00706F00
// FUNCTION: fn_07051
bool CClimaxEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00706F10
// FUNCTION: fn_07052
void CClimaxEffect::Slot03() {
  for (CParticle* particle : this->particles) {
    if (!particle) {
      continue;
    }

    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    CEZ2STRParticle* climax = static_cast<CEZ2STRParticle*>(particle);
    if (FindRenderObject(this->actor)) {
      const float x = climax->position.x;
      const float y = climax->position.y;
      const float z = climax->position.z;

      const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
      const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
      const float viewX = y * view[3] + x * view[0] + z * view[6] + view[9] + 0.0f;
      const float viewY = y * view[4] + x * view[1] + z * view[7] + view[10] - 10.0f;
      const float viewZ = y * view[5] + x * view[2] + z * view[8] + view[11];

      climax->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
      climax->position.y = viewX * inverse[1] + viewY * inverse[4] + viewZ * inverse[7] + inverse[10];
      climax->position.z = viewX * inverse[2] + viewY * inverse[5] + viewZ * inverse[8] + inverse[11];
    }

    climax->Process(0.0f, 0.0f, 0.0f);
    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00707270
// FUNCTION: fn_07053
void CClimaxEffect::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      static_cast<CEZ2STRParticle*>(particle)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
