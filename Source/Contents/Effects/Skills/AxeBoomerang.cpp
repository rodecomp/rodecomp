#include "Contents/Effects/Skills/AxeBoomerang.h"
#include "UI/GameSettings/TalkType.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <string>

// OFFSET: 0x006F5820
// FUNCTION: fn_06837
CAxeBoomerang::CAxeBoomerang() : CEffect(-1) {
  this->particlePosition.x = 0.0f;
  this->particlePosition.y = 0.0f;
  this->particlePosition.z = 0.0f;
  this->launchPosition.x = 0.0f;
  this->rotationParameter = 0;
  this->launchPosition = vector3d{0.0f, 0.0f, 0.0f};
}

// OFFSET: 0x006F58D0
// FUNCTION: fn_46149
__declspec(noinline) CAxeBoomerang::~CAxeBoomerang() {
}

// OFFSET: 0x006F5990
// FUNCTION: fn_46150
__forceinline vector3d CAxeBoomerang::EvaluateTrajectory(vector3d start, vector3d control, vector3d end, float t) {
  const float oneMinusT = 1.0f - t;
  const float startWeight = static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
  const float endWeight = static_cast<float>(std::pow(static_cast<double>(t), 2.0));
  const float controlWeight = (t + t) * oneMinusT;
  return vector3d{
      controlWeight * control.x + endWeight * end.x + startWeight * start.x,
      controlWeight * control.y + endWeight * end.y + startWeight * start.y,
      controlWeight * control.z + endWeight * end.z + startWeight * start.z};
}

// OFFSET: 0x006F5A60
// FUNCTION: fn_06839
void CAxeBoomerang::Close() {
  FreeParticle(this->emitter);
}

// OFFSET: 0x006F5A70
// FUNCTION: fn_06840
void CAxeBoomerang::Open() {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->particlePosition, 5));
  if (this->id != 0x887) {
    if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
      particle->Open("mineffect\\new_axeboomerang\\new_axeboomerang\\new_axeboomerang.str");
    } else {
      particle->Open("new_axeboomerang\\new_axeboomerang\\new_axeboomerang.str");
    }
  } else {
    particle->Load(std::string("shield_shooting\\shield_shooting\\shield_shooting.str"));
  }

  particle->m_loop = true;
  this->emitter = static_cast<CEZ2STRParticle*>(particle);
  this->alive = 1;
  this->returning = 0;
  this->processCount = 0;
  this->trajectoryTime = 1.0f;
}

// OFFSET: 0x006F5CD0
// FUNCTION: fn_06841
bool CAxeBoomerang::IsAlive() {
  return this->alive != 0;
}

// OFFSET: 0x006F5CE0
// FUNCTION: fn_06842
void CAxeBoomerang::OnProcess() {
  this->UpdateParticle(this->emitter);
  ++this->processCount;
}

// OFFSET: 0x006F6060
// FUNCTION: fn_46151
__forceinline void CAxeBoomerang::UpdateParticle(CEZ2STRParticle* emitter) {
  if (!emitter) {
    return;
  }
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    this->alive = 0;
    return;
  }

  CRenderObject* sourceActor = FindRenderObject(this->sourceActor);
  if (sourceActor) {
    this->launchPosition = sourceActor->render.position;
  }

  float scale;
  float angle;
  if (this->returning == 0) {
    this->trajectoryTime -= 0.05f;
    if (this->trajectoryTime <= 0.0f) {
      this->returning = 1;
    }
    scale = 20.0f;
    angle = 1.745329f;
  } else {
    this->trajectoryTime += 0.02f;
    scale = 40.0f;
    angle = 4.537855f;
  }

  matrix rotation;
  rotation.MakeYRotation(angle);

  vector3d delta;
  delta.y = this->launchPosition.y - actor->render.position.y;
  delta.x = this->launchPosition.x - actor->render.position.x;
  delta.z = this->launchPosition.z - actor->render.position.z;
  const float invDist = 1.0f / SquareRoot(delta.y * delta.y + delta.x * delta.x + delta.z * delta.z);
  const vector3d target = actor->render.position;
  const vector3d launch = this->launchPosition;

  vector3d offset;
  offset.x = invDist * delta.x * scale;
  offset.y = invDist * delta.y * scale;
  offset.z = invDist * delta.z * scale;

  const vector3d control{
      offset.y * rotation.values[3] + offset.x * rotation.values[0] + offset.z * rotation.values[6] + rotation.values[9] + this->launchPosition.x,
      offset.y * rotation.values[4] + offset.x * rotation.values[1] + offset.z * rotation.values[7] + rotation.values[10] + this->launchPosition.y,
      offset.y * rotation.values[5] + offset.x * rotation.values[2] + offset.z * rotation.values[8] + rotation.values[11] + this->launchPosition.z};
  this->particlePosition = this->EvaluateTrajectory(launch, control, target, this->trajectoryTime);

  if (1.0f <= this->trajectoryTime) {
    this->alive = 0;
  }

  emitter->position = this->particlePosition;
  emitter->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x006F63F0
// FUNCTION: fn_06843
void CAxeBoomerang::Render() {
  this->RenderParticle(this->emitter);
}

// OFFSET: 0x006F6420
// FUNCTION: fn_46152
__forceinline void CAxeBoomerang::RenderParticle(CEZ2STRParticle* particle) {
  if (particle) {
    CGameMode* mode = g_modeMgr.GetGameMode();
    particle->DrawScreen(mode->camera->frame.view);
  }
}
