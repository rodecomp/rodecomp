#include "Contents/Effects/TriangleShotArrow.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x0072ED00
CTriangleShotArrow::CTriangleShotArrow() : CEffect(-1), current{}, direction{}, origin{} {
  this->unknown_0050 = 0;
}

// OFFSET: 0x0072F2B0
bool CTriangleShotArrow::Slot06() {
  return static_cast<float>(this->step) <= 8.0f / this->speed;
}

// OFFSET: 0x0072F2E0
// FUNCTION: fn_07526
void CTriangleShotArrow::Slot03() {
  if (this->delay > 0) {
    this->delay -= 16;
    return;
  }

  CEZ2STRParticle* arrow = this->arrow;
  if (arrow) {
    const float step = static_cast<float>(this->step);
    this->current.x = step * this->direction.x + this->origin.x;
    this->current.y = step * this->direction.y + this->origin.y;
    this->current.z = step * this->direction.z + this->origin.z;
    this->position[0] = this->current.x;
    this->position[1] = this->current.y;
    this->position[2] = this->current.z;
    arrow->position.x = this->current.x;
    arrow->position.y = this->current.y;
    arrow->position.z = this->current.z;
    arrow->Process(0.0f, 0.0f, 0.0f);
  }

  ++this->step;
}

// OFFSET: 0x0072F470
void CTriangleShotArrow::Slot04() {
  if (this->arrow) {
    this->arrow->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0072F4D0
void CTriangleShotArrow::Slot16(int delay) {
  this->delay = delay;
}
