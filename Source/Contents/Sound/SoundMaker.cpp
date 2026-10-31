#include "Contents/Sound/SoundMaker.h"

#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/EffectActors.h"
#include "Contents/Actors/GndRiser.h"
#include "Contents/Actors/MercenaryAI.h"
#include "Contents/Actors/Npc.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/MsgEffect.h"
#include "Contents/MessageTable.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/World/World.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/World/WorldResource.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24786
int g_groundMarkEnabled;

// GLOBAL: gl_25141
std::uint32_t g_groundMarkAid;

// GLOBAL: gl_25142
std::uint32_t g_groundMarkAltAid;

// GLOBAL: gl_25143
std::uint32_t g_groundMarkAltEnabled;

static bool CallSlot14(CGameActor* actor) {
  union {
    void (CGameActor::*asVoid)();
    bool (CGameActor::*asBool)();
  } slot14;
  slot14.asVoid = &CGameActor::Slot14;
  return (actor->*slot14.asBool)();
}

// OFFSET: 0x00DA1190
// FUNCTION: fn_22354
CCorpse::CCorpse() {
  this->render.visible = 1;
  this->render.isLieOnGround = true;
  this->spriteOffset = 0.0f;
  this->render.unknown_008C = timeGetTime();
}

// OFFSET: 0x00DA0DE0
// FUNCTION: fn_22352
CBlowEffect::CBlowEffect() {
  this->SetSpriteName(ResourceMgr::Instance(), "이팩트\\이팩트.spr");
  this->SetActionName(ResourceMgr::Instance(), "이팩트\\이팩트.act");
  this->render.animation = 0;
}

// OFFSET: 0x00DA1210
// FUNCTION: fn_22355
CGndRiser::CGndRiser() {
}

// OFFSET: 0x00DA1270
// FUNCTION: fn_22356
CHalluNPC::CHalluNPC(CGameActor* source) {
  this->render.visible = 1;
  this->render.unknown_00A4Bytes[0] = 0;
  this->render.unknown_008C = timeGetTime();
  this->SetUnknown0260(source->unknown_0260);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->unknown_02CC = source->unknown_02CC;
  this->SetUnknown03C4(source->Slot55());
  this->render.position = source->render.position;
  this->render.unknown_005C = source->render.unknown_005C;
  this->render.unknown_0078 = source->render.unknown_0078;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(ResourceMgr::Instance(), source->GetActName());
  this->render.unknown_00A2 = source->render.unknown_00A2;
  this->render.unknown_0080 = source->render.unknown_0080;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.unknown_007C = source->render.unknown_007C;
  this->render.unknown_0048 = 0;
  this->stateFlags = source->Slot13();
  this->Slot18(source->GetEffectiveJob(-1), source->unknown_0260, 0);
  this->guildId = source->Slot50();
  this->render.accountId = 0;
  this->unknown_0440 = 0.0f;
  this->unknown_0454 = 0;
  this->unknown_0444 = 0x10000000;
  this->unknown_03E1 = true;
}

// OFFSET: 0x00DA1AF0
// FUNCTION: fn_22359
CHalluPlant::CHalluPlant() {
  this->render.visible = 1;
  this->render.unknown_00A4Bytes[0] = 0;
  this->render.unknown_008C = timeGetTime();
  this->SetUnknown0260(0);
  this->SetActorJob(1079);
  this->unknown_02CC = 0;
  this->SetUnknown03C4(-1);
  this->render.position.x = 0.0f;
  this->render.position.y = 0.0f;
  this->render.position.z = 0.0f;
  this->render.unknown_005C = 1.0f;
  this->render.unknown_0078 = 0;
  this->render.scale = 1.0f;
  this->render.animation = 0;
  this->render.previousAnimation = 4;
  this->render.curMotion = 0;
  this->SetActionName(ResourceMgr::Instance(), "sprite\\몬스터\\blue_plant.act");
  this->render.unknown_00A2 = 0;
  this->render.unknown_0080 = 0;
  this->render.isLieOnGround = false;
  this->render.isSingleColor = false;
  this->render.unknown_007C = 0;
  this->render.unknown_0048 = 0;
  this->stateFlags = 0;
  this->Slot18(this->GetEffectiveJob(-1), this->unknown_0260, 0);
  this->guildId = 0;
  this->render.accountId = 0;
  this->unknown_0440 = 0;
  this->unknown_03E1 = true;
}

// OFFSET: 0x00DA1CB0
// FUNCTION: fn_22360
CMasterChaser::CMasterChaser() {
  this->unknown_0138 = 0;
}

// OFFSET: 0x00DA1D10
// FUNCTION: fn_22361
CNpc::CNpc() {
  this->unknown_042C = nullptr;
  this->unknown_0430 = nullptr;
  this->unknown_0420 = 0;
  this->unknown_0424 = timeGetTime();
  this->unknown_0428 = 0;
  this->unknown_0434 = nullptr;
  this->unknown_0438 = 9999;
}

// OFFSET: 0x00DA1DB0
// FUNCTION: fn_22362
CNumEffect::CNumEffect() {
  this->SetSpriteName(ResourceMgr::Instance(), "이팩트\\숫자.spr");
  this->SetActionName(ResourceMgr::Instance(), "이팩트\\숫자.act");
  this->render.animation = 0;
  this->render.unknown_00A2 = 1;
}

// OFFSET: 0x00DA1E30
// FUNCTION: fn_22363
CSoundMaker::CSoundMaker() {
  this->file.clear();
  this->volume = 100.0f;
  this->range = 1.0f;
  this->width = 3;
  this->height = 3;
  this->radius = SquareRoot(18.0f);
  this->lastTick = timeGetTime() - 4000u;
}

// OFFSET: 0x00DA2220
// FUNCTION: fn_22364
CNpc::~CNpc() {
  if (this->unknown_042C) {
    g_windowMgr.Schedule(this->unknown_042C);
    this->unknown_042C = nullptr;
  }

  if (this->unknown_0430) {
    g_windowMgr.Schedule(this->unknown_0430);
    this->unknown_0430 = nullptr;
  }
}

CBlowEffect::~CBlowEffect() = default;

CBlurPC::~CBlurPC() = default;

CHalluNPC::~CHalluNPC() = default;

CHalluPC2::~CHalluPC2() = default;

CHalluPC::~CHalluPC() = default;

CHalluPlant::~CHalluPlant() = default;

CSoundMaker::~CSoundMaker() = default;

CCorpse::~CCorpse() = default;

CMasterChaser::~CMasterChaser() = default;

CNumEffect::~CNumEffect() = default;

// OFFSET: 0x00DA3340
// FUNCTION: fn_22403
bool CHalluPlant::OnProcess() {
  return ++this->unknown_0440 <= 60;
}

// OFFSET: 0x00DA3980
// FUNCTION: fn_22405
bool CNumEffect::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.previousAnimation = this->render.animation;
  this->render.position.y = this->baseHeight - elapsed * 0.18f;
  this->SetColor(static_cast<int>(255.0f - (elapsed + elapsed)), -1, -1, -1);

  if (this->render.scale > 1.0f) {
    this->render.scale = this->baseScale - elapsed * 0.24f;
  } else if (this->render.scale < 1.0f) {
    this->render.scale = 1.0f;
  }

  return elapsed <= 120.0f;
}

// OFFSET: 0x00DA3A60
// FUNCTION: fn_22406
bool CSoundMaker::OnProcess() {
  if (this->width == 0) {
    return true;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
  if (!player) {
    return true;
  }

  const float distance = this->CalcDist(player->render.position.x, player->render.position.z);
  if (distance - (this->radius + this->range) > 130.0f) {
    return true;
  }

  const float deltaX = player->render.position.x - this->render.position.x;
  const float deltaZ = player->render.position.z - this->render.position.z;
  float signX = 1.0f;
  float signZ = 1.0f;
  if (deltaX < 0.0f && deltaZ >= 0.0f) {
    signX = -1.0f;
  } else if (deltaX < 0.0f && deltaZ < 0.0f) {
    signX = -1.0f;
    signZ = -1.0f;
  } else if (deltaX >= 0.0f && deltaZ < 0.0f) {
    signZ = -1.0f;
  }

  const float slope = deltaZ / std::fmax(deltaX, 0.001f);
  float offsetX;
  float offsetZ;
  if (!(static_cast<float>(this->height / this->width) >= std::fabs(slope))) {
    offsetZ = static_cast<float>(this->height) * signZ * 0.5f;
    offsetX = offsetZ / std::fmax(slope, 0.001f);
  } else {
    offsetX = static_cast<float>(this->width) * signX * 0.5f;
    offsetZ = slope * offsetX;
  }

  const float x = this->render.position.x + offsetX;
  const float z = this->render.position.z + offsetZ;
  if (player->CalcDist(x, z) - this->range > 75.0f || this->file.empty()) {
    return true;
  }

  if (timeGetTime() > this->lastTick + this->cycle) {
    this->lastTick = timeGetTime();
    g_soundMgr->PlayWave(
      this->file.c_str(), x - player->render.position.x, 0.0f, z - player->render.position.z, TruncateToInt(this->range),
      TruncateToInt(this->range / 6.0f), this->volume, 0
    );
  }
  return true;
}

// OFFSET: 0x00DA2AF0
// FUNCTION: fn_22378
int CPc::Slot46() {
  return this->unknown_04D0;
}

// OFFSET: 0x00DA2B00
// FUNCTION: fn_22379
float* CPc::GetExtent(float* extent) const {
  extent[0] = this->unknown_04EC - this->unknown_04E4;
  extent[1] = this->unknown_04F0 - this->unknown_04E8;
  return extent;
}

// OFFSET: 0x00DA2FA0
// FUNCTION: fn_22397
bool CBlurPC::OnProcess() {
  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (scene) {
    const CRenderObject* owner = scene->GetGameActorByAidIncludeMe(this->render.accountId);
    if (owner && (owner->render.flags & 0x10) != 0) {
      this->render.flags = 0x10;
      this->render.pulsePhases[0] = 45;
    }
  }

  if (this->render.color < static_cast<std::uint32_t>(this->unknown_04FC)) {
    return false;
  }

  this->render.color -= this->unknown_04FC;
  return true;
}

// OFFSET: 0x00DA3240
// FUNCTION: fn_22401
bool CHalluPC2::OnProcess() {
  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (scene) {
    const CRenderObject* owner = scene->GetGameActorByAidIncludeMe(this->render.accountId);
    if (owner && (owner->render.flags & 0x10) != 0) {
      this->render.flags = 0x10;
      this->render.pulsePhases[0] = 45;
    }
  }

  ++this->unknown_050C;
  if (this->render.color < static_cast<std::uint32_t>(this->unknown_04FC)) {
    return false;
  }

  this->render.color -= this->unknown_04FC;
  return true;
}

// OFFSET: 0x00DA4100
// FUNCTION: fn_22410
void CCorpse::Render(int argument) {
  if (this->render.visible) {
    this->DrawShadow(argument, this->render.scale);
    this->DrawSprite(argument, false, this->spriteOffset);
  }
}

// OFFSET: 0x00DA44B0
// FUNCTION: fn_22415
void CHalluPlant::Render(int argument) {
  this->DrawSprite(argument, true, 0.0f);
}

// OFFSET: 0x00DA2B40
// FUNCTION: fn_22380
bool CPc::Slot58() {
  return this->unknown_04E0;
}

// OFFSET: 0x00DA2B50
// FUNCTION: fn_22381
int CPc::Slot56() {
  return this->unknown_04DC;
}

// OFFSET: 0x00DA2B60
// FUNCTION: fn_22382
int CPc::Slot42() {
  return this->unknown_04CC;
}

// OFFSET: 0x00DA2B70
// FUNCTION: fn_22383
std::int64_t CPc::Slot35() {
  return (static_cast<std::int64_t>(this->unknown_044C) << 32) | static_cast<std::uint32_t>(this->weapon);
}

// OFFSET: 0x00DA2BC0
// FUNCTION: fn_22384
void Nullsub492() {
}

// OFFSET: 0x00DA2BD0
// FUNCTION: fn_22385
void Nullsub493() {
}

// OFFSET: 0x00DA2BE0
// FUNCTION: fn_22386
void Nullsub494() {
}

// OFFSET: 0x00DA2C30
// FUNCTION: fn_22387
void Nullsub495() {
}

// OFFSET: 0x00DA2C40
// FUNCTION: fn_22388
void Nullsub496() {
}

// OFFSET: 0x00DA2C50
// FUNCTION: fn_22389
void Nullsub497() {
}

// OFFSET: 0x00DA2C60
// FUNCTION: fn_22390
void Nullsub498() {
}

// OFFSET: 0x00DA2C70
// FUNCTION: fn_22391
void Nullsub499() {
}

// OFFSET: 0x00DA2CD0
// FUNCTION: fn_22392
void Nullsub500() {
}

// OFFSET: 0x00DA2CE0
// FUNCTION: fn_22393
void Nullsub501() {
}

// OFFSET: 0x00DA2D70
// FUNCTION: fn_22394
void Nullsub502() {
}

// OFFSET: 0x00DA2E00
// FUNCTION: fn_22395
void CMasterChaser::ClearUnknown0138() {
  this->unknown_0138 = 0;
}

// OFFSET: 0x00DA3F50
// FUNCTION: fn_22408
void CNpc::Slot16() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int column;
  int row;
  mode->WorldCell(this->render.position.x, this->render.position.z, column, row);

  switch (this->GetEffectiveJob(-1)) {
    case 45:
    case 111:
    case 139:
      break;
    default:
      mode->SendMsg(12, this->render.accountId, column, row, 0);
      break;
  }
}

// OFFSET: 0x00DA61C0
// FUNCTION: fn_22434
void CNpc::Slot17() {
  switch (this->GetEffectiveJob(-1)) {
    case 45:
    case 104:
    case 111:
    case 139:
      break;
    default:
      g_modeMgr.GetGameMode()->SendMsg(13, this->render.accountId, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA5800
// FUNCTION: fn_22429
void CPc::Slot59(bool value) {
  this->unknown_04E0 = value;
}

// OFFSET: 0x00DA5810
// FUNCTION: fn_22430
void CPc::Slot57(int value) {
  this->unknown_04DC = value;
}

// OFFSET: 0x00DA4150
// FUNCTION: fn_22411
void CGndRiser::Render(int) {
}

// OFFSET: 0x00DA30B0
// FUNCTION: fn_22399
bool CGndRiser::OnProcess() {
  const float frame = static_cast<float>((timeGetTime() - this->render.unknown_008C) / 24.0);
  if (frame > static_cast<float>(this->lifetime)) {
    return false;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const float half = static_cast<float>(this->lifetime / 2);
    float alpha;
    if (frame > half) {
      const float rise = frame - half;
      const float span = static_cast<float>(this->lifetime) * g_depthAverageHalf;
      alpha = (rise * rise - span * span) / 25.0f;
    } else {
      alpha = -(frame * frame) / 25.0f;
    }

    using GroundSlot15 = void(__thiscall*)(GroundGeometry*, float, int, int);
    GroundGeometry* ground = mode->scene->ground;
    reinterpret_cast<GroundSlot15>((*reinterpret_cast<void***>(ground))[15])(ground, alpha, this->screenX, this->screenY);
  }

  return true;
}

// OFFSET: 0x00DA4960
// FUNCTION: fn_22417
void CSoundMaker::Render(int) {
}

// OFFSET: 0x00DA4970
// FUNCTION: fn_22418
void CBlowEffect::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message != 14 || a != 0) {
    CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const float* position = reinterpret_cast<const float*>(b);
  this->render.position.x = position[0];
  this->render.position.y = position[1];
  this->render.position.z = position[2];
  this->render.visible = 1;
  this->render.color = (d | e) ? 0xFFFF0000u : 0xFFFFFFFFu;
  this->render.unknown_008C = timeGetTime();
}

// OFFSET: 0x00DA4A00
// FUNCTION: fn_22419
void CBlurPC::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  switch (message) {
    case 0:
      this->SetColor(180, b, d, f);
      this->unknown_04FC = 0x8000000;
      break;
    case 1:
      this->render.color &= (b << 24) + 0xFFFFFF;
      this->unknown_04FC = d << 24;
      break;
    case 2:
      this->unknown_04F8 = *reinterpret_cast<const float*>(&b);
      break;
    case 3:
      this->render.isMotionFreezed = 1;
      break;
    default:
      CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4AD0
// FUNCTION: fn_22420
void CCorpse::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x00DA4B10
// FUNCTION: fn_22421
void CGndRiser::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message == 14 && a == 0) {
    const float* position = reinterpret_cast<const float*>(b);
    this->render.position.x = position[0];
    this->render.position.y = position[1];
    this->render.position.z = position[2];
    this->render.visible = 0;
    this->render.unknown_008C = timeGetTime();
    this->lifetime = 30;

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode) {
      mode->WorldCell(this->render.position.x, this->render.position.z, this->screenX, this->screenY);
      this->screenX /= 2;
      this->screenY /= 2;
    }
    return;
  }

  CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x00DA4BF0
// FUNCTION: fn_22422
void CHalluNPC::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CNpc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  switch (message) {
    case 0:
      this->SetColor(180, b, d, f);
      this->unknown_0444 = 0x8000000;
      break;
    case 1:
      this->render.color &= (b << 24) + 0xFFFFFF;
      this->unknown_0444 = d << 24;
      break;
    case 2:
      this->unknown_0440 = *reinterpret_cast<const float*>(&b);
      break;
    case 3:
      this->render.isMotionFreezed = 1;
      break;
    default:
      CNpc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4CC0
// FUNCTION: fn_22423
void CHalluPC2::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  switch (message) {
    case 0:
      this->SetColor(180, 255, 55, 55);
      this->unknown_04FC = 0x8000000;
      break;
    case 1:
      this->render.color &= (b << 24) + 0xFFFFFF;
      this->unknown_04FC = d << 24;
      break;
    case 2:
      this->unknown_04F8 = *reinterpret_cast<const float*>(&b);
      break;
    case 3:
      this->render.isMotionFreezed = 1;
      break;
    default:
      CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4D90
// FUNCTION: fn_22424
void CHalluPC::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  switch (message) {
    case 0:
      this->SetColor(180, b, d, f);
      this->unknown_04FC = 0x8000000;
      break;
    case 1:
      this->render.color &= (b << 24) + 0xFFFFFF;
      this->unknown_04FC = d << 24;
      break;
    case 2:
      this->unknown_04F8 = *reinterpret_cast<const float*>(&b);
      break;
    case 3:
      this->render.isMotionFreezed = 1;
      break;
    default:
      CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4E60
// FUNCTION: fn_22425
void CHalluPlant::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CNpc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  switch (message) {
    case 0:
      this->SetColor(180, b, d, f);
      break;
    case 1:
      this->render.color &= (b << 24) + 0xFFFFFF;
      break;
    case 3:
      this->render.isMotionFreezed = 1;
      break;
    default:
      CNpc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

struct NpcAppearInfo {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int job;
  /* 0008 */ int unknown_0008[22];
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int unknown_0068[3];
  /* 0074 */ int unknown_0074;
};

// OFFSET: 0x00DA4EF0
// FUNCTION: fn_22426
void CNpc::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message == 4 && a == 0) {
    CGameActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    const NpcAppearInfo* info = reinterpret_cast<const NpcAppearInfo*>(b);
    switch (this->unknown_0314) {
      case 8:
      case 9:
      case 10:
      case 13:
      case 14:
        this->unknown_02EC = info->unknown_0074;
        break;
    }
    this->render.visible = IsVisibleJob(info->job);
    this->Slot49(info->guildId, info->guildVersion);
    return;
  }

  if (message == 0x22 && a == 0) {
    if (g_session.IsCompanionAid(this->render.accountId)) {
      if (this->unknown_042C == nullptr) {
        this->unknown_042C = new UIPlayerGage();
        switch (this->unknown_0314) {
          case 8: {
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetFirstValues(
              [] { return static_cast<std::int64_t>(g_session.m_homunHp); },
              [] { return static_cast<std::int64_t>(*reinterpret_cast<int*>(g_session.unknown_55C8)); }
            );
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetSecondValues(
              [] { return static_cast<std::int64_t>(g_session.m_homunSp); },
              [] { return static_cast<std::int64_t>(g_session.unknown_55D0); }
            );
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetThirdValues(
              [] { return static_cast<std::int64_t>(g_session.unknown_55F0); },
              [] { return static_cast<std::int64_t>(g_session.unknown_55F4); }
            );
            this->unknown_042C->SetSize(60, 13);
            static_cast<UIPlayerGage*>(this->unknown_042C)->unknown_009C = 8;
            break;
          }
          case 7: {
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetThirdValues(
              [] { return static_cast<std::int64_t>(g_session.unknown_1044); },
              [] { return static_cast<std::int64_t>(*reinterpret_cast<int*>(g_session.unknown_1048)); }
            );
            this->unknown_042C->SetSize(60, 5);
            static_cast<UIPlayerGage*>(this->unknown_042C)->unknown_009C = 7;
            break;
          }
          case 9: {
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetFirstValues(
              [] { return static_cast<std::int64_t>(g_session.m_mercenaryHp); },
              [] { return static_cast<std::int64_t>(g_session.m_mercenaryMaxHp); }
            );
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetSecondValues(
              [] { return static_cast<std::int64_t>(g_session.m_mercenarySp); },
              [] { return static_cast<std::int64_t>(g_session.m_mercenaryMaxSp); }
            );
            this->unknown_042C->SetSize(60, 9);
            break;
          }
          case 10: {
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetFirstValues(
              [] { return static_cast<std::int64_t>(g_session.unknown_5BC4); },
              [] { return static_cast<std::int64_t>(g_session.unknown_5BCC); }
            );
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetSecondValues(
              [] { return static_cast<std::int64_t>(g_session.unknown_5BC8); },
              [] { return static_cast<std::int64_t>(g_session.unknown_5BD0); }
            );
            this->unknown_042C->SetSize(60, 9);
            break;
          }
          case 13: {
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetFirstValues(
              [this] { return g_session.FindUnknown0784(this->render.accountId); },
              [this] { return g_session.FindUnknown078C(this->render.accountId); }
            );
            this->unknown_042C->SetSize(60, 5);
            static_cast<UIPlayerGage*>(this->unknown_042C)->unknown_009C = 13;
            break;
          }
          case 14: {
            static_cast<UIPlayerGage*>(this->unknown_042C)->SetFirstValues(
              [this] { return g_session.FindUnknown0784(this->render.accountId); },
              [this] { return g_session.FindUnknown078C(this->render.accountId); }
            );
            this->unknown_042C->SetSize(60, 5);
            static_cast<UIPlayerGage*>(this->unknown_042C)->unknown_009C = 14;
            break;
          }
        }
        g_windowMgr.AddRenderedFront(this->unknown_042C);
        if (this->unknown_042C == nullptr) {
          return;
        }
      }
      this->unknown_042C->Move(
        this->render.screenX - 30, this->render.screenY + g_renderer->viewport.height * 12 / 480
      );
      return;
    }

    if (this->unknown_0314 == 13 || this->unknown_0314 == 14) {
      return;
    }

    if (this->unknown_0430 == nullptr) {
      this->unknown_0430 = new UIPcGage();
      this->unknown_0430->SetSize(60, 5);
      g_windowMgr.AddRenderedFront(this->unknown_0430);
      if (this->unknown_0430 == nullptr) {
        return;
      }
    }
    static_cast<UIPcGage*>(this->unknown_0430)->SetValues(b, d);
    static_cast<UIPcGage*>(this->unknown_0430)->SetLimits(0, 0);
    this->unknown_0430->Move(this->render.screenX - 30, this->render.screenY + ScaleByScreenHeight(12));
    return;
  }

  CGameActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x00DA5610
// FUNCTION: fn_22427
void CNumEffect::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message == 14 && a == 0) {
    const float* position = reinterpret_cast<const float*>(b);
    this->render.position.x = position[0];
    this->render.position.y = position[1];
    this->render.position.z = position[2];
    this->render.visible = 1;
    this->render.position.y -= 12.0f;
    this->baseHeight = this->render.position.y;

    this->render.curMotion = d;
    if (d > 9 || d < 0) {
      this->render.curMotion = 0;
    }

    this->render.unknown_0078 = f;
    this->render.scale = 5.0f;
    this->baseScale = 5.0f;
    this->render.unknown_008C = timeGetTime();
    return;
  }

  if (message == 22 && a == 0) {
    return;
  }

  CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x00DA56E0
// FUNCTION: fn_22428
void CSoundMaker::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message != 14 || a != 0) {
    CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const WorldSoundRecord& sound = *reinterpret_cast<const WorldSoundRecord*>(b);
  this->file = sound.file;
  this->volume = sound.volume;
  this->width = sound.width;
  this->height = sound.height;
  this->range = sound.range;
  this->render.position.x = sound.position[0];
  this->render.position.y = sound.position[1];
  this->render.position.z = sound.position[2];
  this->radius = SquareRoot(static_cast<float>(this->width * this->width + this->height * this->height));
  this->cycle = static_cast<std::int64_t>(sound.cycle * 1000.0f);
  this->lastTick = timeGetTime() - this->cycle;
}

// OFFSET: 0x00DA5820
// FUNCTION: fn_22431
void CNpc::Slot18(int job, int value, int) {
  char name[64];
  this->SetActorJob(job);
  this->SetUnknown0260(value);
  this->render.unknown_0048 = 0;
  if (g_session.IsGuildMemberCharacter(this->GetEffectiveJob(-1), name)) {
    this->render.unknown_0048 = 1;
    this->render.name = name;
  }
}

// OFFSET: 0x00DA58A0
// FUNCTION: fn_22432
void CNpc::Slot21(int job) {
  char spriteName[128];
  char actionName[128];
  const char* spriteFormat;
  const char* actionFormat;

  this->render.unknown_010C = 0;
  this->render.unknown_0054 = g_session.GetNpcLocationRadius(job, false);
  if (g_session.IsNpcJob(job) == true) {
    this->render.unknown_010C = job;
    actionFormat = "NPC\\%s.act";
    spriteFormat = "NPC\\%s.spr";
  } else if (job >= 6017 && job <= 6046) {
    actionFormat = "인간족\\몸통\\%s.act";
    spriteFormat = "인간족\\몸통\\%s.spr";
  } else if (job >= 6001 && job <= 6052) {
    actionFormat = "homun\\%s.act";
    spriteFormat = "homun\\%s.spr";
  } else {
    this->render.unknown_010C = job;
    actionFormat = "몬스터\\%s.act";
    spriteFormat = "몬스터\\%s.spr";
  }

  std::sprintf(spriteName, spriteFormat, g_session.GetJobName(job, -1));
  std::sprintf(actionName, actionFormat, g_session.GetJobName(job, -1));
  this->SetSpriteName(ResourceMgr::Instance(), spriteName);
  this->SetActionName(ResourceMgr::Instance(), actionName);
}

// OFFSET: 0x00DA31B0
// FUNCTION: fn_22400
bool CHalluNPC::OnProcess() {
  const unsigned count = ++this->unknown_0454;
  if (this->render.color < static_cast<std::uint32_t>(this->unknown_0444)) {
    return false;
  }

  if (count == 110) {
    static_cast<EffectActor*>(static_cast<CRenderObject*>(this))->LaunchEffect(793, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
  if (count > 105 && (count & 3) == 0) {
    this->render.color -= this->unknown_0444;
  }
  return true;
}

// OFFSET: 0x00DA32B0
// FUNCTION: fn_22402
bool CHalluPC::OnProcess() {
  const unsigned count = ++this->unknown_050C;
  if (this->render.color < static_cast<std::uint32_t>(this->unknown_04FC)) {
    return false;
  }

  if (count == 110) {
    static_cast<EffectActor*>(static_cast<CRenderObject*>(this))->LaunchEffect(793, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
  if (count > 105 && (count & 3) == 0) {
    this->render.color -= this->unknown_04FC;
  }
  return true;
}

// OFFSET: 0x00DA3010
// FUNCTION: fn_22398
bool CCorpse::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.previousAnimation = this->Get8Dir(this->render.angle) + this->render.animation;
  this->render.curMotion = this->GetActRes()->FrameCount(this->render.previousAnimation) - 1;
  this->SetColor(static_cast<int>(255.0f - elapsed), -1, -1, -1);
  return elapsed < 255.0f;
}

// OFFSET: 0x00DA4160
// FUNCTION: fn_22412
void CHalluNPC::Render(int argument) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  if (StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags)) {
    return;
  }

  rand();
  rand();
  rand();
  rand();

  if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
    if (!this->render.visible) {
      this->UpdateTlvertex(matrix);
      return;
    }

    if ((this->render.color & 0xFF000000) == 0xFF000000) {
      this->DrawShadow(argument, this->render.scale);
    }
    if (this->render.enabled) {
      this->RenderMercenary(matrix, this->GetEffectiveJob(-1));
    } else {
      this->UpdateTlvertex(matrix);
    }
    return;
  }

  if (!this->render.visible) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (this->render.unknown_0070 != 6 && this->render.unknown_0070 != 3) {
    this->DrawShadow(argument, this->render.scale);
  }
  if (this->render.enabled) {
    this->DrawSprite(argument, this->IsMonsterClass(this->GetEffectiveJob(-1)), 0.0f);
  } else {
    this->UpdateTlvertex(matrix);
  }
}

// OFFSET: 0x00DA44D0
// FUNCTION: fn_22416
void CNpc::Render(int argument) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  switch (this->GetEffectiveJob(-1)) {
    case 45: {
      if (!this->render.enabled) {
        this->UpdateTlvertex(matrix);
        return;
      }

      float x;
      float y;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&this->render.position.x, matrix, &x, &y, &reciprocalDepth);
      const float scale = g_renderer->ScaleDistance(reciprocalDepth) / g_heightScale;
      this->render.unknown_005C = scale;

      float bounds[5];
      bounds[0] = x - g_renderer->viewport.uiScaleX * 50.0f * scale;
      bounds[1] = y - g_renderer->viewport.uiScaleY * 50.0f * scale;
      bounds[2] = g_renderer->viewport.uiScaleX * 50.0f * scale + x;
      bounds[3] = g_renderer->viewport.uiScaleY * 50.0f * scale + y;
      bounds[4] = reciprocalDepth;
      this->SetRenderInfo(bounds, x, y);

      if (!g_serviceOptions.Get(8)) {
        this->DrawShadow(argument, this->render.scale * 3.0f);
      }
      return;
    }
    case 104:
    case 139:
      return;
    case 111: {
      float x;
      float y;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&this->render.position.x, matrix, &x, &y, &reciprocalDepth);
      const float scale = g_renderer->ScaleDistance(reciprocalDepth) / g_heightScale;
      this->render.unknown_005C = scale;

      const float halfWidth = static_cast<float>(this->unknown_0294 / 2);
      const float halfHeight = static_cast<float>(this->unknown_0298 / 2);
      float bounds[5];
      bounds[0] = x - halfWidth * g_renderer->viewport.uiScaleX * scale;
      bounds[1] = y - halfHeight * g_renderer->viewport.uiScaleY * scale;
      bounds[2] = halfWidth * g_renderer->viewport.uiScaleX * scale + x;
      bounds[3] = y;
      bounds[4] = reciprocalDepth;
      this->SetRenderInfo(bounds, x, y);
      return;
    }
  }

  if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
    if (!this->render.visible) {
      return;
    }

    if ((this->render.color & 0xFF000000) == 0xFF000000) {
      this->DrawShadow(argument, this->render.scale);
    }
    if (this->render.enabled) {
      this->RenderMercenary(matrix, this->GetEffectiveJob(-1));
    } else {
      this->UpdateTlvertex(matrix);
    }
    return;
  }

  bool hidden = false;
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == 184) {
      hidden = true;
      break;
    }
  }

  if (hidden && this->unknown_0314 == 5 && (!this->render.visible || !this->render.enabled)) {
    this->SetColor(-1, 0, 0, 0);
    this->DrawSprite(argument, this->IsMonsterClass(this->GetEffectiveJob(-1)), 0.0f);
    return;
  }

  if (!this->render.visible) {
    return;
  }

  if ((this->render.color & 0xFF000000) == 0xFF000000) {
    this->DrawShadow(argument, this->render.scale);
  }
  if (this->render.enabled) {
    this->DrawSprite(argument, this->IsMonsterClass(this->GetEffectiveJob(-1)), 0.0f);
  } else {
    this->UpdateTlvertex(matrix);
  }
}

// OFFSET: 0x00DA2F00
// FUNCTION: fn_22396
bool CBlowEffect::OnProcess() {
  const float frame = static_cast<float>((timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.previousAnimation = this->render.animation;
  const int frameCount = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation));
  this->render.curMotion = static_cast<int>(frame / this->render.speed) % frameCount;
  this->PlayFrameSounds();
  return frame / this->render.speed / static_cast<float>(frameCount) < 1.0f;
}

// OFFSET: 0x00DA3DF0
// FUNCTION: fn_22407
void CNpc::UpdateBubblePositions() {
  if (this->unknown_042C) {
    this->unknown_042C->Move(
      this->render.screenX - 30,
      static_cast<int>(
        static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.unknown_005C + static_cast<float>(this->render.screenY)
      )
    );
    this->unknown_042C->Invalidate();
  }

  if (this->unknown_0430) {
    this->unknown_0430->Move(
      this->render.screenX - 30,
      static_cast<int>(
        static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.unknown_005C + static_cast<float>(this->render.screenY)
      )
    );
    this->unknown_0430->Invalidate();
  }

  this->UpdateHpGauge();
}

// OFFSET: 0x00DA1470
// FUNCTION: fn_22357
CHalluPC2::CHalluPC2(CPc* source) {
  this->unknown_0510 = source;
  this->render.visible = 1;
  this->render.unknown_00A4Bytes[0] = 0;
  this->render.unknown_008C = timeGetTime();
  this->SetUnknown0260(source->unknown_0260);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->SetUnknown0404(source->GetUnknown0404());
  this->Slot47(source->unknown_02CC);
  this->SetUnknown03C4(source->Slot55());
  this->render.position = source->render.position;
  this->render.unknown_005C = source->render.unknown_005C;
  this->render.unknown_0078 = source->render.unknown_0078;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(ResourceMgr::Instance(), source->GetActName());
  this->render.unknown_00A2 = source->render.unknown_00A2;
  this->render.unknown_0080 = source->render.unknown_0080;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.unknown_007C = source->render.unknown_007C;
  this->render.unknown_0048 = 0;
  this->unknown_0444 = source->unknown_0444;
  this->stateFlags = source->Slot13();
  this->unknown_0478 = source->unknown_0478;
  this->LoadEquipSprites(source->GetEquipSpriteNames());
  this->LoadEquipActions(source->GetEquipActionNames());
  this->unknown_0418 = source->unknown_0418;
  this->guildId = source->Slot50();
  this->render.accountId = source->render.accountId;
  this->unknown_04F8 = 0.0f;
  this->unknown_050C = 0;
  this->unknown_04FC = 0x10000000;
  this->render.unknown_0048 = source->render.unknown_0048;
  this->render.name = source->render.name;
  this->unknown_049C = 0;
  this->unknown_0450 = 0;
  this->unknown_0454 = 0;
  this->unknown_0458 = 0;
}

// OFFSET: 0x00DA0E60
// FUNCTION: fn_22353
CBlurPC::CBlurPC(CPc* source) {
  this->blurSource = source;
  this->render.visible = 1;
  this->render.unknown_00A4Bytes[0] = 0;
  this->render.unknown_008C = timeGetTime();
  this->SetUnknown0260(source->unknown_0260);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->SetUnknown0404(source->GetUnknown0404());
  this->Slot47(source->unknown_02CC);
  this->SetUnknown03C4(source->Slot55());
  this->render.position = source->render.position;
  this->render.unknown_005C = source->render.unknown_005C;
  this->render.unknown_0078 = source->render.unknown_0078;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(ResourceMgr::Instance(), source->GetActName());
  this->render.unknown_00A2 = source->render.unknown_00A2;
  this->render.unknown_0080 = source->render.unknown_0080;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.unknown_007C = source->render.unknown_007C;
  this->render.unknown_0048 = 0;
  this->unknown_0444 = source->unknown_0444;
  this->stateFlags = source->Slot13();
  this->unknown_0478 = source->unknown_0478;
  this->LoadEquipSprites(source->GetEquipSpriteNames());
  this->LoadEquipActions(source->GetEquipActionNames());
  this->unknown_0418 = source->unknown_0418;
  this->guildId = source->Slot50();
  this->render.accountId = source->render.accountId;
  this->unknown_04F8 = 0.0f;
  this->unknown_04FC = 0x10000000;
  this->render.unknown_0048 = source->render.unknown_0048;
  this->render.name = source->render.name;
  this->unknown_049C = 0;
  this->unknown_0450 = 0;
  this->unknown_0454 = 0;
  this->unknown_0458 = 0;
}

// OFFSET: 0x00DA17A0
// FUNCTION: fn_22358
CHalluPC::CHalluPC(CPc* source) {
  this->unknown_0510 = source;
  this->render.visible = 1;
  this->render.unknown_00A4Bytes[0] = 0;
  this->render.unknown_008C = timeGetTime();
  this->SetUnknown0260(source->unknown_0260);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->SetUnknown0404(source->GetUnknown0404());
  this->Slot47(source->unknown_02CC);
  this->SetUnknown03C4(source->Slot55());
  this->render.position = source->render.position;
  this->render.unknown_005C = source->render.unknown_005C;
  this->render.unknown_0078 = source->render.unknown_0078;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(ResourceMgr::Instance(), source->GetActName());
  this->render.unknown_00A2 = source->render.unknown_00A2;
  this->render.unknown_0080 = source->render.unknown_0080;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.unknown_007C = source->render.unknown_007C;
  this->render.unknown_0048 = 0;
  this->unknown_0444 = source->unknown_0444;
  this->stateFlags = source->Slot13();
  this->unknown_0478 = source->unknown_0478;
  this->LoadEquipSprites(source->GetEquipSpriteNames());
  this->LoadEquipActions(source->GetEquipActionNames());
  this->unknown_0418 = source->unknown_0418;
  this->guildId = source->Slot50();
  this->render.accountId = source->render.accountId;
  this->unknown_04F8 = 0.0f;
  this->unknown_050C = 0;
  this->unknown_04FC = 0x10000000;
  this->render.unknown_0048 = source->render.unknown_0048;
  this->render.name = source->render.name;
  this->unknown_03E1 = true;
  this->Slot44(source->Slot46(), 1);
  this->unknown_049C = 0;
  this->unknown_0450 = 0;
  this->unknown_0454 = 0;
  this->unknown_0458 = 0;
}

// OFFSET: 0x00DA28B0
// FUNCTION: fn_22377
void CNpc::UpdateMinimapMarker() {
  if (g_session.IsAllyActor(this->render.accountId, this->GetEffectiveJob(-1)) != 1) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  MinimapMarker* marker = mode->FindMinimapMarker(this->render.accountId);
  if (marker && marker->unknown_0004 != 9999) {
    if (this->unknown_0434) {
      if (marker->unknown_0004 == this->minimapMarkerCell[0] && marker->unknown_0006 == this->minimapMarkerCell[1]) {
        return;
      }
      if (this->unknown_0434) {
        this->unknown_0434->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->unknown_0434 = nullptr;
        this->unknown_0438 = 9999;
      }
    }

    const int base = marker->unknown_0004 < 6 ? 0x24 : 0x36;
    CMsgEffect* effect = mode->SpawnMsgEffect(base + marker->unknown_0004, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_0434 = effect;
    this->minimapMarkerCell[0] = marker->unknown_0004;
    this->minimapMarkerCell[1] = marker->unknown_0006;
    return;
  }

  if (this->unknown_0434) {
    this->unknown_0434->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_0434 = nullptr;
    this->unknown_0438 = 9999;
  }
}

// OFFSET: 0x00DA3360
// FUNCTION: fn_22404
bool CNpc::OnProcess() {
  if (!this->UpdateFadeOut()) {
    return false;
  }

  if (this->render.isSingleColor == true && timeGetTime() > this->render.started + 100) {
    this->render.overrideColor = 0xFFFF0000;
    this->render.isSingleColor = false;
  }

  bool result = true;
  switch (this->GetEffectiveJob(-1)) {
    case 45:
    case 139:
      break;
    case 104:
      if (timeGetTime() > this->unknown_0424 + this->unknown_0294 * 1000) {
        this->unknown_0424 = timeGetTime();
        this->LaunchEffect(*reinterpret_cast<int*>(&this->unknown_0170[4]), vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 111:
      this->ProcessDamageMessages();
      this->UpdateGaugePositions();
      result = CallSlot14(this);
      break;
    default:
      this->ProcessBodyState();
      this->UpdateStateEffects();
      this->ProcessDamageMessages();
      if (this->render.color != 0xFFFFFFFF) {
        this->ModifyArgb(8, 8, 8, 8);
      }
      this->UpdateGaugePositions();
      result = CallSlot14(this);
      break;
  }

  if (this->unknown_0314 != 7) {
    this->PlayFrameSounds();
  }

  WIN32_FIND_DATAA findData;
  if (g_session.IsHomunAid(this->render.accountId)) {
    if (!this->unknown_030C) {
      this->unknown_030C = reinterpret_cast<int>(new CMercenaryAI(0));
    } else if (reinterpret_cast<CMercenaryAI*>(this->unknown_030C)->state) {
      reinterpret_cast<CMercenaryAI*>(this->unknown_030C)->Process(this->render.accountId);
    } else {
      if (g_serviceOptions.Get(0xA4)) {
        HANDLE find = FindFirstFileA(".\\AI\\USER_AI", &findData);
        if (find == INVALID_HANDLE_VALUE) {
          CreateDirectoryA(".\\AI\\USER_AI", nullptr);
        }
        FindClose(find);
        CopyFileA(".\\AI\\AI.lua", ".\\AI\\USER_AI\\AI.lua", TRUE);
        reinterpret_cast<CMercenaryAI*>(this->unknown_030C)->Load(".\\AI\\USER_AI\\AI.lua");
      } else {
        reinterpret_cast<CMercenaryAI*>(this->unknown_030C)->Load(".\\AI\\AI.lua");
      }
      this->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (g_session.IsMercenaryAid(this->render.accountId)) {
    if (!this->unknown_0310) {
      this->unknown_0310 = reinterpret_cast<int>(new CMercenaryAI(1));
    } else if (reinterpret_cast<CMercenaryAI*>(this->unknown_0310)->state) {
      reinterpret_cast<CMercenaryAI*>(this->unknown_0310)->Process(this->render.accountId);
    } else {
      const char* path;
      if (g_serviceOptions.Get(0xAE)) {
        HANDLE find = FindFirstFileA(".\\AI\\USER_AI", &findData);
        if (find == INVALID_HANDLE_VALUE) {
          CreateDirectoryA(".\\AI\\USER_AI", nullptr);
        }
        FindClose(find);
        CopyFileA(".\\AI\\AI_M.lua", ".\\AI\\USER_AI\\AI_M.lua", TRUE);
        path = ".\\AI\\USER_AI\\AI_M.lua";
      } else {
        path = ".\\AI\\AI_M.lua";
      }
      reinterpret_cast<CMercenaryAI*>(this->unknown_0310)->Load(path);
      this->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (g_session.IsPetAid(this->render.accountId)) {
    this->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  this->UpdateBubblePositions();

  if (g_groundMarkEnabled && g_groundMarkAid == this->render.accountId) {
    this->SendMsg(nullptr, 0x86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else if (g_groundMarkAltEnabled && g_groundMarkAltAid == this->render.accountId) {
    this->SendMsg(nullptr, 0x86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 0x87, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  this->UpdateMinimapMarker();

  switch (this->unknown_0314) {
    case 7:
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
      this->SetColor(
        g_serviceOptions.Get(0x10A) ? 0x3A : static_cast<int>(this->render.color >> 24), static_cast<int>((this->render.color >> 16) & 0xFF),
        static_cast<int>((this->render.color >> 8) & 0xFF), static_cast<int>(this->render.color & 0xFF)
      );
      break;
  }

  return result;
}

// OFFSET: 0x00DA3FD0
// FUNCTION: fn_22409
void CBlurPC::Render(int argument) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  if (StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags)) {
    return;
  }

  rand();
  rand();
  rand();
  rand();

  if (!this->render.visible) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (g_serviceType == 4 && IsAdminAccount(this->render.accountId) && this->render.accountId != g_session.m_aid) {
    return;
  }

  if (this->render.unknown_0070 != 6 && this->render.unknown_0070 != 3) {
    this->DrawShadow(argument, this->render.scale);
  }
  if (!this->render.enabled) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (IsObfuscatedMessage(this->render.accountId)) {
    this->RenderOverlaySprite(matrix, 1);
    return;
  }

  if (this->unknown_0474 != 0) {
    this->DrawSprite(argument, false, 0.0f);
    return;
  }

  this->DrawLayers(matrix, 1, 2);
}

// OFFSET: 0x00DA42C0
// FUNCTION: fn_22413
void CHalluPC2::Render(int argument) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  if (StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags)) {
    return;
  }

  rand();
  rand();
  rand();
  rand();

  if (this->render.visible && this->render.enabled) {
    this->DrawLayers(matrix, 1, 1);
    return;
  }

  this->UpdateTlvertex(matrix);
}

// OFFSET: 0x00DA4340
// FUNCTION: fn_22414
void CHalluPC::Render(int argument) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (scene) {
    CRenderObject* source = scene->GetGameActorByAidIncludeMe(this->render.accountId);
    if (source && (source->render.flags & 0x10) != 0) {
      this->render.flags = 0x10;
      this->render.pulsePhases[0] = 45;
    }
  }

  if (StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags)) {
    return;
  }

  rand();
  rand();
  rand();
  rand();

  if (!this->render.visible) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (g_serviceType == 4 && IsAdminAccount(this->render.accountId) && this->render.accountId != g_session.m_aid) {
    return;
  }

  if (this->render.unknown_0070 != 6 && this->render.unknown_0070 != 3) {
    this->DrawShadow(argument, this->render.scale);
  }
  if (!this->render.enabled) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (IsObfuscatedMessage(this->render.accountId)) {
    this->RenderOverlaySprite(matrix, 1);
    return;
  }

  if (this->unknown_0474 != 0) {
    this->DrawSprite(argument, false, 0.0f);
    return;
  }

  this->DrawLayers(matrix, 1, 0);
}
