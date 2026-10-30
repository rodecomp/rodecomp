#include "Contents/Effects/ShadowExceedEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectAttachments.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

// OFFSET: 0x00727CF0
// FUNCTION: fn_07427
CShadowExceed::CShadowExceed() : CEffect(1867), unknown_00D8(0) {
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
    while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(1004) == true) {
      actor->SendMsg(nullptr, 109, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00727FC0
// FUNCTION: fn_07431
void CShadowExceed::Slot01() {
  this->unknown_00D8 = 0;

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
      (*it)->unknown_00D0 = offsetX;
      (*it)->unknown_00D4 = offsetY;
      (*it)->unknown_00D8 = 0.0f;
    }
  }

  this->AddEvent(0, "effect\\SHC_SHADOW_EXCEED.wav", 0, 0);
}

// OFFSET: 0x007289C0
// FUNCTION: fn_07433
bool CShadowExceed::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00728A50
// FUNCTION: fn_07435
void CShadowExceed::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end();) {
    CEZ2STRParticle* emitter = *it;
    if (emitter->life == 0.0f || emitter->life * 60.0f > static_cast<float>(emitter->unknown_00E8)) {
      if (emitter) {
        const float x = this->position[0];
        const float y = this->position[1];
        const float z = this->position[2];
        const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
        const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
        const float viewX = view[0] * x + view[3] * y + view[6] * z + view[9] + emitter->unknown_00D0;
        const float viewY = view[1] * x + view[4] * y + view[7] * z + view[10] + emitter->unknown_00D4;
        const float viewZ = view[2] * x + view[5] * y + view[8] * z + view[11] + emitter->unknown_00D8;
        emitter->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
        emitter->position.y = viewY * inverse[4] + viewX * inverse[1] + viewZ * inverse[7] + inverse[10];
        emitter->position.z = viewY * inverse[5] + viewX * inverse[2] + viewZ * inverse[8] + inverse[11];
        emitter->Process(0.0f, 0.0f, 0.0f);
      }
      ++it;
    } else {
      FreeParticle(emitter);
      it = this->emitters.erase(it);
    }
  }

  if (this->emitters.empty() && !this->unknown_00D8) {
    this->unknown_00D8 = 1;
    CRenderObject* actor = FindRenderObject(this->actor);
    if (actor) {
      while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(1004) == true) {
        actor->SendMsg(nullptr, 109, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      static_cast<EffectActor*>(actor)->LaunchEffect(1004, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
}

// OFFSET: 0x00729230
// FUNCTION: fn_07437
void CShadowExceed::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
