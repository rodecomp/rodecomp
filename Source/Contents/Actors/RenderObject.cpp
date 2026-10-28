#include "Contents/Actors/RenderObject.h"

#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Replay.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Actors/GameActor.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CA8730
// FUNCTION: fn_20991
CRenderObject::CRenderObject(ResourceMgr& resources, JobNames& names) : render(resources, names) {
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
    this->render.action->Release();
  }

  this->render.action = static_cast<CActRes*>(resources.Load(name));

  if (this->render.action) {
    this->render.action->AddRef();
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
    this->render.sprite->Release();
  }

  this->render.sprite = static_cast<CSprRes*>(resources.Load(name));

  if (this->render.sprite) {
    this->render.sprite->AddRef();
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
  this->render.unknown_003C = 0;
  this->render.unknown_006C = repeat;
  this->render.unknown_009C[1] = 0;
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
  CGameMode* mode = static_cast<CGameMode*>(g_world.ActiveGameMode());
  const short scaled = static_cast<short>(400.0f / mode->camera->camera.desiredDistance * height);
  return scaled < 0 ? 0 : scaled;
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

// OFFSET: 0x00CC2240
// FUNCTION: fn_21138
void CRenderObject::SetInstantValue(int key, std::int64_t value) {
  this->render.instant.Set(key, value);
}

// OFFSET: 0x00CC21F0
// FUNCTION: fn_21136
void CGameActor::Slot30() {
}

// OFFSET: 0x00CC27C0

// OFFSET: 0x00CC2D50

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
