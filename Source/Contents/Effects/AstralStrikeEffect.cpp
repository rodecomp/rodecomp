#include "Contents/Effects/AstralStrikeEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

#include <algorithm>
#include <cstdlib>
#include <string>

// OFFSET: 0x006F4830
// FUNCTION: fn_06828
CAstralStrikeEffect::CAstralStrikeEffect() : CEffect(1768) {
}

// OFFSET: 0x006F49D0
// FUNCTION: fn_06830
void CAstralStrikeEffect::Init() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  bottom->Load(std::string("astralstrike\\astralstrike_bottom\\astralstrike_bottom.str"));
  this->emitters.push_back(bottom);

  this->AddEvent(0, "effect\\AG_ASTRAL_STRIKE1.wav", 0, 0);

  CEZ2STRParticle* strike = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  strike->Load(std::string("astralstrike\\astralstrike\\astralstrike.str"));
  this->emitters.push_back(strike);
}

// OFFSET: 0x006F4C50
// FUNCTION: fn_06831
void CAstralStrikeEffect::SpawnStrike() {
  const __int64 ranges[10] = {5, 5, 5, 7, 7, 7, 9, 9, 9, 11};
  const __int64 range = ranges[std::min<unsigned int>(this->unknown_00B0, 10) - 1] * 50;

  vector3d position = {this->position[0], this->position[1], this->position[2]};
  const int span = static_cast<int>(range) + 1;
  const int offsetX = std::max(rand() % span, 0);
  const float half = static_cast<float>(range) * 0.5f;
  position.x += (static_cast<float>(offsetX) - half) * 0.1f;
  const int offsetZ = std::max(rand() % span, 0);
  position.z += (static_cast<float>(offsetZ) - half) * 0.1f;

  switch (rand() % 3) {
  case 0: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 13));
    bottom->Load(std::string("astralstrike\\astralstrike_sub_1_bottom\\astralstrike_sub_1_bottom.str"));
    this->strikes.push_back(bottom);
    CEZ2STRParticle* strike = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
    strike->Load(std::string("astralstrike\\astralstrike_sub_1\\astralstrike_sub_1.str"));
    this->strikes.push_back(strike);
    break;
  }
  case 1: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 13));
    bottom->Load(std::string("astralstrike\\astralstrike_sub_2_bottom\\astralstrike_sub_2_bottom.str"));
    this->strikes.push_back(bottom);
    CEZ2STRParticle* strike = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
    strike->Load(std::string("astralstrike\\astralstrike_sub_2\\astralstrike_sub_2.str"));
    this->strikes.push_back(strike);
    break;
  }
  case 2: {
    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 13));
    bottom->Load(std::string("astralstrike\\astralstrike_sub_3_bottom\\astralstrike_sub_3_bottom.str"));
    this->strikes.push_back(bottom);
    CEZ2STRParticle* strike = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&position, 5));
    strike->Load(std::string("astralstrike\\astralstrike_sub_3\\astralstrike_sub_3.str"));
    this->strikes.push_back(strike);
    break;
  }
  }

  this->AddEvent(0, "effect\\AG_ASTRAL_STRIKE2.wav", this->frame + 1, 0);
}

// OFFSET: 0x006F5430
// FUNCTION: fn_06832
void CAstralStrikeEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(*it);
  }
  this->emitters.clear();

  for (auto it = this->strikes.begin(); it != this->strikes.end(); ++it) {
    FreeParticle(*it);
  }
  this->strikes.clear();
}

// OFFSET: 0x006F54A0
// FUNCTION: fn_06833
void CAstralStrikeEffect::Slot01() {
  this->Init();
}

// OFFSET: 0x006F54B0
// FUNCTION: fn_06834
bool CAstralStrikeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006F54C0
// FUNCTION: fn_06835
void CAstralStrikeEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    CEZ2STRParticle* emitter = *it;
    if (emitter->life == 0.0f || emitter->life * 60.0f > static_cast<float>(emitter->unknown_00E8)) {
      if (emitter) {
        emitter->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(emitter);
      it = this->emitters.erase(it);
    }
  }

  if (this->ActorExists() == true && this->emitters.empty()) {
    const __int64 intervals[10] = {11, 11, 11, 9, 9, 9, 7, 7, 7, 5};
    if (this->frame % intervals[std::min<unsigned int>(this->unknown_00B0, 10) - 1] == 0) {
      this->SpawnStrike();
    }
  }

  for (auto it = this->strikes.begin(); it != this->strikes.end();) {
    CEZ2STRParticle* strike = *it;
    if (strike->life == 0.0f || strike->life * 60.0f > static_cast<float>(strike->unknown_00E8)) {
      if (strike) {
        strike->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(strike);
      it = this->strikes.erase(it);
    }
  }
}

// OFFSET: 0x006F5760
// FUNCTION: fn_06836
void CAstralStrikeEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }

  for (auto it = this->strikes.begin(); it != this->strikes.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
