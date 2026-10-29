#include "Contents/Actors/Npc.h"
#include "Contents/Actors/RenderObject.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Client/Replay.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/EffectActors.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/GuildMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Render/Renderer.h"
#include "Render/SpriteQuad.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

// OFFSET: 0x00CA8730
// FUNCTION: fn_20991
CRenderObject::CRenderObject() {
}

// OFFSET: 0x00CA8F90
// FUNCTION: fn_20998
CRenderObject::~CRenderObject() = default;

// OFFSET: 0x00CAFDC0
// FUNCTION: fn_21089
bool CRenderObject::OnProcess() {
  return false;
}

// OFFSET: 0x00CBF920
// FUNCTION: fn_21115
void CRenderObject::SendMsg(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
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
void CRenderObject::SetRenderInfo(const float*, float, float) {
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
void CRenderObject::SetTlvert(float x, float y) {
  this->render.screenX = TruncateToInt(x);
  this->render.screenY = TruncateToInt(y);
}

// OFFSET: 0x00CBFAE0
// FUNCTION: fn_21118
void CRenderObject::SetAction(unsigned animation, int, int repeat) {
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
void CGameActor::SetAction(unsigned animation, int motion, int repeat) {
  if (this->unknown_0314 == 7) {
    if (!this->render.action || static_cast<int>(this->render.action->animations.size()) <= static_cast<int>(animation)) {
      return;
    }
  }

  CRenderObject::SetAction(animation, motion, repeat);
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
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
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

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
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
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
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

// OFFSET: 0x005A1A30
// FUNCTION: fn_03067
void CGameActor::Slot19(int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x005A1A20
// FUNCTION: fn_03066
void CGameActor::Slot20(int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x005A19F0
// FUNCTION: fn_03063
void CGameActor::Slot26(int) {
}

// OFFSET: 0x005A1A00
// FUNCTION: fn_03064
void CGameActor::Slot27(int) {
}

// OFFSET: 0x005A1980
// FUNCTION: fn_03056
void CGameActor::Slot31() {
}

// OFFSET: 0x005A1970
// FUNCTION: fn_03055
void CGameActor::Slot34(int) {
}

// OFFSET: 0x005A1A50
// FUNCTION: fn_03069
void CNpc::Slot22(int) {
}

// OFFSET: 0x005A1A10
// FUNCTION: fn_03065
void CNpc::Slot25(int) {
}

// OFFSET: 0x005A19A0
// FUNCTION: fn_03058
void CNpc::Slot30() {
}

// OFFSET: 0x005A1A60
// FUNCTION: fn_03070
void CCharacterNpc::Slot21(int) {
}

// OFFSET: 0x00CAC760
// FUNCTION: fn_21044
int CGameActor::GetUnknown0404Job() const {
  switch (this->unknown_0404) {
  case 1:
    return 0x1104;
  case 2:
    return 0x1105;
  default:
    return this->unknown_025C;
  }
}

// OFFSET: 0x00CC40C0
// FUNCTION: fn_21157
void CGameActor::SetUnknown0404(int value) {
  this->unknown_0404 = value;
}

// OFFSET: 0x00CC3DE0
// FUNCTION: fn_21155
void CGameActor::SetUnknown008C(const std::uint32_t* value) {
  this->render.unknown_008C = value[0];
  this->render.unknown_0090 = value[1];
  this->render.unknown_0094 = value[2];
}

// OFFSET: 0x00CC41E0
// FUNCTION: fn_21159
void CGameActor::Slot17() {
  g_modeMgr.GetGameMode()->SendMsg(13, this->render.accountId, 0, 0, 0);
}

// OFFSET: 0x00CC1E30
// FUNCTION: fn_21129
void CGameActor::SetUnknown03C4(int value) {
  this->unknown_03C8 = 0;
  this->unknown_03C4 = value;
  this->Slot21(value > 0 ? value : this->GetEffectiveJob(-1));
  this->Slot15(this->render.unknown_0070, 1);
}

// OFFSET: 0x00CABDE0
// FUNCTION: fn_21038
void CRenderObject::ScaleByFadePulse(float& scale) const {
  if ((this->render.flags & 0x10) != 0) {
    const unsigned index = std::max(this->render.pulsePhases[0] * 4u + 90u, 90u);
    if (index <= 270) {
      scale = ((ClientSine(index) + 1.0f) * 0.25f + 0.5f) * scale;
    } else {
      scale = scale * 0.5f;
    }
  }
}

// OFFSET: 0x00CC4270
// FUNCTION: fn_21161
void CRenderObject::UpdateTlvertex(const float* matrix) {
  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, matrix, &x, &y, &reciprocalDepth);
  this->SetTlvert(x, y);
}

// OFFSET: 0x00CABE60
// FUNCTION: fn_21039
int CGameActor::ActionBase(int state) {
  int base = 16;
  switch (state) {
  case 0:
    base = 0;
    break;
  case 1:
    base = 8;
    break;
  case 3:
    base = 32;
    break;
  case 4:
    base = 24;
    break;
  }

  return base;
}

// OFFSET: 0x00CAC730
// FUNCTION: fn_21043
int ShapeShiftJob(int form, int job) {
  switch (form) {
  case 1:
    return JT_WEREWOLF;
  case 2:
    return JT_WERERAPTOR;
  default:
    return job;
  }
}

// OFFSET: 0x00CAC790
// FUNCTION: fn_21045
static std::string ShapeShiftName(int form, int job) {
  switch (form) {
  case 1:
    switch (job) {
    case JT_KARNOS:
    case JT_KARNOS_B: {
      const std::uint8_t source[] = {0xC8, 0x80, 0x3F, 0xEE, 0x80, 0x66, 0xB1, 0xE3, 0xAD, 0xC5};
      thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
      return name.Text();
    }
    case JT_ALITEA: {
      const std::uint8_t source[] = {0x5E, 0x62, 0x7D, 0xFE, 0xEA, 0x12, 0x31, 0x9F, 0x3A, 0x27};
      thread_local EncryptedName<sizeof(source), 0xD97D5DBDBB2F2709ULL> name(source);
      return name.Text();
    }
    default: {
      const std::uint8_t source[] = {0x28, 0x56, 0x8D, 0x94, 0xD2, 0x28, 0x35, 0xAB, 0x7F};
      thread_local EncryptedName<sizeof(source), 0xED796785D1DF137FULL> name(source);
      return name.Text();
    }
    }
  case 2:
    switch (job) {
    case JT_KARNOS:
    case JT_KARNOS_B: {
      const std::uint8_t source[] = {0xEA, 0xF8, 0xBF, 0x72, 0xE9, 0xFE, 0xBF, 0xD7, 0xF2, 0xEF, 0xDF, 0x37};
      thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
      return name.Text();
    }
    case JT_ALITEA: {
      const std::uint8_t source[] = {0xF0, 0xB0, 0x2D, 0x7C, 0x63, 0x0A, 0x7B, 0xEB, 0xE8, 0xA7, 0x4C, 0x39};
      thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
      return name.Text();
    }
    default: {
      const std::uint8_t source[] = {0xB6, 0x3E, 0xAB, 0x92, 0x87, 0x52, 0x69, 0x89, 0xAE, 0x29, 0xF9};
      thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> name(source);
      return name.Text();
    }
    }
  default:
    return "";
  }
}

// OFFSET: 0x00CACD00
// FUNCTION: fn_21046
std::string CGameActor::GetShapeShiftName() {
  return ShapeShiftName(this->unknown_0404, this->unknown_025C);
}

// OFFSET: 0x00CACD70
// FUNCTION: fn_21047
int CGameActor::MountType() {
  switch (this->GetEffectiveJob(-1)) {
  case JT_CHICKEN:
  case JT_CHICKEN2:
  case JT_CHICKEN_H:
  case JT_CHICKEN2_H:
  case JT_CHICKEN_B:
  case JT_CHICKEN2_B:
    return 3;
  case JT_RUNE_CHICKEN:
  case JT_RUNE_CHICKEN_H:
  case JT_ROYAL_CHICKEN:
  case JT_ROYAL_CHICKEN_H:
  case JT_WOLF_RANGER:
  case JT_WOLF_RANGER_H:
  case JT_MADOGEAR:
  case JT_MADOGEAR_H:
  case JT_RUNE_CHICKEN2:
  case JT_RUNE_CHICKEN2_H:
  case JT_RUNE_CHICKEN3:
  case JT_RUNE_CHICKEN3_H:
  case JT_RUNE_CHICKEN4:
  case JT_RUNE_CHICKEN4_H:
  case JT_RUNE_CHICKEN5:
  case JT_RUNE_CHICKEN5_H:
  case JT_RUNE_CHICKEN_B:
  case JT_ROYAL_CHICKEN_B:
  case JT_WOLF_RANGER_B:
  case JT_MADOGEAR_B:
  case JT_WOLF_WINDHAWK:
  case JT_MEISTER_MADOGEAR:
  case JT_DRAGON_KNIGHT_CHICKEN:
  case JT_IMPERIAL_GUARD_CHICKEN:
    return 5;
  default:
    return 0;
  }
}

// OFFSET: 0x00CC2200
// FUNCTION: fn_21137
void RenderBounds::Set(float left, float top, float right, float bottom, float offset) {
  this->left = left;
  this->right = right;
  this->top = top;
  this->bottom = bottom;
  this->offset = offset;
}

// OFFSET: 0x00CC4210
// FUNCTION: fn_21160
void RenderBounds::Include(const float* box) {
  const float minX = box[1] < box[3] ? box[1] : box[3];
  const float maxX = box[1] > box[3] ? box[1] : box[3];
  if (this->left > minX) {
    this->left = minX;
  }
  if (this->top > box[0]) {
    this->top = box[0];
  }
  if (maxX > this->right) {
    this->right = maxX;
  }
  if (box[2] > this->bottom) {
    this->bottom = box[2];
  }
}

// OFFSET: 0x00CAA870
// FUNCTION: fn_21018
bool CGameActor::ApplyEffectState(int effectId, bool on) {
  if (effectId < 2485) {
    this->SetEffectState(effectId, on);
    return false;
  }

  int type = 0;
  g_session.m_hotkeyScript->Call("GetEffectType", "d>d", effectId - 2485, &type);
  switch (type) {
  case 0:
    this->SetEffectState(effectId, on);
    break;
  case 1:
    this->SetHatEffect(effectId, on);
    break;
  case 2:
    this->SetEffectState(effectId, on);
    this->SetHatEffect(effectId, on);
    break;
  case 3:
    this->SetEffectInstance(2411, effectId - 2485, on);
    break;
  case 4:
    this->SetEffectInstance(2412, effectId - 2485, on);
    break;
  }
  return false;
}

// OFFSET: 0x00CAA9B0
// FUNCTION: fn_21019
void CGameActor::SetHatEffect(int effectId, bool on) {
  int hatEffect = 0;
  g_session.m_hotkeyScript->Call("GetHatEffectID", "d>d", effectId - 2485, &hatEffect);
  if (hatEffect < 1) {
    return;
  }

  const std::int64_t value = hatEffect;
  this->SendMsg(this, 109, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  bool show = on;
  CPc* pc;
  if (hatEffect == 254 && IsPlayerJob(this->GetEffectiveJob(-1)) && (pc = dynamic_cast<CPc*>(this)) != nullptr) {
    pc->unknown_0438 = on ? pc->unknown_0438 | 0x10 : pc->unknown_0438 ^ 0x10;
    show = (pc->unknown_0434 & 0x10) != 0 || (pc->unknown_0438 & 0x10) != 0;
  }
  if (!show) {
    return;
  }

  CEffect* effect = g_effectMgr->Spawn(this->render.accountId, hatEffect, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->unknown_00C8 = 1;
  } else {
    this->LaunchEffect(hatEffect, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x00CAABA0
// FUNCTION: fn_21020
void CGameActor::SetEffectState(int effectId, bool on) {
  if (on) {
    CEffect* effect = g_effectMgr->Spawn(this->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->unknown_00C8 = 1;
    }
  } else {
    g_effectMgr->Retire(this->render.accountId, effectId);
  }
}

// OFFSET: 0x00CAAC10
// FUNCTION: fn_21021
void CGameActor::SetEffectInstance(int effectId, int instance, bool on) {
  if (on) {
    g_effectMgr->Spawn(this->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, instance);
  } else {
    g_effectMgr->Retire(this->render.accountId, instance, effectId);
  }
}

// OFFSET: 0x00CAB020
// FUNCTION: fn_21024
void CGameActor::ClearUnknown037C(int value) {
  if (this->unknown_037C[1] == value) {
    this->unknown_037C[1] = 0;
  } else if (this->unknown_037C[0] == value) {
    this->unknown_037C[0] = 0;
  } else if (this->unknown_037C[2] == value) {
    this->unknown_037C[2] = 0;
  } else if (this->unknown_037C[3] == value) {
    this->unknown_037C[3] = 0;
  } else if (this->unknown_037C[4] == value) {
    this->unknown_037C[4] = 0;
  } else if (this->unknown_037C[5] == value) {
    this->unknown_037C[5] = 0;
  } else if (this->unknown_037C[6] == value) {
    this->unknown_037C[6] = 0;
  } else if (this->unknown_037C[8] == value) {
    this->unknown_037C[8] = 0;
  } else if (this->unknown_037C[9] == value) {
    this->unknown_037C[9] = 0;
  } else if (this->unknown_037C[7] == value) {
    this->unknown_037C[7] = 0;
  } else if (this->unknown_037C[10] == value) {
    this->unknown_037C[10] = 0;
  } else if (this->unknown_037C[11] == value) {
    this->unknown_037C[11] = 0;
  } else if (this->unknown_037C[12] == value) {
    this->unknown_037C[12] = 0;
  }
}

// OFFSET: 0x00CAB9B0
// FUNCTION: fn_21033
CActRes* CRenderObject::GetActRes() {
  return this->render.action;
}

// OFFSET: 0x00CAB9C0
// FUNCTION: fn_21034
int CGameActor::WeaponActionBase(int weaponType) {
  int base = 0;
  switch (weaponType) {
  case 0:
    base = 0;
    break;
  case 1:
    base = 8;
    break;
  case 2:
  case 43:
  case 44:
  case 52:
  case 60:
  case 62:
  case 63:
  case 69:
  case 71:
  case 72:
  case 78:
  case 79:
    base = 80;
    break;
  case 3:
    base = 64;
    break;
  case 4:
    base = 48;
    break;
  case 5:
  case 20:
  case 25:
    base = 24;
    break;
  case 9:
  case 18:
  case 32:
  case 49:
  case 53:
  case 54:
  case 59:
  case 70:
  case 73:
  case 76:
  case 77:
    base = 88;
    break;
  case 11:
    base = 32;
    break;
  case 12:
  case 19:
  case 28:
  case 40:
  case 42:
  case 50:
  case 51:
  case 58:
    base = 40;
    break;
  case 13:
    base = IsJobWeaponAllowed(this->GetEffectiveJob(-1), this->unknown_0260, this->Slot35()) ? 88 : 80;
    break;
  case 16:
  case 17:
  case 21:
  case 22:
  case 23:
  case 24:
  case 27:
  case 30:
  case 33:
  case 34:
  case 35:
  case 36:
  case 37:
  case 38:
  case 39:
  case 41:
  case 45:
  case 47:
  case 48:
  case 55:
  case 56:
  case 57:
  case 61:
  case 64:
  case 65:
  case 66:
  case 67:
  case 68:
  case 80:
  case 81:
    base = 96;
    break;
  case 26:
    switch (this->GetEffectiveJob(-1)) {
    case JT_LINKER:
    case JT_LINKER_B:
    case JT_SOUL_REAPER:
    case JT_SOUL_REAPER_B:
    case JT_SOUL_ASCETIC:
      base = 88;
      break;
    default:
      base = 40;
      break;
    }
    break;
  case 74:
  case 75:
    base = 104;
    break;
  }
  return base;
}

// OFFSET: 0x00CABD30
// FUNCTION: fn_21036
int CGameActor::GetUnknown0404() {
  return this->unknown_0404;
}
