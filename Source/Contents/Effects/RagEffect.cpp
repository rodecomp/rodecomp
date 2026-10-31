#include "Contents/Effects/RagEffect.h"
#include "Contents/Actors/Pc.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Resource/Resource.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Resource/EZeffectResource.h"
#include "Resource/World/TerrainResource.h"
#include "Render/Textures/Texture.h"
#include <algorithm>
#include <bit>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

// INLINED
inline RagEffectState::RagEffectState()
    : parent(nullptr), defZ(0), isDirection(0), type(0), level(1), flag(0), count(0), stateCnt(0), deltaPos{}, deltaPos2{}, deltaPos3{},
      resource(nullptr), resourceData(nullptr), currentLayer(0), loopCount(0), loopLimit(0), unknown_11C84(nullptr), unknown_11C88{-1, -1, -1, -1},
      forceRender(0), tag(0), unknown_11CA0(0) {
  for (unsigned index = 0; index < 20; ++index) {
    auto* record = this->objSeg + index * 80;
    const std::uint32_t zero = 0;

    for (unsigned field = 0; field < 6; ++field) {
      std::memcpy(record + field * 4, &zero, sizeof(zero));
    }
  }

  std::memset(this->currentKey, 0, sizeof(this->currentKey));

  std::memset(this->vertices, 0, sizeof(this->vertices));
}

// INLINED
inline RagEffectState::~RagEffectState() {
  for (EffectPrimitiveState* particle : this->primList) {
    particle->master = nullptr;
  }

  this->primList.clear();

  if (this->parent) {
    this->parent->state.Attachments().Unlink(this);
  }

  if (this->resource) {
    this->resource->Unlock();
  }
}

// OFFSET: 0x00BF84B0
// FUNCTION: fn_20007
CRagEffect::CRagEffect() {
}

CRagEffect::~CRagEffect() = default;

// INLINED
static CEffectPrim& Owner(EffectPrimitiveState* particle) {
  return *reinterpret_cast<CEffectPrim*>(reinterpret_cast<char*>(particle) - offsetof(CEffectPrim, state));
}

// INLINED
static vector3d RotatedForward(const float* matrix, float distance) {
  return {
    ((matrix[3] * 0.0f + matrix[0] * 0.0f) + matrix[6] * distance) + matrix[9],
    ((matrix[4] * 0.0f + matrix[1] * 0.0f) + matrix[7] * distance) + matrix[10],
    ((matrix[5] * 0.0f + matrix[2] * 0.0f) + matrix[8] * distance) + matrix[11]
  };
}

// INLINED
static void AllocateTextures(EffectPrimitiveState& particle) {
  const auto bytes = particle.totalTexture >> 30 != 0 ? std::numeric_limits<std::size_t>::max() : static_cast<std::size_t>(4u * particle.totalTexture);
  particle.textures = static_cast<CTexture**>(::operator new(bytes));
}

// INLINED
static void PlayEffectWave(const CRagEffect& effect, const char* name) {
  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  g_soundMgr->PlayWave(
    name, effect.render.position.x - player->render.position.x, 0.0f, effect.render.position.z - player->render.position.z, 250, 40, 1.0f, 0
  );
}

// INLINED
static std::list<CRenderObject*>& WorldObjects() {
  return static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects;
}

// GLOBAL: gl_25089
static int g_statusIconIds[100];

// OFFSET: 0x00C121A0
// FUNCTION: fn_20040
void CRagEffect::BloodDrain() {
  if (this->state.stateCnt != 0) {
    return;
  }

  for (int angle = -90; angle < 180; angle += 90) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 60;
    this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
    this->state.unknown_11C84->radius = Owner(this->state.unknown_11C84).CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]);
    Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
    this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
    const float speed = this->state.unknown_11C84->radius / this->state.unknown_11C84->duration;
    this->state.unknown_11C84->speed = speed + speed;
    this->state.unknown_11C84->gravSpeed = 2.0f;
    const float rise = -(this->state.unknown_11C84->gravSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->gravAccel = rise + rise;
    this->state.unknown_11C84->latiSpeed = -2.0f;
    const float spread = -(this->state.unknown_11C84->latiSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->latiAccel = spread + spread;
    this->state.unknown_11C84->roll = static_cast<float>(angle);
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    this->state.unknown_11C84->deltaPos3 = RotatedForward(this->state.unknown_11C84->transform, 1.0f);
    this->state.unknown_11C84->rotAxis[0] = this->state.unknown_11C84->deltaPos3.x;
    this->state.unknown_11C84->rotAxis[1] = this->state.unknown_11C84->deltaPos3.y;
    this->state.unknown_11C84->rotAxis[2] = this->state.unknown_11C84->deltaPos3.z;
    this->state.unknown_11C84->red = 100;
    this->state.unknown_11C84->green = 100;
    this->state.unknown_11C84->blue = 250;
    this->state.unknown_11C84->size = 1.7f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->numSegment = 7;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C14510
// FUNCTION: fn_20046
void CRagEffect::FireBall() {
  const int frame = this->state.stateCnt;

  if (frame != 20 && frame != 24 && frame != 28 && frame != 32) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_FireBall.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->pattern |= 0x120u;
  this->state.unknown_11C84->deltaPos2 = {0.0f, -15.0f, 0.0f};
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(this->state.unknown_11C84);
  particle.state.speed = -(particle.CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) / particle.state.duration);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
  this->state.unknown_11C84->size = 1.3f;
  this->state.unknown_11C84->animSpeed = 1;

  if (this->state.stateCnt != 20) {
    this->state.unknown_11C84->red = 246;
    this->state.unknown_11C84->green = 199;
    this->state.unknown_11C84->blue = 76;

    if (this->state.stateCnt == 24) {
      this->state.unknown_11C84->alpha = 180.0f;
    } else if (this->state.stateCnt == 28) {
      this->state.unknown_11C84->alpha = 130.0f;
    } else {
      this->state.unknown_11C84->alpha = 80.0f;
    }
  }

  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\fireball.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\fireball.act");
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C147D0

// OFFSET: 0x00C15E40
// FUNCTION: fn_20054
void CRagEffect::FrostDiver(char type) {
  if (g_serviceOptions.Get(229) != 0) {
    return;
  }

  if (this->state.stateCnt == 0) {
    this->Face(this->state.deltaPos[0], this->state.deltaPos[2]);
    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(-this->render.angle)));
    const vector3d reach = RotatedForward(rotation.values, 5.0f);
    this->state.deltaPos3[0] = this->state.deltaPos3[0] - reach.x;
    this->state.deltaPos3[1] = this->state.deltaPos3[1] - reach.y;
    this->state.deltaPos3[2] = this->state.deltaPos3[2] - reach.z;
    const vector3d step = RotatedForward(rotation.values, 2.0f);
    this->state.deltaPos2[0] = step.x;
    this->state.deltaPos2[1] = step.y;
    this->state.deltaPos2[2] = step.z;
  }

  if (this->state.flag != 0) {
    return;
  }

  const vector3d saved = this->render.position;
  this->render.position.x = this->state.deltaPos3[0] + this->render.position.x;
  this->render.position.y = this->state.deltaPos3[1] + this->render.position.y;
  this->render.position.z = this->state.deltaPos3[2] + this->render.position.z;

  if (this->CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) <= 2.5f) {
    this->state.flag = 1;
  }

  this->render.position = saved;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 40;
  this->state.unknown_11C84->pattern |= 1u;
  this->state.unknown_11C84->slots[0].flag1[0] = type == 3 ? 3 : 0;
  const int angle = std::max(rand() % 360, 0);
  const float distance = static_cast<float>(std::max(rand() % 10, 0) + 5) / 10.0f;
  const float sine = ClientSine(angle);
  const float cosine = ClientCosine(angle);
  this->state.unknown_11C84->deltaPos2.y = 30.0f;
  this->state.unknown_11C84->deltaPos2.x = sine * distance;
  this->state.unknown_11C84->deltaPos2.z = -distance * cosine;
  vector3d& velocity = this->state.unknown_11C84->deltaPos2;
  velocity.x = this->state.deltaPos3[0] + velocity.x;
  velocity.y = this->state.deltaPos3[1] + velocity.y;
  velocity.z = this->state.deltaPos3[2] + velocity.z;
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 40, 0) + 60) / 100.0f;
  this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 30, 0) + 150) / 10.0f;
  this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 30, 0) + 75);
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  EffectPrimitiveState& copy = *this->state.unknown_11C84;
  std::memcpy(copy.rotTransform, copy.transform, sizeof(copy.rotTransform));
  this->state.unknown_11C84->speed = 3.0f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  const float decay = -(slow.speed / slow.duration);
  slow.accel = decay + decay;
  this->state.unknown_11C84->chCnt = 20;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = g_texMgr.Load("effect\\ice.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[0] = texture;
  this->state.deltaPos3[0] = this->state.deltaPos3[0] - this->state.deltaPos2[0];
  this->state.deltaPos3[1] = this->state.deltaPos3[1] - this->state.deltaPos2[1];
  this->state.deltaPos3[2] = this->state.deltaPos3[2] - this->state.deltaPos2[2];
}

// OFFSET: 0x00C16C70
// FUNCTION: fn_20058
void CRagEffect::GrimTooth() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_FrostDiver.wav");
    this->Face(this->state.deltaPos[0], this->state.deltaPos[2]);
    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(-this->render.angle)));
    const vector3d step = RotatedForward(rotation.values, 2.0f);
    this->state.deltaPos2[0] = step.x;
    this->state.deltaPos2[1] = step.y;
    this->state.deltaPos2[2] = step.z;
    this->state.deltaPos3[0] = this->state.deltaPos3[0] - step.x;
    this->state.deltaPos3[1] = this->state.deltaPos3[1] - step.y;
    this->state.deltaPos3[2] = this->state.deltaPos3[2] - step.z;
  }

  if (this->state.flag != 0) {
    return;
  }

  if (this->state.stateCnt % 3 != 0) {
    this->state.deltaPos3[0] = this->state.deltaPos3[0] - this->state.deltaPos2[0];
    this->state.deltaPos3[1] = this->state.deltaPos3[1] - this->state.deltaPos2[1];
    this->state.deltaPos3[2] = this->state.deltaPos3[2] - this->state.deltaPos2[2];
    return;
  }

  const vector3d saved = this->render.position;

  if (this->CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) <= 2.5f) {
    this->state.flag = 1;
  }

  this->render.position = saved;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->pattern |= 1u;
  this->state.unknown_11C84->duration = 40;
  const int angle = std::max(rand() % 360, 0);
  rand();
  ClientSine(angle);
  ClientCosine(angle);
  this->state.unknown_11C84->deltaPos2 = {0.0f, 20.0f, 0.0f};
  vector3d& velocity = this->state.unknown_11C84->deltaPos2;
  velocity.x = this->state.deltaPos3[0] + velocity.x;
  velocity.y = this->state.deltaPos3[1] + velocity.y;
  velocity.z = this->state.deltaPos3[2] + velocity.z;
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 40, 0) + 60) / 100.0f;
  this->state.unknown_11C84->heightSize = 10.0f;
  this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 30, 0) + 75);
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  EffectPrimitiveState& copy = *this->state.unknown_11C84;
  std::memcpy(copy.rotTransform, copy.transform, sizeof(copy.rotTransform));
  this->state.unknown_11C84->speed = 3.0f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  const float decay = -(slow.speed / slow.duration);
  slow.accel = decay + decay;
  this->state.unknown_11C84->chCnt = 6;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = g_texMgr.Load("effect\\stone.bmp", 0, false, true, false);
  this->state.deltaPos3[0] = this->state.deltaPos3[0] - this->state.deltaPos2[0];
  this->state.deltaPos3[1] = this->state.deltaPos3[1] - this->state.deltaPos2[1];
  this->state.unknown_11C84->textures[0] = texture;
  this->state.deltaPos3[2] = this->state.deltaPos3[2] - this->state.deltaPos2[2];
}

// OFFSET: 0x00C1B490
// FUNCTION: fn_20071
void CRagEffect::IceWall() {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\wizard_icewall.wav");
  this->state.duration = 9999;

  for (int offset = -2; offset < 4; offset += 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->pattern |= 1u;
    const int angle = std::max(rand() % 360, 0);
    const float sine = ClientSine(angle);
    const float cosine = ClientCosine(angle);
    this->state.unknown_11C84->deltaPos2.y = 30.0f;
    this->state.unknown_11C84->deltaPos2.x = sine * 0.0f;
    this->state.unknown_11C84->deltaPos2.z = -static_cast<float>(offset) * cosine;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 40, 0) / 100) + 2.5f;
    this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 100, 0) + 300) / 10.0f;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 6, 0) + 87);
    this->state.unknown_11C84->chCnt = 20;
    this->state.unknown_11C84->alpha = 200.0f;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    EffectPrimitiveState& turn = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
    EffectPrimitiveState& copy = *this->state.unknown_11C84;
    std::memcpy(copy.rotTransform, copy.transform, sizeof(copy.rotTransform));
    this->state.unknown_11C84->speed = 3.0f;
    EffectPrimitiveState& slow = *this->state.unknown_11C84;
    const float decay = -(slow.speed / 40.0f);
    slow.accel = decay + decay;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ice.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C1B8B0
// FUNCTION: fn_20072
void CRagEffect::IncAgiDex() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  g_renderer->viewport.Projection().Project(
    &this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex
  );
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_IncAgiDex.wav");
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->longitude = 0.0f;
    this->state.unknown_11C84->speed = 1.5f;
    EffectPrimitiveState& slow = *this->state.unknown_11C84;
    slow.accel = -(slow.speed / static_cast<float>(slow.duration)) * 1.2f;
    this->state.unknown_11C84->widthSize = 40.0f;
    this->state.unknown_11C84->heightSize = 20.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 200.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\dex_agi_up.bmp", 0, false, true, false);
    this->state.unknown_11C84->slots[0].flag1[0] = 0;
  }

  if ((this->state.stateCnt & 1) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 50;
  EffectPrimitiveState& spin = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(spin.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  EffectPrimitiveState& moving = *this->state.unknown_11C84;
  const float distance = static_cast<float>(std::max(rand() % 7, 0) + 2);
  const vector3d step = RotatedForward(moving.transform, distance);
  moving.deltaPos2.x = step.x;
  moving.deltaPos2.y = step.y;
  moving.deltaPos2.z = step.z;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(90));
  this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 20) / 100.0f;
  this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
  this->state.unknown_11C84->heightSize = 0.18f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 200.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
}

// OFFSET: 0x00C1BDC0
// FUNCTION: fn_20073
void CRagEffect::IncAgility() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  g_renderer->viewport.Projection().Project(
    &this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex
  );
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_IncAgility.wav");
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->longitude = 0.0f;
    this->state.unknown_11C84->speed = 1.5f;
    EffectPrimitiveState& slow = *this->state.unknown_11C84;
    slow.accel = -(slow.speed / static_cast<float>(slow.duration)) * 1.2f;
    this->state.unknown_11C84->widthSize = 40.0f;
    this->state.unknown_11C84->heightSize = 20.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 200.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\agi_up.bmp", 0, false, true, false);
    this->state.unknown_11C84->slots[0].flag1[0] = 99;
  }

  if ((this->state.stateCnt & 1) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 50;
  EffectPrimitiveState& spin = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(spin.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  EffectPrimitiveState& moving = *this->state.unknown_11C84;
  const float distance = static_cast<float>(std::max(rand() % 7, 0) + 2);
  const vector3d step = RotatedForward(moving.transform, distance);
  moving.deltaPos2.x = step.x;
  moving.deltaPos2.y = step.y;
  moving.deltaPos2.z = step.z;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(90));
  this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 20) / 100.0f;
  this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
  this->state.unknown_11C84->heightSize = 0.18f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 200.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].flag1[0] = 99;
}

// OFFSET: 0x00C19980
// FUNCTION: fn_20066
void CRagEffect::Hit4(const RenderProjection& projection) {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  projection.Project(&this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_hit4.wav");
  const int facing = static_cast<int>(180.0f - this->state.parent->render.angle);
  const int direction = facing < 0 ? facing + 360 : facing;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 15;
  this->state.unknown_11C84->longitude = static_cast<float>(direction);
  this->state.unknown_11C84->latitude = -90.0f;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->speed = 0.7f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  slow.accel = -(slow.speed / slow.duration) * 0.5f;
  this->state.unknown_11C84->outerSize = 4.0f;
  this->state.unknown_11C84->innerSize = 0.5f;
  this->state.unknown_11C84->heightSpeed = 0.25f;
  this->state.unknown_11C84->heightAccel = 0.15f;
  const int lifetime = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = lifetime - lifetime / 3;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = g_texMgr.Load("effect\\lens2.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[0] = texture;

  for (int count = 5; count != 0; --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->longitude = static_cast<float>(direction - 40 + std::max(rand() % 80, 0));
    this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 100, 0) - 50);
    EffectPrimitiveState& yaw = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(yaw.transform)->MakeYRotation(GetRadian(static_cast<int>(yaw.longitude)));
    EffectPrimitiveState& pitch = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(pitch.transform)->AppendXRotation(GetRadian(static_cast<int>(pitch.latitude)));
    EffectPrimitiveState& spark = *this->state.unknown_11C84;
    spark.deltaPos3 = RotatedForward(spark.transform, static_cast<float>(std::max(rand() % 4, 0) + 4));
    EffectPrimitiveState& moving = *this->state.unknown_11C84;
    moving.deltaPos2.x = moving.deltaPos3.x + 0.0f;
    moving.deltaPos2.y = moving.deltaPos3.y - 10.0f;
    moving.deltaPos2.z = moving.deltaPos3.z + 0.0f;
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 90, 0) + 60) / 100.0f;
    this->state.unknown_11C84->duration = std::max(rand() % 24, 0) + 6;
    EffectPrimitiveState& fading = *this->state.unknown_11C84;
    fading.accel = -(fading.speed / fading.duration) * 0.5f;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 100, 0) + 60) / 100.0f;
    EffectPrimitiveState& shrinking = *this->state.unknown_11C84;
    shrinking.sizeSpeed = -(shrinking.size / shrinking.duration);
    this->state.unknown_11C84->fadeOutCnt = 0;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->numSegment = 3;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C3B2E0
// FUNCTION: fn_20106
void CRagEffect::SoulStrike(char F1) {
  int base = 0;
  int step = 0;
  int total = 0;

  switch (this->state.count) {
  case 1:
    total = 1;
    base = 0;
    step = 1;
    break;
  case 2:
    total = 2;
    step = 180;
    base = -90;
    break;
  case 3:
    total = 3;
    step = 90;
    base = -90;
    break;
  case 4:
    total = 4;
    step = 60;
    base = -90;
    break;
  case 5:
    total = 5;
    step = 45;
    base = -90;
    break;
  case 6:
    total = 6;
    step = 30;
    base = -90;
    break;
  case 7:
    total = 7;
    step = 25;
    base = -90;
    break;
  default:
    break;
  }

  const int frame = this->state.stateCnt + 5;

  if (frame % 11 != 0 || frame / 11 > total) {
    return;
  }

  const int index = frame / 11;
  PlayEffectWave(*this, "effect\\EF_SoulStrike.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 40;
  this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->radius = Owner(this->state.unknown_11C84).CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]);
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  this->state.unknown_11C84->speed = this->state.unknown_11C84->radius / this->state.unknown_11C84->duration;
  this->state.unknown_11C84->gravSpeed = 3.5f;
  const float rise = -(this->state.unknown_11C84->gravSpeed / this->state.unknown_11C84->duration);
  this->state.unknown_11C84->gravAccel = rise + rise;
  this->state.unknown_11C84->latiSpeed = -2.0f;
  const float spread = -(this->state.unknown_11C84->latiSpeed / this->state.unknown_11C84->duration);
  this->state.unknown_11C84->latiAccel = spread + spread;
  this->state.unknown_11C84->roll = static_cast<float>(base + step * index);
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
  this->state.unknown_11C84->deltaPos3 = RotatedForward(this->state.unknown_11C84->transform, 1.0f);
  this->state.unknown_11C84->rotAxis[0] = this->state.unknown_11C84->deltaPos3.x;
  this->state.unknown_11C84->rotAxis[1] = this->state.unknown_11C84->deltaPos3.y;
  this->state.unknown_11C84->rotAxis[2] = this->state.unknown_11C84->deltaPos3.z;
  this->state.unknown_11C84->size = 2.75f;
  const char* action = F1 != 0 ? "이팩트\\particle5.act" : "이팩트\\particle1.act";
  const char* sprite = F1 != 0 ? "이팩트\\particle5.spr" : "이팩트\\particle1.spr";
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), sprite);
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), action);
  this->state.unknown_11C84->numSegment = 12;
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C3B730
// FUNCTION: fn_20107
void CRagEffect::SpearBmr() {
  const int frame = this->state.stateCnt;

  if (frame != 0 && frame != 4 && frame != 8 && frame != 12) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_FireBall.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 15;
  this->state.unknown_11C84->pattern |= 0x120u;
  this->state.unknown_11C84->deltaPos2 = {0.0f, -15.0f, 0.0f};
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(this->state.unknown_11C84);
  particle.state.speed = -(particle.CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) / particle.state.duration);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
  this->state.unknown_11C84->size = 1.0f;
  this->state.unknown_11C84->animSpeed = 1;

  if (this->state.stateCnt != 0) {
    if (this->state.stateCnt == 4) {
      this->state.unknown_11C84->alpha = 180.0f;
    } else if (this->state.stateCnt == 8) {
      this->state.unknown_11C84->alpha = 130.0f;
    } else {
      this->state.unknown_11C84->alpha = 80.0f;
    }
  }

  this->SetAction(this->Get8Dir2(this->state.parent->render.angle, 1), 1, 1);
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\창.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\창.act");
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C3BD40
// FUNCTION: fn_20109
void CRagEffect::SprayPond() {
  if (this->state.stateCnt == 0) {
    for (int angle = 0; angle < 360; angle += 45) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->pattern = 2;
      this->state.unknown_11C84->duration = 130;
      EffectPrimitiveState& yaw = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(yaw.transform)->MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(angle) + 22.5f)));
      EffectPrimitiveState& moving = *this->state.unknown_11C84;
      const vector3d edge = RotatedForward(moving.transform, 12.2f);
      moving.deltaPos2.x = edge.x + 0.0f;
      moving.deltaPos2.y = edge.y - 2.0f;
      moving.deltaPos2.z = edge.z + 0.0f;
      EffectPrimitiveState& facing = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(facing.rotTransform)->MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(angle + 90) + 22.5f)));
      this->state.unknown_11C84->alpha = 120.0f;
      this->state.unknown_11C84->widthSize = 5.0f;
      this->state.unknown_11C84->heightSize = 2.7f;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pond_water.bmp", 0, false, true, false);
    }
  }

  if (this->state.stateCnt % 10 == 0) {
    for (int angle = 0; angle < 360; angle += 45) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 30;
      EffectPrimitiveState& yaw = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(yaw.transform)->MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(angle) + 22.5f)));
      EffectPrimitiveState& moving = *this->state.unknown_11C84;
      const vector3d edge = RotatedForward(moving.transform, 13.8f);
      moving.deltaPos2.x = edge.x;
      moving.deltaPos2.y = edge.y;
      moving.deltaPos2.z = edge.z;
      EffectPrimitiveState& roll = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(roll.rotTransform)->MakeZRotation(GetRadian(90));
      EffectPrimitiveState& facing = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(facing.rotTransform)->AppendYRotation(GetRadian(static_cast<int>(static_cast<float>(angle + 90) + 22.5f)));
      this->state.unknown_11C84->alpha = 100.0f;
      this->state.unknown_11C84->fadeOutCnt = 0;
      this->state.unknown_11C84->widthSize = 5.5f;
      this->state.unknown_11C84->heightSize = 1.0f;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
    }
  }

  if (this->state.stateCnt % 20 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 25;
    this->state.unknown_11C84->radius = 13.0f;
    this->state.unknown_11C84->arcAngle = 10.0f;
    this->state.unknown_11C84->size = 1.25f;
    this->state.unknown_11C84->latitude = 90.0f;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    this->state.unknown_11C84->radiusSpeed = 0.25f;
    EffectPrimitiveState& growing = *this->state.unknown_11C84;
    growing.radiusAccel = -(growing.radiusSpeed / growing.duration);
    this->state.unknown_11C84->alpha = 100.0f;
    this->state.unknown_11C84->fadeOutCnt = 0;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_center.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C3F440
// FUNCTION: fn_20120
void CRagEffect::WaterBall2() {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\wizard_waterball_chulung.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\Waterball.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\Waterball.act");
  this->state.unknown_11C84->duration = 40;
  this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->radius = Owner(this->state.unknown_11C84).CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]);
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  this->state.unknown_11C84->speed = this->state.unknown_11C84->radius / this->state.unknown_11C84->duration;
  this->state.unknown_11C84->gravSpeed = 3.5f;
  const float rise = -(this->state.unknown_11C84->gravSpeed / this->state.unknown_11C84->duration);
  this->state.unknown_11C84->gravAccel = rise + rise;
  this->state.unknown_11C84->latiSpeed = -2.0f;
  const float spread = -(this->state.unknown_11C84->latiSpeed / this->state.unknown_11C84->duration);
  this->state.unknown_11C84->latiAccel = spread + spread;
  this->state.unknown_11C84->roll = 0.0f;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
  this->state.unknown_11C84->deltaPos3 = RotatedForward(this->state.unknown_11C84->transform, 1.0f);
  this->state.unknown_11C84->rotAxis[0] = this->state.unknown_11C84->deltaPos3.x;
  this->state.unknown_11C84->rotAxis[1] = this->state.unknown_11C84->deltaPos3.y;
  this->state.unknown_11C84->rotAxis[2] = this->state.unknown_11C84->deltaPos3.z;
  this->state.unknown_11C84->size = 0.75f;
  this->state.unknown_11C84->numSegment = 12;
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C3FE30
// FUNCTION: fn_20124
void CRagEffect::Yufitel(char F1) {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\hunter_shockwavetrap.wav");

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->pattern |= 0x40u;
  this->state.unknown_11C84->pattern |= 0x400u;
  this->state.unknown_11C84->renderFlag |= 2u;
  this->state.unknown_11C84->widthSize = 3.5f;
  this->state.unknown_11C84->heightSize = 3.5f;
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& center = Owner(this->state.unknown_11C84);
  center.state.speed = -(center.CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) / center.state.duration);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
  this->state.unknown_11C84->deltaPos2.y = -5.0f;
  this->state.unknown_11C84->alpha = 170.0f;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] =
    F1 != 0 ? g_texMgr.Load("effect\\twirl_soft.bmp", 0, false, true, false) : g_texMgr.Load("effect\\thunder_center.bmp", 0, false, true, false);
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->pattern = 0x40u;
  this->state.unknown_11C84->pattern |= 0x400u;
  this->state.unknown_11C84->renderFlag |= 2u;
  this->state.unknown_11C84->repeatAnim = 1;
  this->state.unknown_11C84->widthSize = 4.5f;
  this->state.unknown_11C84->heightSize = 4.5f;
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& ball = Owner(this->state.unknown_11C84);
  ball.state.speed = -(ball.CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) / ball.state.duration);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
  this->state.unknown_11C84->deltaPos2.y = -5.0f;
  this->state.unknown_11C84->animSpeed = 1;
  this->state.unknown_11C84->totalTexture = 6;
  AllocateTextures(*this->state.unknown_11C84);

  if (F1 != 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\twirl_soft.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\thunder_ball_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\twirl_soft.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\thunder_ball_d.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\twirl_soft.bmp", 0, false, true, false);
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_ball_a.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\thunder_ball_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\thunder_ball_c.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\thunder_ball_d.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\thunder_ball_e.bmp", 0, false, true, false);
  }

  this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\thunder_ball_f.bmp", 0, false, true, false);
}

// OFFSET: 0x00C40E70
// FUNCTION: fn_20132
void CRagEffect::AirTexture(const char* name) {
  const int frame = this->state.stateCnt;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (frame != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 98, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 5.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[0];
  slot.vecBNow.x = this->state.deltaPos[0];
  slot.vecBNow.y = this->state.deltaPos[1];
  slot.vecBNow.z = this->state.deltaPos[2];
  this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  Get_Angle(
    this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[0].riseAngle
  );
  this->state.unknown_11C84->slots[0].riseAngle += 90;
  EffectPrimitiveSlot& rise = this->state.unknown_11C84->slots[0];

  if (rise.riseAngle >= 360) {
    rise.riseAngle -= 360;
  }

  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C4F250
// FUNCTION: fn_20181
void CRagEffect::EnergyDrain2() {
  if (this->state.stateCnt != 0) {
    return;
  }

  for (int angle = -90; angle < 180; angle += 90) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 60;
    const float deltaZ = this->state.deltaPos[2] - this->render.position.z;
    EffectPrimitiveState& launched = *this->state.unknown_11C84;
    launched.deltaPos2.x = this->state.deltaPos[0] - this->render.position.x;
    launched.deltaPos2.z = deltaZ;
    launched.deltaPos2.y = -10.0f;
    this->state.unknown_11C84->radius = Owner(this->state.unknown_11C84).CalcDist(
      this->state.deltaPos[0] - (this->state.deltaPos[0] - this->render.position.x) * 0.5f,
      this->state.deltaPos[2] - (this->state.deltaPos[2] - this->render.position.z) * 0.5f
    );
    Owner(this->state.unknown_11C84).Face(
      (this->render.position.x + this->render.position.x) - this->state.deltaPos[0],
      (this->render.position.z + this->render.position.z) - this->state.deltaPos[2]
    );
    this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
    const float speed = this->state.unknown_11C84->radius / this->state.unknown_11C84->duration;
    this->state.unknown_11C84->speed = speed + speed;
    this->state.unknown_11C84->gravSpeed = 2.0f;
    const float rise = -(this->state.unknown_11C84->gravSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->gravAccel = rise + rise;
    this->state.unknown_11C84->latiSpeed = -2.0f;
    const float spread = -(this->state.unknown_11C84->latiSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->latiAccel = spread + spread;
    this->state.unknown_11C84->roll = static_cast<float>(angle);
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    this->state.unknown_11C84->deltaPos3 = RotatedForward(this->state.unknown_11C84->transform, 1.0f);
    this->state.unknown_11C84->rotAxis[0] = this->state.unknown_11C84->deltaPos3.x;
    this->state.unknown_11C84->rotAxis[1] = this->state.unknown_11C84->deltaPos3.y;
    this->state.unknown_11C84->rotAxis[2] = this->state.unknown_11C84->deltaPos3.z;
    this->state.unknown_11C84->red = std::max(rand() % 81, 0) + 160;
    this->state.unknown_11C84->green = std::max(rand() % 81, 0) + 160;
    this->state.unknown_11C84->blue = 255;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 1.5f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->numSegment = 7;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C1DC70
// FUNCTION: fn_20075
void CRagEffect::InitEZ2STRFrame() {
  const CEZeffectRes& clips = *static_cast<const CEZeffectRes*>(this->state.resource);
  this->state.currentLayer = 0;
  this->state.stateCnt = 0;
  this->state.endLayer = clips.clipCount;

  if (clips.clips[0].frameCount == 0) {
    --this->state.endLayer;
  }

  for (int layer = 0; layer < clips.clipCount; ++layer) {
    this->state.currentKey[layer] = 0;
  }
}

// OFFSET: 0x00C36FC0
// FUNCTION: fn_20095
bool CRagEffect::ProcessEZ2STR(float x, float y, float z) {
  RagEffectState& state = this->state;

  if (state.resource == nullptr) {
    return false;
  }

  if (state.currentLayer >= state.endLayer) {
    ++state.loopCount;

    if (state.loopCount > state.loopLimit) {
      return false;
    }

    this->InitEZ2STRFrame();
  }

  const RenderVertex vertex = TransformVert(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view, state.parent->render.position);
  state.tlvertX = vertex.x + x;
  state.tlvertY = vertex.y + y;
  state.tlvertZ = vertex.z + z;
  state.defZ = x == 0.0f && y == -80.0f && z == 0.0f ? 0 : 1;

  float ratio;
  const CEZeffectRes& clips = *static_cast<const CEZeffectRes*>(state.resource);

  for (int layer = 0; layer < clips.clipCount; ++layer) {
    state.layerDrawn[layer] = false;
    const EZeffectClip& clip = clips.clips[layer];

    if (clip.frameCount == 0 || state.currentKey[layer] == -1) {
      continue;
    }

    const EZeffectKeyFrame* key = &clip.frames[state.currentKey[layer]];
    const int elapsed = state.stateCnt - key->frame;

    if (elapsed < 0) {
      continue;
    }

    if (elapsed != 0 && key[1].frame == state.stateCnt) {
      if (++state.currentKey[layer] >= clip.frameCount - 1) {
        state.currentKey[layer] = -1;
        ++state.currentLayer;
      }

      ++key;
    }

    EZeffectTransform& current = state.transforms[layer];
    const EZeffectTransform& delta = key->transform;

    if (key->type == 0) {
      std::memcpy(&current, &delta, sizeof(current));

      if (state.currentKey[layer] != -1 && key[1].frame == key->frame) {
        ++state.currentKey[layer];
      }

      state.layerDrawn[layer] = true;
      continue;
    }

    current.x += delta.x;
    current.y += delta.y;
    current.u += delta.u;
    current.v += delta.v;
    current.us += delta.us;
    current.vs += delta.vs;
    current.u2 += delta.u2;
    current.v2 += delta.v2;
    current.us2 += delta.us2;
    current.vs2 += delta.vs2;
    current.ax[0] += delta.ax[0];
    current.ax[1] += delta.ax[1];
    current.ax[2] += delta.ax[2];
    current.ax[3] += delta.ax[3];
    current.ay[0] += delta.ay[0];
    current.ay[1] += delta.ay[1];
    current.ay[2] += delta.ay[2];
    current.ay[3] += delta.ay[3];
    current.crR += delta.crR;
    current.crG += delta.crG;
    current.crB += delta.crB;
    current.crA += delta.crA;
    current.rz += delta.rz;

    switch (delta.anitype) {
    case 1:
      current.aniframe += delta.aniframe;
      break;

    case 2:
      current.aniframe += delta.anidelta;

      if (current.aniframe >= static_cast<float>(clip.textureCount)) {
        current.aniframe = static_cast<float>(clip.textureCount) - 1.0f;
      }

      break;

    case 3: {
      const float frame = current.aniframe += delta.anidelta;
      const float count = static_cast<float>(clip.textureCount);

      if (frame >= count) {
        ratio = static_cast<float>(static_cast<int>(frame / count));
        current.aniframe = frame - ratio * count;
      }

      break;
    }

    case 4: {
      float frame = current.aniframe -= delta.anidelta;

      if (frame < 0.0f) {
        const float count = static_cast<float>(clip.textureCount);
        frame -= ratio * count;
        ratio = static_cast<float>(static_cast<int>(frame / count));

        if (frame < 0.0f) {
          frame += count;
        }

        current.aniframe = frame;
      }

      break;
    }

    case 5: {
      const int frame = static_cast<int>(static_cast<float>(elapsed) * delta.anidelta + current.aniframe);
      const int count = clip.textureCount - 1;
      const int turns = frame / count;

      if ((turns & 1) != 0) {
        current.aniframe = static_cast<float>(count - (frame - turns * count));
      } else {
        current.aniframe = static_cast<float>(frame - turns * count);
      }

      break;
    }
    }

    state.layerDrawn[layer] = true;
  }

  return true;
}

// OFFSET: 0x00C73480
// FUNCTION: fn_20326
bool CRagEffect::ShieldCharge() {
  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C0B360
// FUNCTION: fn_20016
bool CRagEffect::Aspersio() {
  if (this->state.stateCnt == 70) {
    PlayEffectWave(*this, "effect\\priest_aspersio.wav");
  }

  const int frame = this->state.stateCnt;

  if (frame % 5 == 0 && frame > 60 && frame < 140) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.duration = 400;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    particle.duration = 40;
    particle.radius = 1.5f;
    particle.gravSpeed = 0.19f;
    particle.gravAccel = 0.01f;
    particle.longitude = static_cast<float>(std::max(rand() % 360, 0));
    particle.size = 1.2f;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    const float* m = particle.transform;
    const float distance = particle.radius;
    particle.deltaPos2 = {
      (m[0] * 0.0f - m[3] * 30.0f) + distance * m[6] + m[9],
      (m[1] * 0.0f - m[4] * 30.0f) + distance * m[7] + m[10],
      (m[2] * 0.0f - m[5] * 30.0f) + distance * m[8] + m[11]
    };
    particle.fadeOutCnt = 30;
    Owner(&particle).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(&particle).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    Owner(&particle).InitSegment();
  }

  if (this->state.stateCnt == this->state.duration) {
    return false;
  }

  this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
  return true;
}

// OFFSET: 0x00C0C4A0
// FUNCTION: fn_20018
void __stdcall Nullsub432() {
}

// OFFSET: 0x00C0C550
// FUNCTION: fn_20019
void CRagEffect::Blessing() {
  const EffectActor& parent = *this->state.parent;
  this->render.position.x = parent.render.position.x;
  this->render.position.y = parent.render.position.y;
  this->render.position.z = parent.render.position.z;

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_Blessing.wav");
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    EffectPrimitiveState& ring = *this->state.unknown_11C84;
    ring.duration = this->state.duration;
    ring.pattern |= 1u;
    ring.renderFlag |= 0x200u;
    ring.radius = 10.0f;
    ring.latitude = 90.0f;
    ring.red = 32;
    ring.green = 176;
    ring.blue = 232;
    ring.alpha = 0.0f;
    ring.maxAlpha = 100.0f;
    ring.alphaSpeed = ring.maxAlpha / 30.0f;
    ring.fadeOutCnt = ring.duration - 30;
    Owner(&ring).InitSegment();
    AllocateTextures(ring);
    ring.textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
  }

  if (this->state.stateCnt % 3 == 0 && this->state.stateCnt < 10) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    EffectPrimitiveState& wing = *this->state.unknown_11C84;
    wing.duration = this->state.duration;
    wing.animSpeed = 2;
    wing.alpha = static_cast<float>(this->state.stateCnt * -6 + 200);
    wing.deltaPos2 = {0.0f, -25.0f, 0.0f};
    Owner(&wing).SetSpriteName(ResourceMgr::Instance(), "이팩트\\축복.spr");
    Owner(&wing).SetActionName(ResourceMgr::Instance(), "이팩트\\축복.act");
    Owner(&wing).InitSegment();
  }

  if (this->state.stateCnt % 4 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    EffectPrimitiveState& spark = *this->state.unknown_11C84;
    spark.duration = 80;
    spark.pattern |= 0x80u;
    reinterpret_cast<matrix*>(spark.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    const float* m = spark.transform;
    const float distance = static_cast<float>(std::max(rand() % 7, 0) + 2);
    spark.deltaPos2 = {
      (m[0] * 0.0f - m[3] * 25.0f) + distance * m[6] + m[9],
      (m[1] * 0.0f - m[4] * 25.0f) + distance * m[7] + m[10],
      (m[2] * 0.0f - m[5] * 25.0f) + distance * m[8] + m[11]
    };
    reinterpret_cast<matrix*>(spark.transform)->MakeXRotation(GetRadian(-90));
    spark.speed = static_cast<float>(std::max(rand() % 20, 0) + 20) / 100.0f;
    spark.accel = -(spark.speed / static_cast<float>(spark.duration)) * 0.5f;
    spark.alpha = 0.0f;
    spark.size = 0.5f;
    spark.fadeOutCnt = spark.duration - spark.duration / 5;
    spark.alphaSpeed = spark.maxAlpha / 10.0f;
    Owner(&spark).SetActionName(ResourceMgr::Instance(), "이팩트\\particle6.act");
    Owner(&spark).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle6.spr");
    Owner(&spark).InitSegment();
  }
}

// INLINED
static __forceinline void SpawnBottomQuad(CRagEffect& effect, const vector3d& velocity, int angle) {
  effect.state.unknown_11C84 = &CreateEffectParticle(effect, effect.render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  EffectPrimitiveState& quad = *effect.state.unknown_11C84;
  quad.duration = 9999;
  quad.commonPattern = 1;
  quad.pattern = 0x28;
  quad.type = 0x32;
  quad.deltaPos2 = velocity;
  quad.longitude = static_cast<float>(angle);
  quad.widthSize = 5.0f;
  quad.heightSize = 15.0f;
  quad.heightSpeed = 0.25f;
  quad.alpha = 0.0f;
  quad.maxAlpha = 180.0f;
  quad.alphaSpeed = quad.maxAlpha / 6.0f;
  quad.fadeOutCnt = quad.duration - 35;
  AllocateTextures(quad);
  quad.textures[0] = g_texMgr.Load("effect\\magic_violet.tga", 0, false, true, false);
}

// OFFSET: 0x00C0DA60
// FUNCTION: fn_20023
void CRagEffect::Bottom() {
  if (this->state.stateCnt != 0) {
    return;
  }

  int angle = static_cast<int>(180.0f - this->state.parent->render.angle);

  if (angle < 0) {
    angle += 360;
  }

  matrix rotation;
  rotation.MakeYRotation(GetRadian(angle));
  const float* m = rotation.values;
  this->state.duration = 9999;

  for (int side = 1; side > -2; side -= 2) {
    const float distance = static_cast<float>(side) * 5.0f;
    SpawnBottomQuad(
      *this,
      {m[3] * 0.0f + m[0] * 0.0f + distance * m[6] + m[9], m[4] * 0.0f + m[1] * 0.0f + distance * m[7] + m[10],
       m[5] * 0.0f + m[2] * 0.0f + distance * m[8] + m[11]},
      angle + 90
    );
    SpawnBottomQuad(
      *this,
      {distance * m[0] + m[3] * 0.0f + m[6] * 0.0f + m[9], distance * m[1] + m[4] * 0.0f + m[7] * 0.0f + m[10],
       distance * m[2] + m[5] * 0.0f + m[8] * 0.0f + m[11]},
      angle
    );
  }
}

// OFFSET: 0x00C10830
// FUNCTION: fn_20034
bool CRagEffect::ProcessEZ2STRC10830() {
  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C153A0
// FUNCTION: fn_20049
bool CRagEffect::ProcessEZ2STRC153A0() {
  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C3CC40
// FUNCTION: fn_20112
bool CRagEffect::ProcessEZ2STRC3CC40() {
  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C3FE00
// FUNCTION: fn_20123
bool CRagEffect::ProcessEZ2STRC3FE00() {
  return this->ProcessEZ2STR(0.0f, 0.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C65300
// FUNCTION: fn_20267
bool CRagEffect::ProcessEZ2STRC65300() {
  return this->ProcessEZ2STR(0.0f, 0.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C15990
// FUNCTION: fn_20051
bool CRagEffect::ProcessEZ2STRC15990() {
  if (!this->ProcessEZ2STR(0.0f, -80.0f, 0.0f)) {
    this->state.stateCnt = 9999;
    return false;
  }

  return true;
}

// OFFSET: 0x00C159D0
// FUNCTION: fn_20052
bool CRagEffect::ProcessEZ2STRC159D0() {
  if (!this->ProcessEZ2STR(0.0f, -80.0f, 0.0f)) {
    this->state.stateCnt = 9999;
    return false;
  }

  return true;
}

// OFFSET: 0x00C5EFA0
// FUNCTION: fn_20241
void CRagEffect::Fireworks() {
  if (this->state.stateCnt == 300) {
    g_soundMgr->PlayWave("effect\\폭죽.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  if (this->state.stateCnt > 2300) {
    this->state.stateCnt = 299;
  }
}

// OFFSET: 0x00C410A0
// FUNCTION: fn_20133
void CRagEffect::AttackEnergy2(const char* name) {
  const EffectActor& parent = *this->state.parent;
  this->render.position.x = parent.render.position.x;
  this->render.position.y = parent.render.position.y;
  this->render.position.z = parent.render.position.z;

  if (this->state.stateCnt != 0 && this->state.stateCnt != 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 106, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  EffectPrimitiveState& prim = *this->state.unknown_11C84;
  prim.renderFlag = 5;
  prim.duration = this->state.duration;
  prim.totalTexture = 1;
  AllocateTextures(prim);
  prim.textures[0] = g_texMgr.Load(name, 0, false, true, false);
  prim.size = 4.0f;

  for (EffectPrimitiveSlot& slot : prim.slots) {
    slot.life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[index];
    slot.life = 1;
    slot.process = 0;
    slot.maxHeight = 8.0f;
    slot.alphaB = 0;
    slot.distance = 8.0f;
    slot.fullDisplayAngle = 360;
    slot.height[0] = 10.0f;
    slot.riseAngle = static_cast<std::int16_t>(index * 45);
    slot.height[1] = static_cast<float>(index * 22);
  }
}

// OFFSET: 0x00C1E050
// FUNCTION: fn_20077
bool CRagEffect::LexDivina() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\priest_lexdivina.wav");
  }

  if (this->state.stateCnt == 20) {
    PlayEffectWave(*this, "effect\\priest_lexdivina.wav");
  }

  if (this->state.stateCnt == 35) {
    PlayEffectWave(*this, "effect\\priest_lexdivina.wav");
  }

  if (this->state.stateCnt == 60) {
    PlayEffectWave(*this, "effect\\priest_lexdivina.wav");
  }

  if (this->state.stateCnt == 70) {
    PlayEffectWave(*this, "effect\\priest_lexdivina.wav");
  }

  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
}

// OFFSET: 0x00C37A60
// FUNCTION: fn_20096
void CRagEffect::RemoveTrap() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\hunter_removetrap.wav");
  }

  if (this->state.stateCnt != 0 && this->state.stateCnt != 7) {
    return;
  }

  for (int count = 12; count != 0; --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    particle.duration = 50;
    particle.radiusSpeed = 0.2f;
    rand();
    particle.gravSpeed = -2.0f;
    particle.gravAccel = static_cast<float>(std::max(rand() % 10, 0)) / 1000.0f + 0.08f;
    particle.longitude = static_cast<float>(std::max(rand() % 360, 0));
    particle.size = 0.8f;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    const float* m = particle.transform;
    const float distance = particle.radius;
    particle.deltaPos2 = {
      m[3] * 0.0f + m[0] * 0.0f + distance * m[6] + m[9],
      m[4] * 0.0f + m[1] * 0.0f + distance * m[7] + m[10],
      m[5] * 0.0f + m[2] * 0.0f + distance * m[8] + m[11]
    };
    particle.fadeOutCnt = 35;
    Owner(&particle).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(&particle).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    Owner(&particle).InitSegment();
  }
}

// OFFSET: 0x00C3B9E0
// FUNCTION: fn_20108
void CRagEffect::Sphere() {
  const EffectActor& parent = *this->state.parent;
  this->render.position.x = parent.render.position.x;
  this->render.position.y = parent.render.position.y;
  this->render.position.z = parent.render.position.z;

  if (this->state.stateCnt % 10 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.duration = 500 - this->state.stateCnt;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  particle.duration = 300;
  particle.radius = 0.1f;
  particle.radiusSpeed = 0.15f;
  particle.gravSpeed = 0.15f;
  particle.longitude = 2.0f;
  particle.longSpeed = 8.0f;
  particle.longAccel = -0.01f;
  particle.size = 1.2f;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
  const float* m = particle.transform;
  const float distance = particle.radius;
  particle.deltaPos2 = {
    (m[0] * 0.0f - m[3] * 30.0f) + distance * m[6] + m[9],
    (m[1] * 0.0f - m[4] * 30.0f) + distance * m[7] + m[10],
    (m[2] * 0.0f - m[5] * 30.0f) + distance * m[8] + m[11]
  };
  particle.chRadiSpeed = 1;
  particle.unknown_01D8 = 0;
  particle.chPoint.push_back(95.0f);
  particle.chVal1.push_back(-0.15f);
  particle.chVal2.push_back(particle.radiusAccel);
  Owner(&particle).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
  Owner(&particle).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
  Owner(&particle).InitSegment();
}

// OFFSET: 0x00C17220
// FUNCTION: fn_20059
void CRagEffect::GrimToothAtk() {
  if (this->state.stateCnt != 0) {
    return;
  }

  for (int index = 0; index < 3; ++index) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->pattern |= 1u;
    this->state.unknown_11C84->renderFlag |= 0x200u;
    this->state.duration = 1000;
    this->state.unknown_11C84->duration = 1000;
    ClientSine(index * 120);
    ClientCosine(index * 120);

    if (index == 0) {
      this->state.unknown_11C84->deltaPos2 = {0.0f, 40.0f, -12.0f};
    } else if (index == 1) {
      this->state.unknown_11C84->deltaPos2 = {12.0f, 40.0f, 6.0f};
    } else if (index == 2) {
      this->state.unknown_11C84->deltaPos2 = {-12.0f, 40.0f, 6.0f};
    }

    this->state.unknown_11C84->size = 0.9f;
    this->state.unknown_11C84->heightSize = 25.0f;
    this->state.unknown_11C84->longitude = static_cast<float>((6 - index) * 120);
    this->state.unknown_11C84->latitude = 75.0f;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    EffectPrimitiveState& turn = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
    EffectPrimitiveState& copy = *this->state.unknown_11C84;
    std::memcpy(copy.rotTransform, copy.transform, sizeof(copy.rotTransform));
    this->state.unknown_11C84->speed = 3.5f;
    this->state.unknown_11C84->accel = 0.001f;
    this->state.unknown_11C84->chCnt = 10;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    AllocateTextures(*this->state.unknown_11C84);
    CTexture* texture = g_texMgr.Load("effect\\stone.bmp", 0, false, true, false);
    this->state.deltaPos3[0] = this->state.deltaPos3[0] - this->state.deltaPos2[0];
    this->state.unknown_11C84->textures[0] = texture;
    this->state.deltaPos3[1] = this->state.deltaPos3[1] - this->state.deltaPos2[1];
    this->state.deltaPos3[2] = this->state.deltaPos3[2] - this->state.deltaPos2[2];
  }
}

// OFFSET: 0x00C16430
// FUNCTION: fn_20055
int CRagEffect::GetDuration() {
  return this->state.duration;
}

// OFFSET: 0x00612780
// FUNCTION: fn_04342
void CRagEffect::SetStateDuration(int duration) {
  this->state.duration = duration;
}

// OFFSET: 0x00C3A920
// FUNCTION: fn_20104
void CRagEffect::Snow() {
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;
  this->render.position.x = player.x;
  this->render.position.y = player.y;
  this->render.position.z = player.z;

  if (this->GetDuration() - this->state.stateCnt <= 160) {
    return;
  }

  for (int count = 2; count != 0; --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 21, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->orgPos = this->render.position;
    this->state.unknown_11C84->duration = 320;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    const float z = static_cast<float>(std::max(rand() % 300, 0) - 300);
    const float y = world->terrain->Grid().Height(this->render.position.x, this->render.position.z) - 100.0f;
    const float* m = particle.transform;
    particle.deltaPos2 = {
      y * m[3] + m[0] * 0.0f + z * m[6] + m[9],
      y * m[4] + m[1] * 0.0f + z * m[7] + m[10],
      y * m[5] + m[2] * 0.0f + z * m[8] + m[11]
    };
    reinterpret_cast<matrix*>(particle.transform)->MakeXRotation(GetRadian(90));
    this->state.unknown_11C84->speed = -0.5f;
    this->state.unknown_11C84->accel = 0.0f;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 15, 0) + 25) / 50.0f;
    this->state.unknown_11C84->rollSpeed = 0.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
    const int duration = this->state.unknown_11C84->duration;
    this->state.unknown_11C84->fadeOutCnt = duration - duration / 5;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\ef_snow.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\ef_snow.act");
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C12B90
// FUNCTION: fn_20042
void CRagEffect::Entry() {
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 55;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  this->state.unknown_11C84->accel = -0.00007f;
  this->state.unknown_11C84->longSpeed = -10.0f;
  this->state.unknown_11C84->outerSize = 6.0f;
  this->state.unknown_11C84->outerSpeed = 0.08f;
  this->state.unknown_11C84->innerSize = 5.0f;
  this->state.unknown_11C84->heightSize = 6.5f;
  this->state.unknown_11C84->heightSpeed = -0.1f;
  this->state.unknown_11C84->alpha = 5.0f;
  this->state.unknown_11C84->maxAlpha = 245.0f;
  this->state.unknown_11C84->alphaSpeed = 10.0f;
  int duration = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = duration - duration / 5;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 55;
    this->state.unknown_11C84->arcAngle = 45.0f;
    this->state.unknown_11C84->longSpeed = 10.0f;
    this->state.unknown_11C84->outerSize = 4.5f;
    this->state.unknown_11C84->innerSize = 4.5f;
    this->state.unknown_11C84->heightSpeed = 2.5f;
    EffectPrimitiveState& shrink = *this->state.unknown_11C84;
    const float accel = -(shrink.heightSpeed / static_cast<float>(shrink.duration));
    shrink.heightAccel = accel + accel;
    this->state.unknown_11C84->alpha = 160.0f;
    duration = this->state.unknown_11C84->duration;
    this->state.unknown_11C84->fadeOutCnt = duration - duration / 3;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C0E280
// FUNCTION: fn_20025
bool CRagEffect::CartRevolution() {
  if (this->state.stateCnt == 7 || this->state.stateCnt == 20) {
    PlayEffectWave(*this, "effect\\EF_MagnumBreak.wav");
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 20;
    this->state.unknown_11C84->renderFlag |= 0x200u;
    this->state.unknown_11C84->maxAlpha = 180.0f;
    this->state.unknown_11C84->latitude = 90.0f;
    this->state.unknown_11C84->radiusSpeed = 1.75f;
    EffectPrimitiveState& ring = *this->state.unknown_11C84;
    ring.radiusAccel = -(ring.radiusSpeed / static_cast<float>(ring.duration)) * 0.5f;
    this->state.unknown_11C84->innerSize = 5.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 14, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 20;
    this->state.unknown_11C84->radiusSpeed = 1.35f;
    EffectPrimitiveState& blast = *this->state.unknown_11C84;
    blast.radiusAccel = -(blast.radiusSpeed / static_cast<float>(blast.duration)) * 0.5f;
    this->state.unknown_11C84->longSpeed = 3.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 240.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 7.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\대폭발.tga", 0, false, true, false);
  }

  if (this->state.stateCnt < 300) {
    this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
    return true;
  }

  return false;
}

// OFFSET: 0x00C0DEF0
// FUNCTION: fn_20024
void CRagEffect::BowlingBash() {
  if (this->state.stateCnt != 0 && this->state.stateCnt != 5) {
    return;
  }

  const int facing = static_cast<int>(180.0f - this->state.parent->render.angle);
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20 - this->state.stateCnt;
  const int angle = facing < 0 ? facing + 360 : facing;
  this->state.unknown_11C84->longitude = static_cast<float>(angle + this->state.stateCnt * 20);
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  this->state.unknown_11C84->outerSize = 8.0f;
  this->state.unknown_11C84->outerSpeed = 0.5f;
  this->state.unknown_11C84->outerAccel = -0.03f;
  this->state.unknown_11C84->innerSize = 3.0f;
  this->state.unknown_11C84->innerSpeed = 0.5f;
  this->state.unknown_11C84->innerAccel = -0.03f;
  this->state.unknown_11C84->heightSize = 3.5f;
  this->state.unknown_11C84->alpha = static_cast<float>(this->state.stateCnt * -7 + 240);
  const int duration = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = duration - duration / 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 50;
    this->state.unknown_11C84->renderFlag |= 0x200u;
    this->state.unknown_11C84->latitude = 90.0f;
    this->state.unknown_11C84->radius = 8.0f;
    this->state.unknown_11C84->radiusSpeed = 0.7f;
    this->state.unknown_11C84->radiusAccel = -(this->state.unknown_11C84->radiusSpeed / 30.0f);
    this->state.unknown_11C84->innerSize = 5.0f;
    this->state.unknown_11C84->changePoint = 20;
    this->state.unknown_11C84->changeAccel = 0.0f;
    this->state.unknown_11C84->changeSpeed = 0.08f;
    this->state.unknown_11C84->alpha = 45.0f;
    this->state.unknown_11C84->fadeOutCnt = 35;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C3AF00
// FUNCTION: fn_20105
void CRagEffect::SonicBlowHit() {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\assasin_sonicblow.wav");

  if (this->state.stateCnt != 0) {
    return;
  }

  const int facing = static_cast<int>(180.0f - this->state.parent->render.angle);
  const int spreadX = rand();
  const int spreadY = rand();
  rand();
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 12;
  this->state.unknown_11C84->longitude = static_cast<float>(facing < 0 ? facing + 360 : facing);
  this->state.unknown_11C84->latitude = -90.0f;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  EffectPrimitiveState& lift = *this->state.unknown_11C84;
  const float* m = lift.transform;
  lift.speed3d = {
    (m[0] * 0.0f - m[3] * 2.0f) + m[6] * 0.0f + m[9],
    (m[1] * 0.0f - m[4] * 2.0f) + m[7] * 0.0f + m[10],
    (m[2] * 0.0f - m[5] * 2.0f) + m[8] * 0.0f + m[11]
  };
  lift.deltaPos.x = lift.speed3d.x + lift.deltaPos.x;
  lift.deltaPos.y = lift.speed3d.y + lift.deltaPos.y;
  lift.deltaPos.z = lift.speed3d.z + lift.deltaPos.z;
  this->state.unknown_11C84->deltaPos2.z = 0.0f;
  this->state.unknown_11C84->deltaPos2.x = static_cast<float>((std::max(spreadX % 10, 0) + 1) / 4);
  this->state.unknown_11C84->deltaPos2.y = static_cast<float>((std::max(spreadY % 20, 0) + 1) / 2 - 11);
  this->state.unknown_11C84->speed = 0.4f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  slow.accel = -(slow.speed / static_cast<float>(slow.duration)) * 0.5f;
  this->state.unknown_11C84->outerSize = 7.0f;
  this->state.unknown_11C84->innerSize = 4.0f;
  this->state.unknown_11C84->heightSize = 3.5f;
  const int duration = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = duration - duration / 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\magic_red.tga", 0, false, true, false);
}

// OFFSET: 0x00C130D0
// FUNCTION: fn_20043
void CRagEffect::Exit() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.duration = 100;
    this->state.unknown_11C84->duration = 100;
    this->state.unknown_11C84->outerSize = 4.5f;
    this->state.unknown_11C84->innerSize = 4.5f;
    this->state.unknown_11C84->heightSize = 35.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 90.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 30.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
    PlayEffectWave(*this, "_heal_effect.wav");
  }

  if (g_serviceOptions.Get(229) != 0) {
    return;
  }

  if (this->state.stateCnt % 9 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    rand();
    this->state.unknown_11C84->radius = 3.0f;
    this->state.unknown_11C84->duration = 50;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->gravSpeed = -1.2f;
    EffectPrimitiveState& rise = *this->state.unknown_11C84;
    rise.gravAccel = -(rise.gravSpeed / static_cast<float>(rise.duration)) / 1.5f;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, this->state.unknown_11C84->radius);
    this->state.unknown_11C84->size = 0.55f;
    const int duration = this->state.unknown_11C84->duration;
    this->state.unknown_11C84->fadeOutCnt = duration - duration / 3;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C3E9C0
// FUNCTION: fn_20118
void CRagEffect::Warp() {
  if (this->state.stateCnt % 20 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->renderFlag |= 0x200u;
    this->state.unknown_11C84->radius = 2.0f;
    this->state.unknown_11C84->arcAngle = 22.5f;
    this->state.unknown_11C84->innerSize = 10.0f;
    this->state.unknown_11C84->latitude = 90.0f;
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(this->state.unknown_11C84->latitude / 180.0f * 3.141592f);
    this->state.unknown_11C84->radiusSpeed = 1.25f;
    EffectPrimitiveState& ring = *this->state.unknown_11C84;
    ring.radiusAccel = -(ring.radiusSpeed / static_cast<float>(ring.duration)) * 0.5f;
    this->state.unknown_11C84->alpha = 200.0f;
    const int duration = this->state.unknown_11C84->duration;
    this->state.unknown_11C84->fadeOutCnt = duration - duration / 2;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("EFFECT\\RING_YELLOW.TGA", 0, false, true, false);
  }

  if ((this->state.stateCnt & 1) == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    const int band = std::max(rand() % 200, 0) / 100;
    this->state.unknown_11C84->radius = static_cast<float>(band) * 0.7f + 1.0f;
    this->state.unknown_11C84->duration = 40;
    const int longitude = std::max(rand() % 360, 0);
    const float spin = static_cast<float>(band) * 3.0f + 3.0f;
    this->state.unknown_11C84->longitude = static_cast<float>(longitude);
    this->state.unknown_11C84->longSpeed = spin;
    this->state.unknown_11C84->gravSpeed = static_cast<float>(60 - std::max(rand() % 240, 0)) / 100.0f;
    EffectPrimitiveState& rise = *this->state.unknown_11C84;
    rise.gravAccel = -(rise.gravSpeed / static_cast<float>(rise.duration)) * 0.5f;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, this->state.unknown_11C84->radius);
    this->state.unknown_11C84->size = 0.2f;
    const int life = this->state.unknown_11C84->duration;
    this->state.unknown_11C84->fadeOutCnt = life - life / 3;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C36530
// FUNCTION: fn_20092
void CRagEffect::PotionPillar(float heightSpeed, int duration) {
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = duration;
  this->state.unknown_11C84->outerSize = 4.5f;
  this->state.unknown_11C84->innerSize = 4.5f;
  this->state.unknown_11C84->heightSize = 10.0f;
  this->state.unknown_11C84->heightSpeed = heightSpeed;
  this->state.unknown_11C84->heightAccel = 0.01f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 90.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
}

// OFFSET: 0x00C36B00
// FUNCTION: fn_20094
bool CRagEffect::Potion_Berserk() {
  if (this->state.stateCnt == 0) {
    this->state.parent->SendMsg(nullptr, 127, 0, 250, 0, 120, 0, 120, 0, 0, 0, 0, 0, 0, 0);
    this->PotionPillar(1.0f, 50);
  }

  if (this->state.stateCnt == 18) {
    g_modeMgr.GetGameMode()->SendMsg(122, 1, 0, 0, 0);
    g_soundMgr->PlayWave("effect\\ac_concentration.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  this->render.position = this->state.parent->render.position;

  if (static_cast<unsigned int>(this->state.stateCnt - 13) < 97u && this->state.stateCnt % 4 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 40;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    EffectPrimitiveState& spot = *this->state.unknown_11C84;
    spot.deltaPos2 = RotatedForward(spot.transform, static_cast<float>(std::max(rand() % 7, 0) + 2));
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(90));
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 20) / 60.0f;
    this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
    this->state.unknown_11C84->heightSize = 0.2f;
    this->state.unknown_11C84->red = 240;
    this->state.unknown_11C84->green = 0;
    this->state.unknown_11C84->blue = 0;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 180.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
    this->state.unknown_11C84->slots[0].life = 0;
  }

  this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
  if (this->state.stateCnt < 151) {
    return true;
  }

  this->state.parent->SendMsg(nullptr, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  return false;
}

// OFFSET: 0x00C52370
// FUNCTION: fn_20193
void CRagEffect::GROUNDSAMPLE() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 112, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\magic_target.tga", 0, false, true, false);
    this->state.unknown_11C84->slots[0].life = 0;
    this->state.unknown_11C84->slots[1].life = 0;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].rotStart = 0;
  }
}

// OFFSET: 0x00C475C0
// FUNCTION: fn_20157
void CRagEffect::CALLZONE(const char* name) {
  if (this->state.stateCnt == 100) {
    this->render.position = this->state.parent->render.position;
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 74, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 1;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->render.position.y -= 10.0f;
    this->state.unknown_11C84->slots[0].life = 0;
    this->state.unknown_11C84->slots[1].life = 0;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].distance = 8.0f;
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  }
}

// OFFSET: 0x00C59490
// FUNCTION: fn_20212
bool CRagEffect::JobLvUp50_2(short angle) {
  this->render.position = this->state.parent->render.position;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 75, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\magic_blue.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].maxHeight = 1.5f;
    this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 90 + angle);
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].distance = 2.0f;
    this->state.unknown_11C84->slots[i].height[0] = 1.0f;
    this->state.unknown_11C84->slots[i].flag1[0] = 0;
  }

  return true;
}

// OFFSET: 0x00C629D0
// FUNCTION: fn_20252
bool CRagEffect::RunStop(short angle) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 75, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\magic_blue.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 4.0f;

    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].distance = 2.0f;
      this->state.unknown_11C84->slots[i].height[0] = 1.0f;
      this->state.unknown_11C84->slots[i].maxHeight = 0.0f;
      this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 90 + angle);
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].flag1[0] = 1;
    }
  }

  return true;
}

// OFFSET: 0x00C63860
// FUNCTION: fn_20256
void CRagEffect::SMA(char flag) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 120, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 0;
    this->state.unknown_11C84->size = 0.0f;
    this->state.unknown_11C84->deltaPos.x = this->state.deltaPos[0];
    this->state.unknown_11C84->deltaPos.y = this->state.deltaPos[1];
    this->state.unknown_11C84->deltaPos.z = this->state.deltaPos[2];
    Owner(this->state.unknown_11C84).render.position = this->render.position;

    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = i - 3;
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].vecBNow.x = this->render.position.x;
      this->state.unknown_11C84->slots[i].vecBNow.z = this->render.position.z;
      this->state.unknown_11C84->slots[i].vecBNow.y = (this->render.position.y - 10.0f) + static_cast<float>(std::max(rand() % 5, 0));
      this->state.unknown_11C84->slots[i].rotStart = 0;
      this->state.unknown_11C84->slots[i].height[1] = 0.8f;
      this->state.unknown_11C84->slots[i].flag1[0] = flag;
    }
  }
}

// OFFSET: 0x00C592E0
// FUNCTION: fn_20211
bool CRagEffect::JobLvUp50(int angle) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 5) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 75, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\magic_green.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 9.0f;

    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].maxHeight = 2.0f;
      this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 90 + angle);
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].distance = 2.0f;
      this->state.unknown_11C84->slots[i].height[0] = 1.0f;
      this->state.unknown_11C84->slots[i].flag1[0] = 0;
    }
  }

  if (angle == 0) {
    if (!this->ProcessEZ2STR(0.0f, -80.0f, 0.0f)) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00C5CA50
// FUNCTION: fn_20229
void CRagEffect::Map_Pika(const char* name) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 28, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 4.0f;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;
    this->state.unknown_11C84->slots[0].distance = 46.0f;
    this->state.unknown_11C84->slots[1].distance = 46.0f;
    this->state.unknown_11C84->slots[0].riseAngle = 0;
    this->state.unknown_11C84->slots[1].riseAngle = 180;
    this->state.unknown_11C84->slots[0].alphaB = 25;
    this->state.unknown_11C84->slots[1].alphaB = 25;
    this->state.unknown_11C84->slots[0].height[0] = -1.0f;
    this->state.unknown_11C84->slots[1].height[0] = -1.0f;
  }
}

// OFFSET: 0x00C5D390
// FUNCTION: fn_20233
void CRagEffect::POISON(char flag) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 72, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag |= 0x200u;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  const char* textureName = flag == 0 ? "effect\\poison_f.bmp" : "effect\\white02.bmp";
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(textureName, 0, false, true, false);
  this->render.position.y -= 9.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 30.0f;
  this->state.unknown_11C84->slots[0].maxHeight = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].flag1[10] = flag;
}

// OFFSET: 0x00C61A80
// FUNCTION: fn_20248
void CRagEffect::Rainbow(const char* name, int unused) {
  if (this->state.stateCnt == 0) {
    const CView* camera = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
    this->render.position = this->state.parent->render.position;
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 93, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 1;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->slots[0].alphaT = static_cast<std::int16_t>(camera->camera.yaw);
    this->state.unknown_11C84->slots[0].alphaT = static_cast<std::int16_t>(360 - this->state.unknown_11C84->slots[0].alphaT);
    this->state.unknown_11C84->slots[0].life = 0;
    this->state.unknown_11C84->slots[1].life = 0;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 50.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 180;
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].riseAngle = this->state.unknown_11C84->slots[0].alphaT;
  }
}

// OFFSET: 0x00C423D0
// FUNCTION: fn_20138
void CRagEffect::BLIND(char flag) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 71, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag |= 0x200u;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 2;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\fullb.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);
    this->render.position.y -= 9.0f;
    this->state.unknown_11C84->slots[0].life = 0;
    this->state.unknown_11C84->slots[1].life = 0;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;

    if (flag == 0) {
      this->state.unknown_11C84->slots[0].distance = 65.0f;
      this->state.unknown_11C84->slots[0].flag1[0] = 0;
    } else {
      this->state.unknown_11C84->slots[0].distance = 140.0f - static_cast<float>(flag * 10);
      this->state.unknown_11C84->slots[0].flag1[0] = 1;
    }

    this->state.unknown_11C84->slots[0].alphaB = 0;
  }
}

// OFFSET: 0x00C47E10
// FUNCTION: fn_20160
void CRagEffect::ShockwaveRing(std::int16_t angle) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 54, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\shockwave_c.bmp", 0, false, true, false);
  this->render.position.y -= 9.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 90 + angle);
    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C552F0
// FUNCTION: fn_20202
void CRagEffect::TexturedPulse(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 104, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0x201;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 8.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 8.0f;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 180;
}

// OFFSET: 0x00C14FE0
// FUNCTION: fn_20048
void CRagEffect::RingStack(char white) {
  if (this->state.stateCnt != 0) {
    return;
  }

  for (int index = 1; index <= 3; ++index) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->longSpeed = -3.0f;
    this->state.unknown_11C84->outerSize = static_cast<float>(index + 3);
    this->state.unknown_11C84->innerSize = static_cast<float>(index + 1.5);
    this->state.unknown_11C84->heightSize = static_cast<float>((8 - 2 * index) * (5 - index) + 1) + 2.5f;
    this->state.unknown_11C84->rollSpeed = -0.5f;
    this->state.unknown_11C84->alpha = static_cast<float>(this->state.stateCnt * -7 + 240);
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(white != 0 ? "effect\\ring_white.tga" : "effect\\magic_red.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C41280
// FUNCTION: fn_20134
void CRagEffect::TexturedBurst(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 105, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0x201;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 100.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 8.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = 8.0f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[index].height[0] = 20.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45);
  }
}

// OFFSET: 0x00C515A0
// FUNCTION: fn_20188
void CRagEffect::TexturedRise(const char* name, char large) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 67, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 1;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = large != 1 ? 0.0f : 100.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = large != 1 ? 5.0f : 6.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 0;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(this->state.deltaPos[0]);
  this->state.unknown_11C84->slots[0].alphaB = 150;
  this->state.unknown_11C84->slots[0].height[0] = -0.2f;
}

// OFFSET: 0x00C70F10
// FUNCTION: fn_20317
void CRagEffect::TexturedSpiral(const char* name, int, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 3) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 162, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;
    this->state.unknown_11C84->slots[index].alphaB = 255;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((this->state.stateCnt * 4 + index) * 30);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
  }
}

// OFFSET: 0x00C45050
// FUNCTION: fn_20147
void CRagEffect::TexturedSpin(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 114, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 5.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].distance = 1.6f;
  this->state.unknown_11C84->slots[0].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].height[0] = -12.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = static_cast<char>(std::max(rand() % 31, 0));
}

// OFFSET: 0x00C63240
// FUNCTION: fn_20254
void CRagEffect::ColoredFlash(unsigned char flag, unsigned char red, unsigned char green, unsigned char blue) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 172, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag |= 0x200;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);
  this->render.position.y -= 9.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->argb = red | (green << 8) | (blue << 16);
  this->state.unknown_11C84->slots[0].flag1[10] = static_cast<char>(flag);
}

// OFFSET: 0x00C6F710
// FUNCTION: fn_20309
void CRagEffect::TexturedBeam(char flag, const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 154, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 100.0f;
  this->state.unknown_11C84->slots[0].height[0] = 50.0f;
  this->state.unknown_11C84->slots[0].maxHeight = 5.0f;
  this->state.unknown_11C84->slots[0].height[2] = 80.0f;
  Get_Angle(
    this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[0].riseAngle
  );
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = flag;
}

// OFFSET: 0x00C70980
// FUNCTION: fn_20314
void CRagEffect::TexturedSpiralSmall(const char* name, int, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 3) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 163, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 50.0f;
    this->state.unknown_11C84->slots[index].alphaB = 255;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((this->state.stateCnt * 4 + index) * 30);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
  }
}

// OFFSET: 0x00C70B50
// FUNCTION: fn_20315
void CRagEffect::TexturedSpiralTall(const char* name, int, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 3) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 164, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;
    this->state.unknown_11C84->slots[index].maxHeight = 15.0f;
    this->state.unknown_11C84->slots[index].alphaB = 255;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((this->state.stateCnt * 4 + index) * 30);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
  }
}

// OFFSET: 0x00C70D30
// FUNCTION: fn_20316
void CRagEffect::TexturedSpiralFade(const char* name, int, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 3) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 165, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 0.0f;
    this->state.unknown_11C84->slots[index].height[10] = 0.0f;
    this->state.unknown_11C84->slots[index].maxHeight = 15.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((this->state.stateCnt * 4 + index) * 30);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
  }
}

// OFFSET: 0x00C672B0
// FUNCTION: fn_20278
void CRagEffect::TexturedScatter(const char* name, char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 44, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->render.position.y -= 9.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = -10 - std::max(rand() % 25, 0);
    this->state.unknown_11C84->slots[index].distance = -10.0f;
    this->state.unknown_11C84->slots[index].height[0] = 3.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = 0.7f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = flag;
    this->state.unknown_11C84->slots[index].flag1[1] = 24;
  }
}

// OFFSET: 0x00C67780
// FUNCTION: fn_20280
void CRagEffect::TexturedScatterLow(const char* name, char flag) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 44, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->render.position.y -= 9.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = -20 - std::max(rand() % 10, 0);
    this->state.unknown_11C84->slots[index].distance = -10.0f;
    this->state.unknown_11C84->slots[index].height[0] = 2.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = 1.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = flag;
    this->state.unknown_11C84->slots[index].flag1[1] = 16;
  }
}

// OFFSET: 0x00C6E6F0
// FUNCTION: fn_20303
void CRagEffect::TexturedDualSpin(const char* first, const char* second, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 148, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(first, 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load(second, 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = this->state.param[0] >= 5.0f ? this->state.param[0] : 20.0f;
  const float spin = this->state.param[1];
  this->state.unknown_11C84->slots[0].rotStart = spin < 0.0f || spin >= 360.0f ? 45 : static_cast<std::int16_t>(spin);
  this->state.unknown_11C84->slots[0].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[0] = -2.0f;
  this->state.unknown_11C84->slots[0].height[1] = 0.0f;
}

// OFFSET: 0x00C6E8F0
// FUNCTION: fn_20304
void CRagEffect::TexturedDrop(const char* name, char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 156, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 6.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 120;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 6.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 45;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[1] = flag;
}

// OFFSET: 0x00C65330
// FUNCTION: fn_20268
void CRagEffect::PurpleSlash(std::int16_t angle) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 92, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\purpleslash.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  const int offset = std::max(rand() % 6, 0);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = index - 28 - offset;
    this->state.unknown_11C84->slots[index].distance = 20.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.y = static_cast<float>(std::max(rand() % 5, 0)) + (this->render.position.y - 10.0f);
    this->state.unknown_11C84->slots[index].rotStart = angle;
    this->state.unknown_11C84->slots[index].height[1] = 0.8f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 0;
  }
}

// OFFSET: 0x00C647D0
// FUNCTION: fn_20262
void CRagEffect::TexturedPair(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 76, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 10.0f;
  this->state.unknown_11C84->slots[0].distance = 2.0f;
  this->state.unknown_11C84->slots[0].height[0] = -8.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 0;
  this->state.unknown_11C84->slots[0].alphaB = 200;
  this->state.unknown_11C84->slots[0].riseAngle = 0;
  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].maxHeight = 10.0f;
  this->state.unknown_11C84->slots[1].distance = 2.0f;
  this->state.unknown_11C84->slots[1].height[0] = -8.0f;
  this->state.unknown_11C84->slots[1].rotStart = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 0;
  this->state.unknown_11C84->slots[1].alphaB = 200;
  this->state.unknown_11C84->slots[1].riseAngle = 0;
}

// OFFSET: 0x00C549C0
// FUNCTION: fn_20199
void CRagEffect::BlackMaximize(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\black_maximize_power_sword_bic.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 61, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\a01.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 11.0f;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 10.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 1.0f;
  this->state.unknown_11C84->slots[0].height[0] = 0.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = 1;
}

// OFFSET: 0x00C3FBD0
// FUNCTION: fn_20122
void CRagEffect::WaterOut() {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\wizard_waterball_chulung.wav");

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->pattern = 0x40;
  this->state.unknown_11C84->pattern |= 0x100;
  this->state.unknown_11C84->renderFlag |= 2;
  this->state.unknown_11C84->repeatAnim = 1;
  this->state.unknown_11C84->widthSize = 3.5f;
  this->state.unknown_11C84->heightSize = 3.5f;
  this->state.unknown_11C84->speed3d.y = -(25.0f / static_cast<float>(this->state.unknown_11C84->duration));
  this->state.unknown_11C84->deltaPos2.y = -5.0f;
  this->state.unknown_11C84->animSpeed = 5;
  this->state.unknown_11C84->totalTexture = 3;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\water_out_a.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\water_out_b.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\water_out_c.bmp", 0, false, true, false);
}

// OFFSET: 0x00C5A550
// FUNCTION: fn_20218
void CRagEffect::TexturedDoubleRing(const char* name, char level) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 28, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag = 13;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (level == 1) {
    this->state.unknown_11C84->size = 15.0f;
  } else if (level == 2) {
    this->state.unknown_11C84->size = 17.0f;
  } else if (level == 3) {
    this->state.unknown_11C84->size = 18.0f;
  } else {
    this->state.unknown_11C84->size = 4.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].distance = level == 3 ? 24.0f : 15.0f;
  this->state.unknown_11C84->slots[1].distance = this->state.unknown_11C84->slots[0].distance;
  this->state.unknown_11C84->slots[0].riseAngle = 0;
  this->state.unknown_11C84->slots[1].riseAngle = 180;
  this->state.unknown_11C84->slots[0].alphaB = 200;
  this->state.unknown_11C84->slots[1].alphaB = 200;
  this->state.unknown_11C84->slots[0].height[0] = -1.0f;
  this->state.unknown_11C84->slots[1].height[0] = -1.0f;
}

// OFFSET: 0x00C41470
// FUNCTION: fn_20136
void CRagEffect::TexturedQuadSpin(const char* name, char phase, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 89, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  const float scale = static_cast<float>(kind);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].height[4] = scale;
    std::int16_t base;

    if (kind == 2 || kind == 3 || kind == 6) {
      this->state.unknown_11C84->slots[index].process = 0;
      this->state.unknown_11C84->slots[index].distance = 2.0f;
      base = kind == 3 ? static_cast<std::int16_t>(phase * 7) : static_cast<std::int16_t>(phase * 22);
    } else {
      this->state.unknown_11C84->slots[index].process = -24;
      this->state.unknown_11C84->slots[index].distance = 2.0f;
      base = static_cast<std::int16_t>(phase * 22);
    }

    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(base + index * 90);
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].height[0] = -12.0f;
  }
}

// OFFSET: 0x00C42B40
// FUNCTION: fn_20139
void CRagEffect::TexturedSkillIcon(const char* name, std::int16_t level, std::int16_t mode) {
  char iconName[64];
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != (level != 0 ? level : 0)) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 68, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  this->state.unknown_11C84->textures = new CTexture*[this->state.unknown_11C84->totalTexture];

  if (mode == 2) {
    std::sprintf(iconName, "유저인터페이스\\item\\%s.bmp", CSkillInfo::IdName(*g_session.m_hotkeyScript, 2285));
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(iconName, 0, false, true, false);
  } else if (level > 0) {
    int skillId;
    if (rand() % 2 == 0) {
      skillId = rand() % 12 + 9;
    } else {
      skillId = rand() % 13 + 80;
    }

    std::sprintf(iconName, "유저인터페이스\\item\\%s.bmp", CSkillInfo::IdName(*g_session.m_hotkeyScript, skillId));
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(iconName, 0, false, true, false);
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  }

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;

  if (level > 0) {
    this->state.unknown_11C84->slots[0].distance = 5.0f;
    this->state.unknown_11C84->slots[0].height[0] = 1.0f;
    this->state.unknown_11C84->slots[0].flag1[4] = 4;
  } else {
    this->state.unknown_11C84->slots[0].distance = mode == 3 ? 7.0f : 18.0f;
    this->state.unknown_11C84->slots[0].height[0] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[4] = 1;
  }

  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;

  for (int index = 1; index < 21; ++index) {
    this->state.unknown_11C84->slots[0].height[index] = 0.0f;
  }

  this->state.unknown_11C84->slots[0].flag1[5] = 0;
}

// OFFSET: 0x00C59960
// FUNCTION: fn_20214
void CRagEffect::TrackTarget() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const CRenderObject* target = nullptr;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 38, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_center.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].maxHeight = 0.2f;
  this->state.unknown_11C84->slots[0].height[4] = 2.0f;
  this->state.unknown_11C84->slots[0].vecTNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecTNow.y = this->render.position.y;
  this->state.unknown_11C84->slots[0].vecTNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].height[0] = this->state.deltaPos[1];

  if (this->state.unknown_11C84->slots[0].height[0] > 0.0f) {
    if (this->state.unknown_11C84->slots[0].height[0] == static_cast<float>(g_session.m_aid)) {
      target = reinterpret_cast<const CRenderObject*>(gameMode->scene->player);
    } else {
      target = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(this->state.unknown_11C84->slots[0].height[0]));
    }
  }

  if (target == nullptr) {
    this->state.unknown_11C84->slots[0].life = 0;
  } else {
    this->state.unknown_11C84->slots[0].vecTPre.x = target->render.position.x;
    this->state.unknown_11C84->slots[0].vecTPre.y = target->render.position.y;
    this->state.unknown_11C84->slots[0].vecTPre.z = target->render.position.z;
  }

  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C59B80
// FUNCTION: fn_20215
void CRagEffect::TrackTargetWide() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const CRenderObject* target = nullptr;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 38, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_center.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].maxHeight = 0.7f;
  this->state.unknown_11C84->slots[0].height[4] = 3.0f;
  this->state.unknown_11C84->slots[0].vecTNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecTNow.y = this->render.position.y;
  this->state.unknown_11C84->slots[0].vecTNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].height[0] = this->state.deltaPos[1];

  if (this->state.unknown_11C84->slots[0].height[0] > 0.0f) {
    if (this->state.unknown_11C84->slots[0].height[0] == static_cast<float>(g_session.m_aid)) {
      target = reinterpret_cast<const CRenderObject*>(gameMode->scene->player);
    } else {
      target = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(this->state.unknown_11C84->slots[0].height[0]));
    }
  }

  if (target == nullptr) {
    this->state.unknown_11C84->slots[0].life = 0;
  } else {
    this->state.unknown_11C84->slots[0].vecTPre.x = target->render.position.x;
    this->state.unknown_11C84->slots[0].vecTPre.y = target->render.position.y;
    this->state.unknown_11C84->slots[0].vecTPre.z = target->render.position.z;
  }

  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C59DA0
// FUNCTION: fn_20216
void CRagEffect::TrackCloud(char kind) {
  float scale = 0.0f;
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const CRenderObject* target = nullptr;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 38, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].alphaB = 45;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].maxHeight = 0.4f;

  if (kind == 2) {
    scale = 4.0f;
  }

  this->state.unknown_11C84->slots[0].height[4] = scale;
  this->state.unknown_11C84->slots[0].vecTNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecTNow.y = this->render.position.y;
  this->state.unknown_11C84->slots[0].vecTNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].height[0] = this->state.deltaPos[1];

  if (this->state.unknown_11C84->slots[0].height[0] > 0.0f) {
    if (this->state.unknown_11C84->slots[0].height[0] == static_cast<float>(g_session.m_aid)) {
      target = reinterpret_cast<const CRenderObject*>(gameMode->scene->player);
    } else {
      target = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(this->state.unknown_11C84->slots[0].height[0]));
    }
  }

  if (target == nullptr) {
    this->state.unknown_11C84->slots[0].life = 0;
  } else {
    this->state.unknown_11C84->slots[0].vecTPre.x = target->render.position.x;
    this->state.unknown_11C84->slots[0].vecTPre.y = target->render.position.y;
    this->state.unknown_11C84->slots[0].vecTPre.z = target->render.position.z;
  }

  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C64590
// FUNCTION: fn_20261
void CRagEffect::TexturedSlashFan(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 119, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = index - 3;
    this->state.unknown_11C84->slots[index].distance = 20.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.y = static_cast<float>(std::max(rand() % 5, 0)) + (this->render.position.y - 10.0f);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index);
    this->state.unknown_11C84->slots[index].height[1] = 0.8f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[3] = 0;
  }
}

// OFFSET: 0x00C66460
// FUNCTION: fn_20274
void CRagEffect::TexturedMark(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 103, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 40.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].distance = 2.0f;
  this->state.unknown_11C84->slots[0].height[0] = 50.0f;
  this->state.unknown_11C84->slots[0].height[1] = 5.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = 10;
  this->state.unknown_11C84->slots[0].flag1[4] = kind;
  this->state.unknown_11C84->slots[0].flag1[5] = 0;

  if (kind == 10) {
    this->state.unknown_11C84->slots[0].maxHeight = 14.0f;
    this->state.unknown_11C84->slots[0].height[0] = 70.0f;
    this->state.unknown_11C84->slots[0].flag1[0] = 14;
    this->state.unknown_11C84->slots[0].distance = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[5] = 1;
    return;
  }

  if (kind == 2) {
    this->state.unknown_11C84->slots[0].flag1[5] = 10;
  }
}

// OFFSET: 0x00C61830
// FUNCTION: fn_20247
void CRagEffect::TexturedSpray(const char* name) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 43, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->render.position.y -= 12.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->state.deltaPos[0];
    this->state.unknown_11C84->slots[index].vecBNow.y = this->state.deltaPos[1];
    this->state.unknown_11C84->slots[index].vecBNow.z = this->state.deltaPos[2];
    this->state.unknown_11C84->slots[index].process = -18 - std::max(rand() % 30, 0);
    this->state.unknown_11C84->slots[index].distance = 12.0f;
    this->state.unknown_11C84->slots[index].height[0] = 1.5f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = 3.5f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = 1;
  }
}

// OFFSET: 0x00C54770
// FUNCTION: fn_20198
void CRagEffect::TexturedLevelRing(const char* name, int process, char level) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 88, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (level == 1) {
    this->state.unknown_11C84->size = 101.0f;
  } else if (level == 2) {
    this->state.unknown_11C84->size = 102.0f;
  } else if (level == 3) {
    this->state.unknown_11C84->size = 111.0f;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = process;
    this->state.unknown_11C84->slots[0].distance = 13.0f;
    this->state.unknown_11C84->slots[0].height[0] = -5.0f - static_cast<float>(rand() % 6);
    goto finish;
  } else if (level == 4) {
    this->state.unknown_11C84->size = 113.0f;
  } else if (level == 10) {
    this->state.unknown_11C84->size = 114.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = process;
  this->state.unknown_11C84->slots[0].distance = 15.0f;
  this->state.unknown_11C84->slots[0].height[0] = -5.0f;

finish:
  this->state.unknown_11C84->slots[0].riseAngle = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C47740
// FUNCTION: fn_20158
void CRagEffect::SlashFan(std::int16_t reverse) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 40) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 55, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag |= 0x200;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\slash01.tga", 0, false, true, false);
  this->render.position.y -= 9.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;
    this->state.unknown_11C84->slots[index].maxHeight = 1.0f;
    this->state.unknown_11C84->slots[index].height[0] = reverse == 0 ? -15.0f - static_cast<float>(index) * 2.5f : static_cast<float>(index) * 2.5f + 15.0f;
    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].riseAngle
    );

    if (this->state.unknown_11C84->slots[index].riseAngle > 359) {
      this->state.unknown_11C84->slots[index].riseAngle -= 360;
    }

    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C65F80
// FUNCTION: fn_20272
void CRagEffect::FeatherBurst(const char* name, std::int16_t angle, char) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 91, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 10.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = -std::max(rand() % 20, 0);
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 11, 0)) * 0.2f + 2.0f;
    this->state.unknown_11C84->slots[index].distance = 5.0f;
    this->state.unknown_11C84->slots[index].height[0] = -10.0f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45 + angle);
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 91, 0) + 180);
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 4, 0) + 3);
  }
}

// OFFSET: 0x00C661F0
// FUNCTION: fn_20273
void CRagEffect::FeatherRing(const char* name, std::int16_t angle, char large) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 77, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = large == 1 ? 10.0f : 4.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 11, 0)) * 0.2f + 4.0f;
    this->state.unknown_11C84->slots[index].distance = 14.0f;
    this->state.unknown_11C84->slots[index].height[0] = -14.0f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45 + angle);
    this->state.unknown_11C84->slots[index].alphaB = 100;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 91, 0) + 180);
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 4, 0) + 3);
  }
}

// OFFSET: 0x00C6CB70
// FUNCTION: fn_20297
void CRagEffect::TexturedSlam(const char* name, std::int16_t kind, char) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 147, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;

  if (kind == 1) {
    this->state.unknown_11C84->slots[0].distance = 21.0f;
    this->state.unknown_11C84->slots[0].flag1[4] = 6;
  } else if (kind == 2) {
    this->state.unknown_11C84->slots[0].distance = 21.0f;
    this->state.unknown_11C84->slots[0].flag1[4] = 7;
  } else {
    this->state.unknown_11C84->slots[0].distance = 13.0f;
    this->state.unknown_11C84->slots[0].flag1[4] = 5;
  }

  this->state.unknown_11C84->slots[0].height[0] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;

  for (int index = 1; index < 21; ++index) {
    this->state.unknown_11C84->slots[0].height[index] = 0.0f;
  }
}

// OFFSET: 0x00C6CDE0

// OFFSET: 0x00C6D2C0

// OFFSET: 0x00C54BE0
// FUNCTION: fn_20200
void CRagEffect::KyrieGuard(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\kyrie_guard.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 61, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\guardK2.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 11.0f;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 7.8f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 1.0f;
  this->state.unknown_11C84->slots[0].height[0] = 0.0f;

  if (kind == 2) {
    this->state.unknown_11C84->slots[0].flag1[0] = 2;
    return;
  }

  if (kind == 4) {
    this->state.unknown_11C84->slots[0].flag1[0] = 4;
    return;
  }

  if (kind == 5) {
    this->state.unknown_11C84->slots[0].flag1[0] = 5;
    return;
  }

  this->state.unknown_11C84->slots[0].flag1[0] = 0;

  if (kind == 3) {
    this->state.unknown_11C84->slots[0].flag1[0] = 3;
  }
}

// OFFSET: 0x00C719E0
// FUNCTION: fn_20320
void CRagEffect::TexturedColumn(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& origin = this->state.parent->render;
  this->render.position.x = origin.position.x;
  this->render.position.y = origin.position.y;
  this->render.position.z = origin.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 160, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;

  if (kind == 30) {
    this->state.unknown_11C84->slots[0].distance = 36.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 19;
  } else if (kind == 31) {
    this->state.unknown_11C84->slots[0].distance = 36.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 21;
  } else {
    this->state.unknown_11C84->slots[0].distance = 5.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 10.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 2;
  }

  this->state.unknown_11C84->slots[0].height[10] = 0.0f;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[4] = kind;
}

// OFFSET: 0x00C5B2F0
// FUNCTION: fn_20220
void CRagEffect::TexturedSparkle(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& origin = this->state.parent->render;
  this->render.position.x = origin.position.x;
  this->render.position.y = origin.position.y;
  this->render.position.z = origin.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 81, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].alphaB = 40;
    this->state.unknown_11C84->slots[index].distance = 1.0f;
    this->state.unknown_11C84->slots[index].height[3] = 10.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 2, 0) + 1);

    if (std::max(rand() % 2, 0) == 0) {
      this->state.unknown_11C84->slots[index].height[0] = 105.0f;
      this->state.unknown_11C84->slots[index].height[1] = 105.0f;
      this->state.unknown_11C84->slots[index].height[2] = 255.0f;
    } else {
      this->state.unknown_11C84->slots[index].height[0] = 55.0f;
      this->state.unknown_11C84->slots[index].height[1] = 55.0f;
      this->state.unknown_11C84->slots[index].height[2] = 225.0f;
    }
  }
}

// OFFSET: 0x00C615A0
// FUNCTION: fn_20246
void CRagEffect::TexturedSprayKind(const char* name, float spin, char kind) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 43, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->render.position.y -= 12.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->state.deltaPos[0];
    this->state.unknown_11C84->slots[index].vecBNow.y = this->state.deltaPos[1];
    this->state.unknown_11C84->slots[index].vecBNow.z = this->state.deltaPos[2];
    this->state.unknown_11C84->slots[index].process = -18 - std::max(rand() % 30, 0);
    this->state.unknown_11C84->slots[index].distance = 12.0f;
    this->state.unknown_11C84->slots[index].height[0] = 1.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = spin;
    this->state.unknown_11C84->slots[index].alphaB = 0;

    if (kind == 1) {
      this->state.unknown_11C84->slots[index].flag1[0] = 6;
    } else if (kind == 7) {
      this->state.unknown_11C84->slots[index].flag1[0] = 7;
    } else if (kind == 8) {
      this->state.unknown_11C84->slots[index].flag1[0] = 8;
    } else {
      this->state.unknown_11C84->slots[index].flag1[0] = kind == 9 ? 9 : 2;
    }
  }
}

// OFFSET: 0x00C64040
// FUNCTION: fn_20259
void CRagEffect::TexturedSlashWave(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 125, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.5f + 10.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y - 10.0f;
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index);
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 21, 0)) * 0.1f + 1.0f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 0;

    if (kind == 0) {
      this->state.unknown_11C84->slots[index].flag1[2] = 0;
    } else if (kind == 1) {
      this->state.unknown_11C84->slots[index].flag1[2] = 1;
    }

    this->state.unknown_11C84->slots[index].flag1[3] = 1;
  }
}

// OFFSET: 0x00C48C90
// FUNCTION: fn_20164
void CRagEffect::TexturedBeams(const char* name, char flag) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 129, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 3; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].alphaB = 30;
    this->state.unknown_11C84->slots[index].distance = 0.0f;
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(index) * 0.5f + 3.0f;
    this->state.unknown_11C84->slots[index].height[0] = this->state.unknown_11C84->slots[index].maxHeight;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 25);
    this->state.unknown_11C84->slots[index].vecTNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecTNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecTNow.z = this->render.position.z;
    this->state.deltaPos[0] = this->render.position.x;
    this->state.deltaPos[1] = this->render.position.y;
    this->state.deltaPos[2] = this->render.position.z;
    this->state.deltaPos[1] -= 40.0f;
    this->state.unknown_11C84->slots[index].vecTPre.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecTPre.y = this->state.deltaPos[1];
    this->state.unknown_11C84->slots[index].vecTPre.z = this->state.deltaPos[2];
    this->state.unknown_11C84->slots[index].flag1[0] = flag;
  }
}

// OFFSET: 0x00C6F4B0
// FUNCTION: fn_20308
void CRagEffect::TexturedSwirl(unsigned char clockwise, const char* name) {
  if (name == nullptr) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 56, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0x201;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (EffectPrimitiveSlot& slot : this->state.unknown_11C84->slots) {
    slot.life = 1;
    slot.process = 0;
    slot.distance = 100.0f;
    slot.maxHeight = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 0.5f;
    slot.flag1[0] = static_cast<char>(std::max(rand() % 3, 0) + 125);
    slot.height[0] = (static_cast<float>(std::max(rand() % 11, 0)) + 15.0f) * static_cast<float>(clockwise * 2 - 1);
    slot.height[1] = static_cast<float>(std::max(rand() % 200, 0));
    slot.height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 40.0f;
    slot.height[3] = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 1.0f;
    Get_Angle(this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &slot.riseAngle);
    slot.alphaB = 0;
  }
}

// OFFSET: 0x00C470D0
// FUNCTION: fn_20155
void CRagEffect::TexturedFlare(const char* name, char size, int, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 36, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = static_cast<float>(size);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 15.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = static_cast<std::int16_t>(std::max(rand() % 100, 0));
  this->state.unknown_11C84->slots[0].distance = 2.6f;
  this->state.unknown_11C84->slots[0].riseAngle = 80;

  for (int index = 0; index < 21; ++index) {
    this->state.unknown_11C84->slots[0].height[index] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[index] = 0;
  }
}

// OFFSET: 0x00C45200
// FUNCTION: fn_20148
void CRagEffect::TexturedHalo(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 39, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  if (kind == 3) {
    this->state.unknown_11C84->slots[0].distance = 10.0f;
    this->state.unknown_11C84->slots[0].rotStart = 225;
    this->state.unknown_11C84->slots[0].alphaB = 250;
    this->state.unknown_11C84->size = 111.0f;
  } else if (kind == 4) {
    this->state.unknown_11C84->slots[0].distance = 12.0f;
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 200;
    this->state.unknown_11C84->size = 4.0f;
  } else if (kind == 10 || kind == 11 || kind == 12) {
    this->state.unknown_11C84->slots[0].distance = kind == 10 ? 30.0f : (kind == 11 ? 28.0f : 20.0f);
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 200;
    this->state.unknown_11C84->size = 10.0f;
  } else {
    this->state.unknown_11C84->slots[0].distance = kind == 2 ? 12.0f : 7.0f;
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 100;
    this->state.unknown_11C84->size = 5.0f;
  }

  this->state.unknown_11C84->slots[0].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[0] = -2.0f;
  this->state.unknown_11C84->slots[0].height[4] = static_cast<float>(kind);

  if (kind == 10 || kind == 11 || kind == 12) {
    this->state.unknown_11C84->slots[0].riseAngle = 360;
  }
}

// OFFSET: 0x00C642E0
// FUNCTION: fn_20260
void CRagEffect::TexturedSlashCross(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 5) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 126, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = index - 3;
    this->state.unknown_11C84->slots[index].distance = 20.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;

    switch (this->state.stateCnt) {
    case 0:
      this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
      this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
      break;
    case 1:
      this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x + 5.0f;
      this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
      break;
    case 2:
      this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x - 5.0f;
      this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
      break;
    case 3:
      this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
      this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z + 5.0f;
      break;
    default:
      this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
      this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z - 5.0f;
      break;
    }

    this->state.unknown_11C84->slots[index].vecBNow.y = static_cast<float>(std::max(rand() % 5, 0)) + (this->render.position.y - 10.0f);
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index);
    this->state.unknown_11C84->slots[index].height[1] = 1.5f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[3] = 1;
  }
}

// OFFSET: 0x00C65820
// FUNCTION: fn_20270
void CRagEffect::TexturedOrbit(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 109, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 2.5f;
  this->state.unknown_11C84->slots[0].height[10] = this->state.deltaPos[1];
  this->state.unknown_11C84->slots[0].height[13] = 0.0f;
  this->state.unknown_11C84->slots[0].height[14] = 0.0f;
  this->state.unknown_11C84->slots[0].height[15] = 0.0f;
  this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].vecTNow.x = 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.x = -g_sinTable[90] * 30.0f;
  this->state.unknown_11C84->slots[0].vecBPre.y = -(g_sinTable[0] * 20.0f) - 40.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = 0.0f;
}

// OFFSET: 0x00C612D0
// FUNCTION: fn_20245
void CRagEffect::TexturedCross(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 41, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 5.0f;

  if (kind != 0) {
    return;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 200;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[index].maxHeight = 120.0f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    this->state.unknown_11C84->slots[index].alphaB = 40;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(index) * 0.5f + 6.0f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(83 - index);
    this->state.unknown_11C84->slots[index].alphaT = 1;
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
  }
}

// OFFSET: 0x00C63D60
// FUNCTION: fn_20258
void CRagEffect::TexturedSlashLevel(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 122, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;

    if (kind == 4 || kind == 5) {
      this->state.unknown_11C84->slots[index].distance = 25.0f;
      this->state.unknown_11C84->slots[index].flag1[3] = 3;
    } else if (kind < 2) {
      this->state.unknown_11C84->slots[index].distance = 25.0f;
      this->state.unknown_11C84->slots[index].flag1[3] = 0;
    } else {
      this->state.unknown_11C84->slots[index].distance = 6.0f;
      this->state.unknown_11C84->slots[index].flag1[3] = 2;
    }

    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y - 10.0f;
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index);
    this->state.unknown_11C84->slots[index].height[1] = 2.0f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 0;

    if (kind == 0 || kind == 2 || kind == 4) {
      this->state.unknown_11C84->slots[index].flag1[2] = 0;
      this->state.unknown_11C84->slots[index].flag1[4] = kind == 2 ? 1 : 0;
    } else if (kind == 1 || kind == 3 || kind == 5) {
      this->state.unknown_11C84->slots[index].flag1[2] = 1;
      this->state.unknown_11C84->slots[index].flag1[4] = kind == 3 ? 1 : 0;
    } else {
      this->state.unknown_11C84->slots[index].flag1[4] = 0;
    }
  }
}

// OFFSET: 0x00C44940
// FUNCTION: fn_20144
void CRagEffect::SaintWing(const char* name) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 131, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 24.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].height[0] = 0.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[4] = 1;
  for (int i = 1; i < 21; ++i) {
    this->state.unknown_11C84->slots[0].height[i] = 0.0f;
  }
}

// OFFSET: 0x00C50E30
// FUNCTION: fn_20186
void CRagEffect::FirstAid() {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  g_soundMgr->PlayWave(
    "_heal_effect.wav", this->render.position.x - player->render.position.x, 150.0f, this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
  );

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 58, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pikapika2.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 10.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 12.0f;
  this->state.unknown_11C84->slots[0].height[6] = 0.02f;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 1;
  this->state.unknown_11C84->slots[0].height[8] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C51D80
// FUNCTION: fn_20191
void CRagEffect::GRANDCROSS2(char flag) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 37, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(flag == 0 ? "effect\\ring_yellow.tga" : "effect\\ring_black.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 1.0f;
  for (int i = 0; i < 2; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].maxHeight = 60.0f;
    this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 90);
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].height[0] = 24.0f;
    this->state.unknown_11C84->slots[i].height[1] = 3.2f;
    this->state.unknown_11C84->slots[i].height[2] = 0.0f;
  }
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C5B940
// FUNCTION: fn_20223
void CRagEffect::Magnum2(const char* name) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt == 0) {
    this->state.count = 0;
  } else if (this->state.stateCnt == 20) {
    PlayEffectWave(*this, "permeter_attack.wav");
  } else if (this->state.stateCnt == 30) {
    PlayEffectWave(*this, "effect\\EF_MagnumBreak.wav");
  }

  const int frame = this->state.stateCnt;
  const unsigned int elapsed = static_cast<unsigned int>(frame - 25);
  if (elapsed < 31) {
    if (frame % 3 == 0 && ++this->state.count < 11) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 73, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->duration = this->state.duration;
      this->state.unknown_11C84->totalTexture = 1;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
      this->state.unknown_11C84->size = 5.0f;
      for (int i = 0; i < 4; ++i) {
        this->state.unknown_11C84->slots[i].life = 0;
      }
      const float maxHeight = 20.0f - static_cast<float>(elapsed) * 0.7f;
      const float distance = static_cast<float>(elapsed) * 0.2f + 5.0f;
      for (int i = 0; i < 2; ++i) {
        this->state.unknown_11C84->slots[i].life = 1;
        this->state.unknown_11C84->slots[i].process = 0;
        this->state.unknown_11C84->slots[i].fullDisplayAngle = 360;
        this->state.unknown_11C84->slots[i].maxHeight = maxHeight;
        this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(elapsed * 6 + i * 9);
        this->state.unknown_11C84->slots[i].alphaB = 0;
        this->state.unknown_11C84->slots[i].distance = distance;
        this->state.unknown_11C84->slots[i].riseAngle = static_cast<std::int16_t>(((35 - i * 4) - static_cast<std::int16_t>(frame - 25)) * 2);
        this->state.unknown_11C84->slots[i].alphaT = static_cast<std::int16_t>(i);
      }
    }
  }
}

// OFFSET: 0x00C69350
// FUNCTION: fn_20288
void CRagEffect::TexturedCrossLevel(const char* name, char level) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 31, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->accel = 99.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[index].maxHeight = 25.0f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(index) + 1.0f;
    this->state.unknown_11C84->slots[index].riseAngle = 80;
    this->state.unknown_11C84->slots[index].alphaT = level;
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
  }
}

// OFFSET: 0x00C51AA0
// FUNCTION: fn_20190
void CRagEffect::TexturedCrossFade(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& origin = this->state.parent->render;
  this->render.position.x = origin.position.x;
  this->render.position.y = origin.position.y;
  this->render.position.z = origin.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 113, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 315;
    this->state.unknown_11C84->slots[index].maxHeight = 15.0f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = 20.0f - static_cast<float>(index) * 0.5f;
    this->state.unknown_11C84->slots[index].riseAngle = 50;
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
  }
}

// OFFSET: 0x00C5D540
// FUNCTION: fn_20234
void CRagEffect::TexturedRain(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 87, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;

  if (kind == 0) {
    this->state.unknown_11C84->slots[0].distance = 12.0f;
  } else if (kind == 4) {
    this->state.unknown_11C84->slots[0].distance = 14.0f;
  } else {
    this->state.unknown_11C84->slots[0].distance = 8.0f;
  }

  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].vecBPre.x = 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = 0.0f;

  if (kind == 0) {
    this->state.unknown_11C84->slots[0].vecBPre.y = -115.0f;
  } else if (kind == 4) {
    this->state.unknown_11C84->slots[0].vecBPre.y = -115.0f - static_cast<float>(rand() % 56);
  } else {
    this->state.unknown_11C84->slots[0].vecBPre.y = -80.0f;
  }

  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].height[1] = 3.0f;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 7;

  if (kind == 1) {
    this->state.unknown_11C84->slots[0].flag1[2] = 9;
    this->state.unknown_11C84->slots[0].flag1[3] = 1;
  } else if (kind == 2) {
    this->state.unknown_11C84->slots[0].flag1[2] = 9;
    this->state.unknown_11C84->slots[0].flag1[3] = 3;
  } else if (kind == 3) {
    this->state.unknown_11C84->slots[0].flag1[2] = 9;
    this->state.unknown_11C84->slots[0].flag1[3] = 4;
  }

  this->state.unknown_11C84->slots[0].flag1[4] = kind;
  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C65540
// FUNCTION: fn_20269
void CRagEffect::SonicWave(char sonic) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 86, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = sonic != 1 ? 1 : 4;
  AllocateTextures(*this->state.unknown_11C84);

  if (sonic == 1) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\sonic_wave1.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\sonic_wave2.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\sonic_wave3.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\sonic_wave4.tga", 0, false, true, false);
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\purpleslash.tga", 0, false, true, false);
  }

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].process = index - 28;
    this->state.unknown_11C84->slots[index].distance = 20.0f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.y = static_cast<float>(std::max(rand() % 5, 0)) + (this->render.position.y - 10.0f);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[1] = 0.8f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = sonic;
  }
}

// OFFSET: 0x00C5D0B0
// FUNCTION: fn_20232
void CRagEffect::ThunderSparks() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_center.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = std::max(rand() % 10, 0) - 10;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 16, 0)) * 0.03f + 0.1f;
    this->state.unknown_11C84->slots[index].height[8] = 0.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].vecBNow.x = (static_cast<float>(std::max(rand() % 9, 0)) + this->render.position.x) - 4.0f;
    this->state.unknown_11C84->slots[index].vecBNow.z = (static_cast<float>(std::max(rand() % 9, 0)) + this->render.position.z) - 4.0f;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 10, 0)) * 0.01f;
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 40, 0)) * 0.01f + 0.2f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 0;
    this->state.unknown_11C84->slots[index].flag1[2] = 10;
    this->state.unknown_11C84->slots[index].height[7] = 0.0f;
  }
}

// OFFSET: 0x00C69620
// FUNCTION: fn_20289
void CRagEffect::TexturedCrossKind(const char* name, char level) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 31, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  if (level == 3) {
    this->state.unknown_11C84->size = 17.0f;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;

    if (level == 2) {
      this->state.unknown_11C84->slots[index].maxHeight = 15.0f;
      this->state.unknown_11C84->slots[index].riseAngle = 70;
    } else {
      this->state.unknown_11C84->slots[index].maxHeight = 25.0f;
      this->state.unknown_11C84->slots[index].riseAngle = 80;
    }

    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(index) + 1.0f;
    this->state.unknown_11C84->slots[index].alphaT = level;
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
  }
}

// OFFSET: 0x00C454C0
// FUNCTION: fn_20149
void CRagEffect::TexturedGlow(const char* name, char size) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 50, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = static_cast<float>(size);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[0].maxHeight = static_cast<float>(std::max(rand() % 10, 0)) + 25.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = static_cast<std::int16_t>(50 - std::max(rand() % 10, 0));
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].alphaT = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int index = 0; index < 21; ++index) {
    this->state.unknown_11C84->slots[0].height[index] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[index] = 0;
  }
}

// OFFSET: 0x00C5E240
// FUNCTION: fn_20238
void CRagEffect::TexturedScatterArc(const char* name, std::int16_t angle, std::int16_t phase) {
  if (static_cast<char>(phase) == 0) {
    phase = 35;
  } else if (static_cast<char>(phase) == 1) {
    phase = 0;
  }

  if (this->state.stateCnt != phase) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 94, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.2f + 2.5f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    std::int16_t direction = static_cast<std::int16_t>(std::max(rand() % 90, 0) + angle);

    if (direction >= 360) {
      direction -= 360;
    }

    const float cosine = g_cosTable[direction];
    const float sine = g_sinTable[direction];
    this->state.unknown_11C84->slots[index].process = -std::max(rand() % 11, 0);
    this->state.unknown_11C84->slots[index].riseAngle = direction;
    this->state.unknown_11C84->slots[index].maxHeight = 0.03f;
    this->state.unknown_11C84->slots[index].vecBPre.x = cosine * 4.0f;
    this->state.unknown_11C84->slots[index].vecBPre.z = sine * 4.0f;
    this->state.unknown_11C84->slots[index].vecBPre.y = 0.0f;
    this->state.unknown_11C84->slots[index].vecBNow.y = -4.0f;
    this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[3] = static_cast<float>(std::max(rand() % 360, 0));
  }
}

// OFFSET: 0x00C6F190
// FUNCTION: fn_20307
void CRagEffect::LineStreak(std::int16_t kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 56, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0x201;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\line3y.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;

    if (kind % 10 == 0) {
      this->state.unknown_11C84->slots[index].height[0] = -15.0f - static_cast<float>(std::max(rand() % 11, 0));
    } else if (kind % 10 == 1) {
      this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 11, 0)) + 15.0f;
    }

    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 200, 0));
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 4, 0)) * 0.1f + 0.3f;
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 20.0f;
    this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(std::max(rand() % 3, 0) + 0);
    this->state.unknown_11C84->slots[index].height[3] = static_cast<float>(std::max(rand() % 21, 0)) * 0.1f + 1.5f;
    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].riseAngle
    );
    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C6EAF0
// FUNCTION: fn_20305
void CRagEffect::LineStreakKind(std::int16_t kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 56, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(kind < 10 ? "effect\\line3.tga" : "effect\\line3y.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;

    if (kind % 10 == 0) {
      this->state.unknown_11C84->slots[index].height[0] = -15.0f - static_cast<float>(std::max(rand() % 11, 0));
    } else if (kind % 10 == 1) {
      this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 11, 0)) + 15.0f;
    }

    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 200, 0));
    if (kind < 10) {
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 4, 0)) * 0.1f + 0.3f;
      this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 20.0f;
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(std::max(rand() % 3, 0) + 30);
      this->state.unknown_11C84->slots[index].height[3] = static_cast<float>(std::max(rand() % 21, 0)) * 0.1f + 1.5f;
    }
    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].riseAngle
    );
    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C64FD0
// FUNCTION: fn_20266
void CRagEffect::TexturedShieldThrow(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\cru_shield boomerang.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 42, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  const float deltaX = this->state.deltaPos[0] - this->render.position.x;
  const float deltaZ = this->state.deltaPos[2] - this->render.position.z;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  this->state.unknown_11C84->slots[0].distance = 7.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 255;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 8.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].height[1] = 2.0f;
  this->state.unknown_11C84->slots[0].height[2] =
    (this->state.unknown_11C84->slots[0].height[1] / this->state.unknown_11C84->slots[0].maxHeight) * (this->state.deltaPos[1] - this->render.position.y);
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[1] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 0;
  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C5B580
// FUNCTION: fn_20221
void CRagEffect::TexturedCrossStaged(const char* name, char level) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 41, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (level < 2) {
    this->state.unknown_11C84->size = 4.0f;
  } else if (level == 2) {
    this->state.unknown_11C84->size = 7.0f;
  } else if (level == 3) {
    this->state.unknown_11C84->size = 6.0f;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = index * 30;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[index].maxHeight = 120.0f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    this->state.unknown_11C84->slots[index].alphaB = 50;
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

    if (level == 0 || level > 1) {
      this->state.unknown_11C84->slots[index].alphaB = 50;
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(index) * 0.5f + 2.0f;
    } else {
      this->state.unknown_11C84->slots[index].alphaB = 70;
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(index) * 0.5f + 11.0f;
    }

    this->state.unknown_11C84->slots[index].alphaT = 0;
    this->state.unknown_11C84->slots[index].riseAngle = 89;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
  }
}

// OFFSET: 0x00C4E3F0
// FUNCTION: fn_20177
void CRagEffect::ElectricArc(char kind, std::int16_t) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 102, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\elec1.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 0.1f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].riseAngle
    );
    this->state.unknown_11C84->slots[index].distance = 0.0f;
    this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(index < 2 ? index * 180 : index * 180 - 360);
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 6, 0)) * 0.01f + 0.2f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecTNow.y = 20.0f;

    for (int height = 1; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
    }

    this->state.unknown_11C84->slots[index].alphaT = phase;
  }
}

// OFFSET: 0x00C63A10
// FUNCTION: fn_20257
void CRagEffect::BlueRingStack() {
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 121, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  this->state.unknown_11C84->deltaPos2 = {this->state.deltaPos2[0], this->state.deltaPos2[1], this->state.deltaPos2[2]};

  for (int index = 0; index < 4; ++index) {
    if (index == 3) {
      this->state.unknown_11C84->slots[3].life = 0;
      return;
    }

    const float step = static_cast<float>(index);
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 315;
    this->state.unknown_11C84->slots[index].maxHeight = 13.0f - (step + step);
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = step * 0.2f + (this->state.deltaPos2[1] == 0.0f ? 3.4f : 2.4f);
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((11 - index) * 5);
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
    this->state.unknown_11C84->slots[index].alphaT = 99;
  }
}

// OFFSET: 0x00C58F80
// FUNCTION: fn_20210
void CRagEffect::WhiteSpray(char kind) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 43, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(kind == 4 ? "effect\\white03.bmp" : "effect\\white01.bmp", 0, false, true, false);
  this->render.position.y -= 9.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->state.deltaPos[0];
    this->state.unknown_11C84->slots[index].vecBNow.y = this->state.deltaPos[1];
    this->state.unknown_11C84->slots[index].vecBNow.z = this->state.deltaPos[2];

    if (kind == 2) {
      this->state.unknown_11C84->slots[index].process = std::max(rand() % 26, 0) * -2;
      this->state.unknown_11C84->slots[index].flag1[0] = 2;
      this->state.unknown_11C84->slots[index].height[1] = 2.5f;
    } else if (kind == 3) {
      this->state.unknown_11C84->slots[index].process = -70 - std::max(rand() % 30, 0);
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(rand() % 3 + 3);
      this->state.unknown_11C84->slots[index].height[1] = 4.0f;
    } else if (kind == 4) {
      this->state.unknown_11C84->slots[index].process = -30 - std::max(rand() % 20, 0);
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(rand() % 3 + 3);
      this->state.unknown_11C84->slots[index].height[1] = 3.0f;
    } else {
      this->state.unknown_11C84->slots[index].process = -70 - std::max(rand() % 30, 0);
      this->state.unknown_11C84->slots[index].flag1[0] = 0;
      this->state.unknown_11C84->slots[index].height[1] = 3.5f;
    }

    this->state.unknown_11C84->slots[index].distance = 12.0f;
    this->state.unknown_11C84->slots[index].height[0] = 1.5f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C5D810
// FUNCTION: fn_20235
void CRagEffect::TexturedComet(const char* name, char kind) {
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  const char* second;

  if (kind == 1) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_ball_0002.bmp", 0, false, true, false);
    second = "effect\\thunder_ball_0003.bmp";
  } else {
    if (kind == 2) {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\meteo_comet_b.bmp", 0, false, true, false);
      second = "effect\\meteo_comet_c.bmp";
    } else {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pok1.tga", 0, false, true, false);
      second = "effect\\pok3.tga";
    }
  }

  this->state.unknown_11C84->textures[1] = g_texMgr.Load(second, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  RenderObjectState& primRender = Owner(this->state.unknown_11C84).render;
  primRender.position.x = this->render.position.x;
  primRender.position.y = this->render.position.y;
  primRender.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 10, 0)) * 0.08f + 1.2f;
    this->state.unknown_11C84->slots[index].height[8] = 0.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].vecBNow.x = (static_cast<float>(std::max(rand() % 9, 0)) + this->state.deltaPos[0]) - 4.0f;
    this->state.unknown_11C84->slots[index].vecBNow.z = (static_cast<float>(std::max(rand() % 9, 0)) + this->state.deltaPos[2]) - 4.0f;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->state.deltaPos[1] - 6.0f;
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 20, 0)) * 0.01f + 0.1f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 1;

    if (kind == 0) {
      this->state.unknown_11C84->slots[index].flag1[2] = 11;
    } else if (kind == 1) {
      this->state.unknown_11C84->slots[index].flag1[2] = 1;
    } else if (kind == 2) {
      this->state.unknown_11C84->slots[index].flag1[2] = 7;
    }

    this->state.unknown_11C84->slots[index].height[7] = 0.0f;
  }
}

// OFFSET: 0x00C5EC40
// FUNCTION: fn_20240
void CRagEffect::PokBurst() {
  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  this->render.position.x = player->render.position.x;
  this->render.position.y = player->render.position.y;
  this->render.position.z = player->render.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 52, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 3;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pok1.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\pok3.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\pok2.tga", 0, false, true, false);
  this->render.position.x -= 15.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = -300 - std::max(rand() % 50, 0);
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBNow = {this->render.position.x, this->render.position.y, this->render.position.z};
    this->state.unknown_11C84->slots[index].vecTPre = {this->render.position.x, this->render.position.y, this->render.position.z};
    this->state.unknown_11C84->slots[index].vecTNow = {this->render.position.x, this->render.position.y, this->render.position.z};
    this->state.unknown_11C84->slots[index].vecBPre = {this->render.position.x, this->render.position.y, this->render.position.z};
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 2.0f;
    this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[index].flag1[1] = 0;
    this->state.unknown_11C84->slots[index].flag1[3] = 0;
    this->state.unknown_11C84->slots[index].flag1[2] = static_cast<char>(std::max(rand() % 4, 0) * 20);
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 100, 0)) * 0.004f + 0.4f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  }
}

// OFFSET: 0x00C5DB70
// FUNCTION: fn_20236
void CRagEffect::TexturedTumble(const char* name, char) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 30) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 115, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].maxHeight = 1.0f;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 2.5f;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].vecBPre = {0.0f, 0.0f, 0.0f};
    this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 180, 0));
    this->state.unknown_11C84->slots[index].height[12] = 230.0f;
    this->state.unknown_11C84->slots[index].vecBNow = {0.0f, 0.0f, 0.0f};
    this->state.unknown_11C84->slots[index].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[6] = static_cast<float>(std::max(rand() % 180, 0));
    this->state.unknown_11C84->slots[index].height[13] = 230.0f;
    this->state.unknown_11C84->slots[index].vecTPre = {0.0f, 0.0f, 0.0f};
    this->state.unknown_11C84->slots[index].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[10] = static_cast<float>(std::max(rand() % 180, 0));
    this->state.unknown_11C84->slots[index].height[14] = 230.0f;
    this->state.unknown_11C84->slots[index].height[15] = static_cast<float>(std::max(rand() % 3, 0) + 1);
  }
}

// OFFSET: 0x00C6EE30
// FUNCTION: fn_20306
void CRagEffect::LineStreakTextured(std::int16_t kind, const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 56, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0x201;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  const int mode = kind % 10;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 100.0f;

    if (mode == 0) {
      this->state.unknown_11C84->slots[index].height[0] = -15.0f - static_cast<float>(std::max(rand() % 11, 0));
    } else if (mode == 1) {
      this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 11, 0)) + 15.0f;
    }

    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 200, 0));
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 0.5f;
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 40.0f;
    this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(std::max(rand() % 3, 0) + 10);
    this->state.unknown_11C84->slots[index].height[3] = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 1.0f;

    if (static_cast<unsigned>(kind - 20) < 10) {
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(std::max(rand() % 3, 0) + 33);
    } else if (static_cast<unsigned>(kind - 30) < 10) {
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(std::max(rand() % 3, 0) + 36);
    }

    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].riseAngle
    );
    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C6FC10
// FUNCTION: fn_20310
void CRagEffect::ThunderFreeze(int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  int count = static_cast<int>(this->state.deltaPos[0]);

  if (count >= 6) {
    count = 5;
  } else if (count < 1) {
    return;
  }

  int phase = 0;

  do {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 145, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[5] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[6] = static_cast<float>(phase) / this->state.deltaPos[0];
    this->state.unknown_11C84->slots[0].height[7] = -15.0f;
    this->state.unknown_11C84->slots[0].distance = 1.0f;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[0] = 1;
    this->state.unknown_11C84->slots[0].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->totalTexture = 2;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_center.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\freeze_a.bmp", 0, false, true, false);

    for (int index = 0; index < 4; ++index) {
      this->state.unknown_11C84->slots[index].process = index * -20;
      this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].maxHeight = 3.0f;
      this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].riseAngle = 200;
    }

    phase += 360;
  } while (--count != 0);
}

// OFFSET: 0x00C4A220
// FUNCTION: fn_20168
void CRagEffect::WeddingBurst(const char* name, char silent) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  if (silent == 0) {
    PlayEffectWave(*this, "effect\\wedding.wav");
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 78, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 31, 0)) * 0.01f + 0.3f;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45);
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBNow.x = static_cast<float>(std::max(rand() % 101, 0) - 50);
    this->state.unknown_11C84->slots[index].vecBNow.y = static_cast<float>(-110 - std::max(rand() % 50, 0));
    this->state.unknown_11C84->slots[index].vecBNow.z = static_cast<float>(std::max(rand() % 101, 0) - 50);
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 26, 0)) * 0.01f + 0.25f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
  }
}

// OFFSET: 0x00C5C6B0
// FUNCTION: fn_20228
void CRagEffect::TexturedOrbitDust(const char* name, char rise, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 27, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  const float lift = -static_cast<float>(rise * 2);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].maxHeight = 0.15f;
    this->state.unknown_11C84->slots[index].distance = 1.0f;
    this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBPre = {0.0f, lift, 0.0f};
    this->state.unknown_11C84->slots[index].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBNow = {0.0f, lift, 0.0f};
    this->state.unknown_11C84->slots[index].height[5] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecTPre = {0.0f, lift, 0.0f};
    this->state.unknown_11C84->slots[index].height[6] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecTNow = {0.0f, lift, 0.0f};
    this->state.unknown_11C84->slots[index].height[7] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].flag1[0] = 1;
    this->state.unknown_11C84->slots[index].flag1[1] = kind == 2 ? 2 : 0;
  }
}

// OFFSET: 0x00C74BD0
// FUNCTION: fn_20335
void CRagEffect::TexturedTwinSpin(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 161, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (kind == 1) {
    this->state.unknown_11C84->size = 15.0f;
  } else if (kind == 2) {
    this->state.unknown_11C84->size = 17.0f;
  } else if (kind == 3) {
    this->state.unknown_11C84->size = 18.0f;
  } else {
    this->state.unknown_11C84->size = 4.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].distance = kind == 3 ? 24.0f : 15.0f;
  this->state.unknown_11C84->slots[1].distance = this->state.unknown_11C84->slots[0].distance;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[0].riseAngle = 0;
  this->state.unknown_11C84->slots[1].riseAngle = 180;
  this->state.unknown_11C84->slots[0].alphaB = 200;
  this->state.unknown_11C84->slots[1].alphaB = 200;
  this->state.unknown_11C84->slots[0].height[0] = -1.0f;
  this->state.unknown_11C84->slots[1].height[0] = -1.0f;
}

// OFFSET: 0x00C74DF0
// FUNCTION: fn_20336
void CRagEffect::TexturedScatterSphere(const char* name, char flag, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 159, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].height[4] = 0.0f;
    this->state.unknown_11C84->slots[index].process = -rand() % 101;
    this->state.unknown_11C84->slots[index].distance = 4.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
    this->state.unknown_11C84->slots[index].height[1] = 0.5f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 0;
    this->state.unknown_11C84->slots[index].flag1[2] = flag;
    EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[index];
    const float radius = g_cosTable[slot.riseAngle] * 70.0f;
    slot.vecBNow.z = g_sinTable[slot.riseAngle] * 70.0f;
    slot.vecBNow.x = radius * g_cosTable[slot.rotStart];
    slot.vecBNow.y = radius * g_sinTable[slot.rotStart] + slot.height[0];
  }
}

// OFFSET: 0x00C77B80
// FUNCTION: fn_20347
void CRagEffect::DigitalSpace(int effectId) {
  if (this->state.parent) {
    const RenderObjectState& parent = this->state.parent->render;
    this->render.position.x = parent.position.x;
    this->render.position.y = parent.position.y;
    this->render.position.z = parent.position.z;
  }

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 168, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;

    if (effectId == 1240) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\digital_space\\digital_space.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\digital_space\\digital_space.act");
      Owner(this->state.unknown_11C84).render.previousAnimation = 0;
      this->state.unknown_11C84->repeatAnim = 1;
      this->state.unknown_11C84->animSpeed = 8;
      this->state.unknown_11C84->renderFlag = 0x209;
      this->state.unknown_11C84->pattern |= 0x200;
    }
  }

  if (this->state.parent && this->state.unknown_11C84) {
    this->state.unknown_11C84->unknown_FA10 = this->state.parent->render.unknown_0128;
    this->state.unknown_11C84->unknown_FA14 = this->state.parent->render.unknown_012C;
  }

  if (this->state.parent && this->state.unknown_11C84) {
    this->state.unknown_11C84->alpha = static_cast<float>(this->state.parent->render.unknown_0130[0] ? 70 : 255);
  }

  if (this->state.stateCnt == this->state.duration - 1) {
    this->state.stateCnt = 0;
    this->state.unknown_11C84->stateCnt = 0;
  }
}

// OFFSET: 0x00C75AC0
// FUNCTION: fn_20339
void CRagEffect::TexturedFloatingDust(const char* name, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 1000 || this->state.stateCnt % 10 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 157, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 51, 0)) * 0.01f + 2.5f;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 31, 0)) * 0.1f + 7.0f;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(rand() % 360);
    this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(rand() % 33);
    this->state.unknown_11C84->slots[index].vecBNow.y = static_cast<float>(rand() % 21 - 80);
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(rand() % 3);
  }
}

// OFFSET: 0x00C77D00
// FUNCTION: fn_20348
void CRagEffect::Mavka(char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 10) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 152, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->size = 0.3f;
    this->state.unknown_11C84->slots[index].vecBPre.x = 0.0f;
    this->state.unknown_11C84->slots[index].vecBPre.y = 0.0f;
    this->state.unknown_11C84->slots[index].vecBNow.x = 0.0f;
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;
    this->state.unknown_11C84->slots[index].vecTPre.x = 0.0f;
    this->state.unknown_11C84->slots[index].vecTPre.y = 0.0f;
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].riseAngle = 0;
    this->state.unknown_11C84->slots[index].distance = 2.0f;

    for (int height = 0; height < 6; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = static_cast<float>(rand() % 360);
    }

    for (int height = 6; height < 12; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = static_cast<float>(rand() % 360);
    }

    this->state.unknown_11C84->slots[index].flag1[0] = flag;
  }

  this->state.unknown_11C84->alpha = 0.0f;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\mavka.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\mavka.act");
  this->state.unknown_11C84->animSpeed = 8;
  this->state.unknown_11C84->renderFlag |= 0x200;
  Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C77600
// FUNCTION: fn_20345
void CRagEffect::TexturedFanSweep(const char* name, char wide) {
  if (this->state.stateCnt != 0 && this->state.stateCnt != 20 && this->state.stateCnt != 40) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 146, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = index * -8;
    this->state.unknown_11C84->slots[index].maxHeight = wide ? 20.0f : 30.0f;
    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].rotStart
    );
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 315;
    this->state.unknown_11C84->slots[index].rotStart += 195;

    if (this->state.unknown_11C84->slots[index].rotStart >= 360) {
      this->state.unknown_11C84->slots[index].rotStart -= 360;
    }

    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = (wide ? 9.0f : 6.0f) - static_cast<float>(index) * 0.1f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((14 - index) * 5);
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.z = 0.0f;
    this->state.unknown_11C84->slots[index].vecTPre.x = 0.0f;
  }
}

// OFFSET: 0x00C740E0
// FUNCTION: fn_20331
void CRagEffect::BubbleFanSweep(const char*, char rising) {
  char name[64];

  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 146, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 13;
  AllocateTextures(*this->state.unknown_11C84);

  for (int number = 2, index = 0; number < 28; number += 2, ++index) {
    std::sprintf(name, "effect\\W_bubble%02d.tga", number);
    this->state.unknown_11C84->textures[index] = g_texMgr.Load(name, 0, false, true, false);
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = index * -8;
    this->state.unknown_11C84->slots[index].maxHeight = 30.0f;
    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].rotStart
    );
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 315;
    this->state.unknown_11C84->slots[index].rotStart += 195;

    if (this->state.unknown_11C84->slots[index].rotStart >= 360) {
      this->state.unknown_11C84->slots[index].rotStart -= 360;
    }

    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].distance = 3.0f - static_cast<float>(index) * 0.1f;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((14 - index) * 5);
    this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.z = 0.0f;
    this->state.unknown_11C84->slots[index].vecTPre.x = rising == 1 ? 7.0f : 0.0f;
  }
}

// OFFSET: 0x00C76820
// FUNCTION: fn_20342
void CRagEffect::TexturedDoubleSpiral(const char* name, int, int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 2) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 166, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 6.0f;
    this->state.unknown_11C84->slots[index].alphaB = 205;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((this->state.stateCnt * 4 + index) * 45);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
    this->state.unknown_11C84->slots[index].height[9] = 0.0f;
    this->state.unknown_11C84->slots[index].height[2] = 0.01f;
    this->state.unknown_11C84->slots[index].height[3] = 0.0f;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 166, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = 10.5f;
    this->state.unknown_11C84->slots[index].alphaB = 255;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>((this->state.stateCnt * 4 + index) * 45);
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[0] = -5.0f;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
    this->state.unknown_11C84->slots[index].height[9] = 0.0f;
    this->state.unknown_11C84->slots[index].height[2] = 0.015f;
    this->state.unknown_11C84->slots[index].height[3] = 1.0f;
  }
}

// OFFSET: 0x00C77260
// FUNCTION: fn_20344
void CRagEffect::SnowFall(const char*, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (static_cast<std::uint32_t>(this->state.stateCnt) > 100 || this->state.stateCnt % 8 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\E9_snow03.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\E9_snow02.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  Owner(this->state.unknown_11C84).render.position.x = this->render.position.x;
  Owner(this->state.unknown_11C84).render.position.y = this->render.position.y;
  Owner(this->state.unknown_11C84).render.position.z = this->render.position.z;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 1.5f;
    this->state.unknown_11C84->slots[index].height[8] = 0.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    const int radius = std::max(rand() % 41, 0);
    const std::int16_t angle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.x += static_cast<float>(radius) * g_cosTable[angle];
    this->state.unknown_11C84->slots[index].vecBNow.z += static_cast<float>(radius) * g_sinTable[angle];
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y - 6.0f - static_cast<float>(std::max(rand() % 31, 0));
    this->state.unknown_11C84->slots[index].rotStart = 0;
    this->state.unknown_11C84->slots[index].height[1] = 0.0f;
    this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 20, 0)) * 0.01f + 0.1f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].flag1[1] = 1;

    if (kind == 0) {
      this->state.unknown_11C84->slots[index].flag1[2] = 11;
    } else if (kind == 1) {
      this->state.unknown_11C84->slots[index].flag1[2] = 1;
    } else if (kind == 2) {
      this->state.unknown_11C84->slots[index].flag1[2] = 1;
    } else if (kind == 3) {
      this->state.unknown_11C84->slots[index].flag1[2] = 13;
    }

    this->state.unknown_11C84->slots[index].height[7] = 0.0f;
  }
}

// OFFSET: 0x00C74750
// FUNCTION: fn_20333
void CRagEffect::ElectricFan(const char*, std::int16_t fade) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 135, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\elec1.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 0.1f;

    if (index == 0) {
      this->state.unknown_11C84->slots[0].alphaB = 250;
    } else {
      this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(140 - index * 30);
    }

    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(this->state.parent->render.angle - 90.0f);

    if (this->state.unknown_11C84->slots[index].riseAngle < 0) {
      this->state.unknown_11C84->slots[index].riseAngle += 360;
    }

    this->state.unknown_11C84->slots[index].distance = 20.0f;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 11, 0)) * 0.01f + 0.0f;
    this->state.unknown_11C84->slots[index].vecBNow = this->render.position;
    this->state.unknown_11C84->slots[index].vecBNow.y -= 10.0f;
    this->state.unknown_11C84->slots[index].vecTNow.y = 20.0f;

    for (int step = 20; step > 0; --step) {
      this->state.unknown_11C84->slots[index].height[step] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[step] = 0;
    }

    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].alphaT = fade;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
  }
}

// OFFSET: 0x00C778B0
// FUNCTION: fn_20346
void CRagEffect::TexturedTumbleOrb(const char* name, char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 151, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 2.5f;
  this->state.unknown_11C84->slots[0].height[10] = this->state.deltaPos[1];
  this->state.unknown_11C84->slots[0].height[13] = 0.0f;
  this->state.unknown_11C84->slots[0].height[14] = 0.0f;
  this->state.unknown_11C84->slots[0].height[15] = 0.0f;
  this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].vecTNow.x = 5.0f;
  this->state.unknown_11C84->slots[0].vecBPre.x = -g_sinTable[90] * 30.0f;
  this->state.unknown_11C84->slots[0].vecBPre.y = -(g_sinTable[0] * 20.0f) - 40.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = 0.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = flag;
}

// OFFSET: 0x00C755E0
// FUNCTION: fn_20338
void CRagEffect::TexturedSpread(const char* name, char kind) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 137, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (kind < 2) {
    this->state.unknown_11C84->size = 4.0f;
  } else if (kind == 2) {
    this->state.unknown_11C84->size = 7.0f;
  } else if (kind == 3) {
    this->state.unknown_11C84->size = 6.0f;
  } else if (kind == 10) {
    this->state.unknown_11C84->size = static_cast<float>(rand() % 5 + 4);
  } else if (kind == 11) {
    this->state.unknown_11C84->size = 5.0f;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    float radius;

    if (kind == 11) {
      this->state.unknown_11C84->slots[index].alphaB = 50;
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(index * 20) + 60.0f;
      this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;
      this->state.unknown_11C84->slots[index].process = rand() % 40 + 60;
      radius = static_cast<float>(rand() % 31) * 0.1f + 6.0f;
    } else {
      this->state.unknown_11C84->slots[index].alphaB = 150;
      this->state.unknown_11C84->slots[index].maxHeight = 120.0f;
      this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

      if (kind == 10) {
        this->state.unknown_11C84->slots[index].process = rand() % 100;
        radius = static_cast<float>(rand() % 41);
      } else {
        this->state.unknown_11C84->slots[index].process = rand() % 40 + 60;
        radius = 5.0f;
      }
    }

    const std::int16_t angle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBNow.x = radius * g_cosTable[angle];
    this->state.unknown_11C84->slots[index].vecBNow.z = radius * g_sinTable[angle];
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(rand() % 11) * 0.015f + 0.15f;
    this->state.unknown_11C84->slots[index].alphaT = 0;
    this->state.unknown_11C84->slots[index].riseAngle = 90;

    for (int step = 0; step < 21; ++step) {
      this->state.unknown_11C84->slots[index].height[step] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[step] = 0;
    }

    this->state.unknown_11C84->slots[index].vecTPre.x = this->state.deltaPos[0] - this->render.position.x;
    this->state.unknown_11C84->slots[index].vecTPre.y = this->state.deltaPos[1] - this->render.position.y;
    this->state.unknown_11C84->slots[index].vecTPre.z = this->state.deltaPos[2] - this->render.position.z;
  }
}

// OFFSET: 0x00C75D20
// FUNCTION: fn_20340
void CRagEffect::WhiteRingRipple(char size) {
  if (this->state.stateCnt % 100 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 63, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_white.tga", 0, false, true, false);

  if (size == 1) {
    this->state.unknown_11C84->size = 8.0f;
  } else if (size == 2) {
    this->state.unknown_11C84->size = 6.0f;
  } else {
    this->state.unknown_11C84->size = 4.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 2;
  this->state.unknown_11C84->slots[0].alphaT = 100;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int step = 0; step < 21; ++step) {
    this->state.unknown_11C84->slots[0].height[step] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[step] = 1;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = -10;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[1].rotStart = 25;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 0.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 3;
  this->state.unknown_11C84->slots[1].alphaT = 100;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int step = 0; step < 21; ++step) {
    this->state.unknown_11C84->slots[1].height[step] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[step] = 1;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = -20;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[2].rotStart = 50;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 0.0f;
  this->state.unknown_11C84->slots[2].riseAngle = 4;
  this->state.unknown_11C84->slots[2].alphaT = 100;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int step = 0; step < 21; ++step) {
    this->state.unknown_11C84->slots[2].height[step] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[step] = 1;
  }

  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].alphaT = 4;
}

// OFFSET: 0x00C762A0
// FUNCTION: fn_20341
void CRagEffect::WhiteRingRippleSized(char size) {
  if (this->state.stateCnt % 100 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 63, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_white.tga", 0, false, true, false);

  this->state.unknown_11C84->size = static_cast<float>(size);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 2;
  this->state.unknown_11C84->slots[0].alphaT = 100;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int step = 0; step < 21; ++step) {
    this->state.unknown_11C84->slots[0].height[step] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[step] = 1;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = -10;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[1].rotStart = 25;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 0.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 3;
  this->state.unknown_11C84->slots[1].alphaT = 100;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int step = 0; step < 21; ++step) {
    this->state.unknown_11C84->slots[1].height[step] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[step] = 1;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = -20;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[2].rotStart = 50;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 0.0f;
  this->state.unknown_11C84->slots[2].riseAngle = 4;
  this->state.unknown_11C84->slots[2].alphaT = 100;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int step = 0; step < 21; ++step) {
    this->state.unknown_11C84->slots[2].height[step] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[step] = 1;
  }

  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].alphaT = size == 29 ? 5 : 1;
}

// OFFSET: 0x00C75050
// FUNCTION: fn_20337
void CRagEffect::TexturedSpreadFlat(const char* name, char kind) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 137, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (kind < 2) {
    this->state.unknown_11C84->size = 4.0f;
  } else if (kind == 2) {
    this->state.unknown_11C84->size = 7.0f;
  } else if (kind == 3) {
    this->state.unknown_11C84->size = 6.0f;
  } else if (kind == 10) {
    this->state.unknown_11C84->size = static_cast<float>(rand() % 5 + 4);
  } else if (kind == 11) {
    this->state.unknown_11C84->size = 5.0f;
  } else if (kind == 12) {
    const std::int16_t pick = static_cast<std::int16_t>(rand() % 3);

    if (pick == 0) {
      this->state.unknown_11C84->size = 4.0f;
    } else if (pick == 1) {
      this->state.unknown_11C84->size = 40.0f;
    } else {
      this->state.unknown_11C84->size = 41.0f;
    }
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(index * 90);
    float radius;

    if (kind == 11) {
      this->state.unknown_11C84->slots[index].alphaB = 50;
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(index * 20) + 60.0f;
      this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;
      this->state.unknown_11C84->slots[index].process = rand() % 40 + 60;
      radius = static_cast<float>(rand() % 31) * 0.1f + 6.0f;
    } else if (kind == 12) {
      this->state.unknown_11C84->slots[index].alphaB = 130;
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(index * 30) + 30.0f;
      this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;
      this->state.unknown_11C84->slots[index].process = rand() % 50 + 50;
      radius = static_cast<float>(rand() % 17);
    } else {
      this->state.unknown_11C84->slots[index].alphaB = 150;
      this->state.unknown_11C84->slots[index].maxHeight = 120.0f;
      this->state.unknown_11C84->slots[index].vecBNow.y = 0.0f;

      if (kind == 10) {
        this->state.unknown_11C84->slots[index].process = rand() % 100;
        radius = static_cast<float>(rand() % 41);
      } else {
        this->state.unknown_11C84->slots[index].process = rand() % 40 + 60;
        radius = 5.0f;
      }
    }

    const std::int16_t angle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].vecBNow.x = radius * g_cosTable[angle];
    this->state.unknown_11C84->slots[index].vecBNow.z = radius * g_sinTable[angle];
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(rand() % 11) * 0.015f + 0.15f;
    this->state.unknown_11C84->slots[index].alphaT = 0;
    this->state.unknown_11C84->slots[index].riseAngle = 90;

    for (int step = 0; step < 21; ++step) {
      this->state.unknown_11C84->slots[index].height[step] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[step] = 0;
    }

    this->state.unknown_11C84->slots[index].vecTPre.x = 0.0f;
    this->state.unknown_11C84->slots[index].vecTPre.y = 0.0f;
    this->state.unknown_11C84->slots[index].vecTPre.z = 0.0f;
  }
}

// OFFSET: 0x00C74390
// FUNCTION: fn_20332
void CRagEffect::RisingSparkles(char colorMode) {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if ((this->state.stateCnt & 1) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 50;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  const float distance = static_cast<float>(std::max(rand() % 7, 0) + 3);
  this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, distance);
  this->state.unknown_11C84->deltaPos2.y -= 5.0f;
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(-90));
  this->state.unknown_11C84->accel = -0.015f;
  this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
  this->state.unknown_11C84->heightSize = 0.18f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 200.0f;

  if (colorMode == 2) {
    this->state.unknown_11C84->red = rand() % 256;
    this->state.unknown_11C84->green = this->state.unknown_11C84->red;
    this->state.unknown_11C84->blue = 255;
  } else {
    this->state.unknown_11C84->red = 255;
    this->state.unknown_11C84->green = rand() % 256;
    this->state.unknown_11C84->blue = this->state.unknown_11C84->green;
  }

  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
}

// OFFSET: 0x00C735B0
// FUNCTION: fn_20328
void CRagEffect::TexturedTripleOrbit(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 16.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 74.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 1.3f;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].alphaT = 3080;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[0].height[i] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[i] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 77.0f;
  this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 1.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 90;
  this->state.unknown_11C84->slots[1].alphaT = 3080;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[1].height[i] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[i] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 80.0f;
  this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 0.7f;
  this->state.unknown_11C84->slots[2].riseAngle = 90;
  this->state.unknown_11C84->slots[2].alphaT = 3080;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[2].height[i] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[i] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C73BF0
// FUNCTION: fn_20330
void CRagEffect::TexturedRisingOrbs(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
    if (kind == 0) {
      g_soundMgr->PlayWave(
        "_heal_effect.wav", this->render.position.x - player->render.position.x, -40.0f, this->render.position.z - player->render.position.z, 250, 40,
        1.0f, 0
      );
    } else if (kind == 1) {
      g_soundMgr->PlayWave(
        "effect\\흡기.wav", this->render.position.x - player->render.position.x, -80.0f, this->render.position.z - player->render.position.z, 250, 40, 1.0f,
        0
      );
    }
  }

  if (this->state.stateCnt > 20 || this->state.stateCnt % 4 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  if (kind == 2) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pok1.tga", 0, false, true, false);
    name = "effect\\pok3.tga";
  }

  this->state.unknown_11C84->textures[1] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  Owner(this->state.unknown_11C84).render.position = this->render.position;

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    if (kind == 2) {
      if (i < 2) {
        this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 2.0f;
      } else {
        this->state.unknown_11C84->slots[i].life = 0;
      }

      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 19, 0)) + this->render.position.x - 9.0f;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 19, 0)) + this->render.position.z - 9.0f;
    } else {
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 10, 0)) * 0.1f + 1.5f;
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 7, 0)) + this->render.position.x - 3.0f;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 7, 0)) + this->render.position.z - 3.0f;
    }

    this->state.unknown_11C84->slots[i].height[8] = 0.0f;
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y - 6.0f;
    this->state.unknown_11C84->slots[i].rotStart = 0;
    this->state.unknown_11C84->slots[i].height[1] = 0.0f;
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 20, 0)) * 0.01f + 0.2f;
    this->state.unknown_11C84->slots[i].fullDisplayAngle = 0;
    this->state.unknown_11C84->slots[i].flag1[0] = 0;
    this->state.unknown_11C84->slots[i].flag1[1] = 1;
    if (kind == 0 || kind == 2) {
      this->state.unknown_11C84->slots[i].flag1[2] = 11;
    } else {
      this->state.unknown_11C84->slots[i].flag1[2] = kind == 3 ? 18 : 1;
    }

    this->state.unknown_11C84->slots[i].height[7] = 0.0f;
  }
}

// OFFSET: 0x00C76BE0
// FUNCTION: fn_20343
void CRagEffect::TexturedForkBolt(const char* name, char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 30 || this->state.stateCnt % 10 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 155, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 0;
  }

  const float x = this->render.position.x;
  const float z = this->render.position.z;
  const float y = this->render.position.y - 5.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].maxHeight = static_cast<float>(std::max(rand() % 21, 0)) * 0.01f + 0.8f;
  this->state.unknown_11C84->slots[0].distance = static_cast<float>(std::max(rand() % 21, 0)) * 0.1f + 3.0f;
  this->state.unknown_11C84->slots[0].process = 0;
  Get_Angle(
    this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[0].riseAngle
  );
  this->state.unknown_11C84->slots[0].vecBPre.x = x - this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBPre.y = y - this->render.position.y;
  this->state.unknown_11C84->slots[0].vecBPre.z = z - this->render.position.z;
  this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(this->state.unknown_11C84->slots[0].riseAngle);
  this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(this->state.unknown_11C84->slots[0].riseAngle - 45 + rand() % 91);
  this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(this->state.unknown_11C84->slots[0].riseAngle - 45 + rand() % 91);
  this->state.unknown_11C84->slots[0].height[3] = 180.0f;
  this->state.unknown_11C84->slots[0].height[4] = 180.0f;
  this->state.unknown_11C84->slots[0].height[12] = 0.0f;
  this->state.unknown_11C84->slots[0].vecBNow.x = x - this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.y = y - this->render.position.y;
  this->state.unknown_11C84->slots[0].vecBNow.z = z - this->render.position.z;
  this->state.unknown_11C84->slots[0].height[5] = static_cast<float>(this->state.unknown_11C84->slots[0].riseAngle);
  this->state.unknown_11C84->slots[0].height[6] = static_cast<float>(this->state.unknown_11C84->slots[0].riseAngle - 45 + rand() % 91);
  this->state.unknown_11C84->slots[0].height[7] = static_cast<float>(this->state.unknown_11C84->slots[0].riseAngle - 45 + rand() % 91);
  this->state.unknown_11C84->slots[0].height[8] = 180.0f;
  this->state.unknown_11C84->slots[0].height[9] = 180.0f;
  for (int i = 0; i < 10; ++i) {
    if (this->state.unknown_11C84->slots[0].height[i] < 0.0f) {
      this->state.unknown_11C84->slots[0].height[i] += 360.0f;
    }

    if (this->state.unknown_11C84->slots[0].height[i] >= 360.0f) {
      this->state.unknown_11C84->slots[0].height[i] -= 360.0f;
    }
  }

  this->state.unknown_11C84->slots[0].height[13] = 0.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = flag;
}

// OFFSET: 0x00C77F50
// FUNCTION: fn_20349
void CRagEffect::MonsterSpriteBurst(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  int limit;
  if (kind == 13 || kind == 14) {
    limit = 30;
  } else if (kind < 9) {
    limit = kind >= 2 ? 10 : 50;
  } else {
    limit = 20;
  }

  if (this->state.stateCnt >= limit) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 153, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration - this->state.stateCnt;
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 0;
  }

  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].life = 1;

  std::int16_t angle;
  float radius = 24.0f;
  if (kind == 2) {
    this->state.unknown_11C84->size = 0.6f;
    angle = static_cast<std::int16_t>(this->state.stateCnt * 36);
    radius = 40.0f;
  } else if (kind == 13) {
    angle = static_cast<std::int16_t>(this->state.stateCnt * 12);
    const int ring = this->state.stateCnt % 3;
    if (ring == 0) {
      this->state.unknown_11C84->size = 0.9f;
      radius = 8.0f;
    } else if (ring == 1) {
      this->state.unknown_11C84->size = 1.0f;
      radius = 15.0f;
    } else {
      this->state.unknown_11C84->size = 1.1f;
      radius = 22.0f;
    }
  } else if (kind == 14) {
    angle = static_cast<std::int16_t>(this->state.stateCnt * 12);
    const int ring = this->state.stateCnt % 3;
    if (ring == 0) {
      this->state.unknown_11C84->size = 0.5f;
      radius = 8.0f;
    } else if (ring == 1) {
      this->state.unknown_11C84->size = 0.65f;
      radius = 14.0f;
    } else {
      this->state.unknown_11C84->size = 0.8f;
      radius = 20.0f;
    }
  } else if (kind == 15) {
    angle = static_cast<std::int16_t>(this->state.stateCnt * 18);
    if (this->state.stateCnt % 2 == 0) {
      radius = 4.0f;
      this->state.unknown_11C84->size = static_cast<float>(rand() % 11) * 0.05f + 1.5f;
    } else {
      radius = 10.0f;
      this->state.unknown_11C84->size = static_cast<float>(rand() % 11) * 0.05f + 2.0f;
    }
  } else if (kind == 9) {
    angle = static_cast<std::int16_t>(this->state.stateCnt * 18);
    if (this->state.stateCnt % 2 == 0) {
      this->state.unknown_11C84->size = 0.85f;
      radius = 12.0f;
    } else {
      this->state.unknown_11C84->size = 0.95f;
    }
  } else if (kind > 9) {
    angle = static_cast<std::int16_t>(this->state.stateCnt * 18);
    if (this->state.stateCnt % 2 == 0) {
      radius = 6.0f;
      this->state.unknown_11C84->size = static_cast<float>(rand() % 11) * 0.03f + 0.4f;
    } else {
      radius = 12.0f;
      this->state.unknown_11C84->size = static_cast<float>(rand() % 11) * 0.03f + 0.5f;
    }
  } else if (kind > 2) {
    this->state.unknown_11C84->size = 1.0f;
    angle = static_cast<std::int16_t>(this->state.stateCnt * 36);
    radius = 40.0f;
  } else {
    this->state.unknown_11C84->size = 0.7f;
    angle = static_cast<std::int16_t>(rand() % 360);
    radius = static_cast<float>(rand() % 45 + 5);
  }

  Owner(this->state.unknown_11C84).render.position = this->state.unknown_11C84->master->render.position;
  Owner(this->state.unknown_11C84).render.position.x = radius * g_sinTable[angle] + Owner(this->state.unknown_11C84).render.position.x;
  Owner(this->state.unknown_11C84).render.position.z = radius * g_cosTable[angle] + Owner(this->state.unknown_11C84).render.position.z;
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  Owner(this->state.unknown_11C84).render.position.y =
    world->terrain->Grid().Height(Owner(this->state.unknown_11C84).render.position.x, Owner(this->state.unknown_11C84).render.position.z);

  if (kind == 5 || kind == 10) {
    this->state.unknown_11C84->slots[0].flag1[0] = 4;
  } else if (kind == 11) {
    this->state.unknown_11C84->slots[0].flag1[0] = 6;
  } else if (kind == 12 || kind == 14 || kind == 15) {
    this->state.unknown_11C84->slots[0].flag1[0] = 8;
  } else {
    this->state.unknown_11C84->slots[0].flag1[0] = kind;
  }

  this->state.unknown_11C84->slots[0].flag1[2] = kind == 15 ? 1 : 0;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->slots[1].flag1[1] = static_cast<char>(rand() % 3);

  if (kind == 2) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\워그.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\워그.act");
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind == 3) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\violy.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\violy.act");
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind == 4) {
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    const std::int16_t pose = static_cast<std::int16_t>(rand() % 3);
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\succubus.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\succubus.act");
    if (pose == 0) {
      Owner(this->state.unknown_11C84).SetAction(rand() % 8 + 56, 16, 1);
    } else if (pose == 1) {
      Owner(this->state.unknown_11C84).SetAction(rand() % 8 + 48, 16, 1);
    } else {
      Owner(this->state.unknown_11C84).SetAction(rand() % 8 + 40, 16, 1);
    }
  } else if (kind == 5) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\mineral.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\mineral.act");
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind == 6) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\ktullanux.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\ktullanux.act");
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind == 7) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\garm.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\garm.act");
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind == 8) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\miyabi_ningyo.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\miyabi_ningyo.act");
    this->state.unknown_11C84->animSpeed = rand() % 3 + 6;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8 + 48, 16, 1);
  } else if (kind == 9) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\garden_keeper.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\garden_keeper.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8 + 16, 16, 1);
  } else if (kind == 13) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\ginseng_bullet.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\ginseng_bullet.act");
    this->state.unknown_11C84->animSpeed = 3;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind == 14) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\nepenthes2_bullet.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\nepenthes2_bullet.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  } else if (kind == 15) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\imp.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\imp.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8, 16, 1);
  } else if (kind > 9) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\ifrit.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\ifrit.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(rand() % 8 + 32, 16, 1);
  } else {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "몬스터\\geographer.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "몬스터\\geographer.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  }

  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C78A40
// FUNCTION: fn_20350
bool CRagEffect::AgilityChange(char increase, const std::string& textureName) {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    const auto* player = reinterpret_cast<const CRenderObject*>(world->player);
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, world->objects)->state;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(textureName.c_str(), 0, false, true, false);
    CTexture* texture = this->state.unknown_11C84->textures[0];
    if (texture == nullptr) {
      return false;
    }

    this->state.unknown_11C84->widthSize = static_cast<float>(texture->Width()) * 0.625f;
    this->state.unknown_11C84->heightSize = static_cast<float>(this->state.unknown_11C84->textures[0]->Height()) * 0.625f;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->slots[0].life = 99;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 200.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;

    const char* sound;
    if (increase == 1) {
      this->state.unknown_11C84->speed = 1.5f;
      EffectPrimitiveState& rising = *this->state.unknown_11C84;
      rising.accel = -(rising.speed / rising.duration) * 1.2f;
      sound = "effect\\EF_IncAgility.wav";
    } else {
      this->state.unknown_11C84->deltaPos2.y -= 80.0f;
      this->state.unknown_11C84->longitude = 180.0f;
      this->state.unknown_11C84->speed = 1.0f;
      EffectPrimitiveState& falling = *this->state.unknown_11C84;
      falling.accel = -(falling.speed / falling.duration);
      sound = "effect\\EF_DecAgility.wav";
    }

    g_soundMgr->PlayWave(
      sound, this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
  }

  if ((this->state.stateCnt & 1) != 0) {
    return true;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
  if (this->state.unknown_11C84->textures[0] == nullptr) {
    return false;
  }

  this->state.unknown_11C84->duration = 50;
  this->state.unknown_11C84->slots[0].life = 99;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 200.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
  this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
  this->state.unknown_11C84->heightSize = 0.18f;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  EffectPrimitiveState& moving = *this->state.unknown_11C84;
  moving.deltaPos2 = RotatedForward(moving.transform, static_cast<float>(std::max(rand() % 7, 0) + 2));

  if (increase == 1) {
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(90));
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 20) / 100.0f;
    this->state.unknown_11C84->red = 0;
    this->state.unknown_11C84->green = 255;
    this->state.unknown_11C84->blue = 255;
    return true;
  }

  this->state.unknown_11C84->deltaPos2.y -= 20.0f;
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(-90));
  this->state.unknown_11C84->accel = 0.015f;
  this->state.unknown_11C84->red = 255;
  this->state.unknown_11C84->green = 0;
  this->state.unknown_11C84->blue = 0;
  return true;
}

// OFFSET: 0x00C79130
// FUNCTION: fn_20351
void CRagEffect::TexturedDescendingRings(const char* name, char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 141, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 0;
  }

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = i * -10;
    this->state.unknown_11C84->slots[i].distance = 20.0f;
    this->state.unknown_11C84->slots[i].flag1[2] = 2;
    float drop;
    if (i == 0) {
      drop = 30.0f;
    } else if (i == 1) {
      drop = 20.0f;
    } else if (i == 2) {
      drop = 10.0f;
    } else {
      drop = 0.0f;
    }

    this->state.unknown_11C84->slots[i].vecBPre.y = this->render.position.y - drop;
    this->state.unknown_11C84->slots[i].fullDisplayAngle = 215;
    this->state.unknown_11C84->slots[i].height[8] = 0.0f;
    this->state.unknown_11C84->slots[i].alphaB = 255;
    this->state.unknown_11C84->slots[i].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[i].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[i].rotStart = 0;
    this->state.unknown_11C84->slots[i].flag1[0] = 0;
    this->state.unknown_11C84->slots[i].flag1[4] = flag;
  }
}

// OFFSET: 0x00C79CD0
// FUNCTION: fn_20354
void CRagEffect::TexturedQuadSpiral(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 31, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (long long i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[i].maxHeight = 25.0f;
    this->state.unknown_11C84->slots[i].riseAngle = 80;
    this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 90);
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].alphaT = 5;
    this->state.unknown_11C84->slots[i].distance = static_cast<float>(i) + 1.0f;
    this->state.unknown_11C84->slots[i].vecBNow.y = 0.0f;
    this->state.unknown_11C84->accel = 99.0f;
    for (long long j = 0; j < 21; ++j) {
      this->state.unknown_11C84->slots[i].height[j] = 0.0f;
      this->state.unknown_11C84->slots[i].flag1[j] = 0;
    }
  }
}

// OFFSET: 0x00C79F10
// FUNCTION: fn_20355
void CRagEffect::TexturedRisingPillar(const char* name, char) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 136, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  Owner(this->state.unknown_11C84).render.position = this->render.position;
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 0;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 1.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].height[1] = 0.0f;
  this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 20, 0)) * 0.01f + 0.1f;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 45;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[1] = 1;
  this->state.unknown_11C84->slots[0].flag1[2] = 1;
  this->state.unknown_11C84->slots[0].height[7] = 0.0f;
}

// OFFSET: 0x00C793A0
// FUNCTION: fn_20352
void CRagEffect::CloudBurst() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt < 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 69, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
    this->state.unknown_11C84->deltaPos.x = this->state.deltaPos[0];
    this->state.unknown_11C84->deltaPos.y = this->state.deltaPos[1];
    this->state.unknown_11C84->deltaPos.z = this->state.deltaPos[2];

    const float targetX = this->state.deltaPos[0];
    const float x = this->render.position.x;
    const float targetZ = this->state.deltaPos[2];
    const float z = this->render.position.z;
    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = -std::max(rand() % 20, 0);
      Get_Angle(targetX - x, targetZ - z, &this->state.unknown_11C84->slots[i].rotStart);
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 100, 0)) * 0.06f - 3.0f + this->render.position.x;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 100, 0)) * 0.06f - 3.0f + this->render.position.z;
      this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 100, 0)) * 0.15f;
      this->state.unknown_11C84->slots[i].vecBNow.x -=
        g_cosTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
      this->state.unknown_11C84->slots[i].vecBNow.z -=
        g_sinTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
      this->state.unknown_11C84->slots[i].vecTNow.x = this->state.unknown_11C84->slots[i].vecBNow.x;
      this->state.unknown_11C84->slots[i].vecTNow.z = this->state.unknown_11C84->slots[i].vecBNow.z;
      this->state.unknown_11C84->slots[i].vecTNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].maxHeight = 0.25f;
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 100, 0)) * 0.04f + 8.0f;
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 100, 0)) * 0.2f + 20.0f;
      this->state.unknown_11C84->slots[i].height[4] = 4.0f;
    }
  }

  if (this->state.stateCnt == 10) {
    PlayEffectWave(*this, "effect\\EF_hit2.wav");
  }

  if (this->state.stateCnt == 20) {
    PlayEffectWave(*this, "effect\\EF_hit4.wav");
  }
}

// OFFSET: 0x00C79820
// FUNCTION: fn_20353
void CRagEffect::CloudSpread() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt < 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 69, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
    this->state.unknown_11C84->deltaPos.x = this->state.deltaPos[0];
    this->state.unknown_11C84->deltaPos.y = this->state.deltaPos[1];
    this->state.unknown_11C84->deltaPos.z = this->state.deltaPos[2];

    const float dx = this->state.deltaPos[0] - this->render.position.x;
    const float dz = this->state.deltaPos[2] - this->render.position.z;
    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = -std::max(rand() % 20, 0);
      Get_Angle(dx, dz, &this->state.unknown_11C84->slots[i].rotStart);
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 100, 0)) * 0.3f - 15.0f + this->render.position.x;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 100, 0)) * 0.3f - 15.0f + this->render.position.z;
      this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].height[1] = 100.0f;
      this->state.unknown_11C84->slots[i].vecBNow.x -=
        g_cosTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
      this->state.unknown_11C84->slots[i].vecBNow.z -=
        g_sinTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
      this->state.unknown_11C84->slots[i].vecTNow.x = this->state.unknown_11C84->slots[i].vecBNow.x;
      this->state.unknown_11C84->slots[i].vecTNow.z = this->state.unknown_11C84->slots[i].vecBNow.z;
      this->state.unknown_11C84->slots[i].vecTNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].maxHeight = 0.25f;
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 100, 0)) * 0.04f + 8.0f;
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 100, 0)) * 0.5f + 100.0f;
      this->state.unknown_11C84->slots[i].height[4] = 5.0f;
    }
  }

  if (this->state.stateCnt == 10) {
    PlayEffectWave(*this, "effect\\EF_hit2.wav");
  }

  if (this->state.stateCnt == 20) {
    PlayEffectWave(*this, "effect\\EF_hit4.wav");
  }
}

// OFFSET: 0x00C7A170
// FUNCTION: fn_20356
void CRagEffect::FrostSplash(int, int) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 150, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);

  CTexture* texture;
  bool water;
  if (std::max(rand() % 10, 0) > 0) {
    texture = g_texMgr.Load("effect\\freeze_a_small.bmp", 0, false, true, false);
    water = false;
  } else {
    texture = g_texMgr.Load("effect\\waterp1.tga", 0, false, true, false);
    water = true;
  }

  this->state.unknown_11C84->textures[0] = texture;
  this->state.unknown_11C84->size = 4.0f;

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(this->state.param[1]);
    this->state.unknown_11C84->slots[i].fullDisplayAngle %= 360;
    const std::int16_t angle = this->state.unknown_11C84->slots[i].fullDisplayAngle;
    if (this->state.param[0] < 10.0f) {
      this->state.param[0] = 10.0f;
    }

    this->state.unknown_11C84->slots[i].riseAngle = static_cast<std::int16_t>(this->state.param[0]);
    const float scale = static_cast<float>(this->state.unknown_11C84->slots[i].riseAngle) / 10.0f;
    const unsigned color = static_cast<unsigned>(this->state.param[2]) % 1000000;
    this->state.unknown_11C84->slots[i].flag1[10] = static_cast<char>(color / 10000);
    this->state.unknown_11C84->slots[i].flag1[11] = static_cast<char>((color - this->state.unknown_11C84->slots[i].flag1[12] * 10000) / 100);
    this->state.unknown_11C84->slots[i].flag1[12] = static_cast<char>(color - static_cast<char>(color / 100) * 100);
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].maxHeight = 0.05f;
    this->state.unknown_11C84->slots[i].distance = scale * (water ? 7.0f : 4.5f);

    this->state.unknown_11C84->slots[i].vecBPre.y = static_cast<float>(std::max(rand() % 30, 0));
    this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecBPre.x = static_cast<float>(std::max(rand() % 45, 0) - 22);
    this->state.unknown_11C84->slots[i].vecBPre.z = static_cast<float>(std::max(rand() % 11, 0) - 5);
    this->state.unknown_11C84->slots[i].vecBPre.x = scale * this->state.unknown_11C84->slots[i].vecBPre.x;
    this->state.unknown_11C84->slots[i].vecBPre.z = scale * this->state.unknown_11C84->slots[i].vecBPre.z;
    this->state.unknown_11C84->slots[i].vecBPre.y = scale * this->state.unknown_11C84->slots[i].vecBPre.y;
    {
      const float x = this->state.unknown_11C84->slots[i].vecBPre.x;
      const float z = this->state.unknown_11C84->slots[i].vecBPre.z;
      this->state.unknown_11C84->slots[i].vecBPre.x = g_cosTable[angle] * x - g_sinTable[angle] * z;
      this->state.unknown_11C84->slots[i].vecBPre.z = g_sinTable[angle] * x + g_cosTable[angle] * z;
    }

    this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 45, 0) - 22);
    this->state.unknown_11C84->slots[i].vecBNow.y = static_cast<float>(std::max(rand() % 30, 0));
    this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 11, 0) - 5);
    this->state.unknown_11C84->slots[i].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[5] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[6] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[7] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecBNow.x = scale * this->state.unknown_11C84->slots[i].vecBNow.x;
    this->state.unknown_11C84->slots[i].vecBNow.z = scale * this->state.unknown_11C84->slots[i].vecBNow.z;
    this->state.unknown_11C84->slots[i].vecBNow.y = scale * this->state.unknown_11C84->slots[i].vecBNow.y;
    {
      const float x = this->state.unknown_11C84->slots[i].vecBNow.x;
      const float z = this->state.unknown_11C84->slots[i].vecBNow.z;
      this->state.unknown_11C84->slots[i].vecBNow.x = g_cosTable[angle] * x - g_sinTable[angle] * z;
      this->state.unknown_11C84->slots[i].vecBNow.z = g_sinTable[angle] * x + g_cosTable[angle] * z;
    }

    this->state.unknown_11C84->slots[i].vecTPre.x = static_cast<float>(std::max(rand() % 45, 0) - 22);
    this->state.unknown_11C84->slots[i].vecTPre.y = static_cast<float>(std::max(rand() % 30, 0));
    this->state.unknown_11C84->slots[i].vecTPre.z = static_cast<float>(std::max(rand() % 11, 0) - 5);
    this->state.unknown_11C84->slots[i].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[9] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[10] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[11] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecTPre.x = scale * this->state.unknown_11C84->slots[i].vecTPre.x;
    this->state.unknown_11C84->slots[i].vecTPre.z = scale * this->state.unknown_11C84->slots[i].vecTPre.z;
    this->state.unknown_11C84->slots[i].vecTPre.y = scale * this->state.unknown_11C84->slots[i].vecTPre.y;
    {
      const float x = this->state.unknown_11C84->slots[i].vecTPre.x;
      const float z = this->state.unknown_11C84->slots[i].vecTPre.z;
      this->state.unknown_11C84->slots[i].vecTPre.x = x * g_cosTable[angle] - z * g_sinTable[angle];
      this->state.unknown_11C84->slots[i].vecTPre.z = x * g_sinTable[angle] + z * g_cosTable[angle];
    }

    this->state.unknown_11C84->slots[i].vecTNow.x = static_cast<float>(std::max(rand() % 45, 0) - 22);
    this->state.unknown_11C84->slots[i].vecTNow.y = static_cast<float>(std::max(rand() % 30, 0));
    this->state.unknown_11C84->slots[i].vecTNow.z = static_cast<float>(std::max(rand() % 11, 0) - 5);
    this->state.unknown_11C84->slots[i].height[12] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[13] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[14] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[15] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecTNow.x = scale * this->state.unknown_11C84->slots[i].vecTNow.x;
    this->state.unknown_11C84->slots[i].vecTNow.z = scale * this->state.unknown_11C84->slots[i].vecTNow.z;
    this->state.unknown_11C84->slots[i].vecTNow.y = scale * this->state.unknown_11C84->slots[i].vecTNow.y;
    {
      const float x = this->state.unknown_11C84->slots[i].vecTNow.x;
      const float z = this->state.unknown_11C84->slots[i].vecTNow.z;
      this->state.unknown_11C84->slots[i].vecTNow.x = x * g_cosTable[angle] - z * g_sinTable[angle];
      this->state.unknown_11C84->slots[i].vecTNow.z = x * g_sinTable[angle] + z * g_cosTable[angle];
    }

    this->state.unknown_11C84->slots[i].flag1[0] = 2;
  }
}

// OFFSET: 0x00C708F0
// FUNCTION: fn_20313
void CRagEffect::AssassinCloakingClear() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\assasin_cloaking.wav");
  }

  this->state.parent->SetColor(50, 0, 0, 0);
  this->render.unknown_00A4Bytes[0] = 0;
}

// OFFSET: 0x00C710C0
// FUNCTION: fn_20318
void CRagEffect::TintedRisingSparkles(char) {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if ((this->state.stateCnt & 1) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 50;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  const float distance = static_cast<float>(std::max(rand() % 7, 0) + 3);
  this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, distance);
  this->state.unknown_11C84->deltaPos2.y -= 20.0f;
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(-90));
  this->state.unknown_11C84->accel = 0.015f;
  this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
  this->state.unknown_11C84->heightSize = 0.18f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 200.0f;
  this->state.unknown_11C84->red = rand() % 3 == 0 ? 255 : 140;
  this->state.unknown_11C84->green = rand() % 3 == 0 ? 255 : 140;
  this->state.unknown_11C84->blue = rand() % 3 == 0 ? 255 : 140;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
}

// OFFSET: 0x00C71C60
// FUNCTION: fn_20321
void CRagEffect::SpriteRing(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt > 1) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 167, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->deltaPos2.y = 0.0f;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->pattern |= 0x200u;
  this->state.unknown_11C84->animSpeed = 2;
  this->state.unknown_11C84->repeatAnim = 0;
  this->state.unknown_11C84->animSpeed = 4;

  const char* sprite;
  const char* action;
  if (kind == 20) {
    action = "이팩트\\m_ef05.act";
    sprite = "이팩트\\m_ef05.spr";
  } else {
    action = "이팩트\\m_ef03.act";
    sprite = "이팩트\\m_ef03.spr";
  }

  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), sprite);
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), action);
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].rotStart =
      static_cast<std::int16_t>(static_cast<std::int16_t>(static_cast<std::int16_t>(this->state.stateCnt) * 4 + i) * 45);
    this->state.unknown_11C84->slots[i].height[0] = 0.3f;
    this->state.unknown_11C84->slots[i].vecBNow.x = 0.0f;
    this->state.unknown_11C84->slots[i].vecBNow.y = -5.0f;
    this->state.unknown_11C84->slots[i].vecBNow.z = 0.0f;
    this->state.unknown_11C84->slots[i].vecBNow.x += g_cosTable[this->state.unknown_11C84->slots[i].rotStart] * 5.0f;
    this->state.unknown_11C84->slots[i].vecBNow.z += g_sinTable[this->state.unknown_11C84->slots[i].rotStart] * 5.0f;
    if (kind == 9) {
      this->state.unknown_11C84->slots[i].flag1[0] = 0;
    } else if (kind == 10) {
      this->state.unknown_11C84->slots[i].flag1[0] = 1;
    } else if (kind == 20) {
      this->state.unknown_11C84->slots[i].flag1[0] = 4;
    }
  }

  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C71EA0
// FUNCTION: fn_20322
void CRagEffect::RocketPunch() {
  const int frame = this->state.stateCnt;
  if (frame != 0 && frame != 2 && frame != 4 && frame != 6) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->pattern |= 0x100u;
  this->state.unknown_11C84->deltaPos2 = {0.0f, -15.0f, 0.0f};
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(this->state.unknown_11C84);
  particle.state.speed = -(particle.CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) / particle.state.duration);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
  this->state.unknown_11C84->size = 1.0f;
  this->state.unknown_11C84->animSpeed = 1;
  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84->pattern |= 0x200u;
  } else {
    this->state.unknown_11C84->alpha = static_cast<float>((8 - this->state.stateCnt) * 10);
  }

  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\남_로켓트펀치.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\남_로켓트펀치.act");
  std::int16_t angle;
  Get_Angle(this->render.position.x - this->state.deltaPos[0], this->render.position.z - this->state.deltaPos[2], &angle);
  const CView* camera = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
  std::int16_t direction = static_cast<std::int16_t>(static_cast<int>(camera->camera.yaw) + angle) % 360;
  if (direction < 0) {
    direction += 360;
  }

  if (direction >= 180 && direction < 270) {
    Owner(this->state.unknown_11C84).SetAction(4, 16, 1);
  } else if (direction >= 90 && direction < 180) {
    Owner(this->state.unknown_11C84).SetAction(6, 16, 1);
  } else if (direction >= 0 && direction < 90) {
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  } else {
    Owner(this->state.unknown_11C84).SetAction(2, 16, 1);
  }

  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C73120
// FUNCTION: fn_20325
void CRagEffect::GreenRocketPunch() {
  const int frame = this->state.stateCnt;
  if (frame != 0 && frame != 2 && frame != 4 && frame != 6) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->pattern |= 0x100u;
  this->state.unknown_11C84->deltaPos2 = {0.0f, -15.0f, 0.0f};
  Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
  this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(this->state.unknown_11C84);
  particle.state.speed = -(particle.CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]) / particle.state.duration);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
  this->state.unknown_11C84->size = 1.0f;
  this->state.unknown_11C84->animSpeed = 1;
  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84->pattern |= 0x200u;
  } else {
    this->state.unknown_11C84->alpha = static_cast<float>((8 - this->state.stateCnt) * 10);
  }

  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\남_로켓트펀치.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\남_로켓트펀치.act");
  std::int16_t angle;
  Get_Angle(this->render.position.x - this->state.deltaPos[0], this->render.position.z - this->state.deltaPos[2], &angle);
  const CView* camera = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
  std::int16_t direction = static_cast<std::int16_t>(static_cast<int>(camera->camera.yaw) + angle) % 360;
  if (direction < 0) {
    direction += 360;
  }

  this->state.unknown_11C84->red = 105;
  this->state.unknown_11C84->green = 255;
  this->state.unknown_11C84->blue = 105;
  this->state.unknown_11C84->alpha = static_cast<float>((5 - this->state.stateCnt) * 40);
  if (direction >= 180 && direction < 270) {
    Owner(this->state.unknown_11C84).SetAction(4, 16, 1);
  } else if (direction >= 90 && direction < 180) {
    Owner(this->state.unknown_11C84).SetAction(6, 16, 1);
  } else if (direction >= 0 && direction < 90) {
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  } else {
    Owner(this->state.unknown_11C84).SetAction(2, 16, 1);
  }

  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C6FF90
// FUNCTION: fn_20311
void CRagEffect::TexturedQuadRing(std::int16_t baseAlpha, const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  if (this->state.duration < 56) {
    this->state.duration = 56;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 139, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[0].maxHeight = 25.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[0].maxHeight = 15.0f;
  } else {
    this->state.unknown_11C84->slots[0].maxHeight = 20.0f;
  }

  this->state.unknown_11C84->slots[0].rotStart = 180;
  this->state.unknown_11C84->slots[0].alphaB = baseAlpha + 135;
  this->state.unknown_11C84->slots[0].distance = 4.1f;
  this->state.unknown_11C84->slots[0].riseAngle = 80;
  this->state.unknown_11C84->slots[0].alphaT = kind;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[1].maxHeight = 24.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[1].maxHeight = 14.0f;
  } else {
    this->state.unknown_11C84->slots[1].maxHeight = 19.0f;
  }

  this->state.unknown_11C84->slots[1].rotStart = 270;
  this->state.unknown_11C84->slots[1].alphaB = baseAlpha + 90;
  this->state.unknown_11C84->slots[1].distance = 4.1f;
  this->state.unknown_11C84->slots[1].riseAngle = 80;
  this->state.unknown_11C84->slots[1].alphaT = kind;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[2].maxHeight = 23.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[2].maxHeight = 13.0f;
  } else {
    this->state.unknown_11C84->slots[2].maxHeight = 18.0f;
  }

  this->state.unknown_11C84->slots[2].rotStart = 0;
  this->state.unknown_11C84->slots[2].alphaB = baseAlpha + 45;
  this->state.unknown_11C84->slots[2].distance = 4.1f;
  this->state.unknown_11C84->slots[2].riseAngle = 80;
  this->state.unknown_11C84->slots[2].alphaT = kind;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[3].maxHeight = 22.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[3].maxHeight = 12.0f;
  } else {
    this->state.unknown_11C84->slots[3].maxHeight = 17.0f;
  }

  this->state.unknown_11C84->slots[3].rotStart = 90;
  this->state.unknown_11C84->slots[3].alphaB = baseAlpha;
  this->state.unknown_11C84->slots[3].distance = 4.1f;
  this->state.unknown_11C84->slots[3].riseAngle = 80;
  this->state.unknown_11C84->slots[3].alphaT = kind;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  int delay = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 21; ++j) {
      this->state.unknown_11C84->slots[i].height[j] = 0.0f;
      this->state.unknown_11C84->slots[i].flag1[j] = 0;
    }

    if (this->state.duration <= 60) {
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].process = delay;
    }

    delay -= 5;
  }
}

// INLINED
static __forceinline void LaunchTwinkleRing(CRagEffect& effect, const char* name, char level) {
  effect.state.unknown_11C84 = &CreateEffectParticle(effect, effect.render, 132, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  effect.state.unknown_11C84->renderFlag = 0;
  effect.state.unknown_11C84->renderFlag |= 1u;
  effect.state.unknown_11C84->renderFlag |= 5u;
  effect.state.unknown_11C84->duration = effect.state.duration;
  AllocateTextures(*effect.state.unknown_11C84);
  effect.state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  effect.state.unknown_11C84->size = 1.0f;
  for (int i = 0; i < 4; ++i) {
    effect.state.unknown_11C84->slots[i].life = 1;
    effect.state.unknown_11C84->slots[i].process = i * -20;
    effect.state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    effect.state.unknown_11C84->slots[i].maxHeight = 10.0f;
    effect.state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    effect.state.unknown_11C84->slots[i].alphaB = 100;
    effect.state.unknown_11C84->slots[i].height[4] = 10.0f;
    effect.state.unknown_11C84->slots[i].height[3] = static_cast<float>(i);
    effect.state.unknown_11C84->slots[i].height[5] = static_cast<float>(level);
  }
}

// OFFSET: 0x00C704E0
// FUNCTION: fn_20312
void CRagEffect::TexturedTwinkleRing(const char* name, char level) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    LaunchTwinkleRing(*this, name, level);
  }

  if (this->state.stateCnt == 10) {
    LaunchTwinkleRing(*this, name, level);
  }
}

// OFFSET: 0x00C71460
// FUNCTION: fn_20319
void CRagEffect::TetraVortexPillar(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  switch (this->state.stateCnt) {
  case 0:
    g_soundMgr->PlayWave(
      "effect\\wizard_meteo.wav", this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z,
      250, 40, 1.0f, 0
    );
    g_soundMgr->PlayWave(
      "effect\\hunter_blastmine.wav", this->render.position.x - player->render.position.x, 0.0f,
      this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
    break;
  case 10:
  case 20:
  case 50:
  case 80:
    g_soundMgr->PlayWave(
      "effect\\wl_tetravortex.wav", this->render.position.x - player->render.position.x, 0.0f,
      this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
    break;
  }

  if (this->state.stateCnt > 80 || this->state.stateCnt % 25 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 140, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  if (kind == 4) {
    this->state.unknown_11C84->totalTexture = 8;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\tv-1.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\tv-2.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\tv-3.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\tv-4.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\tv-5.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\tv-6.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[6] = g_texMgr.Load("effect\\tv-7.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[7] = g_texMgr.Load("effect\\tv-8.bmp", 0, false, true, false);
  }

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  if (this->state.param[0] > 0.0f) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0];
  } else {
    this->state.unknown_11C84->slots[0].distance = 30.0f;
  }

  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 10.0f;
  if (kind == 4) {
    switch (this->state.type) {
    case 970:
      this->state.unknown_11C84->slots[0].flag1[2] = 20;
      break;
    case 971:
      this->state.unknown_11C84->slots[0].flag1[2] = 18;
      break;
    case 972:
      this->state.unknown_11C84->slots[0].flag1[2] = 17;
      break;
    case 973:
      this->state.unknown_11C84->slots[0].flag1[2] = 19;
      break;
    }

    this->state.unknown_11C84->slots[0].fullDisplayAngle = 120;
    this->state.unknown_11C84->slots[0].height[10] = 1.0f;
  } else {
    this->state.unknown_11C84->slots[0].flag1[2] = 17;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
  }

  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 200;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->state.deltaPos[0];
  this->state.unknown_11C84->slots[0].vecBNow.z = this->state.deltaPos[2];
  this->state.unknown_11C84->slots[0].vecBNow.y = this->state.deltaPos[1] - 35.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[4] = kind;
}

// INLINED
static __forceinline void LaunchFireballVolley(CRagEffect& effect, float turn, int shade, int fade) {
  effect.state.unknown_11C84 = &CreateEffectParticle(effect, effect.render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  effect.state.unknown_11C84->renderFlag = 5;
  effect.state.unknown_11C84->duration = 20;
  effect.state.unknown_11C84->pattern |= 0x120u;
  effect.state.unknown_11C84->deltaPos2 = {0.0f, -15.0f, 0.0f};
  Owner(effect.state.unknown_11C84).Face(effect.state.deltaPos[0], effect.state.deltaPos[2]);
  if (turn != 0.0f) {
    Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle + turn;
    if (Owner(effect.state.unknown_11C84).render.angle < 0.0f) {
      Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle + 360.0f;
    }

    if (Owner(effect.state.unknown_11C84).render.angle >= 360.0f) {
      Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle - 360.0f;
    }
  }

  effect.state.unknown_11C84->longitude = -Owner(effect.state.unknown_11C84).render.angle;
  Owner(effect.state.unknown_11C84).render.angle = 180.0f - Owner(effect.state.unknown_11C84).render.angle;
  if (Owner(effect.state.unknown_11C84).render.angle < 0.0f) {
    Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(effect.state.unknown_11C84);
  particle.state.speed = -(particle.CalcDist(effect.state.deltaPos[0], effect.state.deltaPos[2]) / particle.state.duration);
  reinterpret_cast<matrix*>(effect.state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(effect.state.unknown_11C84->longitude)));
  effect.state.unknown_11C84->size = 1.0f;
  effect.state.unknown_11C84->animSpeed = 1;
  effect.state.unknown_11C84->red = shade;
  effect.state.unknown_11C84->green = shade;
  effect.state.unknown_11C84->blue = 255;
  effect.state.unknown_11C84->alpha = static_cast<float>((fade - effect.state.stateCnt) * 40);
  Owner(effect.state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\fireball.spr");
  Owner(effect.state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\fireball.act");
  Owner(effect.state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C721A0
// FUNCTION: fn_20323
void CRagEffect::BlueFireballVolley() {
  if (this->state.stateCnt == 5) {
    g_soundMgr->PlayWave("effect\\EF_FireBall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  const int frame = this->state.stateCnt;
  if (frame == 0 || frame == 1 || frame == 2 || frame == 3) {
    LaunchFireballVolley(*this, 0.0f, 55, 5);
  }

  if (this->state.stateCnt == 6 || this->state.stateCnt == 7 || this->state.stateCnt == 8 || this->state.stateCnt == 9) {
    LaunchFireballVolley(*this, -5.0f, 30, 11);
  }

  if (this->state.stateCnt == 12 || this->state.stateCnt == 13 || this->state.stateCnt == 14 || this->state.stateCnt == 15) {
    LaunchFireballVolley(*this, 5.0f, 5, 17);
  }
}

// OFFSET: 0x00C72960
// FUNCTION: fn_20324
void CRagEffect::PaleFireballVolley() {
  if (this->state.stateCnt == 5) {
    g_soundMgr->PlayWave("effect\\EF_FireBall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  const int frame = this->state.stateCnt;
  if (frame == 0 || frame == 1 || frame == 2 || frame == 3) {
    LaunchFireballVolley(*this, 0.0f, 155, 5);
  }

  if (this->state.stateCnt == 6 || this->state.stateCnt == 7 || this->state.stateCnt == 8 || this->state.stateCnt == 9) {
    LaunchFireballVolley(*this, -5.0f, 130, 11);
  }

  if (this->state.stateCnt == 12 || this->state.stateCnt == 13 || this->state.stateCnt == 14 || this->state.stateCnt == 15) {
    LaunchFireballVolley(*this, 5.0f, 105, 17);
  }
}

// OFFSET: 0x00C67970
// FUNCTION: fn_20281
void CRagEffect::TeleportRings(const char* name, std::int16_t level) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  PlayEffectWave(*this, "effect\\EF_Teleportation.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 57, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  const float rise = static_cast<float>(level * 60);
  const float shrink = static_cast<float>(level * 2);
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = rise + 100.0f;
  this->state.unknown_11C84->slots[0].rotStart = 180;
  this->state.unknown_11C84->slots[0].alphaB = 100;
  this->state.unknown_11C84->slots[0].distance = 1.5f - shrink;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = rise + 70.0f;
  this->state.unknown_11C84->slots[1].rotStart = 270;
  this->state.unknown_11C84->slots[1].alphaB = 80;
  this->state.unknown_11C84->slots[1].distance = 3.0f - shrink;
  this->state.unknown_11C84->slots[1].riseAngle = 89;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = rise + 40.0f;
  this->state.unknown_11C84->slots[2].rotStart = 0;
  this->state.unknown_11C84->slots[2].alphaB = 60;
  this->state.unknown_11C84->slots[2].distance = 4.0f - shrink;
  this->state.unknown_11C84->slots[2].riseAngle = 88;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = rise + 15.0f;
  this->state.unknown_11C84->slots[3].rotStart = 90;
  this->state.unknown_11C84->slots[3].alphaB = 40;
  this->state.unknown_11C84->slots[3].distance = 5.0f - shrink;
  this->state.unknown_11C84->slots[3].riseAngle = 87;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 21; ++j) {
      this->state.unknown_11C84->slots[i].height[j] = 0.0f;
    }

    this->state.unknown_11C84->slots[i].flag1[0] = 0;
    this->state.unknown_11C84->slots[i].flag1[1] = static_cast<char>(this->state.unknown_11C84->slots[i].maxHeight);
  }
}

// OFFSET: 0x00C68200
// FUNCTION: fn_20283
void CRagEffect::AlphaCenterStream() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 9) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 69, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_center.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos.x = this->state.deltaPos[0];
  this->state.unknown_11C84->deltaPos.y = this->state.deltaPos[1];
  this->state.unknown_11C84->deltaPos.z = this->state.deltaPos[2];

  const float dz = this->state.deltaPos[2] - this->render.position.z;
  const float dx = this->state.deltaPos[0] - this->render.position.x;
  const float range = SquareRoot(dz * dz + dx * dx);
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = -30 - std::max(rand() % 110, 0);
    Get_Angle(dx, dz, &this->state.unknown_11C84->slots[i].rotStart);
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 100, 0)) * 0.08f - 4.0f + this->render.position.x;
    this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 100, 0)) * 0.08f - 4.0f + this->render.position.z;
    this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 100, 0)) * 0.15f;
    this->state.unknown_11C84->slots[i].vecBNow.x -=
      g_cosTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
    this->state.unknown_11C84->slots[i].vecBNow.z -=
      g_sinTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
    this->state.unknown_11C84->slots[i].vecTNow.x = this->state.unknown_11C84->slots[i].vecBNow.x;
    this->state.unknown_11C84->slots[i].vecTNow.z = this->state.unknown_11C84->slots[i].vecBNow.z;
    this->state.unknown_11C84->slots[i].vecTNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(std::max(rand() % 11, 0)) * 0.008f + 0.02f;
    this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 100, 0)) * 0.06f + 4.0f;
    this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 100, 0)) * 0.3f + range;
    this->state.unknown_11C84->slots[i].height[4] = 3.0f;
  }
}

// OFFSET: 0x00C68600
// FUNCTION: fn_20284
void CRagEffect::CloudDrift() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt < 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 69, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
    this->state.unknown_11C84->deltaPos.x = this->state.deltaPos[0];
    this->state.unknown_11C84->deltaPos.y = this->state.deltaPos[1];
    this->state.unknown_11C84->deltaPos.z = this->state.deltaPos[2];

    const float dx = this->state.deltaPos[0] - this->render.position.x;
    const float dz = this->state.deltaPos[2] - this->render.position.z;
    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = -std::max(rand() % 20, 0);
      Get_Angle(dx, dz, &this->state.unknown_11C84->slots[i].rotStart);
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 100, 0)) * 0.06f - 3.0f + this->render.position.x;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 100, 0)) * 0.06f - 3.0f + this->render.position.z;
      this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 100, 0)) * 0.15f;
      this->state.unknown_11C84->slots[i].vecBNow.x -=
        g_cosTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
      this->state.unknown_11C84->slots[i].vecBNow.z -=
        g_sinTable[this->state.unknown_11C84->slots[i].rotStart] * this->state.unknown_11C84->slots[i].height[1];
      this->state.unknown_11C84->slots[i].vecTNow.x = this->state.unknown_11C84->slots[i].vecBNow.x;
      this->state.unknown_11C84->slots[i].vecTNow.z = this->state.unknown_11C84->slots[i].vecBNow.z;
      this->state.unknown_11C84->slots[i].vecTNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].maxHeight = 0.25f;
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 100, 0)) * 0.04f + 8.0f;
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 100, 0)) * 0.2f + 20.0f;
      this->state.unknown_11C84->slots[i].height[4] = 0.0f;
    }
  }

  if (this->state.stateCnt == 10) {
    PlayEffectWave(*this, "effect\\EF_hit2.wav");
  }

  if (this->state.stateCnt == 20) {
    PlayEffectWave(*this, "effect\\EF_hit4.wav");
  }

  if (this->state.stateCnt == 30) {
    PlayEffectWave(*this, "effect\\EF_hit2.wav");
  }
}

// OFFSET: 0x00C68AF0
// FUNCTION: fn_20285
void CRagEffect::NotifyGameMode(char value) {
  if (this->state.stateCnt == 0) {
    g_modeMgr.gameMode->SendMsg(165, value, 0, 0, 0);
  }
}

// OFFSET: 0x00C68F20
// FUNCTION: fn_20287
void CRagEffect::ScatterItemIcons(char kind) {
  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  this->render.position.x = player->render.position.x;
  this->render.position.y = player->render.position.y;
  this->render.position.z = player->render.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 123, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  if (kind == 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("유저인터페이스\\item\\하얀허브.bmp", 0, false, true, false);
  } else if (kind == 1) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("유저인터페이스\\item\\화이트슬림포션.bmp", 0, false, true, false);
  } else if (kind == 2) {
    const char pick = static_cast<char>(std::max(rand() % 3, 0));
    if (pick == 0) {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("유저인터페이스\\item\\화염병.bmp", 0, false, true, false);
    } else if (pick == 1) {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("유저인터페이스\\item\\염산병.bmp", 0, false, true, false);
    } else {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("유저인터페이스\\item\\알코올.bmp", 0, false, true, false);
    }
  }

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].height[7] = 3.0f;
    this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].flag1[0] = 0;
    this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(static_cast<std::int16_t>(this->state.duration) - 50);
    this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 160, 0) - 80);
    this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 160, 0) - 80);
    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    const int drop = rand() % 66;
    this->state.unknown_11C84->slots[i].vecBNow.y =
      world->terrain->Grid().Height(
        this->state.unknown_11C84->slots[i].vecBNow.x + this->render.position.x, this->state.unknown_11C84->slots[i].vecBNow.z + this->render.position.z
      ) -
      5.0f - static_cast<float>(std::max(drop, 0));
    this->state.unknown_11C84->slots[i].vecBNow.x = this->render.position.x + this->state.unknown_11C84->slots[i].vecBNow.x;
    this->state.unknown_11C84->slots[i].vecBNow.z = this->render.position.z + this->state.unknown_11C84->slots[i].vecBNow.z;
    this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 3.0f;
    this->state.unknown_11C84->slots[i].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[2] = 7;
  }
}

// OFFSET: 0x00C69910
// FUNCTION: fn_20290
void CRagEffect::Vallentine(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\vallentine.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->pattern |= 0x200u;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->deltaPos2.y = 0.0f;
  this->state.unknown_11C84->animSpeed = 2;
  this->state.unknown_11C84->repeatAnim = 0;
  if (kind == 2) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\05vallentine.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\05vallentine.act");
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  } else if (kind == 3) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\fast.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\fast.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  } else if (kind == 4) {
    this->state.unknown_11C84->renderFlag |= 0x200u;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\ro2effect.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\ro2effect.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  } else {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\vallentine.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\vallentine.act");
    Owner(this->state.unknown_11C84).SetAction(kind, 16, 1);
  }

  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C6C6A0
// FUNCTION: fn_20296
void CRagEffect::TexturedSpinShards(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 30 || this->state.stateCnt % 10 != 0) {
    return;
  }

  CEffectPrim* prim = CreateEffectParticle(*this, this->render, 170, 0.0f, 0.0f, 0.0f, WorldObjects());
  if (prim == nullptr) {
    return;
  }

  EffectPrimitiveState& particle = prim->state;
  particle.renderFlag = 5;
  particle.duration = this->state.duration;
  if (static_cast<int>(particle.totalTexture) > 0) {
    AllocateTextures(particle);
    if (particle.textures != nullptr) {
      particle.textures[0] = g_texMgr.Load(name, 0, false, true, false);
    }
  }

  particle.size = 0.0f;
  particle.slots[0].life = 0;
  particle.slots[1].life = 0;
  particle.slots[2].life = 0;
  particle.slots[3].life = 0;

  const float x = this->render.position.x;
  const float z = this->render.position.z;
  const float y = this->render.position.y - 5.0f;
  for (int i = 0; i < 4; ++i) {
    EffectPrimitiveSlot& slot = particle.slots[i];
    slot.life = 1;
    slot.maxHeight = static_cast<float>(std::max(rand() % 21, 0)) * 0.001f + 0.4f;
    const int roll = rand();
    if (kind == 3) {
      slot.distance = static_cast<float>(std::max(roll % 41, 0)) * 0.1f + 1.0f;
    } else {
      slot.distance = static_cast<float>(std::max(roll % 21, 0)) * 0.1f + 3.0f;
    }

    slot.process = 0;
    Get_Angle(this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &slot.riseAngle);
    slot.vecBPre.z = z - this->render.position.z;
    slot.vecBPre.x = x - this->render.position.x;
    slot.vecBPre.y = y - this->render.position.y;
    slot.height[0] = static_cast<float>(slot.riseAngle);
    slot.height[1] = static_cast<float>(slot.riseAngle + rand() % 360);
    slot.height[3] = 180.0f;
    slot.height[4] = 180.0f;
    slot.height[12] = 0.0f;
    slot.height[2] = static_cast<float>(rand() % 360 + slot.riseAngle);
    slot.vecBNow.z = z - this->render.position.z;
    slot.vecBNow.x = x - this->render.position.x;
    slot.vecBNow.y = y - this->render.position.y;
    slot.height[5] = static_cast<float>(slot.riseAngle);
    slot.height[6] = static_cast<float>(slot.riseAngle + rand() % 360);
    const int spin = slot.riseAngle + rand() % 360;
    slot.height[8] = 180.0f;
    slot.height[9] = 180.0f;
    slot.height[7] = static_cast<float>(spin);
    for (int j = 0; j < 10; ++j) {
      if (slot.height[j] < 0.0f) {
        slot.height[j] += 360.0f;
      }

      if (slot.height[j] >= 360.0f) {
        slot.height[j] -= 360.0f;
      }
    }

    slot.height[13] = 0.0f;
    slot.flag1[0] = kind;
  }
}

// OFFSET: 0x00C69B00
// FUNCTION: fn_20291
void CRagEffect::HealCloudRings() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 62, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 90;
  this->state.unknown_11C84->slots[0].maxHeight = 8.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 3.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  for (int j = 0; j < 21; ++j) {
    this->state.unknown_11C84->slots[0].height[j] = 3.0f;
    this->state.unknown_11C84->slots[0].flag1[j] = 0;
  }

  this->state.unknown_11C84->slots[0].alphaT = 0;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 70;
  this->state.unknown_11C84->slots[1].maxHeight = 8.0f;
  this->state.unknown_11C84->slots[1].rotStart = 10;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 3.2f;
  this->state.unknown_11C84->slots[1].riseAngle = 90;
  for (int j = 0; j < 21; ++j) {
    this->state.unknown_11C84->slots[1].height[j] = 2.5f;
    this->state.unknown_11C84->slots[1].flag1[j] = 0;
  }

  this->state.unknown_11C84->slots[1].alphaT = 0;

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 50;
  this->state.unknown_11C84->slots[2].maxHeight = 8.0f;
  this->state.unknown_11C84->slots[2].rotStart = 20;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 3.4f;
  this->state.unknown_11C84->slots[2].riseAngle = 90;
  for (int j = 0; j < 21; ++j) {
    this->state.unknown_11C84->slots[2].height[j] = 2.0f;
    this->state.unknown_11C84->slots[2].flag1[j] = 0;
  }

  this->state.unknown_11C84->slots[2].alphaT = 0;

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 30;
  this->state.unknown_11C84->slots[3].maxHeight = 8.0f;
  this->state.unknown_11C84->slots[3].rotStart = 30;
  this->state.unknown_11C84->slots[3].alphaB = 0;
  this->state.unknown_11C84->slots[3].distance = 3.6f;
  this->state.unknown_11C84->slots[3].riseAngle = 90;
  for (int j = 0; j < 21; ++j) {
    this->state.unknown_11C84->slots[3].height[j] = 1.5f;
    this->state.unknown_11C84->slots[3].flag1[j] = 0;
  }

  this->state.unknown_11C84->slots[3].alphaT = 0;
  PlayEffectWave(*this, "_heal_effect.wav");
}

// OFFSET: 0x00C6BF40
// FUNCTION: fn_20295
void CRagEffect::WhiteLightSparkle(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 130, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1u;
  this->state.unknown_11C84->renderFlag |= 5u;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\whitelight.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int i = 0; i < 4; ++i) {
    if (kind == 1 && i > 0) {
      this->state.unknown_11C84->slots[i].life = 0;
    } else {
      this->state.unknown_11C84->slots[i].life = 1;
    }

    this->state.unknown_11C84->slots[i].maxHeight = 0.15f;
    this->state.unknown_11C84->slots[i].distance = 2.4f;
    if (kind == 1) {
      this->state.unknown_11C84->slots[i].vecBPre.x = 0.0f;
      this->state.unknown_11C84->slots[i].vecBPre.z = 0.0f;
    } else {
      this->state.unknown_11C84->slots[i].vecBPre.x = static_cast<float>(std::max(rand() % 41, 0) - 20);
      this->state.unknown_11C84->slots[i].vecBPre.z = static_cast<float>(std::max(rand() % 41, 0) - 20);
    }

    this->state.unknown_11C84->slots[i].vecBPre.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[11] = 0;
    this->state.unknown_11C84->slots[i].flag1[7] = 0;

    this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 41, 0) - 20);
    this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 41, 0) - 20);
    this->state.unknown_11C84->slots[i].vecBNow.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[5] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[6] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[7] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[12] = 0;
    this->state.unknown_11C84->slots[i].flag1[8] = 0;

    this->state.unknown_11C84->slots[i].vecTPre.x = static_cast<float>(std::max(rand() % 41, 0) - 20);
    this->state.unknown_11C84->slots[i].vecTPre.z = static_cast<float>(std::max(rand() % 41, 0) - 20);
    this->state.unknown_11C84->slots[i].vecTPre.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[9] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[10] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[11] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[13] = 0;
    this->state.unknown_11C84->slots[i].flag1[9] = 0;

    this->state.unknown_11C84->slots[i].vecTNow.x = static_cast<float>(std::max(rand() % 41, 0) - 20);
    this->state.unknown_11C84->slots[i].vecTNow.z = static_cast<float>(std::max(rand() % 41, 0) - 20);
    this->state.unknown_11C84->slots[i].vecTNow.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[12] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[13] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[14] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[15] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[14] = 0;
    this->state.unknown_11C84->slots[i].flag1[10] = 0;

    this->state.unknown_11C84->slots[i].vecBPre.y = 0.0f;
    this->state.unknown_11C84->slots[i].vecBNow.y = 0.0f;
    this->state.unknown_11C84->slots[i].vecTPre.y = 0.0f;
    this->state.unknown_11C84->slots[i].vecTNow.y = 0.0f;
    this->state.unknown_11C84->slots[i].flag1[0] = kind;
    this->state.unknown_11C84->slots[i].flag1[1] = 0;
    this->state.unknown_11C84->slots[i].flag1[2] = 0;
    this->state.unknown_11C84->slots[i].flag1[3] = 0;
    this->state.unknown_11C84->slots[i].flag1[4] = 0;
    this->state.unknown_11C84->slots[i].flag1[5] = static_cast<char>((kind == 2) * 2 + 8);
  }
}

// INLINED
static __forceinline void ScatterFrostVector(CRagEffect& effect, int slot, int vector, int firstHeight) {
  if (effect.state.unknown_11C84->slots[slot].flag1[12] == 0) {
    effect.state.unknown_11C84->slots[slot].vectors[vector].x = static_cast<float>(std::max(rand() % 45, 0) - 22);
  } else {
    effect.state.unknown_11C84->slots[slot].vectors[vector].x = static_cast<float>(std::max(rand() % 23, 0) - 11);
  }

  effect.state.unknown_11C84->slots[slot].vectors[vector].y = static_cast<float>(std::max(rand() % 30, 0));
  effect.state.unknown_11C84->slots[slot].vectors[vector].z = static_cast<float>(std::max(rand() % 11, 0) - 5);
  for (int j = firstHeight; j < firstHeight + 4; ++j) {
    effect.state.unknown_11C84->slots[slot].height[j] = static_cast<float>(std::max(rand() % 360, 0));
  }

  if (effect.state.unknown_11C84->slots[slot].flag1[11] == 1) {
    const float x = effect.state.unknown_11C84->slots[slot].vectors[vector].x;
    effect.state.unknown_11C84->slots[slot].vectors[vector].x = effect.state.unknown_11C84->slots[slot].vectors[vector].z;
    effect.state.unknown_11C84->slots[slot].vectors[vector].z = static_cast<float>(static_cast<int>(x));
  }
}

// OFFSET: 0x00C6B780
// FUNCTION: fn_20294
void CRagEffect::FrostScatter(char swapAxes, char narrow) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 80, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);

  bool water;
  if (std::max(rand() % 10, 0) > 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\freeze_a_small.bmp", 0, false, true, false);
    water = false;
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\waterp1.tga", 0, false, true, false);
    water = true;
  }

  this->state.unknown_11C84->size = 4.0f;
  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].maxHeight = 0.05f;
    this->state.unknown_11C84->slots[i].flag1[11] = swapAxes;
    this->state.unknown_11C84->slots[i].flag1[12] = narrow;
    this->state.unknown_11C84->slots[i].distance = water ? 7.0f : 4.5f;
    ScatterFrostVector(*this, i, 2, 0);
    ScatterFrostVector(*this, i, 0, 4);
    ScatterFrostVector(*this, i, 3, 8);
    ScatterFrostVector(*this, i, 1, 12);
    this->state.unknown_11C84->slots[i].flag1[0] = 2;
  }
}

// INLINED
static __forceinline void SetupRippleSlot(CRagEffect& effect, int slot, std::int16_t rotStart, float distance, std::int16_t riseAngle) {
  effect.state.unknown_11C84->slots[slot].life = 1;
  effect.state.unknown_11C84->slots[slot].process = 0;
  effect.state.unknown_11C84->slots[slot].fullDisplayAngle = 360;
  effect.state.unknown_11C84->slots[slot].rotStart = rotStart;
  effect.state.unknown_11C84->slots[slot].alphaB = 0;
  effect.state.unknown_11C84->slots[slot].distance = distance;
  effect.state.unknown_11C84->slots[slot].maxHeight = effect.state.unknown_11C84->slots[slot].distance;
  effect.state.unknown_11C84->slots[slot].riseAngle = riseAngle;
  effect.state.unknown_11C84->slots[slot].vecBNow.y = 0.0f;
  for (int j = 0; j < 21; ++j) {
    effect.state.unknown_11C84->slots[slot].height[j] = 0.0f;
    effect.state.unknown_11C84->slots[slot].flag1[j] = 0;
  }
}

// INLINED
static __forceinline void LaunchRippleRings(CRagEffect& effect, const char* name, char kind, float smallSize, int turn, float growth) {
  const RenderObjectState& parent = effect.state.parent->render;
  effect.render.position.x = parent.position.x;
  effect.render.position.y = parent.position.y;
  effect.render.position.z = parent.position.z;
  effect.state.unknown_11C84 = &CreateEffectParticle(effect, effect.render, 65, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  effect.state.unknown_11C84->renderFlag = 5;
  effect.state.unknown_11C84->duration = effect.state.duration;
  AllocateTextures(*effect.state.unknown_11C84);
  effect.state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  if (kind == 0) {
    effect.state.unknown_11C84->size = smallSize;
  } else if (kind == 1) {
    effect.state.unknown_11C84->size = 18.0f;
  }

  SetupRippleSlot(effect, 0, static_cast<std::int16_t>(270 + turn), 2.5f + growth, static_cast<std::int16_t>(53 - turn));
  SetupRippleSlot(effect, 1, static_cast<std::int16_t>(0 + turn), 5.0f + growth, static_cast<std::int16_t>(60 - turn));
  SetupRippleSlot(effect, 2, static_cast<std::int16_t>(90 + turn), 7.5f + growth, static_cast<std::int16_t>(55 - turn));
  SetupRippleSlot(effect, 3, static_cast<std::int16_t>(180 + turn), 10.0f + growth, static_cast<std::int16_t>(50 - turn));
}

// OFFSET: 0x00C6A1E0
// FUNCTION: fn_20292
void CRagEffect::AlphaDownRipples(const char* name, char kind) {
  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
  } else {
    if (this->state.stateCnt == 1) {
      LaunchRippleRings(*this, name, kind, 11.0f, 0, 0.0f);
    }

    if (this->state.stateCnt == 2) {
      LaunchRippleRings(*this, name, kind, 4.0f, 1, 0.2f);
    }
  }

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 99999999;
    this->state.unknown_11C84->pattern |= 1u;
    this->state.unknown_11C84->radius = 15.0f;
    this->state.unknown_11C84->deltaPos2.y -= 1.5f;
    this->state.unknown_11C84->latitude = 90.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 128.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
  }

  if (this->state.stateCnt % 10 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->radius = static_cast<float>(std::max(rand() % 200, 0) / 100) * 0.75f + 4.5f;
  this->state.unknown_11C84->radiusSpeed = 0.0005f;
  this->state.unknown_11C84->duration = 70;
  this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
  if (std::max(rand() % 2, 0) == 0) {
    this->state.unknown_11C84->longSpeed = 2.5f - static_cast<float>(std::max(rand() % 11, 0)) * 0.1f;
  } else {
    this->state.unknown_11C84->longSpeed = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f - 2.5f;
  }

  this->state.unknown_11C84->gravSpeed = -0.3f;
  EffectPrimitiveState& falling = *this->state.unknown_11C84;
  falling.gravAccel = -(falling.gravSpeed / falling.duration) / 1.5f;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
  EffectPrimitiveState& moving = *this->state.unknown_11C84;
  moving.deltaPos2 = RotatedForward(moving.transform, moving.radius);
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 10, 0) / 10) + 0.7f;
  const int lifetime = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = lifetime - lifetime / 3;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C0F1C0
// FUNCTION: fn_20029
bool CRagEffect::Concentration() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\ac_concentration.wav");
  }

  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C0F470
// FUNCTION: fn_20030
bool CRagEffect::FirePillarBomb() {
  if (this->state.stateCnt == 30) {
    PlayEffectWave(*this, "effect\\wizard_fire_pillar_b.wav");
  }

  if (this->state.stateCnt == 35) {
    g_modeMgr.GetGameMode()->SendMsg(122, 0, 0, 0, 0);
  }

  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C3D100
// FUNCTION: fn_20114
bool CRagEffect::ProcessEZ2STRAtTarget() {
  RenderObjectState& parent = this->state.parent->render;
  this->state.deltaPos2[0] = parent.position.x;
  this->state.deltaPos2[1] = parent.position.y;
  this->state.deltaPos2[2] = parent.position.z;
  parent.position.x = this->state.deltaPos[0];
  parent.position.y = this->state.deltaPos[1];
  parent.position.z = this->state.deltaPos[2];
  const bool running = this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
  RenderObjectState& restored = this->state.parent->render;
  restored.position.x = this->state.deltaPos2[0];
  restored.position.y = this->state.deltaPos2[1];
  restored.position.z = this->state.deltaPos2[2];
  return running ? true : false;
}

// OFFSET: 0x00C3E6C0
// FUNCTION: fn_20117
bool CRagEffect::TwoHandQuicken() {
  if (this->state.stateCnt == 0) {
    this->state.duration = 9999;
    PlayEffectWave(*this, "effect\\knight_twohandquicken.wav");
  }

  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C5B8B0
// FUNCTION: fn_20222
void CRagEffect::SageMagicRod() {
  if (this->state.stateCnt == 0) {
    const RenderObjectState& parent = this->state.parent->render;
    this->render.position.x = parent.position.x;
    this->render.position.y = parent.position.y;
    this->render.position.z = parent.position.z;
    PlayEffectWave(*this, "effect\\sage_magic rod.wav");
  }
}

// OFFSET: 0x00C0E790
// FUNCTION: fn_20026
void CRagEffect::AssassinCloaking() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\assasin_cloaking.wav");
  }

  this->state.parent->SetColor(50, -1, -1, -1);
  this->render.unknown_00A4Bytes[0] = 0;
}

// OFFSET: 0x00C1E4A0
// FUNCTION: fn_20078
bool CRagEffect::ProcessEZ2STRRaised() {
  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C1E530
// FUNCTION: fn_20079
void CRagEffect::LockOnMarker() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->pattern |= 0x800u;
    this->state.unknown_11C84->longitude = 0.0f;
    this->state.unknown_11C84->longSpeed = 4.5f;
    this->state.unknown_11C84->roll = 90.0f;
    this->state.unknown_11C84->changeWidthAccel = 0.0f;
    this->state.unknown_11C84->changeHeightAccel = 0.0f;
    this->state.unknown_11C84->widthSize = 30.0f;
    this->state.unknown_11C84->heightSize = 30.0f;
    this->state.unknown_11C84->widthSpeed = -1.0f;
    this->state.unknown_11C84->heightSpeed = -1.0f;
    this->state.unknown_11C84->changePoint = 20;
    this->state.unknown_11C84->changeWidthSpeed = 0.0f;
    this->state.unknown_11C84->changeHeightSpeed = 0.0f;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->RGBCycleDelay = 20;
    this->state.unknown_11C84->red = 250;
    this->state.unknown_11C84->blue = 150;
    this->state.unknown_11C84->blueSpeed = -15;
    this->state.unknown_11C84->green = 150;
    this->state.unknown_11C84->greenSpeed = -15;
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\LockOn128.tga", 0, false, true, false);

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->pattern |= 0x800u;
    this->state.unknown_11C84->longitude = 45.0f;
    this->state.unknown_11C84->longSpeed = 4.5f;
    this->state.unknown_11C84->roll = 90.0f;
    this->state.unknown_11C84->widthSize = 30.0f;
    this->state.unknown_11C84->heightSize = 30.0f;
    this->state.unknown_11C84->widthSpeed = -1.0f;
    this->state.unknown_11C84->heightSpeed = -1.0f;
    this->state.unknown_11C84->changePoint = 20;
    this->state.unknown_11C84->changeWidthSpeed = 0.0f;
    this->state.unknown_11C84->changeHeightSpeed = 0.0f;
    this->state.unknown_11C84->changeWidthAccel = 0.0f;
    this->state.unknown_11C84->changeHeightAccel = 0.0f;
    this->state.unknown_11C84->RGBCycleDelay = 20;
    this->state.unknown_11C84->red = 250;
    this->state.unknown_11C84->blue = 150;
    this->state.unknown_11C84->blueSpeed = -15;
    this->state.unknown_11C84->green = 150;
    this->state.unknown_11C84->greenSpeed = -15;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\LockOn128.tga", 0, false, true, false);
  }

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  this->render.position.y = world->terrain->Grid().Height(this->render.position.x, this->render.position.z) - 2.0f;
}

// OFFSET: 0x00C1EC30
// FUNCTION: fn_20080
void CRagEffect::MagnumBreak() {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_MagnumBreak.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->renderFlag |= 0x200u;
  this->state.unknown_11C84->latitude = 90.0f;
  this->state.unknown_11C84->radiusSpeed = 1.75f;
  EffectPrimitiveState& inner = *this->state.unknown_11C84;
  inner.radiusAccel = -(inner.radiusSpeed / inner.duration) * 0.5f;
  this->state.unknown_11C84->innerSize = 12.0f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
  Owner(this->state.unknown_11C84).InitSegment();
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 14, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->radiusSpeed = 1.15f;
  EffectPrimitiveState& blast = *this->state.unknown_11C84;
  blast.radiusAccel = -(blast.radiusSpeed / blast.duration) * 0.5f;
  this->state.unknown_11C84->longSpeed = 3.0f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\대폭발.tga", 0, false, true, false);

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 30;
  this->state.unknown_11C84->renderFlag |= 0x200u;
  this->state.unknown_11C84->latitude = 90.0f;
  this->state.unknown_11C84->radiusSpeed = 1.75f;
  EffectPrimitiveState& outer = *this->state.unknown_11C84;
  outer.radiusAccel = -(outer.radiusSpeed / outer.duration) * 0.5f;
  this->state.unknown_11C84->innerSize = 12.0f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 15.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 15;
  Owner(this->state.unknown_11C84).InitSegment();
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);
}

// OFFSET: 0x00C1FE80
// FUNCTION: fn_20085
void CRagEffect::NapalmFragments() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  g_renderer->viewport.Projection().Project(
    &this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex
  );
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt >= 15 || this->state.stateCnt % 5 > this->state.stateCnt) {
    return;
  }

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_NapalmBeat.wav");
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 30;
  const int distance = std::max(rand() % 50, 0) + 10;
  const float radius = static_cast<float>(distance);
  const float angle = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->deltaPos2.x = ClientSine(static_cast<int>(angle)) * radius;
  this->state.unknown_11C84->deltaPos2.y = ClientCosine(static_cast<int>(angle)) * -static_cast<float>(distance);
  this->state.unknown_11C84->deltaPos2.y -= 20.0f;
  const float size = static_cast<float>(std::max(rand() % 40, 0) + 20);
  this->state.unknown_11C84->heightSize = size;
  this->state.unknown_11C84->widthSize = size;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
  EffectPrimitiveState& fade = *this->state.unknown_11C84;
  fade.fadeOutCnt = fade.duration - fade.duration / 3;
  this->state.unknown_11C84->repeatAnim = 0;
  this->state.unknown_11C84->animSpeed = 2;
  this->state.unknown_11C84->totalTexture = 8;
  AllocateTextures(*this->state.unknown_11C84);
  for (int index = 0; index < static_cast<int>(this->state.unknown_11C84->totalTexture); ++index) {
    char name[128];
    std::sprintf(name, "effect\\폭발%d.TGA", index + 1);
    this->state.unknown_11C84->textures[index] = g_texMgr.Load(name, 0, false, true, false);
  }
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
}

// OFFSET: 0x00C35160
// FUNCTION: fn_20089
void CRagEffect::StoneCurseRing() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\black_overthrust.wav");
  }

  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.duration = 9999;
  PlayEffectWave(*this, "effect\\EF_StoneCurse.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 4, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 40;
  this->state.unknown_11C84->deltaPos2.y = -20.0f;
  this->state.unknown_11C84->arcAngle = 10.0f;
  this->state.unknown_11C84->innerSize = 10.0f;
  this->state.unknown_11C84->radiusSpeed = 7.0f;
  EffectPrimitiveState& ring = *this->state.unknown_11C84;
  const float slowdown = -(ring.radiusSpeed / (ring.duration + 40));
  ring.radiusAccel = slowdown + slowdown;
  this->state.unknown_11C84->fadeOutCnt = 0;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_center.tga", 0, false, true, false);
}

// OFFSET: 0x00C49300

// OFFSET: 0x00C49E60
// FUNCTION: fn_20167
void CRagEffect::SnowDrift(char flag) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 134, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 4;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\E9_snow.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\E9_snow01.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\E9_snow02.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\E9_snow03.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(rand() % 21) * 0.08f + 2.5f;
    this->state.unknown_11C84->slots[index].vecBPre.y = this->render.position.y - 12.0f;
    this->state.unknown_11C84->slots[index].flag1[2] = static_cast<char>(rand() % 4);
    this->state.unknown_11C84->slots[index].height[10] = 0.0f;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = static_cast<std::int16_t>(rand() % 360);
    this->state.unknown_11C84->slots[index].height[8] = 0.0f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
    Get_Angle(this->render.position.x - this->state.deltaPos[0], this->render.position.z - this->state.deltaPos[2], &this->state.unknown_11C84->slots[index].riseAngle);
    this->state.unknown_11C84->slots[index].riseAngle += static_cast<std::int16_t>(rand() % 41 + 160);

    if (this->state.unknown_11C84->slots[index].riseAngle >= 360) {
      this->state.unknown_11C84->slots[index].riseAngle -= 360;
    }

    this->state.unknown_11C84->slots[index].vecBNow.x = this->state.deltaPos[0];
    this->state.unknown_11C84->slots[index].vecBNow.z = this->state.deltaPos[2];
    this->state.unknown_11C84->slots[index].vecBNow.y = this->state.unknown_11C84->slots[index].vecBPre.y;
    this->state.unknown_11C84->slots[index].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].flag1[0] = 3;
    this->state.unknown_11C84->slots[index].flag1[4] = flag;
    this->state.unknown_11C84->slots[index].flag1[5] = static_cast<char>(60 - rand() % 21);
  }
}

// OFFSET: 0x00C4E730
// FUNCTION: fn_20178
void CRagEffect::ElectricRingEarly(char kind, std::int16_t angle) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (static_cast<unsigned>(this->state.stateCnt) >= 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 101, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\elec1.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 0.1f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45 + angle);
    this->state.unknown_11C84->slots[index].distance = 0.0f;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 11, 0)) * 0.01f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecBNow.y -= 5.0f;
    this->state.unknown_11C84->slots[index].vecTNow.y = 20.0f;

    for (int height = 20; height > 0; --height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].alphaT = phase;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 3;
  }
}

// OFFSET: 0x00C4EAF0
// FUNCTION: fn_20179
void CRagEffect::ElectricRing(char kind, std::int16_t angle) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (static_cast<unsigned>(this->state.stateCnt - 11) >= 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 101, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\elec1.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 0.1f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45 + angle);
    this->state.unknown_11C84->slots[index].distance = 0.0f;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 11, 0)) * 0.01f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecTNow.y = 20.0f;

    for (int height = 20; height > 0; --height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].alphaT = phase;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
  }
}

// OFFSET: 0x00C4EEA0
// FUNCTION: fn_20180
void CRagEffect::ElectricRingLate(char kind, std::int16_t angle) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (static_cast<unsigned>(this->state.stateCnt - 20) >= 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 101, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\elec1.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].maxHeight = 0.1f;
    this->state.unknown_11C84->slots[index].alphaB = 0;
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(index * 45 + angle);
    this->state.unknown_11C84->slots[index].distance = 0.0f;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 11, 0)) * 0.01f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecTNow.y = 20.0f;

    for (int height = 20; height > 0; --height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].alphaT = phase;
    this->state.unknown_11C84->slots[index].fullDisplayAngle = 0;
  }
}

// OFFSET: 0x00C58BB0
// FUNCTION: fn_20209
void CRagEffect::WhiteSparks(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (static_cast<unsigned>(this->state.stateCnt) >= 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 70, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = -std::max(rand() % 26, 0);
    this->state.unknown_11C84->slots[index].maxHeight = 0.1f;
    this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 51, 0) + 125);
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 8, 0)) + 6.0f;
    this->state.unknown_11C84->slots[index].flag1[0] = 0;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;

    for (int height = 20; height > 0; --height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].alphaT = phase;
  }
}

// OFFSET: 0x00C67E30
// FUNCTION: fn_20282
void CRagEffect::AlphaCenterTrail() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt >= 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 69, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_center.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  const float deltaZ = this->state.deltaPos[2] - this->render.position.z;
  const float deltaX = this->state.deltaPos[0] - this->render.position.x;
  const float distance = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[index];
    slot.life = 1;
    slot.process = -45 - std::max(rand() % 20, 0);
    Get_Angle(deltaX, deltaZ, &slot.rotStart);
    slot.alphaB = 0;
    slot.vecBNow.x = (static_cast<float>(std::max(rand() % 100, 0)) * 0.08f - 4.0f) + this->render.position.x;
    slot.vecBNow.z = (static_cast<float>(std::max(rand() % 100, 0)) * 0.08f - 4.0f) + this->render.position.z;
    slot.vecBNow.y = this->render.position.y;
    slot.height[1] = static_cast<float>(std::max(rand() % 100, 0)) * 0.15f;
    slot.vecBNow.x -= g_cosTable[slot.rotStart] * slot.height[1];
    slot.vecBNow.z -= g_sinTable[slot.rotStart] * slot.height[1];
    slot.vecTNow.x = slot.vecBNow.x;
    slot.vecTNow.z = slot.vecBNow.z;
    slot.vecTNow.y = this->render.position.y;
    slot.maxHeight = 0.05f;
    slot.height[0] = static_cast<float>(std::max(rand() % 100, 0)) * 0.04f + 8.0f;
    slot.distance = static_cast<float>(std::max(rand() % 100, 0)) * 0.6f + distance;
    slot.height[4] = 2.0f;
  }
}

// OFFSET: 0x00C6B3E0
// FUNCTION: fn_20293
void CRagEffect::Waterfall(const char* name, char flag, char tall, char variant) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 79, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 3;
  AllocateTextures(*this->state.unknown_11C84);

  if (variant == 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\waterfall11.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\waterfall12.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\waterfall13.tga", 0, false, true, false);
  } else if (variant == 1) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\waterfall31.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\waterfall32.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\waterfall33.tga", 0, false, true, false);
  }

  int width = 80;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].alphaB = 80;
    this->state.unknown_11C84->slots[index].flag1[0] = flag;

    if (tall == 0) {
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(index + 36);
    } else if (tall == 1) {
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(index + 18);
    }

    this->state.unknown_11C84->slots[index].flag1[2] = static_cast<char>(static_cast<int>(this->state.param[0]));

    for (int height = 0; height < 20; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = static_cast<float>(height * 40);
    }

    this->state.unknown_11C84->slots[index].vecBNow.z = static_cast<float>(index) - 1.0f;
    this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(width);
    this->state.unknown_11C84->slots[index].flag1[1] = 0;
    width -= 13;

    if (width < 29) {
      return;
    }
  }
}

// OFFSET: 0x00C16620
// FUNCTION: fn_20057
bool CRagEffect::GlassWall() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_GlassWall.wav");
    const int facing = static_cast<int>(180.0f - this->state.parent->render.angle);
    const int angle = facing < 0 ? facing + 360 : facing;
    matrix rotation;
    rotation.MakeYRotation(GetRadian(angle));
    const float* m = rotation.values;
    for (int side = 1; side > -2; side -= 2) {
      const float along = static_cast<float>(side) * 2.6f;
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = this->state.duration;
      this->state.unknown_11C84->pattern = 40;
      this->state.unknown_11C84->type = 50;
      this->state.unknown_11C84->deltaPos2 = RotatedForward(m, along);
      this->state.unknown_11C84->longitude = static_cast<float>(angle + 90);
      this->state.unknown_11C84->widthSize = 3.0f;
      this->state.unknown_11C84->heightSize = 40.0f;
      this->state.unknown_11C84->heightSpeed = 0.25f;
      this->state.unknown_11C84->alpha = 0.0f;
      this->state.unknown_11C84->maxAlpha = 180.0f;
      this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 6.0f;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 6;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
      const float across = static_cast<float>(side) * 3.0f;
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = this->state.duration;
      this->state.unknown_11C84->pattern = 40;
      this->state.unknown_11C84->type = 50;
      this->state.unknown_11C84->deltaPos2 = {
        across * m[0] + m[3] * 0.0f + m[6] * 0.0f + m[9],
        across * m[1] + m[4] * 0.0f + m[7] * 0.0f + m[10],
        across * m[2] + m[5] * 0.0f + m[8] * 0.0f + m[11]
      };
      this->state.unknown_11C84->longitude = static_cast<float>(angle);
      this->state.unknown_11C84->widthSize = 2.6f;
      this->state.unknown_11C84->heightSize = 40.0f;
      this->state.unknown_11C84->heightSpeed = 0.25f;
      this->state.unknown_11C84->alpha = 0.0f;
      this->state.unknown_11C84->maxAlpha = 180.0f;
      this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 6.0f;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 6;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
    }
  }

  this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
  return this->state.stateCnt != this->state.duration;
}

// OFFSET: 0x00C36110
// FUNCTION: fn_20091
void CRagEffect::Portal() {
  if (this->state.stateCnt == 0) {
    g_modeMgr.GetGameMode();
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->arcAngle = 36.0f;
    this->state.unknown_11C84->outerSize = 3.5f;
    this->state.unknown_11C84->innerSize = 3.5f;
    this->state.unknown_11C84->longSpeed = 4.0f;
    this->state.unknown_11C84->heightSize = 40.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 128.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 6.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 6;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->arcAngle = 36.0f;
    this->state.unknown_11C84->outerSize = 4.0f;
    this->state.unknown_11C84->innerSize = 4.0f;
    this->state.unknown_11C84->longSpeed = 5.0f;
    this->state.unknown_11C84->heightSize = 50.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 128.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 6.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 6;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  }

  if (this->state.stateCnt % 14 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 30;
    this->state.unknown_11C84->deltaPos2.y = -1.0f;
    this->state.unknown_11C84->innerSize = 8.0f;
    this->state.unknown_11C84->latitude = 90.0f;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    this->state.unknown_11C84->radiusSpeed = 1.0f;
    EffectPrimitiveState& ring = *this->state.unknown_11C84;
    ring.radiusAccel = -(ring.radiusSpeed / static_cast<float>(ring.duration)) * 0.5f;
    const int duration = this->state.unknown_11C84->duration;
    this->state.unknown_11C84->fadeOutCnt = duration - duration / 2;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  }
}


// OFFSET: 0x00C40450
// FUNCTION: fn_20125
void CRagEffect::YufitelHit(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt % 20 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 10;
    this->state.unknown_11C84->pattern |= 0x40u;
    this->state.unknown_11C84->renderFlag |= 2u;
    this->state.unknown_11C84->renderFlag |= 0x200u;
    this->state.unknown_11C84->deltaPos2.y = -5.0f;
    this->state.unknown_11C84->widthSpeed = 2.5f;
    this->state.unknown_11C84->heightSpeed = 2.5f;
    this->state.unknown_11C84->changePoint = 10;
    this->state.unknown_11C84->changeWidthSpeed = 0.0f;
    this->state.unknown_11C84->changeHeightSpeed = 0.0f;
    this->state.unknown_11C84->changeWidthAccel = -this->state.unknown_11C84->changeWidthSpeed / 50.0f;
    this->state.unknown_11C84->changeHeightAccel = -this->state.unknown_11C84->changeHeightSpeed / 50.0f;
    this->state.unknown_11C84->fadeOutCnt = 1;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] =
      g_texMgr.Load(kind == 0 ? "effect\\thunder_pang.bmp" : "effect\\pokjuk_d.bmp", 0, false, true, false);
  }

  if (this->state.stateCnt != 10) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 300;
  this->state.unknown_11C84->pattern |= 0x40u;
  this->state.unknown_11C84->renderFlag |= 0x200u;
  this->state.unknown_11C84->renderFlag |= 2u;
  this->state.unknown_11C84->deltaPos2.y = -5.0f;
  this->state.unknown_11C84->repeatAnim = 1;
  this->state.unknown_11C84->widthSize = 7.5f;
  this->state.unknown_11C84->heightSize = 7.5f;
  this->state.unknown_11C84->animSpeed = 1;
  this->state.unknown_11C84->totalTexture = 5;
  AllocateTextures(*this->state.unknown_11C84);
  const char* last;
  if (kind == 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_plazma_blast_a.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\thunder_plazma_blast_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\thunder_ball_d.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\thunder_ball_e.bmp", 0, false, true, false);
    last = "effect\\thunder_ball_f.bmp";
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\twirl_soft.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\thunder_ball_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\twirl_soft.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\thunder_ball_c.bmp", 0, false, true, false);
    last = "effect\\twirl_soft.bmp";
  }

  this->state.unknown_11C84->textures[4] = g_texMgr.Load(last, 0, false, true, false);
}

// OFFSET: 0x00C457B0
// FUNCTION: fn_20150
void CRagEffect::TexturedPulseKind(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 64, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1u;
  this->state.unknown_11C84->renderFlag |= 5u;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 1.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));

  bool pulsing = true;
  if (kind == 0) {
    this->state.unknown_11C84->slots[0].maxHeight = g_serviceOptions.Get(229) != 0 ? 10.0f : 50.0f;
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 30;
    this->state.unknown_11C84->slots[0].alphaT = 864;
    this->state.unknown_11C84->slots[0].height[0] = 5.0f;
    this->state.unknown_11C84->slots[0].height[1] = 5.0f;
  } else {
    if (kind == 1) {
      this->state.unknown_11C84->slots[0].maxHeight = 16.0f;
      this->state.unknown_11C84->slots[0].rotStart = 0;
      this->state.unknown_11C84->slots[0].alphaB = 120;
      this->state.unknown_11C84->slots[0].alphaT = 1880;
    } else if (kind == 2) {
      this->state.unknown_11C84->slots[0].maxHeight = 32.0f;
      this->state.unknown_11C84->slots[0].rotStart = 0;
      this->state.unknown_11C84->slots[0].alphaB = 30;
      this->state.unknown_11C84->slots[0].alphaT = 29999;
    } else if (kind == 4) {
      this->state.unknown_11C84->slots[0].maxHeight = 16.0f;
      this->state.unknown_11C84->slots[0].rotStart = 0;
      this->state.unknown_11C84->slots[0].alphaB = 120;
      this->state.unknown_11C84->slots[0].alphaT = 1880;
      this->state.unknown_11C84->slots[0].height[0] = 2.5f;
      this->state.unknown_11C84->slots[0].height[1] = 2.5f;
      this->state.unknown_11C84->slots[0].height[2] = 0.0f;
      this->state.unknown_11C84->slots[0].height[3] = 4.0f;
      this->state.unknown_11C84->slots[0].height[4] = static_cast<float>(std::max(rand() % 50, 0));
      pulsing = false;
    } else {
      if (kind == 3) {
        this->state.unknown_11C84->slots[0].maxHeight = 16.0f;
      }

      this->state.unknown_11C84->slots[0].rotStart = 0;
    }

    if (pulsing) {
      this->state.unknown_11C84->slots[0].height[0] = 2.5f;
      this->state.unknown_11C84->slots[0].height[1] = 2.5f;
    }
  }

  if (pulsing) {
    this->state.unknown_11C84->slots[0].height[2] = 0.0f;
    this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(kind);
  }

  this->state.unknown_11C84->slots[0].height[5] = 0.0f;
  this->state.unknown_11C84->slots[0].height[6] = 0.0f;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C10860
// FUNCTION: fn_20035
void CRagEffect::EarthSpike(char type) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const char* textureName = nullptr;
  if (type == 0) {
    PlayEffectWave(*this, "effect\\wizard_earthspike.wav");
    textureName = "effect\\stone.bmp";
  } else if (type == 2) {
    char waveName[52];
    std::sprintf(waveName, "effect\\EF_IceArrow%d.wav", std::max(rand() % 3, 0) + 1);
    PlayEffectWave(*this, waveName);
    textureName = "effect\\ice.tga";
  }

  CMode* mode = g_modeMgr.GetGameMode();
  mode->SendMsg(122, 1, 0, 0, 0);
  float red = 1.0f;
  float green = 0.24f;
  float blue = 0.2f;
  mode->SendMsg(135, reinterpret_cast<int>(&red), reinterpret_cast<int>(&green), reinterpret_cast<int>(&blue), 0);

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 240;
  this->state.unknown_11C84->pattern = 1;
  this->state.unknown_11C84->pattern |= 2u;
  const int heading = std::max(rand() % 360, 0);
  rand();
  ClientSine(heading);
  ClientCosine(heading);
  this->state.unknown_11C84->deltaPos2 = {0.0f, 10.0f, 0.0f};
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 50, 0) + 300) / 100.0f;
  this->state.unknown_11C84->heightSize = 18.0f;
  this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 20, 0) + 80);
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  EffectPrimitiveState& center = *this->state.unknown_11C84;
  std::memcpy(center.rotTransform, center.transform, sizeof(center.rotTransform));
  this->state.unknown_11C84->speed = 1.0f;
  this->state.unknown_11C84->accel = 0.01f;
  this->state.unknown_11C84->alpha = 250.0f;
  this->state.unknown_11C84->chCnt = 15;
  this->state.unknown_11C84->changePoint = 12;
  this->state.unknown_11C84->changeSpeed = -1.2f;
  this->state.unknown_11C84->changeAccel = 0.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(textureName, 0, false, true, false);

  for (int step = 60; step < 420; step += 60) {
    const int offset = step != 0 ? std::max(rand() % step, 0) : 0;
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 240;
    this->state.unknown_11C84->pattern |= 1u;
    const int angle = std::max(rand() % 360, 0);
    rand();
    const float sine = ClientSine(angle);
    const float cosine = ClientCosine(angle);
    this->state.unknown_11C84->deltaPos2.y = 20.0f;
    this->state.unknown_11C84->deltaPos2.x = sine * 3.5f;
    this->state.unknown_11C84->deltaPos2.z = cosine * -3.5f;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 100, 0) + 400) / 100.0f;
    this->state.unknown_11C84->heightSize = 20.0f;
    this->state.unknown_11C84->longitude = std::min(static_cast<float>(offset + step - 60), 360.0f);
    this->state.unknown_11C84->latitude = 100.0f;
    EffectPrimitiveState& spikeTilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(spikeTilt.transform)->MakeXRotation(GetRadian(static_cast<int>(spikeTilt.latitude)));
    EffectPrimitiveState& spikeTurn = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(spikeTurn.transform)->AppendYRotation(GetRadian(static_cast<int>(spikeTurn.longitude)));
    EffectPrimitiveState& spike = *this->state.unknown_11C84;
    std::memcpy(spike.rotTransform, spike.transform, sizeof(spike.rotTransform));
    this->state.unknown_11C84->speed = 1.5f;
    this->state.unknown_11C84->accel = 0.0f;
    this->state.unknown_11C84->alpha = 250.0f;
    this->state.unknown_11C84->chCnt = 2;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(textureName, 0, false, true, false);
  }
}

// OFFSET: 0x00C39730
// FUNCTION: fn_20102
void CRagEffect::Sightrasher() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt == 0) {
    this->state.parent->SendMsg(nullptr, 109, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    PlayEffectWave(*this, "effect\\wizard_sightrasher.wav");
  }

  if (this->state.stateCnt % 5 == 0 && this->state.stateCnt < 20) {
    for (int i = 0; i < 8; ++i) {
      const int angle = static_cast<int>(static_cast<float>(i) * -45.0f);
      const float sine = ClientSine(angle);
      const float cosine = ClientCosine(angle);
      GetRadian(angle);
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 85 - this->state.stateCnt;
      this->state.unknown_11C84->pattern |= 0x400u;
      const float reach = static_cast<float>(this->state.stateCnt / 5) + 5.0f;
      this->state.unknown_11C84->deltaPos2.x = reach * sine;
      this->state.unknown_11C84->deltaPos2.z = -reach * cosine;
      this->state.unknown_11C84->deltaPos2.y = static_cast<float>(this->state.stateCnt / 3 - 20);
      this->state.unknown_11C84->speed3d.y = 0.0f;
      this->state.unknown_11C84->speed3d.x = static_cast<float>(static_cast<double>(sine));
      this->state.unknown_11C84->speed3d.z = static_cast<float>(static_cast<double>(cosine) * -1.0);
      this->state.unknown_11C84->animSpeed = std::max(rand() % 2, 0) + 3;
      this->state.unknown_11C84->size = 2.5f - static_cast<float>(this->state.stateCnt) / 10.0f;
      this->state.unknown_11C84->alpha = static_cast<float>((75 - this->state.stateCnt) * 2);
      this->state.unknown_11C84->deltaPosAccel = {0.0f, 0.0f, 0.0f};
      this->state.unknown_11C84->changeSpeed3d.x = static_cast<float>(static_cast<double>(sine) * 0.5);
      this->state.unknown_11C84->changeSpeed3d.y = 0.0f;
      this->state.unknown_11C84->changeSpeed3d.z = static_cast<float>(static_cast<double>(cosine) * -0.5);
      this->state.unknown_11C84->changePoint = 35 - this->state.stateCnt;
      this->state.unknown_11C84->changeAccel3d.x = -0.004f;
      this->state.unknown_11C84->changeAccel3d.y = 0.0f;
      this->state.unknown_11C84->changeAccel3d.z = 0.004f;
      this->state.unknown_11C84->fadeOutCnt = 65 - this->state.stateCnt;
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\Sight.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\Sight.act");
      Owner(this->state.unknown_11C84).InitSegment();
    }

    for (int i = 0; i < 8; ++i) {
      const int angle = static_cast<int>(static_cast<float>(i) * -45.0f);
      const float sine = ClientSine(angle);
      const float cosine = ClientCosine(angle);
      GetRadian(angle);
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 85 - this->state.stateCnt;
      this->state.unknown_11C84->pattern |= 0x400u;
      this->state.unknown_11C84->deltaPos2.y = 0.0f;
      const float reach = static_cast<float>(this->state.stateCnt / 5) + 5.0f;
      this->state.unknown_11C84->deltaPos2.x = reach * sine;
      this->state.unknown_11C84->deltaPos2.z = -reach * cosine;
      this->state.unknown_11C84->speed3d.y = 0.0f;
      this->state.unknown_11C84->speed3d.x = static_cast<float>(static_cast<double>(sine));
      this->state.unknown_11C84->speed3d.z = static_cast<float>(static_cast<double>(cosine) * -1.0);
      this->state.unknown_11C84->animSpeed = 3;
      this->state.unknown_11C84->size = 1.0f - static_cast<float>(this->state.stateCnt) / 30.0f;
      this->state.unknown_11C84->alpha = static_cast<float>((75 - this->state.stateCnt) * 2);
      this->state.unknown_11C84->changeSpeed3d.x = static_cast<float>(static_cast<double>(sine) * 0.5);
      this->state.unknown_11C84->changeSpeed3d.z = static_cast<float>(static_cast<double>(cosine) * -0.5);
      this->state.unknown_11C84->changePoint = 35 - this->state.stateCnt;
      this->state.unknown_11C84->changeAccel3d.x = -0.004f;
      this->state.unknown_11C84->changeAccel3d.z = 0.004f;
      this->state.unknown_11C84->fadeOutCnt = 65 - this->state.stateCnt;
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "Shadow.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "Shadow.act");
      Owner(this->state.unknown_11C84).InitSegment();
    }
  }

  if (this->state.stateCnt != 10) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 30;
  this->state.unknown_11C84->latitude = 90.0f;
  this->state.unknown_11C84->radiusSpeed = 2.5f;
  EffectPrimitiveState& ring = *this->state.unknown_11C84;
  ring.radiusAccel = -(ring.radiusSpeed / ring.duration) / 1.5f;
  this->state.unknown_11C84->innerSize = -10.0f;
  this->state.unknown_11C84->alpha = 150.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  Owner(this->state.unknown_11C84).InitSegment();
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);
}

// OFFSET: 0x00C35610
// FUNCTION: fn_20090
void CRagEffect::RingBurstSparks() {
  if (this->state.stateCnt % 20 != 0 || this->state.stateCnt >= this->state.level * 20) {
    return;
  }

  const int facing = static_cast<int>(180.0f - this->state.parent->render.angle);
  const int direction = facing < 0 ? facing + 360 : facing;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 15;
  this->state.unknown_11C84->longitude = static_cast<float>(direction);
  this->state.unknown_11C84->latitude = -90.0f;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->speed = 0.7f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  slow.accel = -(slow.speed / slow.duration) * 0.5f;
  this->state.unknown_11C84->outerSize = static_cast<float>(this->state.stateCnt * 0.1 + 11.0);
  this->state.unknown_11C84->innerSize = static_cast<float>(this->state.stateCnt * 0.1 + 6.0);
  this->state.unknown_11C84->heightSize = 2.5f;
  const int lifetime = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = lifetime - lifetime / 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);

  if (this->state.stateCnt <= 0) {
    return;
  }

  for (int count = 4; count != 0; --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 80, 0) + direction - 40);
    this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 100, 0) - 50);
    EffectPrimitiveState& yaw = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(yaw.transform)->MakeYRotation(GetRadian(static_cast<int>(yaw.longitude)));
    EffectPrimitiveState& pitch = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(pitch.transform)->AppendXRotation(GetRadian(static_cast<int>(pitch.latitude)));
    EffectPrimitiveState& spark = *this->state.unknown_11C84;
    spark.deltaPos3 = RotatedForward(spark.transform, static_cast<float>(std::max(rand() % 4, 0) + 4));
    EffectPrimitiveState& moving = *this->state.unknown_11C84;
    moving.deltaPos2.x = moving.deltaPos3.x + 0.0f;
    moving.deltaPos2.y = moving.deltaPos3.y - 10.0f;
    moving.deltaPos2.z = moving.deltaPos3.z + 0.0f;
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 90, 0) + 60) / 100.0f;
    this->state.unknown_11C84->duration = std::max(rand() % 24, 0) + 6;
    EffectPrimitiveState& fading = *this->state.unknown_11C84;
    fading.accel = -(fading.speed / fading.duration) * 0.5f;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 100, 0) + 60) / 100.0f;
    EffectPrimitiveState& shrinking = *this->state.unknown_11C84;
    shrinking.sizeSpeed = -(shrinking.size / shrinking.duration);
    this->state.unknown_11C84->fadeOutCnt = 0;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->numSegment = 3;
    Owner(this->state.unknown_11C84).InitSegment();
  }

  for (int count = 4; count != 0; --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 6, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 80, 0) + direction + 140);
    this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 100, 0) - 50);
    EffectPrimitiveState& yaw = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(yaw.transform)->MakeYRotation(GetRadian(static_cast<int>(yaw.longitude)));
    EffectPrimitiveState& pitch = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(pitch.transform)->AppendXRotation(GetRadian(static_cast<int>(pitch.latitude)));
    EffectPrimitiveState& spark = *this->state.unknown_11C84;
    spark.deltaPos3 = RotatedForward(spark.transform, static_cast<float>(std::max(rand() % 4, 0) + 4));
    EffectPrimitiveState& moving = *this->state.unknown_11C84;
    moving.deltaPos2.x = moving.deltaPos3.x + 0.0f;
    moving.deltaPos2.y = moving.deltaPos3.y - 10.0f;
    moving.deltaPos2.z = moving.deltaPos3.z + 0.0f;
    EffectPrimitiveState& facingYaw = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(facingYaw.transform)->MakeYRotation(GetRadian(static_cast<int>(facingYaw.longitude)));
    this->state.unknown_11C84->duration = std::max(rand() % 24, 0) + 6;
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 90, 0) + 60) / 100.0f;
    EffectPrimitiveState& fading = *this->state.unknown_11C84;
    fading.accel = -(fading.speed / fading.duration) * 0.5f;
    this->state.unknown_11C84->gravSpeed = -static_cast<float>(std::max(rand() % 90, 0) + 30) / 100.0f;
    EffectPrimitiveState& falling = *this->state.unknown_11C84;
    const float fall = falling.gravSpeed / falling.duration;
    falling.gravAccel = -(fall + fall);
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 100, 0) + 60) / 100.0f;
    EffectPrimitiveState& shrinking = *this->state.unknown_11C84;
    shrinking.sizeSpeed = -(shrinking.size / shrinking.duration);
    this->state.unknown_11C84->fadeOutCnt = 0;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->numSegment = 3;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C7AB90
// FUNCTION: fn_20357
void CRagEffect::WaterBubble(int) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 142, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 117;
  this->state.unknown_11C84->textures = new CTexture*[this->state.unknown_11C84->totalTexture];
  char name[64];

  for (int index = 0; index < 30; ++index) {
    std::sprintf(name, "effect\\W_C_water_%02d.tga", index);
    this->state.unknown_11C84->textures[index] = g_texMgr.Load(name, 0, false, true, false);
  }

  for (int index = 0; index < 30; ++index) {
    std::sprintf(name, "effect\\W_B_water_%02d.tga", index);
    this->state.unknown_11C84->textures[index + 30] = g_texMgr.Load(name, 0, false, true, false);
  }

  for (int index = 0; index < 27; ++index) {
    std::sprintf(name, "effect\\W_bubble%02d.tga", index + 1);
    this->state.unknown_11C84->textures[index + 60] = g_texMgr.Load(name, 0, false, true, false);
  }

  for (int index = 0; index < 30; ++index) {
    std::sprintf(name, "effect\\W_A_water_%02d.tga", index);
    this->state.unknown_11C84->textures[index + 87] = g_texMgr.Load(name, 0, false, true, false);
  }

  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].alphaB = 230;
  this->state.unknown_11C84->slots[0].vecBNow.z = 0.0f;

  if (this->state.param[0] < 20.0f) {
    this->state.unknown_11C84->slots[0].distance = 40.0f;
  } else {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0];
  }

  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(this->state.param[1] >= 360.0f ? 0 : static_cast<int>(this->state.param[1]));
}

// OFFSET: 0x00C134B0
// FUNCTION: fn_20044
void CRagEffect::FireArrow() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 12) {
    char waveName[52];
    std::sprintf(waveName, "effect\\EF_FireArrow%d.wav", std::max(rand() % 3, 0) + 1);
    PlayEffectWave(*this, waveName);
    this->state.count *= 10;
  }

  const int frame = this->state.stateCnt;
  if (frame >= 12 && (frame - 12) % 10 == 0 && frame - 12 < static_cast<int>(this->state.count)) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 70;
    this->state.unknown_11C84->pattern = 0x14;
    EffectPrimitiveState& arrow = *this->state.unknown_11C84;
    const int spreadZ = std::max(rand() % 10, 0);
    const int spreadX = std::max(rand() % 10, 0);
    arrow.deltaPos2.y = -60.0f;
    arrow.deltaPos2.x = static_cast<float>(spreadX + 25);
    arrow.deltaPos2.z = static_cast<float>(spreadZ + 15);
    const int aimZ = std::max(rand() % 4, 0);
    const int aimX = std::max(rand() % 4, 0);
    const float heading = CRenderObject::Heading(
      this->state.unknown_11C84->deltaPos2.x, this->state.unknown_11C84->deltaPos2.z, static_cast<float>(aimX - 2), static_cast<float>(aimZ - 2)
    );
    this->state.unknown_11C84->longitude = -heading;
    this->state.longitude = -heading;
    const float dx = std::fabs(this->state.unknown_11C84->deltaPos2.x);
    const float dz = std::fabs(this->state.unknown_11C84->deltaPos2.z);
    const float run = SquareRoot(dx * dx + dz * dz);
    const float pitch = CRenderObject::Heading(-std::fabs(this->state.unknown_11C84->deltaPos2.y), run, 0.0f, 0.0f);
    this->state.unknown_11C84->latitude = pitch;
    this->state.latitude = pitch;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    EffectPrimitiveState& turn = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
    this->state.unknown_11C84->speed = -2.75f;
    this->state.unknown_11C84->widthSize = 14.0f;
    this->state.unknown_11C84->heightSize = 3.5f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    this->state.unknown_11C84->totalTexture = 6;
    this->state.unknown_11C84->animSpeed = 1;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\불화살1.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\불화살2.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\불화살3.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\불화살4.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\불화살5.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\불화살6.tga", 0, false, true, false);
    this->state.unknown_11C84->slots[0].life = 0;
  }

  if (this->state.stateCnt > 0 && this->state.stateCnt % 4 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = std::max(rand() % 24, 0) + 6;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 40, 0)) + (this->state.longitude - 20.0f);
    this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 20, 0)) + (this->state.latitude - 10.0f);
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    EffectPrimitiveState& turn = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
    this->state.unknown_11C84->deltaPos2.x = this->state.deltaPos[0];
    this->state.unknown_11C84->deltaPos2.y = this->state.deltaPos[1];
    this->state.unknown_11C84->deltaPos2.z = this->state.deltaPos[2];
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 90, 0) + 60) / 100.0f;
    EffectPrimitiveState& slowing = *this->state.unknown_11C84;
    slowing.accel = -(slowing.speed / slowing.duration) / 1.5f;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 30, 0) + 20) / 100.0f;
    EffectPrimitiveState& shrinking = *this->state.unknown_11C84;
    shrinking.sizeSpeed = -(shrinking.size / shrinking.duration);
    this->state.unknown_11C84->fadeOutCnt = 0;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle4.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle4.act");
    Owner(this->state.unknown_11C84).InitSegment();
  }

  if (this->state.flag == 0) {
    return;
  }

  this->state.flag = 0;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 30;
  this->state.unknown_11C84->latitude = 90.0f;
  this->state.unknown_11C84->radiusSpeed = 1.2f;
  EffectPrimitiveState& ring = *this->state.unknown_11C84;
  const float slowdown = -(ring.radiusSpeed / (ring.duration + 40));
  ring.radiusAccel = slowdown + slowdown;
  this->state.unknown_11C84->innerSize = 10.0f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  Owner(this->state.unknown_11C84).InitSegment();
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);
}

// GLOBAL: gl_24762
static int g_blitzBeatCount;

// OFFSET: 0x00C0CAC0
// FUNCTION: fn_20020
void CRagEffect::BlitzBeat() {
  if (this->state.stateCnt == 5) {
    PlayEffectWave(*this, "effect\\hunter_blitzbeat_1st.wav");
  }

  if (this->state.stateCnt == 30) {
    PlayEffectWave(*this, "effect\\hunter_blitzbeat.wav");
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    g_blitzBeatCount = this->state.stateCnt;
    for (int count = 10; count != 0; --count) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 20;
      this->state.unknown_11C84->renderFlag |= 0x200u;
      this->state.unknown_11C84->longitude = this->state.parent->render.angle;
      EffectPrimitiveState& facing = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(facing.transform)->MakeYRotation(GetRadian(static_cast<int>(facing.longitude)));
      const float spin = static_cast<float>(std::max(rand() % 360, 0));
      const float sine = ClientSine(static_cast<int>(spin));
      const float cosine = ClientCosine(static_cast<int>(spin));
      EffectPrimitiveState& moving = *this->state.unknown_11C84;
      moving.speed3d = RotatedForward(moving.transform, 15.0f);
      EffectPrimitiveState& origin = *this->state.unknown_11C84;
      origin.deltaPos.x = origin.deltaPos.x + origin.speed3d.x;
      origin.deltaPos.y = origin.speed3d.y + origin.deltaPos.y;
      origin.deltaPos.z = origin.speed3d.z + origin.deltaPos.z;
      this->state.unknown_11C84->deltaPos2.y = -7.0f;
      this->state.unknown_11C84->deltaPos2.x = sine * 3.5f;
      this->state.unknown_11C84->deltaPos2.z = cosine * -3.5f;
      this->state.unknown_11C84->speed = -1.2f;
      this->state.unknown_11C84->accel = -0.1f;
      this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 40, 0) + 20) / 10.0f;
      this->state.unknown_11C84->heightSize = 0.2f;
      this->state.unknown_11C84->widthSpeed = 0.1f;
      this->state.unknown_11C84->widthAccel = -0.01f;
      this->state.unknown_11C84->alpha = 0.0f;
      this->state.unknown_11C84->maxAlpha = 250.0f;
      this->state.unknown_11C84->changePoint = 10;
      this->state.unknown_11C84->changeSpeed = -0.3f;
      this->state.unknown_11C84->changeAccel = 0.001f;
      this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha * 0.5f;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 2;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
      this->state.unknown_11C84->slots[0].life = 0;
    }
  }

  if (this->state.stateCnt <= 35 || this->state.stateCnt % 3 != 0 || g_blitzBeatCount >= static_cast<int>(this->state.count)) {
    return;
  }

  const float facing = this->state.parent->render.angle;
  const int driftX = rand();
  const int driftY = rand();
  rand();
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 12;
  this->state.unknown_11C84->longitude = static_cast<float>(static_cast<int>(facing));
  this->state.unknown_11C84->latitude = -90.0f;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
  EffectPrimitiveState& thrust = *this->state.unknown_11C84;
  const float* m = thrust.transform;
  thrust.speed3d = {
    (m[0] * 0.0f - m[3] * 2.0f) + m[6] + m[9],
    (m[1] * 0.0f - m[4] * 2.0f) + m[7] + m[10],
    (m[2] * 0.0f - m[5] * 2.0f) + m[8] + m[11]
  };
  EffectPrimitiveState& origin = *this->state.unknown_11C84;
  origin.deltaPos.x = origin.speed3d.x + origin.deltaPos.x;
  origin.deltaPos.y = origin.speed3d.y + origin.deltaPos.y;
  origin.deltaPos.z = origin.speed3d.z + origin.deltaPos.z;
  this->state.unknown_11C84->deltaPos2.z = 0.0f;
  this->state.unknown_11C84->deltaPos2.x = static_cast<float>((std::max(driftX % 10, 0) + 1) / 5);
  this->state.unknown_11C84->deltaPos2.y = static_cast<float>((std::max(driftY % 20, 0) + 1) / 2 - 12);
  this->state.unknown_11C84->speed = 0.4f;
  EffectPrimitiveState& slowing = *this->state.unknown_11C84;
  slowing.accel = -(slowing.speed / slowing.duration) * 0.5f;
  this->state.unknown_11C84->outerSize = 1.5f;
  this->state.unknown_11C84->innerSize = 0.5f;
  this->state.unknown_11C84->heightSize = 3.5f;
  const int lifetime = this->state.unknown_11C84->duration;
  this->state.unknown_11C84->fadeOutCnt = lifetime - lifetime / 2;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = g_texMgr.Load("effect\\magic_green.tga", 0, false, true, false);
  ++g_blitzBeatCount;
  this->state.unknown_11C84->textures[0] = texture;
}

// OFFSET: 0x00C5C450
// FUNCTION: fn_20227
void CRagEffect::Map_MagicZone(const char* name, char type) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 26, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (type == 4 || type == 14) {
    this->state.unknown_11C84->size = 111.0f;
  } else if (type == 5) {
    this->state.unknown_11C84->size = 112.0f;
  } else {
    this->state.unknown_11C84->size = 110.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  if (type == 2 || type == 3) {
    this->state.unknown_11C84->slots[0].distance = 40.0f;
  } else if (type == 4 || type == 14) {
    if (this->state.param[0] > 0.0f) {
      this->state.unknown_11C84->slots[0].distance = this->state.param[0];
    } else {
      this->state.unknown_11C84->slots[0].distance = 40.0f;
    }
  } else {
    this->state.unknown_11C84->slots[0].distance = 30.0f;
  }

  if (type == 4 || type == 5 || type == 14) {
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 250;
    this->state.unknown_11C84->slots[0].height[0] = -0.5f;
  } else if (type == 0 || type == 2) {
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 200;
    this->state.unknown_11C84->slots[0].height[0] = -0.5f;
  } else {
    this->state.unknown_11C84->slots[0].rotStart = 358;
    this->state.unknown_11C84->slots[0].alphaB = 50;
    this->state.unknown_11C84->slots[0].height[0] = -0.4f;
  }

  this->state.unknown_11C84->slots[0].height[4] = static_cast<float>(type);
}

// OFFSET: 0x00C4AD80
// FUNCTION: fn_20172
void CRagEffect::Demonstration() {
  if (this->state.stateCnt != 0) {
    return;
  }

  this->render.position = this->state.parent->render.position;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 51, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 0.0f;
  this->state.unknown_11C84->size = 1.2f;
  this->state.unknown_11C84->slots[0].vecBPre = this->render.position;
  this->state.unknown_11C84->slots[0].vecBPre.y -= 1.0f;
  this->state.unknown_11C84->slots[0].flag1[4] = 0;
  this->state.unknown_11C84->alpha = 200.0f;

  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ub370\ubaac\uc2a4\ud2b8\ub808\uc774\uc158.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ub370\ubaac\uc2a4\ud2b8\ub808\uc774\uc158.act");

  this->state.unknown_11C84->animSpeed = 4;
  Owner(this->state.unknown_11C84).SetAction(0, 16, 0);
  Owner(this->state.unknown_11C84).InitSegment();

  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C5CEA0
// FUNCTION: fn_20231
void CRagEffect::NpcStop(char type) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->render.position = this->state.parent->render.position;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 51, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag |= 0x200;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = 0.0f;
  this->state.unknown_11C84->size = 1.0f;
  this->state.unknown_11C84->slots[0].vecBPre = this->render.position;

  if (type == 2) {
    this->state.unknown_11C84->slots[0].flag1[4] = 1;
    this->state.unknown_11C84->alpha = 100.0f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\cconfine.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\cconfine.act");
    this->state.unknown_11C84->animSpeed = 12;
    this->state.unknown_11C84->repeatAnim = 0;
    this->state.unknown_11C84->pattern = 0x800;
  } else {
    this->state.unknown_11C84->slots[0].vecBPre.y -= 7.0f;
    this->state.unknown_11C84->slots[0].flag1[4] = 1;
    this->state.unknown_11C84->alpha = 200.0f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uc2a4\ud1b1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uc2a4\ud1b1.act");
    this->state.unknown_11C84->animSpeed = 4;
    Owner(this->state.unknown_11C84).SetAction(0, 16, 0);
  }

  Owner(this->state.unknown_11C84).InitSegment();
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C47380
// FUNCTION: fn_20156
void CRagEffect::Bubble_Drop() {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  float count = this->state.param[0];
  if (count <= 1.0f) {
    count = 50.0f;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 127, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\썬더스톰파티클.tga", 0, false, true, false);

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 1.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].vecBPre.x = 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.y = -count;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].height[1] = 1.0f;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<short>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 2;
  this->state.unknown_11C84->slots[0].flag1[4] = static_cast<char>(static_cast<int>(count));

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C52490
// FUNCTION: fn_20194
void CRagEffect::Ghost(char type) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt > 9) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 84, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->size = 1.5f;
  this->state.unknown_11C84->slots[0].vecBNow = this->render.position;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].riseAngle = 0;

  if (type == 2) {
    this->state.unknown_11C84->slots[0].flag1[0] = 2;
    if (this->state.stateCnt == 0) {
      this->state.unknown_11C84->slots[0].height[1] = 250.0f;
    } else {
      this->state.unknown_11C84->slots[0].height[1] = static_cast<float>((10 - this->state.stateCnt) * 3);
    }
  } else {
    this->state.unknown_11C84->slots[0].flag1[0] = 1;
    this->state.unknown_11C84->slots[0].height[1] = static_cast<float>((10 - this->state.stateCnt) * 3);
  }

  this->state.unknown_11C84->slots[0].flag1[2] = type;
  this->state.unknown_11C84->slots[0].distance = 16.0f;
  this->state.unknown_11C84->slots[0].maxHeight = 4.0f;
  this->state.unknown_11C84->alpha = 0.0f;

  if (type == 0) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uc720\ub839.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uc720\ub839.act");
  } else if (type > 0) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ubc15\uc950.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ubc15\uc950.act");
  }

  this->state.unknown_11C84->animSpeed = 2;
  this->state.unknown_11C84->renderFlag |= 0x200;
  Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
  Owner(this->state.unknown_11C84).InitSegment();
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C45FE0
// FUNCTION: fn_20152
void CRagEffect::Bottom_Out(const char* name, char kind) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 49, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  const int count = kind != 1 ? 2 : 1;
  for (int i = 0; i < count; ++i) {
    EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[i];
    slot.life = 1;
    slot.process = std::max(rand() % 11, 0);

    if (kind == 0) {
      slot.distance = static_cast<float>(std::max(rand() % 6, 0)) + 5.0f;
    } else if (kind == 1) {
      slot.distance = static_cast<float>(std::max(rand() % 20, 0)) * 0.1f + 3.0f;
    }

    slot.height[8] = 0.0f;
    slot.alphaB = 0;
    slot.vecBNow.x = this->render.position.x;
    slot.vecBNow.z = this->render.position.z;

    if (kind == 0) {
      slot.vecBPre.y = this->render.position.y;
    } else if (kind == 1) {
      slot.vecBPre.y = this->render.position.y - 5.0f;
    }

    slot.fullDisplayAngle = 45;
    slot.flag1[0] = 0;
    slot.flag1[2] = 2;
    slot.height[7] = 0.0f;
    slot.flag1[1] = kind;
  }
}



// OFFSET: 0x00C66C90
// FUNCTION: fn_20276
void CRagEffect::TANJI2() {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 97, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\blue_ivy.bmp", 0, false, true, false);

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};

  const float dx = this->state.deltaPos[0] - this->render.position.x;
  const float dz = this->state.deltaPos[2] - this->render.position.z;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = -15;
  this->state.unknown_11C84->slots[0].maxHeight = SquareRoot(dx * dx + dz * dz);
  this->state.unknown_11C84->slots[0].distance = static_cast<float>(this->state.count) * 0.4f + 2.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 5;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 8.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].height[1] = 2.0f;
  this->state.unknown_11C84->slots[0].height[2] =
    (this->state.unknown_11C84->slots[0].height[1] / this->state.unknown_11C84->slots[0].maxHeight) *
    (this->state.deltaPos[1] - this->render.position.y);
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[1] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 1;
  this->state.unknown_11C84->slots[0].height[7] = 0.0f;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C674A0
// FUNCTION: fn_20279
void CRagEffect::TEIHIT2() {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 45, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_center.bmp", 0, false, true, false);

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  Owner(this->state.unknown_11C84).render.position = this->render.position;

  for (int i = 0; i < 4; ++i) {
    EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[i];
    slot.life = 1;
    slot.process = std::max(rand() % 10, 0) - 40;
    slot.distance = static_cast<float>(std::max(rand() % 16, 0)) * 0.05f + 0.1f;
    slot.height[8] = 0.0f;
    slot.alphaB = 0;
    slot.vecBNow.x = this->state.deltaPos[0];
    slot.vecBNow.z = this->state.deltaPos[2];
    slot.vecBNow.y = this->state.deltaPos[1] - 8.0f;
    slot.rotStart = 0;
    slot.height[1] = static_cast<float>(std::max(rand() % 50, 0)) * 0.01f + 0.75f;
    slot.height[2] = static_cast<float>(std::max(rand() % 200, 0)) * 0.01f - 1.0f;
    slot.fullDisplayAngle = 0;
    slot.flag1[0] = 0;
    slot.flag1[1] = 0;
    slot.flag1[2] = 2;
    slot.height[7] = 0.0f;
  }
}

// OFFSET: 0x00C64CB0
// FUNCTION: fn_20265
void CRagEffect::ShieldBoomerang3(const char* name) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\cru_shield boomerang.wav");
  } else if (this->state.stateCnt != 3 && this->state.stateCnt != 6 && this->state.stateCnt != 9 && this->state.stateCnt != 12) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 116, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 8.0f;
  this->state.unknown_11C84->slots[0].distance = 5.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 25;
  this->state.unknown_11C84->slots[0].rotStart = std::max(rand() % 360, 0);
  this->state.unknown_11C84->slots[0].height[1] = 2.5f;
  this->state.unknown_11C84->slots[0].vecBNow.x -= this->state.unknown_11C84->slots[0].height[1] * 15.0f * g_cosTable[this->state.unknown_11C84->slots[0].rotStart];
  this->state.unknown_11C84->slots[0].vecBNow.z -= this->state.unknown_11C84->slots[0].height[1] * 15.0f * g_sinTable[this->state.unknown_11C84->slots[0].rotStart];
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 0;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C5DEC0
// FUNCTION: fn_20237
void CRagEffect::ParticleTime(const char* name, char kind) {
  this->render.position = this->state.parent->render.position;

  const int minCount = kind == 2 ? 40 : 0;
  if (static_cast<int>(this->state.stateCnt) < minCount) {
    return;
  }

  const int maxCount = kind == 2 ? 120 : 80;
  if (static_cast<int>(this->state.stateCnt) > maxCount) {
    return;
  }

  if ((this->state.stateCnt & 3) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 2;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pok1.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\pok3.tga", 0, false, true, false);

  this->state.unknown_11C84->size = 0.0f;
  Owner(this->state.unknown_11C84).render.position = this->render.position;

  for (int i = 0; i < 4; ++i) {
    EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[i];
    slot.life = 1;
    slot.process = 0;
    slot.distance = static_cast<float>(std::max(rand() % 10, 0)) * 0.1f + 2.0f;
    slot.height[8] = 0.0f;
    slot.alphaB = 0;
    slot.vecBNow.x = static_cast<float>(std::max(rand() % 11, 0)) + this->render.position.x - 5.0f;
    slot.vecBNow.z = static_cast<float>(std::max(rand() % 11, 0)) + this->render.position.z - 5.0f;
    slot.vecBNow.y = this->render.position.y - 6.0f;
    slot.rotStart = 0;
    slot.height[1] = 0.0f;
    slot.height[2] = static_cast<float>(std::max(rand() % 20, 0)) * 0.01f + 0.1f;
    slot.fullDisplayAngle = 0;
    slot.flag1[0] = 0;
    slot.flag1[1] = 1;

    if (kind == 0) {
      slot.flag1[2] = 11;
    } else if (kind == 1 || kind == 2) {
      slot.flag1[2] = 1;
    } else if (kind == 3) {
      slot.flag1[2] = 13;
    }

    slot.height[7] = 0.0f;
  }
}


// OFFSET: 0x00C68B20
// FUNCTION: fn_20286
void CRagEffect::ThrowItem(const char* name, char kind) {
  this->render.position = this->state.parent->render.position;

  int targetFrame = 0;
  if (kind == 10) {
    targetFrame = 5;
  } else if (kind == 11) {
    targetFrame = 10;
  }

  if (this->state.stateCnt != targetFrame) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 53, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};

  const float dx = this->state.deltaPos[0] - this->render.position.x;
  const float dz = this->state.deltaPos[2] - this->render.position.z;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].maxHeight = SquareRoot(dx * dx + dz * dz);

  if (kind == 2) {
    this->state.unknown_11C84->slots[0].distance = 2.0f;
  } else if (kind == 3) {
    this->state.unknown_11C84->slots[0].distance = 4.0f;
  } else {
    this->state.unknown_11C84->slots[0].distance = 3.0f;
  }

  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 255;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 8.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;

  float step = 2.0f;
  if (kind == 3) {
    step = 3.0f;
  } else if (kind != 2) {
    step = this->state.unknown_11C84->slots[0].maxHeight / 25.0f;
  }
  this->state.unknown_11C84->slots[0].height[1] = step;
  this->state.unknown_11C84->slots[0].height[2] =
    (this->state.unknown_11C84->slots[0].height[1] / this->state.unknown_11C84->slots[0].maxHeight) *
    (this->state.deltaPos[1] - this->render.position.y);

  if (kind == 3) {
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 330;
    this->state.unknown_11C84->slots[0].flag1[0] = 0;
    this->state.unknown_11C84->slots[0].height[7] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 15;
  } else {
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 0;
    this->state.unknown_11C84->slots[0].flag1[0] = 0;
    this->state.unknown_11C84->slots[0].height[7] = 0.0f;

    if (kind == 0) {
      this->state.unknown_11C84->slots[0].flag1[2] = 8;
    } else if (kind == 1) {
      this->state.unknown_11C84->slots[0].flag1[2] = 9;
      this->state.unknown_11C84->slots[0].flag1[3] = static_cast<char>(this->state.count);
    } else if (kind == 2) {
      this->state.unknown_11C84->slots[0].flag1[2] = 0;
    } else if (kind >= 10) {
      this->state.unknown_11C84->slots[0].flag1[2] = 9;
      this->state.unknown_11C84->slots[0].flag1[3] = 4;
    }
  }

  for (int stepIdx = 0; stepIdx < 100; ++stepIdx) {
    if (this->state.unknown_11C84->slots[0].maxHeight < static_cast<float>(stepIdx) * this->state.unknown_11C84->slots[0].height[1]) {
      this->state.unknown_11C84->slots[0].flag1[1] = static_cast<char>(stepIdx);
      break;
    }
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C45B90
// FUNCTION: fn_20151
void CRagEffect::Bottom_Music(const char* name, char kind) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 47, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);

  const char* textureName = name;
  if (kind == 5) {
    const int pick = std::max(rand() % 3, 0);
    if (pick == 0) {
      textureName = "effect\\레드젬스톤.bmp";
    } else if (pick == 1) {
      textureName = "effect\\블루젬스톤.bmp";
    } else {
      textureName = "effect\\옐로우젬스톤.bmp";
    }
  } else if (kind == 2) {
    const int pick = std::max(rand() % 2, 0);
    if (pick == 0) {
      textureName = "effect\\melody_a.bmp";
    } else {
      textureName = "effect\\melody_b.bmp";
    }
  } else if (kind == 8) {
    const int pick = std::max(rand() % 8, 0);
    if (pick == 0) {
      textureName = "effect\\spell_01.bmp";
    } else if (pick == 1) {
      textureName = "effect\\spell_02.bmp";
    } else if (pick == 2) {
      textureName = "effect\\spell_03.bmp";
    } else if (pick == 3) {
      textureName = "effect\\spell_04.bmp";
    } else if (pick == 4) {
      textureName = "effect\\spell_05.bmp";
    } else if (pick == 5) {
      textureName = "effect\\spell_06.bmp";
    } else if (pick == 6) {
      textureName = "effect\\spell_07.bmp";
    } else {
      textureName = "effect\\spell_08.bmp";
    }
  }

  this->state.unknown_11C84->textures[0] = g_texMgr.Load(textureName, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;

  if (kind == 1) {
    this->state.unknown_11C84->slots[0].distance = 5.0f;
  } else if (kind == 4) {
    this->state.unknown_11C84->slots[0].distance = 15.0f;
  } else if (kind == 6) {
    this->state.unknown_11C84->slots[0].distance = 8.0f;
  } else if (kind == 9) {
    this->state.unknown_11C84->slots[0].distance = 1.5f;
  } else {
    this->state.unknown_11C84->slots[0].distance = 3.0f;
  }

  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 200;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 6.0f;
  this->state.unknown_11C84->slots[0].rotStart = std::max(rand() % 360, 0);
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 45;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 2;
  this->state.unknown_11C84->slots[0].height[7] = 0.0f;

  if (kind == 4) {
    this->state.unknown_11C84->slots[0].height[9] = 10.0f;
    return;
  }

  if (kind == 2) {
    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[3].life = 1;
    this->state.unknown_11C84->slots[3].alphaB = 0;
    return;
  }

  if (kind == 3) {
    this->state.unknown_11C84->slots[0].flag1[2] = 3;
    return;
  }

  if (kind == 5 || kind == 6) {
    this->state.unknown_11C84->slots[0].flag1[2] = 4;
  } else if (kind == 7) {
    this->state.unknown_11C84->slots[0].flag1[2] = 7;
  } else if (kind == 9) {
    this->state.unknown_11C84->slots[0].flag1[2] = 9;
    this->state.unknown_11C84->slots[0].flag1[3] = 6;
  }
}

// OFFSET: 0x00C1AF10
// FUNCTION: fn_20070
void CRagEffect::IceArrow() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 12) {
    char soundBuffer[64];
    sprintf(soundBuffer, "effect\\EF_IceArrow%d.wav", std::max(rand() % 3, 0) + 1);
    PlayEffectWave(*this, soundBuffer);
    this->state.count *= 10;
  }

  if (this->state.stateCnt >= 12) {
    const int frameOffset = this->state.stateCnt - 12;
    if (frameOffset % 10 == 0 && frameOffset < static_cast<int>(this->state.count)) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 70;
      this->state.unknown_11C84->pattern = 20;

      this->state.unknown_11C84->deltaPos2.y = -60.0f;
      this->state.unknown_11C84->deltaPos2.x = static_cast<float>(std::max(rand() % 10, 0) + 25);
      this->state.unknown_11C84->deltaPos2.z = static_cast<float>(std::max(rand() % 10, 0) + 15);

      const int rx = std::max(rand() % 4, 0) - 2;
      const int rz = std::max(rand() % 4, 0) - 2;
      const float angleY = -CRenderObject::Heading(
        this->state.unknown_11C84->deltaPos2.x,
        this->state.unknown_11C84->deltaPos2.z,
        static_cast<float>(rx),
        static_cast<float>(rz)
      );
      this->state.unknown_11C84->longitude = angleY;
      this->state.longitude = angleY;

      const float distXZ = SquareRoot(
        this->state.unknown_11C84->deltaPos2.x * this->state.unknown_11C84->deltaPos2.x +
        this->state.unknown_11C84->deltaPos2.z * this->state.unknown_11C84->deltaPos2.z
      );
      const float angleX = CRenderObject::Heading(-this->state.unknown_11C84->deltaPos2.y, distXZ, 0.0f, 0.0f);
      this->state.unknown_11C84->latitude = angleX;
      this->state.latitude = angleX;

      matrix rotation;
      rotation.MakeXRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->latitude)));
      rotation.AppendYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));

      this->state.unknown_11C84->speed = -2.75f;
      this->state.unknown_11C84->widthSize = 11.5f;
      this->state.unknown_11C84->heightSize = 3.8f;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\icearrow.tga", 0, false, true, false);
      this->state.unknown_11C84->slots[0].life = 0;
    }
  }

  if (this->state.flag != 0) {
    this->state.flag = 0;
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 30;
    this->state.unknown_11C84->latitude = 90.0f;
    this->state.unknown_11C84->radiusSpeed = 1.2f;
    const float accel = -(this->state.unknown_11C84->radiusSpeed / static_cast<float>(this->state.unknown_11C84->duration + 40));
    this->state.unknown_11C84->radiusAccel = accel + accel;
    this->state.unknown_11C84->innerSize = 10.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C153D0
// FUNCTION: fn_20050
void CRagEffect::Flasher() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\hunter_flasher.wav");
  }

  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  g_renderer->viewport.Projection().Project(
    &this->state.parent->render.position.x,
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values,
    vertex
  );
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 70;
    this->state.unknown_11C84->pattern = 65;
    this->state.unknown_11C84->renderFlag |= 2;
    this->state.unknown_11C84->deltaPos2.y = -10.0f;
    this->state.unknown_11C84->widthSize = 20.0f;
    this->state.unknown_11C84->heightSize = 20.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 200.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 6.0f;
    this->state.unknown_11C84->chRadiSpeed = 1;

    this->state.unknown_11C84->chPoint.push_back(40.0f);
    this->state.unknown_11C84->chVal1.push_back(120.0f);
    this->state.unknown_11C84->chVal2.push_back(200.0f);

    this->state.unknown_11C84->chPoint.push_back(100.0f);
    this->state.unknown_11C84->chVal1.push_back(0.0f);
    this->state.unknown_11C84->chVal2.push_back(80.0f);

    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\thunder_center.bmp", 0, false, true, false);

    for (int i = 0; i < 20; ++i) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 3, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 70;
      this->state.unknown_11C84->deltaPos2.y = -40.0f;
      this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->longSpeed = static_cast<float>(std::max(rand() % 60, 0) + 10) / 10.0f;
      const float accel = -(this->state.unknown_11C84->longSpeed / static_cast<float>(this->state.unknown_11C84->duration));
      this->state.unknown_11C84->longAccel = accel / 1.5f;
      this->state.unknown_11C84->arcAngle = static_cast<float>(std::max(rand() % 25, 0) + 5) / 10.0f;
      this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 40, 0) + 20);
      this->state.unknown_11C84->heightSpeed = static_cast<float>(std::max(rand() % 30, 0) + 50) / 10.0f;
      this->state.unknown_11C84->alpha = 0.0f;
      this->state.unknown_11C84->maxAlpha = 200.0f;
      this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("EFFECT\\alpha_center.tga", 0, false, true, false);
    }
  }
}


// OFFSET: 0x00C666B0
// FUNCTION: fn_20275
int CRagEffect::SuperAngel(char type, int dummy) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt == 0 || (this->state.stateCnt <= 18 && (type == 1 || type == 11) && this->state.stateCnt % 3 == 0)) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 85, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->size = 1.0f;
    this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 40.0f;
    this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[0].flag1[0] = type;
    this->state.unknown_11C84->slots[0].flag1[1] = static_cast<char>(this->state.stateCnt);
    this->state.unknown_11C84->alpha = 0.0f;

    if (type == 0) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ucc9c\uc0ac.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ucc9c\uc0ac.act");
      this->state.unknown_11C84->renderFlag = 5;
    } else if (type == 1) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ucc9c\uc0ac\ub0a0\uac1c.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ucc9c\uc0ac\ub0a0\uac1c.act");
      this->state.unknown_11C84->renderFlag = 5;
    } else if (type == 2) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uae43\ud1381.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uae43\ud1381.act");
      this->state.unknown_11C84->renderFlag = 5;
    } else if (type == 3) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uae43\ud1382.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uae43\ud1382.act");
      this->state.unknown_11C84->renderFlag = 5;
    } else if (type == 4) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uae43\ud1383.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uae43\ud1383.act");
      this->state.unknown_11C84->renderFlag = 5;
    } else if (type == 10) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ud55c\ubcf5\ucc9c\uc0ac.spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ud55c\ubcf5\ucc9c\uc0ac.act");
      this->state.unknown_11C84->renderFlag = 1;
      this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 30.0f;
      this->state.unknown_11C84->renderFlag |= 0x200;
    } else if (type == 11) {
      Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ud55c\ubcf5\ucc9c\uc0ac(\ub0a0\uac1c).spr");
      Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ud55c\ubcf5\ucc9c\uc0ac(\ub0a0\uac1c).act");
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 30.0f;
    }

    Owner(this->state.unknown_11C84).SetAction(0, 16, 1);
    Owner(this->state.unknown_11C84).InitSegment();
    this->state.unknown_11C84->repeatAnim = 1;
    Owner(this->state.unknown_11C84).render.previousAnimation = 0;
    this->state.unknown_11C84->slots[1].life = 0;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;
  }

  if (type == 0 || type == 10) {
    if (this->state.stateCnt == 65) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 75, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->duration = this->state.duration;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
      this->state.unknown_11C84->size = 4.0f;

      for (short i = 0; i < 4; ++i) {
        EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[i];
        slot.life = 1;
        slot.process = 0;
        slot.maxHeight = 2.0f;
        slot.rotStart = i * 90;
        slot.alphaB = 0;
        slot.distance = 2.0f;
        slot.height[0] = 1.0f;
        slot.flag1[0] = 0;
      }
    }

    if (this->state.stateCnt == 66) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 75, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->duration = this->state.duration;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
      this->state.unknown_11C84->size = 4.0f;

      for (short i = 0; i < 4; ++i) {
        EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[i];
        slot.life = 1;
        slot.process = 0;
        slot.alphaB = 0;
        slot.distance = 2.0f;
        slot.maxHeight = 2.0f;
        slot.rotStart = i * 90 + 45;
        slot.height[0] = 1.0f;
        slot.flag1[0] = 0;
      }
    }
  }

  return this->state.stateCnt;
}

// OFFSET: 0x00C3EE10
// FUNCTION: fn_20119
int CRagEffect::WarpZone() {
  if (this->state.stateCnt == 145) {
    this->state.stateCnt = 0;
  } else if (this->state.stateCnt != 0) {
    goto check_cadence;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 158;
  this->state.unknown_11C84->pattern |= 1;
  this->state.unknown_11C84->radius = 15.0f;
  this->state.unknown_11C84->deltaPos2.y -= 1.0f;
  this->state.unknown_11C84->latitude = 90.0f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 128.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
  Owner(this->state.unknown_11C84).InitSegment();
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);

check_cadence:
  if (this->state.stateCnt % 28 == 0 && this->state.stateCnt < 117) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 80;
    this->state.unknown_11C84->innerSize = 4.0f;
    this->state.unknown_11C84->latitude = 90.0f;
    this->state.unknown_11C84->deltaPos2.y -= 1.0f;
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->latitude)));
    this->state.unknown_11C84->radius = 14.0f;
    this->state.unknown_11C84->radiusSpeed = -0.15f;
    this->state.unknown_11C84->alpha = 5.0f;
    this->state.unknown_11C84->maxAlpha = 200.0f;
    this->state.unknown_11C84->alphaSpeed = 10.0f;
    this->state.unknown_11C84->chPoint.push_back(20.0f);
    this->state.unknown_11C84->chVal1.push_back(0.0f);
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 5;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  }

  int result = this->state.stateCnt / 10;
  if (this->state.stateCnt % 10 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->radius = static_cast<float>(std::max(rand() % 200, 0) / 100) * 0.75f + 5.0f;
    this->state.unknown_11C84->radiusSpeed = 0.0005f;
    this->state.unknown_11C84->duration = 130;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
    rand();
    this->state.unknown_11C84->longSpeed = 2.0f;
    this->state.unknown_11C84->gravSpeed = -0.2f;
    this->state.unknown_11C84->gravAccel = -(this->state.unknown_11C84->gravSpeed / static_cast<float>(this->state.unknown_11C84->duration)) / 1.5f;
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
    this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, this->state.unknown_11C84->radius);
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 10, 0) / 10) + 0.7f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\particle1.act");
    Owner(this->state.unknown_11C84).InitSegment();
  }
  return result;
}

// OFFSET: 0x00C12500
// FUNCTION: fn_20041
void CRagEffect::Enhance() {
  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->arcAngle = 22.5f;
    this->state.unknown_11C84->size = 7.0f;
    this->state.unknown_11C84->latitude = 90.0f;
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->latitude)));
    this->state.unknown_11C84->alpha = 128.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_yellow.tga", 0, false, true, false);

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->outerSize = 6.0f;
    this->state.unknown_11C84->innerSize = 6.0f;
    this->state.unknown_11C84->heightSpeed = 2.0f;
    this->state.unknown_11C84->heightAccel = -(this->state.unknown_11C84->heightSpeed / static_cast<float>(this->state.unknown_11C84->duration)) / 1.5f;
    this->state.unknown_11C84->maxHeightSize = 50.0f;
    this->state.unknown_11C84->alpha = 128.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
  }

  const int interval = g_serviceOptions.Get(229) == 0 ? 5 : 15;
  if (this->state.stateCnt % interval != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 50;
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  const float distance = static_cast<float>(std::max(rand() % 5, 0) + 2);
  this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, distance);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(90));
  this->state.unknown_11C84->speed = 2.0f;
  this->state.unknown_11C84->widthSize = 7.0f;
  this->state.unknown_11C84->heightSize = 0.25f;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
}

// OFFSET: 0x00C179F0
// FUNCTION: fn_20061
void CRagEffect::HealSP() {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt == 0) {
    this->state.duration = 100;
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 100;
    this->state.unknown_11C84->longSpeed = 5.0f;
    this->state.unknown_11C84->outerSize = 2.5f;
    this->state.unknown_11C84->innerSize = 2.5f;
    this->state.unknown_11C84->heightSize = 15.0f;
    this->state.unknown_11C84->red = 0x20;
    this->state.unknown_11C84->green = 0xb0;
    this->state.unknown_11C84->blue = 0xe8;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 16.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 30.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->longSpeed = 5.0f;
    this->state.unknown_11C84->outerSize = 3.5f;
    this->state.unknown_11C84->innerSize = 3.5f;
    this->state.unknown_11C84->heightSize = 25.0f;
    this->state.unknown_11C84->red = 0x20;
    this->state.unknown_11C84->green = 0xb0;
    this->state.unknown_11C84->blue = 0xe8;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 32.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 30.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->longSpeed = 5.0f;
    this->state.unknown_11C84->outerSize = 4.5f;
    this->state.unknown_11C84->innerSize = 4.5f;
    this->state.unknown_11C84->heightSize = 35.0f;
    this->state.unknown_11C84->red = 0x20;
    this->state.unknown_11C84->green = 0xb0;
    this->state.unknown_11C84->blue = 0xe8;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 64.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 30.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);

    PlayEffectWave(*this, "_heal_effect.wav");
  }

  if ((this->state.stateCnt & 1) == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 7, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    rand();
    this->state.unknown_11C84->radius = 3.0f;
    this->state.unknown_11C84->duration = 30;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->gravSpeed = -1.2f;
    this->state.unknown_11C84->gravAccel = -(this->state.unknown_11C84->gravSpeed / static_cast<float>(this->state.unknown_11C84->duration)) / 1.5f;
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->longitude)));
    this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, this->state.unknown_11C84->radius);
    this->state.unknown_11C84->size = 0.55f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\particle2.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\particle2.act");
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C0D2C0
// FUNCTION: fn_20021
void CRagEffect::RedParticleSpray(int count) {
  if (this->state.stateCnt != 0) {
    return;
  }

  for (int angle = -90; count > 0; angle += 90, --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 60;
    this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
    this->state.unknown_11C84->radius = Owner(this->state.unknown_11C84).CalcDist(this->state.deltaPos[0], this->state.deltaPos[2]);
    Owner(this->state.unknown_11C84).Face(this->state.deltaPos[0], this->state.deltaPos[2]);
    this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
    const float speed = this->state.unknown_11C84->radius / this->state.unknown_11C84->duration;
    this->state.unknown_11C84->speed = speed + speed;
    this->state.unknown_11C84->gravSpeed = 2.0f;
    const float rise = -(this->state.unknown_11C84->gravSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->gravAccel = rise + rise;
    this->state.unknown_11C84->latiSpeed = -2.0f;
    const float spread = -(this->state.unknown_11C84->latiSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->latiAccel = spread + spread;
    this->state.unknown_11C84->roll = static_cast<float>(angle);
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    this->state.unknown_11C84->deltaPos3 = RotatedForward(this->state.unknown_11C84->transform, 1.0f);
    this->state.unknown_11C84->rotAxis[0] = this->state.unknown_11C84->deltaPos3.x;
    this->state.unknown_11C84->rotAxis[1] = this->state.unknown_11C84->deltaPos3.y;
    this->state.unknown_11C84->rotAxis[2] = this->state.unknown_11C84->deltaPos3.z;
    this->state.unknown_11C84->red = 250;
    this->state.unknown_11C84->green = 100;
    this->state.unknown_11C84->blue = 100;
    this->state.unknown_11C84->size = 1.7f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\particle1.act");
    this->state.unknown_11C84->numSegment = 7;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C19FC0
// FUNCTION: fn_20067
void CRagEffect::Hit5() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  g_renderer->viewport.Projection().Project(
    &this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex
  );
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_hit5.wav");
  const int angle = std::max(rand() % 360, 0);
  const int angles[2] = {angle, angle + 90};

  for (int index = 0; index < 2; ++index) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 17;
    this->state.unknown_11C84->deltaPos2.y = -20.0f;
    this->state.unknown_11C84->roll = static_cast<float>(angles[index]);
    this->state.unknown_11C84->rollSpeed = -5.0f;
    this->state.unknown_11C84->rollAccel =
      -(this->state.unknown_11C84->rollSpeed / static_cast<float>(this->state.unknown_11C84->duration)) / 1.5f;
    this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 5, 0) + 20);
    this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 10, 0) + 30);
    this->state.unknown_11C84->widthSpeed =
      -(this->state.unknown_11C84->widthSize / static_cast<float>(this->state.unknown_11C84->duration));
    this->state.unknown_11C84->heightSpeed = 2.5f;
    this->state.unknown_11C84->heightAccel = 0.5f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 2;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\lens2.tga", 0, false, true, false);
    this->state.unknown_11C84->slots[0].life = 0;
  }
}

// OFFSET: 0x00C1A2C0
// FUNCTION: fn_20068
void CRagEffect::Hit6() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  g_renderer->viewport.Projection().Project(
    &this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex
  );
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_hit6.wav");
  const int angle = std::max(rand() % 360, 0);
  const int angles[2] = {angle, angle + 90};

  for (int index = 0; index < 2; ++index) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 17;
    this->state.unknown_11C84->deltaPos2.y = -20.0f;
    this->state.unknown_11C84->roll = static_cast<float>(angles[index]);
    this->state.unknown_11C84->rollSpeed = -5.0f;
    this->state.unknown_11C84->rollAccel =
      -(this->state.unknown_11C84->rollSpeed / static_cast<float>(this->state.unknown_11C84->duration)) / 1.5f;
    this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 5, 0) + 10);
    this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 5, 0) + 15);
    this->state.unknown_11C84->widthSpeed =
      -(this->state.unknown_11C84->widthSize / static_cast<float>(this->state.unknown_11C84->duration));
    this->state.unknown_11C84->heightSpeed = 1.7f;
    this->state.unknown_11C84->heightAccel = 0.5f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 2;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\lens2.tga", 0, false, true, false);
    this->state.unknown_11C84->slots[0].life = 0;
  }
}

// OFFSET: 0x00C10F50
// FUNCTION: fn_20036
void CRagEffect::StoneWall() {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\wizard_icewall.wav");
  this->state.duration = 9999;

  for (int offset = -2; offset < 4; offset += 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->pattern |= 1u;
    const int angle = std::max(rand() % 360, 0);
    const float sine = ClientSine(angle);
    const float cosine = ClientCosine(angle);
    this->state.unknown_11C84->deltaPos2.y = 30.0f;
    this->state.unknown_11C84->deltaPos2.x = sine * 0.0f;
    this->state.unknown_11C84->deltaPos2.z = -static_cast<float>(offset) * cosine;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 40, 0) / 100) + 2.5f;
    this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 100, 0) + 300) / 10.0f;
    this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 6, 0) + 87);
    this->state.unknown_11C84->chCnt = 20;
    this->state.unknown_11C84->alpha = 200.0f;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
    EffectPrimitiveState& turn = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
    EffectPrimitiveState& copy = *this->state.unknown_11C84;
    std::memcpy(copy.rotTransform, copy.transform, sizeof(copy.rotTransform));
    this->state.unknown_11C84->speed = 3.0f;
    EffectPrimitiveState& slow = *this->state.unknown_11C84;
    const float decay = -(slow.speed / 40.0f);
    slow.accel = decay + decay;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\stone.bmp", 0, false, true, false);
  }
}


// INLINED
static __forceinline void LaunchTintedFireballVolley(CRagEffect& effect, float turn, int red, int green, int blue, int fade) {
  effect.state.unknown_11C84 = &CreateEffectParticle(effect, effect.render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  effect.state.unknown_11C84->duration = 20;
  effect.state.unknown_11C84->pattern |= 0x120u;
  effect.state.unknown_11C84->deltaPos2 = {0.0f, -15.0f, 0.0f};
  Owner(effect.state.unknown_11C84).Face(effect.state.deltaPos[0], effect.state.deltaPos[2]);
  if (turn != 0.0f) {
    Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle + turn;
    if (Owner(effect.state.unknown_11C84).render.angle < 0.0f) {
      Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle + 360.0f;
    }

    if (Owner(effect.state.unknown_11C84).render.angle >= 360.0f) {
      Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle - 360.0f;
    }
  }

  effect.state.unknown_11C84->longitude = -Owner(effect.state.unknown_11C84).render.angle;
  Owner(effect.state.unknown_11C84).render.angle = 180.0f - Owner(effect.state.unknown_11C84).render.angle;
  if (Owner(effect.state.unknown_11C84).render.angle < 0.0f) {
    Owner(effect.state.unknown_11C84).render.angle = Owner(effect.state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(effect.state.unknown_11C84);
  particle.state.speed = -(particle.CalcDist(effect.state.deltaPos[0], effect.state.deltaPos[2]) / particle.state.duration);
  reinterpret_cast<matrix*>(effect.state.unknown_11C84->transform)->MakeYRotation(GetRadian(static_cast<int>(effect.state.unknown_11C84->longitude)));
  effect.state.unknown_11C84->size = 1.0f;
  effect.state.unknown_11C84->animSpeed = 1;
  effect.state.unknown_11C84->red = red;
  effect.state.unknown_11C84->green = green;
  effect.state.unknown_11C84->blue = blue;
  effect.state.unknown_11C84->alpha = static_cast<float>((fade - effect.state.stateCnt) * 40);
  Owner(effect.state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\fireball.spr");
  Owner(effect.state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\fireball.act");
  Owner(effect.state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C13D80
// FUNCTION: fn_20045
void CRagEffect::ShadedFireballVolley() {
  if (this->state.stateCnt == 5) {
    g_soundMgr->PlayWave("effect\\EF_FireBall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  const int frame = this->state.stateCnt;
  if (frame == 0 || frame == 1 || frame == 2 || frame == 3) {
    LaunchTintedFireballVolley(*this, 0.0f, 155, 155, 255, 5);
  }

  if (this->state.stateCnt == 6 || this->state.stateCnt == 7 || this->state.stateCnt == 8 || this->state.stateCnt == 9) {
    LaunchTintedFireballVolley(*this, -5.0f, 155, 255, 155, 11);
  }

  if (this->state.stateCnt == 12 || this->state.stateCnt == 13 || this->state.stateCnt == 14 || this->state.stateCnt == 15) {
    LaunchTintedFireballVolley(*this, 5.0f, 255, 155, 155, 17);
  }
}

// OFFSET: 0x00C10180
// FUNCTION: fn_20033
void CRagEffect::Detoxication() {
  const unsigned char enabled = g_serviceOptions.Get(0xe5);

  if (enabled == 0) {
    if (this->state.stateCnt % 5 != 0) {
      return;
    }

    if (this->state.stateCnt == 0) {
      PlayEffectWave(*this, "effect\\EF_Detoxication.wav");
    }

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->duration = 40;
    EffectPrimitiveState& poison = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(poison.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    this->state.unknown_11C84->deltaPos2 =
      RotatedForward(this->state.unknown_11C84->transform, static_cast<float>(std::max(rand() % 6, 0) + 2));
    EffectPrimitiveState& trail = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(trail.transform)->MakeXRotation(GetRadian(90));
  } else {
    if (this->state.stateCnt % 15 != 0) {
      return;
    }

    if (this->state.stateCnt == 0) {
      PlayEffectWave(*this, "effect\\EF_Detoxication.wav");
    }

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 40;
    this->state.unknown_11C84->alpha = 0.0f;
    EffectPrimitiveState& poison = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(poison.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    this->state.unknown_11C84->deltaPos2 =
      RotatedForward(this->state.unknown_11C84->transform, static_cast<float>(std::max(rand() % 6, 0) + 2));
    EffectPrimitiveState& trail = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(trail.transform)->MakeXRotation(GetRadian(90));
  }

  this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 30, 0) + 30) / 100.0f;
  this->state.unknown_11C84->accel =
    -(this->state.unknown_11C84->speed / static_cast<float>(this->state.unknown_11C84->duration)) * g_depthAverageHalf;
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 50, 0) + 40) / 100.0f;
  this->state.unknown_11C84->rollSpeed = 3.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
  this->state.unknown_11C84->fadeOutCnt =
    this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 5;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle2.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle2.act");
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C11610
// FUNCTION: fn_20038
void CRagEffect::PoisonAttack(char flag) {
  if (this->state.stateCnt == 0 && flag == 0) {
    PlayEffectWave(*this, "effect\\assasin_enchantpoison.wav");
    PlayEffectWave(*this, "effect\\EF_PoisonAttack.wav");
  } else if (flag == 1) {
    if (this->state.stateCnt % 9 != 0) {
      return;
    }

    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 40;
    EffectPrimitiveState& poison = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(poison.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    this->state.unknown_11C84->deltaPos2 =
      RotatedForward(this->state.unknown_11C84->transform, static_cast<float>(std::max(rand() % 6, 0) + 2));
    EffectPrimitiveState& trail = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(trail.transform)->MakeXRotation(GetRadian(90));
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 30, 0) + 30) / 100.0f;
    this->state.unknown_11C84->accel =
      -(this->state.unknown_11C84->speed / static_cast<float>(this->state.unknown_11C84->duration)) * g_depthAverageHalf;
    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 50, 0) + 40) / 170.0f;
    this->state.unknown_11C84->rollSpeed = 3.0f;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
    this->state.unknown_11C84->fadeOutCnt =
      this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 5;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle3.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle3.act");
    if (this->state.type == 0x304) {
      this->state.unknown_11C84->renderFlag = 0x201;
    }
    Owner(this->state.unknown_11C84).InitSegment();
    return;
  }

  if (this->state.stateCnt % 5 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 40;
  EffectPrimitiveState& poison = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(poison.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  this->state.unknown_11C84->deltaPos2 =
    RotatedForward(this->state.unknown_11C84->transform, static_cast<float>(std::max(rand() % 6, 0) + 2));
  EffectPrimitiveState& trail = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(trail.transform)->MakeXRotation(GetRadian(90));
  this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 30, 0) + 30) / 100.0f;
  this->state.unknown_11C84->accel =
    -(this->state.unknown_11C84->speed / static_cast<float>(this->state.unknown_11C84->duration)) * g_depthAverageHalf;
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 50, 0) + 40) / 100.0f;
  this->state.unknown_11C84->rollSpeed = 3.0f;
  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
  this->state.unknown_11C84->fadeOutCnt =
    this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 5;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle3.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle3.act");
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C18090
// FUNCTION: fn_20062
void CRagEffect::StoneSpike() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\wizard_earthspike.wav");
  g_modeMgr.GetGameMode()->SendMsg(122, 0, 0, 0, 0);
  this->state.duration += 50;

  for (int z = -10; z < 15; z += 5) {
    for (int x = -10; x < 15; x += 5) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = std::max(rand() % 30, 0) + this->state.duration - 10;
      this->state.unknown_11C84->pattern = 1;
      this->state.unknown_11C84->pattern |= 2u;
      const int heading = std::max(rand() % 360, 0);
      rand();
      ClientSine(heading);
      ClientCosine(heading);
      const int heightRand = std::max(rand() % 60, 0);
      this->state.unknown_11C84->deltaPos2 = {static_cast<float>(x), 10.0f, static_cast<float>(z)};
      this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 50, 0) + 300) / 100.0f;
      this->state.unknown_11C84->heightSize = static_cast<float>(heightRand + 1) / 10.0f + 9.0f;
      this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 20, 0) + 80);
      EffectPrimitiveState& tilt = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
      EffectPrimitiveState& turn = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
      EffectPrimitiveState& stone = *this->state.unknown_11C84;
      std::memcpy(stone.rotTransform, stone.transform, sizeof(stone.rotTransform));
      this->state.unknown_11C84->changeSpeed3d.y = -1.0f;
      this->state.unknown_11C84->changeAccel3d.y = -0.01f;
      this->state.unknown_11C84->speed = 1.0f;
      this->state.unknown_11C84->accel = 0.01f;
      this->state.unknown_11C84->changePoint = 11;
      this->state.unknown_11C84->changeSpeed = -1.2f;
      this->state.unknown_11C84->changeAccel = 0.0f;
      this->state.unknown_11C84->chCnt = 14;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\stone.bmp", 0, false, true, false);
    }
  }
}
// OFFSET: 0x00C586B0
// FUNCTION: fn_20208
void CRagEffect::WhiteSparkRing(char kind) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt >= 3) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 100, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  Owner(this->state.unknown_11C84).render.position = this->render.position;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;

    if (kind == 3) {
      this->state.unknown_11C84->slots[index].process = -(25 + std::max(rand() % 21, 0));
      this->state.unknown_11C84->slots[index].maxHeight = 0.06f;
    } else {
      this->state.unknown_11C84->slots[index].process = -(15 + std::max(rand() % 26, 0));
      this->state.unknown_11C84->slots[index].maxHeight = 0.07f;
    }

    this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 121, 0) + 125);
    Get_Angle(
      this->render.position.x - this->state.deltaPos[0], this->render.position.z - this->state.deltaPos[2],
      &this->state.unknown_11C84->slots[index].riseAngle
    );
    this->state.unknown_11C84->slots[index].riseAngle += static_cast<std::int16_t>(std::max(rand() % 121, 0) - 60);

    if (this->state.unknown_11C84->slots[index].riseAngle >= 360) {
      this->state.unknown_11C84->slots[index].riseAngle -= 360;
    }

    if (this->state.unknown_11C84->slots[index].riseAngle < 0) {
      this->state.unknown_11C84->slots[index].riseAngle += 360;
    }

    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 7, 0)) + 6.0f;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 7, 0)) * 0.1f + 0.8f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecTNow.y = 3.0f;

    for (int height = 20; height > 0; --height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].alphaT = phase;
  }
}
// OFFSET: 0x00C5E740
// FUNCTION: fn_20239
void CRagEffect::WhiteLightTrail(const char* name, char kind) {
  const std::int16_t limit = static_cast<std::int16_t>(kind == 0 ? 35 : (kind == 1 ? 0 : kind));

  if (this->state.stateCnt != limit) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 96, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  if (kind == 0) {
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].maxHeight = static_cast<float>(std::max(rand() % 16, 0)) * 0.01f + 0.1f;
    this->state.unknown_11C84->slots[0].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 3.0f;
    this->state.unknown_11C84->slots[0].process = -std::max(rand() % 26, 0);
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].vecBPre.y = static_cast<float>(-std::max(rand() % 8, 0));
    const int heading = std::max(rand() % 360, 0);
    this->state.unknown_11C84->slots[0].vecBPre.x = g_cosTable[heading] * 2.5f;
    this->state.unknown_11C84->slots[0].vecBPre.z = g_sinTable[heading] * 2.5f;
    this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].flag1[0] = 1;
    return;
  }

  if (kind == 1) {
    for (int index = 0; index < 4; ++index) {
      this->state.unknown_11C84->slots[index].life = 1;
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(std::max(rand() % 11, 0)) * 0.005f + 0.08f;
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.05f + 2.0f;
      this->state.unknown_11C84->slots[index].process = -std::max(rand() % 26, 0);
      this->state.unknown_11C84->slots[index].alphaB = 0;
      this->state.unknown_11C84->slots[index].vecBPre.y = static_cast<float>(-std::max(rand() % 8, 0));
      const int heading = std::max(rand() % 360, 0);
      this->state.unknown_11C84->slots[index].vecBPre.x = g_cosTable[heading] * 3.5f;
      this->state.unknown_11C84->slots[index].vecBPre.z = g_sinTable[heading] * 3.5f;
      this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].height[1] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].height[2] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].height[3] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].flag1[0] = 2;
    }
  }
}
// OFFSET: 0x00C482B0
// FUNCTION: fn_20162
void CRagEffect::ElementalOrbit(char kind) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  int count = static_cast<int>(this->state.deltaPos[0]);

  if (count > 5) {
    count = 5;
  } else if (count <= 0) {
    return;
  }

  int phase = 0;
  int angle = 0;

  for (int index = 0; index < count; ++index) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 32, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].height[5] = static_cast<float>(std::max(rand() % 360, 0));

    if (kind == 1) {
      this->state.unknown_11C84->slots[0].height[6] = static_cast<float>(angle);
      this->state.unknown_11C84->slots[0].height[7] = -14.0f;
      this->state.unknown_11C84->slots[0].distance = 1.3f;
      this->state.unknown_11C84->slots[0].height[10] = 1.0f;
      this->state.unknown_11C84->slots[0].flag1[0] = 0;
    } else if (kind == 3 || kind == 4) {
      this->state.unknown_11C84->slots[0].height[6] =
        this->state.deltaPos[0] > 5.0f ? static_cast<float>(angle) : static_cast<float>(phase) / this->state.deltaPos[0];
      this->state.unknown_11C84->slots[0].height[7] = -15.0f;
      this->state.unknown_11C84->slots[0].distance = 1.0f;

      if (this->state.deltaPos[0] > 5.0f) {
        if (this->state.deltaPos[0] - static_cast<float>(index) >= 6.0f) {
          this->state.unknown_11C84->slots[0].distance = 2.0f;
        }
      }

      this->state.unknown_11C84->slots[0].height[10] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[0] = 1;
    } else {
      this->state.unknown_11C84->slots[0].height[6] = static_cast<float>(angle);
      this->state.unknown_11C84->slots[0].height[7] = -15.0f;
      this->state.unknown_11C84->slots[0].distance = 2.0f;
      this->state.unknown_11C84->slots[0].height[10] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[0] = 0;

      if (this->state.deltaPos[0] > 10.0f) {
        this->state.unknown_11C84->slots[0].distance = 2.0f;

        if (this->state.deltaPos[0] - static_cast<float>(index) >= 11.0f) {
          this->state.unknown_11C84->slots[0].flag1[0] = static_cast<char>(21);
        } else {
          this->state.unknown_11C84->slots[0].flag1[0] = static_cast<char>(20);
        }
      } else if (this->state.deltaPos[0] > 5.0f) {
        if (this->state.deltaPos[0] - static_cast<float>(index) >= 6.0f) {
          this->state.unknown_11C84->slots[0].distance = 2.0f;
          this->state.unknown_11C84->slots[0].flag1[0] = static_cast<char>(20);
        }
      }
    }

    if (kind > 9) {
      this->state.unknown_11C84->slots[0].height[6] += static_cast<float>(std::max(rand() % 360, 0));
      if (this->state.unknown_11C84->slots[0].height[6] >= 360.0f) {
        this->state.unknown_11C84->slots[0].height[6] -= 360.0f;
      }
      this->state.unknown_11C84->slots[0].flag1[0] = static_cast<char>(kind);
      this->state.unknown_11C84->slots[0].distance = 6.0f;
      this->state.unknown_11C84->totalTexture = 1;
    }

    this->state.unknown_11C84->slots[0].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].process = 0;
    AllocateTextures(*this->state.unknown_11C84);

    const char* texture;

    switch (kind) {
    case 4:
      texture = "effect\\RL_richs_coin_B\\rcoin1.bmp";
      break;
    case 10:
      texture = "effect\\fire-1.bmp";
      break;
    case 11:
      texture = "effect\\lightning-1.bmp";
      break;
    case 12:
      texture = "effect\\water-1.bmp";
      break;
    case 13:
      texture = "effect\\stone-1.bmp";
      break;
    default:
      texture = "effect\\thunder_center.bmp";
      break;
    }

    this->state.unknown_11C84->textures[0] = g_texMgr.Load(texture, 0, false, true, false);
    phase += 360;
    angle += 72;
  }
}
// OFFSET: 0x00C62260
// FUNCTION: fn_20250
void CRagEffect::WhiteRingWave() {
  if (this->state.stateCnt % 100 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 63, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_white.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 16.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 2;
  this->state.unknown_11C84->slots[0].alphaT = 100;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 1;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = -10;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[1].rotStart = 25;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 0.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 3;
  this->state.unknown_11C84->slots[1].alphaT = 100;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 1;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = -20;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[2].rotStart = 50;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 0.0f;
  this->state.unknown_11C84->slots[2].riseAngle = 4;
  this->state.unknown_11C84->slots[2].alphaT = 100;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 1;
  }

  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].alphaT = 1;
}
// OFFSET: 0x00C59FD0
// FUNCTION: fn_20217
void CRagEffect::TripleRingStack(const char* name, char large) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 23, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (large == 1) {
    this->state.unknown_11C84->size = 7.0f;
  } else {
    this->state.unknown_11C84->size = 4.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[0].maxHeight = 15.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 120;
  this->state.unknown_11C84->slots[0].distance = 3.9f;
  this->state.unknown_11C84->slots[0].riseAngle = 55;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[1].maxHeight = 13.0f;
  this->state.unknown_11C84->slots[1].rotStart = 90;
  this->state.unknown_11C84->slots[1].alphaB = 120;
  this->state.unknown_11C84->slots[1].distance = 4.1f;
  this->state.unknown_11C84->slots[1].riseAngle = 50;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[2].maxHeight = 11.0f;
  this->state.unknown_11C84->slots[2].rotStart = 180;
  this->state.unknown_11C84->slots[2].alphaB = 120;
  this->state.unknown_11C84->slots[2].distance = 4.3f;
  this->state.unknown_11C84->slots[2].riseAngle = 45;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 0;
}
// OFFSET: 0x00C5BED0
// FUNCTION: fn_20226
void CRagEffect::WideTripleRingStack(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 23, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[0].maxHeight = 15.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 50;
  this->state.unknown_11C84->slots[0].alphaT = 1;
  this->state.unknown_11C84->slots[0].distance = 12.9f;
  this->state.unknown_11C84->slots[0].riseAngle = 55;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[1].maxHeight = 13.0f;
  this->state.unknown_11C84->slots[1].rotStart = 90;
  this->state.unknown_11C84->slots[1].alphaB = 50;
  this->state.unknown_11C84->slots[1].alphaT = 1;
  this->state.unknown_11C84->slots[1].distance = 13.1f;
  this->state.unknown_11C84->slots[1].riseAngle = 50;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[2].maxHeight = 11.0f;
  this->state.unknown_11C84->slots[2].rotStart = 180;
  this->state.unknown_11C84->slots[2].alphaB = 50;
  this->state.unknown_11C84->slots[2].alphaT = 1;
  this->state.unknown_11C84->slots[2].distance = 13.3f;
  this->state.unknown_11C84->slots[2].riseAngle = 45;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 0;
}
// OFFSET: 0x00C42E80
// FUNCTION: fn_20140
void CRagEffect::SoulLink(int) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 1) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 68, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 4;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\soul_l.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\soul_u.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\soul_o.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\soul_s.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;

    for (int index = 0; index < 4; ++index) {
      this->state.unknown_11C84->slots[index].life = 0;
    }

    for (int index = 0; index < 4; ++index) {
      this->state.unknown_11C84->slots[index].life = 1;
      this->state.unknown_11C84->slots[index].process = index * 20 - 60;
      this->state.unknown_11C84->slots[index].distance = 5.0f;
      this->state.unknown_11C84->slots[index].alphaB = 0;
      this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(-4 - index * 6);
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(index);
      this->state.unknown_11C84->slots[index].flag1[4] = 2;

      for (int height = 1; height < 11; ++height) {
        this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      }
    }
  } else if (this->state.stateCnt == 21) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 68, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 4;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\soul_k.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\soul_n.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\soul_i.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\soul_l.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;

    for (int index = 0; index < 4; ++index) {
      this->state.unknown_11C84->slots[index].life = 0;
    }

    for (int index = 0; index < 4; ++index) {
      this->state.unknown_11C84->slots[index].life = 1;
      this->state.unknown_11C84->slots[index].process = index * 20 - 120;
      this->state.unknown_11C84->slots[index].distance = 5.0f;
      this->state.unknown_11C84->slots[index].alphaB = 0;
      this->state.unknown_11C84->slots[index].height[0] = static_cast<float>(22 - index * 6);
      this->state.unknown_11C84->slots[index].flag1[0] = static_cast<char>(index);
      this->state.unknown_11C84->slots[index].flag1[4] = 2;

      for (int height = 1; height < 11; ++height) {
        this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      }
    }
  }
}
// OFFSET: 0x00C57CA0
// FUNCTION: fn_20206
void CRagEffect::WhiteSparkStreak(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  int limit = 0;

  if (kind == 1) {
    limit = 2;
  } else if (kind == 3) {
    limit = 5;
  }

  if (this->state.stateCnt < 0 || this->state.stateCnt >= limit) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 100, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;

    if (kind == 1) {
      this->state.unknown_11C84->slots[index].process = -std::max(rand() % 11, 0);
      this->state.unknown_11C84->slots[index].maxHeight = 0.1f;
      this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 51, 0) + 125);
      this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 5, 0)) + 5.0f;
      this->state.unknown_11C84->slots[index].vecTNow.y = 3.0f;
    } else if (kind == 3) {
      this->state.unknown_11C84->slots[index].process = -30 - std::max(rand() % 41, 0);
      this->state.unknown_11C84->slots[index].maxHeight = 0.07f;
      this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 121, 0) + 125);
      this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 6, 0)) + 6.0f;
      this->state.unknown_11C84->slots[index].vecTNow.y = 4.0f;
    } else {
      this->state.unknown_11C84->slots[index].process = -std::max(rand() % 26, 0);
      this->state.unknown_11C84->slots[index].maxHeight = 0.1f;

      if (kind < 3) {
        this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 51, 0) + 125);
      } else {
        this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 121, 0) + 125);
      }

      this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 6, 0)) + 6.0f;
      this->state.unknown_11C84->slots[index].vecTNow.y = 5.0f;
    }

    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 7, 0)) * 0.1f + 0.6f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;

    for (int height = 1; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].alphaT = kind;

    if (kind == 1) {
      for (int step = 0; step < 10; ++step) {
        this->state.unknown_11C84->slots[index].height[0] += 6.0f;

        if (this->state.unknown_11C84->slots[index].height[0] > 180.0f) {
          this->state.unknown_11C84->slots[index].height[0] = 180.0f;
        }

        for (int layer = static_cast<int>(this->state.unknown_11C84->slots[index].vecTNow.y); layer > 0; --layer) {
          this->state.unknown_11C84->slots[index].height[layer] = this->state.unknown_11C84->slots[index].height[layer - 1];
        }
      }
    }
  }
}
// OFFSET: 0x00C61C50
// FUNCTION: fn_20249
void CRagEffect::BlueRingWave() {
  if (this->state.stateCnt % 14 == 0) {
    this->render.position = this->state.parent->render.position;
    PlayEffectWave(*this, "effect\\EF_ReadyPortal.wav");
  }

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 63, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 5.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 0.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 2;
  this->state.unknown_11C84->slots[0].alphaT = 100;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 1;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = -10;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[1].rotStart = 25;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 0.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 3;
  this->state.unknown_11C84->slots[1].alphaT = 100;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 1;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = -20;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 6.001f;
  this->state.unknown_11C84->slots[2].rotStart = 50;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 0.0f;
  this->state.unknown_11C84->slots[2].riseAngle = 4;
  this->state.unknown_11C84->slots[2].alphaT = 100;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 1;
  }

  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].alphaT = 1;
}
// OFFSET: 0x00C526D0
// OFFSET: 0x00C4A650
// FUNCTION: fn_20170
void CRagEffect::QuadRingStack(const char* name) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 24, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 40.0f;
  this->state.unknown_11C84->slots[0].rotStart = 180;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 8.0f;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 39.0f;
  this->state.unknown_11C84->slots[1].rotStart = 270;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 7.9f;
  this->state.unknown_11C84->slots[1].riseAngle = 90;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 38.0f;
  this->state.unknown_11C84->slots[2].rotStart = 0;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 7.8f;
  this->state.unknown_11C84->slots[2].riseAngle = 90;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = 37.0f;
  this->state.unknown_11C84->slots[3].rotStart = 90;
  this->state.unknown_11C84->slots[3].alphaB = 0;
  this->state.unknown_11C84->slots[3].distance = 7.7f;
  this->state.unknown_11C84->slots[3].riseAngle = 90;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[3].height[height] = 0.0f;
    this->state.unknown_11C84->slots[3].flag1[height] = 0;
  }
}

// OFFSET: 0x00C43430
// OFFSET: 0x00C44290
// FUNCTION: fn_20143
void CRagEffect::QuadRingStackSized(const char* name, char kind) {
  if (this->state.duration < 70) {
    this->state.duration = 70;
  }

  this->render.position = this->state.parent->render.position;

  if (kind == 33) {
    kind = 0;
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 25, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  } else {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 23, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  }

  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = static_cast<float>(kind);

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[0].maxHeight = 25.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 180;
  this->state.unknown_11C84->slots[0].distance = 4.5f;
  this->state.unknown_11C84->slots[0].riseAngle = 70;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[1].maxHeight = 22.0f;
  this->state.unknown_11C84->slots[1].rotStart = 90;
  this->state.unknown_11C84->slots[1].alphaB = 180;
  this->state.unknown_11C84->slots[1].distance = 5.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 57;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[2].maxHeight = 19.0f;
  this->state.unknown_11C84->slots[2].rotStart = 180;
  this->state.unknown_11C84->slots[2].alphaB = 180;
  this->state.unknown_11C84->slots[2].distance = 5.5f;
  this->state.unknown_11C84->slots[2].riseAngle = 45;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = 250.0f;
  this->state.unknown_11C84->slots[3].rotStart = 0;
  this->state.unknown_11C84->slots[3].alphaB = 70;
  this->state.unknown_11C84->slots[3].distance = 4.0f;
  this->state.unknown_11C84->slots[3].riseAngle = 89;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[3].height[height] = 0.0f;
    this->state.unknown_11C84->slots[3].flag1[height] = 0;
  }
}

// OFFSET: 0x00C5CC00

// OFFSET: 0x00C649E0
// FUNCTION: fn_20263
void CRagEffect::sub_C649E0() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if ((this->state.stateCnt & 1) != 0 || this->state.stateCnt >= 300) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 40, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].maxHeight = static_cast<float>(std::max(rand() % 2, 0) + 2) * 0.1f;
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 6, 0) + 22) / 100.0f;
  this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].vecBPre.x = static_cast<float>(std::max(rand() % 300, 0)) + this->render.position.x - 150.0f;
  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 200.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = static_cast<float>(std::max(rand() % 300, 0)) + this->render.position.z - 150.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->alpha = 200.0f;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\sakura01.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\sakura01.act");
  Owner(this->state.unknown_11C84).SetAction(rand() % 3, 36, 0);
  Owner(this->state.unknown_11C84).InitSegment();
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C47F70
// FUNCTION: fn_20161
void CRagEffect::sub_C47F70(char flag) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  if (flag == 0) {
    PlayEffectWave(*this, "effect\\mon_\uce68\ud22c\uacbd.wav");
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 46, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].maxHeight = 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.x = this->state.deltaPos[0];
  this->state.unknown_11C84->slots[0].vecBPre.y = this->state.deltaPos[1] - 0.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = this->state.deltaPos[2];

  if (flag == 0) {
    this->state.unknown_11C84->alpha = 200.0f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uce68\ud22c\uacbd_.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uce68\ud22c\uacbd_.act");
    this->state.unknown_11C84->animSpeed = 2;
    this->state.unknown_11C84->size = 2.5f;
  } else if (flag == 1) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\wink.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\wink.act");
    this->state.unknown_11C84->animSpeed = 6;
    this->state.unknown_11C84->size = 1.0f;
    this->state.unknown_11C84->repeatAnim = 0;
    this->state.unknown_11C84->pattern |= 0x200u;
  } else if (flag == 2) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\fvoice.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\fvoice.act");
    this->state.unknown_11C84->animSpeed = 6;
    this->state.unknown_11C84->size = 1.0f;
    this->state.unknown_11C84->repeatAnim = 0;
    this->state.unknown_11C84->pattern |= 0x200u;
  } else if (flag == 12) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ud5cc\ub4dc\ub808\ub4dc\uc2a4\ud53c\uc5b4.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\ud5cc\ub4dc\ub808\ub4dc\uc2a4\ud53c\uc5b4.act");
    this->state.unknown_11C84->animSpeed = 6;
    this->state.unknown_11C84->size = 1.0f;
    this->state.unknown_11C84->repeatAnim = 0;
    this->state.unknown_11C84->pattern |= 0x200u;
  } else if (flag == 13) {
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uc708\ub4dc\ucee4\ud130.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "\uc774\ud329\ud2b8\\\uc708\ub4dc\ucee4\ud130.act");
    this->state.unknown_11C84->animSpeed = 6;
    this->state.unknown_11C84->size = 1.0f;
    this->state.unknown_11C84->repeatAnim = 0;
    this->state.unknown_11C84->pattern |= 0x200u;
  }

  std::int16_t angle;
  Get_Angle(this->render.position.x - this->state.deltaPos[0], this->render.position.z - this->state.deltaPos[2], &angle);
  const CView* camera = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
  std::int16_t direction = static_cast<std::int16_t>(static_cast<int>(camera->camera.yaw) + angle) % 360;
  if (direction < 0) {
    direction += 360;
  }

  this->state.unknown_11C84->renderFlag |= 0x200u;

  int animation;
  if (direction >= 180 && direction < 270) {
    animation = 0;
  } else if (direction >= 90 && direction < 180) {
    animation = 1;
  } else if (direction >= 0 && direction < 90) {
    animation = 2;
  } else {
    animation = 3;
  }

  Owner(this->state.unknown_11C84).SetAction(animation, 16, 1);
  Owner(this->state.unknown_11C84).InitSegment();
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C66F60
// FUNCTION: fn_20277
void CRagEffect::sub_C66F60() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\mon_\ud0c4\uc9c0\uc2e0\ud1b5.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 42, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\blue_ivy.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = -15;
  this->state.unknown_11C84->slots[0].maxHeight =
    SquareRoot((this->state.deltaPos[0] - this->render.position.x) * (this->state.deltaPos[0] - this->render.position.x) +
               (this->state.deltaPos[2] - this->render.position.z) * (this->state.deltaPos[2] - this->render.position.z));
  this->state.unknown_11C84->slots[0].distance = static_cast<float>(this->state.count) * 0.4f + 2.0f;
  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 5;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.y = this->render.position.y - 8.0f;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].height[1] = 2.0f;
  this->state.unknown_11C84->slots[0].height[2] =
    (2.0f / this->state.unknown_11C84->slots[0].maxHeight) * (this->state.deltaPos[1] - this->render.position.y);
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[1] = 0;
  this->state.unknown_11C84->slots[0].flag1[2] = 1;
  this->state.unknown_11C84->slots[0].height[7] = 0.0f;
  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].alphaB = 0;
}

// OFFSET: 0x00C54420
// FUNCTION: fn_20197
void CRagEffect::sub_C54420() {
  if (this->state.stateCnt != 0) {
    return;
  }

  g_modeMgr.GetGameMode()->SendMsg(122, 0, 0, 0, 0);

  for (int z = -10; z < 15; z += 5) {
    for (int x = -10; x < 15; x += 5) {
      for (int i = 0; i < 4; ++i) {
        this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 117, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
        this->state.unknown_11C84->pattern = 1;
        const int angle = std::max(rand() % 360, 0);
        rand();
        ClientSine(angle);
        ClientCosine(angle);
        this->state.unknown_11C84->deltaPos2.y = 10.0f;
        this->state.unknown_11C84->deltaPos2.x = static_cast<float>(x);
        this->state.unknown_11C84->deltaPos2.z = static_cast<float>(z);
        this->state.unknown_11C84->duration = 250;
        this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 50, 0) + 300) / 100.0f;
        this->state.unknown_11C84->heightSize = 18.0f;
        this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
        this->state.unknown_11C84->latitude = static_cast<float>(std::max(rand() % 40, 0) + 60);
        EffectPrimitiveState& tilt = *this->state.unknown_11C84;
        reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
        EffectPrimitiveState& turn = *this->state.unknown_11C84;
        reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
        EffectPrimitiveState& center = *this->state.unknown_11C84;
        std::memcpy(center.rotTransform, center.transform, sizeof(center.rotTransform));
        this->state.unknown_11C84->speed = 1.0f;
        this->state.unknown_11C84->accel = 0.01f;
        this->state.unknown_11C84->alpha = 20.0f;
        this->state.unknown_11C84->chCnt = 15;
        this->state.unknown_11C84->changePoint = 12;
        this->state.unknown_11C84->changeSpeed = -1.2f;
        this->state.unknown_11C84->changeAccel = 0.0f;
        this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
        AllocateTextures(*this->state.unknown_11C84);
        this->state.unknown_11C84->textures[0] = g_texMgr.Load(i < 2 ? "effect\\stone.bmp" : "effect\\ice.tga", 0, false, true, false);
      }
    }
  }
}

// OFFSET: 0x00C51740
// FUNCTION: fn_20189
void CRagEffect::sub_C51740(const char* name, char flag) {
  if (this->state.stateCnt != 0) {
    return;
  }

  this->render.position = this->state.parent->render.position;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 66, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 0;
  }

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;

    std::int16_t alphaB = 25;

    if (flag == 2 || flag == 12 || flag == 13) {
      alphaB = 30;
    } else if (flag == 11) {
      alphaB = 0;
    } else if (flag != 10) {
      alphaB = 40;
    }

    this->state.unknown_11C84->slots[i].alphaB = alphaB;

    this->state.unknown_11C84->slots[i].distance = 0.0f;

    if (i == 3) {
      this->state.unknown_11C84->slots[3].process = 180;

      if (flag == 1 || flag == 11) {
        this->state.unknown_11C84->slots[3].maxHeight = 4.0f;
      } else if (flag == 2 || flag == 12 || flag == 13) {
        this->state.unknown_11C84->slots[3].maxHeight = 1.0f;
      } else {
        this->state.unknown_11C84->slots[3].maxHeight = 2.0f;
      }
    } else if (flag == 1 || flag == 11) {
      this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(i * 2) + 4.0f;
    } else if (flag == 2 || flag == 12 || flag == 13) {
      this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(i) * 0.5f + 1.0f;
    } else {
      this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(i) + 2.0f;
    }

    this->state.unknown_11C84->slots[i].height[0] = this->state.unknown_11C84->slots[i].maxHeight;
    this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 25);
    this->state.unknown_11C84->slots[i].vecTNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[i].vecTNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[i].vecTNow.z = this->render.position.z;
    this->state.deltaPos[0] = this->render.position.x;
    this->state.deltaPos[1] = this->render.position.y;
    this->state.deltaPos[2] = this->render.position.z;

    if (flag == 10) {
      this->state.deltaPos[0] -= 15.0f;
      this->state.deltaPos[1] -= 60.0f;
      this->state.deltaPos[2] -= 15.0f;
    } else {
      this->state.deltaPos[0] -= 70.0f;
      this->state.deltaPos[1] -= 300.0f;
      this->state.deltaPos[2] -= 70.0f;
    }

    this->state.unknown_11C84->slots[i].vecTPre.x = this->state.deltaPos[0];
    this->state.unknown_11C84->slots[i].vecTPre.y = this->state.deltaPos[1];
    this->state.unknown_11C84->slots[i].vecTPre.z = this->state.deltaPos[2];
    this->state.unknown_11C84->slots[i].flag1[0] = flag;
  }
}
// OFFSET: 0x00C38AD0
// FUNCTION: fn_20099
void CRagEffect::SandScatter() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt >= 60 || (this->state.stateCnt & 1) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->pattern = 8;
  this->state.unknown_11C84->duration = this->state.duration - std::max(rand() % 100, 0);
  this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 300, 0) + 260) / 100.0f;
  this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 15, 0) + 130);
  this->state.unknown_11C84->widthSize = 30.0f;
  this->state.unknown_11C84->heightSize = 30.0f;
  this->state.unknown_11C84->deltaPos2.x = static_cast<float>(std::max(rand() % 640, 0) - 500);
  this->state.unknown_11C84->deltaPos2.y = static_cast<float>(std::max(rand() % 50, 0) - 240);
  this->state.unknown_11C84->chCnt = std::max(rand() % 10, 0) + 10;
  this->state.unknown_11C84->alpha = 128.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\sand1.bmp", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
}
// OFFSET: 0x00C39460
// FUNCTION: fn_20101
void CRagEffect::Sight() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if ((this->state.stateCnt & 1) != 0) {
    return;
  }

  const int angle = static_cast<int>(static_cast<float>(this->state.stateCnt) * -5.0f);
  const float sine = ClientSine(angle);
  const float cosine = ClientCosine(angle);
  GetRadian(angle);

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->deltaPos2.x = sine * 15.0f;
  this->state.unknown_11C84->deltaPos2.y = -20.0f;
  this->state.unknown_11C84->deltaPos2.z = cosine * -15.0f;
  this->state.unknown_11C84->deltaPosAccel.y = 0.1f;
  this->state.unknown_11C84->animSpeed = 3;
  this->state.unknown_11C84->size = 2.5f;
  this->state.unknown_11C84->sizeSpeed = -0.1f;
  this->state.unknown_11C84->alpha = 150.0f;
  this->state.unknown_11C84->alphaSpeed = -3.0f;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\Sight.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\Sight.act");
  Owner(this->state.unknown_11C84).InitSegment();

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 20;
  this->state.unknown_11C84->deltaPos2.x = sine * 15.0f;
  this->state.unknown_11C84->deltaPos2.y = 0.0f;
  this->state.unknown_11C84->deltaPos2.z = cosine * -15.0f;
  this->state.unknown_11C84->animSpeed = 2;
  this->state.unknown_11C84->size = 1.5f;
  this->state.unknown_11C84->sizeSpeed = -0.06f;
  this->state.unknown_11C84->alpha = 120.0f;
  this->state.unknown_11C84->alphaSpeed = -1.0f;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "Shadow.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "Shadow.act");
  Owner(this->state.unknown_11C84).InitSegment();
}
// OFFSET: 0x00C3D190
// FUNCTION: fn_20115
void CRagEffect::AlphaCenterRise() {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;

  if (this->state.stateCnt == 0) {
    for (int i = 20; i != 0; --i) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 3, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->duration = 40;
      this->state.unknown_11C84->deltaPos2.y = -40.0f;
      this->state.unknown_11C84->longitude = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->longSpeed = static_cast<float>(std::max(rand() % 60, 0) + 10) / 10.0f;
      const float accel = -(this->state.unknown_11C84->longSpeed / static_cast<float>(this->state.unknown_11C84->duration));
      this->state.unknown_11C84->longAccel = accel / 1.5f;
      this->state.unknown_11C84->arcAngle = static_cast<float>(std::max(rand() % 25, 0) + 5) / 10.0f;
      this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 40, 0) + 20);
      this->state.unknown_11C84->heightSpeed = static_cast<float>(std::max(rand() % 30, 0) + 20) / 10.0f;
      this->state.unknown_11C84->alpha = 0.0f;
      this->state.unknown_11C84->maxAlpha = 200.0f;
      this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 10.0f;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - this->state.unknown_11C84->duration / 3;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("EFFECT\\alpha_center.tga", 0, false, true, false);
    }

    return;
  }

  if (this->state.stateCnt == 35 && this->state.parent != nullptr) {
    this->state.parent->SendMsg(nullptr, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}
// OFFSET: 0x00C36710
// FUNCTION: fn_20093
bool CRagEffect::Potion_Awakening() {
  if (this->state.stateCnt == 0) {
    this->state.parent->SendMsg(nullptr, 127, 0, 250, 0, 250, 0, 200, 0, 0, 0, 0, 0, 0, 0);
    this->PotionPillar(1.0f, 50);
  }

  if (this->state.stateCnt == 18) {
    g_modeMgr.GetGameMode()->SendMsg(122, 1, 0, 0, 0);
  }

  this->render.position = this->state.parent->render.position;

  if (static_cast<unsigned int>(this->state.stateCnt - 13) <= 96u && this->state.stateCnt % 4 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 40;
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
    EffectPrimitiveState& spot = *this->state.unknown_11C84;
    spot.deltaPos2 = RotatedForward(spot.transform, static_cast<float>(std::max(rand() % 7, 0) + 2));
    reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(90));
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 20) / 60.0f;
    this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 60, 0) + 30) / 10.0f;
    this->state.unknown_11C84->heightSize = 0.2f;
    this->state.unknown_11C84->red = 240;
    this->state.unknown_11C84->green = 240;
    this->state.unknown_11C84->blue = 120;
    this->state.unknown_11C84->alpha = 0.0f;
    this->state.unknown_11C84->maxAlpha = 180.0f;
    this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
    this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
    this->state.unknown_11C84->slots[0].life = 0;
  }

  this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
  if (this->state.stateCnt <= 150) {
    return true;
  }

  this->state.parent->SendMsg(nullptr, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  return false;
}
// OFFSET: 0x00C3C4E0
// FUNCTION: fn_20110
void CRagEffect::StatusIconColumn() {
  if (g_renderer == nullptr) {
    return;
  }

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
  }

  if (this->state.count != 0) {
    this->SendMsg(nullptr, 25, 0, 100, 0, 0, 0, 0);
    this->state.count = 0;
    this->state.stateCnt = 0;
  }

  if (this->state.stateCnt != 0) {
    return;
  }

  for (int index = 0; index < 100; ++index) {
    g_statusIconIds[index] = -1;
  }

  int foundCount = 0;
  int offsetX = 0;
  int offsetY = 0;
  int row = 0;
  const int limit = g_renderer->viewport.height - 50;

  for (int type = 0; type < 5; ++type) {
    for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
      const char* texture = g_session.GetEFSTImgFileName(effect.id, type);
      const int id = effect.id == 42 ? 12 : static_cast<int>(effect.id);

      if (texture == nullptr) {
        continue;
      }

      bool known = false;
      for (int index = 0; index < foundCount; ++index) {
        if (g_statusIconIds[index] == id) {
          known = true;
          break;
        }
      }

      if (known) {
        continue;
      }

      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 0, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 0x201;
      this->state.unknown_11C84->unknown_01C8[0] = 1;
      this->state.unknown_11C84->pattern = 4;
      Owner(this->state.unknown_11C84).render.curMotion = 0;
      this->state.unknown_11C84->duration = 99999999;
      this->state.unknown_11C84->widthSize = 16.0f;
      this->state.unknown_11C84->heightSize = 16.0f;

      if (row * 35 + 150 < limit) {
        offsetY += 35;
      } else {
        row = 0;
        offsetY = 35;
        offsetX -= 45;
      }

      this->state.unknown_11C84->deltaPos2.x = static_cast<float>(g_renderer->viewport.width - 32 + offsetX);
      this->state.unknown_11C84->deltaPos2.y = static_cast<float>(offsetY + 150);
      this->state.unknown_11C84->slots[0].life = 98;
      this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(id);
      this->state.unknown_11C84->totalTexture = 2;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load(texture, 0, false, true, false);
      this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);

      if (foundCount < 99) {
        g_statusIconIds[foundCount] = id;
        ++foundCount;
        ++row;
      }
    }
  }
}
// OFFSET: 0x00C3CC70
// FUNCTION: fn_20113
bool CRagEffect::StormGust() {
  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\wizard_stormgust.wav");
  }

  if (g_serviceOptions.Get(229) == 0 && this->state.stateCnt % 29 == 0 &&
      static_cast<unsigned int>(this->state.stateCnt - 31) <= 119u) {
    for (int count = 2; count != 0; --count) {
      const int longitudeAngle = rand() % 360;
      const int radius = std::max(rand() % 25, 0) + 3;
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      EffectPrimitiveState& particle = *this->state.unknown_11C84;
      particle.duration = 215 - this->state.stateCnt;
      particle.pattern |= 1u;

      const int angle = static_cast<int>(static_cast<float>(std::max(rand() % 360, 0)));
      const float sine = ClientSine(angle);
      const float cosine = ClientCosine(angle);
      particle.deltaPos2.x = sine * static_cast<float>(radius);
      particle.deltaPos2.y = 20.0f;
      particle.deltaPos2.z = -static_cast<float>(radius) * cosine;
      particle.size = static_cast<float>(std::max(rand() % 100, 0) + 400) / 100.0f;
      particle.heightSize = 20.0f;
      particle.longitude = static_cast<float>(std::max(longitudeAngle, 0));
      particle.latitude = 100.0f;

      EffectPrimitiveState& tilt = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(tilt.transform)->MakeXRotation(GetRadian(static_cast<int>(tilt.latitude)));
      EffectPrimitiveState& turn = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(turn.transform)->AppendYRotation(GetRadian(static_cast<int>(turn.longitude)));
      EffectPrimitiveState& copy = *this->state.unknown_11C84;
      std::memcpy(copy.rotTransform, copy.transform, sizeof(copy.rotTransform));

      this->state.unknown_11C84->speed = 1.5f;
      this->state.unknown_11C84->accel = 0.0f;
      this->state.unknown_11C84->alpha = 250.0f;
      this->state.unknown_11C84->chCnt = 3;
      this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 10;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ice.tga", 0, false, true, false);
    }
  }

  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f) ? true : false;
}

// OFFSET: 0x00C0FD10
// FUNCTION: fn_20032
void CRagEffect::TexturedSkillAura(char type) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  if (type == 0) {
    g_soundMgr->PlayWave(
      "effect\\hunter_detecting.wav", this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z,
      250, 40, 1.0f, 0
    );
  } else if (type == 2) {
    g_soundMgr->PlayWave(
      "effect\\priest_benedictio.wav", this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z,
      250, 40, 1.0f, 0
    );
  } else if (type == 4) {
    g_soundMgr->PlayWave(
      "effect\\cru_grand cross.wav", this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z,
      250, 40, 1.0f, 0
    );
  } else if (type == 5) {
    g_soundMgr->PlayWave(
      "effect\\ANALYZE.wav", this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z, 250, 40,
      1.0f, 0
    );
  }

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 57;
  this->state.unknown_11C84->widthSpeed = 1.5f;
  this->state.unknown_11C84->heightSpeed = 1.5f;
  this->state.unknown_11C84->widthAccel = this->state.unknown_11C84->widthSpeed / 14.0f;
  this->state.unknown_11C84->heightAccel = this->state.unknown_11C84->heightSpeed / 14.0f;
  this->state.unknown_11C84->deltaPos2.y = -7.0f;
  this->state.unknown_11C84->speed3d.y = -0.5f;
  this->state.unknown_11C84->roll = 90.0f;

  if (type == 3 || type == 6) {
    this->state.unknown_11C84->changePoint = 10;
    this->state.unknown_11C84->fadeOutCnt = 30;
  } else {
    this->state.unknown_11C84->changePoint = 17;
    this->state.unknown_11C84->fadeOutCnt = 40;
  }

  if (type == 4) {
    this->state.unknown_11C84->red = 0xff;
    this->state.unknown_11C84->blue = 0x8c;
    this->state.unknown_11C84->green = 0x8c;
  } else if (type == 5) {
    this->state.unknown_11C84->red = 0xd2;
    this->state.unknown_11C84->blue = 0xd2;
    this->state.unknown_11C84->green = 0xfa;
  } else if (type == 6) {
    this->state.unknown_11C84->red = 10;
    this->state.unknown_11C84->blue = 10;
    this->state.unknown_11C84->green = 10;
  } else if (type == 7) {
    this->state.unknown_11C84->red = 0xfa;
    this->state.unknown_11C84->blue = 0x8c;
    this->state.unknown_11C84->green = 0xd2;
  } else if (type == 8) {
    this->state.unknown_11C84->red = 0x8c;
    this->state.unknown_11C84->blue = 0x8c;
    this->state.unknown_11C84->green = 0xff;
  }

  this->state.unknown_11C84->changeWidthSpeed = 0.1f;
  this->state.unknown_11C84->changeHeightSpeed = 0.1f;
  this->state.unknown_11C84->changeWidthAccel = this->state.unknown_11C84->changeWidthSpeed / 30.0f;
  this->state.unknown_11C84->changeHeightAccel = this->state.unknown_11C84->changeHeightSpeed / 30.0f;
  this->state.unknown_11C84->deltaPos2.y = -5.0f;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\fashasha.tga", 0, false, true, false);
}

// OFFSET: 0x00C60240
// FUNCTION: fn_20243
void CRagEffect::TexturedMagicRing(char type) {
  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);

    if (type == 3) {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_red.tga", 0, false, true, false);
      this->state.unknown_11C84->size = 10.0f;
    } else {
      this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\magic_violet.tga", 0, false, true, false);
      this->state.unknown_11C84->size = 4.0f;
    }

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[0].maxHeight = 80.0f;
    this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 7.0f;
    this->state.unknown_11C84->slots[0].riseAngle = 90;
    this->state.unknown_11C84->slots[0].alphaT = static_cast<std::int16_t>(type == 1 ? -1 : 1400);
    this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[0].height[i] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[i] = 3;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[1].maxHeight = 80.0f;
    this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].distance = 5.5f;
    this->state.unknown_11C84->slots[1].riseAngle = 90;
    this->state.unknown_11C84->slots[1].alphaT = static_cast<std::int16_t>(type == 1 ? -1 : 1400);
    this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[1].height[i] = 0.0f;
      this->state.unknown_11C84->slots[1].flag1[i] = 3;
    }

    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].process = 0;
    this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[2].maxHeight = 80.0f;
    this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[2].distance = 4.0f;
    this->state.unknown_11C84->slots[2].riseAngle = 90;
    this->state.unknown_11C84->slots[2].alphaT = static_cast<std::int16_t>(type == 1 ? -1 : 1400);
    this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[2].height[i] = 0.0f;
      this->state.unknown_11C84->slots[2].flag1[i] = 3;
    }

    this->state.unknown_11C84->slots[3].life = 0;
  }

  if (this->state.stateCnt == 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 62, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
    this->state.unknown_11C84->size = type == 3 ? 10.0f : 4.0f;

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 30;
    this->state.unknown_11C84->slots[0].maxHeight = 2.5f;
    this->state.unknown_11C84->slots[0].rotStart = 0;
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 13.0f;
    this->state.unknown_11C84->slots[0].riseAngle = 90;
    this->state.unknown_11C84->slots[0].alphaT = 3;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[0].height[i] = 1.5f;
      this->state.unknown_11C84->slots[0].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 30;
    this->state.unknown_11C84->slots[1].maxHeight = 5.0f;
    this->state.unknown_11C84->slots[1].rotStart = 90;
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].distance = 11.0f;
    this->state.unknown_11C84->slots[1].riseAngle = 90;
    this->state.unknown_11C84->slots[1].alphaT = 3;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[1].height[i] = 1.5f;
      this->state.unknown_11C84->slots[1].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].process = 0;
    this->state.unknown_11C84->slots[2].fullDisplayAngle = 30;
    this->state.unknown_11C84->slots[2].maxHeight = 7.5f;
    this->state.unknown_11C84->slots[2].rotStart = 180;
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[2].distance = 9.0f;
    this->state.unknown_11C84->slots[2].riseAngle = 90;
    this->state.unknown_11C84->slots[2].alphaT = 3;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[2].height[i] = 1.5f;
      this->state.unknown_11C84->slots[2].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[3].life = 1;
    this->state.unknown_11C84->slots[3].process = 0;
    this->state.unknown_11C84->slots[3].fullDisplayAngle = 30;
    this->state.unknown_11C84->slots[3].maxHeight = 10.0f;
    this->state.unknown_11C84->slots[3].rotStart = 270;
    this->state.unknown_11C84->slots[3].alphaB = 0;
    this->state.unknown_11C84->slots[3].distance = 8.0f;
    this->state.unknown_11C84->slots[3].riseAngle = 90;
    this->state.unknown_11C84->slots[3].alphaT = 3;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[3].height[i] = 1.5f;
      this->state.unknown_11C84->slots[3].flag1[i] = 0;
    }
  }
}

// OFFSET: 0x00C41690
// FUNCTION: fn_20137
void CRagEffect::TexturedRisingRing(const char* name) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 1) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 107, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 12.0f;

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[0].rotStart = 270;
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 2.5f;
    this->state.unknown_11C84->slots[0].maxHeight = this->state.unknown_11C84->slots[0].distance;
    this->state.unknown_11C84->slots[0].riseAngle = 53;
    this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[0].height[i] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[1].rotStart = 0;
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].distance = 5.0f;
    this->state.unknown_11C84->slots[1].maxHeight = this->state.unknown_11C84->slots[1].distance;
    this->state.unknown_11C84->slots[1].riseAngle = 60;
    this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[1].height[i] = 0.0f;
      this->state.unknown_11C84->slots[1].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].process = 0;
    this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[2].rotStart = 90;
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[2].distance = 7.5f;
    this->state.unknown_11C84->slots[2].maxHeight = this->state.unknown_11C84->slots[2].distance;
    this->state.unknown_11C84->slots[2].riseAngle = 55;
    this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[2].height[i] = 0.0f;
      this->state.unknown_11C84->slots[2].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[3].life = 1;
    this->state.unknown_11C84->slots[3].process = 0;
    this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[3].rotStart = 180;
    this->state.unknown_11C84->slots[3].alphaB = 0;
    this->state.unknown_11C84->slots[3].distance = 10.0f;
    this->state.unknown_11C84->slots[3].maxHeight = this->state.unknown_11C84->slots[3].distance;
    this->state.unknown_11C84->slots[3].riseAngle = 50;
    this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[3].height[i] = 0.0f;
      this->state.unknown_11C84->slots[3].flag1[i] = 0;
    }
  }

  if (this->state.stateCnt == 2) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 107, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 12.0f;

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[0].rotStart = 271;
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 2.7f;
    this->state.unknown_11C84->slots[0].maxHeight = this->state.unknown_11C84->slots[0].distance;
    this->state.unknown_11C84->slots[0].riseAngle = 52;
    this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[0].height[i] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[1].rotStart = 1;
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].distance = 5.2f;
    this->state.unknown_11C84->slots[1].maxHeight = this->state.unknown_11C84->slots[1].distance;
    this->state.unknown_11C84->slots[1].riseAngle = 59;
    this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[1].height[i] = 0.0f;
      this->state.unknown_11C84->slots[1].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].process = 0;
    this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[2].rotStart = 91;
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[2].distance = 7.7f;
    this->state.unknown_11C84->slots[2].maxHeight = this->state.unknown_11C84->slots[2].distance;
    this->state.unknown_11C84->slots[2].riseAngle = 54;
    this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[2].height[i] = 0.0f;
      this->state.unknown_11C84->slots[2].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[3].life = 1;
    this->state.unknown_11C84->slots[3].process = 0;
    this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[3].rotStart = 181;
    this->state.unknown_11C84->slots[3].alphaB = 0;
    this->state.unknown_11C84->slots[3].distance = 10.2f;
    this->state.unknown_11C84->slots[3].maxHeight = this->state.unknown_11C84->slots[3].distance;
    this->state.unknown_11C84->slots[3].riseAngle = 49;
    this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[3].height[i] = 0.0f;
      this->state.unknown_11C84->slots[3].flag1[i] = 0;
    }
  }
}

// OFFSET: 0x00C52D20
// FUNCTION: fn_20196
bool CRagEffect::TexturedRisingRingStages(const char* name, char level) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 16.0f;

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[0].maxHeight = 13.0f;
    this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 3.6f;
    this->state.unknown_11C84->slots[0].riseAngle = 25;
    this->state.unknown_11C84->slots[0].alphaT = 3080;
    this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[0].height[i] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[1].maxHeight = 14.0f;
    this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].distance = 3.3f;
    this->state.unknown_11C84->slots[1].riseAngle = 25;
    this->state.unknown_11C84->slots[1].alphaT = 3080;
    this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[1].height[i] = 0.0f;
      this->state.unknown_11C84->slots[1].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].process = 0;
    this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[2].maxHeight = 15.0f;
    this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[2].distance = 3.0f;
    this->state.unknown_11C84->slots[2].riseAngle = 25;
    this->state.unknown_11C84->slots[2].alphaT = 3080;
    this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[2].height[i] = 0.0f;
      this->state.unknown_11C84->slots[2].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[3].life = 0;
  }

  if (level == 1) {
    if (this->state.stateCnt == 30) {
      PlayEffectWave(*this, "effect\\EF_GlassWall.wav");
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->duration = this->state.duration;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
      this->state.unknown_11C84->size = 16.0f;

      this->state.unknown_11C84->slots[0].life = 1;
      this->state.unknown_11C84->slots[0].process = 0;
      this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[0].maxHeight = 24.0f;
      this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[0].alphaB = 0;
      this->state.unknown_11C84->slots[0].distance = 3.6f;
      this->state.unknown_11C84->slots[0].riseAngle = 90;
      this->state.unknown_11C84->slots[0].alphaT = 3080;
      this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[0].height[i] = 0.0f;
        this->state.unknown_11C84->slots[0].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[1].life = 1;
      this->state.unknown_11C84->slots[1].process = 0;
      this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[1].maxHeight = 27.0f;
      this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[1].alphaB = 0;
      this->state.unknown_11C84->slots[1].distance = 3.3f;
      this->state.unknown_11C84->slots[1].riseAngle = 90;
      this->state.unknown_11C84->slots[1].alphaT = 3080;
      this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[1].height[i] = 0.0f;
        this->state.unknown_11C84->slots[1].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[2].life = 1;
      this->state.unknown_11C84->slots[2].process = 0;
      this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[2].maxHeight = 30.0f;
      this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[2].alphaB = 0;
      this->state.unknown_11C84->slots[2].distance = 3.0f;
      this->state.unknown_11C84->slots[2].riseAngle = 90;
      this->state.unknown_11C84->slots[2].alphaT = 3080;
      this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[2].height[i] = 0.0f;
        this->state.unknown_11C84->slots[2].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[3].life = 0;
    }
  } else {
    if (this->state.stateCnt == 30) {
      PlayEffectWave(*this, "effect\\EF_GlassWall.wav");
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->duration = this->state.duration;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
      this->state.unknown_11C84->size = 16.0f;

      this->state.unknown_11C84->slots[0].life = 1;
      this->state.unknown_11C84->slots[0].process = 0;
      this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[0].maxHeight = 34.0f;
      this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[0].alphaB = 0;
      this->state.unknown_11C84->slots[0].distance = 3.6f;
      this->state.unknown_11C84->slots[0].riseAngle = 90;
      this->state.unknown_11C84->slots[0].alphaT = 3080;
      this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[0].height[i] = 0.0f;
        this->state.unknown_11C84->slots[0].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[1].life = 1;
      this->state.unknown_11C84->slots[1].process = 0;
      this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[1].maxHeight = 37.0f;
      this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[1].alphaB = 0;
      this->state.unknown_11C84->slots[1].distance = 3.3f;
      this->state.unknown_11C84->slots[1].riseAngle = 90;
      this->state.unknown_11C84->slots[1].alphaT = 3080;
      this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[1].height[i] = 0.0f;
        this->state.unknown_11C84->slots[1].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[2].life = 1;
      this->state.unknown_11C84->slots[2].process = 0;
      this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[2].maxHeight = 40.0f;
      this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[2].alphaB = 0;
      this->state.unknown_11C84->slots[2].distance = 3.0f;
      this->state.unknown_11C84->slots[2].riseAngle = 90;
      this->state.unknown_11C84->slots[2].alphaT = 3080;
      this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[2].height[i] = 0.0f;
        this->state.unknown_11C84->slots[2].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[3].life = 0;
    }

    if (this->state.stateCnt == 60) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->renderFlag = 5;
      this->state.unknown_11C84->duration = this->state.duration;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
      this->state.unknown_11C84->size = 16.0f;

      this->state.unknown_11C84->slots[0].life = 1;
      this->state.unknown_11C84->slots[0].process = 0;
      this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[0].maxHeight = 74.0f;
      this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[0].alphaB = 0;
      this->state.unknown_11C84->slots[0].distance = 1.3f;
      this->state.unknown_11C84->slots[0].riseAngle = 90;
      this->state.unknown_11C84->slots[0].alphaT = 3080;
      this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[0].height[i] = 0.0f;
        this->state.unknown_11C84->slots[0].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[1].life = 1;
      this->state.unknown_11C84->slots[1].process = 0;
      this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[1].maxHeight = 77.0f;
      this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[1].alphaB = 0;
      this->state.unknown_11C84->slots[1].distance = 1.0f;
      this->state.unknown_11C84->slots[1].riseAngle = 90;
      this->state.unknown_11C84->slots[1].alphaT = 3080;
      this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[1].height[i] = 0.0f;
        this->state.unknown_11C84->slots[1].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[2].life = 1;
      this->state.unknown_11C84->slots[2].process = 0;
      this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
      this->state.unknown_11C84->slots[2].maxHeight = 80.0f;
      this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[2].alphaB = 0;
      this->state.unknown_11C84->slots[2].distance = 0.7f;
      this->state.unknown_11C84->slots[2].riseAngle = 90;
      this->state.unknown_11C84->slots[2].alphaT = 3080;
      this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;
      for (int i = 0; i < 21; ++i) {
        this->state.unknown_11C84->slots[2].height[i] = 0.0f;
        this->state.unknown_11C84->slots[2].flag1[i] = 0;
      }

      this->state.unknown_11C84->slots[3].life = 0;
    }
  }

  return this->state.stateCnt != this->state.duration;
}

// OFFSET: 0x00C60EE0
// FUNCTION: fn_20244
void CRagEffect::Cloud11Burst(char kind) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 62, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\cloud11.tga", 0, false, true, false);
  this->state.unknown_11C84->size = kind == 1 ? 8.0f : 5.0f;

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].fullDisplayAngle = 30;

    float maxHeight;
    if (kind == 1) {
      maxHeight = static_cast<float>(i) * 2.0f + 3.0f;
    } else if (kind == 2) {
      maxHeight = 48.0f - static_cast<float>(i) * 5.0f;
    } else if (kind == 3) {
      maxHeight = 28.0f - static_cast<float>(i) * 5.0f;
    } else {
      maxHeight = static_cast<float>(i) * 2.0f + 5.0f;
    }

    this->state.unknown_11C84->slots[i].maxHeight = maxHeight;
    this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(i * 90);
    this->state.unknown_11C84->slots[i].alphaB = 0;

    float distance;
    if (kind >= 2) {
      this->state.unknown_11C84->slots[i].alphaT = 2;
      if (kind == 2) {
        distance = 14.0f - static_cast<float>(i) * 2.0f;
      } else if (kind == 3) {
        distance = 6.0f - static_cast<float>(i);
      } else {
        distance = static_cast<float>(std::max(rand() % 101, 0)) * 0.03f + 4.5f;
      }
    } else {
      this->state.unknown_11C84->slots[i].alphaT = 1;
      if (kind == 1) {
        distance = static_cast<float>(std::max(rand() % 101, 0)) * 0.01f + 2.5f;
      } else {
        distance = static_cast<float>(std::max(rand() % 101, 0)) * 0.03f + 4.5f;
      }
    }

    this->state.unknown_11C84->slots[i].distance = distance;
    this->state.unknown_11C84->slots[i].riseAngle = static_cast<std::int16_t>(std::max(rand() % 21, 0) + 80);

    for (int j = 0; j < 21; ++j) {
      this->state.unknown_11C84->slots[i].height[j] = 1.0f;
      this->state.unknown_11C84->slots[i].flag1[j] = 0;
    }
  }
}

// OFFSET: 0x00C582B0
// FUNCTION: fn_20207
void CRagEffect::White02Burst(char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (static_cast<unsigned>(this->state.stateCnt - 6) >= 4) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 100, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 1;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\white02.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  const std::int16_t phase = kind;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = -std::max(rand() % 31, 0);
    this->state.unknown_11C84->slots[index].maxHeight = kind == 3 ? 0.05f : 0.1f;
    this->state.unknown_11C84->slots[index].alphaB = static_cast<std::int16_t>(std::max(rand() % 121, 0) + 125);
    this->state.unknown_11C84->slots[index].riseAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(std::max(rand() % 6, 0)) + 6.0f;
    this->state.unknown_11C84->slots[index].height[0] = 0.0f;
    this->state.unknown_11C84->slots[index].vecTNow.x = static_cast<float>(std::max(rand() % 7, 0)) * 0.1f + 0.8f;
    this->state.unknown_11C84->slots[index].vecBNow.x = this->render.position.x;
    this->state.unknown_11C84->slots[index].vecBNow.y = this->render.position.y;
    this->state.unknown_11C84->slots[index].vecBNow.z = this->render.position.z;
    this->state.unknown_11C84->slots[index].vecTNow.y = 4.0f;

    for (int height = 20; height > 0; --height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[index].alphaT = phase;
  }
}

// OFFSET: 0x00C51F50
// FUNCTION: fn_20192
void CRagEffect::GrandCrossRings(short angle, char kind) {
  this->render.position = this->state.parent->render.position;
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\cru_grand cross.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 34, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);

  if (kind == 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_white.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 1.0f;
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_black.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 12.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 90;
  this->state.unknown_11C84->slots[0].maxHeight = 120.0f;
  this->state.unknown_11C84->slots[0].alphaB = angle;
  this->state.unknown_11C84->slots[0].distance = 19.9f;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].flag1[4] = kind;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 90;
  this->state.unknown_11C84->slots[1].maxHeight = 120.0f;
  this->state.unknown_11C84->slots[1].alphaB = angle;
  this->state.unknown_11C84->slots[1].distance = 19.9f;
  this->state.unknown_11C84->slots[1].riseAngle = 90;
  this->state.unknown_11C84->slots[1].flag1[4] = kind;

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 90;
  this->state.unknown_11C84->slots[2].maxHeight = 120.0f;
  this->state.unknown_11C84->slots[2].alphaB = angle;
  this->state.unknown_11C84->slots[2].distance = 19.9f;
  this->state.unknown_11C84->slots[2].riseAngle = 90;
  this->state.unknown_11C84->slots[2].flag1[4] = kind;

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 90;
  this->state.unknown_11C84->slots[3].maxHeight = 120.0f;
  this->state.unknown_11C84->slots[3].alphaB = angle;
  this->state.unknown_11C84->slots[3].distance = 19.9f;
  this->state.unknown_11C84->slots[3].riseAngle = 90;
  this->state.unknown_11C84->slots[3].flag1[4] = kind;

  this->state.unknown_11C84->slots[0].rotStart = 180;
  this->state.unknown_11C84->slots[0].flag1[0] = 23;
  this->state.unknown_11C84->slots[0].flag1[1] = 23;
  this->state.unknown_11C84->slots[1].rotStart = 90;
  this->state.unknown_11C84->slots[1].flag1[0] = 23;
  this->state.unknown_11C84->slots[1].flag1[1] = -23;
  this->state.unknown_11C84->slots[2].rotStart = 0;
  this->state.unknown_11C84->slots[2].flag1[0] = -23;
  this->state.unknown_11C84->slots[2].flag1[1] = -23;
  this->state.unknown_11C84->slots[3].rotStart = 270;
  this->state.unknown_11C84->slots[3].flag1[0] = -23;
  this->state.unknown_11C84->slots[3].flag1[1] = 23;
}

// OFFSET: 0x00C44C20
// FUNCTION: fn_20146
void CRagEffect::WaterfallSheets(char first, char second, char third) {
  if (this->state.stateCnt != 0) {
    return;
  }

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 79, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 3;
  AllocateTextures(*this->state.unknown_11C84);

  if (third == 0) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\waterfall11.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\waterfall12.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\waterfall13.tga", 0, false, true, false);
  } else if (third == 1) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\waterfall31.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\waterfall32.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\waterfall33.tga", 0, false, true, false);
  } else if (third == 10) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\alpha_down.tga", 0, false, true, false);
  }

  int width = 80;
  int tall = 30;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 1;
    this->state.unknown_11C84->slots[index].process = 0;
    this->state.unknown_11C84->slots[index].alphaB = 120;
    this->state.unknown_11C84->slots[index].flag1[0] = first;
    this->state.unknown_11C84->slots[index].distance = static_cast<float>(index + 36);
    this->state.unknown_11C84->slots[index].flag1[2] = 10;

    for (int height = 0; height < 20; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = static_cast<float>(height * 40);
    }

    this->state.unknown_11C84->slots[index].vecBNow.z = static_cast<float>(index) * 0.25f - 1.0f;

    if (second == 0) {
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(width);
      this->state.unknown_11C84->slots[index].flag1[1] = 1;
    } else if (second == 1) {
      this->state.unknown_11C84->slots[index].maxHeight = static_cast<float>(tall);
      this->state.unknown_11C84->slots[index].flag1[1] = 1;
    } else if (second == 10) {
      this->state.unknown_11C84->slots[index].maxHeight = 0.0f;
      this->state.unknown_11C84->slots[index].alphaB = 70;
      this->state.unknown_11C84->slots[index].flag1[1] = 10;
    } else {
      this->state.unknown_11C84->slots[index].flag1[1] = 1;
    }

    tall -= 6;
    width -= 13;

    if (width < 29) {
      return;
    }
  }
}

// OFFSET: 0x00C63400
// FUNCTION: fn_20255
void CRagEffect::TexturedBlueRing(const char* name) {
  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 121, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;
  this->state.unknown_11C84->deltaPos = {this->state.deltaPos[0], this->state.deltaPos[1], this->state.deltaPos[2]};
  this->state.unknown_11C84->deltaPos2 = {this->state.deltaPos2[0], this->state.deltaPos2[1], this->state.deltaPos2[2]};

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[0].maxHeight = 13.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = this->state.deltaPos2[1] == 0.0f ? 3.4f : 2.4f;
  this->state.unknown_11C84->slots[0].riseAngle = 55;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[1].maxHeight = 11.0f;
  this->state.unknown_11C84->slots[1].rotStart = 90;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = this->state.deltaPos2[1] == 0.0f ? 3.6f : 2.6f;
  this->state.unknown_11C84->slots[1].riseAngle = 50;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[2].maxHeight = 9.0f;
  this->state.unknown_11C84->slots[2].rotStart = 180;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = this->state.deltaPos2[1] == 0.0f ? 3.8f : 2.8f;
  this->state.unknown_11C84->slots[2].riseAngle = 45;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[3].life = 0;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 21; ++j) {
      this->state.unknown_11C84->slots[i].height[j] = 0.0f;
      this->state.unknown_11C84->slots[i].flag1[j] = 0;
    }
  }
}

// OFFSET: 0x00C48ED0
// FUNCTION: fn_20165
void CRagEffect::EmpShockBurst(const char* name, char flag) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt >= 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  }

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 132, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 0;
    this->state.unknown_11C84->renderFlag |= 1;
    this->state.unknown_11C84->renderFlag |= 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 1.0f;

    int process = 0;

    for (int i = 0; process > -80; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = process;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].maxHeight = 10.0f;
      this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].alphaB = 200;
      this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(i);
      this->state.unknown_11C84->slots[i].height[4] = 10.0f;
      this->state.unknown_11C84->slots[i].height[5] = static_cast<float>(flag);
      process -= 20;
    }
  }

  if (this->state.stateCnt == 10) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 132, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 0;
    this->state.unknown_11C84->renderFlag |= 1;
    this->state.unknown_11C84->renderFlag |= 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 1.0f;

    int process = 0;

    for (int i = 0; process > -80; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = process;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].maxHeight = 10.0f;
      this->state.unknown_11C84->slots[i].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].alphaB = 200;
      this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(i);
      this->state.unknown_11C84->slots[i].height[4] = 10.0f;
      this->state.unknown_11C84->slots[i].height[5] = static_cast<float>(flag);
      process -= 20;
    }
  }
}

// OFFSET: 0x00C64CA0
// FUNCTION: fn_20264
void CPc::Slot29(int value) {
  this->unknown_0460[0] = value;
}

// OFFSET: 0x00C46410
// FUNCTION: fn_20153
void CRagEffect::TexturedMagicRing(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 48, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int i = 0; i < 4; ++i) {
    if (kind == 0) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].vecBPre.x = static_cast<float>(std::max(rand() % 40, 0) - 20);
      this->state.unknown_11C84->slots[i].vecBPre.z = static_cast<float>(std::max(rand() % 40, 0) - 20);
      this->state.unknown_11C84->slots[i].vecBNow.x = 0.0f;
      this->state.unknown_11C84->slots[i].vecBNow.z = 0.0f;
      this->state.unknown_11C84->slots[i].alphaB = 15;
    } else if (kind == 1 || kind == 2) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      if (i < 2) {
        this->state.unknown_11C84->slots[i].vecBPre.x = static_cast<float>(std::max(rand() % 7, 0)) - 3.0f;
        this->state.unknown_11C84->slots[i].vecBPre.z = -3.0f;
        this->state.unknown_11C84->slots[i].vecBNow.x = -this->state.unknown_11C84->slots[i].vecBPre.x;
        this->state.unknown_11C84->slots[i].vecBNow.z = 3.0f;
      } else {
        this->state.unknown_11C84->slots[i].vecBPre.x = -3.0f;
        this->state.unknown_11C84->slots[i].vecBPre.z = static_cast<float>(std::max(rand() % 7, 0)) - 3.0f;
        this->state.unknown_11C84->slots[i].vecBNow.x = 3.0f;
        this->state.unknown_11C84->slots[i].vecBNow.z = -this->state.unknown_11C84->slots[i].vecBPre.z;
      }
      this->state.unknown_11C84->slots[i].alphaB = 50;
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(kind);
    } else if (kind == 3 || kind == 4) {
      this->state.unknown_11C84->slots[i].life = i < 2 ? 1 : 0;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
      if (i < 1) {
        this->state.unknown_11C84->slots[i].vecBPre.x = 6.0f;
        this->state.unknown_11C84->slots[i].vecBPre.z = 0.0f;
        this->state.unknown_11C84->slots[i].vecBNow.x = -6.0f;
        this->state.unknown_11C84->slots[i].vecBNow.z = 0.0f;
      } else {
        this->state.unknown_11C84->slots[i].vecBPre.x = 0.0f;
        this->state.unknown_11C84->slots[i].vecBPre.z = -6.0f;
        this->state.unknown_11C84->slots[i].vecBNow.x = 0.0f;
        this->state.unknown_11C84->slots[i].vecBNow.z = 6.0f;
      }
      this->state.unknown_11C84->slots[i].alphaB = 100;
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(kind);
    }
  }
}

// OFFSET: 0x00C48820
// FUNCTION: fn_20163
void CRagEffect::IncAgilityCenter(char kind) {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  g_renderer->viewport.Projection().Project(&this->state.parent->render.position.x, view, vertex);
  this->state.tlvertX = vertex.x;
  this->state.tlvertY = vertex.y;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, kind == 3 ? "effect\\소울 체인지.wav" : "effect\\EF_IncAgility.wav");
  }

  if ((this->state.stateCnt & 1) != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 16, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = 50;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(std::max(rand() % 360, 0)));
  const float distance = static_cast<float>(std::max(rand() % 7, 0) + 2);
  this->state.unknown_11C84->deltaPos2 = RotatedForward(this->state.unknown_11C84->transform, distance);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->transform)->MakeXRotation(GetRadian(90));

  if (kind >= 2) {
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 40) / 100.0f;
    this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 100, 0) + 40) / 10.0f;
    this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 6, 0)) * 0.05f + 0.1f;
  } else {
    this->state.unknown_11C84->speed = static_cast<float>(std::max(rand() % 50, 0) + 20) / 100.0f;
    this->state.unknown_11C84->widthSize = static_cast<float>(std::max(rand() % 100, 0) + 20) / 10.0f;
    this->state.unknown_11C84->heightSize = static_cast<float>(std::max(rand() % 4, 0)) * 0.1f + 0.1f;
  }

  this->state.unknown_11C84->alpha = 0.0f;
  this->state.unknown_11C84->maxAlpha = 60.0f;
  this->state.unknown_11C84->alphaSpeed = this->state.unknown_11C84->maxAlpha / 20.0f;
  this->state.unknown_11C84->fadeOutCnt = this->state.unknown_11C84->duration - 20;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ac_center2.tga", 0, false, true, false);
}

// OFFSET: 0x00C4F650
// FUNCTION: fn_20182
void CRagEffect::SonicSlashFan(char kind) {
  if (this->state.stateCnt > 4) {
    return;
  }

  for (int angle = -90; angle < 180; angle += 90) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->duration = 60;
    this->state.unknown_11C84->deltaPos2 = {0.0f, -10.0f, 0.0f};
    this->state.unknown_11C84->radius = Owner(this->state.unknown_11C84).CalcDist(this->render.position.x, this->render.position.z);
    Owner(this->state.unknown_11C84).Face(
      static_cast<float>(std::max(rand() % 7, 0)) + this->render.position.x - 3.0f,
      static_cast<float>(std::max(rand() % 7, 0)) + this->render.position.z - 3.0f
    );
    this->state.unknown_11C84->longitude = -Owner(this->state.unknown_11C84).render.angle;
    const float speed = this->state.unknown_11C84->radius / this->state.unknown_11C84->duration;
    this->state.unknown_11C84->speed = speed + speed;
    this->state.unknown_11C84->gravSpeed = 2.0f;
    const float rise = -(this->state.unknown_11C84->gravSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->gravAccel = rise + rise;
    this->state.unknown_11C84->latiSpeed = -2.0f;
    const float spread = -(this->state.unknown_11C84->latiSpeed / this->state.unknown_11C84->duration);
    this->state.unknown_11C84->latiAccel = spread + spread;
    this->state.unknown_11C84->roll = static_cast<float>(angle);
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.transform)->MakeYRotation(GetRadian(static_cast<int>(particle.longitude)));
    this->state.unknown_11C84->deltaPos3 = RotatedForward(this->state.unknown_11C84->transform, 1.0f);
    this->state.unknown_11C84->rotAxis[0] = this->state.unknown_11C84->deltaPos3.x;
    this->state.unknown_11C84->rotAxis[1] = this->state.unknown_11C84->deltaPos3.y;
    this->state.unknown_11C84->rotAxis[2] = this->state.unknown_11C84->deltaPos3.z;

    if (kind == 0) {
      this->state.unknown_11C84->red = std::max(rand() % 81, 0) + 160;
      this->state.unknown_11C84->green = 255;
      this->state.unknown_11C84->blue = std::max(rand() % 81, 0) + 160;
    } else if (kind == 1) {
      const int color = std::max(rand() % 55, 0) + 200;
      this->state.unknown_11C84->red = color;
      this->state.unknown_11C84->green = color;
      this->state.unknown_11C84->blue = color;
    } else if (kind == 2) {
      const int color = std::max(rand() % 100, 0) + 20;
      this->state.unknown_11C84->red = color;
      this->state.unknown_11C84->green = color;
      this->state.unknown_11C84->blue = color;
    }

    this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 1.0f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->numSegment = 7;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C65AD0
// FUNCTION: fn_20271
void CRagEffect::TexturedStorm2(const char* name, char kind) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 111, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
    this->state.unknown_11C84->slots[0].life = 0;
    this->state.unknown_11C84->slots[1].life = 0;
    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;

    for (int i = 0; i < 2; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].maxHeight = 0.07f;
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 1.0f;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].vecBPre.x = this->state.deltaPos[0];
      this->state.unknown_11C84->slots[i].vecBPre.y = this->state.deltaPos[1];
      this->state.unknown_11C84->slots[i].vecBPre.z = this->state.deltaPos[2];
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[12] = 0.0f;
      this->state.unknown_11C84->slots[i].vecBNow.x = this->state.deltaPos[0];
      this->state.unknown_11C84->slots[i].vecBNow.y = this->state.deltaPos[1];
      this->state.unknown_11C84->slots[i].vecBNow.z = this->state.deltaPos[2];
      this->state.unknown_11C84->slots[i].height[4] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[5] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[6] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[7] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[13] = 0.0f;
      this->state.unknown_11C84->slots[i].vecTPre.x = this->state.deltaPos[0];
      this->state.unknown_11C84->slots[i].vecTPre.y = this->state.deltaPos[1];
      this->state.unknown_11C84->slots[i].vecTPre.z = this->state.deltaPos[2];
      this->state.unknown_11C84->slots[i].height[8] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[9] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[10] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[11] = static_cast<float>(std::max(rand() % 360, 0));
      this->state.unknown_11C84->slots[i].height[14] = 0.0f;

      if (kind == 2) {
        this->state.unknown_11C84->slots[i].height[15] = 4.0f;
      } else if (kind == 3) {
        this->state.unknown_11C84->slots[i].height[15] = static_cast<float>(rand() % 3 + 1);
      } else {
        this->state.unknown_11C84->slots[i].height[15] = 0.0f;
      }
    }
  }
}

// OFFSET: 0x00C57830
// FUNCTION: fn_20205
void CRagEffect::RingRedPulse(const char* name, float x, float z) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 110, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 10.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 12.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 2.3f;
  this->state.unknown_11C84->slots[0].riseAngle = 86;
  this->state.unknown_11C84->slots[0].alphaT = static_cast<std::int16_t>(this->state.duration) - 25;
  this->state.unknown_11C84->slots[0].vecBNow.x = x;
  this->state.unknown_11C84->slots[0].vecBNow.y = 1234.5f;
  this->state.unknown_11C84->slots[0].vecBNow.z = z;

  for (int j = 0; j < 21; ++j) {
    this->state.unknown_11C84->slots[0].height[j] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[j] = 33;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 25.0f;
  this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 2.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 90;
  this->state.unknown_11C84->slots[1].alphaT = static_cast<std::int16_t>(this->state.duration) - 25;
  this->state.unknown_11C84->slots[1].vecBNow.x = x;
  this->state.unknown_11C84->slots[1].vecBNow.y = 1234.5f;
  this->state.unknown_11C84->slots[1].vecBNow.z = z;

  for (int j = 0; j < 21; ++j) {
    this->state.unknown_11C84->slots[1].height[j] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[j] = 33;
  }

  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C47990
// FUNCTION: fn_20159
void CRagEffect::LineStreakLayered(short kind) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 56, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0x201;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] =
    g_texMgr.Load(kind < 10 ? "effect\\line3.tga" : "effect\\line3y.tga", 0, false, true, false);
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].distance = 100.0f;

    if (kind % 10 == 0) {
      this->state.unknown_11C84->slots[i].height[0] = -15.0f - static_cast<float>(std::max(rand() % 11, 0));
    } else if (kind % 10 == 1) {
      this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 11, 0)) + 15.0f;
    }

    this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 200, 0));

    if (kind < 10) {
      this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(rand() % 4) * 0.1f + 0.3f;
      this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 20.0f;
      this->state.unknown_11C84->slots[i].flag1[0] = static_cast<char>(rand() % 3);
      this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 21, 0)) * 0.1f + 1.5f;
    } else if (kind > 19) {
      this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(rand() % 4) * 0.1f + 0.4f;
      this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 30.0f;
      this->state.unknown_11C84->slots[i].flag1[0] = static_cast<char>(rand() % 3 + 20);
      this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 21, 0)) * 0.1f + 1.5f;
    } else {
      this->state.unknown_11C84->slots[i].maxHeight = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + g_depthAverageHalf;
      this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 31, 0)) + 40.0f;
      this->state.unknown_11C84->slots[i].flag1[0] = static_cast<char>(rand() % 3 + 10);
      this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 11, 0)) * 0.1f + 1.0f;
    }

    Get_Angle(
      this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z,
      &this->state.unknown_11C84->slots[i].riseAngle
    );
    this->state.unknown_11C84->slots[i].alphaB = 0;
  }
}

// OFFSET: 0x00C50980

// OFFSET: 0x00C510C0
// FUNCTION: fn_20187
void CRagEffect::TexturedSlashAir(const char* name, char size, char phase) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 108, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = static_cast<float>(size);

  for (int i = 0; i < 4; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].fullDisplayAngle = 315;
    this->state.unknown_11C84->slots[i].alphaB = 0;
    this->state.unknown_11C84->slots[i].vecBNow.y = 0.0f;

    if (phase == 0) {
      if (i == 0) {
        this->state.unknown_11C84->slots[i].distance = 4.5f;
        this->state.unknown_11C84->slots[i].riseAngle = 85;
        this->state.unknown_11C84->slots[i].maxHeight = 27.0f;
        this->state.unknown_11C84->slots[i].rotStart = 0;
      } else if (i == 1) {
        this->state.unknown_11C84->slots[i].distance = 5.0f;
        this->state.unknown_11C84->slots[i].riseAngle = 80;
        this->state.unknown_11C84->slots[i].maxHeight = 28.0f;
        this->state.unknown_11C84->slots[i].rotStart = 90;
      } else if (i == 2) {
        this->state.unknown_11C84->slots[i].distance = 5.5f;
        this->state.unknown_11C84->slots[i].riseAngle = 70;
        this->state.unknown_11C84->slots[i].maxHeight = 29.0f;
        this->state.unknown_11C84->slots[i].rotStart = 180;
      } else if (i == 3) {
        this->state.unknown_11C84->slots[i].distance = 5.0f;
        this->state.unknown_11C84->slots[i].riseAngle = 75;
        this->state.unknown_11C84->slots[i].maxHeight = 30.0f;
        this->state.unknown_11C84->slots[i].rotStart = 270;
      }
    } else if (phase == 1) {
      if (i == 0) {
        this->state.unknown_11C84->slots[i].distance = 5.5f;
        this->state.unknown_11C84->slots[i].riseAngle = 50;
        this->state.unknown_11C84->slots[i].maxHeight = 30.0f;
        this->state.unknown_11C84->slots[i].rotStart = 90;
      } else if (i == 1) {
        this->state.unknown_11C84->slots[i].distance = 6.0f;
        this->state.unknown_11C84->slots[i].riseAngle = 55;
        this->state.unknown_11C84->slots[i].maxHeight = 29.0f;
        this->state.unknown_11C84->slots[i].rotStart = 180;
      } else if (i == 2) {
        this->state.unknown_11C84->slots[i].distance = 6.5f;
        this->state.unknown_11C84->slots[i].riseAngle = 65;
        this->state.unknown_11C84->slots[i].maxHeight = 28.0f;
        this->state.unknown_11C84->slots[i].rotStart = 270;
      } else if (i == 3) {
        this->state.unknown_11C84->slots[i].distance = 6.0f;
        this->state.unknown_11C84->slots[i].riseAngle = 60;
        this->state.unknown_11C84->slots[i].maxHeight = 27.0f;
        this->state.unknown_11C84->slots[i].rotStart = 0;
      }
    }

    this->state.unknown_11C84->slots[i].vecTNow.x = static_cast<float>(this->state.unknown_11C84->slots[i].riseAngle);
    this->state.unknown_11C84->slots[i].vecTNow.y = this->state.unknown_11C84->slots[i].maxHeight;

    for (int j = 0; j < 21; ++j) {
      this->state.unknown_11C84->slots[i].height[j] = 0.0f;
      this->state.unknown_11C84->slots[i].flag1[j] = 0;
    }
  }
}

// OFFSET: 0x00C54E60
// FUNCTION: fn_20201
void CRagEffect::SuperSlashFan(char count, char value) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 35, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 5;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\super1.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\super2.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\super3.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\super4.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\super5.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int i = 0; i < count; ++i) {
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].distance = 3.5f;
    this->state.unknown_11C84->slots[i].vecBPre.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[0] = 10.0f;
    this->state.unknown_11C84->slots[i].height[1] = 70.0f;
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecBNow.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[4] = 10.0f;
    this->state.unknown_11C84->slots[i].height[5] = 130.0f;
    this->state.unknown_11C84->slots[i].height[6] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[7] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecTPre.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[8] = 10.0f;
    this->state.unknown_11C84->slots[i].height[9] = 190.0f;
    this->state.unknown_11C84->slots[i].height[10] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[11] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecTNow.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[12] = 10.0f;
    this->state.unknown_11C84->slots[i].height[13] = 250.0f;
    this->state.unknown_11C84->slots[i].height[14] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[15] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[4] = value;
  }
}

// OFFSET: 0x00C4AF00
// FUNCTION: fn_20173
void CRagEffect::SuperSlashSpread(char count, char value) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 99, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->totalTexture = 5;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\super1.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\super2.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\super3.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\super4.bmp", 0, false, true, false);
  this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\super5.bmp", 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  for (int i = 0; i < count; ++i) {
    this->state.unknown_11C84->slots[i].process = 0;
    this->state.unknown_11C84->slots[i].life = 1;
    this->state.unknown_11C84->slots[i].distance = 2.0f;
    this->state.unknown_11C84->slots[i].vecBPre.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[0] = 10.0f;
    this->state.unknown_11C84->slots[i].height[1] = 40.0f;
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecBNow.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[4] = 10.0f;
    this->state.unknown_11C84->slots[i].height[5] = 80.0f;
    this->state.unknown_11C84->slots[i].height[6] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[7] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecTPre.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[8] = 10.0f;
    this->state.unknown_11C84->slots[i].height[9] = 120.0f;
    this->state.unknown_11C84->slots[i].height[10] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[11] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].vecTNow.y = -static_cast<float>(std::max(rand() % 10, 0));
    this->state.unknown_11C84->slots[i].height[12] = 10.0f;
    this->state.unknown_11C84->slots[i].height[13] = 160.0f;
    this->state.unknown_11C84->slots[i].height[14] = static_cast<float>(std::max(rand() % 5, 0));
    this->state.unknown_11C84->slots[i].height[15] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].flag1[4] = value;
  }
}

// OFFSET: 0x00C74B60
// FUNCTION: fn_20334
void CPlayer::RefreshEffect164() {
  this->SendMsg(nullptr, 109, 0, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->unknown_0544 = this->LaunchEffect(164, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
}

// OFFSET: 0x00C40DF0

// OFFSET: 0x00C4FAB0
// FUNCTION: fn_20183
void CRagEffect::PortalRings() {
  this->render.position.x = this->state.parent->render.position.x;
  this->render.position.y = this->state.parent->render.position.y;
  this->render.position.z = this->state.parent->render.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  g_soundMgr->PlayWave(
    "effect\\EF_Portal.wav",
    this->render.position.x - player->render.position.x,
    -150.0f,
    this->render.position.z - player->render.position.z,
    250,
    40,
    1.0f,
    0
  );

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ring_blue.tga", 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 30.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 3.7f;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].alphaT = 1400;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[0].height[i] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[i] = 2;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 30.0f;
  this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 3.4f;
  this->state.unknown_11C84->slots[1].riseAngle = 90;
  this->state.unknown_11C84->slots[1].alphaT = 1400;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[1].height[i] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[i] = 2;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 4.0f;
  this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 3.6f;
  this->state.unknown_11C84->slots[2].riseAngle = 10;
  this->state.unknown_11C84->slots[2].alphaT = 1400;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[2].height[i] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[i] = 2;
  }

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = 4.0f;
  this->state.unknown_11C84->slots[3].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[3].alphaB = 0;
  this->state.unknown_11C84->slots[3].distance = 3.7f;
  this->state.unknown_11C84->slots[3].riseAngle = 5;
  this->state.unknown_11C84->slots[3].alphaT = 1400;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[3].height[i] = 0.0f;
    this->state.unknown_11C84->slots[3].flag1[i] = 2;
  }
}

// OFFSET: 0x00C46860
// FUNCTION: fn_20154
void CRagEffect::TexturedGroundRing(const char* name, char flag, int unused1, int unused2) {
  this->render.position.x = this->state.parent->render.position.x;
  this->render.position.y = this->state.parent->render.position.y;
  this->render.position.z = this->state.parent->render.position.z;

  if (this->state.stateCnt > 3) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 169, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = static_cast<float>(flag);

  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 15.0f;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].alphaB = static_cast<std::int16_t>(std::max(rand() % 100, 0));
  this->state.unknown_11C84->slots[0].distance = static_cast<float>(this->state.stateCnt * 2) + g_depthAverageHalf;
  this->state.unknown_11C84->slots[0].vecTNow.x = this->state.unknown_11C84->slots[0].distance;
  this->state.unknown_11C84->slots[0].distance -= static_cast<float>(rand() % 21) * 0.1f;
  this->state.unknown_11C84->slots[0].riseAngle = 80;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[0].height[i] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[i] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 16.0f;
  this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[1].alphaB = static_cast<std::int16_t>(std::max(rand() % 100, 0));
  this->state.unknown_11C84->slots[1].distance = static_cast<float>(this->state.stateCnt * 2) + 1.0f;
  this->state.unknown_11C84->slots[1].vecTNow.x = this->state.unknown_11C84->slots[1].distance;
  this->state.unknown_11C84->slots[1].distance += static_cast<float>(rand() % 21) * 0.1f;
  this->state.unknown_11C84->slots[1].riseAngle = 80;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[1].height[i] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[i] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 17.0f;
  this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[2].alphaB = static_cast<std::int16_t>(std::max(rand() % 100, 0));
  this->state.unknown_11C84->slots[2].distance = static_cast<float>(this->state.stateCnt * 2) + 1.5f;
  this->state.unknown_11C84->slots[2].vecTNow.x = this->state.unknown_11C84->slots[2].distance;
  this->state.unknown_11C84->slots[2].distance -= static_cast<float>(rand() % 21) * 0.1f;
  this->state.unknown_11C84->slots[2].riseAngle = 80;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[2].height[i] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[i] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = 18.0f;
  this->state.unknown_11C84->slots[3].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[3].alphaB = static_cast<std::int16_t>(std::max(rand() % 100, 0));
  this->state.unknown_11C84->slots[3].distance = static_cast<float>(this->state.stateCnt * 2) + 2.0f;
  this->state.unknown_11C84->slots[3].vecTNow.x = this->state.unknown_11C84->slots[3].distance;
  this->state.unknown_11C84->slots[3].distance += static_cast<float>(rand() % 21) * 0.1f;
  this->state.unknown_11C84->slots[3].riseAngle = 80;

  for (int i = 0; i < 21; ++i) {
    this->state.unknown_11C84->slots[3].height[i] = 0.0f;
    this->state.unknown_11C84->slots[3].flag1[i] = 0;
  }
}

// OFFSET: 0x00C5CC00
// FUNCTION: fn_20230
void CRagEffect::sub_C5CC00() {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 99999998) {
    this->state.duration = 99999999;
    this->state.stateCnt = 0;
  } else if ((this->state.stateCnt & 1) != 0 || this->state.stateCnt >= 200) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 40, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->duration = this->state.duration;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].maxHeight = static_cast<float>(std::max(rand() % 4, 0) + 2) * 0.03f;
  this->state.unknown_11C84->size = static_cast<float>(std::max(rand() % 6, 0) + 35) / 100.0f;
  this->state.unknown_11C84->slots[0].vecBPre.x = static_cast<float>(std::max(rand() % 300, 0)) + this->render.position.x - 150.0f;
  this->state.unknown_11C84->slots[0].height[0] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[1] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[2] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].height[3] = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 200.0f;
  this->state.unknown_11C84->slots[0].vecBPre.z = static_cast<float>(std::max(rand() % 300, 0)) + this->render.position.z - 150.0f;
  this->state.unknown_11C84->slots[0].flag1[0] = 1;
  this->state.unknown_11C84->alpha = 200.0f;
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\단풍.act");
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\단풍.spr");
  Owner(this->state.unknown_11C84).SetAction(0, 36, 0);
  Owner(this->state.unknown_11C84).InitSegment();
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
}

// OFFSET: 0x00C526D0
// FUNCTION: fn_20195
bool CRagEffect::GlassWallRising(const char* name) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_GlassWall.wav");
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
    this->state.unknown_11C84->size = 10.0f;

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[0].maxHeight = 34.0f;
    this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].distance = 3.6f;
    this->state.unknown_11C84->slots[0].riseAngle = 90;
    this->state.unknown_11C84->slots[0].alphaT = 5500;
    this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[0].height[height] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[1].maxHeight = 37.0f;
    this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].distance = 3.3f;
    this->state.unknown_11C84->slots[1].riseAngle = 90;
    this->state.unknown_11C84->slots[1].alphaT = 3080;
    this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[1].height[height] = 0.0f;
      this->state.unknown_11C84->slots[1].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[2].life = 1;
    this->state.unknown_11C84->slots[2].process = 0;
    this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[2].maxHeight = 40.0f;
    this->state.unknown_11C84->slots[2].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[2].alphaB = 0;
    this->state.unknown_11C84->slots[2].distance = 3.0f;
    this->state.unknown_11C84->slots[2].riseAngle = 90;
    this->state.unknown_11C84->slots[2].alphaT = 3080;
    this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[2].height[height] = 0.0f;
      this->state.unknown_11C84->slots[2].flag1[height] = 0;
    }

    this->state.unknown_11C84->slots[3].life = 0;
  }

  this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
  return this->state.stateCnt != this->state.duration;
}

// OFFSET: 0x00C62B60
// FUNCTION: fn_20253
void CRagEffect::TexturedQuadRingSized(std::int16_t baseAlpha, const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  if (this->state.duration < 56) {
    this->state.duration = 56;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 30, 0.0f, 0.0f, 0.0f, WorldObjects())->state;

  if (this->state.unknown_11C84 == nullptr) {
    return;
  }

  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

  if (kind == 1) {
    this->state.unknown_11C84->size = 8.0f;
  } else if (kind == 2) {
    this->state.unknown_11C84->size = 5.0f;
  } else if (kind == 3) {
    this->state.unknown_11C84->size = 10.0f;
  } else if (kind == 4) {
    this->state.unknown_11C84->size = 11.0f;
  } else if (kind == 5) {
    this->state.unknown_11C84->size = 4.0f;
  } else if (kind == 6) {
    this->state.unknown_11C84->size = 12.0f;
  } else if (kind == 15) {
    this->state.unknown_11C84->size = 7.0f;
  } else if (kind == 20) {
    this->state.unknown_11C84->size = 24.0f;
  } else if (kind == 21) {
    this->state.unknown_11C84->size = 25.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->size = 26.0f;
  } else if (kind == 23) {
    this->state.unknown_11C84->size = 27.0f;
  } else if (kind == 31) {
    this->state.unknown_11C84->size = 31.0f;
  } else if (kind == 42) {
    this->state.unknown_11C84->size = 42.0f;
  } else if (kind == 43) {
    this->state.unknown_11C84->size = 43.0f;
  } else if (kind == 44) {
    this->state.unknown_11C84->size = 44.0f;
  } else if (kind == 45) {
    this->state.unknown_11C84->size = 45.0f;
  } else if (kind == 46) {
    this->state.unknown_11C84->size = 46.0f;
  } else if (kind == 47) {
    this->state.unknown_11C84->size = 47.0f;
  } else if (kind == 49) {
    this->state.unknown_11C84->size = 49.0f;
  } else if (kind == 50) {
    this->state.unknown_11C84->size = 50.0f;
  } else {
    this->state.unknown_11C84->size = 5.0f;
  }

  if (kind == 48) {
    this->state.unknown_11C84->size = 48.0f;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[0].maxHeight = 25.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[0].maxHeight = 15.0f;
  } else {
    this->state.unknown_11C84->slots[0].maxHeight = 20.0f;
  }

  this->state.unknown_11C84->slots[0].rotStart = 180;
  this->state.unknown_11C84->slots[0].alphaB = baseAlpha + 135;
  this->state.unknown_11C84->slots[0].distance = 4.1f;
  this->state.unknown_11C84->slots[0].riseAngle = 80;
  this->state.unknown_11C84->slots[0].alphaT = kind;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[1].maxHeight = 24.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[1].maxHeight = 14.0f;
  } else {
    this->state.unknown_11C84->slots[1].maxHeight = 19.0f;
  }

  this->state.unknown_11C84->slots[1].rotStart = 270;
  this->state.unknown_11C84->slots[1].alphaB = baseAlpha + 90;
  this->state.unknown_11C84->slots[1].distance = 4.1f;
  this->state.unknown_11C84->slots[1].riseAngle = 80;
  this->state.unknown_11C84->slots[1].alphaT = kind;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[2].maxHeight = 23.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[2].maxHeight = 13.0f;
  } else {
    this->state.unknown_11C84->slots[2].maxHeight = 18.0f;
  }

  this->state.unknown_11C84->slots[2].rotStart = 0;
  this->state.unknown_11C84->slots[2].alphaB = baseAlpha + 45;
  this->state.unknown_11C84->slots[2].distance = 4.1f;
  this->state.unknown_11C84->slots[2].riseAngle = 80;
  this->state.unknown_11C84->slots[2].alphaT = kind;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  if (kind == 2) {
    this->state.unknown_11C84->slots[3].maxHeight = 22.0f;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[3].maxHeight = 12.0f;
  } else {
    this->state.unknown_11C84->slots[3].maxHeight = 17.0f;
  }

  this->state.unknown_11C84->slots[3].rotStart = 90;
  this->state.unknown_11C84->slots[3].alphaB = baseAlpha;
  this->state.unknown_11C84->slots[3].distance = 4.1f;
  this->state.unknown_11C84->slots[3].riseAngle = 80;
  this->state.unknown_11C84->slots[3].alphaT = kind;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  int delay = 0;
  for (int index = 0; index < 4; ++index) {
    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }

    if (this->state.duration <= 60) {
      this->state.unknown_11C84->slots[index].alphaB = 0;
      this->state.unknown_11C84->slots[index].process = delay;
    }

    delay -= 5;
  }
}

// OFFSET: 0x00C43B90
// FUNCTION: fn_20142
void CRagEffect::TexturedFireWall(const char* name, int unused) {
  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_FireWall.wav");

  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 118, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 13.0f;

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[0].maxHeight = 25.0f;
  this->state.unknown_11C84->slots[0].rotStart = 0;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 2.4f;
  this->state.unknown_11C84->slots[0].riseAngle = 70;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[1].maxHeight = 22.0f;
  this->state.unknown_11C84->slots[1].rotStart = 90;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 2.7f;
  this->state.unknown_11C84->slots[1].riseAngle = 57;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 315;
  this->state.unknown_11C84->slots[2].maxHeight = 19.0f;
  this->state.unknown_11C84->slots[2].rotStart = 180;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 3.0f;
  this->state.unknown_11C84->slots[2].riseAngle = 45;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = 30.0f;
  this->state.unknown_11C84->slots[3].rotStart = 0;
  this->state.unknown_11C84->slots[3].alphaB = 0;
  this->state.unknown_11C84->slots[3].distance = 2.2f;
  this->state.unknown_11C84->slots[3].riseAngle = 90;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[3].height[height] = 0.0f;
    this->state.unknown_11C84->slots[3].flag1[height] = 0;
  }
}

// OFFSET: 0x00C50220
// FUNCTION: fn_20184
void CRagEffect::TeleportSpiral(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_Teleportation.wav");

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 4.0f;

  this->state.unknown_11C84->slots[3].life = 1;
  this->state.unknown_11C84->slots[3].process = 0;
  this->state.unknown_11C84->slots[3].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[3].maxHeight = 20.0f;
  this->state.unknown_11C84->slots[3].rotStart = 0;
  this->state.unknown_11C84->slots[3].alphaB = 0;
  this->state.unknown_11C84->slots[3].distance = 2.4f;
  this->state.unknown_11C84->slots[3].riseAngle = 84;
  this->state.unknown_11C84->slots[3].alphaT = 60;
  this->state.unknown_11C84->slots[3].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[3].height[height] = 0.0f;
    this->state.unknown_11C84->slots[3].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[2].life = 1;
  this->state.unknown_11C84->slots[2].process = 0;
  this->state.unknown_11C84->slots[2].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[2].maxHeight = 45.0f;
  this->state.unknown_11C84->slots[2].rotStart = 90;
  this->state.unknown_11C84->slots[2].alphaB = 0;
  this->state.unknown_11C84->slots[2].distance = 2.2f;
  this->state.unknown_11C84->slots[2].riseAngle = 86;
  this->state.unknown_11C84->slots[2].alphaT = 60;
  this->state.unknown_11C84->slots[2].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[2].height[height] = 0.0f;
    this->state.unknown_11C84->slots[2].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[1].life = 1;
  this->state.unknown_11C84->slots[1].process = 0;
  this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[1].maxHeight = 70.0f;
  this->state.unknown_11C84->slots[1].rotStart = 180;
  this->state.unknown_11C84->slots[1].alphaB = 0;
  this->state.unknown_11C84->slots[1].distance = 2.0f;
  this->state.unknown_11C84->slots[1].riseAngle = 88;
  this->state.unknown_11C84->slots[1].alphaT = 60;
  this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[1].height[height] = 0.0f;
    this->state.unknown_11C84->slots[1].flag1[height] = 0;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
  this->state.unknown_11C84->slots[0].maxHeight = 180.0f;
  this->state.unknown_11C84->slots[0].rotStart = 270;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].distance = 1.8f;
  this->state.unknown_11C84->slots[0].riseAngle = 90;
  this->state.unknown_11C84->slots[0].alphaT = 58;
  this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

  for (int height = 0; height < 21; ++height) {
    this->state.unknown_11C84->slots[0].height[height] = 0.0f;
    this->state.unknown_11C84->slots[0].flag1[height] = 0;
  }
}

// OFFSET: 0x00C4B390
// FUNCTION: fn_20174
void CRagEffect::TexturedAura(const char* name, char kind) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 82, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);
  this->state.unknown_11C84->size = 0.0f;

  for (int index = 0; index < 4; ++index) {
    this->state.unknown_11C84->slots[index].life = 0;
  }

  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;

  if (kind == 1) {
    this->state.unknown_11C84->slots[0].distance = 8.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 0;
    this->state.unknown_11C84->slots[0].height[10] = 1.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 120;
  } else if (kind == 2) {
    this->state.unknown_11C84->slots[0].distance = 10.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 10.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 2;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  } else if (kind == 3) {
    this->state.unknown_11C84->slots[0].distance = 8.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 3;
    this->state.unknown_11C84->slots[0].height[10] = 2.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 155;
  } else if (kind == 4) {
    this->state.unknown_11C84->slots[0].distance = 6.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 0;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  } else if (kind == 10 || kind == 11) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] * 0.7f : 70.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 10;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 12) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] * 1.2f : 120.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 12;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 13) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] * 3.0f : 300.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 12;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 14) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] : 30.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 4;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 15) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] : 30.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 15;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 16 || kind == 17) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] * 0.9f : 9.5f;
    if (kind == 17) {
      this->state.unknown_11C84->slots[0].distance *= 0.85f;
    }

    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 5;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 18) {
    this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] * 0.9f : 9.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y;
    this->state.unknown_11C84->slots[0].flag1[2] = 5;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 19) {
    this->state.unknown_11C84->slots[0].distance = 8.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 15;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else if (kind == 20) {
    this->state.unknown_11C84->slots[0].distance = 2.4f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 27.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 0;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  } else if (kind == 21) {
    this->state.unknown_11C84->slots[0].distance = 24.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 10.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 4;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 90;
  } else if (kind == 22) {
    this->state.unknown_11C84->slots[0].distance = 4.0f;
    float lift = -30.0f;

    if (this->state.parent != nullptr) {
      CActRes* action = this->state.parent->GetActRes();

      if (action != nullptr) {
        const SpriteFrameCell& cell = action->Frame(0, 0).Cell(0);
        const float height = static_cast<float>(cell.y) * 0.29f;
        lift = height == 0.0f ? -30.0f : height - 4.0f;
      }
    }

    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y + lift;
    this->state.unknown_11C84->slots[0].flag1[2] = 16;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  } else if (kind == 30) {
    this->state.unknown_11C84->slots[0].distance = 24.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 19;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  } else if (kind == 31) {
    this->state.unknown_11C84->slots[0].distance = 2.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 0;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  } else if (kind == 32) {
    this->state.unknown_11C84->slots[0].distance = 8.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 30.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = kind;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  } else {
    this->state.unknown_11C84->slots[0].distance = 5.0f;
    this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 10.0f;
    this->state.unknown_11C84->slots[0].flag1[2] = 2;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
  }

  this->state.unknown_11C84->slots[0].height[8] = 0.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[4] = static_cast<char>(kind == 4 ? 1 : kind);
  this->state.unknown_11C84->slots[0].height[15] = this->render.position.y - this->state.unknown_11C84->slots[0].vecBPre.y;
}

// OFFSET: 0x00C56940
// FUNCTION: fn_20204
void CRagEffect::HealBurst(const char* name, char flag) {
  this->render.position.x = this->state.parent->render.position.x;
  this->render.position.y = this->state.parent->render.position.y;
  this->render.position.z = this->state.parent->render.position.z;

  const int limit = flag != 0 ? 25 : 50;

  if (this->state.stateCnt == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 60, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load(name, 0, false, true, false);

    if (flag == 0) {
      this->state.unknown_11C84->size = 9.0f;
    } else if (flag == 1) {
      this->state.unknown_11C84->size = 5.0f;
    }

    this->state.unknown_11C84->slots[0].life = 1;
    this->state.unknown_11C84->slots[0].process = 0;
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[0].alphaB = 0;
    this->state.unknown_11C84->slots[0].riseAngle = 90;
    this->state.unknown_11C84->slots[0].vecBNow.y = 0.0f;

    if (flag == 0) {
      this->state.unknown_11C84->slots[0].alphaT = 60;
      this->state.unknown_11C84->slots[0].maxHeight = 40.0f;
      this->state.unknown_11C84->slots[0].distance = 4.6f;
    } else if (flag == 1) {
      this->state.unknown_11C84->slots[0].alphaT = 30;
      this->state.unknown_11C84->slots[0].maxHeight = static_cast<float>(std::max(rand() % 20, 0)) + 40.0f;
      this->state.unknown_11C84->slots[0].distance = 4.5f;
      this->state.unknown_11C84->slots[0].distance += static_cast<float>(std::max(rand() % 100, 0)) * 0.005f;
    }

    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[0].height[i] = 0.0f;
      this->state.unknown_11C84->slots[0].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[1].life = 1;
    this->state.unknown_11C84->slots[1].process = 0;
    this->state.unknown_11C84->slots[1].fullDisplayAngle = 360;
    this->state.unknown_11C84->slots[1].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[1].alphaB = 0;
    this->state.unknown_11C84->slots[1].riseAngle = 90;
    this->state.unknown_11C84->slots[1].vecBNow.y = 0.0f;

    if (flag == 0) {
      this->state.unknown_11C84->slots[1].alphaT = 60;
      this->state.unknown_11C84->slots[1].maxHeight = 40.0f;
      this->state.unknown_11C84->slots[1].distance = 4.8f;
    } else if (flag == 1) {
      this->state.unknown_11C84->slots[1].alphaT = 30;
      this->state.unknown_11C84->slots[1].maxHeight = this->state.unknown_11C84->slots[0].maxHeight;
      this->state.unknown_11C84->slots[1].distance = this->state.unknown_11C84->slots[0].distance + 0.2f;
    }

    for (int i = 0; i < 21; ++i) {
      this->state.unknown_11C84->slots[1].height[i] = 0.0f;
      this->state.unknown_11C84->slots[1].flag1[i] = 0;
    }

    this->state.unknown_11C84->slots[2].life = 0;
    this->state.unknown_11C84->slots[3].life = 0;

    PlayEffectWave(*this, "_heal_effect.wav");
    return;
  }

  if (this->state.stateCnt > limit) {
    return;
  }

  if (this->state.stateCnt % 6 != 0) {
    return;
  }

  if (flag == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pok1.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
    Owner(this->state.unknown_11C84).render.position = this->render.position;

    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 10, 0)) * 0.1f + 2.0f;
      this->state.unknown_11C84->slots[i].height[8] = 0.0f;
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 13, 0)) + this->render.position.x - 6.0f;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 13, 0)) + this->render.position.z - 6.0f;
      this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].rotStart = 0;
      this->state.unknown_11C84->slots[i].height[1] = 0.0f;
      this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 40, 0)) * 0.01f + 0.3f;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = 0;
      this->state.unknown_11C84->slots[i].flag1[0] = 0;
      this->state.unknown_11C84->slots[i].flag1[1] = 0;
      this->state.unknown_11C84->slots[i].flag1[2] = 11;
      this->state.unknown_11C84->slots[i].height[7] = 0.0f;
    }

    return;
  }

  if (flag == 1) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 59, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->renderFlag = 5;
    this->state.unknown_11C84->duration = this->state.duration;
    this->state.unknown_11C84->totalTexture = 2;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\pok1.tga", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\pok3.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
    Owner(this->state.unknown_11C84).render.position = this->render.position;

    for (int i = 0; i < 4; ++i) {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].process = 0;
      this->state.unknown_11C84->slots[i].distance = static_cast<float>(std::max(rand() % 10, 0)) * 0.1f + 2.0f;
      this->state.unknown_11C84->slots[i].height[8] = 0.0f;
      this->state.unknown_11C84->slots[i].alphaB = 0;
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 13, 0)) + this->render.position.x - 6.0f;
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 13, 0)) + this->render.position.z - 6.0f;
      this->state.unknown_11C84->slots[i].vecBNow.y = this->render.position.y;
      this->state.unknown_11C84->slots[i].rotStart = 0;
      this->state.unknown_11C84->slots[i].height[1] = 0.0f;
      this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 40, 0)) * 0.01f + 0.4f;
      this->state.unknown_11C84->slots[i].fullDisplayAngle = 0;
      this->state.unknown_11C84->slots[i].flag1[0] = 0;
      this->state.unknown_11C84->slots[i].flag1[1] = 1;
      this->state.unknown_11C84->slots[i].flag1[2] = 11;
      this->state.unknown_11C84->slots[i].height[7] = 0.0f;
    }
  }
}

// OFFSET: 0x00C5A760
// FUNCTION: fn_20219
void CRagEffect::FreezingCircle(char kind) {
  this->render.position.x = this->state.parent->render.position.x;
  this->render.position.y = this->state.parent->render.position.y;
  this->render.position.z = this->state.parent->render.position.z;

  if (this->state.stateCnt == 99999998) {
    this->state.stateCnt = 0;
    this->state.duration = 99999999;
  } else if (this->state.stateCnt != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 29, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 0;
  this->state.unknown_11C84->renderFlag |= 1;
  this->state.unknown_11C84->renderFlag |= 5;
  this->state.unknown_11C84->duration = this->state.duration;
  AllocateTextures(*this->state.unknown_11C84);

  if (kind == 3) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\ghost.bmp", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
  } else if (kind < 1) {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\freezing_circle.bmp", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;
  } else {
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\whitelight.tga", 0, false, true, false);
    this->state.unknown_11C84->size = 0.0f;

    if (kind == 2) {
      this->state.unknown_11C84->size = 7.0f;
    }
  }

  for (int i = 0; i < 4; ++i) {
    if (kind == 3) {
      if (i < 1) {
        this->state.unknown_11C84->slots[i].life = 1;
      } else {
        this->state.unknown_11C84->slots[i].life = 0;
      }

      this->state.unknown_11C84->slots[i].maxHeight = 0.15f;
      this->state.unknown_11C84->slots[i].distance = 3.2f;
      this->state.unknown_11C84->slots[i].maxHeight = 0.6f;
      this->state.unknown_11C84->slots[i].vecBPre.x = static_cast<float>(std::max(rand() % 15, 0) - 8);
      this->state.unknown_11C84->slots[i].vecBPre.z = static_cast<float>(std::max(rand() % 15, 0) - 8);
    } else if (kind == 7 || kind == 8) {
      if (i < 2) {
        this->state.unknown_11C84->slots[i].life = 1;
      } else {
        this->state.unknown_11C84->slots[i].life = 0;
      }

      this->state.unknown_11C84->slots[i].maxHeight = 0.15f;

      if (kind == 7) {
        this->state.unknown_11C84->slots[i].distance = 0.8f;
      } else {
        this->state.unknown_11C84->slots[i].distance = 2.4f;
      }

      this->state.unknown_11C84->slots[i].vecBPre.x = 0.0f;
      this->state.unknown_11C84->slots[i].vecBPre.z = 0.0f;
    } else {
      this->state.unknown_11C84->slots[i].life = 1;
      this->state.unknown_11C84->slots[i].maxHeight = 0.15f;

      if (kind == 6) {
        this->state.unknown_11C84->slots[i].distance = 2.4f;
        this->state.unknown_11C84->slots[i].vecBPre.x = static_cast<float>(std::max(rand() % 35, 0) - 17);
        this->state.unknown_11C84->slots[i].vecBPre.z = 0.0f;
      } else if (kind == 1 || kind == 4 || kind == 5 || kind >= 8) {
        this->state.unknown_11C84->slots[i].distance = 2.4f;
        this->state.unknown_11C84->slots[i].vecBPre.x = 0.0f;
        this->state.unknown_11C84->slots[i].vecBPre.z = 0.0f;
      } else {
        this->state.unknown_11C84->slots[i].distance = 0.8f;
        this->state.unknown_11C84->slots[i].vecBPre.x = 0.0f;
        this->state.unknown_11C84->slots[i].vecBPre.z = 0.0f;
      }
    }

    this->state.unknown_11C84->slots[i].vecBPre.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[0] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[1] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[2] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[3] = static_cast<float>(std::max(rand() % 360, 0));

    if (kind == 3) {
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 15, 0) - 8);
      this->state.unknown_11C84->slots[i].vecBNow.z = static_cast<float>(std::max(rand() % 15, 0) - 8);
    } else if (kind == 6) {
      this->state.unknown_11C84->slots[i].vecBNow.x = static_cast<float>(std::max(rand() % 35, 0) - 17);
      this->state.unknown_11C84->slots[i].vecBNow.z = 0.0f;
    } else {
      this->state.unknown_11C84->slots[i].vecBNow.x = 0.0f;
      this->state.unknown_11C84->slots[i].vecBNow.z = 0.0f;
    }

    this->state.unknown_11C84->slots[i].vecBNow.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[4] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[5] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[6] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[7] = static_cast<float>(std::max(rand() % 360, 0));

    if (kind == 3) {
      this->state.unknown_11C84->slots[i].vecTPre.x = static_cast<float>(std::max(rand() % 15, 0) - 8);
      this->state.unknown_11C84->slots[i].vecTPre.z = static_cast<float>(std::max(rand() % 15, 0) - 8);
    } else if (kind == 6) {
      this->state.unknown_11C84->slots[i].vecTPre.x = static_cast<float>(std::max(rand() % 35, 0) - 17);
      this->state.unknown_11C84->slots[i].vecTPre.z = 0.0f;
    } else {
      this->state.unknown_11C84->slots[i].vecTPre.x = 0.0f;
      this->state.unknown_11C84->slots[i].vecTPre.z = 0.0f;
    }

    this->state.unknown_11C84->slots[i].vecTPre.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[8] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[9] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[10] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[11] = static_cast<float>(std::max(rand() % 360, 0));

    if (kind == 3) {
      this->state.unknown_11C84->slots[i].vecTNow.x = static_cast<float>(std::max(rand() % 15, 0) - 8);
      this->state.unknown_11C84->slots[i].vecTNow.z = static_cast<float>(std::max(rand() % 15, 0) - 8);
    } else if (kind == 6) {
      this->state.unknown_11C84->slots[i].vecTNow.x = static_cast<float>(std::max(rand() % 35, 0) - 17);
      this->state.unknown_11C84->slots[i].vecTNow.z = 0.0f;
    } else {
      this->state.unknown_11C84->slots[i].vecTNow.x = 0.0f;
      this->state.unknown_11C84->slots[i].vecTNow.z = 0.0f;
    }

    this->state.unknown_11C84->slots[i].vecTNow.y = static_cast<float>(std::max(rand() % 100, 0));
    this->state.unknown_11C84->slots[i].height[12] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[13] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[14] = static_cast<float>(std::max(rand() % 360, 0));
    this->state.unknown_11C84->slots[i].height[15] = static_cast<float>(std::max(rand() % 360, 0));

    if (kind == 4) {
      this->state.unknown_11C84->slots[i].vecBPre.y = -40.0f;
      this->state.unknown_11C84->slots[i].vecBNow.y = -50.0f;
      this->state.unknown_11C84->slots[i].vecTPre.y = -60.0f;
      this->state.unknown_11C84->slots[i].vecTNow.y = -70.0f;
      this->state.unknown_11C84->slots[i].flag1[0] = 14;
    } else if (kind == 6) {
      this->state.unknown_11C84->slots[i].vecBPre.y = static_cast<float>(-40 - std::max(rand() % 40, 0));
      this->state.unknown_11C84->slots[i].vecBNow.y = static_cast<float>(-40 - std::max(rand() % 40, 0));
      this->state.unknown_11C84->slots[i].vecTPre.y = static_cast<float>(-40 - std::max(rand() % 40, 0));
      this->state.unknown_11C84->slots[i].vecTNow.y = static_cast<float>(-40 - std::max(rand() % 40, 0));
      this->state.unknown_11C84->slots[i].flag1[0] = 16;
    } else {
      if (kind == 5 || kind == 7) {
        this->state.unknown_11C84->slots[i].vecBPre.y = 0.0f;
        this->state.unknown_11C84->slots[i].vecBNow.y = 0.0f;
        this->state.unknown_11C84->slots[i].vecTPre.y = 0.0f;
        this->state.unknown_11C84->slots[i].vecTNow.y = 0.0f;
      }

      if (kind == 1) {
        this->state.unknown_11C84->slots[i].flag1[0] = kind;
      } else if (kind == 3) {
        this->state.unknown_11C84->slots[i].flag1[0] = 11;
      } else if (kind == 5 || kind == 7 || kind == 8) {
        this->state.unknown_11C84->slots[i].flag1[0] = 15;
        this->state.unknown_11C84->slots[i].flag1[1] = 0;
        this->state.unknown_11C84->slots[i].flag1[2] = 0;
        this->state.unknown_11C84->slots[i].flag1[3] = 0;
        this->state.unknown_11C84->slots[i].flag1[4] = 0;
        this->state.unknown_11C84->slots[i].flag1[5] = kind;
      } else {
        this->state.unknown_11C84->slots[i].flag1[0] = 0;
      }
    }
  }
}

// OFFSET: 0x00C4BB00
// FUNCTION: fn_20175
void CRagEffect::TorchDust(char kind) {
  this->render.position = this->state.parent->render.position;

  if (this->state.stateCnt != 0) {
    return;
  }

  this->render.position = this->state.parent->render.position;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 128, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->renderFlag = 5;
  this->state.unknown_11C84->duration = this->state.duration;

  if (kind == 0) {
    this->state.unknown_11C84->totalTexture = 9;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\dust01.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\dust02.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\dust03.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\dust04.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\dust05.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\dust06.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[6] = g_texMgr.Load("effect\\dust07.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[7] = g_texMgr.Load("effect\\dust08.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[8] = g_texMgr.Load("effect\\dust09.bmp", 0, false, true, false);
  } else if (kind == 2) {
    this->state.unknown_11C84->totalTexture = 13;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\torch_green01.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\torch_green02.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\torch_green03.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\torch_green04.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\torch_green05.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\torch_green06.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[6] = g_texMgr.Load("effect\\torch_green07.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[7] = g_texMgr.Load("effect\\torch_green08.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[8] = g_texMgr.Load("effect\\torch_green09.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[9] = g_texMgr.Load("effect\\torch_green10.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[10] = g_texMgr.Load("effect\\torch_green11.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[11] = g_texMgr.Load("effect\\torch_green12.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[12] = g_texMgr.Load("effect\\torch_green13.bmp", 0, false, true, false);
  } else if (kind == 1) {
    this->state.unknown_11C84->totalTexture = 13;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\torch_red01.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\torch_red02.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\torch_red03.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\torch_red04.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\torch_red05.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\torch_red06.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[6] = g_texMgr.Load("effect\\torch_red07.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[7] = g_texMgr.Load("effect\\torch_red08.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[8] = g_texMgr.Load("effect\\torch_red09.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[9] = g_texMgr.Load("effect\\torch_red10.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[10] = g_texMgr.Load("effect\\torch_red11.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[11] = g_texMgr.Load("effect\\torch_red12.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[12] = g_texMgr.Load("effect\\torch_red13.bmp", 0, false, true, false);
  } else if (kind == 3) {
    this->state.unknown_11C84->totalTexture = 13;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\torch_violet01.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\torch_violet02.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\torch_violet03.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\torch_violet04.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\torch_violet05.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\torch_violet06.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[6] = g_texMgr.Load("effect\\torch_violet07.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[7] = g_texMgr.Load("effect\\torch_violet08.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[8] = g_texMgr.Load("effect\\torch_violet09.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[9] = g_texMgr.Load("effect\\torch_violet10.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[10] = g_texMgr.Load("effect\\torch_violet11.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[11] = g_texMgr.Load("effect\\torch_violet12.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[12] = g_texMgr.Load("effect\\torch_violet13.bmp", 0, false, true, false);
  } else if (kind == 4) {
    this->state.unknown_11C84->totalTexture = 11;
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = g_texMgr.Load("effect\\lord_stick_a.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = g_texMgr.Load("effect\\lord_stick_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = g_texMgr.Load("effect\\lord_stick_c.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = g_texMgr.Load("effect\\lord_stick_e.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = g_texMgr.Load("effect\\lord_stick_f.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[5] = g_texMgr.Load("effect\\lord_stick_g.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[6] = g_texMgr.Load("effect\\lord_stick_h.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[7] = g_texMgr.Load("effect\\lord_stick_plazma_a.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[8] = g_texMgr.Load("effect\\lord_stick_plazma_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[9] = g_texMgr.Load("effect\\lord_stick_plazma_c.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[10] = g_texMgr.Load("effect\\lord_blast_stick_a.bmp", 0, false, true, false);
  }

  this->state.unknown_11C84->size = 0.0f;
  this->state.unknown_11C84->slots[0].life = 0;
  this->state.unknown_11C84->slots[1].life = 0;
  this->state.unknown_11C84->slots[2].life = 0;
  this->state.unknown_11C84->slots[3].life = 0;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].distance = this->state.param[0] > 0.0f ? this->state.param[0] : 20.0f;
  this->state.unknown_11C84->slots[0].vecBPre.y = this->render.position.y - 10.0f;
  this->state.unknown_11C84->slots[0].flag1[2] = 4;

  if (kind == 4) {
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 120;
    this->state.unknown_11C84->slots[0].height[10] = 1.0f;
    this->state.unknown_11C84->slots[0].height[8] = 0.0f;
    this->state.unknown_11C84->slots[0].alphaB = 130;
  } else {
    this->state.unknown_11C84->slots[0].fullDisplayAngle = 135;
    this->state.unknown_11C84->slots[0].height[10] = 0.0f;
    this->state.unknown_11C84->slots[0].height[8] = 0.0f;
    this->state.unknown_11C84->slots[0].alphaB = kind == 0 ? 100 : 130;
  }

  this->state.unknown_11C84->slots[0].vecBNow.x = this->render.position.x;
  this->state.unknown_11C84->slots[0].vecBNow.z = this->render.position.z;
  this->state.unknown_11C84->slots[0].rotStart = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->slots[0].flag1[0] = 0;
  this->state.unknown_11C84->slots[0].flag1[4] = kind;
}

// OFFSET: 0x00C41420
// FUNCTION: fn_20135
void CRagEffect::WhiteYellowRings() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    this->TexturedQuadRingSized(45, "effect\\ring_white.tga", 22);
    this->TexturedQuadRingSized(25, "effect\\ring_yellow.tga", 22);
  }
}

// OFFSET: 0x00C44B70
// FUNCTION: fn_20145
void CRagEffect::BlueSpellRings() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
    g_soundMgr->PlayWave(
      "effect\\EF_BeginSpell.wav", this->render.position.x - player->render.position.x, -100.0f, this->render.position.z - player->render.position.z, 250, 40,
      1.0f, 0
    );
    this->TexturedQuadRingSized(45, "effect\\ring_blue.tga", 5);
    this->TexturedQuadRingSized(25, "effect\\ring_blue.tga", 5);
  }
}

// OFFSET: 0x00C73B40
// FUNCTION: fn_20329
void CRagEffect::WhiteSpellRings() {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
    g_soundMgr->PlayWave(
      "effect\\EF_BeginSpell.wav", this->render.position.x - player->render.position.x, -100.0f, this->render.position.z - player->render.position.z, 250, 40,
      1.0f, 0
    );
    this->TexturedQuadRingSized(45, "effect\\ring_white.tga", 15);
    this->TexturedQuadRingSized(25, "effect\\ring_white.tga", 15);
  }
}

// OFFSET: 0x00C4A5A0
// FUNCTION: fn_20169
void CRagEffect::TexturedSpellRings(const char* name) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    PlayEffectWave(*this, "effect\\EF_BeginSpell.wav");
    this->TexturedQuadRingSized(45, name, 3);
    this->TexturedQuadRingSized(25, name, 3);
  }
}

// OFFSET: 0x00C4ACC0
// FUNCTION: fn_20171
void CRagEffect::BlackSpellRings(char silent) {
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (this->state.stateCnt == 0) {
    if (silent == 0) {
      const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      g_soundMgr->PlayWave(
        "effect\\EF_BeginSpell.wav", this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z, 250, 40,
        1.0f, 0
      );
    }

    this->TexturedQuadRingSized(45, "effect\\ring_black.tga", 6);
    this->TexturedQuadRingSized(25, "effect\\ring_black.tga", 6);
  }
}
