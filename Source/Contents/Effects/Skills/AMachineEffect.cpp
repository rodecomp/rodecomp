#include "Contents/Effects/Skills/AMachineEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"

// INLINED
static __forceinline vector3d TransformPoint(const matrix& transform, const vector3d point) {
  return vector3d{point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9],
                  point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10],
                  point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11]};
}

// OFFSET: 0x006F6510
// FUNCTION: fn_06846
CAMachineEffect::CAMachineEffect() : CEffect(-1), state(0) {
}

// OFFSET: 0x006F6770
// FUNCTION: fn_46157
__declspec(noinline) CAMachineEffect::~CAMachineEffect() {
}

// OFFSET: 0x006F6A00
// FUNCTION: fn_46158
__forceinline CEZ2STRParticle* CAMachineEffect::FindStateParticle(int state) {
  auto it = this->stateParticles.find(state);
  if (it != this->stateParticles.end()) {
    return it->second;
  }
  return nullptr;
}

// OFFSET: 0x006F6A50
// FUNCTION: fn_06851
void CAMachineEffect::Open() {
  this->retained = 1;

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (particle) {
    particle->Load("a_machine\\a_machine\\a_machine_attack_waiting.str");
    this->stateParticles[0] = particle;
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (particle) {
    particle->Load("a_machine\\a_machine\\a_machine_attack.str");
    this->stateParticles[1] = particle;
  }

  particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (particle) {
    particle->Load("a_machine\\a_machine\\a_machine_waiting.str");
    particle->m_loop = true;
    this->stateParticles[2] = particle;
  }
}

// OFFSET: 0x006F6940
// FUNCTION: fn_06850
void CAMachineEffect::Close() {
  for (std::map<int, CEZ2STRParticle*>::iterator it = this->stateParticles.begin(); it != this->stateParticles.end(); ++it) {
    if (it->second) {
      FreeParticle(it->second);
      it->second = nullptr;
    }
  }
  this->stateParticles.clear();
}

// OFFSET: 0x006F6DB0
// FUNCTION: fn_06853
void CAMachineEffect::OnProcess() {
  const int key = this->state;
  std::map<int, CEZ2STRParticle*>::iterator it = this->stateParticles.find(key);
  if (it == this->stateParticles.end()) {
    return;
  }

  CEZ2STRParticle* particle = it->second;
  if (!particle) {
    return;
  }

  const vector3d source = this->positionVector;
  const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
  vector3d viewPos = TransformPoint(view, source);
  const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;
  const vector3d offset{0.0f, -8.0f, 0.0f};
  viewPos += offset;
  particle->position = TransformPoint(inverse, viewPos);
  if (!particle->Process(0.0f, 0.0f, 0.0f)) {
    this->SetState(2);
  }
}

// OFFSET: 0x006F7000
// FUNCTION: fn_06854
void CAMachineEffect::Render() {
  CEZ2STRParticle* particle = this->FindStateParticle(this->state);
  if (!particle) {
    return;
  }

  particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
}

// OFFSET: 0x006F6DA0
// FUNCTION: fn_06852
bool CAMachineEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006F7070
// FUNCTION: fn_06855
void CAMachineEffect::PlayAttack() {
  CEZ2STRParticle* particle = this->FindStateParticle(1);
  if (!particle) {
    return;
  }

  particle->Restart();
  this->state = 1;
}

// OFFSET: 0x006F70E0
// FUNCTION: fn_06856
void CAMachineEffect::SetState(int state) {
  CEZ2STRParticle* particle = this->FindStateParticle(state);
  if (!particle) {
    return;
  }

  particle->Restart();
  this->state = state;
}
