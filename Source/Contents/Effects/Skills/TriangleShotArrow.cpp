#include "Contents/Effects/Skills/TriangleShotArrow.h"
#include "UI/GameSettings/TalkType.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0072ED00
// FUNCTION: fn_07521
CTriangleShotArrow::CTriangleShotArrow() : CEffect(-1) {
  this->current.x = 0.0f;
  this->current.y = 0.0f;
  this->current.z = 0.0f;
  this->direction.x = 0.0f;
  this->direction.y = 0.0f;
  this->direction.z = 0.0f;
  this->origin.x = 0.0f;
  this->origin.y = 0.0f;
  this->origin.z = 0.0f;
  this->rotationParameter = 0;
}

// OFFSET: 0x0072EDC0
// FUNCTION: fn_46011
__declspec(noinline) CTriangleShotArrow::~CTriangleShotArrow() {
}

// OFFSET: 0x0072EE80
// FUNCTION: fn_07523
void CTriangleShotArrow::Close() {
  FreeParticle(this->arrow);
}

// OFFSET: 0x0072EE90
// FUNCTION: fn_07524
void CTriangleShotArrow::Open() {
  vector3d& pos = this->current;
  pos.x = this->position[0];
  pos.y = this->position[1];
  pos.z = this->position[2];

  CEZ2STRParticle* arrow = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&pos, 5));
  if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
    arrow->Open("mineffect\\new_triangleshot\\new_triangleshot_arrow\\new_triangleshot_arrow.str");
  } else {
    arrow->Open("new_triangleshot\\new_triangleshot_arrow\\new_triangleshot_arrow.str");
  }
  arrow->m_loop = true;
  this->arrow = arrow;

  this->speed = 1.0f;
  this->delay = 0;
  this->current.y -= 10.0f;
  vector3d target = this->sourcePositionVector;
  this->step = 0;
  target.y -= 10.0f;

  const vector3d delta{target.x - pos.x, target.y - pos.y, target.z - pos.z};
  this->direction = delta;
  const float length = SquareRoot(delta.y * delta.y + delta.x * delta.x + delta.z * delta.z);
  this->direction.x *= 0.125f;
  this->direction.y *= 0.125f;
  this->direction.z *= 0.125f;
  if (length < 40.0f) {
    this->speed = 40.0f / length;
    this->direction *= this->speed;
  }

  const float dy = target.y - pos.y;
  const float dx = target.x - pos.x;
  const float dz = target.z - pos.z;
  const float inverse = 1.0f / SquareRoot(dx * dx + dy * dy + dz * dz);
  const float x = inverse * dx * 4.0f + pos.x;
  const float y = inverse * dy * 4.0f + pos.y;
  const float z = inverse * dz * 4.0f + pos.z;
  this->origin.x = pos.x = x;
  this->origin.y = pos.y = y;
  this->origin.z = pos.z = z;
  this->position[0] = x;
  this->position[1] = y;
  this->position[2] = z;

  CGameMode* mode = g_modeMgr.GetGameMode();
  const float angle = static_cast<float>(atan2(static_cast<double>(target.z - this->current.z), static_cast<double>(target.x - pos.x)));
  float yaw = mode->CameraDesiredYaw() + angle / 3.141592f * 180.0f;
  if (yaw < 0.0f) {
    yaw += 360.0f;
  }
  this->arrow->m_angle = 360.0f - yaw;
}

// OFFSET: 0x0072F2B0
// FUNCTION: fn_07525
bool CTriangleShotArrow::IsAlive() {
  if (static_cast<float>(this->step) > 8.0f / this->speed) {
    return false;
  }

  return true;
}

// OFFSET: 0x0072F3B0
// FUNCTION: fn_46012
__forceinline void CTriangleShotArrow::UpdateArrow(CEZ2STRParticle* particle) {
  if (particle) {
    const float step = static_cast<float>(this->step);
    this->current.x = step * this->direction.x + this->origin.x;
    this->current.y = step * this->direction.y + this->origin.y;
    this->current.z = step * this->direction.z + this->origin.z;
    this->position[0] = this->current.x;
    this->position[1] = this->current.y;
    this->position[2] = this->current.z;
    particle->position = this->current;
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0072F2E0
// FUNCTION: fn_07526
void CTriangleShotArrow::OnProcess() {
  if (this->delay > 0) {
    this->delay -= 16;
    return;
  }

  this->UpdateArrow(this->arrow);
  ++this->step;
}

// OFFSET: 0x0072F4A0
// FUNCTION: fn_46013
__forceinline void CTriangleShotArrow::RenderArrow(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x0072F470
// FUNCTION: fn_07527
void CTriangleShotArrow::Render() {
  this->RenderArrow(this->arrow);
}

// OFFSET: 0x0072F4D0
// FUNCTION: fn_07528
void CTriangleShotArrow::SetDelay(int delay) {
  this->delay = delay;
}
