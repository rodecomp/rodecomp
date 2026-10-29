#include "Contents/Effects/TetraBallEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x0072EA90
unsigned char CTetraBallEffect::ActiveCount() {
  unsigned char count = 0;
  for (int index = 0; index < 5; ++index) {
    if (this->balls[index]) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x0072EB00
// FUNCTION: fn_07516
bool CTetraBallEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0072EB10
void CTetraBallEffect::Slot03() {
  for (int index = 0; index < 5; ++index) {
    CEZ2STRParticle* ball = this->balls[index];
    if (ball) {
      ball->position.x = this->position[0];
      ball->position.y = this->position[1];
      ball->position.z = this->position[2];
      ball->Process(0.0f, 0.0f, 0.0f);
      ++ball->unknown_00E8;
    }
  }
}

// OFFSET: 0x0072EBD0
void CTetraBallEffect::Slot04() {
  for (int index = 0; index < 5; ++index) {
    if (this->balls[index]) {
      this->balls[index]->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0072EC40
void CTetraBallEffect::Spread() {
  const unsigned char count = this->ActiveCount();
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
void CTetraBallEffect::MoveFirstTo(int index) {
  if (index != 0) {
    this->balls[index] = this->balls[0];
    this->balls[0] = nullptr;
  }
}
