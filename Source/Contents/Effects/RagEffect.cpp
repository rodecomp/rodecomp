#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Resource/Resource.h"
#include "Client/Mode/CGameMode.h"
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

// OFFSET: 0x00C36530
// FUNCTION: fn_20092
void CRagEffect::PotionPillar(TextureCache& textures, float heightSpeed, int duration) {
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
  this->state.unknown_11C84->textures[0] = textures.Load("effect\\alpha_down.tga", 0, false, true, false);
}

// OFFSET: 0x00C475C0

// OFFSET: 0x00C59490

// OFFSET: 0x00C629D0

// OFFSET: 0x00C63860

// OFFSET: 0x00C592E0

// OFFSET: 0x00C5CA50

// OFFSET: 0x00C5D390

// OFFSET: 0x00C61A80

// OFFSET: 0x00C423D0
