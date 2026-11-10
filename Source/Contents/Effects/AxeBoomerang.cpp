#include "Contents/Effects/AxeBoomerang.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectEmitter.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
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
  this->launchPosition.x = 0.0f;
  this->launchPosition.y = 0.0f;
  this->launchPosition.z = 0.0f;
}

// OFFSET: 0x006F5A60
// FUNCTION: fn_06839
void CAxeBoomerang::Close() {
  FreeParticle(reinterpret_cast<CParticle*>(this->emitter));
}

// OFFSET: 0x006F5A70
// FUNCTION: fn_06840
void CAxeBoomerang::Open() {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->particlePosition, 5));
  if (this->id != 0x887) {
    particle->Open(g_serviceOptions.Get(229) ? "mineffect\\new_axeboomerang\\new_axeboomerang\\new_axeboomerang.str"
                                             : "new_axeboomerang\\new_axeboomerang\\new_axeboomerang.str");
  } else {
    particle->Load(std::string("shield_shooting\\shield_shooting\\shield_shooting.str"));
  }

  particle->m_loop = true;
  this->emitter = reinterpret_cast<EZ2STREmitter*>(particle);
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
  if (this->emitter) {
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

      const float dy = this->launchPosition.y - actor->render.position.y;
      const float dx = this->launchPosition.x - actor->render.position.x;
      const float dz = this->launchPosition.z - actor->render.position.z;
      const float length = SquareRoot(dy * dy + dx * dx + dz * dz);
      const float invDist = 1.0f / length;

      const float offsetX = invDist * dx * scale;
      const float offsetY = invDist * dy * scale;
      const float offsetZ = invDist * dz * scale;

      const float t = this->trajectoryTime;
      const float oneMinusT = 1.0f - t;
      const float term0 = static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
      const float term1 = (t + t) * oneMinusT;
      const float term2 = static_cast<float>(std::pow(static_cast<double>(t), 2.0));

      const bool finished = 1.0f <= t;

      this->particlePosition.x = (offsetY * rotation.values[3] + offsetX * rotation.values[0] + offsetZ * rotation.values[6] + rotation.values[9] + this->launchPosition.x) * term1 + term2 * actor->render.position.x + term0 * this->launchPosition.x;
      this->particlePosition.y = (offsetY * rotation.values[4] + offsetX * rotation.values[1] + offsetZ * rotation.values[7] + rotation.values[10] + this->launchPosition.y) * term1 + term2 * actor->render.position.y + term0 * this->launchPosition.y;
      this->particlePosition.z = (offsetY * rotation.values[5] + offsetX * rotation.values[2] + offsetZ * rotation.values[8] + rotation.values[11] + this->launchPosition.z) * term1 + term2 * actor->render.position.z + term0 * this->launchPosition.z;

      if (finished) {
        this->alive = 0;
      }

      this->emitter->position = this->particlePosition;
      reinterpret_cast<CEZ2STRParticle*>(this->emitter)->Process(0.0f, 0.0f, 0.0f);
    }
  }

  ++this->processCount;
}

// OFFSET: 0x006F63F0
// FUNCTION: fn_06843
void CAxeBoomerang::Render() {
  CEZ2STRParticle* particle = reinterpret_cast<CEZ2STRParticle*>(this->emitter);
  if (particle) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    particle->DrawScreen(mode->camera->frame.view);
  }
}
