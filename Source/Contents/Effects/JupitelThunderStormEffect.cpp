#include "Contents/Effects/JupitelThunderStormEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

// INLINED
static bool ParticleAlive(const CParticle* particle) {
  return particle->life == 0.0f || static_cast<float>(particle->unknown_00E8) < particle->life * 60.0f;
}

// OFFSET: 0x00705F80
// FUNCTION: fn_07040
CJupitelThunderStormEffect::CJupitelThunderStormEffect() : CEffect(2366), particle(nullptr), step{}, duration(10) {
}

// OFFSET: 0x00706110
// FUNCTION: fn_07042
void CJupitelThunderStormEffect::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->particles.clear();
}

// OFFSET: 0x007065F0
// FUNCTION: fn_07044
bool CJupitelThunderStormEffect::Slot06() {
  if (this->particle && ParticleAlive(this->particle)) {
    return true;
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it && ParticleAlive(*it)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00706690
// FUNCTION: fn_07045
void CJupitelThunderStormEffect::Slot03() {
  if (CParticle* particle = this->particle) {
    if (ParticleAlive(particle)) {
      vector3d position;
      if (this->frame < this->duration) {
        position.x = particle->position.x + this->step.x;
        position.y = particle->position.y + this->step.y;
        position.z = particle->position.z + this->step.z;
      } else {
        position.x = this->sourcePosition[0];
        position.y = this->sourcePosition[1];
        position.z = this->sourcePosition[2];
      }
      particle->position = position;
      static_cast<CEZ2STRParticle*>(this->particle)->Process(0.0f, 0.0f, 0.0f);
    } else {
      FreeParticle(particle);
      this->particle = nullptr;
    }
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      static_cast<CEZ2STRParticle*>(*it)->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x007067C0
// FUNCTION: fn_07046
void CJupitelThunderStormEffect::Slot04() {
  if (this->particle) {
    static_cast<CEZ2STRParticle*>(this->particle)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      static_cast<CEZ2STRParticle*>(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
