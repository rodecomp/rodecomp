#include "Contents/Actors/Pc.h"

#include "Client/Session.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/MessageTable.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Text/FormatString.h"

// GLOBAL: gl_24753
extern const int g_maxMoveSpeed = 432;

// OFFSET: 0x00DAFE50
// FUNCTION: fn_22606
void CGrannyPc::SetAction(unsigned animation, int, int repeat) {
  if (!this->model) {
    return;
  }

  this->render.unknown_0070 = 0;
  switch (animation) {
    case 8:
      this->render.unknown_0070 = 1;
      break;
    case 16:
      this->render.unknown_0070 = 6;
      break;
    case 24:
      this->render.unknown_0070 = 5;
      break;
    case 32:
      this->render.unknown_0070 = 11;
      break;
    case 40:
      this->render.unknown_0070 = 12;
      break;
    case 48:
      this->render.unknown_0070 = 4;
      break;
    case 64:
      this->render.unknown_0070 = 3;
      break;
    case 80:
    case 88:
      this->render.unknown_0070 = 2;
      break;
  }
  this->PlayMotion(repeat);
}

// OFFSET: 0x00DB01F0
// FUNCTION: fn_22611
void CGrannyPc::PlayMotion(int repeat) {
  this->motion = this->render.unknown_0070;
  for (int i = 0; i < 20; ++i) {
    this->model->FreeControl(this->controls[i]);
    this->controls[i] = nullptr;
  }
  this->unknown_0F24 = 0;

  const int loopCount = (repeat == 1 && this->render.unknown_0070 != 6) ? 1 : 0;
  this->controls[this->motion] = this->model->PlayAnimation(this->motion, this->instance, this->controls[this->motion], loopCount);
}

// OFFSET: 0x00DAFE00
// FUNCTION: fn_22604
void __stdcall fn_22604(int) {
}

// OFFSET: 0x00DB01E0
// FUNCTION: fn_22610
void __stdcall fn_22610(int) {
}

// OFFSET: 0x00DB07A0
// FUNCTION: fn_22623
void __stdcall fn_22623(int, int) {
}

// OFFSET: 0x00DB07B0
// FUNCTION: fn_22624
void __stdcall fn_22624(int, int) {
}

// OFFSET: 0x00DB5330
// FUNCTION: fn_22664
void __stdcall fn_22664() {
}

// OFFSET: 0x00DAFFA0
// FUNCTION: fn_22607
void CPc::Slot44(int first, int second) {
  this->unknown_04D0 = first;
  if (static_cast<unsigned char>(second) != 0) {
    int job = this->GetEffectiveJob(-1);
    switch (job) {
      case JT_MARRIED:
      case JT_SANTA:
      case JT_SUMMER:
      case JT_HANBOK:
      case JT_OKTOBERFEST:
      case JT_SUMMER2:
        job = this->GetEffectiveJob(-1);
        break;
      default:
        if (this->unknown_0404 != 0) {
          job = this->GetUnknown0404Job();
        } else {
          job = this->GetEffectiveJob(-1);
        }
        break;
    }
    this->Slot21(job);
    this->Slot24(this->weapon, this->unknown_044C);
    this->Slot32();
  }
}

// OFFSET: 0x00DB0000
// FUNCTION: fn_22608
void CPc::Slot33(int value) {
  this->render.previousAnimation = value;
  if (value < 2 && static_cast<unsigned>(this->unknown_04D0 - 0x10ec) > 0x11) {
    if (static_cast<unsigned>(this->unknown_025C - JT_NOVICE_H) <= 0x15) {
      this->render.previousAnimation = this->unknown_025C <= 0xfa7;
    } else {
      this->render.previousAnimation = static_cast<unsigned>(this->unknown_025C - JT_NOVICE_H) <= 6;
    }
  }
  this->Slot32();
}

// OFFSET: 0x00DB02A0
// FUNCTION: fn_22613
void CGrannyPc::Slot31() {
  if (this->unknown_0438 != 0) {
    this->unknown_0460 = g_session.GetHeadPalName(this->unknown_0434, this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0438);
  }
}

// OFFSET: 0x00DB0350
// FUNCTION: fn_22614
void CPc::Slot31() {
  if (this->unknown_0444 != 0) {
    this->unknown_0478 = g_session.GetHeadPalName(this->unknown_0440, this->unknown_04D4, this->unknown_0260, this->unknown_0444);
  }
}

// OFFSET: 0x00DB0280
// FUNCTION: fn_22612
void CPc::Slot34(int value) {
  this->unknown_0444 = value;
  this->Slot31();
}

// OFFSET: 0x00DB0400
// FUNCTION: fn_22615
void CGrannyPc::Slot40(int value) {
  if (value < 0) {
    const std::int64_t emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit);
    this->SendMsg(nullptr, 160, 0, 0, 0, static_cast<int>(emotion), static_cast<int>(emotion >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  this->unknown_0428 = value;
}

// OFFSET: 0x00DB0480
// FUNCTION: fn_22616
void CPc::Slot40(int value) {
  if (value < 0) {
    const std::int64_t emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit);
    this->SendMsg(nullptr, 160, 0, 0, 0, static_cast<int>(emotion), static_cast<int>(emotion >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  this->unknown_0430 = value;
}

// OFFSET: 0x00DB0760
// FUNCTION: fn_22621
void CGrannyPc::Slot41(int first, int second) {
  this->Slot40(first);
  this->unknown_042C = second;
}

// OFFSET: 0x00DB0780
// FUNCTION: fn_22622
void CPc::Slot41(int first, int second) {
  this->Slot40(first);
  this->unknown_0434 = second;
}

// OFFSET: 0x00DB07C0
// FUNCTION: fn_22625
void CPc::Slot47(unsigned char value) {
  this->unknown_02CC = value;
  this->Slot28(this->unknown_045C);
}

// OFFSET: 0x00DB07E0
// FUNCTION: fn_22626
void CGrannyPc::Slot26(int value) {
  this->unknown_044C = value;
}

// OFFSET: 0x00DB09D0
// FUNCTION: fn_22628
void CGrannyPc::Slot27(int value) {
  this->unknown_0450 = value;
}

// OFFSET: 0x00DB2660
// FUNCTION: fn_22644
void CGrannyPc::Slot28(int) {
  this->GetEffectiveJob(-1);
}

// OFFSET: 0x00DB0B40
// FUNCTION: fn_22630
void CGrannyPc::Slot25(int value) {
  this->unknown_0448 = value;
}

// OFFSET: 0x00DB0CB0
// FUNCTION: fn_22632
void CGrannyPc::Slot20(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int) {
  this->Slot19(first, second, third, fourth, fifth, sixth, seventh, eighth, ninth, this->unknown_0454, tenth);
}

// OFFSET: 0x00DB1960
// FUNCTION: fn_22636
void CPc::Slot60() {
  if (this->unknown_0458 == 185) {
    this->Slot27(185);
  }
}

// OFFSET: 0x00DB1980
// FUNCTION: fn_22637
void CGrannyPc::Slot22(int value) {
  this->unknown_0434 = value;
}

// OFFSET: 0x00DB1AC0
// FUNCTION: fn_22639
void CGrannyPc::Slot21(int value) {
  this->render.unknown_0054 = g_session.GetNpcLocationRadius(value, false);
}

// OFFSET: 0x00DB2830
// FUNCTION: fn_22646
void CGrannyPc::Slot24(int, int) {
}

// OFFSET: 0x00DB3720
// FUNCTION: fn_22648
void CGrannyPc::Slot23(int first, int second) {
  this->unknown_0440 = first;
  this->unknown_0444 = second;
}

// OFFSET: 0x00DBA6C0
// FUNCTION: fn_22674
void CPlayer::Slot17() {
}

// OFFSET: 0x00DB51E0
// FUNCTION: fn_22662
int CPlayer::AngleToDirection(float angle) {
  return (static_cast<int>(angle) + 180) / 45 % 8;
}

// OFFSET: 0x00DB52B0
// FUNCTION: fn_22663
bool CPlayer::IsSpecialState(int state) const {
  switch (state) {
    case 2:
    case 5:
    case 6:
    case 62:
    case 63:
    case 64:
    case 65:
    case 67:
    case 68:
    case 69:
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
      return true;
    default:
      return false;
  }
}

// OFFSET: 0x00DB06C0
// FUNCTION: fn_22619
void CGrannyPc::Slot39(int value) {
  if (this->unknown_0440 != 11 || this->unknown_0444 != 0) {
    value = std::min(value, g_maxMoveSpeed);
  }

  this->render.unknown_0054 = static_cast<float>(value) / 432.0f;
}

// OFFSET: 0x00DB0710
// FUNCTION: fn_22620
void CPc::Slot39(int value) {
  const int weapon = this->weapon;

  if (weapon > 105) {
    GetWeaponViewValue(weapon);
  }

  if (weapon != 11) {
    value = std::min(value, g_maxMoveSpeed);
  }

  this->render.unknown_0054 = static_cast<float>(value) / 432.0f;
}

// OFFSET: 0x00DDD190
// FUNCTION: fn_23381
static std::string GetBodyImfName(int job, int sex, int, int costumeJob) {
  const int spriteJob = g_session.GetSpriteJob(job, costumeJob, true);
  return FormatOwnedString("%s_%s.imf", g_session.GetBodyJobName(spriteJob).c_str(), g_session.GetSexName(sex).c_str());
}

// OFFSET: 0x00DB0500

// OFFSET: 0x00DB05E0

// OFFSET: 0x00DAFB60

// OFFSET: 0x00DAFE10

// OFFSET: 0x00DB2230

// OFFSET: 0x00DB2360

// OFFSET: 0x00DB4C70
// FUNCTION: fn_22652
int CPc::MapMotion(int motion) {
  switch (this->GetEffectiveJob(-1)) {
    case 2:
    case 9:
    case 0x10:
    case 0xFA3:
    case 0xFAA:
    case 0xFB1:
    case 0xFB9:
    case 0xFC0:
    case 0xFC7:
      return motion == 5 ? 10 : motion;
    case 6:
    case 0xFA7:
    case 0xFBD:
      return (motion == 8 || motion == 6 || motion == 7) ? 2 : motion;
    case 0xC:
    case 0x11:
    case 0xFAD:
    case 0xFB2:
    case 0xFC3:
    case 0xFC8:
    case 0xFDB:
    case 0xFE1:
    case 0xFE8:
    case 0xFEF:
    case 0x1005:
    case 0x100C:
    case 0x109E:
      if (motion == 8 || motion == 7) {
        return 2;
      }

      if (motion == 6 && (
        this->GetEffectiveJob(-1) == 0x11 || this->GetEffectiveJob(-1) == 0xFB2 || this->GetEffectiveJob(-1) == 0xFC8
      )) {
        return 1;
      }

      return motion;
    default:
      return motion;
  }
}
