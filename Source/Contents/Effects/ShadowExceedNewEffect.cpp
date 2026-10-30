#include "Contents/Effects/SkillEffects.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectAttachments.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/Vector3d.h"

#include <cmath>
#include <string>

// OFFSET: 0x00727EF0
// FUNCTION: fn_07430
void CShadowExceedNew::Close() {
  for (auto it = this->frontEmitters.begin(); it != this->frontEmitters.end(); ++it) {
    FreeParticle(*it);
  }
  this->frontEmitters.clear();

  for (auto it = this->backEmitters.begin(); it != this->backEmitters.end(); ++it) {
    FreeParticle(*it);
  }
  this->backEmitters.clear();
}

// OFFSET: 0x00728500
// FUNCTION: fn_07432
void CShadowExceedNew::Slot01() {
  this->retained = 1;
  this->AddEvent(0, "effect\\SHC_SHADOW_EXCEED.wav", 0, 0);

  CEZ2STRParticle* emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (emitter) {
    emitter->Load(std::string("SHADOW_CROSS\\SHC_SHADOW_EXCEED\\shadow_exceed\\shadow_exceed_f_shadow_bottom.str"));
    emitter->unknown_00D0 = 0.0f;
    emitter->unknown_00D4 = -12.0f;
    emitter->unknown_00D8 = 0.0f;
    emitter->m_scale = 0.14f;
    this->frontEmitters.push_back(emitter);
  }

  emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (emitter) {
    emitter->Load(std::string("SHADOW_CROSS\\SHC_SHADOW_EXCEED\\shadow_exceed\\shadow_exceed_f_shadow.str"));
    emitter->unknown_00D0 = 0.0f;
    emitter->unknown_00D4 = -10.0f;
    emitter->unknown_00D8 = 0.0f;
    emitter->m_scale = 0.14f;
    this->frontEmitters.push_back(emitter);
  }

  emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (emitter) {
    emitter->Load(std::string("SHADOW_CROSS\\SHC_SHADOW_EXCEED\\shadow_exceed\\shadow_exceed_b_shadow_bottom.str"));
    emitter->unknown_00D0 = 0.0f;
    emitter->unknown_00D4 = -12.0f;
    emitter->unknown_00D8 = 0.0f;
    emitter->m_scale = 0.14f;
    emitter->depthOffset = 0.0003f;
    this->backEmitters.push_back(emitter);
  }

  emitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (emitter) {
    emitter->Load(std::string("SHADOW_CROSS\\SHC_SHADOW_EXCEED\\shadow_exceed\\shadow_exceed_b_shadow.str"));
    emitter->unknown_00D0 = 0.0f;
    emitter->unknown_00D4 = -10.0f;
    emitter->unknown_00D8 = 0.0f;
    emitter->m_scale = 0.14f;
    emitter->depthOffset = 0.0003f;
    this->backEmitters.push_back(emitter);
  }
}

// OFFSET: 0x007289D0
// FUNCTION: fn_07434
bool CShadowExceedNew::Slot06() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (actor) {
    if (this->frontEmitters.size() != 0 && this->backEmitters.size() != 0) {
      return true;
    }
    if (!static_cast<EffectActor*>(actor)->state.Attachments().HasId(1004)) {
      static_cast<EffectActor*>(actor)->LaunchEffect(1004, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
  return false;
}

// OFFSET: 0x00728DC0
// FUNCTION: fn_07436
void CShadowExceedNew::Slot03() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  const float angle = actor->render.angle / 180.0f * 3.141592f;
  const float cosine = std::cos(angle);
  const float sine = std::sin(angle);
  const float y = actor->render.position.y + 0.0f;
  const float z = cosine * 5.0f + actor->render.position.z;
  const float x = -sine * 5.0f + actor->render.position.x;
  const int direction = actor->render.previousAnimation % 8;
  this->unknown_00CC = static_cast<unsigned int>(direction - 2) < 4;

  for (auto it = this->frontEmitters.begin(); it != this->frontEmitters.end();) {
    CEZ2STRParticle* emitter = *it;
    emitter->position.x = x + emitter->unknown_00D0;
    emitter->position.y = y + emitter->unknown_00D4;
    emitter->position.z = z + emitter->unknown_00D8;
    emitter->m_flipX = direction == 0 || direction == 1;
    if (!emitter->Process(0.0f, 0.0f, 0.0f)) {
      FreeParticle(*it);
      it = this->frontEmitters.erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = this->backEmitters.begin(); it != this->backEmitters.end();) {
    CEZ2STRParticle* emitter = *it;
    emitter->position.x = x + emitter->unknown_00D0;
    emitter->position.y = y + emitter->unknown_00D4;
    emitter->position.z = z + emitter->unknown_00D8;
    emitter->m_flipX = direction == 4 || direction == 5;
    if (!emitter->Process(0.0f, 0.0f, 0.0f)) {
      FreeParticle(*it);
      it = this->backEmitters.erase(it);
    } else {
      ++it;
    }
  }
}

// OFFSET: 0x00729280
// FUNCTION: fn_07438
void CShadowExceedNew::Slot04() {
  if (!this->unknown_00CC) {
    for (auto it = this->frontEmitters.begin(); it != this->frontEmitters.end(); ++it) {
      (*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  } else {
    for (auto it = this->backEmitters.begin(); it != this->backEmitters.end(); ++it) {
      (*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
