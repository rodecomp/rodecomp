#include "Contents/Actors/GameActor.h"

#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/Pc.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Resource/Sprites/ActionResource.h"

#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CA83A0
// FUNCTION: fn_20988
CGameActor::CGameActor()
    : unknown_0214(), unknown_02F0(0), unknown_02F4(0), unknown_0318(), unknown_0330(), unknown_0348(), unknown_0360(),
      unknown_03F0(), unknown_0404(0), unknown_0408(0) {
  this->ResetState();
}

// OFFSET: 0x00CAF4F0
// FUNCTION: fn_21084
void CGameActor::ResetState() {
  this->unknown_037C[11] = 0;
  this->unknown_037C[2] = 0;
  this->unknown_037C[3] = 0;
  this->unknown_037C[1] = 0;
  this->unknown_037C[0] = 0;
  this->unknown_037C[6] = 0;
  this->unknown_037C[4] = 0;
  this->unknown_037C[5] = 0;
  this->unknown_037C[9] = 0;
  this->unknown_037C[7] = 0;
  this->unknown_037C[8] = 0;
  this->unknown_037C[10] = 0;
  this->unknown_037C[12] = 0;
  this->render.position.x = 0.0f;
  this->render.position.y = 0.0f;
  this->render.position.z = 0.0f;
  this->render.angle = 0.0f;
  this->render.animation = 0;
  this->render.previousAnimation = 0;
  this->render.curMotion = 0;
  this->render.unknown_0070 = 0;
  this->render.unknown_006C = 0;
  this->render.speed = 1.0f;
  this->render.unknown_00A4Bytes[0] = 1;
  this->unknown_0214.x = 0.0f;
  this->unknown_0214.y = 0.0f;
  this->unknown_0214.z = 0.0f;
  this->unknown_0240 = 0;
  this->render.accountId = 0xFFFFFFFF;
  this->unknown_0258 = g_session.GetServerTime();
  this->unknown_0244 = timeGetTime();
  this->render.screenX = 0;
  this->render.screenY = 0;
  this->unknown_0264[0] = 0;
  this->unknown_0264[1] = 0;
  this->unknown_0264[2] = 0;
  this->unknown_0264[3] = 0;
  this->unknown_0288 = 0;
  this->unknown_0290 = 0;
  this->render.isMotionFinished = false;
  this->hitTargetAid = 0;
  this->unknown_0378 = -1.0f;
  this->unknown_023C = -1;
  this->render.unknown_00A4Bytes[1] = 1;
  this->render.unknown_008C = timeGetTime();
  this->unknown_027C = 0;
  this->unknown_0280 = timeGetTime() - 5000;
  this->unknown_0284 = timeGetTime() - 10000;
  this->unknown_02D8 = 3600;
  this->render.color = 0xFFFFFFFF;
  this->render.visible = 1;
  this->render.unknown_00A2 = 0;
  this->unknown_02B0 = 0;
  this->bodyState = 0;
  this->stateFlags = 0;
  this->Slot47(0);
  this->unknown_02C4 = 0;
  this->unknown_02C8 = 0;
  this->unknown_02DC = 0;
  this->unknown_02B8 = 0;
  this->unknown_02B4 = 0;
  this->unknown_0210 = 0;
  this->unknown_0178 = 0;
  this->unknown_017C = 0;
  this->unknown_0180 = 0;
  std::memset(this->unknown_0190, 0, sizeof(this->unknown_0190));
  this->guildId = 0;
  this->unknown_0278 = 0;
  this->unknown_02E0 = 0;
  this->unknown_0188 = 0;
  this->unknown_018C = 0;
  this->unknown_0238 = 0;
  this->unknown_0184 = 0;
  this->unknown_0250 = 0;
  this->unknown_030C = 0;
  this->unknown_0310 = 0;
  this->unknown_0314 = 4;
  this->unknown_0264[4] = 0;
  this->unknown_03B8[0] = -1;
  this->unknown_03B8[1] = -1;
  this->unknown_03B8[2] = -1;
  this->unknown_03C4 = -1;
  this->unknown_03C8 = 0;
  this->charmType = 0;
  this->unknown_03D0 = 0;
  this->unknown_0158[0] = 0;
  this->unknown_0158[1] = 0;
  this->unknown_0158[2] = 0;
  this->unknown_0158[3] = 0;
  this->unknown_0158[4] = 0;
  this->unknown_0158[5] = 0;
  this->unknown_02F8 = 0;
  this->unknown_02FC = 0;
  this->unknown_0300 = 0;
  this->unknown_0304 = 0;
  this->unknown_0305 = 0;
  this->unknown_0308 = 0;
  this->unknown_03DC = 0;
  this->unknown_03E0 = 0;
  this->unknown_02EC = 0;
  this->unknown_03D4 = 0;
  this->unknown_03D8 = 0;
  this->unknown_03E8 = 0;
  this->unknown_03EC = 0;
}

// OFFSET: 0x00CB0A90
// FUNCTION: fn_21095
void CGameActor::UpdateMotionFrame() {
  const int frames = static_cast<int>(this->render.action->FrameCount(this->render.previousAnimation));
  this->render.curMotion = static_cast<int>(this->unknown_0240 * 0.37f * 4.0f / this->render.speed) % frames;
}

// OFFSET: 0x00CACFE0
// FUNCTION: fn_21050
int __stdcall MountedJob(int job, std::uint32_t flags) {
  if (job == 0xfd6) {
    if (StateFlag19(flags)) {
      return 0xff0;
    }
    if (StateFlag23(flags)) {
      return 0xff8;
    }
    if (StateFlag24(flags)) {
      return 0xffa;
    }
    if (StateFlag25(flags)) {
      return 0xffc;
    }
    if (StateFlag26(flags)) {
      return 0xffe;
    }
  } else if (job == 0xfdc) {
    if (StateFlag19(flags)) {
      return 0xff1;
    }
    if (StateFlag23(flags)) {
      return 0xff9;
    }
    if (StateFlag24(flags)) {
      return 0xffb;
    }
    if (StateFlag25(flags)) {
      return 0xffd;
    }
    if (StateFlag26(flags)) {
      return 0xfff;
    }
  } else if (job == 0x1000) {
    if (StateFlag19(flags)) {
      return 0x100d;
    }
  } else if (job == 0xfd8) {
    if (StateFlag21(flags)) {
      return 0xff4;
    }
  } else if (job == 0xfde) {
    if (StateFlag21(flags)) {
      return 0xff5;
    }
  } else if (job == 0x1002) {
    if (StateFlag21(flags)) {
      return 0x100f;
    }
  } else if (job == 0xfda) {
    if (StateFlag22(flags)) {
      return 0xff6;
    }
  } else if (job == 0xfe0) {
    if (StateFlag22(flags)) {
      return 0xff7;
    }
  } else if (job == 0x1004) {
    if (StateFlag22(flags)) {
      return 0x1010;
    }
  } else if (job == 0x7) {
    if (StateFlag05(flags)) {
      return 0xd;
    }
  } else if (job == 0xe) {
    if (StateFlag05(flags)) {
      return 0x15;
    }
  } else if (job == 0xfa8) {
    if (StateFlag05(flags)) {
      return 0xfae;
    }
  } else if (job == 0xfbe) {
    if (StateFlag05(flags)) {
      return 0xfc4;
    }
  } else if (job == 0xfaf) {
    if (StateFlag05(flags)) {
      return 0xfb6;
    }
  } else if (job == 0xfc5) {
    if (StateFlag05(flags)) {
      return 0xfcc;
    }
  } else if (job == 0xfe2) {
    if (StateFlag05(flags)) {
      return 0xff2;
    }
  } else if (job == 0xfe9) {
    if (StateFlag05(flags)) {
      return 0xff3;
    }
  } else if (job == 0x1006) {
    if (StateFlag05(flags)) {
      return 0x100e;
    }
  } else if (job == 0xfcf) {
    if (StateFlag15(flags)) {
      return 0xfd0;
    }
  } else if (job == 0x1082) {
    if (StateFlag15(flags)) {
      return 0x108e;
    }
  } else if (job == 0x10a1) {
    if (StateFlag21(flags)) {
      return 0x10b6;
    }
  } else if (job == 0x109d) {
    if (StateFlag22(flags)) {
      return 0x10b7;
    }
  } else if (job == 0x109c) {
    if (StateFlag19(flags)) {
      return 0x10b8;
    }
  } else if (job == 0x10a2) {
    if (StateFlag05(flags)) {
      return 0x10b9;
    }
  } else if (job == 0x10ce) {
    if (StateFlag15(flags)) {
      return 0x10dc;
    }
  }
  return job;
}

// OFFSET: 0x00CAD420
// FUNCTION: fn_21051
int CGameActor::GetEffectCounter(int effectId) {
  switch (effectId) {
  case 0x81:
  case 0x17d:
  case 0x14ce:
  case 0x14d1:
  case 0x1978:
    return this->unknown_037C[0];
  case 0x8c3:
  case 0x8c4:
    return this->unknown_037C[1];
  default:
    return 0;
  }
}

// OFFSET: 0x00CAD4B0
// FUNCTION: fn_21053
float CGameActor::GetNameOffset() {
  float scale = 1.0f;
  if ((this->render.flags & 0x10) != 0) {
    const unsigned int angle = this->render.pulsePhases[0] * 4 + 90;
    if (angle > 270) {
      scale = 0.5f;
    } else {
      scale = (g_sinTable[angle] + 1.0f) * 0.25f + g_depthAverageHalf;
    }
  }

  switch (this->GetEffectiveJob(-1)) {
  case 0xd:
  case 0x15:
  case 0xfae:
  case 0xfb6:
  case 0xff0:
  case 0xff1:
  case 0xff2:
  case 0xff3:
  case 0xff4:
  case 0xff5:
  case 0xff6:
  case 0xff7:
  case 0xff8:
  case 0xff9:
  case 0xffa:
  case 0xffb:
  case 0xffc:
  case 0xffd:
  case 0xffe:
  case 0xfff:
  case 0x10b6:
  case 0x10b7:
  case 0x10b8:
  case 0x10b9:
    return scale * 6.0f;
  case 0xfc4:
  case 0xfcc:
  case 0x100d:
  case 0x100e:
  case 0x100f:
  case 0x1010:
    return scale * 4.0f;
  default:
    break;
  }

  if (!g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) && !g_session.IsNpcJob(this->GetEffectiveJob(-1)) &&
      this->render.accountId != g_session.m_aid) {
    return 0.0f;
  }

  if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
    return this->unknown_02CC ? 4.0f : -1.0f;
  }

  if (g_session.IsDoramJob(this->GetEffectiveJob(-1))) {
    return this->unknown_02CC ? scale * -3.0f : scale * -2.0f;
  }

  if (!this->unknown_02CC) {
    return 0.0f;
  }

  CPc* pc = dynamic_cast<CPc*>(this);
  if (!pc) {
    return scale * 6.0f;
  }

  switch (static_cast<unsigned int>(pc->unknown_04D4)) {
  case 0x18:
  case 0x1077:
  case 0x10d2:
    return 0.0f;
  case 0:
  case 0x17:
  case 0xfa1:
  case 0x105e:
  case 0x10d3:
  case 0x10ff:
  case 0x1101:
  case 0x1103:
    return scale + scale;
  default:
    return scale * 6.0f;
  }
}

// OFFSET: 0x00CAD780
// FUNCTION: fn_21054
float CGameActor::GetNameHeight() {
  switch (this->GetEffectiveJob(-1)) {
  case 0xd:
  case 0x15:
  case 0xfae:
  case 0xfb6:
  case 0xff0:
  case 0xff1:
  case 0xff2:
  case 0xff3:
  case 0xff4:
  case 0xff5:
  case 0xff6:
  case 0xff7:
  case 0xff8:
  case 0xff9:
  case 0xffa:
  case 0xffb:
  case 0xffc:
  case 0xffd:
  case 0xffe:
  case 0xfff:
  case 0x10b6:
  case 0x10b7:
  case 0x10b8:
  case 0x10b9:
    return 23.0f;
  case 0xfc4:
  case 0xfcc:
  case 0x100d:
  case 0x100e:
  case 0x100f:
  case 0x1010:
    return 19.0f;
  default:
    break;
  }

  if (!g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) && !g_session.IsNpcJob(this->GetEffectiveJob(-1)) &&
      this->render.accountId != g_session.m_aid) {
    return 18.0f;
  }

  if (g_session.IsDoramJob(this->GetEffectiveJob(-1)) || !this->unknown_02CC) {
    return 18.0f;
  }

  CPc* pc = dynamic_cast<CPc*>(this);
  if (!pc) {
    return 23.0f;
  }

  switch (static_cast<unsigned int>(pc->unknown_04D4)) {
  case 0x18:
  case 0x1077:
  case 0x10d2:
    return 18.0f;
  case 0:
  case 0x17:
  case 0xfa1:
  case 0x105e:
  case 0x10d3:
  case 0x10ff:
  case 0x1101:
  case 0x1103:
    return 19.0f;
  default:
    return 23.0f;
  }
}

// OFFSET: 0x00CAD990
// FUNCTION: fn_21055
CSprRes* CGameActor::GetSprite() {
  return this->render.sprite;
}

// OFFSET: 0x00CAD9A0
// FUNCTION: fn_21056
float CGameActor::GetElapsedFrames() {
  return static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
}

// OFFSET: 0x00CADB30
// FUNCTION: fn_21060
void CGameActor::ResetUnknown03B8() {
  this->unknown_03B8[0] = -1;
  this->unknown_03B8[1] = -1;
  this->unknown_03B8[2] = -1;
  this->unknown_03C4 = -1;
  this->unknown_03C8 = 0;
}

// OFFSET: 0x00CADC90
// FUNCTION: fn_21065
bool CGameActor::IsMonsterClass(int job) {
  return (job > 1000 && job < 3999) || static_cast<unsigned int>(job - 20021) <= 11978;
}

// OFFSET: 0x00CADCC0
// FUNCTION: fn_21066
bool CGameActor::HasPendingDamage() {
  return !this->unknown_02A8.empty();
}

// OFFSET: 0x00CADCF0
// FUNCTION: fn_21067
bool CGameActor::UpdateUnknown03E8(std::int64_t value) {
  const std::int64_t previous = (static_cast<std::int64_t>(this->unknown_03EC) << 32) | static_cast<std::uint32_t>(this->unknown_03E8);
  this->unknown_03EC = static_cast<int>(value >> 32);
  this->unknown_03E8 = static_cast<int>(value);
  return previous < value;
}

// OFFSET: 0x00CADD30
// FUNCTION: fn_21068
int CGameActor::IsPlayerClass(int job) {
  return g_session.IsPlayerJobId(job);
}

// OFFSET: 0x00CAE1B0
// FUNCTION: fn_21072
void CGameActor::SetStackedEffect(int effectId, bool on) {
  if (!on) {
    this->unknown_03FC.erase(effectId);
    this->ApplyEffectState(effectId, false);
    return;
  }

  for (const int id : this->unknown_03FC) {
    this->ApplyEffectState(id, false);
  }
  this->unknown_03FC.insert(effectId);
  for (const int id : this->unknown_03FC) {
    this->ApplyEffectState(id, true);
  }
}
