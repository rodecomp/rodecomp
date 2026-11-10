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
}

// OFFSET: 0x0072F4E0
// FUNCTION: fn_07529
CViolentQuakeEffect::CViolentQuakeEffect() : CEffect(-1) {
}

// OFFSET: 0x0072F640
// FUNCTION: fn_07531
void CViolentQuakeEffect::InitAllBloom() {
  switch (rand() % 2) {
  case 0: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    bottom->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb01_bottom.str"));
    this->emitters.push_back(bottom);
    CEZ2STRParticle* bomb = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    bomb->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb01.str"));
    this->emitters.push_back(bomb);
    break;
  }
  case 1: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    bottom->Load(std::string("allbloom\\allbloom_bomb\\allbloom_bomb02_bottom.str"));
    this->emitters.push_back(bottom);
    CEZ2STRParticle* bomb = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
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

// OFFSET: 0x0072FBE0
// FUNCTION: fn_07533
void CViolentQuakeEffect::Open() {
  switch (this->id) {
  case EF_VIOLENTQUAKE: {
    CEZ2STRParticle* emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    emitter->Load(FormatOwnedString("violentquake\\violentquake\\violentquake_%02d.str", std::max(0, rand() % 3) + 1));
    emitter->offsetX = 0.0f;
    emitter->offsetY = -8.0f;
    emitter->offsetZ = 0.0f;
    this->emitters.push_back(emitter);
    this->AddEvent(0, "effect\\AG_VIOLENT_QUAKE.wav", 0, 0);
    break;
  }
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

// OFFSET: 0x0072FD60
// FUNCTION: fn_07535
void CViolentQuakeEffect::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* emitter = *it;
    if (!emitter) {
      continue;
    }

    const vector3d origin = *reinterpret_cast<const vector3d*>(this->position);
    const matrix view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
    const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

    const float viewX = view.values[0] * origin.x + view.values[3] * origin.y + view.values[6] * origin.z + view.values[9] + emitter->offsetX;
    const float viewY = view.values[1] * origin.x + view.values[4] * origin.y + view.values[7] * origin.z + view.values[10] + emitter->offsetY;
    const float viewZ = view.values[2] * origin.x + view.values[5] * origin.y + view.values[8] * origin.z + view.values[11] + emitter->offsetZ;

    emitter->position.x = viewY * inverse.values[3] + viewX * inverse.values[0] + viewZ * inverse.values[6] + inverse.values[9];
    emitter->position.y = viewY * inverse.values[4] + viewX * inverse.values[1] + viewZ * inverse.values[7] + inverse.values[10];
    emitter->position.z = viewY * inverse.values[5] + viewX * inverse.values[2] + viewZ * inverse.values[8] + inverse.values[11];
    emitter->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00730190
// FUNCTION: fn_07536
void CViolentQuakeEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
