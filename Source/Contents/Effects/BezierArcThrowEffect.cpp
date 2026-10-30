#include "Contents/Effects/BezierArcThrowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// INLINED
static vector3d TransformPoint(const matrix& transform, const vector3d& point) {
  vector3d result;
  result.x = point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9];
  result.y = point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10];
  result.z = point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11];
  return result;
}

// OFFSET: 0x007AAEA0
// FUNCTION: fn_09102
CThunderingOrb::CThunderingOrb(const std::string& name) : CBezierArcThrowEffect(name) {}

// OFFSET: 0x007AAF40
// FUNCTION: fn_09103
CWindBomb::CWindBomb(const std::string& name) : CBezierArcThrowEffect(name) {}

// OFFSET: 0x007AB290
// FUNCTION: fn_09107
void CBezierArcThrowEffect::Close() {
  if (this->m_particle) {
    FreeParticle(this->m_particle);
    this->m_particle = nullptr;
  }
}

// OFFSET: 0x007AB2C0
// FUNCTION: fn_09108
void CBezierArcThrowEffect::Slot01() {
  this->m_particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (this->m_particle) {
    this->m_particle->Load(this->m_name);
    this->m_particle->m_loop = true;
    this->m_target.x = 0.0f;
    this->m_target.y = 0.0f;
    this->m_target.z = 0.0f;
    this->m_progress = 1.0f;
    this->m_active = true;
  }
}

// OFFSET: 0x007AB340
// FUNCTION: fn_09110
void CBezierArcThrowEffect::Slot03() {
  if (!this->m_active) {
    return;
  }

  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor || !this->m_particle) {
    this->m_active = false;
    return;
  }

  const vector3d actorPos = actor->render.position;
  CView* camera = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
  vector3d actorView = TransformPoint(camera->frame.view, actorPos);
  float targetViewX = this->m_target.y * camera->frame.view.values[3] +
                      this->m_target.x * camera->frame.view.values[0] +
                      this->m_target.z * camera->frame.view.values[6] +
                      camera->frame.view.values[9];

  matrix rotation;
  if (targetViewX < actorView.x) {
    rotation.MakeYRotation(2.094395f);
  } else {
    rotation.MakeYRotation(4.188789f);
  }

  actorView.y -= 10.0f;
  vector3d ctrlPt = TransformPoint(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse, actorView);
  this->m_progress -= 0.07f;

  vector3d delta;
  delta.y = this->m_target.y - ctrlPt.y;
  delta.x = this->m_target.x - ctrlPt.x;
  delta.z = this->m_target.z - ctrlPt.z;

  float distSq = delta.y * delta.y + delta.x * delta.x + delta.z * delta.z;
  float dist = SquareRoot(distSq);
  float invDist = 1.0f / SquareRoot(distSq);

  vector3d norm;
  norm.x = delta.x * invDist * dist;
  norm.y = delta.y * invDist * dist;
  norm.z = delta.z * invDist * dist;

  vector3d rotated = TransformPoint(rotation, norm);

  float t = this->m_progress;
  float u = 1.0f - t;
  float u2 = static_cast<float>(std::pow(static_cast<double>(u), 2.0));
  float t2 = static_cast<float>(std::pow(static_cast<double>(t), 2.0));
  float mid = (t + t) * u;

  this->m_particle->position.x = (rotated.x + this->m_target.x) * mid + ctrlPt.x * t2 + this->m_target.x * u2;
  this->m_particle->position.y = (rotated.y + this->m_target.y) * mid + ctrlPt.y * t2 + this->m_target.y * u2;
  this->m_particle->position.z = (rotated.z + this->m_target.z) * mid + ctrlPt.z * t2 + this->m_target.z * u2;

  this->m_particle->Process(0.0f, 0.0f, 0.0f);
  if (0.0f >= this->m_progress) {
    FreeParticle(this->m_particle);
    this->m_particle = nullptr;
  }
}

// OFFSET: 0x007AB800
// FUNCTION: fn_09111
void CBezierArcThrowEffect::Slot04() {
  if (this->m_particle) {
    this->m_particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x007AB830
// FUNCTION: fn_09112
void CBezierArcThrowEffect::SetTarget(const vector3d& target) {
  this->m_target.x = target.x;
  this->m_target.y = target.y;
  this->m_target.z = target.z;
}
