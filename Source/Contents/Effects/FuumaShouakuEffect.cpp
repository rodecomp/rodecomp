#include "Contents/Effects/FuumaShouakuEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

// OFFSET: 0x007028C0
// FUNCTION: fn_07007
CFuumaShouakuEffect::CFuumaShouakuEffect()
    : CEffect(0x930), first(nullptr), second(nullptr), unknown_00D4(0), unknown_00D8(0), unknown_00DC(0), unknown_00E0(0) {
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
void CFuumaShouakuEffect::Slot01() {
  this->retained = 1;
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->first = particle;
  if (!particle) {
    return;
  }

  particle->Load(std::string("SHINKIRO_SHIRANUI\\SS_FUUMASHOUAKU\\fuumashouaku\\fuumashouaku.str"));

  const float dy = this->sourcePosition[1] - this->position[1];
  const float dx = this->sourcePosition[0] - this->position[0];
  const float dz = this->sourcePosition[2] - this->position[2];
  this->unknown_00E0 = static_cast<int>(SquareRoot(dy * dy + dx * dx + dz * dz) * g_depthAverageHalf);

  this->unknown_00D4 = this->sourcePosition[0] - this->position[0];
  this->unknown_00D8 = this->sourcePosition[1] - this->position[1];
  this->unknown_00DC = this->sourcePosition[2] - this->position[2];
  const float scale = 1.0f / SquareRoot(this->unknown_00D4 * this->unknown_00D4 + this->unknown_00D8 * this->unknown_00D8 + this->unknown_00DC * this->unknown_00DC);
  this->unknown_00D4 = scale * this->unknown_00D4 * 2.0f;
  this->unknown_00D8 = scale * this->unknown_00D8 * 2.0f;
  this->unknown_00DC = scale * this->unknown_00DC * 2.0f;

  this->first->m_angle = ScreenAngle(this->position, this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
}

// OFFSET: 0x00702CC0
// FUNCTION: fn_07011
bool CFuumaShouakuEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00702CD0
// FUNCTION: fn_07012
void CFuumaShouakuEffect::Slot03() {
  if (this->frame < this->unknown_00E0) {
    if (this->first) {
      this->first->position.x += this->unknown_00D4;
      this->first->position.y += this->unknown_00D8;
      this->first->position.z += this->unknown_00DC;
    }
  } else if (!this->second) {
    if (this->first) {
      FreeParticle(this->first);
      this->first = nullptr;
    }

    this->second = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->sourcePosition), 5));
    if (this->second) {
      this->second->Load(std::string("SHINKIRO_SHIRANUI\\SS_FUUMASHOUAKU\\fuumashouaku_00\\fuumashouaku_00.str"));
      this->second->m_loop = true;
      this->AddEvent(0, "effect\\SHINKIRO_SHIRANUI\\SS_FUUMASHOUAKU.wav", this->unknown_00E0 + 1, 0);
    }
  } else if (this->second->m_frame == 1) {
    this->frame = this->unknown_00E0;
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
void CFuumaShouakuEffect::Slot04() {
  if (this->first) {
    this->first->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  if (this->second) {
    this->second->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}
