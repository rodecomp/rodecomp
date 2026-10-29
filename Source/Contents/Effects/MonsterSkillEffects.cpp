#include "Contents/Effects/MonsterSkillEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

#include <windows.h>
#include <timeapi.h>

// INLINED
static void SetScreenVertex(RenderVertex& vertex, float x, float y, std::uint32_t color, float u, float v) {
  vertex.x = x;
  vertex.y = y;
  vertex.z = 0.000001f;
  vertex.rhw = 0.999999f;
  vertex.diffuse = color;
  vertex.specular = 0xFF000000;
  vertex.u = u;
  vertex.v = v;
}

// OFFSET: 0x0070C070

// OFFSET: 0x0070C3B0

// OFFSET: 0x0070C3D0
// FUNCTION: fn_07131
bool CRootTwistEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070C3E0

// OFFSET: 0x0070C7F0

// OFFSET: 0x0070D150
// FUNCTION: fn_07145
bool CSpriteMableEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070D660
// FUNCTION: fn_07153
bool CStemSpearEffect::Slot06() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed < this->unknown_002C) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0070D690

// OFFSET: 0x0070DB30
// FUNCTION: fn_07160
bool CSvgSpiritEffect::Slot06() {
  return this->emitter->elapsed < this->unknown_002C;
}

// OFFSET: 0x0070DB50

// OFFSET: 0x0070DF30
// FUNCTION: fn_07167
bool CTunaPartyEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070EB60
// FUNCTION: fn_07178
bool CCurseOfCubeEffect::Slot06() {
  if (!this->ActorExists() || this->emitters.empty()) {
    return false;
  }

  return true;
}

// OFFSET: 0x0070F350
// FUNCTION: fn_07185
void CWarningPlaneEffect::Slot01() {
}

// OFFSET: 0x0070F360

// OFFSET: 0x0070FC60

// OFFSET: 0x007100D0
CDancingKnifeEffect::CDancingKnifeEffect() : CEffect(1829) {
}

// OFFSET: 0x00710830
// FUNCTION: fn_07203
bool CDancingKnifeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00710AE0
CEternalSlashAttackEffect::CEternalSlashAttackEffect() : CEffect(-1) {
}

// OFFSET: 0x00710F50
bool CEternalSlashAttackEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00711120
CEternalSlashCountEffect::CEternalSlashCountEffect() : CEffect(1913), emitter(nullptr) {
}

// OFFSET: 0x00711340
bool CEternalSlashCountEffect::Slot06() {
  EZ2STREmitter* emitter = this->emitter;
  if (!emitter) {
    return false;
  }

  if (emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f;
}

// OFFSET: 0x00711420
CFallenAngelEffect::CFallenAngelEffect() : CEffect(1140), emitter(nullptr), unknown_00D0(0), unknown_00D4(0), unknown_00D8(0) {
}

// OFFSET: 0x00711690
bool CFallenAngelEffect::Slot06() {
  if (this->emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007118C0
CFloralFlareRoad::CFloralFlareRoad() : CEffect(1804) {
}

// OFFSET: 0x00711DA0
// FUNCTION: fn_07232
bool CFloralFlareRoad::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00711F80
CFootprintEffect::CFootprintEffect() : CEffect(2411), stepDistance(0.0), unknown_0110(1), lastPosition{}, unknown_0120(-1000.0f) {
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
}

// OFFSET: 0x007123C0
bool CFootprintEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x007127A0
// FUNCTION: fn_07240
bool CFootprintEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00713370
bool CFootprintStrEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (g_serviceOptions.Get(269) && this->actor != g_session.m_aid) {
    return false;
  }

  return actor->EffectVisible();
}

// OFFSET: 0x00713830
// FUNCTION: fn_07250
bool CFootprintStrEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00715400
// FUNCTION: fn_07266
void CFromTheAbyssEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00715410
// FUNCTION: fn_07267
bool CFromTheAbyssEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00716BD0
// FUNCTION: fn_07278
bool CFullScreenEffect::Slot06() {
  if (this->loaded == 1 && this->ActorExists()) {
    return true;
  }

  return false;
}

// OFFSET: 0x00716BF0
// FUNCTION: fn_07279
void CFullScreenEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    RenderPrimitive& primitive = particle->primitive;
    primitive.texture = particle->texture;
    const float left = g_textureOffsetX / primitive.texture->TextureWidth();
    const float right = (primitive.texture->Width() + g_textureOffsetX) / primitive.texture->TextureWidth();
    const float top = g_textureOffsetY / primitive.texture->TextureHeight();
    const float bottom = (primitive.texture->Height() + g_textureOffsetY) / primitive.texture->TextureHeight();
    const std::uint32_t color = ((particle->alpha << 8 | particle->red) << 8 | particle->green) << 8 | particle->blue;

    RenderVertex* vertices = primitive.quad;
    switch (particle->unknown_000C) {
    case 0:
      SetScreenVertex(vertices[0], 0.0f, 0.0f, color, right, top);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.centerX), 0.0f, color, left, top);
      SetScreenVertex(vertices[2], 0.0f, static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      break;
    case 1:
      SetScreenVertex(vertices[0], static_cast<float>(g_renderer->viewport.centerX), 0.0f, color, left, top);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.width), 0.0f, color, right, top);
      SetScreenVertex(vertices[2], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      break;
    case 2:
      SetScreenVertex(vertices[0], 0.0f, static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      SetScreenVertex(vertices[2], 0.0f, static_cast<float>(g_renderer->viewport.height), color, right, top);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.height), color, left, top);
      break;
    case 3:
      SetScreenVertex(vertices[0], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.centerY), color, left, bottom);
      SetScreenVertex(vertices[1], static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.centerY), color, right, bottom);
      SetScreenVertex(vertices[2], static_cast<float>(g_renderer->viewport.centerX), static_cast<float>(g_renderer->viewport.height), color, left, top);
      SetScreenVertex(vertices[3], static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.height), color, right, top);
      break;
    }

    primitive.unknown_0018 = particle->sourceBlend;
    primitive.unknown_001C = particle->destinationBlend;
    g_renderer->queues.Submit(&primitive, static_cast<std::uint16_t>(particle->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}

// OFFSET: 0x0071F5F0
// FUNCTION: fn_07344
CPetitioEffect::CPetitioEffect() : CEffect(1911) {
}

// OFFSET: 0x0071FA30
// FUNCTION: fn_07348
bool CPetitioEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0071FBF0
// FUNCTION: fn_07351
CPneumaticusProcellaEffect::CPneumaticusProcellaEffect() : CEffect(1957) {
}

// OFFSET: 0x00720480
// FUNCTION: fn_07357
CRandomHitEffect::CRandomHitEffect() : CEffect(-1), active(true) {
}

// OFFSET: 0x00720970
// FUNCTION: fn_07364
CRandomOptEffect::CRandomOptEffect() : CEffect(-1), offsets{} {
}

// OFFSET: 0x00720AF0
// FUNCTION: fn_07366
void CRandomOptEffect::Slot08(int red, int green, int blue) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00720B30
// FUNCTION: fn_07367
void CRandomOptEffect::Slot11(std::string name) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetTexture(name);
  }
}

// OFFSET: 0x00721560
// FUNCTION: fn_07374
CRichsCoinEffect::CRichsCoinEffect() : CEffect(1135), unknown_00E4(0) {
}

// OFFSET: 0x00721AE0
// FUNCTION: fn_07378
bool CRichsCoinEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00722070
// FUNCTION: fn_07381
CSavageImpactEffect::CSavageImpactEffect() : CEffect(1837), offset{} {
}

// OFFSET: 0x00722460
// FUNCTION: fn_07385
bool CSavageImpactEffect::Slot06() {
  if (this->emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007226A0
// FUNCTION: fn_07389
CServantWeaponEffect::CServantWeaponEffect() : CEffect(-1), offsets{} {
}

// OFFSET: 0x00722730
// FUNCTION: fn_07390
CServantWeaponNewEffect::CServantWeaponNewEffect() : CEffect(2460) {
}

// OFFSET: 0x00723A90
// FUNCTION: fn_07401
void CServantWeaponNewEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00723AA0
// FUNCTION: fn_07402
bool CServantWeaponEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00723AB0
// FUNCTION: fn_07403
bool CServantWeaponNewEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00725130
// FUNCTION: fn_07410
CSevereRainStorm::CSevereRainStorm() : CEffect(-1) {
  this->unknown_0050 = 0;
}
