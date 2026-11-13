#include "Client/Mode/ModeMessage.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Jobs/JobId.h"
#include "Network/Packets/NoticeEnums.h"

#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/EffectActors.h"
#include "Contents/Actors/GndRiser.h"
#include "Contents/Actors/MercenaryAI.h"
#include "Contents/Actors/Npc.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/MsgEffect.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
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
#include "Render/Drawing/SpriteGeometry.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00DA1190
// FUNCTION: fn_22354
CCorpse::CCorpse() {
  this->render.visible = 1;
  this->render.isLieOnGround = true;
  this->spriteOffset = 0.0f;
  this->render.motionStartTime = timeGetTime();
}

// OFFSET: 0x00DA0DE0
// FUNCTION: fn_22352
CBlowEffect::CBlowEffect() {
  this->SetSpriteName("이팩트\\이팩트.spr");
  this->SetActionName("이팩트\\이팩트.act");
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
  this->render.visibilityOptions[0] = 0;
  this->render.motionStartTime = timeGetTime();
  this->SetSexBit(source->sex);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->isRiding = source->isRiding;
  this->SetMonsterTransformJob(source->GetMonsterTransformJob());
  this->render.position.x = source->render.position.x;
  this->render.position.y = source->render.position.y;
  this->render.position.z = source->render.position.z;
  this->render.distanceScale = source->render.distanceScale;
  this->render.spriteOffsetX = source->render.spriteOffsetX;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(source->GetActName());
  this->render.flatDepth = source->render.flatDepth;
  this->render.depthLayer = source->render.depthLayer;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.spriteAngle = source->render.spriteAngle;
  this->render.bodyPalette = 0;
  this->stateFlags = source->GetStateFlags();
  this->SetSprAct(source->GetEffectiveJob(-1), source->sex, 0);
  this->guildId = source->GetGdid();
  this->render.accountId = 0;
  this->hallucinationOffset = 0.0f;
  this->processCount = 0;
  this->fadeStep = 0x10000000;
  this->modelReady = true;
}

// OFFSET: 0x00DA1AF0
// FUNCTION: fn_22359
CHalluPlant::CHalluPlant() {
  this->render.visible = 1;
  this->render.visibilityOptions[0] = 0;
  this->render.motionStartTime = timeGetTime();
  this->SetSexBit(0);
  this->SetActorJob(1079);
  this->isRiding = 0;
  this->SetMonsterTransformJob(-1);
  this->render.position.x = 0.0f;
  this->render.position.y = 0.0f;
  this->render.position.z = 0.0f;
  this->render.distanceScale = 1.0f;
  this->render.spriteOffsetX = 0;
  this->render.scale = 1.0f;
  this->render.animation = 0;
  this->render.previousAnimation = 4;
  this->render.curMotion = 0;
  this->SetActionName("sprite\\몬스터\\blue_plant.act");
  this->render.flatDepth = 0;
  this->render.depthLayer = 0;
  this->render.isLieOnGround = false;
  this->render.isSingleColor = false;
  this->render.spriteAngle = 0;
  this->render.bodyPalette = 0;
  this->stateFlags = 0;
  this->SetSprAct(this->GetEffectiveJob(-1), this->sex, 0);
  this->guildId = 0;
  this->render.accountId = 0;
  this->processCount = 0;
  this->modelReady = true;
}

// OFFSET: 0x00DA1CB0
// FUNCTION: fn_22360
CMasterChaser::CMasterChaser() {
  this->chaseState = 0;
}

// OFFSET: 0x00DA1D10
// FUNCTION: fn_22361
CNpc::CNpc() {
  this->playerGaugeWindow = nullptr;
  this->pcGaugeWindow = nullptr;
  this->npcState = 0;
  this->effectTick = timeGetTime();
  this->appearanceInitialized = 0;
  this->attachedEffect = nullptr;
  this->attachedEffectId = 9999;
}

// OFFSET: 0x00DA1DB0
// FUNCTION: fn_22362
CNumEffect::CNumEffect() {
  this->SetSpriteName("이팩트\\숫자.spr");
  this->SetActionName("이팩트\\숫자.act");
  this->render.animation = 0;
  this->render.flatDepth = 1;
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
  if (this->playerGaugeWindow) {
    g_windowMgr.Schedule(this->playerGaugeWindow);
    this->playerGaugeWindow = nullptr;
  }

  if (this->pcGaugeWindow) {
    g_windowMgr.Schedule(this->pcGaugeWindow);
    this->pcGaugeWindow = nullptr;
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
  return ++this->processCount <= 60;
}

// OFFSET: 0x00DA3980
// FUNCTION: fn_22405
bool CNumEffect::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.motionStartTime) / 24.0);
  this->render.previousAnimation = this->render.animation;
  this->render.position.y = this->baseHeight - elapsed * 0.18f;
  this->SetColor(static_cast<int>(255.0f - (elapsed + elapsed)), -1, -1, -1);

  if (this->render.scale > 1.0f) {
    this->render.scale = this->baseScale - elapsed * 0.24f;
  } else if (this->render.scale < 1.0f) {
    this->render.scale = 1.0f;
  }

  if (elapsed > 120.0f) {
    return false;
  }
  return true;
}

// OFFSET: 0x00DA3A60
// FUNCTION: fn_22406
bool CSoundMaker::OnProcess() {
  if (this->width == 0) {
    return true;
  }

  const auto* player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
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
  if (deltaX >= 0.0f && deltaZ >= 0.0f) {
  } else if (deltaX < 0.0f && deltaZ >= 0.0f) {
    signX = -1.0f;
  } else if (deltaX < 0.0f && deltaZ < 0.0f) {
    signX = -1.0f;
    signZ = -1.0f;
  } else if (deltaX >= 0.0f && deltaZ < 0.0f) {
    signZ = -1.0f;
  }

  const float slope = deltaZ / std::max(deltaX, 0.001f);
  float offsetX;
  float offsetZ;
  if (!(static_cast<float>(this->height / this->width) >= std::fabs(slope))) {
    offsetZ = static_cast<float>(this->height) * signZ * 0.5f;
    offsetX = offsetZ / std::max(slope, 0.001f);
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
      this->file.c_str(), x - player->render.position.x, 0.0f, z - player->render.position.z, static_cast<int>(this->range),
      static_cast<int>(this->range / 6.0f), this->volume, 0);
  }
  return true;
}

// OFFSET: 0x00DA2AF0
// FUNCTION: fn_22378
int CPc::GetBodyCostume() {
  return this->bodyStyle;
}

// OFFSET: 0x00DA2B00
// FUNCTION: fn_22379
float* CPc::GetExtent(float* extent) const {
  extent[0] = this->extentRight - this->extentLeft;
  extent[1] = this->extentBottom - this->extentTop;
  return extent;
}

// OFFSET: 0x00DA2FA0
// FUNCTION: fn_22397
bool CBlurPC::OnProcess() {
  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (scene) {
    const CRenderObject* owner = scene->GetGameActorByAidIncludeMe(this->render.accountId);
    if (owner && 0 != (owner->render.flags & 0x10)) {
      this->render.flags = 0x10;
      this->render.pulsePhases[0] = 45;
    }
  }

  if (this->render.color >= static_cast<std::uint32_t>(this->destCellZ)) {
    this->render.color -= this->destCellZ;
    return true;
  }

  return false;
}

// OFFSET: 0x00DA3240
// FUNCTION: fn_22401
bool CHalluPC2::OnProcess() {
  CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (scene) {
    const CRenderObject* owner = scene->GetGameActorByAidIncludeMe(this->render.accountId);
    if (owner && 0 != (owner->render.flags & 0x10)) {
      this->render.flags = 0x10;
      this->render.pulsePhases[0] = 45;
    }
  }

  ++this->preMoveOn;
  if (this->render.color >= static_cast<std::uint32_t>(this->destCellZ)) {
    this->render.color -= this->destCellZ;
    return true;
  }

  return false;
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
bool CPc::IsArrowEffectSuppressed() {
  return this->arrowEffectSuppressed;
}

// OFFSET: 0x00DA2B50
// FUNCTION: fn_22381
int CPc::GetMadogearType() {
  return this->madogearType;
}

// OFFSET: 0x00DA2B60
// FUNCTION: fn_22382
int CPc::GetPkState() {
  return this->pkState;
}

// OFFSET: 0x00DA2B70
// FUNCTION: fn_22383
std::int64_t CPc::GetWeapon() {
  return this->weaponPair;
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
void CMasterChaser::ClearChaseState() {
  this->chaseState = 0;
}

// OFFSET: 0x00DA3F50
// FUNCTION: fn_22408
void CNpc::RegisterPos() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int column;
  int row;
  mode->WorldCell(this->render.position.x, this->render.position.z, column, row);

  switch (this->GetEffectiveJob(-1)) {
    case JT_WARPNPC:
    case JT_HIDDEN_NPC:
    case JT_HIDDEN_WARP_NPC:
      break;
    default:
      mode->SendMsg(MM_ADDREGISTERPOS, this->render.accountId, column, row, 0);
      break;
  }
}

// OFFSET: 0x00DA61C0
// FUNCTION: fn_22434
void CNpc::UnRegisterPos() {
  switch (this->GetEffectiveJob(-1)) {
    case JT_WARPNPC:
    case JT_EFFECTLAUNCHER:
    case JT_HIDDEN_NPC:
    case JT_HIDDEN_WARP_NPC:
      break;
    default:
      g_modeMgr.GetGameMode()->SendMsg(MM_DELETEREGISTERPOS, this->render.accountId, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA5800
// FUNCTION: fn_22429
void CPc::SetArrowEffectSuppressed(bool value) {
  this->arrowEffectSuppressed = value;
}

// OFFSET: 0x00DA5810
// FUNCTION: fn_22430
void CPc::SetMadogearType(int value) {
  this->madogearType = value;
}

// OFFSET: 0x00DA4150
// FUNCTION: fn_22411
void CGndRiser::Render(int) {
}

// OFFSET: 0x00DA30B0
// FUNCTION: fn_22399
bool CGndRiser::OnProcess() {
  const float frame = static_cast<float>((timeGetTime() - this->render.motionStartTime) / 24.0);
  if (frame > static_cast<float>(this->lifetime)) {
    return false;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    GroundGeometry* ground = mode->scene->ground;
    const float half = static_cast<float>(this->lifetime / 2);
    if (frame > half) {
      const float rise = frame - half;
      const float span = static_cast<float>(this->lifetime) * g_depthAverageHalf;
      const float alpha = (rise * rise - span * span) / 25.0f;
      ground->SetGndRiseAmount(alpha, this->screenX, this->screenY);
    } else {
      const float alpha = -frame * frame / 25.0f;
      ground->SetGndRiseAmount(alpha, this->screenX, this->screenY);
    }
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
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  if (message != 14) {
    CRenderObject::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }

  const float* position = reinterpret_cast<const float*>(static_cast<std::intptr_t>(b));
  this->render.position.x = position[0];
  this->render.position.y = position[1];
  this->render.position.z = position[2];
  this->render.visible = 1;
  this->render.color = c ? 0xFFFF0000u : 0xFFFFFFFFu;
  this->render.motionStartTime = timeGetTime();
}

// OFFSET: 0x00DA4A00
// FUNCTION: fn_22419
void CBlurPC::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
    switch (message) {
      case BM_RGB:
        this->SetColor(180, static_cast<int>(b), static_cast<int>(c), static_cast<int>(d));
        this->destCellZ = 0x8000000;
        return;
      case BM_ALPHA:
        this->render.color &= (static_cast<int>(b) << 24) + 0xFFFFFF;
        this->destCellZ = static_cast<int>(c) << 24;
        return;
      case BM_MOTIONY:
        this->motionY = *reinterpret_cast<const float*>(&b);
        return;
      case BM_MOTION_FREEZE:
        this->render.isMotionFreezed = 1;
        return;
    }

  CPc::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA4AD0
// FUNCTION: fn_22420
void CCorpse::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  CRenderObject::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA4B10
// FUNCTION: fn_22421
void CGndRiser::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  if (message == 14) {
    const float* position = reinterpret_cast<const float*>(static_cast<std::intptr_t>(b));
    this->render.position.x = position[0];
    this->render.position.y = position[1];
    this->render.position.z = position[2];
    this->render.visible = 0;
    this->render.motionStartTime = timeGetTime();
    this->lifetime = 30;

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode) {
      mode->WorldCell(this->render.position.x, this->render.position.z, this->screenX, this->screenY);
      this->screenX /= 2;
      this->screenY /= 2;
    }
    return;
  }

  EffectActor::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA4BF0
// FUNCTION: fn_22422
void CHalluNPC::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
    switch (message) {
      case BM_RGB:
        this->SetColor(180, static_cast<int>(b), static_cast<int>(c), static_cast<int>(d));
        this->fadeStep = 0x8000000;
        return;
      case BM_ALPHA:
        this->render.color &= (static_cast<int>(b) << 24) + 0xFFFFFF;
        this->fadeStep = static_cast<int>(c) << 24;
        return;
      case BM_MOTIONY:
        this->hallucinationOffset = *reinterpret_cast<const float*>(&b);
        return;
      case BM_MOTION_FREEZE:
        this->render.isMotionFreezed = 1;
        return;
    }

  CNpc::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA4CC0
// FUNCTION: fn_22423
void CHalluPC2::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
    switch (message) {
      case BM_RGB:
        this->SetColor(180, 255, 55, 55);
        this->destCellZ = 0x8000000;
        return;
      case BM_ALPHA:
        this->render.color &= (static_cast<int>(b) << 24) + 0xFFFFFF;
        this->destCellZ = static_cast<int>(c) << 24;
        return;
      case BM_MOTIONY:
        this->motionY = *reinterpret_cast<const float*>(&b);
        return;
      case BM_MOTION_FREEZE:
        this->render.isMotionFreezed = 1;
        return;
    }

  CPc::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA4D90
// FUNCTION: fn_22424
void CHalluPC::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
    switch (message) {
      case BM_RGB:
        this->SetColor(180, static_cast<int>(b), static_cast<int>(c), static_cast<int>(d));
        this->destCellZ = 0x8000000;
        return;
      case BM_ALPHA:
        this->render.color &= (static_cast<int>(b) << 24) + 0xFFFFFF;
        this->destCellZ = static_cast<int>(c) << 24;
        return;
      case BM_MOTIONY:
        this->motionY = *reinterpret_cast<const float*>(&b);
        return;
      case BM_MOTION_FREEZE:
        this->render.isMotionFreezed = 1;
        return;
    }

  CPc::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA4E60
// FUNCTION: fn_22425
void CHalluPlant::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  if (message != 0) {
    if (message == 1) {
      this->render.color &= (static_cast<int>(b) << 24) + 0xFFFFFF;
      return;
    }

    if (message == 3) {
      this->render.isMotionFreezed = 1;
      return;
    }

    CNpc::SendMsg(sender, message, b, c, d, e, 0, 0);
  } else {
    this->SetColor(180, static_cast<int>(b), static_cast<int>(c), static_cast<int>(d));
  }
}
struct NpcAppearInfo {
  /* 0000 */ int aid;
  /* 0004 */ int job;
  /* 0008 */ int padding_0008[22];
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int padding_0068[3];
  /* 0074 */ int ownerId;
};

// OFFSET: 0x00DA4EF0
// FUNCTION: fn_22426
void CNpc::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  if (message == 4) {
    CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    const NpcAppearInfo* info = reinterpret_cast<const NpcAppearInfo*>(static_cast<std::intptr_t>(b));
    switch (this->objectType) {
      case NPC_HO_TYPE:
      case NPC_MERSOL_TYPE:
      case NPC_ELEMENTAL_TYPE:
      case NPC_ABR_TYPE:
      case NPC_BIONIC_TYPE:
        this->emblemVersion = info->ownerId;
        break;
    }
    this->render.visible = g_session.IsVisibleJob(info->job);
    this->SetGuildInfo(info->guildId, info->guildVersion);
    return;
  }

  if (message == AM_UPDATEGAGE) {
    if (g_session.IsCompanionAid(this->render.accountId)) {
      if (this->playerGaugeWindow == nullptr) {
        this->playerGaugeWindow = new UIPlayerGage();
        switch (this->objectType) {
          case NPC_HO_TYPE: {
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetFirstValues(
              [] { return static_cast<std::int64_t>(g_session.m_homunHp); },
              [] { return static_cast<std::int64_t>(g_session.m_homunMaxHp); }
            );
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetSecondValues(
              [] { return static_cast<std::int64_t>(g_session.m_homunSp); },
              [] { return static_cast<std::int64_t>(g_session.homunMaxSp); }
            );
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetThirdValues(
              [] { return static_cast<std::int64_t>(g_session.homunHunger); },
              [] { return static_cast<std::int64_t>(g_session.homunMaxHunger); }
            );
            this->playerGaugeWindow->SetSize(60, 13);
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->actorKind = 8;
            break;
          }
          case 7: {
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetThirdValues(
              [] { return static_cast<std::int64_t>(g_session.petHunger); },
              [] { return static_cast<std::int64_t>(g_session.m_petMaxHunger); }
            );
            this->playerGaugeWindow->SetSize(60, 5);
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->actorKind = 7;
            break;
          }
          case NPC_MERSOL_TYPE: {
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetFirstValues(
              [] { return static_cast<std::int64_t>(g_session.m_mercenaryHp); },
              [] { return static_cast<std::int64_t>(g_session.m_mercenaryMaxHp); }
            );
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetSecondValues(
              [] { return static_cast<std::int64_t>(g_session.m_mercenarySp); },
              [] { return static_cast<std::int64_t>(g_session.m_mercenaryMaxSp); }
            );
            this->playerGaugeWindow->SetSize(60, 9);
            break;
          }
          case NPC_ELEMENTAL_TYPE: {
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetFirstValues(
              [] { return static_cast<std::int64_t>(g_session.elementalHp); },
              [] { return static_cast<std::int64_t>(g_session.elementalMaxHp); }
            );
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetSecondValues(
              [] { return static_cast<std::int64_t>(g_session.elementalSp); },
              [] { return static_cast<std::int64_t>(g_session.elementalMaxSp); }
            );
            this->playerGaugeWindow->SetSize(60, 9);
            break;
          }
          case NPC_ABR_TYPE: {
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetFirstValues(
              [this] { return g_session.FindSummonHp(this->render.accountId); },
              [this] { return g_session.FindSummonMaxHp(this->render.accountId); }
            );
            this->playerGaugeWindow->SetSize(60, 5);
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->actorKind = 13;
            break;
          }
          case NPC_BIONIC_TYPE: {
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->SetFirstValues(
              [this] { return g_session.FindSummonHp(this->render.accountId); },
              [this] { return g_session.FindSummonMaxHp(this->render.accountId); }
            );
            this->playerGaugeWindow->SetSize(60, 5);
            static_cast<UIPlayerGage*>(this->playerGaugeWindow)->actorKind = 14;
            break;
          }
        }
        g_windowMgr.AddRenderedFront(this->playerGaugeWindow);
        if (this->playerGaugeWindow == nullptr) {
          return;
        }
      }
      this->playerGaugeWindow->Move(
        this->render.screenX - 30, this->render.screenY + g_renderer->viewport.height * 12 / 480
      );
      return;
    }

    if (this->objectType == NPC_ABR_TYPE || this->objectType == NPC_BIONIC_TYPE) {
      return;
    }

    if (this->pcGaugeWindow == nullptr) {
      this->pcGaugeWindow = new UIPcGage();
      this->pcGaugeWindow->SetSize(60, 5);
      g_windowMgr.AddRenderedFront(this->pcGaugeWindow);
      if (this->pcGaugeWindow == nullptr) {
        return;
      }
    }
    static_cast<UIPcGage*>(this->pcGaugeWindow)->SetValues(static_cast<int>(b), static_cast<int>(c));
    static_cast<UIPcGage*>(this->pcGaugeWindow)->SetLimits(0, 0);
    const int y = this->render.screenY + ScaleByScreenHeight(12);
    this->pcGaugeWindow->Move(this->render.screenX - 30, y);
    return;
  }
  CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA5610
// FUNCTION: fn_22427
void CNumEffect::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  if (message == 14) {
    const float* position = reinterpret_cast<const float*>(static_cast<std::intptr_t>(b));
    float* target = &this->render.position.x;
    target[0] = position[0];
    target[1] = position[1];
    target[2] = position[2];
    this->render.visible = 1;
    this->render.position.y -= 12.0f;
    this->baseHeight = this->render.position.y;

    const int motion = static_cast<int>(c);
    this->render.curMotion = motion;
    if (motion > 9 || motion < 0) {
      this->render.curMotion = 0;
    }

    this->render.spriteOffsetX = static_cast<int>(d);
    this->render.scale = 5.0f;
    this->baseScale = 5.0f;
    this->render.motionStartTime = timeGetTime();
    return;
  }

  if (message == 22) {
    return;
  }

  CRenderObject::SendMsg(sender, message, b, c, d, e, 0, 0);
}

// OFFSET: 0x00DA56E0
// FUNCTION: fn_22428
void CSoundMaker::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  if (message != 14) {
    CRenderObject::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }

  const WorldSoundRecord& sound = *reinterpret_cast<const WorldSoundRecord*>(static_cast<std::intptr_t>(b));
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
void CNpc::SetSprAct(int job, int value, int) {
  char name[64];
  this->SetActorJob(job);
  this->SetSexBit(value);
  this->render.bodyPalette = 0;
  if (g_session.IsGuildMemberCharacter(this->GetEffectiveJob(-1), name)) {
    this->render.bodyPalette = 1;
    this->render.name = name;
  }
}

// OFFSET: 0x00DA58A0
// FUNCTION: fn_22432
void CNpc::SetSprJob(int job) {
  char spriteName[128];
  char actionName[128];
  const char* spriteFormat;
  const char* actionFormat;

  this->render.spriteJob = 0;
  this->render.shadowScale = g_session.GetNpcLocationRadius(job, false);
  if (g_session.IsNpcJob(job) == true) {
    this->render.spriteJob = job;
    actionFormat = "NPC\\%s.act";
    spriteFormat = "NPC\\%s.spr";
  } else if (job >= 6017 && job <= 6046) {
    actionFormat = "인간족\\몸통\\%s.act";
    spriteFormat = "인간족\\몸통\\%s.spr";
  } else if (job >= 6001 && job <= 6052) {
    actionFormat = "homun\\%s.act";
    spriteFormat = "homun\\%s.spr";
  } else {
    this->render.spriteJob = job;
    actionFormat = "몬스터\\%s.act";
    spriteFormat = "몬스터\\%s.spr";
  }

  std::sprintf(spriteName, spriteFormat, g_session.GetJobName(job, -1));
  std::sprintf(actionName, actionFormat, g_session.GetJobName(job, -1));
  this->SetSpriteName(spriteName);
  this->SetActionName(actionName);
}

// OFFSET: 0x00DA31B0
// FUNCTION: fn_22400
bool CHalluNPC::OnProcess() {
  ++this->processCount;
  if (this->render.color >= static_cast<std::uint32_t>(this->fadeStep)) {
    if (this->processCount == 110) {
      static_cast<EffectActor*>(static_cast<CRenderObject*>(this))->LaunchEffect(EF_HALLUTIME, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (static_cast<unsigned>(this->processCount) > 105 && (this->processCount & 3) == 0) {
      this->render.color -= this->fadeStep;
    }
    return true;
  }

  return false;
}

// OFFSET: 0x00DA32B0
// FUNCTION: fn_22402
bool CHalluPC::OnProcess() {
  ++this->preMoveOn;
  if (this->render.color >= static_cast<std::uint32_t>(this->destCellZ)) {
    if (this->preMoveOn == 110) {
      static_cast<EffectActor*>(static_cast<CRenderObject*>(this))->LaunchEffect(EF_HALLUTIME, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (static_cast<unsigned>(this->preMoveOn) > 105 && (this->preMoveOn & 3) == 0) {
      this->render.color -= this->destCellZ;
    }
    return true;
  }

  return false;
}

// OFFSET: 0x00DA3010
// FUNCTION: fn_22398
bool CCorpse::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.motionStartTime) / 24.0);
  this->render.previousAnimation = this->Get8Dir(this->render.angle) + this->render.animation;
  this->render.curMotion = this->GetActRes()->FrameCount(this->render.previousAnimation) - 1;
  this->SetColor(static_cast<int>(255.0f - elapsed), -1, -1, -1);
  return !(elapsed >= 255.0f);
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
    if (this->render.visible) {
      if ((this->render.color & 0xFF000000) == 0xFF000000) {
        this->DrawShadow(argument, this->render.scale);
      }
      if (this->render.enabled) {
        this->RenderMercenary(matrix, this->GetEffectiveJob(-1));
      } else {
        this->UpdateTlvertex(matrix);
      }
    } else {
      this->UpdateTlvertex(matrix);
    }
    return;
  }

  if (this->render.visible) {
    if (this->render.state != 6 && this->render.state != 3) {
      this->DrawShadow(argument, this->render.scale);
    }
    if (this->render.enabled) {
      this->DrawSprite(argument, g_session.IsMonsterClass(this->GetEffectiveJob(-1)), 0.0f);
    } else {
      this->UpdateTlvertex(matrix);
    }
  } else {
    this->UpdateTlvertex(matrix);
  }
}

// OFFSET: 0x00DA44D0
// FUNCTION: fn_22416
void CNpc::Render(int argument) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  switch (this->GetEffectiveJob(-1)) {
    case JT_HIDDEN_NPC: {
      float x;
      float y;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&this->render.position.x, matrix, &x, &y, &reciprocalDepth);
      CRenderer* renderer = g_renderer;
      const float distance = renderer->ScaleDistance(reciprocalDepth);
      const float scale = distance / g_heightScale;
      this->render.distanceScale = scale;

      const float halfWidth = static_cast<float>(this->xSize / 2);
      const float halfHeight = static_cast<float>(this->ySize / 2);
      float bounds[5];
      bounds[0] = x - halfWidth * renderer->viewport.uiScaleX * scale;
      bounds[1] = y - halfHeight * renderer->viewport.uiScaleY * scale;
      bounds[2] = halfWidth * renderer->viewport.uiScaleX * scale + x;
      bounds[3] = y;
      bounds[4] = reciprocalDepth;
      this->SetRenderInfo(bounds, x, y);
      return;
    }
    case JT_WARPNPC: {
      if (!this->render.enabled) {
        break;
      }

      float x;
      float y;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&this->render.position.x, matrix, &x, &y, &reciprocalDepth);
      CRenderer* renderer = g_renderer;
      const float distance = renderer->ScaleDistance(reciprocalDepth);
      const float scale = distance / g_heightScale;
      this->render.distanceScale = scale;

      float bounds[5];
      bounds[0] = x - renderer->viewport.uiScaleX * 50.0f * scale;
      bounds[1] = y - renderer->viewport.uiScaleY * 50.0f * scale;
      bounds[2] = renderer->viewport.uiScaleX * 50.0f * scale + x;
      bounds[3] = renderer->viewport.uiScaleY * 50.0f * scale + y;
      bounds[4] = reciprocalDepth;
      this->SetRenderInfo(bounds, x, y);

      if (!g_serviceOptions.Get(TT_EFFECT_ON_OFF)) {
        this->DrawShadow(argument, this->render.scale * 3.0f);
      }
      return;
    }
    case JT_EFFECTLAUNCHER:
    case JT_HIDDEN_WARP_NPC:
      return;
    default:
      if (this->GetEffectiveJob(-1) < 6017 || this->GetEffectiveJob(-1) > 6046) {
        for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
          if (effect.id == 184) {
            if (this->objectType == NPC_MOB_TYPE && (!this->render.visible || !this->render.enabled)) {
              this->SetColor(-1, 0, 0, 0);
              this->DrawSprite(argument, g_session.IsMonsterClass(this->GetEffectiveJob(-1)), 0.0f);
              return;
            }
            break;
          }
        }

        if (!this->render.visible) {
          return;
        }
        if ((this->render.color & 0xFF000000) == 0xFF000000) {
          this->DrawShadow(argument, this->render.scale);
        }
        if (this->render.enabled) {
          this->DrawSprite(argument, g_session.IsMonsterClass(this->GetEffectiveJob(-1)), 0.0f);
          return;
        }
      } else {
        if (!this->render.visible) {
          return;
        }
        if ((this->render.color & 0xFF000000) == 0xFF000000) {
          this->DrawShadow(argument, this->render.scale);
        }
        if (this->render.enabled) {
          this->RenderMercenary(matrix, this->GetEffectiveJob(-1));
          return;
        }
      }
      break;
  }
  this->UpdateTlvertex(matrix);
}

// OFFSET: 0x00DA2F00
// FUNCTION: fn_22396
bool CBlowEffect::OnProcess() {
  const float frame = static_cast<float>((timeGetTime() - this->render.motionStartTime) / 24.0);
  this->render.previousAnimation = this->render.animation;
  const int frameCount = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation));
  this->render.curMotion = static_cast<int>(frame / this->render.speed) % frameCount;
  this->PlayFrameSounds();
  if (frame / this->render.speed / static_cast<float>(frameCount) >= 1.0f) {
    return false;
  }
  return true;
}

// OFFSET: 0x00DA3DF0
// FUNCTION: fn_22407
void CNpc::UpdateBubblePositions() {
  if (this->playerGaugeWindow) {
    this->playerGaugeWindow->Move(
      this->render.screenX - 30,
      static_cast<int>(
        static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.distanceScale + static_cast<float>(this->render.screenY)
      )
    );
    this->playerGaugeWindow->Invalidate();
  }

  if (this->pcGaugeWindow) {
    this->pcGaugeWindow->Move(
      this->render.screenX - 30,
      static_cast<int>(
        static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.distanceScale + static_cast<float>(this->render.screenY)
      )
    );
    this->pcGaugeWindow->Invalidate();
  }

  this->UpdateHpGauge();
}

// OFFSET: 0x00DA1470
// FUNCTION: fn_22357
CHalluPC2::CHalluPC2(CPc* source) {
  this->sourcePc = source;
  this->render.visible = 1;
  this->render.visibilityOptions[0] = 0;
  this->render.motionStartTime = timeGetTime();
  this->SetSexBit(source->sex);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->SetShapeShift(source->GetShapeShift());
  this->SetRiding(source->isRiding);
  this->SetMonsterTransformJob(source->GetMonsterTransformJob());
  this->render.position = source->render.position;
  this->render.distanceScale = source->render.distanceScale;
  this->render.spriteOffsetX = source->render.spriteOffsetX;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(source->GetActName());
  this->render.flatDepth = source->render.flatDepth;
  this->render.depthLayer = source->render.depthLayer;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.spriteAngle = source->render.spriteAngle;
  this->render.bodyPalette = 0;
  this->headPalette = source->headPalette;
  this->stateFlags = source->GetStateFlags();
  this->headPaletteName = std::string(source->headPaletteName);
  this->LoadEquipSprites(source->GetEquipSpriteNames());
  this->LoadEquipActions(source->GetEquipActionNames());
  this->imfName = source->imfName;
  this->guildId = source->GetGdid();
  this->render.accountId = source->render.accountId;
  this->motionY = 0.0f;
  this->preMoveOn = 0;
  this->destCellZ = 0x10000000;
  this->render.bodyPalette = source->render.bodyPalette;
  this->render.name = source->render.name;
  this->robe = 0;
  this->accessory = 0;
  this->accessory2 = 0;
  this->accessory3 = 0;
}

// OFFSET: 0x00DA0E60
// FUNCTION: fn_22353
CBlurPC::CBlurPC(CPc* source) {
  this->blurSource = source;
  this->render.visible = 1;
  this->render.visibilityOptions[0] = 0;
  this->render.motionStartTime = timeGetTime();
  this->SetSexBit(source->sex);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->SetShapeShift(source->GetShapeShift());
  this->SetRiding(source->isRiding);
  this->SetMonsterTransformJob(source->GetMonsterTransformJob());
  this->render.position = source->render.position;
  this->render.distanceScale = source->render.distanceScale;
  this->render.spriteOffsetX = source->render.spriteOffsetX;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(source->GetActName());
  this->render.flatDepth = source->render.flatDepth;
  this->render.depthLayer = source->render.depthLayer;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.spriteAngle = source->render.spriteAngle;
  this->render.bodyPalette = 0;
  this->headPalette = source->headPalette;
  this->stateFlags = source->GetStateFlags();
  this->headPaletteName = std::string(source->headPaletteName);
  this->LoadEquipSprites(source->GetEquipSpriteNames());
  this->LoadEquipActions(source->GetEquipActionNames());
  this->imfName = source->imfName;
  this->guildId = source->GetGdid();
  this->render.accountId = source->render.accountId;
  this->motionY = 0.0f;
  this->destCellZ = 0x10000000;
  this->render.bodyPalette = source->render.bodyPalette;
  this->render.name = source->render.name;
  this->robe = 0;
  this->accessory = 0;
  this->accessory2 = 0;
  this->accessory3 = 0;
}

// OFFSET: 0x00DA17A0
// FUNCTION: fn_22358
CHalluPC::CHalluPC(CPc* source) {
  this->sourcePc = source;
  this->render.visible = 1;
  this->render.visibilityOptions[0] = 0;
  this->render.motionStartTime = timeGetTime();
  this->SetSexBit(source->sex);
  this->SetActorJob(source->GetEffectiveJob(-1));
  this->SetShapeShift(source->GetShapeShift());
  this->SetRiding(source->isRiding);
  this->SetMonsterTransformJob(source->GetMonsterTransformJob());
  this->render.position = source->render.position;
  this->render.distanceScale = source->render.distanceScale;
  this->render.spriteOffsetX = source->render.spriteOffsetX;
  this->render.scale = source->render.scale;
  this->render.animation = source->render.animation;
  this->render.previousAnimation = source->render.previousAnimation;
  this->render.curMotion = source->render.curMotion;
  this->SetActionName(source->GetActName());
  this->render.flatDepth = source->render.flatDepth;
  this->render.depthLayer = source->render.depthLayer;
  this->render.isLieOnGround = source->render.isLieOnGround;
  this->render.isSingleColor = source->render.isSingleColor;
  this->render.spriteAngle = source->render.spriteAngle;
  this->render.bodyPalette = 0;
  this->headPalette = source->headPalette;
  this->stateFlags = source->GetStateFlags();
  this->headPaletteName = std::string(source->headPaletteName);
  this->LoadEquipSprites(source->GetEquipSpriteNames());
  this->LoadEquipActions(source->GetEquipActionNames());
  this->imfName = source->imfName;
  this->guildId = source->GetGdid();
  this->render.accountId = source->render.accountId;
  this->motionY = 0.0f;
  this->preMoveOn = 0;
  this->destCellZ = 0x10000000;
  this->render.bodyPalette = source->render.bodyPalette;
  this->render.name = source->render.name;
  this->modelReady = true;
  this->SetBodyCostume(source->GetBodyCostume(), 1);
  this->robe = 0;
  this->accessory = 0;
  this->accessory2 = 0;
  this->accessory3 = 0;
}

// OFFSET: 0x00DA28B0
// FUNCTION: fn_22377
void CNpc::UpdateMinimapMarker() {
  if (g_session.IsAllyActor(this->render.accountId, this->GetEffectiveJob(-1)) != 1) {
    return;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  MinimapMarker* marker = mode->FindMinimapMarker(this->render.accountId);
  if (marker && marker->effectType != 9999) {
    if (this->attachedEffect && marker->effectType == this->minimapMarkerCell[0] && marker->questIconId == this->minimapMarkerCell[1]) {
      return;
    }
    if (this->attachedEffect) {
      this->attachedEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->attachedEffect = nullptr;
      this->attachedEffectId = 9999;
    }

    CMsgEffect* effect = mode->SpawnMsgEffect((6 > marker->effectType ? 0x24 : 0x36) + marker->effectType, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->attachedEffect = effect;
    this->minimapMarkerCell[0] = marker->effectType;
    this->minimapMarkerCell[1] = marker->questIconId;
    return;
  }

  if (this->attachedEffect) {
    this->attachedEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
    this->attachedEffect = nullptr;
    this->attachedEffectId = 9999;
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
    case JT_WARPNPC:
    case JT_HIDDEN_WARP_NPC:
      break;
    case JT_HIDDEN_NPC:
      this->ProcessDamageMessages();
      this->UpdateGaugePositions();
      result = this->ProcessState();
      break;
    case JT_EFFECTLAUNCHER:
      if (timeGetTime() > this->effectTick + this->xSize * 1000) {
        this->effectTick = timeGetTime();
        this->LaunchEffect(this->moveSpeed, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    default:
      this->ProcessBodyState();
      this->UpdateStateEffects();
      this->ProcessDamageMessages();
      if (this->render.color != 0xFFFFFFFF) {
        this->ModifyArgb(8, 8, 8, 8);
      }
      this->UpdateGaugePositions();
      result = this->ProcessState();
      break;
  }

  if (this->objectType != 7) {
    this->PlayFrameSounds();
  }

  WIN32_FIND_DATAA findData;
  if (g_session.IsHomunAid(this->render.accountId)) {
    if (this->homunAI) {
      if (this->homunAI->state) {
        this->homunAI->Process(this->render.accountId);
      } else {
        if (g_serviceOptions.Get(TT_HOMUN_USERAI_ON_OFF)) {
          HANDLE find = FindFirstFileA(".\\AI\\USER_AI", &findData);
          if (find == INVALID_HANDLE_VALUE) {
            CreateDirectoryA(".\\AI\\USER_AI", nullptr);
          }
          FindClose(find);
          CopyFileA(".\\AI\\AI.lua", ".\\AI\\USER_AI\\AI.lua", TRUE);
          this->homunAI->Load(".\\AI\\USER_AI\\AI.lua");
        } else {
          this->homunAI->Load(".\\AI\\AI.lua");
        }
        this->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
      }
    } else {
      this->homunAI = new CMercenaryAI(0);
    }
  }

  if (g_session.IsMercenaryAid(this->render.accountId)) {
    if (this->merAI) {
      if (this->merAI->state) {
        this->merAI->Process(this->render.accountId);
      } else {
        const char* path;
        if (g_serviceOptions.Get(TT_MER_USERAI_ON_OFF)) {
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
        this->merAI->Load(path);
        this->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
      }
    } else {
      this->merAI = new CMercenaryAI(1);
    }
  }

  if (g_session.IsPetAid(this->render.accountId)) {
    this->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
  }

  this->UpdateBubblePositions();

  if (g_session.homunAlive && g_session.homunEnemyGid == this->render.accountId) {
    this->SendMsg(nullptr, 0x86, 0, 0, 0, 0, 0, 0);
  } else if (g_session.mercenaryAlive && g_session.mercenaryEnemyGid == this->render.accountId) {
    this->SendMsg(nullptr, 0x86, 0, 0, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 0x87, 0, 0, 0, 0, 0, 0);
  }

  this->UpdateMinimapMarker();

  switch (this->objectType) {
    case 7:
    case NPC_HO_TYPE:
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      this->SetColor(
        g_serviceOptions.Get(TT_HIDE_SUMMON_ON_OFF) ? 0x3A : static_cast<int>(this->render.color >> 24), static_cast<int>((this->render.color >> 16) & 0xFF),
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

  if (this->render.visible) {
    if (g_serviceType == 4 && IsAdminAccount(this->render.accountId) && this->render.accountId != g_session.m_aid) {
      return;
    }

    if (this->render.state != 6 && this->render.state != 3) {
      this->DrawShadow(argument, this->render.scale);
    }
    if (this->render.enabled) {
      if (IsObfuscatedMessage(this->render.accountId)) {
        this->RenderOverlaySprite(matrix, 1);
        return;
      }

      if (this->renderWithoutLayer != 0) {
        this->DrawSprite(argument, false, 0.0f);
        return;
      }

      this->DrawLayers(matrix, 1, 2);
    } else {
      this->UpdateTlvertex(matrix);
    }
  } else {
    this->UpdateTlvertex(matrix);
  }
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

  if (this->render.visible) {
    if (g_serviceType == 4 && IsAdminAccount(this->render.accountId) && this->render.accountId != g_session.m_aid) {
      return;
    }

    if (this->render.state != 6 && this->render.state != 3) {
      this->DrawShadow(argument, this->render.scale);
    }
    if (this->render.enabled) {
      if (IsObfuscatedMessage(this->render.accountId)) {
        this->RenderOverlaySprite(matrix, 1);
        return;
      }

      if (this->renderWithoutLayer != 0) {
        this->DrawSprite(argument, false, 0.0f);
        return;
      }

      this->DrawLayers(matrix, 1, 0);
    } else {
      this->UpdateTlvertex(matrix);
    }
  } else {
    this->UpdateTlvertex(matrix);
  }
}



// OFFSET: 0x00DA59B0
// FUNCTION: fn_22433
void CNpc::SetState(int state, int) {
  if (state == -1) {
    return;
  }

  switch (this->GetEffectiveJob(-1)) {
    case JT_HIDDEN_NPC:
      this->xSize *= 2;
      this->ySize *= 2;
      break;
    case JT_EFFECTLAUNCHER:
      this->render.position.y -= static_cast<float>(this->ySize * 5);
      break;
    case JT_WARPNPC: {
      int height = this->ySize;
      if (height <= 10) {
        this->ySize = 10;
        height = 10;
      }
      int width = this->xSize;
      if (width <= 10) {
        this->xSize = 10;
        width = 10;
      }
      this->boundsLeft = this->render.position.x - static_cast<float>(width / 2);
      this->boundsRight = static_cast<float>(width / 2) + this->render.position.x;
      this->boundsTop = this->render.position.z - static_cast<float>(height / 2);
      this->boundsBottom = static_cast<float>(height / 2) + this->render.position.z;
      if (this->appearanceInitialized == 0 && this->render.enabled) {
        this->appearanceInitialized = 1;
        this->LaunchEffect(EF_WARPZONE2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case JT_HIDDEN_WARP_NPC:
      break;
    default: {
      CGameActor* actor = static_cast<CGameActor*>(g_session.FindGameActor(this->render.accountId));
      bool isDisguised = false;
      if (actor && actor->objectType == 12) {
        isDisguised = true;
      }
      if (g_session.IsAllyActor(this->render.accountId, this->GetEffectiveJob(-1)) == 1 && this->GetEffectiveJob(-1) != JT_MOB_TOMB &&
          !isDisguised) {
        this->SetAction(0, 4, 0);
        this->RegisterPos();
        break;
      }

      if (this->render.state == 4) {
        this->render.position.x = this->damageDestX;
        this->render.position.z = this->damageDestZ;
      }

      switch (state) {
        case ST_STAND:
          this->render.isLieOnGround = false;
          this->render.visibilityOptions[1] = 1;
          this->SetAction(0, 4, 0);
          if (this->healthState != 0) {
            int hungerLevel = g_session.GetPetHungryState(g_session.petHunger);
            int emotion = state;
            switch (this->healthState) {
              case 1:
                emotion = g_session.GetPetEmotionNumber(this->GetEffectiveJob(-1), 7, hungerLevel);
                this->SetAction(40, 4, 1);
                break;
              case 2:
                emotion = g_session.GetPetEmotionNumber(this->GetEffectiveJob(-1), 8, hungerLevel);
                this->SetAction(48, 4, 1);
                break;
              case 3:
                emotion = g_session.GetPetEmotionNumber(this->GetEffectiveJob(-1), 9, hungerLevel);
                this->SetAction(56, 4, 1);
                break;
              case 4:
                emotion = g_session.GetPetEmotionNumber(this->GetEffectiveJob(-1), 5, hungerLevel);
                this->SetAction(64, 4, 1);
                break;
              default:
                this->SetAction(40, 4, 1);
                break;
            }
            if (this->render.accountId == g_session.petGid) {
              g_modeMgr.GetGameMode()->SendMsg(MM_PET_ACT, emotion, 0, 0, 0);
            }
            this->healthState = 0;
            this->render.isMotionFinished = false;
            this->RegisterPos();
          } else {
            if (this->render.state != 0) {
              this->petEmotionStartTick = timeGetTime();
            }
            if (g_session.moveHomunEnemy) {
              g_session.homunEnemyX = 0;
              g_session.homunEnemyY = 0;
              g_session.moveHomunEnemy = 0;
            }
            if (g_session.moveMercenaryEnemy) {
              g_session.mercenaryEnemyX = 0;
              g_session.mercenaryEnemyY = 0;
              g_session.moveMercenaryEnemy = 0;
            }
            this->RegisterPos();
          }
          break;
        case ST_MOVE:
          if (this->bodyState != 1 && this->bodyState != 2) {
            this->render.isMotionFreezed = false;
          }
          if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
            this->SetAction(8, 6, 0);
          } else {
            this->SetAction(8, 4, 0);
          }
          this->UnRegisterPos();
          break;
        case ST_SKILL:
          switch (this->objectType) {
            case NPC_HO_TYPE:
            case NPC_ELEMENTAL_TYPE:
            case NPC_ABR_TYPE:
            case NPC_BIONIC_TYPE:
              this->SetAction(40, 4, 1);
              break;
            case NPC_MERSOL_TYPE:
              this->SetAction(16, 4, 1);
              break;
          }
          if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
            this->SetAction(96, 6, 1);
          }
          break;
        case ST_ATTACK:
          if (this->bodyState != 1 && this->bodyState != 2) {
            this->render.isMotionFreezed = false;
          }
          if (this->objectType == NPC_HO_TYPE) {
            if (this->GetEffectiveJob(-1) == JT_MER_ELEANOR) {
              this->SetAction(16, 4, 1);
            } else if (std::rand() % 2) {
              this->SetAction(16, 4, 1);
            } else {
              this->SetAction(48, 4, 1);
            }
          } else if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
            this->SetAction(80, 3, 1);
          } else {
            this->SetAction(16, 4, 1);
          }
          this->attackMotion = static_cast<float>(this->GetAttackMotion());
          this->RegisterPos();
          this->LaunchAttackProjectile(this->hitTargetAid, this->render.speed * this->attackMotion);
          break;
        case ST_DAMAGE:
          if (this->render.frameSequenceActive != 0 || this->render.animationSequenceActive != 0 || this->render.timedSequenceActive != 0) {
            return;
          }
          if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
            this->SetAction(48, 5, 1);
            break;
          }
          if (this->render.previousAnimation < 40) {
            this->SetAction(24, 4, 1);
            this->RegisterPos();
            break;
          }
          this->ProcessMotion();
          return;
        case ST_DEAD:
          this->render.frameSequenceActive = 0;
          this->render.animationSequenceActive = 0;
          this->render.timedSequenceActive = 0;
          this->render.isMotionFreezed = false;
          if (this->bodyState == 2) {
            this->SendMsg(nullptr, AM_DELETE_ICEEFFECT, 0, 0, 0, 0, 0, 0);
            this->SendMsg(nullptr, AM_MAKE_ICEBREAKE_FFECT, 0, 0, 0, 0, 0, 0);
          }
          this->render.isLieOnGround = true;
          this->render.isMotionFreezed = false;
          this->render.visibilityOptions[1] = 0;
          if (this->GetEffectiveJob(-1) >= 6017 && this->GetEffectiveJob(-1) <= 6046) {
            this->SetAction(64, 4, 1);
            this->UnRegisterPos();
          } else {
            this->SetAction(32, 8, 1);
            this->UnRegisterPos();
          }
          break;
        case ST_SLEEP:
          if (this->bodyState == 2) {
            this->SendMsg(nullptr, AM_DELETE_ICEEFFECT, 0, 0, 0, 0, 0, 0);
            this->SendMsg(nullptr, AM_MAKE_ICEBREAKE_FFECT, 0, 0, 0, 0, 0, 0);
          }
          this->render.isLieOnGround = false;
          this->render.isMotionFinished = true;
          this->render.isMotionFreezed = true;
          this->SetAction(0, 8, 1);
          break;
      }
      break;
    }
  }

  this->render.previousAnimation = this->render.animation + this->Get8Dir(this->render.angle);
  this->render.state = state;
  this->render.motionStartTime = timeGetTime();
}
