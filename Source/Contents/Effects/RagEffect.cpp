#include "Contents/Effects/RagEffect.h"
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
#include "Render/Projection/RenderProjection.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Resource/EZeffectResource.h"
#include "Resource/World/TerrainResource.h"

#include <algorithm>
#include <bit>
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
  g_soundMgr->PlayWave(name, effect.render.position.x - player->render.position.x, 0.0f, effect.render.position.z - player->render.position.z, 250, 40, 1.0f, 0);
}

// INLINED
static std::list<CRenderObject*>& WorldObjects() {
  return static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects;
}

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
  Get_Angle(this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[0].riseAngle);
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
      (this->render.position.x + this->render.position.x) - this->state.deltaPos[0], (this->render.position.z + this->render.position.z) - this->state.deltaPos[2]
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
      *this, {m[3] * 0.0f + m[0] * 0.0f + distance * m[6] + m[9], m[4] * 0.0f + m[1] * 0.0f + distance * m[7] + m[10], m[5] * 0.0f + m[2] * 0.0f + distance * m[8] + m[11]},
      angle + 90
    );
    SpawnBottomQuad(
      *this, {distance * m[0] + m[3] * 0.0f + m[6] * 0.0f + m[9], distance * m[1] + m[4] * 0.0f + m[7] * 0.0f + m[10], distance * m[2] + m[5] * 0.0f + m[8] * 0.0f + m[11]},
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

// OFFSET: 0x00C3A920

// OFFSET: 0x00C12B90
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
void CRagEffect::TexturedSpiral(const char* name) {
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
  Get_Angle(this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[0].riseAngle);
  this->state.unknown_11C84->slots[0].alphaB = 0;
  this->state.unknown_11C84->slots[0].flag1[0] = flag;
}

// OFFSET: 0x00C70980
// FUNCTION: fn_20314
void CRagEffect::TexturedSpiralSmall(const char* name) {
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
void CRagEffect::TexturedSpiralTall(const char* name) {
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
void CRagEffect::TexturedSpiralFade(const char* name) {
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
void CRagEffect::TexturedDualSpin(const char* first, const char* second) {
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
    Get_Angle(this->state.deltaPos[0] - this->render.position.x, this->state.deltaPos[2] - this->render.position.z, &this->state.unknown_11C84->slots[index].riseAngle);

    if (this->state.unknown_11C84->slots[index].riseAngle > 359) {
      this->state.unknown_11C84->slots[index].riseAngle -= 360;
    }

    this->state.unknown_11C84->slots[index].alphaB = 0;
  }
}

// OFFSET: 0x00C65F80
// FUNCTION: fn_20272
void CRagEffect::FeatherBurst(const char* name, std::int16_t angle) {
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
void CRagEffect::TexturedSlam(const char* name, std::int16_t kind) {
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
void CRagEffect::TexturedFlare(const char* name, char size) {
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
    this->state.unknown_11C84->slots[index].vecBNow.z = 0.0f;

    for (int height = 0; height < 21; ++height) {
      this->state.unknown_11C84->slots[index].height[height] = 0.0f;
      this->state.unknown_11C84->slots[index].flag1[height] = 0;
    }
  }
}

// OFFSET: 0x00C69350

// OFFSET: 0x00C51AA0

// OFFSET: 0x00C5D540

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
  g_soundMgr->PlayWave("_heal_effect.wav", this->render.position.x - player->render.position.x, 150.0f, this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0);

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

// OFFSET: 0x00C5C450
