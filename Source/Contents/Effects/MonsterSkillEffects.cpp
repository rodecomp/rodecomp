#include "Contents/Effects/MonsterSkillEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/GravityControlEffect.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SkillEffects.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <cstdlib>
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

// INLINED
static CSpriteParticle* AsSprite(EZ2STREmitter* emitter) {
  return reinterpret_cast<CSpriteParticle*>(emitter);
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
// FUNCTION: fn_07130
void CRootTwistEffect::Slot12() {
  if (this->id == 1110) {
    this->AddEvent(0, "effect\\su_roottwist.wav", 0, 0);
  }
}

// OFFSET: 0x0070C3D0
// FUNCTION: fn_07131
bool CRootTwistEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070C3E0

// OFFSET: 0x0070C440
// FUNCTION: fn_07133
void CRootTwistEffect::Slot04() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->sub_B40F30(1);
  }
}

// OFFSET: 0x0070C7F0
// FUNCTION: fn_07137
bool CSprArrowEffect::Slot06() {
  return this->unknown_00FD;
}

// OFFSET: 0x0070CB80
// FUNCTION: fn_07139
void CSprArrowEffect::Slot04() {
  if (this->unknown_00CC != 0 && this->unknown_00FC) {
    AsSprite(reinterpret_cast<EZ2STREmitter*>(this->unknown_00CC))->sub_B40F30(1);
  }
}

// OFFSET: 0x0070CEA0
// FUNCTION: fn_07141
CSpriteMableEffect::CSpriteMableEffect() : CEffect(1096) {
}

// OFFSET: 0x0070D150
// FUNCTION: fn_07145
bool CSpriteMableEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070D2C0
// FUNCTION: fn_07147
void CSpriteMableEffect::Slot04() {
  if (!g_serviceOptions.Get(240)) {
    return;
  }

  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->sub_B40F30(1);
  }
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
CSvgSpiritEffect::CSvgSpiritEffect() : CEffect(1128), emitter(nullptr) {
}

// OFFSET: 0x0070D690

// OFFSET: 0x0070D810
// FUNCTION: fn_07155
void CStemSpearEffect::Slot04() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    if ((*emitter)->elapsed > 0 && (*emitter)->elapsed < this->unknown_002C) {
      AsSprite(*emitter)->sub_B40F30(1);
    }
  }
}

// OFFSET: 0x0070DB30
// FUNCTION: fn_07160
bool CSvgSpiritEffect::Slot06() {
  return this->emitter->elapsed < this->unknown_002C;
}

// OFFSET: 0x0070DB50

// OFFSET: 0x0070DCD0
// FUNCTION: fn_07162
void CSvgSpiritEffect::Slot04() {
  EZ2STREmitter* emitter = this->emitter;
  if (emitter->elapsed > 0 && emitter->elapsed < this->unknown_002C) {
    AsSprite(emitter)->sub_B40F30(1);
  }
}

// OFFSET: 0x0070DCF0
// FUNCTION: fn_07163
CTunaPartyEffect::CTunaPartyEffect() : CEffect(1097) {
}

// OFFSET: 0x0070DF30
// FUNCTION: fn_07167
bool CTunaPartyEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070E090
// FUNCTION: fn_07169
void CTunaPartyEffect::Slot04() {
  for (auto emitter = this->emitters.begin(); emitter != this->emitters.end(); ++emitter) {
    AsSprite(*emitter)->sub_B40F30(1);
  }
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
// FUNCTION: fn_07194
bool CCylinderEffect::Slot06() {
  return this->startTime + this->unknown_002C >= timeGetTime();
}

// OFFSET: 0x0070F720
// FUNCTION: fn_07189
CCylinderEffect::CCylinderEffect() : CEffect(-1) {
}

// OFFSET: 0x0070F390
// FUNCTION: fn_07187
void CWarningPlaneEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (this->expireTime != 0 && emitter) {
      CEZ2STRParticle* particle = AsParticle(emitter);

      matrix rotation;
      rotation.Identity();
      rotation.MakeXRotation(1.5707963f);
      rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);

      matrix scale;
      scale.Identity();
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix transform;
      transform.Compose(scale, rotation);
      particle->transform = transform;

      if (this->fadingIn == 1) {
        particle->alpha += 2;
        if (particle->alpha >= 250) {
          this->fadingIn = 0;
        }
      } else {
        particle->alpha -= 2;
        if (particle->alpha <= 50) {
          this->fadingIn = 1;
        }
      }
    }

    ++emitter->elapsed;
  }
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
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart / 21.0),
                               static_cast<float>((texPart + 1) / 21.0), false, true};
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

// OFFSET: 0x00711280
// FUNCTION: fn_07216
void CEternalSlashCountEffect::Slot01() {
  if (static_cast<unsigned int>(this->unknown_00B0) - 1 > 4) {
    return;
  }

  CEZ2STRParticle* particle =
      static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (!particle) {
    return;
  }

  particle->Load(FormatOwnedString("eternal_slash\\eternal_slash_count\\eternal_slash_count_%02d.str", this->unknown_00B0));
  particle->unknown_00D0 = 0.0f;
  particle->unknown_00D4 = -25.0f;
  particle->unknown_00D8 = 0.0f;
  this->emitter = reinterpret_cast<EZ2STREmitter*>(particle);
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

  emitter->position.x = start.x + dx * speed * 0.09f;
  emitter->position.y = start.y;
  emitter->position.z = start.z + dz * speed * 0.09f;

  AsParticle(emitter)->Process(0.0f, -80.0f, 0.0f);
  emitter->elapsed++;
}

// OFFSET: 0x007117F0
// FUNCTION: fn_07226
void CFallenAngelEffect::Slot04() {
  if (this->emitter) {
    AsParticle(this->emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x00711850
// FUNCTION: fn_07227
void CFallenAngelEffect::SetPosition(vector3d position) {
  vector3d target = position;
  target.y += (g_cameraDistance - 230.0f) * 0.0625f + 10.0f;

  this->position[0] = target.x;
  this->position[1] = target.y;
  this->position[2] = target.z;
  this->emitter->position = target;
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

// OFFSET: 0x00715C80
// FUNCTION: fn_07270
void CFromTheAbyssEffect::Slot04() {
  for (auto it = this->unknown_00E8.begin(); it != this->unknown_00E8.end(); ++it) {
    EZ2STREmitter* emitter = it->second.emitter;
    if (emitter) {
      AsParticle(emitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
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

// OFFSET: 0x00717DE0
// FUNCTION: fn_07285
void CGaleStorm::Slot01() {
  struct LevelData {
    int count;
    int unknown;
  };
  LevelData table[10] = {{5}, {5}, {5}, {5}, {7}, {7}, {7}, {9}, {9}, {11}};

  unsigned int index = this->unknown_00B0;
  if (index > 10) {
    index = 10;
  }

  this->unknown_00D8 = table[index - 1].count;
  this->unknown_00DC = table[index - 1].unknown;
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

// OFFSET: 0x007183F0
// FUNCTION: fn_07292
void CGravityControlEffect::Slot04() {
  if (!this->target) {
    return;
  }

  AsParticle(reinterpret_cast<EZ2STREmitter*>(this->target))
      ->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
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

// OFFSET: 0x007187F0
// FUNCTION: fn_07297
void CHawkVumerangEffect::Close() {
  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D4.clear();
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00718B90
// FUNCTION: fn_07299
bool CHawkVumerangEffect::Slot06() {
  return this->emitters.size() != 0;
}

// OFFSET: 0x00718E70
// FUNCTION: fn_07301
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
// FUNCTION: fn_07312
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
CNewCloudKillEffect::CNewCloudKillEffect() : CEffect(1196), unknown_00F4(0), unknown_00F8(0) {
}

// OFFSET: 0x0071D460
// FUNCTION: fn_07333
void CNewCloudKillEffect::Close() {
  for (auto it = this->unknown_00CC.begin(); it != this->unknown_00CC.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00CC.clear();
  for (auto it = this->unknown_00D4.begin(); it != this->unknown_00D4.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D4.clear();
  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00DC.clear();
  for (auto it = this->unknown_00E4.begin(); it != this->unknown_00E4.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00E4.clear();
  for (auto it = this->unknown_00EC.begin(); it != this->unknown_00EC.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00EC.clear();
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

// OFFSET: 0x00720310
// FUNCTION: fn_07355
void CPneumaticusProcellaEffect::Slot03() {
  if (this->emitter) {
    AsParticle(this->emitter)->Process(0.0f, -80.0f, 0.0f);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    EZ2STREmitter* emitter = *it;
    if (!emitter) {
      continue;
    }

    if (emitter->lifetime == 0.0f || emitter->lifetime * 60.0f <= static_cast<float>(emitter->elapsed)) {
      FreeParticle(AsParticle(emitter));
      *it = nullptr;
      continue;
    }

    CEZ2STRParticle* particle = AsParticle(emitter);
    particle->position.x = this->position[0] + emitter->offset.x;
    particle->position.y = this->position[1] + emitter->offset.y;
    particle->position.z = this->position[2] + emitter->offset.z;
    particle->Process(0.0f, -80.0f, 0.0f);
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

    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f,
                               static_cast<float>(texPart) / 21.0f, false, true};
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

// OFFSET: 0x007233D0
// FUNCTION: fn_07399
void CServantWeaponNewEffect::Close() {
  for (EZ2STREmitter* emitter : this->emitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->emitters.clear();
  for (EZ2STREmitter* trail : this->trails) {
    FreeParticle(AsParticle(trail));
  }
  this->trails.clear();
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

// OFFSET: 0x00724EB0
// FUNCTION: fn_07407
void CServantWeaponEffect::Slot04() {
  for (int column = 0; column < 5; ++column) {
    if (this->emitters[0][column] != nullptr) {
      AsParticle(this->emitters[0][column])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
    if (this->emitters[1][column] != nullptr) {
      AsParticle(this->emitters[1][column])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
    if (this->emitters[2][column] != nullptr) {
      AsParticle(this->emitters[2][column])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
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

// OFFSET: 0x0071D650
// FUNCTION: fn_07334
void __stdcall fn_07334(CParticle* particle) {
  particle->alpha = 0;
  *reinterpret_cast<int*>(particle->unknown_0048) = rand() % 50 + 150;
  particle->unknown_00A8 = 0;
  particle->unknown_00E8 = 0;
  InitParticleQuad(particle->vertices);

  const int spread = rand() % 15 + 2;
  const int angle = rand() % 360;
  particle->unknown_00D4 = 0.0f;
  particle->unknown_00D8 = static_cast<float>(spread) * g_sinTable[angle];
  particle->unknown_00D0 = static_cast<float>(spread) * g_cosTable[angle];
  particle->scale = static_cast<float>(rand() % 10) * 0.1f + 0.5f;
  particle->unknown_00C0 = static_cast<float>(rand() % 30) * 0.01f + 0.05f;
}

// OFFSET: 0x0071DB70
// FUNCTION: fn_07337
void __stdcall fn_07337(CParticle* particle) {
  particle->unknown_00A9[0] = rand() % 2;

  const int angle = rand() % 360;
  particle->unknown_00D4 = 0.0f;
  particle->alpha = 0xFF;
  particle->unknown_00D0 = g_cosTable[angle] * 18.0f;
  particle->unknown_00D8 = g_sinTable[angle] * 18.0f;
  particle->scale = static_cast<float>(rand() % 31) * 0.1f + 2.0f;
  particle->unknown_00A8 = 0;
  particle->unknown_00C0 = static_cast<float>(rand() % 31) * 0.01f + 0.05f;
  InitParticleQuad(particle->vertices);
}

// OFFSET: 0x0071DC70
// FUNCTION: fn_07338
void __stdcall fn_07338(CParticle* particle) {
  particle->scale = static_cast<float>(rand() % 5) + 10.0f;
  particle->unknown_00A8 = 10;
  particle->unknown_00A9[0] = rand() % 4;
  particle->unknown_004C = rand() % 50 + 10;
  *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(rand() % 360);
  particle->unknown_00D8 = static_cast<float>(rand() % 10) - 5.0f;
  particle->unknown_00D0 = static_cast<float>(rand() % 10) - 5.0f;
  particle->unknown_00D4 = -10.0f;
  particle->unknown_00E8 = 0;
  InitParticleQuad(particle->vertices);
}

// OFFSET: 0x0071DD80
// FUNCTION: fn_07339
void __stdcall fn_07339(CParticle* particle) {
  particle->alpha = 0;
  particle->unknown_00A8 = 0;
  particle->unknown_00E8 = 0;
  InitParticleQuad(particle->vertices);

  particle->scale = static_cast<float>(rand() % 51) * 0.1f + 7.0f;
  particle->unknown_00D4 = particle->scale * -0.5f;

  const int spread = rand() % 13 + 2;
  const int angle = rand() % 360;
  particle->unknown_00D8 = static_cast<float>(spread) * g_sinTable[angle];
  particle->unknown_00D0 = static_cast<float>(spread) * g_cosTable[angle];

  particle->unknown_00A9[0] = rand() % 4;
  *reinterpret_cast<int*>(particle->unknown_0048) = rand() % 151 + 50;
  particle->unknown_00C0 = static_cast<float>(rand() % 10) * 0.01f + 0.05f;
}

// OFFSET: 0x0070C2B0
// FUNCTION: fn_07128
void CRootTwistEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070D000
// FUNCTION: fn_07143
void CSpriteMableEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070DE50
// FUNCTION: fn_07165
void CTunaPartyEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070EAD0
// FUNCTION: fn_07175
void CCurseOfCubeEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070F310
// FUNCTION: fn_07184
void CWarningPlaneEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00710C40
// FUNCTION: fn_07208
void CEternalSlashAttackEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x00716960
// FUNCTION: fn_07276
void CFullScreenEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x00717DA0
// FUNCTION: fn_07284
void CGaleStorm::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x007211E0
// FUNCTION: fn_07369
void CRandomOptEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0070FB10
// FUNCTION: fn_07192
void CCylinderEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070C520
// FUNCTION: fn_07135
void CSprArrowEffect::Close() {
  if (this->unknown_00CC != 0) {
    FreeParticle(reinterpret_cast<CParticle*>(this->unknown_00CC));
    this->unknown_00CC = 0;
  }
}

// OFFSET: 0x0070D460
// FUNCTION: fn_07150
void CStemSpearEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0070D980
// FUNCTION: fn_07158
void CSvgSpiritEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x00711250
// FUNCTION: fn_07215
void CEternalSlashCountEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x007205A0
// FUNCTION: fn_07359
void CRandomHitEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
}

// OFFSET: 0x00711570
// FUNCTION: fn_07222
void CFallenAngelEffect::Close() {
  FreeParticle(AsParticle(this->emitter));
}

// OFFSET: 0x007221B0
// FUNCTION: fn_07383
void CSavageImpactEffect::Close() {
  FreeParticle(AsParticle(this->emitter));
}

// OFFSET: 0x007193B0
// FUNCTION: fn_07306
void CDropItemEffect::Close() {
  for (std::size_t i = 0; i < this->emitters.size(); ++i) {
    if (this->emitters[i] != nullptr) {
      FreeParticle(AsParticle(this->emitters[i]));
    }
  }
}

// OFFSET: 0x00711A20
// FUNCTION: fn_07230
void CFloralFlareRoad::Close() {
  FreeParticle(AsParticle(this->unknown_00CC));
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
}

// OFFSET: 0x0071FD50
// FUNCTION: fn_07353
void CPneumaticusProcellaEffect::Close() {
  if (this->emitter != nullptr) {
    FreeParticle(AsParticle(this->emitter));
    this->emitter = nullptr;
  }
  for (std::vector<EZ2STREmitter*>::iterator it = this->emitters.begin(); it < this->emitters.end(); ++it) {
    if (*it != nullptr) {
      FreeParticle(AsParticle(*it));
      *it = nullptr;
    }
  }
  this->emitters.clear();
}

// OFFSET: 0x00723370
// FUNCTION: fn_07398
void CServantWeaponEffect::Close() {
  for (int column = 0; column < 5; ++column) {
    if (this->emitters[0][column] != nullptr) {
      FreeParticle(AsParticle(this->emitters[0][column]));
      this->emitters[0][column] = nullptr;
    }
    if (this->emitters[1][column] != nullptr) {
      FreeParticle(AsParticle(this->emitters[1][column]));
      this->emitters[1][column] = nullptr;
    }
    if (this->emitters[2][column] != nullptr) {
      FreeParticle(AsParticle(this->emitters[2][column]));
      this->emitters[2][column] = nullptr;
    }
  }
}

// OFFSET: 0x00712350
// FUNCTION: fn_07237
void CFootprintEffect::Close() {
  for (EZ2STREmitter* emitter : this->emitters) {
    FreeParticle(AsParticle(emitter));
  }
  this->emitters.clear();
}

// OFFSET: 0x00710790
// FUNCTION: fn_07201
void CDancingKnifeEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D8.clear();
}

// OFFSET: 0x0071C4B0
// FUNCTION: fn_07322
void CJupitelThunderHit::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->emitters.clear();
  for (auto it = this->unknown_00D8.begin(); it != this->unknown_00D8.end(); ++it) {
    FreeParticle(AsParticle(*it));
  }
  this->unknown_00D8.clear();
}

// OFFSET: 0x00721860
// FUNCTION: fn_07376
void CRichsCoinEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
  for (auto it = this->coins.begin(); it != this->coins.end(); ++it) {
    FreeParticle(*it);
  }
  this->coins.clear();
}
