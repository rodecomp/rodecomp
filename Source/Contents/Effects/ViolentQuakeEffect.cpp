#include "Contents/Effects/ViolentQuakeEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

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
void CViolentQuakeEffect::Slot01() {
  switch (this->id) {
  case 1760: {
    CEZ2STRParticle* emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    emitter->Load(FormatOwnedString("violentquake\\violentquake\\violentquake_%02d.str", std::max(rand() % 3, 0) + 1));
    emitter->unknown_00D0 = 0.0f;
    emitter->unknown_00D4 = -8.0f;
    emitter->unknown_00D8 = 0.0f;
    this->emitters.push_back(emitter);
    this->AddEvent(0, "effect\\AG_VIOLENT_QUAKE.wav", 0, 0);
    break;
  }
  case 1798:
    this->InitAllBloom();
    break;
  }
}

// OFFSET: 0x0072FD00
// FUNCTION: fn_07534
bool CViolentQuakeEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* emitter = *it;
    if (emitter->life == 0.0f || emitter->life * 60.0f > static_cast<float>(emitter->unknown_00E8)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0072FD60
// FUNCTION: fn_07535
void CViolentQuakeEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* emitter = *it;
    if (!emitter) {
      continue;
    }

    const float x = this->position[0];
    const float y = this->position[1];
    const float z = this->position[2];
    const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
    const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
    const float viewX = view[0] * x + view[3] * y + view[6] * z + view[9] + emitter->unknown_00D0;
    const float viewY = view[1] * x + view[4] * y + view[7] * z + view[10] + emitter->unknown_00D4;
    const float viewZ = view[2] * x + view[5] * y + view[8] * z + view[11] + emitter->unknown_00D8;
    emitter->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
    emitter->position.y = viewY * inverse[4] + viewX * inverse[1] + viewZ * inverse[7] + inverse[10];
    emitter->position.z = viewY * inverse[5] + viewX * inverse[2] + viewZ * inverse[8] + inverse[11];
    emitter->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00730190
// FUNCTION: fn_07536
void CViolentQuakeEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
