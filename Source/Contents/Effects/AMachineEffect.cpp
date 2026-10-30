#include "Contents/Effects/AMachineEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x006F6510
// FUNCTION: fn_06846
CAMachineEffect::CAMachineEffect() : CEffect(-1), unknown_00D4(0) {
}

// OFFSET: 0x006F6A50
// FUNCTION: fn_06851
void CAMachineEffect::Slot01() {
  this->retained = 1;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    particle->Load("a_machine\\a_machine\\a_machine_attack_waiting.str");
    this->unknown_00CC[0] = particle;
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    particle->Load("a_machine\\a_machine\\a_machine_attack.str");
    this->unknown_00CC[1] = particle;
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    particle->Load("a_machine\\a_machine\\a_machine_waiting.str");
    particle->m_loop = true;
    this->unknown_00CC[2] = particle;
  }
}

// OFFSET: 0x006F6940
// FUNCTION: fn_06850
void CAMachineEffect::Close() {
  for (auto& entry : this->unknown_00CC) {
    if (entry.second) {
      FreeParticle(entry.second);
      entry.second = nullptr;
    }
  }
  this->unknown_00CC.clear();
}

// OFFSET: 0x006F6DB0
// FUNCTION: fn_06853
void CAMachineEffect::Slot03() {
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(this->unknown_00D4);
  if (it == this->unknown_00CC.end()) {
    return;
  }

  CEZ2STRParticle* particle = it->second;
  if (!particle) {
    return;
  }

  const float x = this->position[0];
  const float y = this->position[1];
  const float z = this->position[2];
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  float viewX = view[0] * x + view[3] * y + view[6] * z + view[9];
  float viewY = view[1] * x + view[4] * y + view[7] * z + view[10] - 8.0f;
  float viewZ = view[2] * x + view[5] * y + view[8] * z + view[11];
  const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
  particle->position.x = viewX * inverse[0] + viewY * inverse[3] + viewZ * inverse[6] + inverse[9];
  particle->position.y = viewX * inverse[1] + viewY * inverse[4] + viewZ * inverse[7] + inverse[10];
  particle->position.z = viewX * inverse[2] + viewY * inverse[5] + viewZ * inverse[8] + inverse[11];
  if (!particle->Process(0.0f, 0.0f, 0.0f)) {
    this->SetState(2);
  }
}

// OFFSET: 0x006F7000
// FUNCTION: fn_06854
void CAMachineEffect::Slot04() {
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(this->unknown_00D4);
  if (it == this->unknown_00CC.end()) {
    return;
  }

  CEZ2STRParticle* particle = it->second;
  if (!particle) {
    return;
  }

  particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x006F6DA0
// FUNCTION: fn_06852
bool CAMachineEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006F7070
// FUNCTION: fn_06855
void CAMachineEffect::PlayAttack() {
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(1);
  if (it == this->unknown_00CC.end()) {
    return;
  }

  CEZ2STRParticle* particle = it->second;
  if (!particle) {
    return;
  }

  particle->Restart();
  this->unknown_00D4 = 1;
}

// OFFSET: 0x006F70E0
// FUNCTION: fn_06856
void CAMachineEffect::SetState(int state) {
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(state);
  if (it == this->unknown_00CC.end()) {
    return;
  }

  CEZ2STRParticle* particle = it->second;
  if (!particle) {
    return;
  }

  particle->Restart();
  this->unknown_00D4 = state;
}
