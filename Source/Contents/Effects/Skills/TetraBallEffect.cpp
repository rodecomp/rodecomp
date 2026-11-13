#include "Contents/Effects/Skills/TetraBallEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"

// OFFSET: 0x0072E680
// FUNCTION: fn_07509
CTetraBallEffect::CTetraBallEffect() : CEffect(-1) {
  this->rotationParameter = 0;
  for (int index = 0; index < 5; ++index) {
    this->balls[index] = nullptr;
  }
}

// OFFSET: 0x0072E7D0
// FUNCTION: fn_07511
void CTetraBallEffect::AddBall(int id, int index) {
  CEZ2STRParticle* ball = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  ball->m_loop = true;

  switch (id) {
  case EF_TETRA_VORTEX_BALL_1:
    ball->Open("tetravortex\\tetra_ball_01_fire\\tetra_01_fire.str");
    break;
  case EF_TETRA_VORTEX_BALL_3:
    ball->Open("tetravortex\\tetra_ball_02_wind\\tetra_02_wind.str");
    break;
  case EF_TETRA_VORTEX_BALL_4:
    ball->Open("tetravortex\\tetra_ball_03_earth\\tetra_03_earth.str");
    break;
  case EF_TETRA_VORTEX_BALL_2:
    ball->Open("tetravortex\\tetra_ball_04_water\\tetra_04_water.str");
    break;
  }

  this->balls[index] = ball;
  this->Spread();
}

// OFFSET: 0x0072EA20
// FUNCTION: fn_07512
void CTetraBallEffect::Close() {
  for (int index = 0; index < 5; ++index) {
    if (this->balls[index]) {
      FreeParticle(this->balls[index]);
      this->balls[index] = nullptr;
    }
  }
}

// OFFSET: 0x0072EA50
// FUNCTION: fn_07513
void CTetraBallEffect::RemoveBall(int index) {
  if (this->balls[index]) {
    FreeParticle(this->balls[index]);
    this->balls[index] = nullptr;
    this->Spread();
  }
}

// OFFSET: 0x0072EA90
// FUNCTION: fn_07514
int CTetraBallEffect::ActiveCount() {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (this->balls[index]) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x0072EAE0
// FUNCTION: fn_07515
void CTetraBallEffect::Open() {
  this->retained = 1;
  this->AddBall(this->id, 0);
}

// OFFSET: 0x0072EB00
// FUNCTION: fn_07516
bool CTetraBallEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0072EB10
// FUNCTION: fn_07517
void CTetraBallEffect::OnProcess() {
  for (int index = 0; index < 5; ++index) {
    CEZ2STRParticle* ball = this->balls[index];
    if (ball) {
      ball->position.x = this->position[0];
      ball->position.y = this->position[1];
      ball->position.z = this->position[2];
      ball->Process(0.0f, 0.0f, 0.0f);
      ++ball->elapsedFrames;
    }
  }
}

// OFFSET: 0x0072EBD0
// FUNCTION: fn_07518
void CTetraBallEffect::Render() {
  for (int index = 0; index < 5; ++index) {
    CEZ2STRParticle* ball = this->balls[index];
    if (ball) {
      ball->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0072EC40
// FUNCTION: fn_07519
void CTetraBallEffect::Spread() {
  const int count = this->ActiveCount();
  if (count > 1) {
    for (int index = 0; index < 5; ++index) {
      if (this->balls[index]) {
        this->balls[index]->Restart();
        this->balls[index]->Advance(static_cast<float>(index) / static_cast<float>(count));
      }
    }
  }
}

// OFFSET: 0x0072ECD0
// FUNCTION: fn_07520
void CTetraBallEffect::MoveFirstTo(int index) {
  if (index != 0) {
    this->balls[index] = this->balls[0];
    this->balls[0] = nullptr;
  }
}
