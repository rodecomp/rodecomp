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
  this->unknown_00DC.x = 0.0f;
  this->unknown_00DC.y = 0.0f;
  this->unknown_00DC.z = 0.0f;
  this->unknown_00E8.x = 0.0f;
  this->unknown_0050 = 0;
  this->unknown_00E8.x = 0.0f;
  this->unknown_00E8.y = 0.0f;
  this->unknown_00E8.z = 0.0f;
}

// OFFSET: 0x006F5A60
// FUNCTION: fn_06839
void CAxeBoomerang::Close() {
  FreeParticle(reinterpret_cast<CParticle*>(this->unknown_00CC));
}

// OFFSET: 0x006F5A70
// FUNCTION: fn_06840
void CAxeBoomerang::Slot01() {
  this->unknown_00CC = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(&this->unknown_00DC, 5));
  CEZ2STRParticle* particle = reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC);
  if (this->id == 0x887) {
    particle->Load(std::string("shield_shooting\\shield_shooting\\shield_shooting.str"));
  } else if (g_serviceOptions.Get(229) == 0) {
    particle->Open(std::string("new_axeboomerang\\new_axeboomerang\\new_axeboomerang.str"));
  } else {
    particle->Open(std::string("mineffect\\new_axeboomerang\\new_axeboomerang\\new_axeboomerang.str"));
  }

  particle->m_loop = true;
  this->unknown_00D0 = 1;
  this->unknown_00D1 = 0;
  this->unknown_00D4 = 0;
  this->unknown_00D8 = 1.0f;
}

// OFFSET: 0x006F5CD0
// FUNCTION: fn_06841
bool CAxeBoomerang::Slot06() {
  return this->unknown_00D0 != 0;
}

// OFFSET: 0x006F5CE0
// FUNCTION: fn_06842
void CAxeBoomerang::Slot03() {
  if (this->unknown_00CC) {
    CRenderObject* actor = FindRenderObject(this->actor);
    if (!actor) {
      this->unknown_00D0 = 0;
    } else {
      CRenderObject* sourceActor = FindRenderObject(this->sourceActor);
      if (sourceActor) {
        this->unknown_00E8 = sourceActor->render.position;
      }

      float scale;
      float angle;
      if (this->unknown_00D1 == 0) {
        this->unknown_00D8 -= 0.05f;
        if (this->unknown_00D8 <= 0.0f) {
          this->unknown_00D1 = 1;
        }
        scale = 20.0f;
        angle = 1.745329f;
      } else {
        this->unknown_00D8 += 0.02f;
        scale = 40.0f;
        angle = 4.537855f;
      }

      matrix rotation;
      rotation.MakeYRotation(angle);

      const float dy = this->unknown_00E8.y - actor->render.position.y;
      const float dx = this->unknown_00E8.x - actor->render.position.x;
      const float dz = this->unknown_00E8.z - actor->render.position.z;
      const float length = SquareRoot(dy * dy + dx * dx + dz * dz);
      const float invDist = 1.0f / length;

      const float offsetX = invDist * dx * scale;
      const float offsetY = invDist * dy * scale;
      const float offsetZ = invDist * dz * scale;

      const float t = this->unknown_00D8;
      const float oneMinusT = 1.0f - t;
      const float term0 = static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
      const float term1 = (t + t) * oneMinusT;
      const float term2 = static_cast<float>(std::pow(static_cast<double>(t), 2.0));

      const bool finished = 1.0f <= t;

      this->unknown_00DC.x = (offsetY * rotation.values[3] + offsetX * rotation.values[0] + offsetZ * rotation.values[6] + rotation.values[9] + this->unknown_00E8.x) * term1 + term2 * actor->render.position.x + term0 * this->unknown_00E8.x;
      this->unknown_00DC.y = (offsetY * rotation.values[4] + offsetX * rotation.values[1] + offsetZ * rotation.values[7] + rotation.values[10] + this->unknown_00E8.y) * term1 + term2 * actor->render.position.y + term0 * this->unknown_00E8.y;
      this->unknown_00DC.z = (offsetY * rotation.values[5] + offsetX * rotation.values[2] + offsetZ * rotation.values[8] + rotation.values[11] + this->unknown_00E8.z) * term1 + term2 * actor->render.position.z + term0 * this->unknown_00E8.z;

      if (finished) {
        this->unknown_00D0 = 0;
      }

      this->unknown_00CC->position = this->unknown_00DC;
      reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->Process(0.0f, 0.0f, 0.0f);
    }
  }

  ++this->unknown_00D4;
}

// OFFSET: 0x006F63F0
// FUNCTION: fn_06843
void CAxeBoomerang::Slot04() {
  if (this->unknown_00CC) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->DrawScreen(mode->camera->frame.view);
  }
}
