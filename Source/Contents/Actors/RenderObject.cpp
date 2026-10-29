#include "Contents/Actors/RenderObject.h"

#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Client/Replay.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/EffectActors.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/GuildMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Render/SpriteQuad.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CA8730
// FUNCTION: fn_20991
CRenderObject::CRenderObject() {
}

// OFFSET: 0x00CA8F90
// FUNCTION: fn_20998
CRenderObject::~CRenderObject() = default;

// OFFSET: 0x00CAFDC0
// FUNCTION: fn_21089
bool CRenderObject::Update() {
  return false;
}

// OFFSET: 0x00CBF920
// FUNCTION: fn_21115
void CRenderObject::SendObjectMessage(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x00CBF930
// FUNCTION: fn_21116
void CRenderObject::SetActionName(ResourceMgr& resources, const char* name) {
  if (this->render.action) {
    this->render.action->Unlock();
  }

  this->render.action = static_cast<CActRes*>(resources.Load(name));

  if (this->render.action) {
    this->render.action->Lock();
  }
}

// OFFSET: 0x00CC2D40
// FUNCTION: fn_21144
void CRenderObject::SetScreenBounds(const float*, float, float) {
}

// OFFSET: 0x00CC2DA0
// FUNCTION: fn_21146
void CGameActor::SetUnknown0260(int value) {
  this->unknown_0260 = value & 1;
}

// OFFSET: 0x00CC2E00
// FUNCTION: fn_21151
void CRenderObject::SetSpriteName(ResourceMgr& resources, const char* name) {
  if (this->render.sprite) {
    this->render.sprite->Unlock();
  }

  this->render.sprite = static_cast<CSprRes*>(resources.Load(name));

  if (this->render.sprite) {
    this->render.sprite->Lock();
  }
}

// OFFSET: 0x00CC3DC0
// FUNCTION: fn_21154
void CRenderObject::SetScreenPosition(float x, float y) {
  this->render.screenX = TruncateToInt(x);
  this->render.screenY = TruncateToInt(y);
}

// OFFSET: 0x00CBFAE0
// FUNCTION: fn_21118
void CRenderObject::SetAnimation(unsigned animation, int, int repeat) {
  float speed = this->render.action->Speed(animation);
  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }
  if (speed > 0.0f) {
    this->render.speed = speed;
  }
  if (this->render.speed < 1.0f) {
    if (g_replayAnimationScaling != 0) {
      speed = Replay::Instance().AnimationScale();
      if (speed > 0.0f) {
        this->render.speed = speed;
      }
    } else {
      this->render.speed = 1.0f;
    }
  }
  speed = this->render.speed * this->render.unknown_0064;
  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }
  if (speed > 0.0f) {
    this->render.speed = speed;
  }

  this->render.animation = animation;
  this->render.previousAnimation = animation;
  this->render.curMotion = 0;
  this->render.unknown_006C = repeat;
  this->render.isMotionFinished = 0;
  this->render.unknown_0060 = 1.0f;
  this->render.unknown_0064 = 1.0f;
  this->render.unknown_00C8 = 0;
  this->render.unknown_00D8 = 0;
  this->render.unknown_00DC = 0;
}

// OFFSET: 0x00CBFC00
// FUNCTION: fn_21119
void CRenderObject::SetColor(int alpha, int red, int green, int blue) {
  if (alpha != -1) {
    this->render.color = (this->render.color & 0x00FFFFFFu) | (static_cast<std::uint32_t>(alpha) << 24);
  }
  if (red != -1) {
    this->render.color = (this->render.color & 0xFF00FFFFu) | ((static_cast<std::uint32_t>(red) & 0xFFu) << 16);
  }
  if (green != -1) {
    this->render.color = (this->render.color & 0xFFFF00FFu) | ((static_cast<std::uint32_t>(green) & 0xFFu) << 8);
  }
  if (blue != -1) {
    this->render.color = (this->render.color & 0xFFFFFF00u) | static_cast<unsigned char>(blue);
  }
}

// OFFSET: 0x00CC1C20
// FUNCTION: fn_21125
void CGameActor::SetUnknown0305(unsigned char value, int extra) {
  this->unknown_0305 = value;
  this->unknown_0308 = extra;
}

// OFFSET: 0x00CC1D20
// FUNCTION: fn_21127
void CGameActor::RaiseUnknown03C8(int value) {
  if (value > 0 && this->unknown_03C8 < value) {
    this->unknown_03C8 = value;
  }
}

// OFFSET: 0x00CC1E70
// FUNCTION: fn_21130
void CRenderObject::SetSequenceDeadline(std::uint32_t duration) {
  this->render.sequenceDeadline = timeGetTime() + duration;
}

// OFFSET: 0x00CC1E90
// FUNCTION: fn_21131
void CRenderObject::SetSequenceInterval(unsigned char interval) {
  this->render.unknown_00C2 = interval;
}

// OFFSET: 0x00CC1EA0
// FUNCTION: fn_21132
void CRenderObject::SetAnimationSequence(
  unsigned char animation0, unsigned char frame0, unsigned char animation1, unsigned char frame1,
  unsigned char animation2, unsigned char frame2, unsigned char animation3, unsigned char frame3,
  unsigned char animation4, unsigned char frame4, int timed, std::uint32_t duration, int loop
) {
  this->render.sequenceAnimations[0] = animation0;
  this->render.sequenceFrames[0] = frame0;
  this->render.sequenceAnimations[1] = animation1;
  this->render.sequenceFrames[1] = frame1;
  this->render.sequenceAnimations[2] = animation2;
  this->render.sequenceFrames[2] = frame2;
  this->render.sequenceAnimations[3] = animation3;
  this->render.sequenceFrames[3] = frame3;
  this->render.sequenceAnimations[4] = animation4;
  this->render.sequenceFrames[4] = frame4;
  this->render.sequenceAnimation = animation0;
  this->render.sequenceFrame = frame0;
  this->render.sequenceIndex = 0;
  this->render.sequenceLast = 4;
  this->render.sequenceLoop = loop;
  this->render.sequenceTimed = timed;
  this->render.sequenceFinished = 0;
  this->render.unknown_00D8 = 1;
  this->render.unknown_00C8 = 0;
  this->render.unknown_00DC = 0;
  this->render.sequenceDeadline = timed != 0 ? timeGetTime() + duration : 0;
  this->render.sequenceTick = 1;
}

// OFFSET: 0x00CC1FA0
// FUNCTION: fn_21133
void CRenderObject::SetFrameSequence(
  unsigned char animation, unsigned char firstFrame, unsigned char lastFrame, unsigned char finishAnimation,
  unsigned char finishFrame, int timed, std::uint32_t duration, int loop
) {
  this->render.sequenceAnimation = animation;
  this->render.sequenceFrame = firstFrame;
  this->render.sequenceIndex = firstFrame;
  this->render.sequenceLast = lastFrame;
  this->render.sequenceFinishAnimation = finishAnimation;
  this->render.sequenceFinishFrame = finishFrame;
  this->render.sequenceLoop = loop;
  this->render.sequenceTimed = timed;
  this->render.sequenceFinished = 0;
  this->render.unknown_00C8 = 1;
  this->render.unknown_00DC = 0;
  this->render.unknown_00D8 = 0;
  this->render.sequenceDeadline = timed != 0 ? timeGetTime() + duration : 0;
  this->render.sequenceTick = 1;
}

// OFFSET: 0x00CC2050
// FUNCTION: fn_21134
void CGameActor::SetUnknown02E4(int value) {
  this->unknown_02E4 = value;
}

// OFFSET: 0x00CC2780
// FUNCTION: fn_21140
void CRenderObject::SetAnimationSpeed(float speed) {
  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }
  if (speed > 0.0f) {
    this->render.speed = speed;
  }
}


// OFFSET: 0x00CC2810
// FUNCTION: fn_21142
void CGameActor::SetUnknown02C8(int value) {
  this->unknown_02C8 = value;
}

// OFFSET: 0x00CBF970
// FUNCTION: fn_21117
void CGameActor::SetAnimation(unsigned animation, int motion, int repeat) {
  if (this->unknown_0314 == 7) {
    if (!this->render.action || static_cast<int>(this->render.action->animations.size()) <= static_cast<int>(animation)) {
      return;
    }
  }

  CRenderObject::SetAnimation(animation, motion, repeat);
  this->unknown_0238 = 0;
  this->unknown_0378 = -1.0f;
}

// OFFSET: 0x00CC15F0
// FUNCTION: fn_21121
int CRenderObject::HoverHeight() {
  this->render.unknown_00F8 += 2;
  if (this->render.unknown_00F8 >= 360) {
    this->render.unknown_00F8 -= 360;
  }

  const short angle = this->render.unknown_00F8;
  const int state = this->render.unknown_0070;
  if (state == 0 || state == 1 || (this->render.flags & 0x40000) != 0) {
    if (this->render.unknown_00FA < 20) {
      if (angle % 30 == 0) {
        ++this->render.unknown_00FA;
      }
    } else if (this->render.unknown_00FA > 20 && angle % 30 == 0) {
      --this->render.unknown_00FA;
    }
  } else if (state == 6) {
    if (this->render.unknown_00FA < 100) {
      if (angle % 30 == 0) {
        ++this->render.unknown_00FA;
      }
    } else if (this->render.unknown_00FA > 100 && angle % 30 == 0) {
      --this->render.unknown_00FA;
    }
  } else if (this->render.unknown_00FA != 0) {
    --this->render.unknown_00FA;
  }

  const short height = static_cast<short>(ClientSine(angle) * 4.0f + this->render.unknown_00FA);
  CGameMode* mode = static_cast<CGameMode*>(g_world.GetGameMode());
  const short scaled = static_cast<short>(400.0f / mode->camera->camera.desiredDistance * height);
  return scaled < 0 ? 0 : scaled;
}

// OFFSET: 0x00CC1740
// FUNCTION: fn_21122
void CRenderObject::DrawStatusOverlays(SpriteQuad quad, const std::uint32_t* palette) {
  if ((this->render.flags & 0x100) != 0) {
    const short level = this->render.unknown_00F4;
    bool draw = true;
    if (level <= 5) {
      quad.color = (static_cast<short>(level * 50) << 24) + 0xFFFFFF;
    } else if (level <= 8) {
      quad.color = 0xFFFFFFFF;
    } else if (level <= 11) {
      quad.color = (static_cast<short>(895 - level * 80) << 24) + 0xFFFFFF;
    } else {
      draw = false;
    }

    if (draw) {
      this->DrawQuad(quad, palette, 5, 0.0f, 0.0f, 0);
    }
  }

  if ((this->render.flags & 0x4000) != 0) {
    this->DrawTintedQuad(quad, 5, 5, 255, palette);
  }

  if ((this->render.flags & 0x40000) != 0) {
    this->DrawTintedQuad(quad, 255, 5, 5, palette);
  }
}

// OFFSET: 0x00CC1810
// FUNCTION: fn_21123
void CRenderObject::ScaleByPulse(float& scale) const {
  if ((this->render.flags & 0x10) != 0) {
    const unsigned index = std::max(this->render.pulsePhases[0] * 4u + 90u, 90u);
    if (index < 270) {
      scale = ((ClientSine(index) + 1.0f) * 0.25f + 0.5f) * scale;
    } else {
      scale = scale * 0.5f;
    }
  }

  if ((this->render.flags & 0x400) != 0) {
    const unsigned index = std::max(this->render.pulsePhases[1] * 4u + 90u, 90u);
    if (index < 270) {
      scale = (1.5f - (ClientSine(index) + 1.0f) * 0.25f) * scale;
    } else {
      scale = scale * 1.5f;
    }
  }
}

// OFFSET: 0x00CC18F0
// FUNCTION: fn_21124
void CRenderObject::ApplySwing(int mode, int& x, int& y, int& outX, int& outY) const {
  if ((this->render.flags & 0x40) == 0) {
    return;
  }

  const int swing = static_cast<int>(180.0f - ClientSine(this->render.unknown_00F4 * 5 + 90) * 180.0f);
  const int angle = swing >= 360 ? swing - 360 : swing;
  if (mode == 1) {
    x = static_cast<int>(ClientSine(angle) * 40.0f + x);
    y = static_cast<int>(y - (ClientCosine(angle) * 40.0f - 40.0f));
    outX = x;
    outY = y;
  } else if (mode >= 2) {
    x = static_cast<int>(ClientSine(angle) * 10.0f + x);
    y = static_cast<int>(y - (ClientCosine(angle) * 10.0f - 10.0f));
  }
}

// OFFSET: 0x00CC1C40
// FUNCTION: fn_21126
void CRenderObject::DrawTintedQuad(SpriteQuad& quad, short red, short green, short blue, const std::uint32_t* palette) {
  const unsigned char phase = this->render.unknown_00F6;
  if (phase <= 10) {
    quad.color = (((((phase * 15) << 8) + red) << 8) + green << 8) + blue;
  } else if (phase <= 20) {
    quad.color = ((((0xA0 << 8) + red) << 8) + green << 8) + blue;
  } else if (phase <= 50) {
    const short alpha = static_cast<short>((51 - phase) * 5);
    quad.color = ((((alpha << 8) + red) << 8) + green << 8) + blue;
  }

  if (this->render.unknown_00F6 <= 50) {
    this->DrawQuad(quad, palette, 5, 0.0f, 0.0f, 0);
    this->DrawQuad(quad, palette, 5, 0.0f, 0.0f, 0);
  }
}

// OFFSET: 0x00CC2240
// FUNCTION: fn_21138
void CRenderObject::SetInstantValue(int key, std::int64_t value) {
  this->render.instant.Set(key, value);
}

// OFFSET: 0x00CC2060
// FUNCTION: fn_21135
void CGameActor::Slot49(int guildId, int version) {
  if (guildId <= 0) {
    return;
  }

  this->guildId = guildId;
  if (this->GetEffectiveJob(-1) != 20269) {
    if (this->GetEffectiveJob(-1) != JT_GUILD_FLAG && version <= 0) {
      return;
    }
    if (!g_session.IsNpcJob(this->GetEffectiveJob(-1)) && !g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) &&
        !g_session.IsGuardianJob(this->GetEffectiveJob(-1)) && this->GetEffectiveJob(-1) != JT_GUILD_FLAG &&
        this->GetEffectiveJob(-1) != JT_EMPELIUM) {
      return;
    }
  }

  g_guildMgr->SetEmblemVersion(guildId, version);
}

// OFFSET: 0x00CC21F0
// FUNCTION: fn_21136
void CGameActor::Slot30() {
}

// OFFSET: 0x00CC27C0
// FUNCTION: fn_21141
void CGameActor::SetUnknown0318(const float* value, int index) {
  if (index == 0) {
    std::memcpy(this->unknown_0318, value, sizeof(this->unknown_0318));
  } else if (index == 1) {
    std::memcpy(this->unknown_0348, value, sizeof(this->unknown_0348));
  }
}

// OFFSET: 0x00CC2D50
// FUNCTION: fn_21145
void CGameActor::SetUnknown0330(const float* value, int index) {
  if (index == 0) {
    std::memcpy(this->unknown_0330, value, sizeof(this->unknown_0330));
  } else if (index == 1) {
    std::memcpy(this->unknown_0360, value, sizeof(this->unknown_0360));
  }
}

// OFFSET: 0x00CC2DC0
// FUNCTION: fn_21147
void CGameActor::Slot25(int) {
}

// OFFSET: 0x00CC2DD0
// FUNCTION: fn_21148
void CGameActor::Slot18(int, int, int) {
}

// OFFSET: 0x00CC2DE0
// FUNCTION: fn_21149
void CGameActor::Slot22(int) {
}

// OFFSET: 0x00CC2DF0
// FUNCTION: fn_21150
void CGameActor::Slot21(int) {
}

// OFFSET: 0x00CC2E40
// FUNCTION: fn_21152
void CGameActor::Slot23(int, int) {
}

// OFFSET: 0x00CAA1F0
// FUNCTION: fn_21015
void CRenderObject::JitterBounds(float* output, const int* input, float offset, int spread, int base) {
  if ((this->render.flags & 0x2000) == 0) {
    return;
  }

  float jitter[4];
  for (int index = 0; index < 4; ++index) {
    int value = 0;
    if (spread != 0) {
      const int remainder = rand() % spread;
      if (remainder >= 0) {
        value = remainder;
      }
    }

    CGameMode* mode = static_cast<CGameMode*>(g_world.GetGameMode());
    jitter[index] = 400.0f / mode->camera->camera.desiredDistance * static_cast<float>(value - base);
  }

  output[0] = static_cast<float>(input[1]) + jitter[0];
  output[2] = static_cast<float>(input[3]) + jitter[1];
  output[1] = static_cast<float>(input[0]) + offset + jitter[2];
  output[3] = static_cast<float>(input[2]) + offset + jitter[3];
}

// OFFSET: 0x00CAFEE0
// FUNCTION: fn_21090
void CGameActor::ProcessBodyState() {
  switch (this->bodyState) {
  case BODYSTATE_FREEZING:
  case BODYSTATE_STONECURSE:
    this->render.isMotionFreezed = true;
    break;
  }
}

// OFFSET: 0x0077E3A0
// FUNCTION: fn_08455
int CGameActor::GetShield() {
  return -1;
}

// OFFSET: 0x005A1280
// FUNCTION: fn_03042
void CGameActor::Slot62(int value) {
  this->SetUnknown0260(value);
}

// OFFSET: 0x005A1290
// FUNCTION: fn_03043
int CGameActor::Slot46() {
  return 0;
}

// OFFSET: 0x005A12A0
// FUNCTION: fn_03044
std::uint32_t CGameActor::Slot13() {
  return this->stateFlags;
}

// OFFSET: 0x005A12B0
// FUNCTION: fn_03045
int CGameActor::Slot50() {
  return this->guildId;
}

// OFFSET: 0x005A12C0
// FUNCTION: fn_03046
float* CGameActor::GetExtent(float* extent) const {
  extent[0] = 0.0f;
  extent[1] = 0.0f;
  return extent;
}

// OFFSET: 0x005A12E0
// FUNCTION: fn_03047
bool CGameActor::Slot58() {
  return false;
}

// OFFSET: 0x005A12F0
// FUNCTION: fn_03048
void CGameActor::Slot45() {
  int radius = 0;
  g_session.m_hotkeyScript->Call("GetNpcLocationRadius", "d>d", this->GetEffectiveJob(-1), &radius);
}

// OFFSET: 0x005A13B0
// FUNCTION: fn_03049
int CGameActor::Slot56() {
  return 0;
}

// OFFSET: 0x005A13C0
// FUNCTION: fn_03050
int CGameActor::Slot42() {
  return 0;
}

// OFFSET: 0x005A13D0
// FUNCTION: fn_03051
std::int64_t CGameActor::Slot35() {
  return -1;
}

// OFFSET: 0x005A1930
// FUNCTION: fn_45454
void CGameActor::Slot44(int, int) {
}

// OFFSET: 0x005A1960
// FUNCTION: fn_03054
void CGameActor::Slot48(int value) {
  this->guildId = value;
}

// OFFSET: 0x005A1990
// FUNCTION: fn_03057
void CGameActor::Slot40(int) {
}

// OFFSET: 0x005A19B0
// FUNCTION: fn_03059
void CGameActor::Slot59(bool) {
}

// OFFSET: 0x005A19C0
// FUNCTION: fn_03060
void CGameActor::Slot57(int) {
}

// OFFSET: 0x005A19D0
// FUNCTION: fn_03061
void CGameActor::Slot41(int, int) {
}

// OFFSET: 0x005A19E0
// FUNCTION: fn_03062
void CGameActor::Slot47(unsigned char value) {
  this->unknown_02CC = value;
}

// OFFSET: 0x005A1A40
// FUNCTION: fn_03068
void CGameActor::Slot60() {
}

// OFFSET: 0x005A1A70
// FUNCTION: fn_03071
void CGameActor::Slot43(int, int) {
}

// OFFSET: 0x00CABD40
// FUNCTION: fn_21037
int CGameActor::Slot37() {
  const CActRes* action = this->render.action;
  const int count = static_cast<int>(action->FrameCount(this->render.previousAnimation));

  for (int frame = 0; frame < count; ++frame) {
    const int sound = action->Frame(this->render.previousAnimation, frame).sound;

    if (sound != -1 && std::strcmp(action->Sound(sound), "atk") == 0) {
      return frame;
    }
  }

  return count - (count != 1 ? 2 : 1);
}

// OFFSET: 0x00CACEF0
// FUNCTION: fn_21048
float* CGameActor::Slot51(int index) {
  if (index == 0) {
    return this->unknown_0318;
  }
  if (index == 1) {
    return this->unknown_0348;
  }
  return nullptr;
}

// OFFSET: 0x00CAD480
// FUNCTION: fn_21052
float* CGameActor::Slot53(int index) {
  if (index == 0) {
    return this->unknown_0330;
  }
  if (index == 1) {
    return this->unknown_0360;
  }
  return nullptr;
}

// OFFSET: 0x00CAE500
// FUNCTION: fn_21074
void CGameActor::Slot38() {
  CGameMode* mode = static_cast<CGameMode*>(g_world.GetGameMode());
  const float angle = this->render.angle;
  const int motion = this->render.curMotion;
  const int animation = this->render.animation;
  const char* sprite = this->render.sprite ? this->render.sprite->name.text : nullptr;
  const char* action = this->render.action ? this->render.action->name.text : nullptr;
  CCorpse* corpse = mode->SpawnCorpse(
    this->render.position.x, this->render.position.y, this->render.position.z, action, sprite, animation, motion, angle
  );

  if (corpse) {
    if (this->render.unknown_010C > 0) {
      corpse->render.unknown_010C = this->render.unknown_010C;
    }

    if (CEffect* effect = g_effectMgr->Find(static_cast<int>(this->render.accountId), 2374)) {
      corpse->spriteOffset = *reinterpret_cast<const float*>(&effect->unknown_0048);
    }
  }
}

// OFFSET: 0x00CABEB0
// FUNCTION: fn_21040
int CGameActor::Slot55() {
  if (this->unknown_03C4 != -1) {
    return this->unknown_03C4;
  }

  if (this->render.accountId == g_session.m_aid) {
    return g_session.GetPairValue();
  }

  const int first = this->unknown_03B8[0] != -1 ? this->unknown_03B8[0] : -1;
  const int second = this->unknown_03B8[1] == -1 ? first : this->unknown_03B8[1];
  const int third = this->unknown_03B8[2];
  return third != -1 ? third : second;
}

// OFFSET: 0x00CABFA0
// FUNCTION: fn_21042
int CGameActor::GetEffectiveJob(int job) {
  if (job == -1) {
    job = this->unknown_025C;
  }

  if (StateFlag12(this->Slot13())) {
    return JT_MARRIED;
  }

  if (!StateFlag15(this->Slot13())) {
    if (StateFlag16(this->Slot13())) {
      return JT_SANTA;
    }
    if (StateFlag18(this->Slot13())) {
      return JT_SUMMER;
    }
    if (StateFlag27(this->Slot13())) {
      return JT_HANBOK;
    }
    if (StateFlag28(this->Slot13())) {
      return JT_OKTOBERFEST;
    }
    if (this->unknown_03DC == 1) {
      return JT_SUMMER2;
    }
  }

  if (StateFlag05(this->Slot13())) {
    switch (job) {
      case JT_KNIGHT:
        return JT_CHICKEN;
      case JT_CRUSADER:
        return JT_CHICKEN2;
      case JT_KNIGHT_H:
        return JT_CHICKEN_H;
      case JT_CRUSADER_H:
        return JT_CHICKEN2_H;
      case JT_KNIGHT_B:
        return JT_CHICKEN_B;
      case JT_CRUSADER_B:
        return JT_CHICKEN2_B;
      case JT_ROYAL_GUARD:
        return JT_ROYAL_CHICKEN;
      case JT_ROYAL_GUARD_H:
        return JT_ROYAL_CHICKEN_H;
      case JT_ROYAL_GUARD_B:
        return JT_ROYAL_CHICKEN_B;
      case JT_IMPERIAL_GUARD:
        return JT_IMPERIAL_GUARD_CHICKEN;
    }
  }

  if (StateFlag19(this->Slot13())) {
    switch (job) {
      case JT_RUNE_KNIGHT:
        return JT_RUNE_CHICKEN;
      case JT_RUNE_KNIGHT_H:
        return JT_RUNE_CHICKEN_H;
      case JT_RUNE_KNIGHT_B:
        return JT_RUNE_CHICKEN_B;
      case JT_DRAGON_KNIGHT:
        return JT_DRAGON_KNIGHT_CHICKEN;
    }
  } else if (StateFlag23(this->Slot13())) {
    if (job == JT_RUNE_KNIGHT) {
      return JT_RUNE_CHICKEN2;
    }
    if (job == JT_RUNE_KNIGHT_H) {
      return JT_RUNE_CHICKEN2_H;
    }
  } else if (StateFlag24(this->Slot13())) {
    if (job == JT_RUNE_KNIGHT) {
      return JT_RUNE_CHICKEN3;
    }
    if (job == JT_RUNE_KNIGHT_H) {
      return JT_RUNE_CHICKEN3_H;
    }
  } else if (StateFlag25(this->Slot13())) {
    if (job == JT_RUNE_KNIGHT) {
      return JT_RUNE_CHICKEN4;
    }
    if (job == JT_RUNE_KNIGHT_H) {
      return JT_RUNE_CHICKEN4_H;
    }
  } else if (StateFlag26(this->Slot13())) {
    if (job == JT_RUNE_KNIGHT) {
      return JT_RUNE_CHICKEN5;
    }
    if (job == JT_RUNE_KNIGHT_H) {
      return JT_RUNE_CHICKEN5_H;
    }
  }

  if (StateFlag21(this->Slot13())) {
    switch (job) {
      case JT_RANGER:
        return JT_WOLF_RANGER;
      case JT_RANGER_H:
        return JT_WOLF_RANGER_H;
      case JT_RANGER_B:
        return JT_WOLF_RANGER_B;
      case JT_WINDHAWK:
        return JT_WOLF_WINDHAWK;
    }
  }

  if (this->Slot56() != 0) {
    switch (job) {
      case JT_MECHANIC:
        return JT_MADOGEAR;
      case JT_MECHANIC_H:
        return JT_MADOGEAR_H;
      case JT_MECHANIC_B:
        return JT_MADOGEAR_B;
      case JT_MEISTER:
        return JT_MEISTER_MADOGEAR;
    }
  }

  if (StateFlag15(this->Slot13())) {
    switch (job) {
      case JT_STAR:
        return JT_STAR2;
      case JT_STAR_B:
        return JT_STAR2_B;
      case JT_STAR_EMPEROR:
        return JT_STAR2_EMPEROR;
      case JT_STAR_EMPEROR_B:
        return JT_STAR2_EMPEROR_B;
      case JT_SKY_EMPEROR:
        return JT_SKY_EMPEROR2;
    }
  }

  return job;
}

// OFFSET: 0x00CCCAF0
// FUNCTION: fn_21199
void CGameActor::Slot39(int value) {
  const float speed = static_cast<float>(std::min(value, g_maxMoveSpeed)) / 432.0f;
  this->render.unknown_0054 = speed;
  this->render.speed = speed;
}
