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

// INLINED
static CEZ2STRParticle* AsParticle(EZ2STREmitter* emitter) {
  return reinterpret_cast<CEZ2STRParticle*>(emitter);
}

// OFFSET: 0x0070C070

// OFFSET: 0x0070C150
// FUNCTION: fn_07126
CRootTwistEffect::CRootTwistEffect() : CEffect(1110) {
}

// OFFSET: 0x0070C0B0
// FUNCTION: fn_07124
void CCrimsonArrowEffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0] + this->emitter->offset.x;
  particle->position.y = this->position[1] + this->emitter->offset.y;
  particle->position.z = this->position[2] + this->emitter->offset.z;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0070C120
// FUNCTION: fn_07125
void CCrimsonArrowEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0070C3B0

// OFFSET: 0x0070C3D0
// FUNCTION: fn_07131
bool CRootTwistEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070C3E0

// OFFSET: 0x0070C7F0

// OFFSET: 0x0070CEA0
// FUNCTION: fn_07141
CSpriteMableEffect::CSpriteMableEffect() : CEffect(1096) {
}

// OFFSET: 0x0070D150
// FUNCTION: fn_07145
bool CSpriteMableEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070D300
// FUNCTION: fn_07148
CStemSpearEffect::CStemSpearEffect() : CEffect(1100) {
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

// OFFSET: 0x0070D850
// FUNCTION: fn_07156
CSvgSpiritEffect::CSvgSpiritEffect() : CEffect(1128) {
}

// OFFSET: 0x0070D690

// OFFSET: 0x0070DB30
// FUNCTION: fn_07160
bool CSvgSpiritEffect::Slot06() {
  return this->emitter->elapsed < this->unknown_002C;
}

// OFFSET: 0x0070DB50

// OFFSET: 0x0070DCF0
// FUNCTION: fn_07163
CTunaPartyEffect::CTunaPartyEffect() : CEffect(1097) {
}

// OFFSET: 0x0070DF30
// FUNCTION: fn_07167
bool CTunaPartyEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070E0C0
// FUNCTION: fn_07170
CCurseOfCubeEffect::CCurseOfCubeEffect()
    : CEffect(-1), unknown_00D8(0), unknown_00DC(0), unknown_00E0(-10.0f), unknown_00E4(0) {
}

// OFFSET: 0x0070EB60
// FUNCTION: fn_07178
bool CCurseOfCubeEffect::Slot06() {
  if (!this->ActorExists() || this->emitters.empty()) {
    return false;
  }

  return true;
}

// OFFSET: 0x0070F080
// FUNCTION: fn_07180
void CCurseOfCubeEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x0070F0D0
// FUNCTION: fn_07181
CWarningPlaneEffect::CWarningPlaneEffect() : CEffect(1114), fadingIn(0), expireTime(0) {
}

// OFFSET: 0x0070F350
// FUNCTION: fn_07185
void CWarningPlaneEffect::Slot01() {
}

// OFFSET: 0x0070F360

// OFFSET: 0x0070FC60

// OFFSET: 0x0070F720
// FUNCTION: fn_07189
CCylinderEffect::CCylinderEffect() : CEffect(-1) {
}

// OFFSET: 0x0070F670
// FUNCTION: fn_07188
void CWarningPlaneEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (this->expireTime != 0 && *it) {
      AsParticle(*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x0070FFA0
// FUNCTION: fn_07196
void CCylinderEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* particle = AsParticle(*it);
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, static_cast<float>(texPart + 1) / 21.0, 1.0f,
                               static_cast<float>(texPart) / 21.0, false, true};
    particle->Render(quad);
  }
}

// OFFSET: 0x007100D0
// FUNCTION: fn_07197
CDancingKnifeEffect::CDancingKnifeEffect() : CEffect(1829) {
}

// OFFSET: 0x00710830
// FUNCTION: fn_07203
bool CDancingKnifeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00710A20
// FUNCTION: fn_07205
void CDancingKnifeEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }

  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00710AE0
// FUNCTION: fn_07206
CEternalSlashAttackEffect::CEternalSlashAttackEffect() : CEffect(-1) {
}

// OFFSET: 0x00710F50
// FUNCTION: fn_07210
bool CEternalSlashAttackEffect::Slot06() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00710FB0
// FUNCTION: fn_07211
void CEternalSlashAttackEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    particle->position.x = this->position[0] + emitter->offset.x;
    particle->position.y = this->position[1] + emitter->offset.y;
    particle->position.z = this->position[2] + emitter->offset.z;
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x007110A0
// FUNCTION: fn_07212
void CEternalSlashAttackEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00711120
// FUNCTION: fn_07213
CEternalSlashCountEffect::CEternalSlashCountEffect() : CEffect(1913), emitter(nullptr) {
}

// OFFSET: 0x00711340
// FUNCTION: fn_07217
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

// OFFSET: 0x00711380
// FUNCTION: fn_07218
void CEternalSlashCountEffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0] + this->emitter->offset.x;
  particle->position.y = this->position[1] + this->emitter->offset.y;
  particle->position.z = this->position[2] + this->emitter->offset.z;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x007113F0
// FUNCTION: fn_07219
void CEternalSlashCountEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00711420
// FUNCTION: fn_07220
CFallenAngelEffect::CFallenAngelEffect() : CEffect(1140), emitter(nullptr), unknown_00D0(0), unknown_00D4(0), unknown_00D8(0) {
}

// OFFSET: 0x00711690
// FUNCTION: fn_07224
bool CFallenAngelEffect::Slot06() {
  if (this->emitter->lifetime == 0.0f) {
    return true;
  }

  return static_cast<float>(this->emitter->elapsed) < this->emitter->lifetime * 60.0f;
}

// OFFSET: 0x007116D0
// FUNCTION: fn_07225
void CFallenAngelEffect::Slot03() {
  EZ2STREmitter* emitter = this->emitter;
  const vector3d start = emitter->position;
  const float dx = this->unknown_00D0 - this->position[0];
  const float dz = this->unknown_00D8 - this->position[2];
  const float speed = 1.0f / SquareRoot(dx * dx + dz * dz);

  AsParticle(emitter)->Process(0.0f, -80.0f, 0.0f);

  emitter->position.x = start.x + dx * speed * 0.9f;
  emitter->position.y = start.y;
  emitter->position.z = start.z + dz * speed * 0.9f;
  emitter->elapsed++;
}

// OFFSET: 0x007117F0
// FUNCTION: fn_07226
void CFallenAngelEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x007118C0
// FUNCTION: fn_07228
CFloralFlareRoad::CFloralFlareRoad() : CEffect(1804) {
}

// OFFSET: 0x00711DA0
// FUNCTION: fn_07232
bool CFloralFlareRoad::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00711ED0
// FUNCTION: fn_07234
void CFloralFlareRoad::Slot04() {
  if (this->unknown_00CC) {
    AsParticle(this->unknown_00CC)
      ->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00711F80
CFootprintEffect::CFootprintEffect() : CEffect(2411), stepDistance(0.0), unknown_0110(1), lastPosition{}, unknown_0120(-1000.0f) {
  this->lastPosition.x = this->position[0];
  this->lastPosition.y = this->position[1];
  this->lastPosition.z = this->position[2];
}

// OFFSET: 0x007123C0
// FUNCTION: fn_07238
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

// OFFSET: 0x00712C50
// FUNCTION: fn_07244
void CFootprintEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->Render(true, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00713370
// FUNCTION: fn_07248
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

// OFFSET: 0x00713F70
// FUNCTION: fn_07253
void CFootprintStrEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->DrawGround(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    AsParticle(*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
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

// OFFSET: 0x00716800
// FUNCTION: fn_07274
CFullScreenEffect::CFullScreenEffect() : CEffect(-1), loaded(1) {
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

// OFFSET: 0x007176E0
// FUNCTION: fn_07280
CGaleStorm::CGaleStorm() : CEffect(1973), unknown_00D8(5), unknown_00DC(0) {
}

// OFFSET: 0x00717EB0
// FUNCTION: fn_07286
bool CGaleStorm::Slot06() {
  if (this->frame > this->unknown_002C && this->emitters.empty()) {
    return false;
  }

  return this->ActorExists();
}

// OFFSET: 0x00718040
// FUNCTION: fn_07288
void CGaleStorm::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x007184D0
CHawkVumerangEffect::CHawkVumerangEffect()
    : CEffect(1968),
      unknown_00E0(10),
      unknown_00E4(0),
      unknown_00E8(6.283185307179586),
      unknown_00F0(0),
      unknown_00F4(-17.0f),
      unknown_00F8(0),
      unknown_0100{},
      unknown_010C{},
      unknown_0118{} {
}

// OFFSET: 0x00718B90
// FUNCTION: fn_07299
bool CHawkVumerangEffect::Slot06() {
  return this->emitters.size() != 0;
}

// OFFSET: 0x00718E70
void CHawkVumerangEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter) {
      AsParticle(emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00719030
CDropItemEffect::CDropItemEffect()
    : CEffect(-1), emitterCount(2), soundPath("wav\\effect\\"), position{} {
  this->emitters.resize(this->emitterCount);
}

// OFFSET: 0x00719C30
// FUNCTION: fn_07311
void CDropItemEffect::Slot03() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      AsParticle(this->emitters[i])->Process(0.0f, 0.0f, 0.0f);
      this->emitters[i]->elapsed++;
    }
  }
}

// OFFSET: 0x00719CA0
void CDropItemEffect::Slot04() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i]) {
      AsParticle(this->emitters[i])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0071B8C0
// FUNCTION: fn_07318
CJupitelThunderHit::CJupitelThunderHit() : CEffect(-1) {
  this->unknown_002C = 250;
}

// OFFSET: 0x0071C540
// FUNCTION: fn_07324
bool CJupitelThunderHit::Slot06() {
  if (this->id == 1200) {
    return this->ActorExists();
  }

  if (this->actor == 0) {
    return true;
  }

  return this->frame < this->unknown_002C;
}

// OFFSET: 0x0071CB70
// FUNCTION: fn_07326
void CJupitelThunderHit::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && emitter->elapsed >= 0) {
      AsParticle(emitter)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (emitter && emitter->elapsed >= 0) {
      AsParticle(emitter)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x0071CCB0
// FUNCTION: fn_07327
CNewCloudKillEffect::CNewCloudKillEffect() : CEffect(1196) {
}

// OFFSET: 0x0071DEC0
// FUNCTION: fn_07340
bool CNewCloudKillEffect::Slot06() {
  return this->ActorExists();
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

// OFFSET: 0x0071FA90
// FUNCTION: fn_07349
void CPetitioEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    particle->position.x = this->position[0];
    particle->position.y = this->position[1] - 12.0f;
    particle->position.z = this->position[2];
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0071FB70
// FUNCTION: fn_07350
void CPetitioEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x0071FBF0
// FUNCTION: fn_07351
CPneumaticusProcellaEffect::CPneumaticusProcellaEffect() : CEffect(1957) {
}

// OFFSET: 0x00720410
// FUNCTION: fn_07356
void CPneumaticusProcellaEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      AsParticle(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00720480
// FUNCTION: fn_07357
CRandomHitEffect::CRandomHitEffect() : CEffect(-1), active(true) {
}

// OFFSET: 0x007208A0
// FUNCTION: fn_07362
void CRandomHitEffect::Slot03() {
  if (!this->emitter) {
    return;
  }

  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0] + this->emitter->offset.x;
  particle->position.y = this->position[1] + this->emitter->offset.y;
  particle->position.z = this->position[2] + this->emitter->offset.z;
  particle->Process(0.0f, 0.0f, 0.0f);
  this->active = particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00720940
// FUNCTION: fn_07363
void CRandomHitEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
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

// OFFSET: 0x00721430
// FUNCTION: fn_07373
void CRandomOptEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    if (texPart <= 0) {
      continue;
    }

    const ParticleQuad quad = {false, 0.0f, static_cast<float>(texPart) / 21.0f, 1.0f,
                               static_cast<float>(texPart - 1) / 21.0f, false, true};
    particle->Render(quad);
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

// OFFSET: 0x00721F80
// FUNCTION: fn_07380
void CRichsCoinEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->coins.begin(); it != this->coins.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
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

// OFFSET: 0x007224A0
// FUNCTION: fn_07386
void CSavageImpactEffect::Slot03() {
  CEZ2STRParticle* particle = AsParticle(this->emitter);
  particle->position.x = this->position[0];
  particle->position.y = this->position[1] - 12.0f;
  particle->position.z = this->position[2];
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x00722560
// FUNCTION: fn_07387
void CSavageImpactEffect::Slot04() {
  AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
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

// OFFSET: 0x00724F40
// FUNCTION: fn_07408
void CServantWeaponNewEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    AsParticle(*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (auto it = this->trails.begin(); it != this->trails.end(); ++it) {
    AsParticle(*it)->DrawBillboards(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00725130
// FUNCTION: fn_07410
CSevereRainStorm::CSevereRainStorm() : CEffect(-1) {
  this->unknown_0050 = 0;
}
