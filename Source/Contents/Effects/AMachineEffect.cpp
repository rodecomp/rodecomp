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
void CAMachineEffect::Open() {
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
  for (std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.begin(); it != this->unknown_00CC.end(); ++it) {
    if (it->second) {
      FreeParticle(it->second);
      it->second = nullptr;
    }
  }
  this->unknown_00CC.clear();
}

// OFFSET: 0x006F6DB0
// FUNCTION: fn_06853
void CAMachineEffect::OnProcess() {
  const int key = this->unknown_00D4;
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(key);
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
  const matrix view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
  float viewX = view.values[0] * x + view.values[3] * y + view.values[6] * z + view.values[9] + 0.0f;
  float viewY = view.values[1] * x + view.values[4] * y + view.values[7] * z + view.values[10] - 8.0f;
  float viewZ = view.values[2] * x + view.values[5] * y + view.values[8] * z + view.values[11] + 0.0f;
  particle->position.x = viewX * inverse[0] + viewY * inverse[3] + viewZ * inverse[6] + inverse[9];
  particle->position.y = viewX * inverse[1] + viewY * inverse[4] + viewZ * inverse[7] + inverse[10];
  particle->position.z = viewX * inverse[2] + viewY * inverse[5] + viewZ * inverse[8] + inverse[11];
  if (!particle->Process(0.0f, 0.0f, 0.0f)) {
    this->SetState(2);
  }
}

// OFFSET: 0x006F7000
// FUNCTION: fn_06854
void CAMachineEffect::Render() {
  const int key = this->unknown_00D4;
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(key);
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
bool CAMachineEffect::IsAlive() {
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
  const int key = state;
  std::map<int, CEZ2STRParticle*>::iterator it = this->unknown_00CC.find(key);
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
