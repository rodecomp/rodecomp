#include "Contents/Actors/Pc.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/AttackMotion.h"
#include "Contents/Actors/Item.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EnergyOrbEffect.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/TetraBallEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Effects/MsgEffect.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Items/WeaponType.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/SpriteLayerDrawing.h"
#include "Contents/SpriteRidingOffset.h"
#include "Contents/World/World.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/MovementRouteInterpolation.h"
#include "Contents/NavigationManager.h"
#include "Contents/TerrainPathSearch.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Render/SpriteQuad.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <cmath>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24753
extern const int g_maxMoveSpeed = 432;

// GLOBAL: gl_25175
unsigned char g_statusRefreshSuppressed;

// OFFSET: 0x00DA7700
// FUNCTION: fn_22558
CPc::CPc() : footprintPosition() {
  this->unknown_04F4 = 0;
  this->equipSprites.resize(9, nullptr);
  this->equipActions.resize(9, nullptr);
  this->unknown_0490 = nullptr;
  this->unknown_0444 = 0;
  this->unknown_045C = 0;
  this->unknown_0430 = 0;
  this->unknown_0434 = 0;
  this->guildId = 0;
  this->unknown_043C = 0;
  this->unknown_0460[0] = 0;
  this->unknown_0474 = 0;
  this->unknown_0494 = 0;
  this->unknown_0498 = 0;
  this->render.unknown_00A8 = 1;
  this->unknown_049C = -1;
  this->blend = -1;
  this->unknown_04A4 = -1;
  this->unknown_04A8 = -1;
  this->msgEffect = nullptr;
  this->unknown_04CC = 0;
  this->unknown_04D0 = 0;
  this->unknown_04D4 = 0;
  this->unknown_04D8[0] = 0;
  this->unknown_04D9 = false;
  this->unknown_0438 = 0;
  this->unknown_04DC = 0;
  this->unknown_04E0 = false;
  this->unknown_04E4 = 0.0f;
  this->unknown_04E8 = 0.0f;
  this->unknown_04EC = 0.0f;
  this->unknown_04F0 = 0.0f;
  this->weapon = 0;
  this->unknown_044C = 0;
}

// OFFSET: 0x00DA7BA0
// FUNCTION: fn_22560
CPc::~CPc() {
  for (int i = 0; i < 9; ++i) {
    if (this->equipSprites[i] != nullptr) {
      this->equipSprites[i]->Unlock();
    }
    if (this->equipActions[i] != nullptr) {
      this->equipActions[i]->Unlock();
    }
  }
  if (this->unknown_0490 != nullptr) {
    g_windowMgr.Schedule(this->unknown_0490);
    this->unknown_0490 = nullptr;
  }
}

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
    int job;
    if (g_session.IsCostumeJob(this->GetEffectiveJob(-1)) != 1 && this->unknown_0404 != 0) {
      job = this->GetUnknown0404Job();
    } else {
      job = this->GetEffectiveJob(-1);
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

// OFFSET: 0x00DB3C30
// FUNCTION: fn_22650
void CGrannyPc::Slot15(int state, int resetPosition) {
  const int current = this->render.unknown_0070;
  if (current == 3 || this->unknown_017C != 0) {
    return;
  }

  if (current == 4 && static_cast<char>(resetPosition) == 1) {
    this->render.position.x = this->unknown_02D0;
    this->render.position.z = this->unknown_02D4;
  }

  using SpawnArrow = void (CGameMode::*)(const vector3d*, std::uint32_t, float, int);
  const SpawnArrow spawnArrow = reinterpret_cast<SpawnArrow>(&CGameMode::SpawnArrowEffect);

  switch (state) {
  case 0:
    CGameActor::Slot15(0, resetPosition);
    this->render.curMotion = this->unknown_0430;
    if (current == 4 || current == 2 || this->unknown_02C8 != 0 || current == 8) {
      this->SetAction(0x20, 4, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_02CC = 0;
    break;
  case 1:
    this->unknown_0178 = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    this->unknown_0430 = 0;
    CGameActor::Slot15(1, resetPosition);
    break;
  case 2: {
    this->unknown_0178 = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    const bool weaponAllowed = IsJobWeaponAllowed(
      this->GetEffectiveJob(-1), this->unknown_0260,
      (static_cast<std::int64_t>(this->unknown_0444) << 32) | static_cast<std::uint32_t>(this->unknown_0440)
    );
    CGameActor::Slot15(weaponAllowed ? 9 : 2, resetPosition);
    this->unknown_0378 = AttackMotionLength(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, weaponAllowed);
    bool usesArrow = false;
    g_luaScript->Call("IsItemUsingArrow", "d>b", this->unknown_0440, &usesArrow);
    if (usesArrow == true) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      (mode->*spawnArrow)(&this->render.position, this->hitTargetAid, this->unknown_0378 * this->render.speed, 0);
      this->unknown_0378 = 8.0f / this->render.speed + this->unknown_0378;
    }
    break;
  }
  case 6:
    this->Slot16();
    this->SetAction(0x10, 4, 1);
    this->render.isMotionFinished = true;
    this->render.curMotion = this->unknown_0430;
    break;
  case 7:
    if (current == 6) {
      return;
    }
    if (this->GetEffectiveJob(-1) == 0x13 || this->GetEffectiveJob(-1) == 0x14 || this->GetEffectiveJob(-1) == 0xE ||
        this->GetEffectiveJob(-1) == 0xFB4 || this->GetEffectiveJob(-1) == 0xFB5 || this->GetEffectiveJob(-1) == 0xFAF ||
        this->GetEffectiveJob(-1) == 0xFCA || this->GetEffectiveJob(-1) == 0xFCB || this->GetEffectiveJob(-1) == 0xFC5 ||
        this->GetEffectiveJob(-1) == 0xFE4 || this->GetEffectiveJob(-1) == 0xFEB || this->GetEffectiveJob(-1) == 0xFE5 ||
        this->GetEffectiveJob(-1) == 0xFEC || this->GetEffectiveJob(-1) == 0xFE9 || this->GetEffectiveJob(-1) == 0xFE2) {
      this->SetAction(0x20, 6, 1);
    } else if (this->GetEffectiveJob(-1) == 0xF || this->GetEffectiveJob(-1) == 0xFB0 || this->GetEffectiveJob(-1) == 0xFC6 ||
               this->GetEffectiveJob(-1) == 0xFE6 || this->GetEffectiveJob(-1) == 0xFED || this->GetEffectiveJob(-1) == 0x100A ||
               this->GetEffectiveJob(-1) == 0x10A6) {
      this->SetAction(0x60, 6, 1);
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 400, 0);
    } else if (this->GetEffectiveJob(-1) == 0x10A7 || this->GetEffectiveJob(-1) == 0x10A8) {
      this->SetAction(0x20, 6, 1);
    } else {
      this->SetAction(0x60, 6, 1);
    }
    this->unknown_0378 = 6.0f;
    break;
  case 8:
    this->Slot16();
    this->SetAction(0x60, 6, 1);
    this->render.isMotionFinished = true;
    break;
  case 9: {
    this->unknown_0178 = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    const bool weaponAllowed = IsJobWeaponAllowed(
      this->GetEffectiveJob(-1), this->unknown_0260,
      (static_cast<std::int64_t>(this->unknown_0444) << 32) | static_cast<std::uint32_t>(this->unknown_0440)
    );
    CGameActor::Slot15(weaponAllowed ? 9 : 2, resetPosition);
    this->unknown_0378 = AttackMotionLength(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, weaponAllowed);
    if ((this->unknown_0440 == 14 && this->unknown_0444 == 0) || (this->unknown_0440 == 13 && this->unknown_0444 == 0)) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      (mode->*spawnArrow)(&this->render.position, this->hitTargetAid, this->unknown_0378 * this->render.speed, 0);
      this->unknown_0378 = 8.0f / this->render.speed + this->unknown_0378;
    }
    break;
  }
  default:
    CGameActor::Slot15(state, resetPosition);
    if (this->render.unknown_0070 == 3) {
      this->PlayMotion(1);
    }
    return;
  }

  this->unknown_0F28 = this->Get8Dir(this->render.angle) + this->unknown_0F2C;
  this->render.unknown_0070 = state;
  this->render.unknown_008C = timeGetTime();
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
static __declspec(noinline) std::string GetBodyImfName(int job, int sex, int head, int costumeJob) {
  return g_equipmentResourceMgr->GetBodyImfName(job, sex, head, costumeJob);
}

// OFFSET: 0x00DB0500
// FUNCTION: fn_22617
void CGrannyPc::Slot30() {
  this->unknown_0410 = GetBodyImfName(
    this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0434, this->GetEffectiveJob(-1)
  );
}

// OFFSET: 0x00DB05E0
// FUNCTION: fn_22618
void CPc::Slot30() {
  int job = this->GetEffectiveJob(-1);

  if (static_cast<unsigned>(job - 0x10CE) < 0x1C && this->unknown_02CC != 0) {
    job = g_session.GetRidingJob(job);
  }

  this->unknown_0418 = GetBodyImfName(job, this->unknown_0260, this->unknown_0440, this->unknown_04D0);
}

// OFFSET: 0x00DAFB60
// FUNCTION: fn_22603
void CPc::LoadEquipAction(int slot, const char* name) {
  if (this->equipActions[slot]) {
    this->equipActions[slot]->Unlock();
  }

  const int job = this->render.accountId;
  const bool monster = IsMonsterJob(job);
  const bool hidden = monster && IsObfuscatedMessage(job);
  const bool male = this->unknown_0260 != 0;

  if (monster && slot == 6) {
    this->equipActions[slot] = nullptr;
  } else {
    if (monster && slot == 0) {
      if (hidden) {
        name = male ? "인간족\\몸통\\남\\운영자2_남.Act" : "인간족\\몸통\\여\\운영자2_여.Act";
      } else {
        name = male ? "인간족\\몸통\\남\\운영자_남.Act" : "인간족\\몸통\\여\\운영자_여.Act";
      }
    } else if (monster && slot == 5) {
      if (hidden) {
        name = male ? "인간족\\운영자\\운영자2_남_검.Act" : "인간족\\운영자\\운영자2_여_검.Act";
      } else {
        name = male ? "인간족\\운영자\\운영자_남_검.Act" : "인간족\\운영자\\운영자_여_검.Act";
      }
    }

    this->equipActions[slot] = ResourceMgr::Instance().Load(name);

    if (this->equipActions[slot]) {
      this->equipActions[slot]->Lock();
    }
  }

  if (this->equipActions.size() > 1) {
    CActRes* baseAction = static_cast<CActRes*>(this->equipActions[1]);

    if (baseAction) {
      const ActionFrame& baseFrame = baseAction->Frame(0, 0);
      const SpriteFrameCell& cell = baseFrame.Cell(1);
      const int baseJob = JobNames::BaseJob(this->GetEffectiveJob(-1));
      const int costumeJob = this->Slot46();
      const std::string actionName =
        g_equipmentResourceMgr->GetBodyResourceName(0, baseJob, this->unknown_0260, costumeJob, 0);
      CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));

      if (action) {
        const ActionFrame& actionFrame = action->Frame(0, 0);

        if (!actionFrame.anchors.empty() && !baseFrame.anchors.empty()) {
          this->unknown_04E4 = static_cast<float>(actionFrame.anchors[0].x - baseFrame.anchors[0].x + cell.x);
          this->unknown_04E8 = static_cast<float>(actionFrame.anchors[0].y - baseFrame.anchors[0].y + cell.y);
        }
      }
    }
  }
}

// OFFSET: 0x00DAFE10
// FUNCTION: fn_22605
void CPc::LoadEquipActions(const std::vector<std::string>& names) {
  for (int slot = 0; slot < 9; ++slot) {
    this->LoadEquipAction(slot, names[slot].c_str());
  }
}

// OFFSET: 0x00DB2230
// FUNCTION: fn_22641
void CPc::LoadEquipSprite(int slot, const char* name) {
  if (this->equipSprites[slot]) {
    this->equipSprites[slot]->Unlock();
  }

  const int job = this->render.accountId;
  const bool monster = IsMonsterJob(job);
  const bool hidden = monster && IsObfuscatedMessage(job);
  const bool male = this->unknown_0260 != 0;

  if (monster && slot == 6) {
    this->equipSprites[slot] = nullptr;
  } else {
    if (monster && slot == 0) {
      if (hidden) {
        name = male ? "인간족\\몸통\\남\\운영자2_남.Spr" : "인간족\\몸통\\여\\운영자2_여.Spr";
      } else {
        name = male ? "인간족\\몸통\\남\\운영자_남.Spr" : "인간족\\몸통\\여\\운영자_여.Spr";
      }
    } else if (monster && slot == 5) {
      if (hidden) {
        name = male ? "인간족\\운영자\\운영자2_남_검.Spr" : "인간족\\운영자\\운영자2_여_검.Spr";
      } else {
        name = male ? "인간족\\운영자\\운영자_남_검.Spr" : "인간족\\운영자\\운영자_여_검.Spr";
      }
    }

    this->equipSprites[slot] = ResourceMgr::Instance().Load(name);

    if (this->equipSprites[slot]) {
      this->equipSprites[slot]->Lock();
    }
  }
}

// OFFSET: 0x00DB2360
// FUNCTION: fn_22642
void CPc::LoadEquipSprites(const std::vector<std::string>& names) {
  for (int slot = 0; slot < 9; ++slot) {
    this->LoadEquipSprite(slot, names[slot].c_str());
  }
}

// OFFSET: 0x00DB23A0
// FUNCTION: fn_22643
void CPc::Slot15(int first, int second) {
  if (this->unknown_049C == first) {
    return;
  }

  if (static_cast<unsigned char>(second) != 0) {
    return;
  }

  this->unknown_049C = first;

  g_session.m_hotkeyScript->CallTable("TB_Layer_Priority", "IsHelmRobe", "d>b", first, &this->unknown_04D9);

  if (this->unknown_04D9) {
    const int accessory = this->unknown_049C;
    this->LoadEquipAction(
      8,
      g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, accessory).c_str()
    );
    this->LoadEquipSprite(
      8,
      g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, accessory).c_str()
    );
  } else {
    this->LoadEquipAction(
      8, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, 0).c_str()
    );
    this->LoadEquipSprite(
      8, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, 0).c_str()
    );
  }
}

// OFFSET: 0x00DA7F10
// FUNCTION: fn_22563
void CPc::AddMsgEffect(CGameObject* effect) {
  this->unknown_03B0.push_back(effect);
}

// OFFSET: 0x00DA8180
// FUNCTION: fn_22564
void CPc::Slot62(int value) {
  this->SetUnknown0260(value);
  this->Slot19(
    this->unknown_04D4, this->unknown_0260, this->unknown_0440, this->weapon, this->unknown_044C, this->unknown_0450, this->unknown_0454,
    this->unknown_0458, this->unknown_045C, this->unknown_0444, this->render.unknown_0048
  );
}

// OFFSET: 0x00DA8C80
// FUNCTION: fn_22566
void CPc::RenderBodyOutline(int layer, const float* position, float x, float y, float distanceScale, float scale, float depth, std::uint32_t color) {
  if (!g_renderer) {
    return;
  }

  RenderBounds bounds;
  bounds.left = static_cast<float>(g_renderer->viewport.width);
  bounds.top = static_cast<float>(g_renderer->viewport.height);
  bounds.right = 0.0f;
  bounds.bottom = 0.0f;
  bounds.offset = depth;

  CSprRes* sprite = static_cast<CSprRes*>(this->GetEquipSprite(0));
  CActRes* action = static_cast<CActRes*>(this->GetEquipAction(0));
  if (!sprite || !action) {
    return;
  }

  int motion;
  if (this->render.unknown_0070 == 3 || this->unknown_017C != 0) {
    this->render.previousAnimation = this->ActionBase(this->render.unknown_0070);
    motion = action->FrameCount(this->render.previousAnimation) - 1;
    this->render.curMotion = motion;
  } else {
    motion = this->render.curMotion;
  }

  const ActionFrame* frame = &action->Frame(this->render.previousAnimation, motion);
  if (!frame) {
    return;
  }

  if (frame->cellCount < 1) {
    frame = &action->Frame(0, this->render.curMotion);
    if (!frame) {
      return;
    }
  }

  for (int index = 0; index < frame->cellCount; ++index) {
    const SpriteFrameCell* cell = &frame->Cell(index);
    if (!cell || cell->image == -1) {
      continue;
    }

    SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
    float bottomRhw;
    float topRhw;
    SpriteQuad quad;
    int box[4];
    std::uint32_t quadColor;
    if (this->Slot55() == JT_THANATOS) {
      this->Slot09(index, index, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &quadColor, 0, 0, 0);
      if (this->render.unknown_0070 == 0 && index == 1) {
        quad.bottomRhw = bottomRhw + 0.000008f;
        quad.topRhw = topRhw + 0.00008f;
      }
    } else {
      this->Slot09(layer, layer, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &quadColor, 0, 0, 0);
      if (g_cameraDistance >= 320.0f) {
        quad.bottomRhw = bottomRhw - 0.0000025f;
        quad.topRhw = topRhw - 0.0000025f;
      }
    }

    this->RaiseUnknown03C8((box[3] - box[1]) / 4);
    quad.image = image;
    quad.color = color;
    if (this->render.unknown_0130[0] == 1) {
      quad.color = color & 0xFFFFFF | 0x46000000;
    }

    if (this->render.unknown_00A4Bytes[1] != 0) {
      bounds.Include(&quad.top);
    }

    if (IsScreenShakeActive()) {
      this->DrawFlippedQuad(quad, sprite->palette, 5, x, y, 0);
    } else {
      this->DrawQuad(quad, sprite->palette, 5, 0.0f, 0.0f, 0);
    }
  }
}

// OFFSET: 0x00DA8F70
// FUNCTION: fn_22567
bool CPc::IsDrawOnTop() {
  bool drawOnTop = true;
  if (g_session.m_newRobeDrawOnTop) {
    int job = this->GetEffectiveJob(-1);
    if (this->unknown_02CC) {
      job = g_session.GetRidingJob(job);
    }

    int layer = 1;
    g_session.m_hotkeyScript->Call(
      "_New_DrawOnTop", "ddddd>bd", this->unknown_049C, this->unknown_0260, job, this->render.previousAnimation, this->render.curMotion,
      &drawOnTop, &layer
    );
  } else {
    g_session.m_hotkeyScript->Call(
      "DrawOnTop", "ddddd>b", this->unknown_049C, this->unknown_0260, this->unknown_025C, this->render.previousAnimation,
      this->render.curMotion, &drawOnTop
    );
  }

  return drawOnTop;
}

// OFFSET: 0x00DA90E0
// FUNCTION: fn_22568
const ActionFrame& CPc::SelectStanceFrame(int layer, CActRes* actRes, int* animation, int* frame, bool* replaced) {
  const ActionFrame& current = actRes->Frame(this->render.previousAnimation, this->render.curMotion);
  if (this->unknown_02CC || layer < 1 || layer > 4 || this->render.previousAnimation < 88) {
    return current;
  }

  switch (this->GetEffectiveJob(-1)) {
    case JT_TAEKWON:
    case JT_STAR:
    case JT_STAR2:
    case JT_TAEKWON_B:
    case JT_STAR_B:
    case JT_STAR2_B:
    case JT_STAR_EMPEROR:
    case JT_STAR_EMPEROR_B:
    case JT_STAR2_EMPEROR:
    case JT_STAR2_EMPEROR_B:
    case JT_SKY_EMPEROR:
    case JT_SKY_EMPEROR2:
      break;
    default:
      return current;
  }

  if (this->render.previousAnimation >= 88 && this->render.previousAnimation <= 95 && this->render.curMotion >= 3 && this->render.curMotion <= 5) {
    if (this->unknown_0260 == 0) {
      if (this->render.curMotion != 3) {
        *replaced = true;
        if (this->render.previousAnimation <= 89) {
          if (this->render.curMotion == 4) {
            *animation = 6;
          } else if (this->render.curMotion == 5) {
            *animation = 5;
          }
        } else if (this->render.previousAnimation <= 91) {
          if (this->render.curMotion == 4) {
            *animation = 6;
          } else if (this->render.curMotion == 5) {
            *animation = 7;
          }
        } else if (this->render.previousAnimation <= 93) {
          if (this->render.curMotion == 4) {
            *animation = 2;
          } else if (this->render.curMotion == 5) {
            *animation = 1;
          }
        } else if (this->render.previousAnimation <= 95) {
          if (this->render.curMotion == 4) {
            *animation = 2;
          } else if (this->render.curMotion == 5) {
            *animation = 3;
          }
        }
      }
    } else if (this->unknown_0260 == 1) {
      if (this->render.curMotion != 5) {
        *replaced = true;
        if (this->render.previousAnimation <= 89) {
          if (this->render.curMotion == 3) {
            *animation = 6;
          } else if (this->render.curMotion == 4) {
            *animation = 5;
          }
        } else if (this->render.previousAnimation <= 91) {
          if (this->render.curMotion == 3) {
            *animation = 6;
          } else if (this->render.curMotion == 4) {
            *animation = 7;
          }
        } else if (this->render.previousAnimation <= 93) {
          if (this->render.curMotion == 3) {
            *animation = 2;
          } else if (this->render.curMotion == 4) {
            *animation = 1;
          }
        } else if (this->render.previousAnimation <= 95) {
          if (this->render.curMotion == 3) {
            *animation = 2;
          } else if (this->render.curMotion == 4) {
            *animation = 3;
          }
        }
      }
    }
  }

  if (this->render.previousAnimation >= 96 && this->render.curMotion == 1) {
    *replaced = true;
    if (this->render.previousAnimation >= 96 && this->render.previousAnimation <= 97) {
      *animation = 0;
    } else if (this->render.previousAnimation >= 98 && this->render.previousAnimation <= 99) {
      *animation = 6;
    } else {
      *animation = 2;
    }
  }

  if (*replaced == true) {
    return actRes->Frame(*animation, *frame);
  }

  return current;
}

// OFFSET: 0x00DA99A0
// FUNCTION: fn_22569
void CPc::ApplyLayerEffects(
  SpriteQuad* quad, const SpriteFrameCell* cell, int layer, const int* box, float shiftX, float distance, float bottomRhw, float topRhw,
  float screenX, float screenY, const std::uint32_t* palette, const SpriteRleImage* image, int* blend, int offsetX, int offsetY
) {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == 184) {
      if ((!this->render.visible || !this->render.enabled) && !StateFlag06(this->stateFlags)) {
        return;
      }
      break;
    }
  }

  if ((this->render.flags & 1) != 0 && layer < 5) {
    *blend = 5;
  }

  if ((this->render.flags & 2) != 0 && layer == 5) {
    *blend = 5;
  }

  if ((this->render.flags & 4) != 0 && layer == 5) {
    if (!this->render.unknown_00ECBytes[1]) {
      this->DrawQuad(*quad, palette, layer, 0.0f, 0.0f, 0);
      this->render.unknown_00ECBytes[1] = 1;
    } else {
      this->render.unknown_00ECBytes[1] = 0;
    }
  }

  short hover = 0;
  if (StateFlag15(this->Slot13()) == true) {
    hover = static_cast<short>(this->HoverHeight());
  }

  const std::int64_t hoverOffset = hover;
  this->render.unknown_0128 = static_cast<std::uint32_t>(hoverOffset);
  this->render.unknown_012C = static_cast<std::uint32_t>(hoverOffset >> 32);

  SpriteQuad jitter;
  if ((this->render.flags & 8) != 0 && layer < 5 && (this->render.flags & 0x30000) == 0) {
    if (++this->render.unknown_00ECBytes[2] > 180) {
      this->render.unknown_00ECBytes[2] = 0;
    }

    float wave = g_sinTable[this->render.unknown_00ECBytes[2]] + 3.0f;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    float spread = 400.0f / mode->camera->camera.desiredDistance * wave;
    jitter.top = static_cast<float>(box[1]) - spread - static_cast<float>(hover);
    jitter.bottom = static_cast<float>(box[3]) + spread - static_cast<float>(hover);
    float left = static_cast<float>(box[0]) + shiftX;
    float right = static_cast<float>(box[2]) + shiftX;
    if (left > right) {
      left += spread;
      right -= spread;
    } else {
      left -= spread;
      right += spread;
    }
    jitter.left = left;
    jitter.right = right;
    const float jitterBottomRhw = bottomRhw - 0.00005f;
    const float jitterTopRhw = topRhw - 0.00005f;
    jitter.bottomRhw = jitterBottomRhw;
    jitter.topRhw = jitterTopRhw;
    jitter.color = 0x64FFFFFF;
    jitter.angle = static_cast<float>(cell->unknown_001C + static_cast<int>(this->render.unknown_007C));
    jitter.image = image;
    this->DrawQuad(jitter, palette, 5, 0.0f, 0.0f, 0);

    wave = g_sinTable[this->render.unknown_00ECBytes[2]];
    mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    spread = 400.0f / mode->camera->camera.desiredDistance * (wave + wave + 5.0f);
    jitter.top = static_cast<float>(box[1]) - spread - static_cast<float>(hover);
    jitter.bottom = static_cast<float>(box[3]) + spread - static_cast<float>(hover);
    left = static_cast<float>(box[0]) + shiftX;
    right = static_cast<float>(box[2]) + shiftX;
    if (left > right) {
      left += spread;
      right -= spread;
    } else {
      left -= spread;
      right += spread;
    }
    jitter.left = left;
    jitter.right = right;
    jitter.bottomRhw = jitterBottomRhw;
    jitter.topRhw = jitterTopRhw;
    jitter.color = 0x78FFFFFF;
    jitter.angle = static_cast<float>(cell->unknown_001C + static_cast<int>(this->render.unknown_007C));
    jitter.image = image;
    this->DrawQuad(jitter, palette, 5, 0.0f, 0.0f, 0);
  }

  if ((this->render.flags & 0x80800) != 0 && (this->render.flags & 0x20000) == 0) {
    const float scale = this->render.scale;
    for (int step = 1, depth = 3; depth < 7; ++step, depth += 3) {
      const unsigned char alpha = this->render.unknown_00F2;
      const float distanceScale = g_renderer->ScaleDistance(distance);
      const float factor = GetSkillFactor(this->GetEffectiveJob(-1)) * distanceScale * (static_cast<float>(step) * 0.01f + 1.0f);
      const float shift =
        g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.unknown_0078)) * this->render.scale) * factor;
      const int boxTop = box[1];
      const float top = static_cast<float>(static_cast<double>(cell->y + offsetY) * factor * cell->scaleY * scale + screenY);
      const float bottom = static_cast<float>(
        static_cast<double>((static_cast<short>(image->unknown_0006) + 1) * image->height + cell->y + offsetY) * factor * cell->scaleY *
          scale +
        screenY + 1.0
      );

      double left;
      double right;
      if ((cell->mirrored & 1) != 0) {
        left = static_cast<double>((static_cast<short>(image->unknown_0004) + 1) * image->width + cell->x + offsetX) * factor * cell->scaleX;
        right = static_cast<double>(offsetX + cell->x) * factor * cell->scaleX;
        if (boxTop >= g_renderer->viewport.height || boxTop <= 0 || box[2] >= g_renderer->viewport.width || box[0] <= 0) {
          continue;
        }
      } else {
        if (boxTop >= g_renderer->viewport.height || box[3] <= 0 || box[0] >= g_renderer->viewport.width || box[2] <= 0) {
          continue;
        }
        left = static_cast<double>(cell->x + offsetX) * factor * cell->scaleX;
        right = static_cast<double>((static_cast<short>(image->unknown_0004) + 1) * image->width + cell->x + offsetX) * factor * cell->scaleX;
      }

      jitter.image = image;
      jitter.left = static_cast<float>(scale * left + screenX) + shift;
      jitter.right = static_cast<float>(scale * right + screenX + 1.0) + shift;
      jitter.top = top - static_cast<float>(hover);
      jitter.bottom = (static_cast<float>(boxTop) - top) * g_depthAverageHalf + bottom - static_cast<float>(hover);
      jitter.bottomRhw = static_cast<float>(depth) * 0.000005f + bottomRhw + 0.00001f;
      jitter.topRhw = static_cast<float>(depth) * 0.000005f + topRhw + 0.00001f;
      if ((this->render.flags & 0x800) != 0) {
        jitter.color = (alpha << 24) + 0x3737FF;
      } else if ((this->render.flags & 0x80000) != 0) {
        jitter.color = (alpha << 24) + 0x56405;
      }
      jitter.angle = static_cast<float>(cell->unknown_001C + static_cast<int>(this->render.unknown_007C));
      this->DrawQuad(jitter, palette, 5, 0.0f, 0.0f, 0);
    }
  }

  if ((this->render.flags & 0x30000) == 0) {
    this->Slot10(cell, box, shiftX, bottomRhw, topRhw, palette, image, 4.0f, hover);
    this->Slot11(quad, cell, box, shiftX, bottomRhw, topRhw, palette, image, *blend, hover);
  }

  this->Slot12(quad, box, shiftX, 11, 5);

  if ((this->render.flags & 0x10000) != 0) {
    const short level = this->render.unknown_00F4;
    const short alpha = static_cast<short>(255 - level * 20);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const float rise = 400.0f / mode->camera->camera.desiredDistance * static_cast<float>(level * 20);
    quad->top = static_cast<float>(box[1]) - rise;
    quad->bottom = static_cast<float>(box[3]) - rise;
    quad->color = (std::max<short>(alpha, 0) << 24) + 0xFFFFFF;
  }

  if ((this->render.flags & 0x20000) != 0) {
    const unsigned char phase = this->render.unknown_00F6;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    short alpha = static_cast<short>((this->render.unknown_00F6 - 10) * 17);
    if (alpha > 255) {
      alpha = 255;
    }
    const float rise = std::max(0.0f, 400.0f / mode->camera->camera.desiredDistance * static_cast<float>((25 - phase) * 20));
    quad->top = static_cast<float>(box[1]) - rise;
    quad->color = (std::max<short>(alpha, 0) << 24) + 0xFFFFFF;
    quad->bottom = static_cast<float>(box[3]) - rise;
  }

  if (hover != 0) {
    quad->top -= static_cast<float>(hover);
    quad->bottom -= static_cast<float>(hover);
  }

  if ((this->render.flags & 0x100000) != 0) {
    if (layer == 0 || layer > 4) {
      quad->color = 0x1E373737;
      *blend = 5;
    }

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const float sink = 24000.0f / mode->camera->camera.desiredDistance;
    quad->top = static_cast<float>(box[1]) + sink;
    quad->bottom = static_cast<float>(box[3]) + sink;
  }
}

// OFFSET: 0x00DB2840
// FUNCTION: fn_22647
void CPc::Slot24(int first, int second) {
  this->weapon = first;
  this->unknown_044C = second;

  int job = this->GetEffectiveJob(-1);
  if (this->IsCostumeOutfit()) {
    job = this->unknown_04D4;
  }

  const int viewJob = job > 3950 ? job - 3950 : job;

  if (this->unknown_02CC) {
    return;
  }

  this->LoadEquipAction(5, "");
  this->LoadEquipSprite(5, "");
  this->LoadEquipAction(6, "");
  this->LoadEquipSprite(6, "");

  switch (this->GetEffectiveJob(-1)) {
    case JT_TAEKWON:
    case JT_STAR:
    case JT_STAR2:
    case JT_TAEKWON_B:
    case JT_STAR_B:
    case JT_STAR2_B:
    case JT_STAR_EMPEROR:
    case JT_STAR_EMPEROR_B:
    case JT_STAR2_EMPEROR:
    case JT_STAR2_EMPEROR_B:
    case JT_SKY_EMPEROR:
    case JT_SKY_EMPEROR2: {
      if ((this->GetEffectiveJob(-1) == JT_TAEKWON || this->GetEffectiveJob(-1) == JT_TAEKWON_B) &&
          this->GetEffectiveJob(-1) != this->unknown_04D0) {
        return;
      }

      const std::string actName = g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, 0, -1, 1, this->unknown_04D0);
      const std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, 0, -1, 1, this->unknown_04D0);
      this->LoadEquipAction(6, actName.c_str());
      this->LoadEquipSprite(6, sprName.c_str());
      return;
    }
    case JT_WOLF_RANGER:
    case JT_WOLF_RANGER_H:
    case JT_WOLF_RANGER_B:
    case JT_WOLF_WINDHAWK: {
      const std::string actName = g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, 0, -1, 0, this->unknown_04D0);
      const std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, 0, -1, 0, this->unknown_04D0);
      this->LoadEquipAction(6, actName.c_str());
      this->LoadEquipSprite(6, sprName.c_str());
      return;
    }
    case JT_MADOGEAR:
    case JT_MADOGEAR_H:
    case JT_MADOGEAR_B: {
      std::string actName = g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, 0, -1, 0, this->unknown_04D0);
      std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, 0, -1, 0, this->unknown_04D0);
      if (this->Slot56() == 2) {
        std::size_t pos = actName.rfind("마도기어_");
        if (pos != std::string::npos) {
          actName.replace(pos, 9, "마도아머_", 9);
        }
        pos = sprName.rfind("마도기어_");
        if (pos != std::string::npos) {
          sprName.replace(pos, 9, "마도아머_", 9);
        }
      }
      this->LoadEquipAction(6, actName.c_str());
      this->LoadEquipSprite(6, sprName.c_str());
      return;
    }
    case JT_MEISTER_MADOGEAR: {
      std::string actName = g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, 0, -1, 0, this->unknown_04D0);
      std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, 0, -1, 0, this->unknown_04D0);
      if (this->Slot56() == 2) {
        std::size_t pos = actName.rfind("MEISTER_MADOGEAR1");
        if (pos != std::string::npos) {
          actName.replace(pos, 17, "MEISTER_MADOGEAR2", 17);
        }
        pos = sprName.rfind("MEISTER_MADOGEAR1");
        if (pos != std::string::npos) {
          sprName.replace(pos, 17, "MEISTER_MADOGEAR2", 17);
        }
      }
      this->LoadEquipAction(6, actName.c_str());
      this->LoadEquipSprite(6, sprName.c_str());
      return;
    }
  }

  const int weaponItem = this->weapon;
  const int shieldItem = this->unknown_044C;
  if (weaponItem == 0 && shieldItem == 0) {
    return;
  }

  if (GetWeaponViewValue(shieldItem) > 0) {
    int weaponType = this->GetDualWieldWeaponType(weaponItem, shieldItem);
    switch (this->GetEffectiveJob(-1)) {
      case JT_ASSASSIN:
      case JT_ASSASSIN_H:
      case JT_ASSASSIN_B:
      case JT_GUILLOTINE_CROSS:
      case JT_GUILLOTINE_CROSS_H:
      case JT_GUILLOTINE_CROSS_B:
      case JT_SHADOW_CROSS:
        if (weaponType == W_FIST || weaponType == W_2HAXE || weaponType == W_MACE) {
          weaponType = W_1HSWORD;
        }
        break;
    }

    const std::string actName = g_session.GetWeaponActName(viewJob, this->unknown_0260, weaponType, -1, this->unknown_04D0);
    const std::string sprName = g_session.GetWeaponSprName(viewJob, this->unknown_0260, weaponType, -1, this->unknown_04D0);
    this->LoadEquipAction(5, actName.c_str());
    this->LoadEquipSprite(5, sprName.c_str());

    switch (weaponType) {
      case W_DAGGER:
      case W_1HSWORD:
      case W_2HSWORD:
      case W_1HSPEAR:
      case W_2HSPEAR:
      case W_1HAXE:
      case W_2HAXE:
      case W_KATAR:
      case W_REVOLVER:
      case W_RIFLE:
      case W_DOUBLE_DD:
      case W_DOUBLE_SS:
      case W_DOUBLE_AA:
      case W_DOUBLE_DS:
      case W_DOUBLE_DA:
      case W_DOUBLE_SA: {
        const std::string effectActName =
          g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, weaponType, -1, 0, this->unknown_04D0);
        const std::string effectSprName =
          g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, weaponType, -1, 0, this->unknown_04D0);
        this->LoadEquipAction(6, effectActName.c_str());
        this->LoadEquipSprite(6, effectSprName.c_str());
        break;
      }
    }
    return;
  }

  int viewType = GetWeaponViewValue(this->weapon);
  const std::string actName = g_session.GetWeaponActName(viewJob, this->unknown_0260, viewType, weaponItem, this->unknown_04D0);
  if (ResourceMgr::Instance().Exists(actName.c_str())) {
    this->LoadEquipAction(5, actName.c_str());
    const std::string sprName = g_session.GetWeaponSprName(viewJob, this->unknown_0260, viewType, weaponItem, this->unknown_04D0);
    this->LoadEquipSprite(5, sprName.c_str());
    viewType = this->MapMotion(viewType);
  } else {
    viewType = GetWeaponViewValue(weaponItem);
    if (viewType == -1 || viewType == 0 || weaponItem == 1599) {
      return;
    }

    if (viewType == W_KNUCKLE) {
      switch (this->GetEffectiveJob(-1)) {
        case JT_PRIEST:
        case JT_PRIEST_H:
        case JT_PRIEST_B:
        case JT_ARCHBISHOP:
        case JT_ARCHBISHOP_H:
        case JT_ARCHBISHOP_B:
          return;
      }
    }

    if (this->GetEffectiveJob(-1) == JT_CARDINAL && viewType == W_KNUCKLE) {
      return;
    }

    viewType = this->MapMotion(viewType);
    const std::string fallbackActName = g_session.GetWeaponActName(viewJob, this->unknown_0260, viewType, -1, this->unknown_04D0);
    if (ResourceMgr::Instance().Exists(fallbackActName.c_str())) {
      this->LoadEquipAction(5, fallbackActName.c_str());
      const std::string fallbackSprName = g_session.GetWeaponSprName(viewJob, this->unknown_0260, viewType, -1, this->unknown_04D0);
      this->LoadEquipSprite(5, fallbackSprName.c_str());
    }
  }

  std::string effectActName = g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, viewType, weaponItem, 0, this->unknown_04D0);
  std::string effectSprName = g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, viewType, weaponItem, 0, this->unknown_04D0);
  if (!ResourceMgr::Instance().Exists(effectActName.c_str())) {
    effectActName = g_session.GetWeaponEffectActName(viewJob, this->unknown_0260, viewType, -1, 0, this->unknown_04D0);
    effectSprName = g_session.GetWeaponEffectSprName(viewJob, this->unknown_0260, viewType, -1, 0, this->unknown_04D0);
    if (!ResourceMgr::Instance().Exists(effectActName.c_str())) {
      return;
    }
  }

  this->LoadEquipAction(6, effectActName.c_str());
  this->LoadEquipSprite(6, effectSprName.c_str());
}

// OFFSET: 0x00DB3740
// FUNCTION: fn_22649
void CPc::Slot23(int first, int second) {
  this->weapon = first;
  this->unknown_044C = second;

  if (!g_session.IsJobWeaponTypeAllowed(this->GetEffectiveJob(-1), static_cast<std::int64_t>(second) << 32 | static_cast<std::uint32_t>(first))) {
    this->LoadEquipAction(5, "");
    this->LoadEquipSprite(5, "");
    this->LoadEquipAction(6, "");
    this->LoadEquipSprite(6, "");
    return;
  }

  const std::string actName = g_session.GetWeaponActName(this->GetEffectiveJob(-1), this->unknown_0260, this->weapon, -1, this->unknown_04D0);
  const std::string sprName = g_session.GetWeaponSprName(this->GetEffectiveJob(-1), this->unknown_0260, this->weapon, -1, this->unknown_04D0);
  this->LoadEquipAction(5, actName.c_str());
  this->LoadEquipSprite(5, sprName.c_str());

  switch (static_cast<std::int64_t>(this->unknown_044C) << 32 | static_cast<std::uint32_t>(this->weapon)) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_2HSWORD:
    case W_1HSPEAR:
    case W_2HSPEAR:
    case W_1HAXE:
    case W_2HAXE:
    case W_KATAR:
    case W_REVOLVER:
    case W_RIFLE:
    case W_DOUBLE_DD:
    case W_DOUBLE_SS:
    case W_DOUBLE_AA:
    case W_DOUBLE_DS:
    case W_DOUBLE_DA:
    case W_DOUBLE_SA: {
      const std::string effectActName =
        g_session.GetWeaponEffectActName(this->GetEffectiveJob(-1), this->unknown_0260, this->weapon, -1, 0, this->unknown_04D0);
      const std::string effectSprName =
        g_session.GetWeaponEffectSprName(this->GetEffectiveJob(-1), this->unknown_0260, this->weapon, -1, 0, this->unknown_04D0);
      this->LoadEquipAction(6, effectActName.c_str());
      this->LoadEquipSprite(6, effectSprName.c_str());
      break;
    }
    default:
      this->LoadEquipAction(6, "");
      this->LoadEquipSprite(6, "");
      break;
  }
}

// OFFSET: 0x00DB99F0
// FUNCTION: fn_22671
void CPlayer::Slot16() {
}

// OFFSET: 0x00DB07F0
// FUNCTION: fn_22627
void CPc::Slot26(int value) {
  this->unknown_0454 = value;
  this->LoadEquipAction(3, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, value).c_str());
  this->LoadEquipSprite(
    3, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, this->unknown_0454).c_str()
  );

  if (this->state.Attachments().HasId(678)) {
    this->SendMsg(nullptr, 109, 0, 678, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->SendMsg(nullptr, 109, 0, 679, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->SendMsg(nullptr, 109, 0, 680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DB09E0
// FUNCTION: fn_22629
void CPc::Slot27(int value) {
  this->unknown_0458 = value;
  this->LoadEquipAction(4, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, value).c_str());
  this->LoadEquipSprite(
    4, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, this->unknown_0458).c_str()
  );
}

// OFFSET: 0x00DB0B50
// FUNCTION: fn_22631
void CPc::Slot25(int value) {
  this->unknown_0450 = value;
  this->LoadEquipAction(2, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, value).c_str());
  this->LoadEquipSprite(
    2, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->unknown_0260, this->unknown_0440, this->unknown_0450).c_str()
  );
}

// OFFSET: 0x00DB1990
// FUNCTION: fn_22638
void CPc::Slot22(int value) {
  this->unknown_0440 = value;
  this->LoadEquipAction(1, g_session.GetHeadActName(this->unknown_04D4, this->unknown_0260, &this->unknown_0440).c_str());
  this->LoadEquipSprite(1, g_session.GetHeadSprName(this->unknown_04D4, this->unknown_0260, &this->unknown_0440).c_str());
}

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

// OFFSET: 0x00DAA2E0
// FUNCTION: fn_22570
void CGrannyPc::ReleaseMeshes() {
  for (int i = 0; i < 20; ++i) {
    if (this->controls[i]) {
      this->controls[i] = nullptr;
    }
  }

  for (int mesh = 0; mesh < 30; ++mesh) {
    RenderCommand* command = this->commands[mesh];
    if (command) {
      command->FreeVertices();
      delete command;
      this->commands[mesh] = nullptr;
      delete this->vertices[mesh];
      this->vertices[mesh] = nullptr;
    }
  }

  for (int layer = 0; layer < 10; ++layer) {
    for (int mesh = 0; mesh < 30; ++mesh) {
      RenderCommand* command = this->layerCommands[layer][mesh];
      if (command) {
        command->FreeVertices();
        delete command;
        this->layerCommands[layer][mesh] = nullptr;
        delete this->layerVertices[layer][mesh];
        this->layerVertices[layer][mesh] = nullptr;
      }
    }
  }

  this->unknown_2550 = 0;
}

// OFFSET: 0x00DAA3F0
// FUNCTION: fn_22571
std::vector<std::string> CPc::GetEquipActionNames() const {
  std::vector<std::string> names(9);
  for (int slot = 0; slot < 9; ++slot) {
    if (this->equipActions[slot]) {
      names[slot] = this->equipActions[slot]->name.text;
    } else {
      names[slot] = "";
    }
  }

  return names;
}

// OFFSET: 0x00DAA520
// FUNCTION: fn_22572
CRes* CPc::GetEquipAction(unsigned slot) const {
  if (slot < this->equipActions.size()) {
    return this->equipActions[slot];
  }

  return nullptr;
}

// OFFSET: 0x00DAA5F0
// FUNCTION: fn_22573
int GetSlotLayer(int slot) {
  switch (slot) {
    case 1:
      return 1;
  }

  return 0;
}

// OFFSET: 0x00DAA610
// FUNCTION: fn_22574
int CPc::Slot55() {
  return CGameActor::Slot55();
}

// OFFSET: 0x00DAAD80
// FUNCTION: fn_22577
int CPc::JobActionBase(int state) {
  int base = 0;
  switch (state) {
    case 0:
      base = 32;
      break;
    case 6:
      base = 16;
      break;
    case 7:
      switch (this->GetEffectiveJob(-1)) {
        case JT_CRUSADER:
        case JT_BARD:
        case JT_DANCER:
        case JT_GUNSLINGER:
        case JT_CRUSADER_B:
        case JT_BARD_B:
        case JT_DANCER_B:
        case JT_TAEKWON:
        case JT_STAR:
        case JT_STAR2:
        case JT_TAEKWON_B:
        case JT_STAR_B:
        case JT_GUNSLINGER_B:
        case JT_STAR2_B:
        case JT_STAR_EMPEROR:
        case JT_STAR_EMPEROR_B:
        case JT_STAR2_EMPEROR:
        case JT_STAR2_EMPEROR_B:
        case JT_SKY_EMPEROR:
        case JT_SKY_EMPEROR2:
          return 32;
        case JT_DO_SUMMONER:
        case JT_DO_CART_SUMMONER:
        case JT_DO_SUMMONER_B:
        case JT_DO_CART_SUMMONER_B:
        case JT_SPIRIT_HANDLER:
        case JT_SPIRIT_HANDLER_RIDING:
        case JT_DRUID:
        case JT_DRUID_B:
        case JT_KARNOS:
        case JT_KARNOS_B:
        case JT_ALITEA:
          return 80;
        case JT_MONK:
        case JT_NINJA:
        case JT_CRUSADER_H:
        case JT_MONK_H:
        case JT_BARD_H:
        case JT_DANCER_H:
        case JT_MONK_B:
        case JT_ROYAL_GUARD:
        case JT_SURA:
        case JT_ROYAL_GUARD_H:
        case JT_SURA_H:
        case JT_SURA_B:
        case JT_NINJA_B:
        case JT_INQUISITOR:
          return 96;
        default:
          return 96;
      }
    case 8:
      base = 96;
      switch (this->GetEffectiveJob(-1)) {
        case JT_DO_SUMMONER:
        case JT_DO_SUMMONER_B:
        case JT_SPIRIT_HANDLER:
          base = 40;
          break;
      }
      break;
  }

  return base;
}

// OFFSET: 0x00DAB010
// FUNCTION: fn_22578
std::vector<std::string> CPc::GetEquipSpriteNames() const {
  std::vector<std::string> names(9);
  for (int slot = 0; slot < 9; ++slot) {
    if (this->equipSprites[slot]) {
      names[slot] = this->equipSprites[slot]->name.text;
    } else {
      names[slot] = "";
    }
  }

  return names;
}

// OFFSET: 0x00DAB140
// FUNCTION: fn_22579
CRes* CPc::GetEquipSprite(int slot) const {
  return this->equipSprites[slot];
}

// OFFSET: 0x00DAB160
// FUNCTION: fn_22580
std::int64_t CGrannyPc::Slot35() {
  return (static_cast<std::int64_t>(this->unknown_0444) << 32) | static_cast<std::uint32_t>(this->unknown_0440);
}

// OFFSET: 0x00DAB250
// FUNCTION: fn_22582
bool CPc::IsCostumeOutfit() const {
  return StateFlag12(this->stateFlags) || StateFlag16(this->stateFlags) || StateFlag18(this->stateFlags) ||
    StateFlag27(this->stateFlags) || StateFlag28(this->stateFlags) || this->unknown_03DC == 1;
}

// OFFSET: 0x00DAB460
// FUNCTION: fn_22584
int CPc::GetDualWieldWeaponType(int first, int second) {
  int result = W_FIST;
  int main = first;
  int sub = second;

  if (!first) {
    if (!second) {
      return result;
    }

    main = second;
    sub = 0;
  }

  if (main >= 1100 && main < 1150 || main >= 13400 && main < 13500 || main >= 500000 && main < 510000) {
    result = W_1HSWORD;
    if (sub >= 1100 && sub < 1150 || sub >= 13400 && sub < 13500 || sub >= 500000 && sub < 510000) {
      return W_DOUBLE_SS;
    }
    if (sub >= 1200 && sub < 1250 || sub >= 13000 && sub < 13100 || sub >= 28700 && sub < 28900 || sub >= 510000 && sub < 520000) {
      return W_DOUBLE_DS;
    }
    if (sub >= 1300 && sub < 1350 || sub >= 520000 && sub < 530000) {
      return W_DOUBLE_SA;
    }
  } else if (main >= 1200 && main < 1250 || main >= 13000 && main < 13100 || main >= 28700 && main < 28900 || main >= 510000 && main < 520000) {
    result = W_DAGGER;
    if (sub >= 1100 && sub < 1150 || sub >= 13400 && sub < 13500 || sub >= 500000 && sub < 510000) {
      return W_DOUBLE_DS;
    }
    if (sub >= 1200 && sub < 1250 || sub >= 13000 && sub < 13100 || sub >= 28700 && sub < 28900 || sub >= 510000 && sub < 520000) {
      return W_DOUBLE_DD;
    }
    if (sub >= 1300 && sub < 1350 || sub >= 520000 && sub < 530000) {
      return W_DOUBLE_DA;
    }
  } else if (main >= 1300 && main < 1350 || main >= 520000 && main < 530000) {
    result = W_1HAXE;
    if (sub >= 1300 && sub < 1350 || sub >= 520000 && sub < 530000) {
      return W_DOUBLE_AA;
    }
  }

  return result;
}

// OFFSET: 0x00DAB6A0
// FUNCTION: fn_22585
bool IsReadyOrAttackAction(int action) {
  if (action < 32 || action >= 40 && action < 80 || action >= 96) {
    return false;
  }

  return true;
}

// OFFSET: 0x00DABE20
// FUNCTION: fn_22588
void CGrannyPc::UpdateAnimation(C3dGrannyModelRes* model) {
  if (!model) {
    model = this->model;
  }

  this->clock = GrannyGetSystemSeconds();
  this->unknown_0F24 += GrannyGetSecondsElapsed(&this->lastClock, &this->clock) * 2.0f;
  this->lastClock = this->clock;

  if (model != this->model) {
    model->PlayAnimation(this->motion, GrannyInstantiateModel(model->model), nullptr, 0);
    model->Animate(this->unknown_0F24, GrannyInstantiateModel(model->model));
    return;
  }

  if (this->motion == 2 && this->controls[2] && GrannyGetControlDurationLeft(this->controls[2]) <= 0.0f) {
    this->motion = 0;
    this->controls[this->motion] = model->PlayAnimation(this->motion, GrannyInstantiateModel(model->model), this->controls[this->motion], 0);
  }

  model->Animate(this->unknown_0F24 * g_depthAverageHalf, this->instance);
}

// OFFSET: 0x00DAC030
// FUNCTION: fn_22591
void CGrannyPc::Slot08() {
  if (this->render.unknown_0070 == 2) {
    this->render.isMotionFinished = GrannyGetControlDurationLeft(this->controls[2]) < 0.1f;
  } else {
    this->render.isMotionFinished = true;
  }
}

// OFFSET: 0x00DAB210
// FUNCTION: fn_22581
bool CPc::HasAppearanceOverride() {
  if (this->Slot55() > 0) {
    return true;
  }

  if (g_session.IsCostumeJob(this->GetEffectiveJob(-1)) == true) {
    return false;
  }

  return this->HasShapeShift();
}

// OFFSET: 0x00DACF90
// FUNCTION: fn_22597
void CGrannyPc::RenderCached(const matrix& view, int layer, C3dGrannyModelRes* model) {
  if (!this->render.visible || this->render.accountId == 0xFFFFFFFF) {
    return;
  }

  this->brightness = (this->alpha << 24) + 0xFFFFFF;
  g_modeMgr.GetGameMode();
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  bool attached = false;
  matrix rotation;
  rotation.MakeXRotation(1.570796f);
  rotation.AppendYRotation((this->angle + 180.0f) / 180.0f * 3.141592f);
  rotation.AppendScale(0.3f, 0.3f, 0.3f);
  vector3d position;
  position.x = this->render.position.x - 2.0f;
  position.y = this->render.position.y;
  position.z = this->render.position.z - 2.0f;
  rotation.AppendTranslate(&position.x);

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, view.values, &screenX, &screenY, &reciprocalDepth);

  float bounds[5];
  bounds[0] = static_cast<float>(g_renderer->viewport.width);
  bounds[1] = static_cast<float>(g_renderer->viewport.height);
  bounds[2] = 0.0f;
  bounds[3] = 0.0f;
  bounds[4] = reciprocalDepth;

  RenderCommand** commands = this->layerCommands[layer];
  if (!model) {
    model = this->model;
    commands = this->commands;
  }

  for (int mesh = 0; mesh < model->instance->meshCount; ++mesh) {
    const GrannyPNT332Vertex* source = reinterpret_cast<const GrannyPNT332Vertex*>(model->GetRigidVertices(mesh));
    if (!source) {
      if (layer == 0) {
        source = reinterpret_cast<const GrannyPNT332Vertex*>(this->vertices[mesh]);
      } else {
        source = reinterpret_cast<const GrannyPNT332Vertex*>(this->layerVertices[layer][mesh]);
      }

      if (!source) {
        continue;
      }
    }

    if (!this->meshTextures[layer][mesh]) {
      continue;
    }

    const matrix* transform;
    if (layer == this->attachLayer) {
      attached = true;
      transform = &this->attachTransform;
    } else {
      transform = &this->meshTransforms[mesh];
    }

    matrix local;
    local.Compose(*transform, rotation);
    matrix world;
    world.Compose(local, view);

    const std::uint16_t* indices = this->meshIndices[layer][mesh];
    RenderCommand* command = commands[mesh];
    int vertexCount = 0;
    for (int triangle = this->meshTriangleCounts[layer][mesh]; triangle > 0; --triangle) {
      for (int corner = 0; corner < 3; ++corner) {
        RenderVertex& output = command->vertices[vertexCount];
        const GrannyPNT332Vertex& vertex = source[indices[vertexCount]];
        ++vertexCount;
        position.x = vertex.position[0];
        position.y = vertex.position[1];
        position.z = vertex.position[2];
        if (attached == true && mesh > 0) {
          const float* values = this->meshTransforms[mesh].values;
          const vector3d corner = position;
          position.x = corner.x * values[0] + corner.y * values[3] + corner.z * values[6] + values[9];
          position.y = corner.y * values[4] + corner.x * values[1] + corner.z * values[7] + values[10];
          position.z = corner.y * values[5] + corner.x * values[2] + corner.z * values[8] + values[11];
        }

        g_renderer->viewport.Projection().Project(&position.x, world.values, output);
      }
    }

    command->texture = this->meshTextures[layer][mesh];
    command->vertexCount = this->meshTriangleCounts[layer][mesh] * 3;
    g_renderer->queues.Submit(command, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }

  if (model == this->model) {
    if (this->model->ProjectBounds(bounds, view, rotation, mode->camera->frame.eye)) {
      this->SetRenderInfo(bounds, screenX, screenY);
    } else {
      this->SetTlvert(screenX, screenY);
    }
  }
}

// OFFSET: 0x00DA88D0
// FUNCTION: fn_22565
void CPc::DrawBody(int, int layer, const float* position, float x, float y, float distanceScale, float scale, float depth) {
  RenderBounds bounds;
  bounds.left = static_cast<float>(g_renderer->viewport.width);
  bounds.top = static_cast<float>(g_renderer->viewport.height);
  bounds.right = 0.0f;
  bounds.bottom = 0.0f;
  bounds.offset = depth;

  CSprRes* sprite = static_cast<CSprRes*>(this->equipSprites[0]);
  CActRes* action = static_cast<CActRes*>(this->GetEquipAction(0));

  int motion;
  if (this->render.unknown_0070 == 3 || this->unknown_017C != 0) {
    this->render.previousAnimation = this->ActionBase(this->render.unknown_0070);
    motion = action->FrameCount(this->render.previousAnimation) - 1;
    this->render.curMotion = motion;
  } else {
    motion = this->render.curMotion;
  }

  const ActionFrame* frame = &action->Frame(this->render.previousAnimation, motion);
  if (frame->cellCount < 1) {
    frame = &action->Frame(0, this->render.curMotion);
  }

  for (int index = 0; index < frame->cellCount; ++index) {
    const SpriteFrameCell* cell = &frame->Cell(index);
    if (cell->image == -1) {
      continue;
    }

    SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
    float bottomRhw;
    float topRhw;
    SpriteQuad quad;
    int box[4];
    std::uint32_t color;
    if (this->Slot55() == JT_THANATOS) {
      this->Slot09(index, index, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, 0, 0, 0);
      if (this->render.unknown_0070 == 0 && index == 1) {
        quad.bottomRhw = bottomRhw + 0.000008f;
        quad.topRhw = topRhw + 0.00008f;
      }
    } else {
      this->Slot09(layer, layer, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, 0, 0, 0);
      if (g_cameraDistance <= 320.0f) {
        quad.bottomRhw = bottomRhw - 0.0000025f;
        quad.topRhw = topRhw - 0.0000025f;
      }
    }

    this->RaiseUnknown03C8((box[3] - box[1]) / 4);
    if (this->render.unknown_0130[0] == 1) {
      quad.color = quad.color & 0xFFFFFF | 0x46000000;
    }

    quad.image = image;
    if (this->render.unknown_00A4Bytes[1] != 0) {
      bounds.Include(&quad.top);
    }

    int blend = this->blend;
    const std::uint32_t* palette = sprite->palette;
    if (this->render.unknown_0048 != 0 && this->Slot55() < 0 && this->HasShapeShift() == true) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->render.name.c_str()))->palette;
    }

    this->ApplyLayerEffects(&quad, cell, 0, box, scale, bottomRhw, bottomRhw, topRhw, x, y, palette, image, &blend, 0, 0);
    if (IsScreenShakeActive()) {
      this->DrawFlippedQuad(quad, palette, 0x101, x, y, 0);
    } else {
      this->DrawQuad(quad, palette, 0x101, 0.0f, 0.0f, 0);
    }

    this->DrawStatusOverlays(quad, palette);
  }

  this->SetRenderInfo(&bounds.left, x, y);
  this->render.unknown_00ECBytes[0] = 1;
}

// OFFSET: 0x00DAD4D0
// FUNCTION: fn_22598
void CPc::RenderOverlaySprite(const float* matrix, int) {
  const float* origin = &this->render.position.x;
  float position[3];
  position[0] = origin[0] * matrix[0] + origin[1] * matrix[3] + origin[2] * matrix[6] + matrix[9];
  position[1] = origin[0] * matrix[1] + origin[1] * matrix[4] + origin[2] * matrix[7] + matrix[10];
  position[2] = origin[0] * matrix[2] + origin[1] * matrix[5] + origin[2] * matrix[8] + matrix[11];

  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(origin, matrix, &x, &y, &reciprocalDepth);
  const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);
  this->render.unknown_005C = distanceScale / g_heightScale;
  const float scale =
    g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.unknown_0078)) * this->render.scale) * this->render.unknown_005C;

  RenderBounds bounds;
  bounds.left = static_cast<float>(g_renderer->viewport.width);
  bounds.top = static_cast<float>(g_renderer->viewport.height);
  bounds.right = 0.0f;
  bounds.bottom = 0.0f;
  bounds.offset = reciprocalDepth;

  ResourceMgr::Instance().Load(this->unknown_0418.c_str());
  int count = 0;
  bool drawn = false;
  if (std::strlen(this->equipActions[0] ? this->equipActions[0]->name.text : "") == 0) {
    return;
  }

  if (std::strlen(this->equipSprites[0] ? this->equipSprites[0]->name.text : "") == 0) {
    return;
  }

  CActRes* action = static_cast<CActRes*>(this->GetEquipAction(0));
  CSprRes* sprite = static_cast<CSprRes*>(this->equipSprites[0]);
  if (this->render.curMotion >= action->FrameCount(this->render.previousAnimation)) {
    return;
  }

  const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);
  for (int index = 0; index < frame.cellCount; ++index) {
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    float bottomRhw;
    float topRhw;
    SpriteQuad quad;
    int box[4];
    std::uint32_t color;
    if (!this->Slot09(0, count, distanceScale, cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, 0, 0, 0)) {
      continue;
    }

    ++count;
    drawn = true;
    quad.image = image;
    if (this->render.unknown_00A4Bytes[1] != 0) {
      bounds.Include(&quad.top);
    }

    const std::uint32_t* palette;
    if (this->render.unknown_0048 != 0) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->render.name.c_str()))->palette;
    } else {
      palette = sprite->palette;
    }

    int effectId;
    this->SendMsg(nullptr, 120, 0, reinterpret_cast<int>(&effectId), reinterpret_cast<int>(&effectId) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->DrawQuad(quad, palette, 0x101, 0.0f, 0.0f, 0);
  }

  if (drawn) {
    this->SetRenderInfo(&bounds.left, x, y);
  } else {
    this->SetTlvert(x, y);
  }
}

// OFFSET: 0x00DAB980
// FUNCTION: fn_22586
bool CGrannyPc::OnProcess() {
  this->UpdateStateEffects();
  this->ProcessActorEffects();
  this->UpdateGaugePositions();
  this->UpdateGaugeWindow();

  if (this->render.isSingleColor == true && timeGetTime() > this->render.started + 100) {
    this->render.overrideColor = 0xFFFF0000;
    this->render.isSingleColor = false;
  }

  if (this->render.color != 0xFFFFFFFF) {
    this->ModifyArgb(8, 8, 8, 8);
  }

  union {
    void (CGameActor::*asVoid)();
    bool (CGameActor::*asBool)();
  } slot14;
  slot14.asVoid = &CGameActor::Slot14;
  const bool result = (this->*slot14.asBool)();

  this->PlayFrameSounds();

  if (this->unknown_0180 != 0) {
    const vector3d& player =
      reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      reinterpret_cast<const char*>(this->unknown_0190), this->render.position.x - player.x, 0.0f,
      this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    this->unknown_0180 = 0;
  }

  float facing = this->render.angle;
  if (facing > 360.0f) {
    facing = static_cast<float>(static_cast<int>(facing) % 360);
    this->render.angle = facing;
  }

  float angle;
  if (this->unknown_10A0 == 1) {
    angle = 180.0f - facing;
    this->unknown_10A0 = 0;
    this->angle = angle;
  } else {
    angle = this->angle;
    int delta = static_cast<int>(angle - static_cast<float>(static_cast<int>(180.0f - static_cast<float>(static_cast<int>(facing)))));
    if (delta > 360 || delta < -360) {
      delta %= 360;
    }

    const int distance = delta < 0 ? delta + 360 : delta;
    if (distance > 16) {
      if (distance < 180) {
        angle -= 16.0f;
      } else {
        angle += 16.0f;
      }
      this->angle = angle;
    }
  }

  if (angle > 360.0f) {
    angle = static_cast<float>(static_cast<int>(angle) % 360);
    this->angle = angle;
  }
  if (angle < 0.0f) {
    this->angle = angle + 360.0f;
  }

  return result;
}

// OFFSET: 0x00DABBE0
// FUNCTION: fn_22587
bool CPc::OnProcess() {
  this->UpdateStateEffects();
  this->ProcessActorEffects();
  this->UpdateGaugePositions();
  this->UpdateGaugeWindow();

  if (this->render.isSingleColor == true && timeGetTime() > this->render.started + 100) {
    this->render.overrideColor = 0xFFFF0000;
    this->render.isSingleColor = false;
  }

  if (this->render.color != 0xFFFFFFFF) {
    this->ModifyArgb(8, 8, 8, 8);
  }

  union {
    void (CGameActor::*asVoid)();
    bool (CGameActor::*asBool)();
  } slot14;
  slot14.asVoid = &CGameActor::Slot14;
  const bool result = (this->*slot14.asBool)();

  this->PlayFrameSounds();

  if (this->unknown_0180 != 0) {
    const vector3d& player =
      reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      reinterpret_cast<const char*>(this->unknown_0190), this->render.position.x - player.x, 0.0f,
      this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    this->unknown_0180 = 0;
  }

  const bool show = this->Slot55() > 0;
  if (!StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags) && !StateFlag01(this->stateFlags)) {
    if (show) {
      if (!this->msgEffect) {
        CMsgEffect* effect = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->SpawnMsgEffect(44, &this->render.position, this->render.accountId, true);
        this->AddMsgEffect(effect);
        effect->SendMsg(this, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->msgEffect = effect;
      }
      return result;
    }
  }

  if (this->msgEffect) {
    this->msgEffect->SendMsg(nullptr, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->msgEffect = nullptr;
  }

  return result;
}

// OFFSET: 0x00DAC580
// FUNCTION: fn_22595
void CPc::Render(int argument) {
  bool hasEffect = false;
  if (StateFlag14(this->stateFlags)) {
    static_cast<CPlayer*>(this)->UpdateFootprints(1);
  }
  if ((this->render.flags & 0x8000) != 0) {
    static_cast<CPlayer*>(this)->UpdateFootprints(2);
  }

  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == 0xB8) {
      hasEffect = true;
      break;
    }
  }

  this->render.unknown_0130[0] = 0;
  if (g_serviceOptions.Get(0x109) && this->render.accountId != g_session.m_aid) {
    this->render.unknown_0130[0] = 1;
  }

  const float* matrix = reinterpret_cast<const float*>(argument);
  if (hasEffect == true && (!this->render.visible || !this->render.enabled) && !StateFlag06(this->stateFlags)) {
    this->SetColor(-1, 0, 0, 0);
    this->DrawLayers(matrix, 0, 0);
    return;
  }

  if (!this->render.visible) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (this->render.unknown_0070 != 6 && this->render.unknown_0070 != 3) {
    this->DrawShadow(argument, this->render.scale);
  }
  if (this->render.unknown_0070 == 6) {
    switch (this->GetEffectiveJob(-1)) {
    case 0xFD0:
    case 0x108E:
    case 0x1093:
    case 0x1094:
    case 0x10DC:
      this->DrawShadow(argument, this->render.scale);
      break;
    }
  }

  if (!this->render.enabled) {
    this->UpdateTlvertex(matrix);
    return;
  }

  if (IsObfuscatedMessage(this->render.accountId)) {
    this->RenderOverlaySprite(matrix, 0);
    return;
  }

  if (this->unknown_0474 != 0) {
    this->DrawSprite(argument, false, 0.0f);
    return;
  }

  this->DrawLayers(matrix, 0, 0);
}

// OFFSET: 0x00DAD8C0
// FUNCTION: fn_22599
void CPc::DrawLayers(const float* matrix, int unknown0030, char unknown0034) {
  SpriteLayerRenderer renderer(this);
  renderer.Render(matrix, unknown0030, unknown0034);
}

// OFFSET: 0x00DABF50
// FUNCTION: fn_22589
void CGrannyPc::UpdateGaugeWindow() {
  if (reinterpret_cast<UIWindow*>(this->unknown_0478) != nullptr) {
    reinterpret_cast<UIWindow*>(this->unknown_0478)->Move(
      this->render.screenX - 30,
      static_cast<int>(static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.unknown_005C + static_cast<float>(this->render.screenY))
    );
  }
  this->UpdateHpGauge();
}

// OFFSET: 0x00DABFC0
// FUNCTION: fn_22590
void CPc::UpdateGaugeWindow() {
  if (reinterpret_cast<UIWindow*>(this->unknown_0490) != nullptr) {
    reinterpret_cast<UIWindow*>(this->unknown_0490)->Move(
      this->render.screenX - 30,
      static_cast<int>(static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.unknown_005C + static_cast<float>(this->render.screenY))
    );
  }
  this->UpdateHpGauge();
}

// OFFSET: 0x00DB1760
// FUNCTION: fn_22635
void CPc::Slot20(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) {
  this->unknown_04D4 = first;
  this->SetUnknown0260(second);
  this->unknown_0440 = third;
  this->weapon = fourth;
  this->unknown_044C = fifth;
  this->unknown_0450 = sixth;
  this->unknown_0454 = seventh;
  this->unknown_0458 = eighth;
  this->unknown_045C = ninth;
  this->unknown_0444 = tenth;
  this->render.unknown_0048 = eleventh;
  this->SetActorJob(first);

  int job = first;
  if (StateFlag12(this->stateFlags)) {
    job = 0x16;
  } else if (StateFlag16(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    job = 0x1a;
  } else if (StateFlag18(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    job = 0x1b;
  } else if (StateFlag27(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    job = 0x1c;
  } else if (StateFlag28(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    job = 0x1d;
  } else if (this->unknown_03DC == 1 && !StateFlag15(this->stateFlags)) {
    job = 0x1e;
  } else if (this->unknown_02CC != 0) {
    job = g_session.GetRidingJob(job);
  }

  this->SetActorJob(job);
  this->Slot22(this->unknown_0440);
  this->Slot23(this->weapon, this->unknown_044C);
  this->Slot25(this->unknown_0450);
  this->Slot26(this->unknown_0454);
  this->Slot27(this->unknown_0458);
  this->Slot28(this->unknown_045C);
  this->Slot30();
  this->Slot31();
  this->Slot33(this->render.unknown_0048);
  this->Slot32();
}

// OFFSET: 0x00DB0CE0
// FUNCTION: fn_22633
void CPc::Slot19(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) {
  this->unknown_04D4 = first;
  this->SetUnknown0260(second);
  this->unknown_0440 = third;
  this->weapon = fourth;
  this->unknown_044C = fifth;
  this->unknown_0450 = sixth;
  this->unknown_0454 = seventh;
  this->unknown_0458 = eighth;
  this->unknown_045C = GetWeaponViewValue(fifth) < 0 ? fifth : 0;
  this->unknown_0444 = ninth;
  this->render.unknown_0048 = tenth;
  this->render.unknown_0048 = g_session.GetBodyStyle(first, tenth, this->unknown_04D0);

  int job = first;
  bool applyJob = true;
  if (StateFlag12(eleventh)) {
    job = 0x16;
  } else if (StateFlag16(eleventh) && !StateFlag15(eleventh)) {
    job = 0x1a;
  } else if (StateFlag18(eleventh) && !StateFlag15(eleventh)) {
    job = 0x1b;
  } else if (StateFlag27(eleventh) && !StateFlag15(eleventh)) {
    job = 0x1c;
  } else if (StateFlag28(eleventh) && !StateFlag15(eleventh)) {
    job = 0x1d;
  } else if (this->unknown_03DC == 1 && !StateFlag15(this->stateFlags)) {
    job = 0x1e;
  } else if (this->HasShapeShift()) {
    applyJob = false;
    this->Slot21(this->GetUnknown0404Job());
  } else if (this->unknown_02CC != 0) {
    job = g_session.GetRidingJob(first);
  }

  if (applyJob) {
    this->SetActorJob(job);
  }

  this->Slot22(this->unknown_0440);

  if (!StateFlag12(eleventh) && !StateFlag16(eleventh) && !StateFlag18(eleventh) && !StateFlag27(eleventh) &&
      !StateFlag28(eleventh) && this->unknown_03DC != 1 && this->unknown_02CC == 0) {
    switch (this->GetEffectiveJob(-1)) {
      case 0xC:
      case 0xFAD:
      case 0xFC3:
      case 0xFDB:
      case 0xFE1:
      case 0x1005:
      case 0x109E:
        this->Slot24(fourth, fifth);
        break;
      default:
        this->Slot24(fourth, fourth >> 31);
        break;
    }
  }

  this->Slot25(this->unknown_0450);
  this->Slot26(this->unknown_0454);
  this->Slot27(this->unknown_0458);

  if (!StateFlag12(eleventh) && !StateFlag16(eleventh) && !StateFlag18(eleventh) && !StateFlag27(eleventh) &&
      !StateFlag28(eleventh) && this->unknown_03DC != 1 && this->unknown_02CC == 0) {
    this->Slot28(this->unknown_045C);
  }

  this->Slot30();
  this->Slot31();
  this->Slot32();
}

// OFFSET: 0x00DB1AE0
// FUNCTION: fn_22640
void CPc::Slot21(int job) {
  if (job == JT_BARD || job == JT_BARD_H || job == JT_BARD_B) {
    if (this->unknown_0260 == 0) {
      this->unknown_0260 = 1;
    }
  } else if (job == JT_DANCER || job == JT_DANCER_H || job == JT_DANCER_B) {
    if (this->unknown_0260 == 1) {
      this->unknown_0260 = 0;
    }
  } else if (job == JT_MINSTREL) {
    if (this->unknown_0260 == 0) {
      this->unknown_0260 = 1;
    }
  } else if (job == JT_WANDERER) {
    if (this->unknown_0260 == 1) {
      this->unknown_0260 = 0;
    }
  } else if (job == JT_MINSTREL_H) {
    if (this->unknown_0260 == 0) {
      this->unknown_0260 = 1;
    }
  } else if (job == JT_WANDERER_H) {
    if (this->unknown_0260 == 1) {
      this->unknown_0260 = 0;
    }
  } else if (job == JT_TROUBADOUR) {
    if (this->unknown_0260 == 0) {
      this->unknown_0260 = 1;
    }
  } else if (job == JT_TROUVERE) {
    if (this->unknown_0260 == 1) {
      this->unknown_0260 = 0;
    }
  }

  this->render.unknown_0054 = g_session.GetNpcLocationRadius(job, this->unknown_02CC == 1);

  if (this->Slot55() > 0) {
    this->LoadEquipAction(0, g_session.GetMonsterActName(this->Slot55()).c_str());
    this->LoadEquipSprite(0, g_session.GetMonsterSprName(this->Slot55()).c_str());
  } else if (StateFlag12(this->stateFlags) == true || job == JT_MARRIED) {
    this->LoadEquipAction(0, g_session.GetRaceBodyResourceName(0, this->unknown_04D4, JT_MARRIED, this->unknown_0260, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetRaceBodyResourceName(1, this->unknown_04D4, JT_MARRIED, this->unknown_0260, 0).c_str());
  } else if (StateFlag16(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, GetBodyActionResourceName(JT_SANTA, this->unknown_0260, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_SANTA, this->unknown_0260, 0, 0).c_str());
  } else if (StateFlag18(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, GetBodyActionResourceName(JT_SUMMER, this->unknown_0260, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_SUMMER, this->unknown_0260, 0, 0).c_str());
  } else if (StateFlag27(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, GetBodyActionResourceName(JT_HANBOK, this->unknown_0260, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_HANBOK, this->unknown_0260, 0, 0).c_str());
  } else if (StateFlag28(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, GetBodyActionResourceName(JT_OKTOBERFEST, this->unknown_0260, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_OKTOBERFEST, this->unknown_0260, 0, 0).c_str());
  } else if (this->unknown_03DC == 1 && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, GetBodyActionResourceName(JT_SUMMER2, this->unknown_0260, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_SUMMER2, this->unknown_0260, 0, 0).c_str());
  } else if (this->unknown_02CC != 0) {
    this->LoadEquipAction(
      0, GetBodyActionResourceName(g_session.GetRidingJob(job), this->unknown_0260, this->unknown_04D0, 0).c_str()
    );
    this->LoadEquipSprite(0, g_session.GetBodySprName(g_session.GetRidingJob(job), this->unknown_0260, this->unknown_04D0, 0).c_str());
  } else {
    const int effectiveJob = this->GetEffectiveJob(JobNames::BaseJob(job));
    switch (effectiveJob) {
      case JT_MADOGEAR:
      case JT_MADOGEAR_H:
      case JT_MEISTER_MADOGEAR:
        if (this->Slot56() == 2) {
          this->render.unknown_0054 = 1.5f;
        }
        break;
      case JT_MADOGEAR_B:
        if (this->Slot56() == 2) {
          this->render.unknown_0054 = 1.12f;
        }
        break;
    }

    this->LoadEquipAction(
      0, GetBodyActionResourceName(effectiveJob, this->unknown_0260, this->unknown_04D0, this->Slot56()).c_str()
    );
    this->LoadEquipSprite(0, g_session.GetBodySprName(effectiveJob, this->unknown_0260, this->unknown_04D0, this->Slot56()).c_str());
  }

  this->SetActionName(ResourceMgr::Instance(), this->equipActions[0] ? this->equipActions[0]->GetName() : "");
  this->SetSpriteName(ResourceMgr::Instance(), this->equipSprites[0] ? this->equipSprites[0]->GetName() : "");
  this->Slot30();
}

// OFFSET: 0x00DB4E70
// FUNCTION: fn_22653
__declspec(noinline) void CPlayer::UpdateMsgEffect(bool show) {
  if (!StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags) && !StateFlag01(this->stateFlags)) {
    if (show) {
      if (!this->msgEffect) {
        CMsgEffect* effect = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->SpawnMsgEffect(44, &this->render.position, this->render.accountId, true);
        this->AddMsgEffect(effect);
        effect->SendMsg(this, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->msgEffect = effect;
      }
      return;
    }
  }

  if (this->msgEffect) {
    this->msgEffect->SendMsg(nullptr, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->msgEffect = nullptr;
  }
}

// OFFSET: 0x00DB0FC0
// FUNCTION: fn_22634
void CGrannyPc::Slot19(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) {
  this->SetUnknown0260(second);
  this->unknown_0434 = third;
  this->unknown_0440 = fourth;
  this->unknown_0448 = sixth;
  this->unknown_044C = seventh;
  this->unknown_0450 = eighth;
  this->unknown_0454 = ninth;
  this->unknown_0444 = fifth;
  this->unknown_0438 = tenth;
  this->render.unknown_0048 = eleventh;
  this->SetActorJob(first);
  this->Slot22(this->unknown_0434);
  this->Slot23(this->unknown_0440, this->unknown_0444);
  this->Slot25(this->unknown_0448);
  this->Slot26(this->unknown_044C);
  this->Slot27(this->unknown_0450);
  this->Slot28(this->unknown_0454);
  this->Slot30();
  this->Slot31();

  if (this->model) {
    this->ReleaseMeshes();
  }

  std::strcpy(this->modelName, "SWM@hd기본_3.gr2");
  this->model = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->instance = GrannyInstantiateModel(this->model->model);
  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->commands[mesh] = reinterpret_cast<RenderCommand*>(command);
    reinterpret_cast<RenderCommand*>(command)->AllocateTriangles(this->model->GetTriangleCount(mesh));
    this->vertices[mesh] = new RenderVertex[this->model->GetTriangleCount(mesh) * 3];
    std::memset(this->vertices[mesh], 0, this->model->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  this->unknown_0F24 = 0.0f;
  this->lastClock = GrannyGetSystemSeconds();
  this->unknown_02E0 = 0;

  char* tag = std::strstr(this->modelName, "@hd");
  if (!tag) {
    return;
  }

  tag[1] = 'h';
  tag[2] = 'r';
  char name[MAX_PATH];
  std::strcpy(name, tag + 1);
  int length = 0;
  while (name[length] != '_') {
    ++length;
  }

  name[length + 1] = '9';
  name[length + 2] = '9';
  name[length + 3] = '\0';
  std::strcat(name, ".gr2");
  this->layerModels[1] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(name));
  for (int mesh = 0; mesh < this->layerModels[1]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[1][mesh] = reinterpret_cast<RenderCommand*>(command);
    reinterpret_cast<RenderCommand*>(command)->AllocateTriangles(this->layerModels[1]->GetTriangleCount(mesh));
    this->layerVertices[1][mesh] = new RenderVertex[this->layerModels[1]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[1][mesh], 0, this->layerModels[1]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  tag[1] = 'b';
  tag[2] = 'd';
  this->layerModels[2] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->layerModels[2]->RebindMeshes(*this->model);
  for (int mesh = 0; mesh < this->layerModels[2]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[2][mesh] = reinterpret_cast<RenderCommand*>(command);
    reinterpret_cast<RenderCommand*>(command)->AllocateTriangles(this->layerModels[2]->GetTriangleCount(mesh));
    this->layerVertices[2][mesh] = new RenderVertex[this->layerModels[2]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[2][mesh], 0, this->layerModels[2]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  tag[1] = 'p';
  tag[2] = 't';
  this->layerModels[3] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->layerModels[3]->RebindMeshes(*this->model);
  for (int mesh = 0; mesh < this->layerModels[3]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[3][mesh] = reinterpret_cast<RenderCommand*>(command);
    reinterpret_cast<RenderCommand*>(command)->AllocateTriangles(this->layerModels[3]->GetTriangleCount(mesh));
    this->layerVertices[3][mesh] = new RenderVertex[this->layerModels[3]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[3][mesh], 0, this->layerModels[3]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  tag[1] = 'l';
  tag[2] = 'e';
  this->layerModels[4] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->layerModels[4]->RebindMeshes(*this->model);
  for (int mesh = 0; mesh < this->layerModels[4]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[4][mesh] = reinterpret_cast<RenderCommand*>(command);
    reinterpret_cast<RenderCommand*>(command)->AllocateTriangles(this->layerModels[4]->GetTriangleCount(mesh));
    this->layerVertices[4][mesh] = new RenderVertex[this->layerModels[4]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[4][mesh], 0, this->layerModels[4]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  if (this->layerModels[1]) {
    this->model->FindHairPivot();
    this->attachLayer = 1;
  }
}

// OFFSET: 0x00DB0040
// FUNCTION: fn_22609
void CPc::Slot32() {
  if (this->render.unknown_0048 == 0) {
    return;
  }

  if (this->Slot55() < 0 && this->IsShapeShifted()) {
    std::string paletteName;
    this->render.name.assign(FormatOwnedString("몸\\%s_%d.pal", this->GetShapeShiftName().c_str(), this->render.unknown_0048));
    return;
  }

  int job = this->GetEffectiveJob(-1);
  const int costumeJob = this->unknown_04D0;
  if (this->unknown_02CC != 0) {
    job = g_session.GetRidingJob(job);
  }

  this->render.name.assign(g_session.GetBodyPalName(job, this->unknown_0260, this->render.unknown_0048, costumeJob, this->Slot56(), 0));
}

// OFFSET: 0x00DB2670
// FUNCTION: fn_22645
void CPc::Slot28(int value) {
  this->unknown_045C = value;

  if (this->unknown_02CC != 0) {
    this->LoadEquipAction(7, "");
    this->LoadEquipSprite(7, "");
    return;
  }

  const int job = this->GetEffectiveJob(-1);
  this->LoadEquipAction(7, g_session.GetShieldActName(job, this->unknown_0260, this->unknown_045C, this->unknown_04D0).c_str());
  this->LoadEquipSprite(7, g_session.GetShieldSprName(job, this->unknown_0260, this->unknown_045C, this->unknown_04D0).c_str());
}

// OFFSET: 0x00DB6670
// FUNCTION: fn_22668
void CPlayer::ApplyFollowOffset(CGameActor* target, float x, float z, int* row, int* column) {
  union {
    void (CGameActor::*asVoid)();
    int (CGameActor::*asInt)();
  } slot45;
  slot45.asVoid = &CGameActor::Slot45;
  int step = (target->*slot45.asInt)();
  if (target->unknown_02E0 == 1) {
    step = 5;
  } else if (step == 0) {
    return;
  }

  float angle = static_cast<float>(std::atan2(
    static_cast<double>(x - this->render.position.x), static_cast<double>(-(z - this->render.position.z))
  )) / 3.141592f * 180.0f;
  if (angle >= 360.0f) {
    angle -= 360.0f;
  }
  if (angle < 0.0f) {
    angle += 360.0f;
  }

  if (angle >= 0.0f && angle < 25.0f) {
    *row += step;
  } else if (angle >= 25.0f && angle < 70.0f) {
    *column += step;
    *row += step;
  } else if (angle >= 70.0f && angle < 115.0f) {
    *column += step;
  } else if (angle >= 115.0f && angle < 160.0f) {
    *column += step;
    *row -= step;
  } else if (angle >= 160.0f && angle < 205.0f) {
    *row -= step;
  } else if (angle >= 205.0f && angle < 250.0f) {
    *column -= step;
    *row += step;
  } else if (angle >= 250.0f && angle < 295.0f) {
    *column -= step;
  } else if (angle >= 295.0f && angle < 340.0f) {
    *column -= step;
    *row -= step;
  } else {
    *row += step;
  }
}

// OFFSET: 0x00DB6820
// FUNCTION: fn_22669
float CPlayer::AdjustFollowDistance(CGameActor* target, float distance) {
  if (target->unknown_02E0 == 1) {
    return std::max(0.0f, distance - 20.0f);
  }

  union {
    void (CGameActor::*asVoid)();
    int (CGameActor::*asInt)();
  } slot45;
  slot45.asVoid = &CGameActor::Slot45;
  if ((target->*slot45.asInt)() <= 0) {
    return std::max(0.0f, distance);
  }

  return std::max(0.0f, distance - static_cast<float>((target->*slot45.asInt)() + 1) * 4.0f);
}

// GLOBAL: gl_25248
bool g_refreshWindow289;

struct PlayerMoveRequest {
  std::uint32_t serverTime;
  int column;
  int row;
  int subColumn;
  int subRow;
  int destinationX;
  int destinationY;
};

// OFFSET: 0x00DB5710
// FUNCTION: fn_22666
bool CPlayer::OnProcess() {
  this->ProcessBodyState();
  this->UpdateStateEffects();
  this->ProcessActorEffects();
  this->UpdateGaugePositions();

  if (this->render.isSingleColor == true && timeGetTime() > this->render.started + 100) {
    this->render.overrideColor = 0xFFFF0000;
    this->render.isSingleColor = false;
  }

  if (this->render.color != 0xFFFFFFFF) {
    this->ModifyArgb(8, 8, 8, 8);
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (this->unknown_050C != 0 && timeGetTime() > (g_session.m_averagePing >> 1) + this->unknown_0504) {
    PlayerMoveRequest request;
    reinterpret_cast<const ::TerrainHeight*>(mode)->WorldSubcell(
      this->render.position.x, this->render.position.z, request.column, request.row, request.subColumn, request.subRow
    );
    request.serverTime = g_session.GetServerTime();
    request.destinationX = reinterpret_cast<const int&>(this->unknown_04F8);
    request.destinationY = this->unknown_04FC;
    const int address = reinterpret_cast<int>(&request);
    this->SendMsg(nullptr, 0, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  this->UpdatePendingAction();

  union {
    void (CGameActor::*asVoid)();
    bool (CGameActor::*asBool)();
  } slot14;
  slot14.asVoid = &CGameActor::Slot14;
  const bool result = (this->*slot14.asBool)();

  this->PlayFrameSounds();

  if (this->unknown_0180 != 0) {
    const vector3d& player =
      reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      reinterpret_cast<const char*>(this->unknown_0190), this->render.position.x - player.x, 0.0f,
      this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    this->unknown_0180 = 0;
  }

  this->UpdateMsgEffect(this->Slot55() > 0);

  if (g_refreshWindow289) {
    UIWindow* window = g_windowMgr.LookupWindow(289);
    if (window != nullptr) {
      window->Invalidate();
    }
  }

  return result;
}

// OFFSET: 0x00DB5980
// FUNCTION: fn_22667
void CPlayer::UpdatePendingAction() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  switch (this->unknown_0508) {
  case 1:
  case 5: {
    CGameActor* target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(this->unknown_051C));
    if (target == nullptr) {
      break;
    }
    if (target->render.unknown_0070 == 3) {
      return;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    if (this->GetEffectiveJob(-1) == 16 || this->GetEffectiveJob(-1) == 4017 || this->GetEffectiveJob(-1) == 4039) {
      const int state = static_cast<int>(this->render.unknown_0070);
      if (state != 1 && state != 0 && state != 4 && state != 7 && state != 8) {
        return;
      }
    } else if ((this->GetEffectiveJob(-1) >= 4046 && this->GetEffectiveJob(-1) < 4361) || this->GetEffectiveJob(-1) == 24 ||
               this->GetEffectiveJob(-1) == 25) {
      const int state = static_cast<int>(this->render.unknown_0070);
      if (state != 1 && state != 0 && state != 4 && state != 7 && state != 8 && state < 16) {
        return;
      }
    } else {
      const int state = static_cast<int>(this->render.unknown_0070);
      if (state != 1 && state != 0 && state != 4) {
        return;
      }
    }

    const float distance = this->AdjustFollowDistance(
      target, target->CalcDist(this->render.position.x, this->render.position.y, this->render.position.z)
    );
    const bool inRange = static_cast<float>(g_session.unknown_0F4C * 5) + 5.5f >= distance;
    if ((reinterpret_cast<const int&>(this->unknown_04F8) != column || this->unknown_04FC != row) && !inRange) {
      const std::uint32_t delay = this->unknown_0508 != 5 ? 450 : 1200;
      const bool elapsed = timeGetTime() > this->unknown_0524 + delay;
      const int state = static_cast<int>(this->render.unknown_0070);
      int cellX = target->unknown_0158[5];
      int cellY = *reinterpret_cast<const int*>(target->unknown_0170);
      float x;
      float z;
      if (this->unknown_0508 == 5) {
        x = target->render.position.x;
        z = target->render.position.z;
      } else {
        reinterpret_cast<const ::TerrainHeight*>(mode)->CellCenter(cellX, cellY, x, z);
      }
      this->ApplyFollowOffset(target, x, z, &cellY, &cellX);
      mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, cellX, cellY, g_session.unknown_0F4C);
      if ((reinterpret_cast<const int&>(this->unknown_04F8) != cellX || this->unknown_04FC != cellY) && state != 2 &&
          this->unknown_0514 == 0 && elapsed) {
        reinterpret_cast<int&>(this->unknown_04F8) = cellX;
        this->unknown_04FC = cellY;
        this->unknown_0524 = timeGetTime();
        this->SendMsg(nullptr, 91, 0, cellX, cellX >> 31, cellY, cellY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      return;
    }

    if (this->unknown_0510 == 2) {
      this->SendMsg(nullptr, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (this->unknown_0514 != 0 || timeGetTime() <= this->unknown_0500 + 1200) {
      return;
    }
    ++this->unknown_0520;
    this->unknown_0500 = timeGetTime();
    this->unknown_0514 = 1;
    if (this->unknown_0510 != 2) {
      this->SendMsg(nullptr, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      this->SendMsg(nullptr, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    return;
  }
  case 2: {
    CItem* item = mode->scene->GetItemByAID(this->unknown_051C);
    if (item == nullptr) {
      break;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    int itemX;
    int itemY;
    mode->WorldCell(item->render.position.x, item->render.position.z, itemX, itemY);
    const int targetX = itemX;
    const int targetY = itemY;
    mode->StepTowardWalkableCell(column, row, itemX, itemY);
    const int state = static_cast<int>(this->render.unknown_0070);
    if (state != 0 && state < 16) {
      return;
    }

    if (mode->IsWithinRange(column, row, targetX, targetY, 1)) {
      this->SendMsg(nullptr, 16, 0, targetX, targetX >> 31, targetY, targetY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      if (!mode->HasUnknown01A4(this->unknown_051C)) {
        mode->PushUnknown01A4(this->unknown_051C);
        mode->SendMsg(136, this->unknown_051C, 0, 0, 0);
      }
      break;
    }

    if (reinterpret_cast<const int&>(this->unknown_04F8) == itemX && this->unknown_04FC == itemY) {
      return;
    }
    this->unknown_04FC = itemY;
    reinterpret_cast<int&>(this->unknown_04F8) = itemX;
    this->SendMsg(nullptr, 91, 0, itemX, itemX >> 31, itemY, itemY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    return;
  }
  case 3: {
    CGameActor* target;
    if (this->unknown_051C == g_session.m_aid) {
      target = this;
    } else {
      target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(this->unknown_051C));
    }
    if (target == nullptr) {
      break;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    const float x = target->render.position.x;
    int cellX = target->unknown_0158[5];
    int cellY = *reinterpret_cast<const int*>(target->unknown_0170);
    const float z = target->render.position.z;
    const int range = mode->GetSkillAttackRange(this->unknown_052C, this->unknown_0530);
    mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, cellX, cellY, range);
    const int state = static_cast<int>(this->render.unknown_0070);
    if (state != 1 && state != 0 && state != 4 && state < 16) {
      return;
    }

    const float distance = this->AdjustFollowDistance(
      target, target->CalcDist(this->render.position.x, this->render.position.y, this->render.position.z)
    );
    const bool inRange = static_cast<float>(range * 5) + 5.5f >= distance;
    if ((reinterpret_cast<const int&>(this->unknown_04F8) == column && this->unknown_04FC == row) || inRange) {
      this->SendMsg(nullptr, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }

    if (reinterpret_cast<const int&>(this->unknown_04F8) == cellX && this->unknown_04FC == cellY) {
      return;
    }
    if (this->render.unknown_0070 == 2) {
      return;
    }
    this->unknown_04FC = cellY;
    reinterpret_cast<int&>(this->unknown_04F8) = cellX;
    this->ApplyFollowOffset(target, x, z, &cellY, &cellX);
    this->SendMsg(nullptr, 91, 0, cellX, cellX >> 31, cellY, cellY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    return;
  }
  case 4: {
    int groundX = this->unknown_0534;
    int groundY = this->unknown_0538;
    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    const int range = mode->GetSkillAttackRange(this->unknown_052C, this->unknown_0530);
    float x;
    float z;
    reinterpret_cast<const ::TerrainHeight*>(mode)->CellCenter(groundX, groundY, x, z);
    mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, groundX, groundY, range);
    const int state = static_cast<int>(this->render.unknown_0070);
    if (state != 1 && state != 0 && state != 4 && state < 16) {
      return;
    }

    const float distance = this->CalcDist(x, this->render.position.y, z);
    const bool inRange = static_cast<float>(range * 5) + 5.5f >= distance;
    if ((reinterpret_cast<const int&>(this->unknown_04F8) == column && this->unknown_04FC == row) || inRange) {
      this->SendMsg(nullptr, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }

    if (reinterpret_cast<const int&>(this->unknown_04F8) == groundX && this->unknown_04FC == groundY) {
      return;
    }
    if (this->render.unknown_0070 == 2) {
      return;
    }
    this->unknown_04FC = groundY;
    reinterpret_cast<int&>(this->unknown_04F8) = groundX;
    this->SendMsg(nullptr, 91, 0, groundX, groundX >> 31, groundY, groundY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    return;
  }
  case 6: {
    CGameActor* skill = static_cast<CGameActor*>(mode->scene->GetSkillByAID(this->unknown_051C));
    if (skill == nullptr) {
      break;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    int cellX;
    int cellY;
    mode->WorldCell(skill->render.position.x, skill->render.position.z, cellX, cellY);
    int range = mode->GetSkillAttackRange(this->unknown_052C, this->unknown_0530);
    if (mode->SendMsg(95, 0, 0, 0, 0) == 0 && range < 5 && this->unknown_052C != 124) {
      range = g_session.unknown_0F4C > 5 ? g_session.unknown_0F4C : 5;
    }
    float x;
    float z;
    reinterpret_cast<const ::TerrainHeight*>(mode)->CellCenter(cellX, cellY, x, z);
    mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, cellX, cellY, range);
    const int state = static_cast<int>(this->render.unknown_0070);
    if (state != 1 && state != 0 && state != 4 && state < 16) {
      return;
    }

    const float distance = this->CalcDist(x, this->render.position.y, z);
    const bool inRange = static_cast<float>(range * 5) + 5.5f >= distance;
    if ((reinterpret_cast<const int&>(this->unknown_04F8) != column || this->unknown_04FC != row) && !inRange) {
      if (reinterpret_cast<const int&>(this->unknown_04F8) == cellX && this->unknown_04FC == cellY) {
        return;
      }
      if (this->render.unknown_0070 == 2 && skill->GetEffectiveJob(-1) != 141 && skill->GetEffectiveJob(-1) != 143 &&
          skill->GetEffectiveJob(-1) != 152) {
        return;
      }
      this->unknown_04FC = cellY;
      reinterpret_cast<int&>(this->unknown_04F8) = cellX;
      this->SendMsg(nullptr, 91, 0, cellX, cellX >> 31, cellY, cellY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      return;
    }

    switch (skill->GetEffectiveJob(-1)) {
    case 141:
    case 143:
    case 152:
    case 218:
    case 222:
    case 230:
    case 20852:
    case 20853:
    case 20854:
    case 20855:
      if (this->unknown_052C == 0) {
        if (this->unknown_0510 == 2) {
          this->SendMsg(nullptr, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        if (this->unknown_0514 != 0 || timeGetTime() <= this->unknown_0500 + 1200) {
          return;
        }
        ++this->unknown_0520;
        this->unknown_0500 = timeGetTime();
        this->unknown_0514 = 1;
        if (this->unknown_0510 != 2) {
          this->SendMsg(nullptr, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        } else {
          this->SendMsg(nullptr, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        return;
      }
      break;
    }
    this->SendMsg(nullptr, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  default:
    return;
  }

  this->SendMsg(nullptr, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00DB68B0

// GLOBAL: gl_25218
bool g_unknown173AD3F;

struct PlayerSkillRequest {
  int skill;
  int x;
  int y;
  int level;
};

// OFFSET: 0x00DB9A00
// FUNCTION: fn_22672
void CPlayer::SendMsg(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
  case 0x00: {
    this->unknown_050C = 0;
    const int* route = reinterpret_cast<const int*>(b);
    this->routeStartTick = route[0];
    if (this->FindRoute(route[1], route[2], route[3], route[4], route[5], route[6], *reinterpret_cast<const int*>(g_session.unknown_16F4))) {
      this->unknown_0158[5] = route[5];
      *reinterpret_cast<int*>(this->unknown_0170) = route[6];
      this->Slot15(1, 1);
      this->unknown_023C = 0;
    } else {
      int column;
      int row;
      int subColumn;
      int subRow;
      reinterpret_cast<const TerrainHeight*>(mode)->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
      if (this->FindRoute(column, row, subColumn, subRow, route[5], route[6], *reinterpret_cast<const int*>(g_session.unknown_16F4))) {
        this->unknown_0158[5] = route[5];
        *reinterpret_cast<int*>(this->unknown_0170) = route[6];
        this->Slot15(1, 1);
      }
    }
    g_navigationManager.CheckArrival(this->unknown_0158[5], *reinterpret_cast<const int*>(this->unknown_0170));
    break;
  }
  case 0x08:
    this->Slot15(0, 1);
    break;
  case 0x09:
    this->unknown_050C = 1;
    this->unknown_0504 = timeGetTime();
    reinterpret_cast<int&>(this->unknown_04F8) = b;
    this->unknown_04FC = d;
    break;
  case 0x0A:
    if (this->render.unknown_0070 != 6 && !StateFlag01(this->stateFlags)) {
      CRenderObject* target = mode->scene->GetGameActorByAID(b);
      if (target != nullptr) {
        this->unknown_0508 =
          static_cast<std::uint32_t>(*reinterpret_cast<const int*>(g_session.unknown_16F4) * 4 / 3) <
              reinterpret_cast<const std::uint32_t*>(static_cast<CGameActor*>(target)->unknown_0170)[1]
            ? 5
            : 1;
        this->unknown_051C = b;
        this->unknown_0514 = 0;
        this->unknown_0510 = d;
        this->unknown_0520 = 0;
      }
    }
    break;
  case 0x0D:
    this->unknown_0514 = 0;
    this->unknown_0508 = 0;
    mode->SendMsg(0x1A, 0, 0, 0, 0);
    break;
  case 0x0F: {
    const int attackMode = this->unknown_0508;
    if (attackMode != 1 && attackMode != 5) {
      if (attackMode == 3) {
        mode->SendMsg(0x1A, 0, 0, 0, 0);
        break;
      }
      if (attackMode != 6) {
        break;
      }
    }
    if (this->unknown_0510 != 0) {
      this->unknown_0510 = 2;
      if (this->unknown_0520 != 0) {
        this->SendMsg(nullptr, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        mode->SendMsg(0x49, 0, 0, 0, 0);
      }
    }
    break;
  }
  case 0x10: {
    const std::uint32_t state = this->render.unknown_0070;
    if (state != 0 && state != 4 && state != 6) {
      break;
    }
    float targetX;
    float targetZ;
    reinterpret_cast<const TerrainHeight*>(mode)->CellCenter(b, d, targetX, targetZ);
    const float heading = CRenderObject::Heading(this->render.position.x, this->render.position.z, targetX, targetZ);
    const float facing = static_cast<float>((CPlayer::AngleToDirection(static_cast<float>(heading + 22.5)) + 4) * 45);
    int delta = static_cast<int>(this->render.angle - facing);
    if (delta < 0) {
      delta += 360;
    }
    int direction = delta / 45;
    if (direction < 0) {
      direction += 8;
    } else if (direction >= 8) {
      direction -= 8;
    }
    if (direction == 0 || direction == 4) {
      this->unknown_043C = 0;
      this->render.angle = facing;
    } else if (direction == 1) {
      if (this->unknown_043C == 1) {
        this->unknown_043C = 0;
        this->render.angle = facing;
      } else {
        this->unknown_043C = 1;
      }
    } else {
      if (direction == 2 || direction == 3) {
        this->unknown_043C = 1;
        const float angle = facing + 45.0f;
        this->render.angle = angle;
        if (angle >= 360.0f) {
          this->render.angle = angle - 360.0f;
        }
      }
      if (direction == 7) {
        if (this->unknown_043C == 2) {
          this->unknown_043C = 0;
          this->render.angle = facing;
        } else {
          this->unknown_043C = 2;
        }
      } else if (direction == 5 || direction == 6) {
        const float angle = facing - 45.0f;
        this->unknown_043C = 2;
        this->render.angle = angle;
        if (angle < 0.0f) {
          this->render.angle = angle + 360.0f;
        }
      }
    }
    const int motion = this->unknown_043C;
    this->render.curMotion = motion;
    this->render.angle = static_cast<float>((CPlayer::AngleToDirection(this->render.angle) + 4) * 45);
    mode->SendMsg(0x8B, CPlayer::AngleToDirection(this->render.angle), motion, 0, 0);
    break;
  }
  case 0x11:
    this->SendMsg(nullptr, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (this->render.unknown_0070 != 0x4B) {
      if (this->IsSpecialState(this->render.unknown_0070)) {
        this->unknown_0518 = 1;
        this->unknown_053C = b;
        this->unknown_0540 = d;
      } else {
        this->unknown_0518 = 0;
        this->unknown_053C = -1;
        this->unknown_0540 = -1;
        this->SendMsg(nullptr, 0x5B, 0, b, b >> 31, d, d >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
    break;
  case 0x12:
    if (this->render.unknown_0070 != 6 && !StateFlag01(this->stateFlags)) {
      this->unknown_0508 = 2;
      this->unknown_051C = b;
    }
    break;
  case 0x1D:
    mode->SendMsg(0x89, 0, this->unknown_051C, 0, 0);
    break;
  case 0x1F:
    if (!StateFlag01(this->stateFlags)) {
      if (this->render.unknown_0070 == 0) {
        mode->SendMsg(0x89, 2, this->unknown_051C, 0, 0);
      } else if (this->render.unknown_0070 == 6) {
        mode->SendMsg(0x89, 3, this->unknown_051C, 0, 0);
      }
    }
    break;
  case 0x28: {
    CRenderObject* target = mode->scene->GetGameActorByAID(this->unknown_051C);
    if (target == nullptr || static_cast<CGameActor*>(target)->unknown_0314 != 7) {
      mode->SendMsg(0x89, 7, this->unknown_051C, 0, 0);
    }
    break;
  }
  case 0x29:
    if (this->render.unknown_0070 != 6 &&
        (!StateFlag01(this->stateFlags) || (d == 0x89 && e == 0) || (d == 0xD4 && e == 0) || (d == 0x211 && e == 0) ||
         (d == 0x212 && e == 0) || (d == 0x250 && e == 0))) {
      this->unknown_0508 = 3;
      this->unknown_051C = b;
      this->unknown_052C = d;
    }
    break;
  case 0x2A:
    mode->SendMsg(0x45, this->unknown_052C, this->unknown_051C, this->unknown_0530, 0);
    break;
  case 0x3C:
    if (this->render.unknown_0070 != 6 && !StateFlag01(this->stateFlags)) {
      const int attackMode = this->unknown_0508 == 1 ? 5 : 1;
      this->unknown_0524 = timeGetTime() - 1200;
      this->unknown_0508 = attackMode;
      this->unknown_051C = b;
      this->unknown_0514 = 0;
      this->unknown_0520 = 0;
      reinterpret_cast<int&>(this->unknown_04F8) = -1;
      this->unknown_04FC = -1;
    }
    break;
  case 0x41:
    if (this->render.unknown_0070 != 6 &&
        (!StateFlag01(this->stateFlags) || (b == 0x89 && c == 0) || (b == 0xD4 && c == 0) || (b == 0x211 && c == 0) ||
         (b == 0x212 && c == 0))) {
      this->unknown_0508 = 4;
      this->unknown_052C = b;
      this->unknown_0534 = d;
      this->unknown_0538 = f;
      this->unknown_051C = 0;
    }
    break;
  case 0x42: {
    PlayerSkillRequest request;
    request.skill = this->unknown_052C;
    request.x = this->unknown_0534;
    request.y = this->unknown_0538;
    request.level = this->unknown_0530;
    if (request.skill == 0x7D) {
      g_windowMgr.EnsureWindow(0x54)->SendMsg(nullptr, 0x44, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
    } else if (request.skill == 0xDC) {
      g_windowMgr.EnsureWindow(0x65)->SendMsg(nullptr, 0x44, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
    } else if (request.skill == 0x108 || request.skill == 0xA04) {
      int metadata = 0;
      int column = 0;
      int row = 0;
      int subColumn = 0;
      int subRow = 0;
      reinterpret_cast<const TerrainHeight*>(g_modeMgr.GetGameMode())
        ->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
      MovementRouteInterpolation route;
      if (g_pathFinder.FindPath(timeGetTime(), column, row, subColumn, subRow, this->unknown_0534, this->unknown_0538, 150, route, 15) == true) {
        metadata = route.EndMetadata();
      }
      mode->SendMsg(0x46, reinterpret_cast<int>(&request), metadata, 0, 0);
    } else {
      mode->SendMsg(0x46, reinterpret_cast<int>(&request), 0, 0, 0);
    }
    break;
  }
  case 0x5A:
    this->unknown_0530 = b;
    break;
  case 0x5B:
    g_unknown173AD3F = false;
    if (timeGetTime() > this->unknown_0528 + 350) {
      this->unknown_0528 = timeGetTime();
      mode->SendMsg(0x8A, b, d, 0, 0);
    }
    break;
  case 0x67:
    if (this->render.unknown_0070 != 6 && !StateFlag01(this->stateFlags)) {
      const int* skill = reinterpret_cast<const int*>(d);
      this->unknown_0508 = 6;
      this->unknown_052C = skill[1];
      this->unknown_0530 = skill[3];
      this->unknown_051C = b;
      this->unknown_0510 = f;
      this->unknown_0514 = 0;
      this->unknown_0520 = 0;
    }
    break;
  case 0x95:
    this->route.points.clear();
    break;
  default:
    this->CPc::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, j, k, l, m);
    break;
  }
}

// OFFSET: 0x00DAC750
// FUNCTION: fn_22596
void CGrannyPc::RenderModel(
  const matrix& view, const GrannyPcLight& light, C3dGrannyModelRes* model, int layer, C3dGrannyModelRes* animation
) {
  if (!this->render.visible || this->render.accountId == 0xFFFFFFFF) {
    return;
  }

  CTexture* texture = nullptr;
  std::uint16_t* indices = nullptr;

  this->brightness = (this->alpha << 24) + 0xFFFFFF;
  g_modeMgr.GetGameMode();
  union {
    void (CGrannyPc::*asVoid)(C3dGrannyModelRes*);
    granny_world_pose* (CGrannyPc::*asPose)(C3dGrannyModelRes*);
  } update;
  update.asVoid = &CGrannyPc::UpdateAnimation;
  granny_world_pose* pose = nullptr;
  RenderCommand** commands = this->layerCommands[layer];
  if (!model) {
    model = this->model;
    this->unknown_2550 = (this->*update.asPose)(nullptr);
    commands = this->commands;
  } else if (animation) {
    pose = (this->*update.asPose)(animation);
  } else {
    pose = this->unknown_2550;
  }

  matrix rotation;
  rotation.MakeXRotation(1.570796f);
  rotation.AppendYRotation((this->angle + 180.0f) / 180.0f * 3.141592f);
  rotation.AppendScale(0.3f, 0.3f, 0.3f);
  vector3d position;
  position.x = this->render.position.x - 2.0f;
  position.y = this->render.position.y;
  position.z = this->render.position.z - 2.0f;
  rotation.AppendTranslate(&position.x);

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, view.values, &screenX, &screenY, &reciprocalDepth);

  bool attached = false;
  matrix inverse;
  inverse.MakeInverse(view);
  vector3d direction;
  direction.x = light.direction.x * inverse.values[0] + light.direction.y * inverse.values[3] + light.direction.z * inverse.values[6] + inverse.values[9];
  direction.y = light.direction.x * inverse.values[1] + light.direction.y * inverse.values[4] + light.direction.z * inverse.values[7] + inverse.values[10];
  direction.z = light.direction.x * inverse.values[2] + light.direction.y * inverse.values[5] + inverse.values[8] * light.direction.z + inverse.values[11];
  const float scale = 1.0f / SquareRoot(direction.y * direction.y + direction.x * direction.x + direction.z * direction.z);
  const float lightX = scale * direction.x;
  const float lightY = scale * direction.y;
  const float lightZ = scale * direction.z;

  float bounds[5];
  bounds[0] = static_cast<float>(g_renderer->viewport.width);
  bounds[1] = static_cast<float>(g_renderer->viewport.height);
  bounds[2] = 0.0f;
  bounds[3] = 0.0f;
  bounds[4] = reciprocalDepth;

  for (int mesh = 0; mesh < model->instance->meshCount; ++mesh) {
    if (layer == 0) {
      model->deformBuffer = this->vertices[mesh];
    } else {
      model->deformBuffer = this->layerVertices[layer][mesh];
    }

    int triangleCount;
    matrix transform;
    const GrannyPNT332Vertex* source = static_cast<const GrannyPNT332Vertex*>(
      model->GetMeshRenderData(mesh, &texture, &indices, &triangleCount, &transform, pose)
    );
    if (!source || !texture) {
      continue;
    }

    const matrix* attachment;
    if (layer == this->attachLayer) {
      attached = true;
      attachment = &this->attachTransform;
    } else {
      attachment = &transform;
    }

    matrix local;
    local.Compose(*attachment, rotation);
    matrix world;
    world.Compose(local, view);

    int vertexCount = 0;
    RenderCommand* command = commands[mesh];
    for (int triangle = 0; triangle < triangleCount; ++triangle) {
      for (int corner = 3; corner != 0; --corner) {
        const GrannyPNT332Vertex& vertex = source[indices[vertexCount]];
        RenderVertex& output = command->vertices[vertexCount];
        ++vertexCount;
        position.x = vertex.position[0];
        position.y = vertex.position[1];
        position.z = vertex.position[2];
        if (attached == true && mesh > 0) {
          const vector3d point = position;
          position.x = point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9];
          position.y = transform.values[4] * point.y + transform.values[1] * point.x + transform.values[7] * point.z + transform.values[10];
          position.z = transform.values[5] * point.y + transform.values[2] * point.x + transform.values[8] * point.z + transform.values[11];
        }

        g_renderer->viewport.Projection().Project(&position.x, world.values, output);
        output.u = vertex.uv[0];
        output.v = vertex.uv[1];
        const float shade = lightY * vertex.normal[1] + lightX * vertex.normal[0] + lightZ * vertex.normal[2];
        const unsigned brightness = static_cast<unsigned>(
          std::max(0.0f, std::min(1.0f, 1.0f - shade)) * static_cast<float>(light.intensity) + static_cast<float>(255 - light.intensity)
        );
        this->brightness = brightness;
        output.diffuse = ((light.red + 0x38 + brightness) << 16 & 0xFF0000) + ((light.green + 0x38 + brightness) << 8 & 0xFF00) +
                         static_cast<unsigned char>(light.blue + 0x38 + brightness) + (this->alpha << 24);
      }

      if (layer != 1) {
        this->unknown_2558.push_back(position);
      }
    }

    command->texture = texture;
    command->vertexCount = vertexCount;
    g_renderer->queues.Submit(command, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    this->meshTextures[layer][mesh] = texture;
    this->meshIndices[layer][mesh] = indices;
    this->meshTriangleCounts[layer][mesh] = triangleCount;
    this->meshTransforms[mesh] = transform;
  }

  if (model == this->model) {
    if (this->model->ProjectBounds(bounds, view, rotation, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.eye)) {
      this->SetRenderInfo(bounds, screenX, screenY);
    } else {
      this->SetTlvert(screenX, screenY);
    }
  }
}
