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
    default:
      break;
  }

  static_cast<CEZ2STRParticle*>(bottom)->m_loop = true;
  this->particles[1] = bottom;
  this->AddEvent(0, "effect\\AG_CLIMAX.wav", 0, 0);
}

// OFFSET: 0x00706F00
// FUNCTION: fn_07051
bool CClimaxEffect::IsAlive() {
  return this->ActorExists();
}

// INLINED
static void MatrixMult(vector3d& result, const vector3d& v, const float* m) {
  result.x = v.x * m[0] + v.y * m[3] + v.z * m[6] + m[9];
  result.y = v.x * m[1] + v.y * m[4] + v.z * m[7] + m[10];
  result.z = v.x * m[2] + v.y * m[5] + v.z * m[8] + m[11];
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

    CEZ2STRParticle* climax = static_cast<CEZ2STRParticle*>(this->particles[index]);
    if (climax) {
      if (FindRenderObject(this->actor)) {
        const vector3d position = climax->position;
        const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
        const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
        vector3d viewPosition;
        MatrixMult(viewPosition, position, view);
        viewPosition.x += 0.0f;
        viewPosition.y -= 10.0f;
        vector3d world;
        MatrixMult(world, viewPosition, inverse);
        climax->position = world;
      }

      climax->Process(0.0f, 0.0f, 0.0f);
    }

    ++this->particles[index]->elapsedFrames;
  }
}

// OFFSET: 0x00707270
// FUNCTION: fn_07053
void CClimaxEffect::Render() {
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = this->particles[index];
    if (particle) {
      static_cast<CEZ2STRParticle*>(particle)->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
    }
  }
}
