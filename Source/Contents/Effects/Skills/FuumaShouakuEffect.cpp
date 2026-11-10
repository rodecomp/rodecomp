#include "Contents/Effects/Skills/FuumaShouakuEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

// OFFSET: 0x007028C0
// FUNCTION: fn_07007
CFuumaShouakuEffect::CFuumaShouakuEffect()
    : CEffect(0x930), first(nullptr), second(nullptr), velocityX(0), velocityY(0), velocityZ(0), travelFrames(0) {
}

// OFFSET: 0x00702A20
// FUNCTION: fn_07009
void CFuumaShouakuEffect::Close() {
  if (this->first) {
    FreeParticle(this->first);
    this->first = nullptr;
  }

  if (this->second) {
    FreeParticle(this->second);
    this->second = nullptr;
  }
}

// OFFSET: 0x00702A60
// FUNCTION: fn_07010
void CFuumaShouakuEffect::Open() {
  this->retained = 1;
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->first = particle;
  if (!particle) {
    return;
  }

  particle->Load(std::string("SHINKIRO_SHIRANUI\\SS_FUUMASHOUAKU\\fuumashouaku\\fuumashouaku.str"));

  const float* target = this->sourcePosition;
  const float* origin = this->position;
  const float dy = target[1] - origin[1];
  const float dx = target[0] - origin[0];
  const float dz = target[2] - origin[2];
  this->travelFrames = static_cast<int>(SquareRoot(dy * dy + dx * dx + dz * dz) * g_depthAverageHalf);

  this->velocityX = target[0] - origin[0];
  this->velocityY = target[1] - origin[1];
  this->velocityZ = target[2] - origin[2];
  const float scale = 1.0f / SquareRoot(this->velocityX * this->velocityX + this->velocityY * this->velocityY + this->velocityZ * this->velocityZ);
  this->velocityX = scale * this->velocityX * 2.0f;
  this->velocityY = scale * this->velocityY * 2.0f;
  this->velocityZ = scale * this->velocityZ * 2.0f;

  this->first->m_angle = ScreenAngle(origin, target, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
}

// OFFSET: 0x00702CC0
// FUNCTION: fn_07011
bool CFuumaShouakuEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00702CD0
// FUNCTION: fn_07012
void CFuumaShouakuEffect::OnProcess() {
  if (this->frame >= this->travelFrames) {
    if (!this->second) {
      if (this->first) {
        FreeParticle(this->first);
        this->first = nullptr;
      }

      this->second = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->sourcePosition), 5));
      if (this->second) {
        this->second->Load(std::string("SHINKIRO_SHIRANUI\\SS_FUUMASHOUAKU\\fuumashouaku_00\\fuumashouaku_00.str"));
        this->second->m_loop = true;
        this->AddEvent(0, "effect\\SHINKIRO_SHIRANUI\\SS_FUUMASHOUAKU.wav", this->travelFrames + 1, 0);
      }
    } else if (this->second->m_frame == 1) {
      this->frame = this->travelFrames;
    }
  } else if (this->first) {
    CEZ2STRParticle* particle = this->first;
    particle->position = {particle->position.x + this->velocityX, particle->position.y + this->velocityY, particle->position.z + this->velocityZ};
  }

  if (this->first) {
    this->first->Process(0.0f, 0.0f, 0.0f);
  }

  if (this->second) {
    this->second->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00702F30
// FUNCTION: fn_07013
void CFuumaShouakuEffect::Render() {
  if (this->first) {
    this->first->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  if (this->second) {
    this->second->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}
