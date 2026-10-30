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
// FUNCTION: fn_06897
void CCartCannon::Slot01() {
  this->currentPosition.x = this->position[0];
  this->currentPosition.y = this->position[1];
  this->currentPosition.z = this->position[2];

  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->currentPosition, 5));
  if (g_serviceOptions.Get(229)) {
    particle->Open("mineffect\\new_cart_cannon\\new_cart_cannon\\new_cart_cannon.str");
  } else {
    particle->Open("new_cart_cannon\\new_cart_cannon\\new_cart_cannon.str");
  }
  particle->m_loop = true;
  this->particle = particle;

  this->speed = 1.0f;
  this->delay = 0;
  this->currentPosition.y -= 10.0f;
  vector3d target;
  target.x = this->sourcePosition[0];
  target.y = this->sourcePosition[1] - 10.0f;
  target.z = this->sourcePosition[2];
  this->currentStep = 0;

  this->stepDelta.x = target.x - this->currentPosition.x;
  this->stepDelta.y = target.y - this->currentPosition.y;
  this->stepDelta.z = target.z - this->currentPosition.z;

  const float length = SquareRoot(
    this->stepDelta.x * this->stepDelta.x +
    this->stepDelta.y * this->stepDelta.y +
    this->stepDelta.z * this->stepDelta.z
  );

  this->stepDelta.x /= 10.0f;
  this->stepDelta.y /= 10.0f;
  this->stepDelta.z /= 10.0f;

  if (length < 40.0f) {
    this->speed = 40.0f / length;
    this->stepDelta.x *= this->speed;
    this->stepDelta.y *= this->speed;
    this->stepDelta.z *= this->speed;
  }

  const float dx = target.x - this->currentPosition.x;
  const float dy = target.y - this->currentPosition.y;
  const float dz = target.z - this->currentPosition.z;
  const float inverse = 1.0f / SquareRoot(dx * dx + dy * dy + dz * dz);

  this->currentPosition.x = inverse * dx * 4.0f + this->currentPosition.x;
  this->currentPosition.y = inverse * dy * 4.0f + this->currentPosition.y;
  this->currentPosition.z = inverse * dz * 4.0f + this->currentPosition.z;

  this->startPosition = this->currentPosition;
  this->position[0] = this->currentPosition.x;
  this->position[1] = this->currentPosition.y;
  this->position[2] = this->currentPosition.z;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float angle = static_cast<float>(atan2(static_cast<double>(target.z - this->currentPosition.z), static_cast<double>(target.x - this->currentPosition.x)));
  const float cameraYaw = mode->camera ? mode->camera->camera.desiredYaw : 0.0f;
  float yaw = cameraYaw + angle / 3.141592f * 180.0f;
  if (yaw < 0.0f) {
    yaw += 360.0f;
  }
  yaw = 270.0f - yaw;
  if (yaw < 0.0f) {
    yaw += 360.0f;
  }
  this->particle->m_angle = yaw;
}

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
