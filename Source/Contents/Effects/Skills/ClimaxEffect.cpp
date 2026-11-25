#include "Contents/Effects/Skills/ClimaxEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"

// OFFSET: 0x00706840
// FUNCTION: fn_07047
CClimaxEffect::CClimaxEffect() : CEffect(-1) {
  this->particles[0] = nullptr;
  this->particles[1] = nullptr;
}

// OFFSET: 0x007068B0
// FUNCTION: fn_46251
__declspec(noinline) CClimaxEffect::~CClimaxEffect() {
}

// OFFSET: 0x00706AD0
// FUNCTION: fn_07049
void CClimaxEffect::Close() {
  for (int index = 0; index < 2; ++index) {
    if (this->particles[index]) {
      FreeParticle(this->particles[index]);
      this->particles[index] = nullptr;
    }
  }
}

// OFFSET: 0x00706B00
// FUNCTION: fn_07050
void CClimaxEffect::Open() {
  this->retained = 1;

  CParticle* particle = CreateEZ2STRParticle(&this->positionVector, 5);
  static_cast<CEZ2STRParticle*>(particle)->Load("climax\\climax\\climax.str");
  static_cast<CEZ2STRParticle*>(particle)->m_loop = true;
  this->particles[0] = particle;

  CParticle* bottom = CreateEZ2STRParticle(&this->positionVector, 13);
  switch (this->effectLevel) {
    case 1:
      static_cast<CEZ2STRParticle*>(bottom)->Load("climax\\climax_bottom\\climax_bottom_1.str");
      break;
    case 2:
      static_cast<CEZ2STRParticle*>(bottom)->Load("climax\\climax_bottom\\climax_bottom_2.str");
      break;
    case 3:
      static_cast<CEZ2STRParticle*>(bottom)->Load("climax\\climax_bottom\\climax_bottom_3.str");
      break;
    case 4:
      static_cast<CEZ2STRParticle*>(bottom)->Load("climax\\climax_bottom\\climax_bottom_4.str");
      break;
    case 5:
      static_cast<CEZ2STRParticle*>(bottom)->Load("climax\\climax_bottom\\climax_bottom_5.str");
      break;
  }

  static_cast<CEZ2STRParticle*>(bottom)->m_loop = true;
  this->particles[1] = bottom;
  this->AddEvent(EFFECT_EVENT_SOUND, "effect\\AG_CLIMAX.wav", 0, 0);
}

// OFFSET: 0x00706F00
// FUNCTION: fn_07051
bool CClimaxEffect::IsAlive() {
  return this->ActorExists();
}

// INLINED
static void TransformToView(vector3d& result, const vector3d& v, const float* view) {
  const float y = v.y;
  const float x = v.x;
  const float z = v.z;

  result.x = y * view[3];
  result.x += x * view[0];
  result.x += z * view[6];
  result.x += view[9];
  result.y = y * view[4];
  result.y += x * view[1];
  result.y += z * view[7];
  result.y += view[10];
  result.z = y * view[5];
  result.z += x * view[2];
  result.z += z * view[8];
  result.z += view[11];
}

// INLINED
static void MatrixMult(vector3d& result, const vector3d& v, const float* m) {
  result.x = v.y * m[3];
  result.x += v.x * m[0];
  result.x += v.z * m[6];
  result.x += m[9];
  result.y = v.x * m[1];
  result.y += v.y * m[4];
  result.y += v.z * m[7];
  result.y += m[10];
  result.z = v.x * m[2];
  result.z += v.y * m[5];
  result.z += v.z * m[8];
  result.z += m[11];
}

// OFFSET: 0x00706970
// FUNCTION: fn_46252
__forceinline void CClimaxEffect::PositionEmitter(CEZ2STRParticle* emitter) {
  if (FindRenderObject(this->actor)) {
    vector3d position = emitter->position;
    const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
    const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
    vector3d viewPosition;
    TransformToView(viewPosition, position, view);
    viewPosition.x += 0.0f;
    viewPosition.y -= 10.0f;
    MatrixMult(position, viewPosition, inverse);
    emitter->position = position;
  }
}

// OFFSET: 0x007070E0
// FUNCTION: fn_46253
__forceinline void CClimaxEffect::ProcessEmitter(CEZ2STRParticle* emitter) {
  if (emitter) {
    this->PositionEmitter(emitter);
    emitter->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00706F10
// FUNCTION: fn_07052
void CClimaxEffect::OnProcess() {
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = this->particles[index];
    if (!particle) {
      continue;
    }

    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    this->ProcessEmitter(static_cast<CEZ2STRParticle*>(this->particles[index]));

    ++this->particles[index]->elapsedFrames;
  }
}

// OFFSET: 0x007072B0
// FUNCTION: fn_46254
__forceinline void CClimaxEffect::RenderEmitter(CEZ2STRParticle* emitter) {
  if (emitter) {
    emitter->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x00707270
// FUNCTION: fn_07053
void CClimaxEffect::Render() {
  for (int index = 0; index < 2; ++index) {
    this->RenderEmitter(static_cast<CEZ2STRParticle*>(this->particles[index]));
  }
}
