#include "Contents/Effects/TriangleShotArrow.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

CParticle* sub_B82ED0(const vector3d* position, int flags);
void sub_B82FD0(CParticle* particle);

// OFFSET: 0x0072ED00
// FUNCTION: fn_07521
CTriangleShotArrow::CTriangleShotArrow() : CEffect(-1), current{}, direction{}, origin{} {
  this->unknown_0050 = 0;
}

// OFFSET: 0x0072EE80
// FUNCTION: fn_07523
void CTriangleShotArrow::Close() {
  sub_B82FD0(this->arrow);
}

// OFFSET: 0x0072EE90
// FUNCTION: fn_07524
void CTriangleShotArrow::Slot01() {
  this->current.x = this->position[0];
  this->current.y = this->position[1];
  this->current.z = this->position[2];

  CEZ2STRParticle* arrow = static_cast<CEZ2STRParticle*>(sub_B82ED0(&this->current, 5));
  if (g_serviceOptions.Get(229)) {
    arrow->Open("mineffect\\new_triangleshot\\new_triangleshot_arrow\\new_triangleshot_arrow.str");
  } else {
    arrow->Open("new_triangleshot\\new_triangleshot_arrow\\new_triangleshot_arrow.str");
  }
  arrow->m_loop = true;
  this->arrow = arrow;

  this->speed = 1.0f;
  this->delay = 0;
  this->current.y -= 10.0f;
  vector3d target;
  target.x = this->sourcePosition[0];
  target.y = this->sourcePosition[1] - 10.0f;
  target.z = this->sourcePosition[2];
  this->step = 0;

  this->direction.x = target.x - this->current.x;
  this->direction.y = target.y - this->current.y;
  this->direction.z = target.z - this->current.z;
  const float length = SquareRoot(this->direction.x * this->direction.x + this->direction.y * this->direction.y +
                                  this->direction.z * this->direction.z);
  this->direction.x *= 0.125f;
  this->direction.y *= 0.125f;
  this->direction.z *= 0.125f;
  if (length < 40.0f) {
    this->speed = 40.0f / length;
    this->direction.x *= this->speed;
    this->direction.y *= this->speed;
    this->direction.z *= this->speed;
  }

  const float dx = target.x - this->current.x;
  const float dy = target.y - this->current.y;
  const float dz = target.z - this->current.z;
  const float inverse = 1.0f / SquareRoot(dx * dx + dy * dy + dz * dz);
  this->current.x = inverse * dx * 4.0f + this->current.x;
  this->current.y = inverse * dy * 4.0f + this->current.y;
  this->current.z = inverse * dz * 4.0f + this->current.z;
  this->origin = this->current;
  this->position[0] = this->current.x;
  this->position[1] = this->current.y;
  this->position[2] = this->current.z;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float angle = static_cast<float>(atan2(static_cast<double>(target.z - this->current.z), static_cast<double>(target.x - this->current.x)));
  const float cameraYaw = mode->camera ? mode->camera->camera.desiredYaw : 0.0f;
  float yaw = cameraYaw + angle / 3.141592f * 180.0f;
  if (yaw < 0.0f) {
    yaw += 360.0f;
  }
  this->arrow->m_angle = 360.0f - yaw;
}

// OFFSET: 0x0072F2B0
// FUNCTION: fn_07525
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
// FUNCTION: fn_07527
void CTriangleShotArrow::Slot04() {
  if (this->arrow) {
    this->arrow->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0072F4D0
// FUNCTION: fn_07528
void CTriangleShotArrow::Slot16(int delay) {
  this->delay = delay;
}
