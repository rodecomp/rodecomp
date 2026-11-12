#include "Contents/Effects/Skills/ShadowExceedEffect.h"
#include "Contents/Actors/ActorEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

namespace {
// INLINED
__forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// INLINED
__forceinline vector3d TransformPoint(const matrix& transform, const vector3d& point) {
  vector3d result;
  result.x = point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9];
  result.y = point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10];
  result.z = point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11];
  return result;
}
}

// OFFSET: 0x00727CF0
// FUNCTION: fn_07427
CShadowExceed::CShadowExceed() : CEffect(1867), finishSpawned(0) {
}

// OFFSET: 0x00727E60
// FUNCTION: fn_07429
void CShadowExceed::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(*it);
  }
  this->emitters.clear();

  CRenderObject* actor = FindRenderObject(this->actor);
  if (actor) {
    while (static_cast<EffectActor*>(actor)->HasId(1004) == true) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00727FC0
// FUNCTION: fn_07431
void CShadowExceed::Open() {
  this->finishSpawned = 0;

  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  const int direction = actor->render.previousAnimation % 8;
  if (static_cast<unsigned int>(direction - 2) <= 3) {
    CEZ2STRParticle* shadow = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    shadow->Load(std::string("shadow_exceed\\shadow_exceed\\shadow_exceed_b_shadow.str"));
    if (direction == 4 || direction == 5) {
      shadow->m_flipX = true;
    }
    this->emitters.push_back(shadow);

    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    bottom->Load(std::string("shadow_exceed\\shadow_exceed\\shadow_exceed_b_shadow_bottom.str"));
    this->emitters.push_back(bottom);
  } else {
    CEZ2STRParticle* shadow = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    shadow->Load(std::string("shadow_exceed\\shadow_exceed\\shadow_exceed_f_shadow.str"));
    if (direction == 0 || direction == 1) {
      shadow->m_flipX = true;
    }
    this->emitters.push_back(shadow);

    CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
    bottom->Load(std::string("shadow_exceed\\shadow_exceed\\shadow_exceed_f_shadow_bottom.str"));
    this->emitters.push_back(bottom);
  }

  const float offsetY = ClientCosine(static_cast<int>(actor->render.angle)) * -3.5f - 7.3f;
  const float offsetX = 0.3f - ClientSine(static_cast<int>(actor->render.angle)) * 5.0f;
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->offsetX = offsetX;
      (*it)->offsetY = offsetY;
      (*it)->offsetZ = 0.0f;
    }
  }

  this->AddEvent(0, "effect\\SHC_SHADOW_EXCEED.wav", 0, 0);
}

// OFFSET: 0x007289C0
// FUNCTION: fn_07433
bool CShadowExceed::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00728A50
// FUNCTION: fn_07435
void CShadowExceed::OnProcess() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    CEZ2STRParticle* emitter = *it;
    if (IsParticleActive(*it) == true) {
      if (emitter) {
        const vector3d position = {this->position[0], this->position[1], this->position[2]};
        const matrix view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
        const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
        vector3d viewPos = TransformPoint(view, position);
        viewPos.x += emitter->offsetX;
        viewPos.y += emitter->offsetY;
        viewPos.z += emitter->offsetZ;
        emitter->position = TransformPoint(inverse, viewPos);
        emitter->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(emitter);
      it = this->emitters.erase(it);
    }
  }

  if (this->emitters.empty() && !this->finishSpawned) {
    this->finishSpawned = 1;
    CRenderObject* actor = FindRenderObject(this->actor);
    if (actor) {
      while (static_cast<EffectActor*>(actor)->HasId(1004) == true) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      static_cast<EffectActor*>(actor)->LaunchEffect(1004, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
}

// OFFSET: 0x00729230
// FUNCTION: fn_07437
void CShadowExceed::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
