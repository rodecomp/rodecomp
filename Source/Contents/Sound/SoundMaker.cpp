#include "Contents/Sound/SoundMaker.h"

#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/EffectActors.h"
#include "Contents/Actors/GndRiser.h"
#include "Contents/Actors/Npc.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sound/SoundMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/World/World.h"
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

// OFFSET: 0x00DA1190
// FUNCTION: fn_22354
CCorpse::CCorpse() {
  this->render.visible = 1;
  this->render.isLieOnGround = true;
  this->spriteOffset = 0.0f;
  this->render.unknown_008C = timeGetTime();
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
