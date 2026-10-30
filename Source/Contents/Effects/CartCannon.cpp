#include "Contents/Effects/CartCannon.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x006FA900
// FUNCTION: fn_06894
CCartCannon::CCartCannon() : CEffect(-1) {
  this->currentPosition.x = 0.0f;
  this->currentPosition.y = 0.0f;
  this->currentPosition.z = 0.0f;
  this->stepDelta.x = 0.0f;
  this->stepDelta.y = 0.0f;
  this->stepDelta.z = 0.0f;
  this->startPosition.x = 0.0f;
  this->startPosition.y = 0.0f;
  this->startPosition.z = 0.0f;
  this->unknown_0050 = 0;
}

// OFFSET: 0x006FAA80
// FUNCTION: fn_06896
void CCartCannon::Close() {
  FreeParticle(this->particle);
}

// OFFSET: 0x006FAA90

// OFFSET: 0x006FAEC0
// FUNCTION: fn_06898
bool CCartCannon::Slot06() {
  return static_cast<float>(this->currentStep) <= 10.0f / this->speed;
}

// OFFSET: 0x006FAEF0
// FUNCTION: fn_06899
void CCartCannon::Slot03() {
  if (this->delay > 0) {
    this->delay -= 16;
    return;
  }

  if (this->particle) {
    const float step = static_cast<float>(this->currentStep);
    this->currentPosition.x = step * this->stepDelta.x + this->startPosition.x;
    this->currentPosition.y = step * this->stepDelta.y + this->startPosition.y;
    this->currentPosition.z = step * this->stepDelta.z + this->startPosition.z;

    this->position[0] = this->currentPosition.x;
    this->position[1] = this->currentPosition.y;
    this->position[2] = this->currentPosition.z;

    this->particle->position = this->currentPosition;
    this->particle->Process(0.0f, 0.0f, 0.0f);
  }

  ++this->currentStep;
}

// OFFSET: 0x006FB080
// FUNCTION: fn_06900
void CCartCannon::Slot04() {
  if (this->particle) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->particle->DrawScreen(mode->camera->frame.view);
  }
}

// OFFSET: 0x006FB0E0
// FUNCTION: fn_06901
void CCartCannon::SetDelay(int delay) {
  this->delay = delay;
}
