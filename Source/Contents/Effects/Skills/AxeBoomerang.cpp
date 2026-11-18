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
  if (CEZ2STRParticle* emitter = this->emitter) {
    CRenderObject* actor = FindRenderObject(this->actor);
    if (!actor) {
      this->alive = 0;
    } else {
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

      const float oneMinusT = 1.0f - this->trajectoryTime;
      const float term0 = static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
      const float term2 = static_cast<float>(std::pow(static_cast<double>(this->trajectoryTime), 2.0));
      const float term1 = (this->trajectoryTime + this->trajectoryTime) * oneMinusT;

      const bool finished = 1.0f <= this->trajectoryTime;

      this->particlePosition.x = (offset.y * rotation.values[3] + offset.x * rotation.values[0] + offset.z * rotation.values[6] + rotation.values[9] + this->launchPosition.x) * term1 + term2 * target.x + term0 * launch.x;
      this->particlePosition.y = (offset.y * rotation.values[4] + offset.x * rotation.values[1] + offset.z * rotation.values[7] + rotation.values[10] + this->launchPosition.y) * term1 + term2 * target.y + term0 * launch.y;
      this->particlePosition.z = (offset.y * rotation.values[5] + offset.x * rotation.values[2] + offset.z * rotation.values[8] + rotation.values[11] + this->launchPosition.z) * term1 + term2 * target.z + term0 * launch.z;

      if (finished) {
        this->alive = 0;
      }

      emitter->position = this->particlePosition;
      static_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);
    }
  }

  ++this->processCount;
}

// OFFSET: 0x006F63F0
// FUNCTION: fn_06843
void CAxeBoomerang::Render() {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(this->emitter);
  if (particle) {
    CGameMode* mode = g_modeMgr.GetGameMode();
    particle->DrawScreen(mode->camera->frame.view);
  }
}
