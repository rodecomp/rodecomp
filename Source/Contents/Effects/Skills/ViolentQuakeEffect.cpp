#include "Contents/Effects/Skills/ViolentQuakeEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

namespace {
// INLINED
__forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// INLINED
__forceinline vector3d TransformPoint(const matrix& transform, const vector3d point) {
  return vector3d{point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9],
                  point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10],
                  point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11]};
}
}

// OFFSET: 0x0072F4E0
// FUNCTION: fn_07529
CViolentQuakeEffect::CViolentQuakeEffect() : CEffect(-1) {
}

// OFFSET: 0x0072F560
// FUNCTION: fn_46234
__declspec(noinline) CViolentQuakeEffect::~CViolentQuakeEffect() {
}

// OFFSET: 0x0072F640
// FUNCTION: fn_07531
void CViolentQuakeEffect::InitAllBloom() {
  int roll = rand() % 2;
  if (roll < 0) {
    roll = 0;
  }

  switch (roll) {
  case 0: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
    bottom->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb01_bottom.str"));
    this->emitters.push_back(bottom);
    CEZ2STRParticle* bomb = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
    bomb->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb01.str"));
    this->emitters.push_back(bomb);
    break;
  }
  case 1: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
    bottom->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb02_bottom.str"));
    this->emitters.push_back(bottom);
    CEZ2STRParticle* bomb = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
    bomb->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb02.str"));
    this->emitters.push_back(bomb);
    break;
  }
  }

  this->AddEvent(0, "effect\\AG_ALL_BLOOM.wav", 0, 0);
}

// OFFSET: 0x0072FBA0
// FUNCTION: fn_07532
void CViolentQuakeEffect::Close() {
  for (std::vector<CEZ2STRParticle*>::iterator it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(*it);
  }

  this->emitters.clear();
}

// OFFSET: 0x0072FAA0
// FUNCTION: fn_46235
__forceinline void CViolentQuakeEffect::InitViolentQuake() {
  CEZ2STRParticle* emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  const int roll = rand() % 3;
  emitter->Load(FormatOwnedString("violentquake\\violentquake\\violentquake_%02d.str", (roll >= 0 ? roll : 0) + 1));
  emitter->offsetX = 0.0f;
  emitter->offsetY = -8.0f;
  emitter->offsetZ = 0.0f;
  this->emitters.push_back(emitter);
  this->AddEvent(0, "effect\\AG_VIOLENT_QUAKE.wav", 0, 0);
}

// OFFSET: 0x0072FBE0
// FUNCTION: fn_07533
void CViolentQuakeEffect::Open() {
  switch (this->id) {
  case EF_VIOLENTQUAKE:
    this->InitViolentQuake();
    break;
  case EF_UNK_1798:
    this->InitAllBloom();
    break;
  }
}

// OFFSET: 0x0072FD00
// FUNCTION: fn_07534
bool CViolentQuakeEffect::IsAlive() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (IsParticleActive(*it) == true) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0072FF90
// FUNCTION: fn_46236
__forceinline void CViolentQuakeEffect::ProcessEmitter(CEZ2STRParticle* emitter) {
  if (emitter) {
    const vector3d origin = this->positionVector;
    const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
    vector3d viewPos{view.values[0] * origin.x + view.values[3] * origin.y + view.values[6] * origin.z + view.values[9],
                     view.values[1] * origin.x + view.values[4] * origin.y + view.values[7] * origin.z + view.values[10],
                     view.values[2] * origin.x + view.values[5] * origin.y + view.values[8] * origin.z + view.values[11]};
    const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;
    viewPos.x += emitter->offsetX;
    viewPos.y += emitter->offsetY;
    viewPos.z += emitter->offsetZ;
    emitter->position = TransformPoint(inverse, viewPos);
    emitter->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0072FD60
// FUNCTION: fn_07535
void CViolentQuakeEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    this->ProcessEmitter(*it);
  }
}

// OFFSET: 0x007301E0
// FUNCTION: fn_46237
__forceinline void CViolentQuakeEffect::RenderEmitter(CEZ2STRParticle* emitter) {
  if (emitter) {
    emitter->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x00730190
// FUNCTION: fn_07536
void CViolentQuakeEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    this->RenderEmitter(*it);
  }
}
