#include "Contents/Effects/Skills/BezierArcThrowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// INLINED
static vector3d Subtract(const vector3d& left, const vector3d& right) {
  return vector3d{left.x - right.x, left.y - right.y, left.z - right.z};
}

// INLINED
static vector3d TransformPoint(const matrix& transform, const vector3d& point) {
  vector3d result;
  result.x = point.x * transform.values[0] + point.y * transform.values[3] + point.z * transform.values[6] + transform.values[9];
  result.y = point.x * transform.values[1] + point.y * transform.values[4] + point.z * transform.values[7] + transform.values[10];
  result.z = point.x * transform.values[2] + point.y * transform.values[5] + point.z * transform.values[8] + transform.values[11];
  return result;
}

// OFFSET: 0x007AAEA0
// FUNCTION: fn_09102
CThunderingOrb::CThunderingOrb(const std::string& name) : CBezierArcThrowEffect(name) {}

// OFFSET: 0x007AAF40
// FUNCTION: fn_09103
CWindBomb::CWindBomb(const std::string& name) : CBezierArcThrowEffect(name) {}

// OFFSET: 0x007AAFE0
// FUNCTION: fn_46016
__declspec(noinline) CBezierArcThrowEffect::~CBezierArcThrowEffect() {
}

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
void CBezierArcThrowEffect::Open() {
  this->m_particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
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
void CBezierArcThrowEffect::OnProcess() {
  if (!this->m_active) {
    return;
  }

  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor || !this->m_particle) {
    this->m_active = false;
    return;
  }

  const vector3d actorPos = actor->render.position;
  const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
  vector3d actorView = TransformPoint(view, actorPos);
  float targetViewX = this->m_target.y * view.values[3] +
                      this->m_target.x * view.values[0] +
                      this->m_target.z * view.values[6] +
                      view.values[9];

  matrix rotation;
  if (targetViewX >= actorView.x) {
    rotation.MakeYRotation(240.0f / 180.0f * 3.141592f);
  } else {
    rotation.MakeYRotation(120.0f / 180.0f * 3.141592f);
  }

  actorView.y -= 10.0f;
  vector3d ctrlPt = TransformPoint(g_modeMgr.GetGameMode()->camera->frame.inverse, actorView);
  this->m_progress -= 0.07f;

  const vector3d delta = Subtract(this->m_target, ctrlPt);

  float distSq = delta.y * delta.y + delta.x * delta.x + delta.z * delta.z;
  float dist = SquareRoot(distSq);
  float invDist = 1.0f / SquareRoot(distSq);

  vector3d norm;
  norm.x = delta.x * invDist * dist;
  norm.y = delta.y * invDist * dist;
  norm.z = delta.z * invDist * dist;

  vector3d rotated = TransformPoint(rotation, norm);

  vector3d start = this->m_target;
  float t = this->m_progress;
  float u = 1.0f - t;
  const vector3d first = start * static_cast<float>(std::pow(static_cast<double>(u), 2.0));
  const vector3d last = ctrlPt * static_cast<float>(std::pow(static_cast<double>(t), 2.0));
  vector3d end{rotated.x + this->m_target.x, rotated.y + this->m_target.y, rotated.z + this->m_target.z};
  const vector3d middle = end * ((t + t) * u);

  CEZ2STRParticle* particle = this->m_particle;
  particle->position.x = middle.x + last.x + first.x;
  particle->position.y = middle.y + last.y + first.y;
  particle->position.z = middle.z + last.z + first.z;

  this->m_particle->Process(0.0f, 0.0f, 0.0f);
  if (0.0f >= this->m_progress) {
    FreeParticle(this->m_particle);
    this->m_particle = nullptr;
  }
}

// OFFSET: 0x007AB800
// FUNCTION: fn_09111
void CBezierArcThrowEffect::Render() {
  if (this->m_particle) {
    this->m_particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x007AB830
// FUNCTION: fn_09112
void CBezierArcThrowEffect::SetTarget(const vector3d& target) {
  this->m_target = target;
}
