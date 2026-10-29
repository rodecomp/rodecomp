#include "Contents/Sound/SoundMaker.h"

#include "Contents/Actors/EffectActors.h"
#include "Contents/Actors/GndRiser.h"
#include "Contents/Actors/Npc.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sound/SoundMgr.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/World/WorldScene.h"
#include "UI/Window/UIWindowMgr.h"
#include "Resource/World/WorldResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <cmath>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00DA1E30
// FUNCTION: fn_22363
CSoundMaker::CSoundMaker(ResourceMgr& resources, JobNames& names) : CRenderObject(resources, names) {
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
bool CHalluPlant::Update() {
  return ++this->unknown_0440 <= 60;
}

// OFFSET: 0x00DA3980
// FUNCTION: fn_22405
bool CNumEffect::Update() {
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
bool CSoundMaker::Update() {
  if (this->width == 0) {
    return true;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_world.ActiveGameMode())->scene->player);
  if (!player) {
    return true;
  }

  const float distance = this->HorizontalDistance(player->render.position.x, player->render.position.z);
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
  if (player->HorizontalDistance(x, z) - this->range > 75.0f || this->file.empty()) {
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

// OFFSET: 0x00DA2B00
// FUNCTION: fn_22379
float* CPc::GetExtent(float* extent) const {
  extent[0] = this->unknown_04EC - this->unknown_04E4;
  extent[1] = this->unknown_04F0 - this->unknown_04E8;
  return extent;
}

// OFFSET: 0x00DA4150
// FUNCTION: fn_22411
void CGndRiser::Draw(int) {
}

// OFFSET: 0x00DA4960
// FUNCTION: fn_22417
void CSoundMaker::Draw(int) {
}

// OFFSET: 0x00DA4970
// FUNCTION: fn_22418
void CBlowEffect::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message != 14 || a != 0) {
    CRenderObject::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
void CBlurPC::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CPc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
      CPc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4AD0
// FUNCTION: fn_22420
void CCorpse::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  CRenderObject::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x00DA4BF0
// FUNCTION: fn_22422
void CHalluNPC::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CNpc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
      CNpc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4CC0
// FUNCTION: fn_22423
void CHalluPC2::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CPc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
      CPc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4D90
// FUNCTION: fn_22424
void CHalluPC::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CPc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
      CPc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA4E60
// FUNCTION: fn_22425
void CHalluPlant::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (a != 0) {
    CNpc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
      CNpc::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00DA5610
// FUNCTION: fn_22427
void CNumEffect::SendObjectMessage(
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

  CRenderObject::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x00DA56E0
// FUNCTION: fn_22428
void CSoundMaker::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message != 14 || a != 0) {
    CRenderObject::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
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
