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

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <limits>

// INLINED
inline RagEffectState::RagEffectState()
    : parent(nullptr), unknown_0160(0), unknown_0164(0), id(0), unknown_016C(1), unknown_0170(0), unknown_0174(0), unknown_0178(0), offset{}, unknown_018C{},
      resource(nullptr), resourceData(nullptr), currentLayer(0), loopCount(0), loopLimit(0), unknown_11C84(nullptr), unknown_11C88{-1, -1, -1, -1},
      forceRender(0), tag(0), unknown_11CA0(0) {
  for (unsigned index = 0; index < 20; ++index) {
    auto* record = this->unknown_01AC + index * 80;
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
  for (EffectPrimitiveState* particle : this->related) {
    particle->master = nullptr;
  }

  this->related.clear();

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
  const auto bytes = particle.textureCount >> 30 != 0 ? std::numeric_limits<std::size_t>::max() : static_cast<std::size_t>(4u * particle.textureCount);
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
  if (this->state.unknown_0178 != 0) {
    return;
  }

  for (int angle = -90; angle < 180; angle += 90) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->lifetime = 60;
    this->state.unknown_11C84->velocity = {0.0f, -10.0f, 0.0f};
    this->state.unknown_11C84->unknown_0348 = Owner(this->state.unknown_11C84).CalcDist(this->state.offset[0], this->state.offset[2]);
    Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
    this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
    const float speed = this->state.unknown_11C84->unknown_0348 / this->state.unknown_11C84->lifetime;
    this->state.unknown_11C84->unknown_02BC = speed + speed;
    this->state.unknown_11C84->unknown_02EC = 2.0f;
    const float rise = -(this->state.unknown_11C84->unknown_02EC / this->state.unknown_11C84->lifetime);
    this->state.unknown_11C84->unknown_02F0 = rise + rise;
    this->state.unknown_11C84->unknown_0334 = -2.0f;
    const float spread = -(this->state.unknown_11C84->unknown_0334 / this->state.unknown_11C84->lifetime);
    this->state.unknown_11C84->unknown_0340 = spread + spread;
    this->state.unknown_11C84->unknown_0314 = static_cast<float>(angle);
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.matrix1)->MakeYRotation(GetRadian(static_cast<int>(particle.unknown_030C)));
    this->state.unknown_11C84->unknown_03A0 = RotatedForward(this->state.unknown_11C84->matrix1, 1.0f);
    this->state.unknown_11C84->unknown_0290[0] = this->state.unknown_11C84->unknown_03A0.x;
    this->state.unknown_11C84->unknown_0290[1] = this->state.unknown_11C84->unknown_03A0.y;
    this->state.unknown_11C84->unknown_0290[2] = this->state.unknown_11C84->unknown_03A0.z;
    this->state.unknown_11C84->red = 100;
    this->state.unknown_11C84->green = 100;
    this->state.unknown_11C84->blue = 250;
    this->state.unknown_11C84->scale = 1.7f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->unknown_01F4 = 7;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C14510
// FUNCTION: fn_20046
void CRagEffect::FireBall() {
  const int frame = this->state.unknown_0178;

  if (frame != 20 && frame != 24 && frame != 28 && frame != 32) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_FireBall.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 20;
  this->state.unknown_11C84->flags |= 0x120u;
  this->state.unknown_11C84->velocity = {0.0f, -15.0f, 0.0f};
  Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
  this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(this->state.unknown_11C84);
  particle.state.unknown_02BC = -(particle.CalcDist(this->state.offset[0], this->state.offset[2]) / particle.state.lifetime);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->matrix1)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->unknown_030C)));
  this->state.unknown_11C84->scale = 1.3f;
  this->state.unknown_11C84->unknown_01E8 = 1;

  if (this->state.unknown_0178 != 20) {
    this->state.unknown_11C84->red = 246;
    this->state.unknown_11C84->green = 199;
    this->state.unknown_11C84->blue = 76;

    if (this->state.unknown_0178 == 24) {
      this->state.unknown_11C84->alpha = 180.0f;
    } else if (this->state.unknown_0178 == 28) {
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
void CRagEffect::FrostDiver(TextureCache& textures, char type) {
  if (g_serviceOptions.Get(229) != 0) {
    return;
  }

  if (this->state.unknown_0178 == 0) {
    this->Face(this->state.offset[0], this->state.offset[2]);
    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(-this->render.angle)));
    const vector3d reach = RotatedForward(rotation.values, 5.0f);
    this->state.unknown_018C[3] = this->state.unknown_018C[3] - reach.x;
    this->state.unknown_018C[4] = this->state.unknown_018C[4] - reach.y;
    this->state.unknown_018C[5] = this->state.unknown_018C[5] - reach.z;
    const vector3d step = RotatedForward(rotation.values, 2.0f);
    this->state.unknown_018C[0] = step.x;
    this->state.unknown_018C[1] = step.y;
    this->state.unknown_018C[2] = step.z;
  }

  if (this->state.unknown_0170 != 0) {
    return;
  }

  const vector3d saved = this->render.position;
  this->render.position.x = this->state.unknown_018C[3] + this->render.position.x;
  this->render.position.y = this->state.unknown_018C[4] + this->render.position.y;
  this->render.position.z = this->state.unknown_018C[5] + this->render.position.z;

  if (this->CalcDist(this->state.offset[0], this->state.offset[2]) <= 2.5f) {
    this->state.unknown_0170 = 1;
  }

  this->render.position = saved;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 40;
  this->state.unknown_11C84->flags |= 1u;
  this->state.unknown_11C84->slots[0].flag1[0] = type == 3 ? 3 : 0;
  const int angle = std::max(rand() % 360, 0);
  const float distance = static_cast<float>(std::max(rand() % 10, 0) + 5) / 10.0f;
  const float sine = ClientSine(angle);
  const float cosine = ClientCosine(angle);
  this->state.unknown_11C84->velocity.y = 30.0f;
  this->state.unknown_11C84->velocity.x = sine * distance;
  this->state.unknown_11C84->velocity.z = -distance * cosine;
  vector3d& velocity = this->state.unknown_11C84->velocity;
  velocity.x = this->state.unknown_018C[3] + velocity.x;
  velocity.y = this->state.unknown_018C[4] + velocity.y;
  velocity.z = this->state.unknown_018C[5] + velocity.z;
  this->state.unknown_11C84->scale = static_cast<float>(std::max(rand() % 40, 0) + 60) / 100.0f;
  this->state.unknown_11C84->unknown_0360 = static_cast<float>(std::max(rand() % 30, 0) + 150) / 10.0f;
  this->state.unknown_11C84->unknown_030C = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->unknown_0310 = static_cast<float>(std::max(rand() % 30, 0) + 75);
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.matrix1)->MakeXRotation(GetRadian(static_cast<int>(tilt.unknown_0310)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.matrix1)->AppendYRotation(GetRadian(static_cast<int>(turn.unknown_030C)));
  EffectPrimitiveState& copy = *this->state.unknown_11C84;
  std::memcpy(copy.matrix2, copy.matrix1, sizeof(copy.matrix2));
  this->state.unknown_11C84->unknown_02BC = 3.0f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  const float decay = -(slow.unknown_02BC / slow.lifetime);
  slow.unknown_02C0 = decay + decay;
  this->state.unknown_11C84->unknown_01D4 = 20;
  this->state.unknown_11C84->unknown_01F0 = this->state.unknown_11C84->lifetime - 10;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = textures.Load("effect\\ice.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[0] = texture;
  this->state.unknown_018C[3] = this->state.unknown_018C[3] - this->state.unknown_018C[0];
  this->state.unknown_018C[4] = this->state.unknown_018C[4] - this->state.unknown_018C[1];
  this->state.unknown_018C[5] = this->state.unknown_018C[5] - this->state.unknown_018C[2];
}

// OFFSET: 0x00C16C70
// FUNCTION: fn_20058
void CRagEffect::GrimTooth(TextureCache& textures) {
  if (this->state.unknown_0178 == 0) {
    PlayEffectWave(*this, "effect\\EF_FrostDiver.wav");
    this->Face(this->state.offset[0], this->state.offset[2]);
    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(-this->render.angle)));
    const vector3d step = RotatedForward(rotation.values, 2.0f);
    this->state.unknown_018C[0] = step.x;
    this->state.unknown_018C[1] = step.y;
    this->state.unknown_018C[2] = step.z;
    this->state.unknown_018C[3] = this->state.unknown_018C[3] - step.x;
    this->state.unknown_018C[4] = this->state.unknown_018C[4] - step.y;
    this->state.unknown_018C[5] = this->state.unknown_018C[5] - step.z;
  }

  if (this->state.unknown_0170 != 0) {
    return;
  }

  if (this->state.unknown_0178 % 3 != 0) {
    this->state.unknown_018C[3] = this->state.unknown_018C[3] - this->state.unknown_018C[0];
    this->state.unknown_018C[4] = this->state.unknown_018C[4] - this->state.unknown_018C[1];
    this->state.unknown_018C[5] = this->state.unknown_018C[5] - this->state.unknown_018C[2];
    return;
  }

  const vector3d saved = this->render.position;

  if (this->CalcDist(this->state.offset[0], this->state.offset[2]) <= 2.5f) {
    this->state.unknown_0170 = 1;
  }

  this->render.position = saved;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 9, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->flags |= 1u;
  this->state.unknown_11C84->lifetime = 40;
  const int angle = std::max(rand() % 360, 0);
  rand();
  ClientSine(angle);
  ClientCosine(angle);
  this->state.unknown_11C84->velocity = {0.0f, 20.0f, 0.0f};
  vector3d& velocity = this->state.unknown_11C84->velocity;
  velocity.x = this->state.unknown_018C[3] + velocity.x;
  velocity.y = this->state.unknown_018C[4] + velocity.y;
  velocity.z = this->state.unknown_018C[5] + velocity.z;
  this->state.unknown_11C84->scale = static_cast<float>(std::max(rand() % 40, 0) + 60) / 100.0f;
  this->state.unknown_11C84->unknown_0360 = 10.0f;
  this->state.unknown_11C84->unknown_030C = static_cast<float>(std::max(rand() % 360, 0));
  this->state.unknown_11C84->unknown_0310 = static_cast<float>(std::max(rand() % 30, 0) + 75);
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.matrix1)->MakeXRotation(GetRadian(static_cast<int>(tilt.unknown_0310)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.matrix1)->AppendYRotation(GetRadian(static_cast<int>(turn.unknown_030C)));
  EffectPrimitiveState& copy = *this->state.unknown_11C84;
  std::memcpy(copy.matrix2, copy.matrix1, sizeof(copy.matrix2));
  this->state.unknown_11C84->unknown_02BC = 3.0f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  const float decay = -(slow.unknown_02BC / slow.lifetime);
  slow.unknown_02C0 = decay + decay;
  this->state.unknown_11C84->unknown_01D4 = 6;
  this->state.unknown_11C84->unknown_01F0 = this->state.unknown_11C84->lifetime - 10;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = textures.Load("effect\\stone.bmp", 0, false, true, false);
  this->state.unknown_018C[3] = this->state.unknown_018C[3] - this->state.unknown_018C[0];
  this->state.unknown_018C[4] = this->state.unknown_018C[4] - this->state.unknown_018C[1];
  this->state.unknown_11C84->textures[0] = texture;
  this->state.unknown_018C[5] = this->state.unknown_018C[5] - this->state.unknown_018C[2];
}

// OFFSET: 0x00C19980
// FUNCTION: fn_20066
void CRagEffect::Hit4(const RenderProjection& projection, TextureCache& textures) {
  RenderVertex vertex{0.0f, 0.0f, 0.0f, 0.0f, 0u, 0xFF000000u, 0.0f, 0.0f};
  projection.Project(&this->state.parent->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, vertex);
  this->state.unknown_013C[6] = std::bit_cast<std::uint32_t>(vertex.x);
  this->state.unknown_013C[7] = std::bit_cast<std::uint32_t>(vertex.y);

  if (this->state.unknown_0178 != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_hit4.wav");
  const int facing = static_cast<int>(180.0f - this->state.parent->render.angle);
  const int direction = facing < 0 ? facing + 360 : facing;
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 13, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 15;
  this->state.unknown_11C84->unknown_030C = static_cast<float>(direction);
  this->state.unknown_11C84->unknown_0310 = -90.0f;
  EffectPrimitiveState& tilt = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(tilt.matrix1)->MakeXRotation(GetRadian(static_cast<int>(tilt.unknown_0310)));
  EffectPrimitiveState& turn = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(turn.matrix1)->AppendYRotation(GetRadian(static_cast<int>(turn.unknown_030C)));
  this->state.unknown_11C84->velocity = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->unknown_02BC = 0.7f;
  EffectPrimitiveState& slow = *this->state.unknown_11C84;
  slow.unknown_02C0 = -(slow.unknown_02BC / slow.lifetime) * 0.5f;
  this->state.unknown_11C84->unknown_02F8 = 4.0f;
  this->state.unknown_11C84->unknown_02F4 = 0.5f;
  this->state.unknown_11C84->unknown_0364 = 0.25f;
  this->state.unknown_11C84->unknown_0368 = 0.15f;
  const int lifetime = this->state.unknown_11C84->lifetime;
  this->state.unknown_11C84->unknown_01F0 = lifetime - lifetime / 3;
  AllocateTextures(*this->state.unknown_11C84);
  CTexture* texture = textures.Load("effect\\lens2.tga", 0, false, true, false);
  this->state.unknown_11C84->textures[0] = texture;

  for (int count = 5; count != 0; --count) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->unknown_030C = static_cast<float>(direction - 40 + std::max(rand() % 80, 0));
    this->state.unknown_11C84->unknown_0310 = static_cast<float>(std::max(rand() % 100, 0) - 50);
    EffectPrimitiveState& yaw = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(yaw.matrix1)->MakeYRotation(GetRadian(static_cast<int>(yaw.unknown_030C)));
    EffectPrimitiveState& pitch = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(pitch.matrix1)->AppendXRotation(GetRadian(static_cast<int>(pitch.unknown_0310)));
    EffectPrimitiveState& spark = *this->state.unknown_11C84;
    spark.unknown_03A0 = RotatedForward(spark.matrix1, static_cast<float>(std::max(rand() % 4, 0) + 4));
    EffectPrimitiveState& moving = *this->state.unknown_11C84;
    moving.velocity.x = moving.unknown_03A0.x + 0.0f;
    moving.velocity.y = moving.unknown_03A0.y - 10.0f;
    moving.velocity.z = moving.unknown_03A0.z + 0.0f;
    this->state.unknown_11C84->unknown_02BC = static_cast<float>(std::max(rand() % 90, 0) + 60) / 100.0f;
    this->state.unknown_11C84->lifetime = std::max(rand() % 24, 0) + 6;
    EffectPrimitiveState& fading = *this->state.unknown_11C84;
    fading.unknown_02C0 = -(fading.unknown_02BC / fading.lifetime) * 0.5f;
    this->state.unknown_11C84->scale = static_cast<float>(std::max(rand() % 100, 0) + 60) / 100.0f;
    EffectPrimitiveState& shrinking = *this->state.unknown_11C84;
    shrinking.unknown_031C = -(shrinking.scale / shrinking.lifetime);
    this->state.unknown_11C84->unknown_01F0 = 0;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->unknown_01F4 = 3;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C3B2E0
// FUNCTION: fn_20106
void CRagEffect::SoulStrike(char F1) {
  int base = 0;
  int step = 0;
  int total = 0;

  switch (this->state.unknown_0174) {
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

  const int frame = this->state.unknown_0178 + 5;

  if (frame % 11 != 0 || frame / 11 > total) {
    return;
  }

  const int index = frame / 11;
  PlayEffectWave(*this, "effect\\EF_SoulStrike.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 40;
  this->state.unknown_11C84->velocity = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->unknown_0348 = Owner(this->state.unknown_11C84).CalcDist(this->state.offset[0], this->state.offset[2]);
  Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
  this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
  this->state.unknown_11C84->unknown_02BC = this->state.unknown_11C84->unknown_0348 / this->state.unknown_11C84->lifetime;
  this->state.unknown_11C84->unknown_02EC = 3.5f;
  const float rise = -(this->state.unknown_11C84->unknown_02EC / this->state.unknown_11C84->lifetime);
  this->state.unknown_11C84->unknown_02F0 = rise + rise;
  this->state.unknown_11C84->unknown_0334 = -2.0f;
  const float spread = -(this->state.unknown_11C84->unknown_0334 / this->state.unknown_11C84->lifetime);
  this->state.unknown_11C84->unknown_0340 = spread + spread;
  this->state.unknown_11C84->unknown_0314 = static_cast<float>(base + step * index);
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.matrix1)->MakeYRotation(GetRadian(static_cast<int>(particle.unknown_030C)));
  this->state.unknown_11C84->unknown_03A0 = RotatedForward(this->state.unknown_11C84->matrix1, 1.0f);
  this->state.unknown_11C84->unknown_0290[0] = this->state.unknown_11C84->unknown_03A0.x;
  this->state.unknown_11C84->unknown_0290[1] = this->state.unknown_11C84->unknown_03A0.y;
  this->state.unknown_11C84->unknown_0290[2] = this->state.unknown_11C84->unknown_03A0.z;
  this->state.unknown_11C84->scale = 2.75f;
  const char* action = F1 != 0 ? "이팩트\\particle5.act" : "이팩트\\particle1.act";
  const char* sprite = F1 != 0 ? "이팩트\\particle5.spr" : "이팩트\\particle1.spr";
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), sprite);
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), action);
  this->state.unknown_11C84->unknown_01F4 = 12;
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C3B730
// FUNCTION: fn_20107
void CRagEffect::SpearBmr() {
  const int frame = this->state.unknown_0178;

  if (frame != 0 && frame != 4 && frame != 8 && frame != 12) {
    return;
  }

  PlayEffectWave(*this, "effect\\EF_FireBall.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 5, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 15;
  this->state.unknown_11C84->flags |= 0x120u;
  this->state.unknown_11C84->velocity = {0.0f, -15.0f, 0.0f};
  Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
  this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& particle = Owner(this->state.unknown_11C84);
  particle.state.unknown_02BC = -(particle.CalcDist(this->state.offset[0], this->state.offset[2]) / particle.state.lifetime);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->matrix1)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->unknown_030C)));
  this->state.unknown_11C84->scale = 1.0f;
  this->state.unknown_11C84->unknown_01E8 = 1;

  if (this->state.unknown_0178 != 0) {
    if (this->state.unknown_0178 == 4) {
      this->state.unknown_11C84->alpha = 180.0f;
    } else if (this->state.unknown_0178 == 8) {
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
void CRagEffect::SprayPond(TextureCache& textures) {
  if (this->state.unknown_0178 == 0) {
    for (int angle = 0; angle < 360; angle += 45) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->flags = 2;
      this->state.unknown_11C84->lifetime = 130;
      EffectPrimitiveState& yaw = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(yaw.matrix1)->MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(angle) + 22.5f)));
      EffectPrimitiveState& moving = *this->state.unknown_11C84;
      const vector3d edge = RotatedForward(moving.matrix1, 12.2f);
      moving.velocity.x = edge.x + 0.0f;
      moving.velocity.y = edge.y - 2.0f;
      moving.velocity.z = edge.z + 0.0f;
      EffectPrimitiveState& facing = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(facing.matrix2)->MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(angle + 90) + 22.5f)));
      this->state.unknown_11C84->alpha = 120.0f;
      this->state.unknown_11C84->unknown_0354 = 5.0f;
      this->state.unknown_11C84->unknown_0360 = 2.7f;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = textures.Load("effect\\pond_water.bmp", 0, false, true, false);
    }
  }

  if (this->state.unknown_0178 % 10 == 0) {
    for (int angle = 0; angle < 360; angle += 45) {
      this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
      this->state.unknown_11C84->lifetime = 30;
      EffectPrimitiveState& yaw = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(yaw.matrix1)->MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(angle) + 22.5f)));
      EffectPrimitiveState& moving = *this->state.unknown_11C84;
      const vector3d edge = RotatedForward(moving.matrix1, 13.8f);
      moving.velocity.x = edge.x;
      moving.velocity.y = edge.y;
      moving.velocity.z = edge.z;
      EffectPrimitiveState& roll = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(roll.matrix2)->MakeZRotation(GetRadian(90));
      EffectPrimitiveState& facing = *this->state.unknown_11C84;
      reinterpret_cast<matrix*>(facing.matrix2)->AppendYRotation(GetRadian(static_cast<int>(static_cast<float>(angle + 90) + 22.5f)));
      this->state.unknown_11C84->alpha = 100.0f;
      this->state.unknown_11C84->unknown_01F0 = 0;
      this->state.unknown_11C84->unknown_0354 = 5.5f;
      this->state.unknown_11C84->unknown_0360 = 1.0f;
      AllocateTextures(*this->state.unknown_11C84);
      this->state.unknown_11C84->textures[0] = textures.Load("effect\\alpha_down.tga", 0, false, true, false);
    }
  }

  if (this->state.unknown_0178 % 20 == 0) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 12, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->lifetime = 25;
    this->state.unknown_11C84->unknown_0348 = 13.0f;
    this->state.unknown_11C84->unknown_0288 = 10.0f;
    this->state.unknown_11C84->scale = 1.25f;
    this->state.unknown_11C84->unknown_0310 = 90.0f;
    EffectPrimitiveState& tilt = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(tilt.matrix1)->MakeXRotation(GetRadian(static_cast<int>(tilt.unknown_0310)));
    this->state.unknown_11C84->unknown_034C = 0.25f;
    EffectPrimitiveState& growing = *this->state.unknown_11C84;
    growing.unknown_0350 = -(growing.unknown_034C / growing.lifetime);
    this->state.unknown_11C84->alpha = 100.0f;
    this->state.unknown_11C84->unknown_01F0 = 0;
    Owner(this->state.unknown_11C84).InitSegment();
    AllocateTextures(*this->state.unknown_11C84);
    this->state.unknown_11C84->textures[0] = textures.Load("effect\\alpha_center.tga", 0, false, true, false);
  }
}

// OFFSET: 0x00C3F440
// FUNCTION: fn_20120
void CRagEffect::WaterBall2() {
  if (this->state.unknown_0178 != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\wizard_waterball_chulung.wav");
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\Waterball.spr");
  Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\Waterball.act");
  this->state.unknown_11C84->lifetime = 40;
  this->state.unknown_11C84->velocity = {0.0f, -10.0f, 0.0f};
  this->state.unknown_11C84->unknown_0348 = Owner(this->state.unknown_11C84).CalcDist(this->state.offset[0], this->state.offset[2]);
  Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
  this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
  this->state.unknown_11C84->unknown_02BC = this->state.unknown_11C84->unknown_0348 / this->state.unknown_11C84->lifetime;
  this->state.unknown_11C84->unknown_02EC = 3.5f;
  const float rise = -(this->state.unknown_11C84->unknown_02EC / this->state.unknown_11C84->lifetime);
  this->state.unknown_11C84->unknown_02F0 = rise + rise;
  this->state.unknown_11C84->unknown_0334 = -2.0f;
  const float spread = -(this->state.unknown_11C84->unknown_0334 / this->state.unknown_11C84->lifetime);
  this->state.unknown_11C84->unknown_0340 = spread + spread;
  this->state.unknown_11C84->unknown_0314 = 0.0f;
  EffectPrimitiveState& particle = *this->state.unknown_11C84;
  reinterpret_cast<matrix*>(particle.matrix1)->MakeYRotation(GetRadian(static_cast<int>(particle.unknown_030C)));
  this->state.unknown_11C84->unknown_03A0 = RotatedForward(this->state.unknown_11C84->matrix1, 1.0f);
  this->state.unknown_11C84->unknown_0290[0] = this->state.unknown_11C84->unknown_03A0.x;
  this->state.unknown_11C84->unknown_0290[1] = this->state.unknown_11C84->unknown_03A0.y;
  this->state.unknown_11C84->unknown_0290[2] = this->state.unknown_11C84->unknown_03A0.z;
  this->state.unknown_11C84->scale = 0.75f;
  this->state.unknown_11C84->unknown_01F4 = 12;
  Owner(this->state.unknown_11C84).InitSegment();
}

// OFFSET: 0x00C3FE30
// FUNCTION: fn_20124
void CRagEffect::Yufitel(TextureCache& textures, char F1) {
  if (this->state.unknown_0178 != 0) {
    return;
  }

  PlayEffectWave(*this, "effect\\hunter_shockwavetrap.wav");

  if (this->state.unknown_0178 != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 20;
  this->state.unknown_11C84->flags |= 0x40u;
  this->state.unknown_11C84->flags |= 0x400u;
  this->state.unknown_11C84->unknown_01C4 |= 2u;
  this->state.unknown_11C84->unknown_0354 = 3.5f;
  this->state.unknown_11C84->unknown_0360 = 3.5f;
  Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
  this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& center = Owner(this->state.unknown_11C84);
  center.state.unknown_02BC = -(center.CalcDist(this->state.offset[0], this->state.offset[2]) / center.state.lifetime);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->matrix1)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->unknown_030C)));
  this->state.unknown_11C84->velocity.y = -5.0f;
  this->state.unknown_11C84->alpha = 170.0f;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] =
    F1 != 0 ? textures.Load("effect\\twirl_soft.bmp", 0, false, true, false) : textures.Load("effect\\thunder_center.bmp", 0, false, true, false);
  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 15, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->lifetime = 20;
  this->state.unknown_11C84->flags = 0x40u;
  this->state.unknown_11C84->flags |= 0x400u;
  this->state.unknown_11C84->unknown_01C4 |= 2u;
  this->state.unknown_11C84->unknown_0254 = 1;
  this->state.unknown_11C84->unknown_0354 = 4.5f;
  this->state.unknown_11C84->unknown_0360 = 4.5f;
  Owner(this->state.unknown_11C84).Face(this->state.offset[0], this->state.offset[2]);
  this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
  Owner(this->state.unknown_11C84).render.angle = 180.0f - Owner(this->state.unknown_11C84).render.angle;

  if (Owner(this->state.unknown_11C84).render.angle < 0.0f) {
    Owner(this->state.unknown_11C84).render.angle = Owner(this->state.unknown_11C84).render.angle + 360.0f;
  }

  CEffectPrim& ball = Owner(this->state.unknown_11C84);
  ball.state.unknown_02BC = -(ball.CalcDist(this->state.offset[0], this->state.offset[2]) / ball.state.lifetime);
  reinterpret_cast<matrix*>(this->state.unknown_11C84->matrix1)->MakeYRotation(GetRadian(static_cast<int>(this->state.unknown_11C84->unknown_030C)));
  this->state.unknown_11C84->velocity.y = -5.0f;
  this->state.unknown_11C84->unknown_01E8 = 1;
  this->state.unknown_11C84->textureCount = 6;
  AllocateTextures(*this->state.unknown_11C84);

  if (F1 != 0) {
    this->state.unknown_11C84->textures[0] = textures.Load("effect\\twirl_soft.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = textures.Load("effect\\thunder_ball_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = textures.Load("effect\\twirl_soft.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = textures.Load("effect\\thunder_ball_d.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = textures.Load("effect\\twirl_soft.bmp", 0, false, true, false);
  } else {
    this->state.unknown_11C84->textures[0] = textures.Load("effect\\thunder_ball_a.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[1] = textures.Load("effect\\thunder_ball_b.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[2] = textures.Load("effect\\thunder_ball_c.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[3] = textures.Load("effect\\thunder_ball_d.bmp", 0, false, true, false);
    this->state.unknown_11C84->textures[4] = textures.Load("effect\\thunder_ball_e.bmp", 0, false, true, false);
  }

  this->state.unknown_11C84->textures[5] = textures.Load("effect\\thunder_ball_f.bmp", 0, false, true, false);
}

// OFFSET: 0x00C40E70
// FUNCTION: fn_20132
void CRagEffect::AirTexture(TextureCache& textures, const char* name) {
  const int frame = this->state.unknown_0178;
  const RenderObjectState& parent = this->state.parent->render;
  this->render.position.x = parent.position.x;
  this->render.position.y = parent.position.y;
  this->render.position.z = parent.position.z;

  if (frame != 0) {
    return;
  }

  this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 98, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
  this->state.unknown_11C84->unknown_01C4 = 5;
  this->state.unknown_11C84->lifetime = this->state.lifetime;
  AllocateTextures(*this->state.unknown_11C84);
  this->state.unknown_11C84->textures[0] = textures.Load(name, 0, false, true, false);
  this->state.unknown_11C84->scale = 0.0f;
  this->state.unknown_11C84->slots[0].life = 1;
  this->state.unknown_11C84->slots[0].process = 0;
  this->state.unknown_11C84->slots[0].distance = 5.0f;
  this->state.unknown_11C84->slots[0].alphaB = 0;
  EffectPrimitiveSlot& slot = this->state.unknown_11C84->slots[0];
  slot.vecBNow.x = this->state.offset[0];
  slot.vecBNow.y = this->state.offset[1];
  slot.vecBNow.z = this->state.offset[2];
  this->state.unknown_11C84->slots[0].fullDisplayAngle = static_cast<std::int16_t>(std::max(rand() % 360, 0));
  Get_Angle(this->state.offset[0] - this->render.position.x, this->state.offset[2] - this->render.position.z, &this->state.unknown_11C84->slots[0].riseAngle);
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
  if (this->state.unknown_0178 != 0) {
    return;
  }

  for (int angle = -90; angle < 180; angle += 90) {
    this->state.unknown_11C84 = &CreateEffectParticle(*this, this->render, 8, 0.0f, 0.0f, 0.0f, WorldObjects())->state;
    this->state.unknown_11C84->lifetime = 60;
    const float deltaZ = this->state.offset[2] - this->render.position.z;
    EffectPrimitiveState& launched = *this->state.unknown_11C84;
    launched.velocity.x = this->state.offset[0] - this->render.position.x;
    launched.velocity.z = deltaZ;
    launched.velocity.y = -10.0f;
    this->state.unknown_11C84->unknown_0348 = Owner(this->state.unknown_11C84).CalcDist(
      this->state.offset[0] - (this->state.offset[0] - this->render.position.x) * 0.5f,
      this->state.offset[2] - (this->state.offset[2] - this->render.position.z) * 0.5f
    );
    Owner(this->state.unknown_11C84).Face(
      (this->render.position.x + this->render.position.x) - this->state.offset[0], (this->render.position.z + this->render.position.z) - this->state.offset[2]
    );
    this->state.unknown_11C84->unknown_030C = -Owner(this->state.unknown_11C84).render.angle;
    const float speed = this->state.unknown_11C84->unknown_0348 / this->state.unknown_11C84->lifetime;
    this->state.unknown_11C84->unknown_02BC = speed + speed;
    this->state.unknown_11C84->unknown_02EC = 2.0f;
    const float rise = -(this->state.unknown_11C84->unknown_02EC / this->state.unknown_11C84->lifetime);
    this->state.unknown_11C84->unknown_02F0 = rise + rise;
    this->state.unknown_11C84->unknown_0334 = -2.0f;
    const float spread = -(this->state.unknown_11C84->unknown_0334 / this->state.unknown_11C84->lifetime);
    this->state.unknown_11C84->unknown_0340 = spread + spread;
    this->state.unknown_11C84->unknown_0314 = static_cast<float>(angle);
    EffectPrimitiveState& particle = *this->state.unknown_11C84;
    reinterpret_cast<matrix*>(particle.matrix1)->MakeYRotation(GetRadian(static_cast<int>(particle.unknown_030C)));
    this->state.unknown_11C84->unknown_03A0 = RotatedForward(this->state.unknown_11C84->matrix1, 1.0f);
    this->state.unknown_11C84->unknown_0290[0] = this->state.unknown_11C84->unknown_03A0.x;
    this->state.unknown_11C84->unknown_0290[1] = this->state.unknown_11C84->unknown_03A0.y;
    this->state.unknown_11C84->unknown_0290[2] = this->state.unknown_11C84->unknown_03A0.z;
    this->state.unknown_11C84->red = std::max(rand() % 81, 0) + 160;
    this->state.unknown_11C84->green = std::max(rand() % 81, 0) + 160;
    this->state.unknown_11C84->blue = 255;
    this->state.unknown_11C84->scale = static_cast<float>(std::max(rand() % 6, 0)) * 0.1f + 1.5f;
    Owner(this->state.unknown_11C84).SetSpriteName(ResourceMgr::Instance(), "이팩트\\particle1.spr");
    Owner(this->state.unknown_11C84).SetActionName(ResourceMgr::Instance(), "이팩트\\particle1.act");
    this->state.unknown_11C84->unknown_01F4 = 7;
    Owner(this->state.unknown_11C84).InitSegment();
  }
}

// OFFSET: 0x00C1DC70
// FUNCTION: fn_20075
void CRagEffect::InitEZ2STRFrame() {
  const CEZeffectRes& clips = *static_cast<const CEZeffectRes*>(this->state.resource);
  this->state.currentLayer = 0;
  this->state.unknown_0178 = 0;
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
  state.unknown_013C[6] = std::bit_cast<std::uint32_t>(vertex.x + x);
  state.unknown_013C[7] = std::bit_cast<std::uint32_t>(vertex.y + y);
  state.unknown_013C[8] = std::bit_cast<std::uint32_t>(vertex.z + z);
  state.unknown_0160 = x == 0.0f && y == -80.0f && z == 0.0f ? 0 : 1;

  float ratio;
  const CEZeffectRes& clips = *static_cast<const CEZeffectRes*>(state.resource);

  for (int layer = 0; layer < clips.clipCount; ++layer) {
    state.layerDrawn[layer] = false;
    const EZeffectClip& clip = clips.clips[layer];

    if (clip.frameCount == 0 || state.currentKey[layer] == -1) {
      continue;
    }

    const EZeffectKeyFrame* key = &clip.frames[state.currentKey[layer]];
    const int elapsed = state.unknown_0178 - key->frame;

    if (elapsed < 0) {
      continue;
    }

    if (elapsed != 0 && key[1].frame == state.unknown_0178) {
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
  return this->ProcessEZ2STR(0.0f, -80.0f, 0.0f);
}
