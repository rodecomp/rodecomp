#include "Client/Mode/ModeMessage.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/ActorEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Skills/SkillUnitId.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/AttackMotion.h"
#include "Contents/Actors/Item.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Skills/EnergyOrbEffect.h"
#include "Contents/Effects/Skills/MonsterSkillEffects.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Skills/TetraBallEffect.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Effects/Core/MsgEffect.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Items/WeaponType.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Sprites/SpriteLayerDrawing.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Contents/World/World.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Movement/MovementRouteInterpolation.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/Movement/TerrainPathSearch.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include "Contents/Guild/GuildInfo.h"

// GLOBAL: gl_24753
int g_maxMoveSpeed = 432;

// GLOBAL: gl_25175
unsigned char g_statusRefreshSuppressed;

// GLOBAL: gl_25150
static std::uint32_t g_groundShadowPixels[35 * 35];

// OFFSET: 0x00DA7700
// FUNCTION: fn_22558
CPc::CPc() : footprintPosition({0.0f, 0.0f, 0.0f}) {
  this->outlineColor = 0;
  this->equipSprites.resize(9, nullptr);
  this->equipActions.resize(9, nullptr);
  this->gaugeWindow = nullptr;
  this->headPalette = 0;
  this->shield = 0;
  this->honor = 0;
  this->virtue = 0;
  this->guildId = 0;
  this->headDir = 0;
  this->shoe[0] = 0;
  this->renderWithoutLayer = 0;
  this->pkRank = 0;
  this->pkTotal = 0;
  this->render.isCharacter = 1;
  this->robe = -1;
  this->blend = -1;
  this->layerCellX = -1;
  this->layerCellY = -1;
  this->msgEffect = nullptr;
  this->pkState = 0;
  this->bodyStyle = 0;
  this->appearanceJob = 0;
  this->alternateDoramAppearance[0] = 0;
  this->isHelmRobe = false;
  this->appearanceFlags = 0;
  this->madogearType = 0;
  this->arrowEffectSuppressed = false;
  this->extentLeft = 0.0f;
  this->extentTop = 0.0f;
  this->extentRight = 0.0f;
  this->extentBottom = 0.0f;
  this->weapon = 0;
  this->secondaryWeapon = 0;
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
  if (this->gaugeWindow != nullptr) {
    g_windowMgr.Schedule(this->gaugeWindow);
    this->gaugeWindow = nullptr;
  }
}

// OFFSET: 0x00DAFE50
// FUNCTION: fn_22606
void CGrannyPc::SetAction(unsigned animation, int, int repeat) {
  if (!this->model) {
    return;
  }

  this->render.state = ST_STAND;
  switch (animation) {
    case 0:
      this->render.state = ST_STAND;
      break;
    case 48:
      this->render.state = ST_DAMAGE;
      break;
    case 80:
    case 88:
      this->render.state = ST_ATTACK;
      break;
    case 64:
      this->render.state = ST_DEAD;
      break;
    case 8:
      this->render.state = ST_MOVE;
      break;
    case 24:
      this->render.state = ST_PICKUP;
      break;
    case 32:
      this->render.state = ST_ATTACKWAIT;
      break;
    case 40:
      this->render.state = ST_THROW;
      break;
    case 16:
      this->render.state = ST_SIT;
      break;
  }
  this->PlayMotion(repeat);
}

// OFFSET: 0x00DB01F0
// FUNCTION: fn_22611
void CGrannyPc::PlayMotion(int repeat) {
  this->motion = this->render.state;
  for (int i = 0; i < 20; ++i) {
    this->model->FreeControl(this->controls[i]);
    this->controls[i] = nullptr;
  }
  this->animationTime = 0;

  if (repeat == 1 && this->render.state != ST_SIT) {
    this->controls[this->motion] = this->model->PlayAnimation(this->motion, this->instance, this->controls[this->motion], 1);
  } else {
    this->controls[this->motion] = this->model->PlayAnimation(this->motion, this->instance, this->controls[this->motion], 0);
  }
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
void CPc::SetBodyCostume(int first, int second) {
  this->bodyStyle = first;
  if (static_cast<unsigned char>(second) != 0) {
    int job;
    if (this->IsShapeShifted()) {
      job = this->GetShapeShiftJob();
    } else {
      job = this->GetEffectiveJob(-1);
    }
    this->SetSprJob(job);
    this->SetSprWeapon2(this->weapon, this->secondaryWeapon);
    this->SetBodyPaletteName();
  }
}

// OFFSET: 0x00DB0000
// FUNCTION: fn_22608
void CPc::SetBodyPalette(int value) {
  this->render.bodyPalette = value;
  this->render.bodyPalette = g_session.GetBodyStyle(this->job, this->render.bodyPalette, this->bodyStyle);
  this->SetBodyPaletteName();
}

// OFFSET: 0x00DB02A0
// FUNCTION: fn_22613
void CGrannyPc::SetHeadPaletteName() {
  if (this->headPalette != 0) {
    this->headPaletteName = static_cast<const std::string&>(g_session.GetHeadPalName(this->head, this->GetEffectiveJob(-1), this->sex, this->headPalette));
  }
}

// OFFSET: 0x00DB0350
// FUNCTION: fn_22614
void CPc::SetHeadPaletteName() {
  if (this->headPalette != 0) {
    this->headPaletteName = static_cast<const std::string&>(g_session.GetHeadPalName(this->head, this->appearanceJob, this->sex, this->headPalette));
  }
}

// OFFSET: 0x00DB0280
// FUNCTION: fn_22612
void CPc::SetHeadPalette(int value) {
  this->headPalette = value;
  this->SetHeadPaletteName();
}

// OFFSET: 0x00DB0400
// FUNCTION: fn_22615
void CGrannyPc::SetHonor(int value) {
  if (value < 0) {
    const std::int64_t emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit);
    this->SendMsg(nullptr, AM_CASH_EMOTION, 0, emotion, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, AM_DELETE_PROHIBITEFFECT, 0, 0, 0, 0, 0, 0);
  }
  this->honor = value;
}

// OFFSET: 0x00DB0480
// FUNCTION: fn_22616
void CPc::SetHonor(int value) {
  if (value < 0) {
    const std::int64_t emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit);
    this->SendMsg(nullptr, AM_CASH_EMOTION, 0, emotion, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, AM_DELETE_PROHIBITEFFECT, 0, 0, 0, 0, 0, 0);
  }
  this->honor = value;
}

// OFFSET: 0x00DB0760
// FUNCTION: fn_22621
void CGrannyPc::SetPropensityInfo(int first, int second) {
  this->SetHonor(first);
  this->virtue = second;
}

// OFFSET: 0x00DB0780
// FUNCTION: fn_22622
void CPc::SetPropensityInfo(int first, int second) {
  this->SetHonor(first);
  this->virtue = second;
}

// OFFSET: 0x00DB07C0
// FUNCTION: fn_22625
void CPc::SetRiding(unsigned char value) {
  this->isRiding = value;
  this->SetSprShield(this->shield);
}

// OFFSET: 0x00DB07E0
// FUNCTION: fn_22626
void CGrannyPc::SetSprAccessory2(int value) {
  this->accessory2 = value;
}

// OFFSET: 0x00DB09D0
// FUNCTION: fn_22628
void CGrannyPc::SetSprAccessory3(int value) {
  this->accessory3 = value;
}

// OFFSET: 0x00DB2660
// FUNCTION: fn_22644
void CGrannyPc::SetSprShield(int) {
  this->GetEffectiveJob(-1);
}

// OFFSET: 0x00DB0B40
// FUNCTION: fn_22630
void CGrannyPc::SetSprAccessory(int value) {
  this->accessory = value;
}

// OFFSET: 0x00DB0CB0
// FUNCTION: fn_22632
void CGrannyPc::SetSprAct2(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int) {
  this->SetSprActFull(first, second, third, fourth, fifth, sixth, seventh, eighth, this->shield, ninth, tenth);
}

// OFFSET: 0x00DB1960
// FUNCTION: fn_22636
void CPc::RefreshAccessory3() {
  if (this->accessory3 == 185) {
    this->SetSprAccessory3(185);
  }
}

// OFFSET: 0x00DB1980
// FUNCTION: fn_22637
void CGrannyPc::SetSprHead(int value) {
  this->head = value;
}

// OFFSET: 0x00DB1AC0
// FUNCTION: fn_22639
void CGrannyPc::SetSprJob(int value) {
  this->render.shadowScale = g_session.GetNpcLocationRadius(value, false);
}

// OFFSET: 0x00DB2830
// FUNCTION: fn_22646
void CGrannyPc::SetSprWeapon2(int, int) {
}

// OFFSET: 0x00DB3720
// FUNCTION: fn_22648
void CGrannyPc::SetSprWeapon(int weaponLow32, int weaponHigh32) {
  struct WeaponValueParts {
    int low32;
    int high32;
  };
  const WeaponValueParts values = {weaponLow32, weaponHigh32};
  std::memcpy(&this->weaponPair, &values, sizeof(values));
}

// OFFSET: 0x00DB3C30
// FUNCTION: fn_22650
void CGrannyPc::SetState(int state, int resetPosition) {
  const int current = this->render.state;
  if (current == ST_DEAD || this->isTrickDead != 0) {
    return;
  }

  if (current == ST_DAMAGE && static_cast<char>(resetPosition) == 1) {
    this->render.position.x = this->damageDestX;
    this->render.position.z = this->damageDestZ;
  }


  switch (state) {
  case ST_BEGIN_SKILL:
    this->RegisterPos();
    this->SetAction(0x60, 6, 1);
    this->render.isMotionFinished = true;
    break;
  case ST_SIT:
    this->RegisterPos();
    this->SetAction(0x10, 4, 1);
    this->render.isMotionFinished = true;
    this->render.curMotion = this->headDir;
    break;
  case ST_MOVE:
    this->isCounter = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    this->headDir = 0;
    CGameActor::SetState(ST_MOVE, resetPosition);
    break;
  case ST_STAND:
    CGameActor::SetState(ST_STAND, resetPosition);
    this->render.curMotion = this->headDir;
    if (current == ST_DAMAGE || current == ST_ATTACK || this->healthState != 0 || current == ST_BEGIN_SKILL) {
      this->SetAction(0x20, 4, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
    break;
  case ST_SKILL:
    if (current == ST_SIT) {
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
    this->attackMotion = 6.0f;
    break;
  case ST_ATTACK: {
    this->isCounter = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    const bool weaponAllowed = IsJobWeaponAllowed(
      this->GetEffectiveJob(-1), this->sex,
      (static_cast<std::int64_t>(this->secondaryWeapon) << 32) | static_cast<std::uint32_t>(this->weapon)
    );
    CGameActor::SetState(weaponAllowed ? 9 : 2, resetPosition);
    this->attackMotion = AttackMotionLength(this->GetEffectiveJob(-1), this->sex, this->weapon, weaponAllowed);
    bool usesArrow = false;
    g_session.m_hotkeyScript->Call("IsItemUsingArrow", "d>b", this->weapon, &usesArrow);
    if (usesArrow == true) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->SpawnArrowEffect(&this->render.position, this->hitTargetAid, this->attackMotion * this->render.speed, 0);
      this->attackMotion = 8.0f / this->render.speed + this->attackMotion;
    }
    break;
  }
  case ST_ATTACK2: {
    this->isCounter = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    const bool weaponAllowed = IsJobWeaponAllowed(
      this->GetEffectiveJob(-1), this->sex,
      (static_cast<std::int64_t>(this->secondaryWeapon) << 32) | static_cast<std::uint32_t>(this->weapon)
    );
    CGameActor::SetState(weaponAllowed ? 9 : 2, resetPosition);
    this->attackMotion = AttackMotionLength(this->GetEffectiveJob(-1), this->sex, this->weapon, weaponAllowed);
    if ((this->weapon == 14 && this->secondaryWeapon == 0) || (this->weapon == 13 && this->secondaryWeapon == 0)) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->SpawnArrowEffect(&this->render.position, this->hitTargetAid, this->attackMotion * this->render.speed, 0);
      this->attackMotion = 8.0f / this->render.speed + this->attackMotion;
    }
    break;
  }
  default:
    CGameActor::SetState(state, resetPosition);
    if (this->render.state == ST_DEAD) {
      this->PlayMotion(1);
    }
    return;
  }

  this->directionalAnimation = this->Get8Dir(this->render.angle) + this->baseAnimation;
  this->render.state = state;
  this->render.motionStartTime = timeGetTime();
}

// OFFSET: 0x00DBA6C0
// FUNCTION: fn_22674
void CPlayer::UnRegisterPos() {
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
    case ST_ATTACK:
    case ST_PICKUP:
    case ST_SIT:
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
void CGrannyPc::SetModifyFactorOfmotionSpeed(int value) {
  if (this->weapon != 11 || this->secondaryWeapon != 0) {
    value = value > g_maxMoveSpeed ? g_maxMoveSpeed : value;
  }

  this->render.motionSpeedScale = static_cast<float>(value) / 432.0f;
}


// OFFSET: 0x00DB0710
// FUNCTION: fn_22620
void CPc::SetModifyFactorOfmotionSpeed(int value) {
  int weapon = this->weapon;
  int speed = value;

  if (weapon > 105) {
    weapon = GetWeaponViewValue(weapon);
  }

  if (weapon != 11) {
    speed = speed > g_maxMoveSpeed ? g_maxMoveSpeed : speed;
  }

  this->render.motionSpeedScale = static_cast<float>(speed) / 432.0f;
}

// OFFSET: 0x00DDD190
// FUNCTION: fn_23381
__declspec(noinline) std::string CSession::GetBodyImfName(int job, int sex, int head, int costumeJob) {
  return g_equipmentResourceMgr->GetBodyImfName(job, sex, head, costumeJob);
}

// OFFSET: 0x00DB0500
// FUNCTION: fn_22617
void CGrannyPc::SetImfFileName() {
  this->imfName = g_session.GetBodyImfName(
    this->GetEffectiveJob(-1), this->sex, this->head, this->GetEffectiveJob(-1)
  );
}

// OFFSET: 0x00DB05E0
// FUNCTION: fn_22618
void CPc::SetImfFileName() {
  int job = this->GetEffectiveJob(-1);

  if (job >= 0x10CE && job <= 0x10E9 && this->isRiding != 0) {
    job = g_session.GetRidingJob(job);
  }

  this->imfName = g_session.GetBodyImfName(job, this->sex, this->head, this->bodyStyle);
}

// OFFSET: 0x00DAFB60
// FUNCTION: fn_22603
void CPc::LoadEquipAction(int slot, const char* name) {
  if (this->equipActions[slot]) {
    this->equipActions[slot]->Unlock();
  }

  if (IsMonsterJob(this->render.accountId)) {
    if (IsObfuscatedMessage(this->render.accountId)) {
      switch (slot) {
        case 0:
          if (this->sex != 0) {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\남\\운영자2_남.Act"));
          } else {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\여\\운영자2_여.Act"));
          }
          break;
        case 5:
          if (this->sex != 0) {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자2_남_검.Act"));
          } else {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자2_여_검.Act"));
          }
          break;
        case 6:
          this->equipActions[slot] = nullptr;
          break;
        default:
          this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));
          break;
      }
    } else {
      switch (slot) {
        case 0:
          if (this->sex != 0) {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\남\\운영자_남.Act"));
          } else {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\여\\운영자_여.Act"));
          }
          break;
        case 5:
          if (this->sex != 0) {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자_남_검.Act"));
          } else {
            this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자_여_검.Act"));
          }
          break;
        case 6:
          this->equipActions[slot] = nullptr;
          break;
        default:
          this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));
          break;
      }
    }
  } else {
    this->equipActions[slot] = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));
  }

  if (this->equipActions[slot]) {
    this->equipActions[slot]->Lock();
  }

  if (this->equipActions.size() <= 1) {
    return;
  }

  CActRes* baseAction = static_cast<CActRes*>(this->equipActions[1]);
  if (!baseAction) {
    return;
  }

  const ActionFrame* baseFrame = &baseAction->Frame(0, 0);
  if (!baseFrame) {
    return;
  }

  const SpriteFrameCell* cell = &baseFrame->Cell(1);
  if (!cell) {
    return;
  }

  const int baseJob = JobNames::BaseJob(this->GetEffectiveJob(-1));
  const int costumeJob = this->GetBodyCostume();
  const std::string actionName = g_equipmentResourceMgr->GetBodyResourceName(0, baseJob, this->sex, costumeJob, 0);
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));
  if (!action) {
    return;
  }

  const ActionFrame* actionFrame = &action->Frame(0, 0);
  if (actionFrame && !actionFrame->anchors.empty() && !baseFrame->anchors.empty()) {
    this->extentLeft = static_cast<float>(actionFrame->anchors[0].x - baseFrame->anchors[0].x + cell->x);
    this->extentTop = static_cast<float>(actionFrame->anchors[0].y - baseFrame->anchors[0].y + cell->y);
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

  if (IsMonsterJob(this->render.accountId)) {
    if (IsObfuscatedMessage(this->render.accountId)) {
      switch (slot) {
      case 0:
        if (this->sex) {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\남\\운영자2_남.Spr"));
        } else {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\여\\운영자2_여.Spr"));
        }
        break;
      case 5:
        if (this->sex) {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자2_남_검.Spr"));
        } else {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자2_여_검.Spr"));
        }
        break;
      case 6:
        this->equipSprites[slot] = nullptr;
        break;
      default:
        this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(name));
        break;
      }
    } else {
      switch (slot) {
      case 0:
        if (this->sex) {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\남\\운영자_남.Spr"));
        } else {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\몸통\\여\\운영자_여.Spr"));
        }
        break;
      case 5:
        if (this->sex) {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자_남_검.Spr"));
        } else {
          this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load("인간족\\운영자\\운영자_여_검.Spr"));
        }
        break;
      case 6:
        this->equipSprites[slot] = nullptr;
        break;
      default:
        this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(name));
        break;
      }
    }
  } else {
    this->equipSprites[slot] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(name));
  }

  if (this->equipSprites[slot]) {
    this->equipSprites[slot]->Lock();
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
void CPc::SetSprRobe(int first, int second) {
  if (this->robe == first) {
    return;
  }

  if (static_cast<unsigned char>(second) != 0) {
    return;
  }

  this->robe = first;

  g_session.m_hotkeyScript->CallTable("TB_Layer_Priority", "IsHelmRobe", "d>b", first, &this->isHelmRobe);

  if (this->isHelmRobe == true) {
    this->LoadEquipAction(
      8,
      g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->sex, this->head, this->robe).c_str()
    );
    this->LoadEquipSprite(
      8,
      g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->sex, this->head, this->robe).c_str()
    );
  } else {
    this->LoadEquipAction(
      8, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->sex, this->head, 0).c_str()
    );
    this->LoadEquipSprite(
      8, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->sex, this->head, 0).c_str()
    );
  }
}

// OFFSET: 0x00DA7F10
// FUNCTION: fn_22563
void CPc::AddMsgEffect(CGameObject* effect) {
  this->msgEffectList.push_back(effect);
}

// OFFSET: 0x00DA8180
// FUNCTION: fn_22564
void CPc::SetSex(int value) {
  this->SetSexBit(value);
  this->SetSprActFull(
    this->appearanceJob, this->sex, this->head, this->weapon, this->secondaryWeapon, this->accessory, this->accessory2,
    this->accessory3, this->shield, this->headPalette, this->render.bodyPalette
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
  if (this->render.state == ST_DEAD || this->isTrickDead != 0) {
    this->render.previousAnimation = this->ActionBase(this->render.state);
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
    tagRECT box;
    std::uint32_t quadColor;
    if (this->GetMonsterTransformJob() != JT_THANATOS) {
      this->PrepareSprite(layer, layer, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, reinterpret_cast<int*>(&box), &quadColor, 0, 0, 0);
      if (g_cameraDistance >= 320.0f) {
        quad.bottomRhw = bottomRhw - 0.0000025f;
        quad.topRhw = topRhw - 0.0000025f;
      }
    } else {
      this->PrepareSprite(index, index, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, reinterpret_cast<int*>(&box), &quadColor, 0, 0, 0);
      if (this->render.state == ST_STAND && index == 1) {
        quad.bottomRhw = bottomRhw + 0.000008f;
        quad.topRhw = topRhw + 0.00008f;
      }
    }

    this->RaiseTransformPriority((box.bottom - box.top) / 4);
    quad.image = image;
    quad.color = color;
    if (this->render.translucentOther[0] == 1) {
      quad.color = color & 0xFFFFFF | 0x46000000;
    }

    if (this->render.visibilityOptions[1] != 0) {
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
    if (this->isRiding) {
      job = g_session.GetRidingJob(job);
    }

    int layer = 1;
    g_session.m_hotkeyScript->Call(
      "_New_DrawOnTop", "ddddd>bd", this->robe, this->sex, job, this->render.previousAnimation, this->render.curMotion,
      &drawOnTop, &layer
    );
  } else {
    g_session.m_hotkeyScript->Call(
      "DrawOnTop", "ddddd>b", this->robe, this->sex, this->job, this->render.previousAnimation,
      this->render.curMotion, &drawOnTop
    );
  }

  return drawOnTop;
}

// OFFSET: 0x00DA90E0
// FUNCTION: fn_22568
const ActionFrame& CPc::SelectStanceFrame(int layer, CActRes* actRes, int* animation, int* frame, bool* replaced) {
  const ActionFrame& current = actRes->Frame(this->render.previousAnimation, this->render.curMotion);
  if (this->isRiding || layer < 1 || layer > 4 || this->render.previousAnimation < 88) {
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
    if (this->sex == 0) {
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
    } else if (this->sex == 1) {
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

  if ((this->render.flags & 1) != 0 && layer <= 4) {
    *blend = 5;
  }

  if ((this->render.flags & 2) != 0 && layer == 5) {
    *blend = 5;
  }

  if ((this->render.flags & 4) != 0 && layer == 5) {
    if (!this->render.effectPhaseBytes[1]) {
      this->DrawQuad(*quad, palette, layer, 0.0f, 0.0f, 0);
      this->render.effectPhaseBytes[1] = 1;
    } else {
      this->render.effectPhaseBytes[1] = 0;
    }
  }

  short hover = 0;
  if (StateFlag15(this->GetStateFlags()) == true) {
    hover = static_cast<short>(this->HoverHeight());
  }

  const long long hoverOffset = hover;
  this->render.hoverOffset = hoverOffset;

  SpriteQuad jitter;
  if ((this->render.flags & 8) != 0 && layer <= 4 && (this->render.flags & 0x30000) == 0) {
    if (++this->render.effectPhaseBytes[2] > 180) {
      this->render.effectPhaseBytes[2] = 0;
    }

    float wave = g_sinTable[this->render.effectPhaseBytes[2]] + 3.0f;
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
    jitter.angle = static_cast<float>(cell->rotation + static_cast<int>(this->render.spriteAngle));
    jitter.image = image;
    this->DrawQuad(jitter, palette, 5, 0.0f, 0.0f, 0);

    wave = g_sinTable[this->render.effectPhaseBytes[2]];
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
    jitter.angle = static_cast<float>(cell->rotation + static_cast<int>(this->render.spriteAngle));
    jitter.image = image;
    this->DrawQuad(jitter, palette, 5, 0.0f, 0.0f, 0);
  }

  if ((this->render.flags & 0x80800) != 0 && (this->render.flags & 0x20000) == 0) {
    const float scale = this->render.scale;
    for (int step = 1, depth = 3; depth <= 6; ++step, depth += 3) {
      const unsigned char alpha = this->render.outlineAlpha;
      const float distanceScale = g_renderer->ScaleDistance(distance);
      const float factor = GetSkillFactor(this->GetEffectiveJob(-1)) * distanceScale * (static_cast<float>(step) * 0.01f + 1.0f);
      const float shift =
        g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) * this->render.scale) * factor;
      const int boxTop = box[1];
      const float top = static_cast<float>(static_cast<double>(cell->y + offsetY) * factor * cell->scaleY * scale + screenY);
      const float bottom = static_cast<float>(
        static_cast<double>((static_cast<short>(image->doubleHeight) + 1) * image->height + cell->y + offsetY) * factor * cell->scaleY *
          scale +
        screenY + 1.0
      );

      double left;
      double right;
      if ((cell->mirrored & 1) != 0) {
        left = static_cast<double>((static_cast<short>(image->doubleWidth) + 1) * image->width + cell->x + offsetX) * factor * cell->scaleX;
        right = static_cast<double>(offsetX + cell->x) * factor * cell->scaleX;
        if (boxTop >= g_renderer->viewport.height || boxTop <= 0 || box[2] >= g_renderer->viewport.width || box[0] <= 0) {
          continue;
        }
      } else {
        if (boxTop >= g_renderer->viewport.height || box[3] <= 0 || box[0] >= g_renderer->viewport.width || box[2] <= 0) {
          continue;
        }
        left = static_cast<double>(cell->x + offsetX) * factor * cell->scaleX;
        right = static_cast<double>((static_cast<short>(image->doubleWidth) + 1) * image->width + cell->x + offsetX) * factor * cell->scaleX;
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
      jitter.angle = static_cast<float>(cell->rotation + static_cast<int>(this->render.spriteAngle));
      this->DrawQuad(jitter, palette, 5, 0.0f, 0.0f, 0);
    }
  }

  if ((this->render.flags & 0x30000) == 0) {
    this->RenderBlur(cell, box, shiftX, bottomRhw, topRhw, palette, image, 4.0f, hover);
    this->RenderAura(quad, cell, box, shiftX, bottomRhw, topRhw, palette, image, *blend, hover);
  }

  this->ApplySpriteJitter(quad, box, shiftX, 11, 5);

  if ((this->render.flags & 0x10000) != 0) {
    const short level = this->render.effectPhase;
    const short alpha = static_cast<short>(255 - level * 20);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const float rise = 400.0f / mode->camera->camera.desiredDistance * static_cast<float>(level * 20);
    quad->top = static_cast<float>(box[1]) - rise;
    quad->bottom = static_cast<float>(box[3]) - rise;
    quad->color = (std::max<short>(alpha, 0) << 24) + 0xFFFFFF;
  }

  if ((this->render.flags & 0x20000) != 0) {
    const int riseUnits = (25 - this->render.tintPhase) * 20;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    short alpha = static_cast<short>((this->render.tintPhase - 10) * 17);
    if (alpha > 255) {
      alpha = 255;
    }
    const float rise = std::max(400.0f / mode->camera->camera.desiredDistance * static_cast<float>(riseUnits), 0.0f);
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
void CPc::SetSprWeapon2(int first, int second) {
  this->weapon = first;
  this->secondaryWeapon = second;

  int job = this->GetEffectiveJob(-1);
  if (this->IsCostumeOutfit()) {
    job = this->appearanceJob;
  }

  const int viewJob = job > 3950 ? job - 3950 : job;

  if (this->isRiding) {
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
          this->GetEffectiveJob(-1) != this->bodyStyle) {
        return;
      }

      const std::string actName = g_session.GetWeaponEffectActName(viewJob, this->sex, 0, -1, 1, this->bodyStyle);
      const std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->sex, 0, -1, 1, this->bodyStyle);
      this->LoadEquipAction(6, actName.c_str());
      this->LoadEquipSprite(6, sprName.c_str());
      return;
    }
    case JT_WOLF_RANGER:
    case JT_WOLF_RANGER_H:
    case JT_WOLF_RANGER_B:
    case JT_WOLF_WINDHAWK: {
      const std::string actName = g_session.GetWeaponEffectActName(viewJob, this->sex, 0, -1, 0, this->bodyStyle);
      const std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->sex, 0, -1, 0, this->bodyStyle);
      this->LoadEquipAction(6, actName.c_str());
      this->LoadEquipSprite(6, sprName.c_str());
      return;
    }
    case JT_MADOGEAR:
    case JT_MADOGEAR_H:
    case JT_MADOGEAR_B: {
      std::string actName = g_session.GetWeaponEffectActName(viewJob, this->sex, 0, -1, 0, this->bodyStyle);
      std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->sex, 0, -1, 0, this->bodyStyle);
      if (this->GetMadogearType() == 2) {
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
      std::string actName = g_session.GetWeaponEffectActName(viewJob, this->sex, 0, -1, 0, this->bodyStyle);
      std::string sprName = g_session.GetWeaponEffectSprName(viewJob, this->sex, 0, -1, 0, this->bodyStyle);
      if (this->GetMadogearType() == 2) {
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
  const int shieldItem = this->secondaryWeapon;
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
        switch (weaponType) {
          case W_FIST:
          case W_2HAXE:
          case W_MACE:
            weaponType = W_1HSWORD;
            break;
        }
        break;
    }

    const std::string actName = g_session.GetWeaponActName(viewJob, this->sex, weaponType, -1, this->bodyStyle);
    const std::string sprName = g_session.GetWeaponSprName(viewJob, this->sex, weaponType, -1, this->bodyStyle);
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
          g_session.GetWeaponEffectActName(viewJob, this->sex, weaponType, -1, 0, this->bodyStyle);
        const std::string effectSprName =
          g_session.GetWeaponEffectSprName(viewJob, this->sex, weaponType, -1, 0, this->bodyStyle);
        this->LoadEquipAction(6, effectActName.c_str());
        this->LoadEquipSprite(6, effectSprName.c_str());
        break;
      }
    }
    return;
  }

  int viewType = GetWeaponViewValue(this->weapon);
  const std::string actName = g_session.GetWeaponActName(viewJob, this->sex, viewType, weaponItem, this->bodyStyle);
  if (ResourceMgr::Instance().Exists(actName.c_str())) {
    this->LoadEquipAction(5, actName.c_str());
    const std::string sprName = g_session.GetWeaponSprName(viewJob, this->sex, viewType, weaponItem, this->bodyStyle);
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
    const std::string fallbackActName = g_session.GetWeaponActName(viewJob, this->sex, viewType, -1, this->bodyStyle);
    if (ResourceMgr::Instance().Exists(fallbackActName.c_str())) {
      this->LoadEquipAction(5, fallbackActName.c_str());
      const std::string fallbackSprName = g_session.GetWeaponSprName(viewJob, this->sex, viewType, -1, this->bodyStyle);
      this->LoadEquipSprite(5, fallbackSprName.c_str());
    }
  }

  std::string effectActName = g_session.GetWeaponEffectActName(viewJob, this->sex, viewType, weaponItem, 0, this->bodyStyle);
  std::string effectSprName = g_session.GetWeaponEffectSprName(viewJob, this->sex, viewType, weaponItem, 0, this->bodyStyle);
  if (!ResourceMgr::Instance().Exists(effectActName.c_str())) {
    effectActName = g_session.GetWeaponEffectActName(viewJob, this->sex, viewType, -1, 0, this->bodyStyle);
    effectSprName = g_session.GetWeaponEffectSprName(viewJob, this->sex, viewType, -1, 0, this->bodyStyle);
    if (!ResourceMgr::Instance().Exists(effectActName.c_str())) {
      return;
    }
  }

  this->LoadEquipAction(6, effectActName.c_str());
  this->LoadEquipSprite(6, effectSprName.c_str());
}

// OFFSET: 0x00DB3740
// FUNCTION: fn_22649
void CPc::SetSprWeapon(int first, int second) {
  this->weapon = first;
  this->secondaryWeapon = second;

  if (!g_session.IsJobWeaponTypeAllowed(this->GetEffectiveJob(-1), *reinterpret_cast<const std::int64_t*>(&first))) {
    this->LoadEquipAction(5, "");
    this->LoadEquipSprite(5, "");
    this->LoadEquipAction(6, "");
    this->LoadEquipSprite(6, "");
    return;
  }

  const std::string actName = g_session.GetWeaponActName(this->GetEffectiveJob(-1), this->sex, this->weapon, -1, this->bodyStyle);
  const std::string sprName = g_session.GetWeaponSprName(this->GetEffectiveJob(-1), this->sex, this->weapon, -1, this->bodyStyle);
  this->LoadEquipAction(5, actName.c_str());
  this->LoadEquipSprite(5, sprName.c_str());

  switch (this->weaponPair) {
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
        g_session.GetWeaponEffectActName(this->GetEffectiveJob(-1), this->sex, this->weapon, -1, 0, this->bodyStyle);
      const std::string effectSprName =
        g_session.GetWeaponEffectSprName(this->GetEffectiveJob(-1), this->sex, this->weapon, -1, 0, this->bodyStyle);
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
void CPlayer::RegisterPos() {
}

// OFFSET: 0x00DB07F0
// FUNCTION: fn_22627
void CPc::SetSprAccessory2(int value) {
  this->accessory2 = value;
  this->LoadEquipAction(3, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->sex, this->head, value).c_str());
  this->LoadEquipSprite(
    3, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->sex, this->head, this->accessory2).c_str()
  );

  if (this->HasId(EF_GREEN99_3)) {
    this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GREEN99_3, 0, 0, 0, 0, 0);
    this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GREEN99_5, 0, 0, 0, 0, 0);
    this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GREEN99_6, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DB09E0
// FUNCTION: fn_22629
void CPc::SetSprAccessory3(int value) {
  this->accessory3 = value;
  this->LoadEquipAction(4, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->sex, this->head, value).c_str());
  this->LoadEquipSprite(
    4, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->sex, this->head, this->accessory3).c_str()
  );
}

// OFFSET: 0x00DB0B50
// FUNCTION: fn_22631
void CPc::SetSprAccessory(int value) {
  this->accessory = value;
  this->LoadEquipAction(2, g_session.GetAccessoryActName(this->GetEffectiveJob(-1), this->sex, this->head, value).c_str());
  this->LoadEquipSprite(
    2, g_session.GetAccessorySprName(this->GetEffectiveJob(-1), this->sex, this->head, this->accessory).c_str()
  );
}

// OFFSET: 0x00DB1990
// FUNCTION: fn_22638
void CPc::SetSprHead(int value) {
  this->head = value;
  this->LoadEquipAction(1, g_session.GetHeadActName(this->appearanceJob, this->sex, &this->head).c_str());
  this->LoadEquipSprite(1, g_session.GetHeadSprName(this->appearanceJob, this->sex, &this->head).c_str());
}

// OFFSET: 0x00DB4C70
// FUNCTION: fn_22652
int CPc::MapMotion(int motion) {
  int mapped = motion;
  switch (this->GetEffectiveJob(-1)) {
    case JT_THIEF:
    case JT_THIEF_H:
    case JT_THIEF_B:
      if (motion == 8 || motion == 6 || motion == 7) {
        mapped = 2;
      }
      break;
    case JT_ASSASSIN:
    case JT_ROGUE:
    case JT_ASSASSIN_H:
    case JT_ROGUE_H:
    case JT_ASSASSIN_B:
    case JT_ROGUE_B:
    case JT_GUILLOTINE_CROSS:
    case JT_GUILLOTINE_CROSS_H:
    case JT_SHADOW_CHASER:
    case JT_SHADOW_CHASER_H:
    case JT_GUILLOTINE_CROSS_B:
    case JT_SHADOW_CHASER_B:
    case JT_SHADOW_CROSS:
      if (motion == 8 || motion == 7) {
        mapped = 2;
      } else if (motion == 6 && (
        this->GetEffectiveJob(-1) == JT_ROGUE || this->GetEffectiveJob(-1) == JT_ROGUE_H || this->GetEffectiveJob(-1) == JT_ROGUE_B
      )) {
        mapped = 1;
      }
      break;
    case JT_MAGICIAN:
    case JT_WIZARD:
    case JT_SAGE:
    case JT_MAGICIAN_H:
    case JT_WIZARD_H:
    case JT_SAGE_H:
    case JT_MAGICIAN_B:
    case JT_WIZARD_B:
    case JT_SAGE_B:
      if (motion == 5) {
        mapped = 10;
      }
      break;
  }

  return mapped;
}

// OFFSET: 0x00DAA2E0
// FUNCTION: fn_22570
__declspec(noinline) void CGrannyPc::ReleaseMeshes() {
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

  this->worldPose = 0;
}

// OFFSET: 0x00DAA3F0
// FUNCTION: fn_22571
std::vector<std::string> CPc::GetEquipActionNames() const {
  std::vector<std::string> names(9);
  for (int slot = 0; slot < 9; ++slot) {
    if (this->equipActions[slot]) {
      names[slot] = this->equipActions[slot]->GetName();
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
int CPc::GetMonsterTransformJob() {
  return CGameActor::GetMonsterTransformJob();
}

// OFFSET: 0x00DAAD80
// FUNCTION: fn_22577
__declspec(noinline) int CPc::JobActionBase(int state) {
  int base = 0;
  switch (state) {
    case ST_BEGIN_SKILL:
      base = 96;
      switch (this->GetEffectiveJob(-1)) {
        case JT_DO_SUMMONER:
        case JT_DO_SUMMONER_B:
        case JT_SPIRIT_HANDLER:
          base = 40;
          break;
      }
      break;
    case ST_SIT:
      base = 16;
      break;
    case ST_STAND:
      base = 32;
      break;
    case ST_SKILL:
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
  }

  return base;
}

// OFFSET: 0x00DAB010
// FUNCTION: fn_22578
std::vector<std::string> CPc::GetEquipSpriteNames() const {
  std::vector<std::string> names(9);
  for (int slot = 0; slot < 9; ++slot) {
    if (this->equipSprites[slot]) {
      names[slot] = this->equipSprites[slot]->GetName();
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
std::int64_t CGrannyPc::GetWeapon() {
  return this->weaponPair;
}

// OFFSET: 0x00DAB250
// FUNCTION: fn_22582
bool CPc::IsCostumeOutfit() const {
  return StateFlag12(this->stateFlags) || StateFlag16(this->stateFlags) || StateFlag18(this->stateFlags) ||
    StateFlag27(this->stateFlags) || StateFlag28(this->stateFlags) || this->mountedState == 1;
}

// OFFSET: 0x00DAB460
// FUNCTION: fn_22584
int CPc::GetDualWieldWeaponType(int first, int second) {
  int result = W_FIST;
  int main = first;
  int sub;

  if (!first) {
    main = second;
    if (!main) {
      return result;
    }

    sub = 0;
  } else {
    sub = second;
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
  if (action < 32) {
    return false;
  }

  if (action >= 40 && action < 80) {
    return false;
  }

  if (action > 95) {
    return false;
  }

  return true;
}

// OFFSET: 0x00DABE20
// FUNCTION: fn_22588
granny_world_pose* CGrannyPc::UpdateAnimation(C3dGrannyModelRes* model) {
  if (!model) {
    model = this->model;
  }

  this->clock = GrannyGetSystemSeconds();
  this->animationTime += GrannyGetSecondsElapsed(&this->lastClock, &this->clock) * 2.0f;
  this->lastClock = this->clock;

  if (model != this->model) {
    model->PlayAnimation(this->motion, GrannyInstantiateModel(model->model), nullptr, 0);
    return model->Animate(this->animationTime, GrannyInstantiateModel(model->model));
  }

  if (this->motion == 2 && this->controls[2] && !(GrannyGetControlDurationLeft(this->controls[2]) > 0.0f)) {
    this->motion = 0;
    this->controls[this->motion] = model->PlayAnimation(this->motion, GrannyInstantiateModel(model->model), this->controls[this->motion], 0);
  }

  return model->Animate(this->animationTime * g_depthAverageHalf, this->instance);
}

// OFFSET: 0x00DAC030
// FUNCTION: fn_22591
void CGrannyPc::ProcessMotion() {
  if (this->render.state == ST_ATTACK) {
    this->render.isMotionFinished = GrannyGetControlDurationLeft(this->controls[2]) < 0.1f;
  } else {
    this->render.isMotionFinished = true;
  }
}

// OFFSET: 0x00DAB210
// FUNCTION: fn_22581
bool CPc::HasAppearanceOverride() {
  return this->GetMonsterTransformJob() > 0 || g_session.IsCostumeJob(this->GetEffectiveJob(-1)) != true && this->HasShapeShift();
}

// OFFSET: 0x00DACF90
// FUNCTION: fn_22597
void CGrannyPc::RenderCached(const matrix& view, int layer, C3dGrannyModelRes* model) {
  if (!this->render.visible || this->render.accountId == 0xFFFFFFFF) {
    return;
  }

  this->brightness = (this->alpha << 24) + 0xFFFFFF;
  g_modeMgr.GetGameMode();
  CView* camera = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
  bool attached = false;
  matrix rotation;
  rotation.MakeXRotation(1.570796f);
  rotation.AppendYRotation((this->angle + 180.0f) / 180.0f * 3.141592f);
  rotation.AppendScale(0.3f, 0.3f, 0.3f);
  vector3d position;
  position.y = this->render.position.y;
  position.x = this->render.position.x - 2.0f;
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

        g_renderer->viewport.Project(&position.x, world.values, output);
      }
    }

    command->texture = this->meshTextures[layer][mesh];
    command->vertexCount = this->meshTriangleCounts[layer][mesh] * 3;
    g_renderer->queues.Submit(command, 0, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }

  if (model == this->model) {
    if (this->model->ProjectBounds(bounds, view, rotation, camera->frame.eye)) {
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
  if (this->render.state == ST_DEAD || this->isTrickDead != 0) {
    this->render.previousAnimation = this->ActionBase(this->render.state);
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
    tagRECT box;
    std::uint32_t color;
    if (this->GetMonsterTransformJob() != JT_THANATOS) {
      this->PrepareSprite(layer, layer, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, reinterpret_cast<int*>(&box), &color, 0, 0, 0);
      if (g_cameraDistance <= 320.0f) {
        quad.bottomRhw = bottomRhw - 0.0000025f;
        quad.topRhw = topRhw - 0.0000025f;
      }
    } else {
      this->PrepareSprite(index, index, distanceScale, *cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, reinterpret_cast<int*>(&box), &color, 0, 0, 0);
      if (this->render.state == ST_STAND && index == 1) {
        quad.bottomRhw = bottomRhw + 0.000008f;
        quad.topRhw = topRhw + 0.00008f;
      }
    }

    this->RaiseTransformPriority((box.bottom - box.top) / 4);
    if (this->render.translucentOther[0] == 1) {
      quad.color = quad.color & 0xFFFFFF | 0x46000000;
    }

    quad.image = image;
    if (this->render.visibilityOptions[1] != 0) {
      bounds.Include(&quad.top);
    }

    int blend = this->blend;
    const std::uint32_t* palette = sprite->palette;
    if (this->render.bodyPalette != 0 && this->GetMonsterTransformJob() < 0 && this->HasShapeShift() == true) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->render.name.c_str()))->palette;
    }

    this->ApplyLayerEffects(&quad, cell, 0, reinterpret_cast<const int*>(&box), scale, bottomRhw, bottomRhw, topRhw, x, y, palette, image, &blend, 0, 0);
    if (IsScreenShakeActive()) {
      this->DrawFlippedQuad(quad, palette, 0x101, x, y, 0);
    } else {
      this->DrawQuad(quad, palette, 0x101, 0.0f, 0.0f, 0);
    }

    this->DrawStatusOverlays(quad, palette);
  }

  this->SetRenderInfo(&bounds.left, x, y);
  this->render.effectPhaseBytes[0] = 1;
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
  this->render.distanceScale = distanceScale / g_heightScale;
  const float scale =
    g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) * this->render.scale) * this->render.distanceScale;

  RenderBounds bounds;
  bounds.left = static_cast<float>(g_renderer->viewport.width);
  bounds.top = static_cast<float>(g_renderer->viewport.height);
  bounds.right = 0.0f;
  bounds.bottom = 0.0f;
  bounds.offset = reciprocalDepth;

  ResourceMgr::Instance().Load(this->imfName.c_str());
  int count = 0;
  bool drawn = false;
  if (std::strlen(this->equipActions[0] ? this->equipActions[0]->GetName() : "") == 0) {
    return;
  }

  if (std::strlen(this->equipSprites[0] ? this->equipSprites[0]->GetName() : "") == 0) {
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
    if (!this->PrepareSprite(0, count, distanceScale, cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, 0, 0, 0)) {
      continue;
    }

    ++count;
    drawn = true;
    quad.image = image;
    if (this->render.visibilityOptions[1] != 0) {
      bounds.Include(&quad.top);
    }

    const std::uint32_t* palette;
    if (this->render.bodyPalette != 0) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->render.name.c_str()))->palette;
    } else {
      palette = sprite->palette;
    }

    int effectId;
    this->SendMsg(nullptr, AM_IS_HAVE_EFFECT, reinterpret_cast<int>(&effectId), 0, 0, 0, 0, 0);
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

  const bool result = this->ProcessState();

  this->PlayFrameSounds();

  if (this->isPlayHitWave != 0) {
    const vector3d& player =
      static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      this->hitWaveName, this->render.position.x - player.x, 0.0f,
      this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    this->isPlayHitWave = 0;
  }

  float facing = this->render.angle;
  if (facing > 360.0f) {
    facing = static_cast<float>(static_cast<int>(facing) % 360);
    this->render.angle = facing;
  }

  float angle;
  if (this->snapFacing == 1) {
    angle = 180.0f - facing;
    this->snapFacing = 0;
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

  const bool result = this->ProcessState();

  this->PlayFrameSounds();

  if (this->isPlayHitWave != 0) {
    const vector3d& player =
      static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      this->hitWaveName, this->render.position.x - player.x, 0.0f,
      this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    this->isPlayHitWave = 0;
  }

  const bool show = this->GetMonsterTransformJob() > 0;
  if (!StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags) && !StateFlag01(this->stateFlags)) {
    if (show) {
      if (!this->msgEffect) {
        CMsgEffect* effect = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->SpawnMsgEffect(44, &this->render.position, this->render.accountId, true);
        this->AddMsgEffect(effect);
        effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
        this->msgEffect = effect;
      }
    } else if (this->msgEffect) {
      this->msgEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->msgEffect = nullptr;
    }
  } else if (this->msgEffect) {
    this->msgEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
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

  this->render.translucentOther[0] = 0;
  if (g_serviceOptions.Get(TT_HIDE_PLAYER_ON_OFF) && this->render.accountId != g_session.m_aid) {
    this->render.translucentOther[0] = 1;
  }

  const float* matrix = reinterpret_cast<const float*>(argument);
  if (hasEffect == true && (!this->render.visible || !this->render.enabled) && !StateFlag06(this->stateFlags)) {
    this->SetColor(-1, 0, 0, 0);
    this->DrawLayers(matrix, 0, 0);
    return;
  }

  if (this->render.visible) {
    if (this->render.state != ST_SIT && this->render.state != ST_DEAD) {
      this->DrawShadow(argument, this->render.scale);
    }
    if (this->render.state == ST_SIT) {
      switch (this->GetEffectiveJob(-1)) {
      case JT_STAR2:
      case JT_STAR2_B:
      case JT_STAR2_EMPEROR:
      case JT_STAR2_EMPEROR_B:
      case JT_SKY_EMPEROR2:
        this->DrawShadow(argument, this->render.scale);
        break;
      }
    }

    if (this->render.enabled) {
      if (IsObfuscatedMessage(this->render.accountId)) {
        this->RenderOverlaySprite(matrix, 0);
        return;
      }

      if (this->renderWithoutLayer != 0) {
        this->DrawSprite(argument, false, 0.0f);
        return;
      }

      this->DrawLayers(matrix, 0, 0);
    } else {
      this->UpdateTlvertex(matrix);
    }
  } else {
    this->UpdateTlvertex(matrix);
  }
}

// OFFSET: 0x00DAD8C0
// FUNCTION: fn_22599
void CPc::DrawLayers(const float* matrix, int blendMode, char blendVariant) {
  SpriteLayerRenderer renderer(this);
  renderer.Render(matrix, blendMode, blendVariant);
}

// OFFSET: 0x00DABF50
// FUNCTION: fn_22589
void CGrannyPc::UpdateGaugeWindow() {
  if (static_cast<UIWindow*>(this->gaugeWindow) != nullptr) {
    static_cast<UIWindow*>(this->gaugeWindow)->Move(
      this->render.screenX - 30,
      static_cast<int>(static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.distanceScale + static_cast<float>(this->render.screenY))
    );
  }
  this->UpdateHpGauge();
}

// OFFSET: 0x00DABFC0
// FUNCTION: fn_22590
void CPc::UpdateGaugeWindow() {
  if (static_cast<UIWindow*>(this->gaugeWindow) != nullptr) {
    static_cast<UIWindow*>(this->gaugeWindow)->Move(
      this->render.screenX - 30,
      static_cast<int>(static_cast<float>(g_renderer->viewport.height * 12 / 480) * this->render.distanceScale + static_cast<float>(this->render.screenY))
    );
  }
  this->UpdateHpGauge();
}

// OFFSET: 0x00DB1760
// FUNCTION: fn_22635
void CPc::SetSprActFull(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) {
  this->appearanceJob = first;
  this->SetSexBit(second);
  this->head = third;
  this->weapon = fourth;
  this->secondaryWeapon = fifth;
  this->accessory = sixth;
  this->accessory2 = seventh;
  this->accessory3 = eighth;
  this->shield = ninth;
  this->headPalette = tenth;
  this->render.bodyPalette = eleventh;
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
  } else if (this->mountedState == 1 && !StateFlag15(this->stateFlags)) {
    job = 0x1e;
  } else if (this->isRiding != 0) {
    job = g_session.GetRidingJob(job);
  }

  this->SetActorJob(job);
  this->SetSprHead(this->head);
  this->SetSprWeapon(this->weapon, this->secondaryWeapon);
  this->SetSprAccessory(this->accessory);
  this->SetSprAccessory2(this->accessory2);
  this->SetSprAccessory3(this->accessory3);
  this->SetSprShield(this->shield);
  this->SetImfFileName();
  this->SetHeadPaletteName();
  this->SetBodyPalette(this->render.bodyPalette);
  this->SetBodyPaletteName();
}

// OFFSET: 0x00DB0CE0
// FUNCTION: fn_22633
void CPc::SetSprAct2(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) {
  this->appearanceJob = first;
  this->SetSexBit(second);
  this->head = third;
  this->weapon = fourth;
  this->secondaryWeapon = fifth;
  this->accessory = sixth;
  this->accessory2 = seventh;
  this->accessory3 = eighth;
  this->shield = GetWeaponViewValue(fifth) < 0 ? fifth : 0;
  this->headPalette = ninth;
  this->render.bodyPalette = tenth;
  this->render.bodyPalette = g_session.GetBodyStyle(first, tenth, this->bodyStyle);

  if (StateFlag12(eleventh)) {
    this->SetActorJob(0x16);
  } else if (StateFlag16(eleventh) && !StateFlag15(eleventh)) {
    this->SetActorJob(0x1a);
  } else if (StateFlag18(eleventh) && !StateFlag15(eleventh)) {
    this->SetActorJob(0x1b);
  } else if (StateFlag27(eleventh) && !StateFlag15(eleventh)) {
    this->SetActorJob(0x1c);
  } else if (StateFlag28(eleventh) && !StateFlag15(eleventh)) {
    this->SetActorJob(0x1d);
  } else if (this->mountedState == 1 && !StateFlag15(this->stateFlags)) {
    this->SetActorJob(0x1e);
  } else if (this->HasShapeShift() == true) {
    this->SetSprJob(this->GetShapeShiftJob());
  } else if (this->isRiding != 0) {
    this->SetActorJob(g_session.GetRidingJob(first));
  } else {
    this->SetActorJob(first);
  }

  this->SetSprHead(this->head);

  if (!StateFlag12(eleventh) && !StateFlag16(eleventh) && !StateFlag18(eleventh) && !StateFlag27(eleventh) &&
      !StateFlag28(eleventh) && this->mountedState != 1 && this->isRiding == 0) {
    switch (this->GetEffectiveJob(-1)) {
      case JT_ASSASSIN:
      case JT_ASSASSIN_H:
      case JT_ASSASSIN_B:
      case JT_GUILLOTINE_CROSS:
      case JT_GUILLOTINE_CROSS_H:
      case JT_GUILLOTINE_CROSS_B:
      case JT_SHADOW_CROSS:
        this->SetSprWeapon2(fourth, fifth);
        break;
      default:
        this->SetSprWeapon2(fourth, fourth >> 31);
        break;
    }
  }

  this->SetSprAccessory(this->accessory);
  this->SetSprAccessory2(this->accessory2);
  this->SetSprAccessory3(this->accessory3);

  if (!StateFlag12(eleventh) && !StateFlag16(eleventh) && !StateFlag18(eleventh) && !StateFlag27(eleventh) &&
      !StateFlag28(eleventh) && this->mountedState != 1 && this->isRiding == 0) {
    this->SetSprShield(this->shield);
  }

  this->SetImfFileName();
  this->SetHeadPaletteName();
  this->SetBodyPaletteName();
}

// OFFSET: 0x00DB1AE0
// FUNCTION: fn_22640
void CPc::SetSprJob(int job) {
  if (job == JT_BARD || job == JT_BARD_H || job == JT_BARD_B) {
    if (this->sex == 0) {
      this->sex = 1;
    }
  } else if (job == JT_DANCER || job == JT_DANCER_H || job == JT_DANCER_B) {
    if (this->sex == 1) {
      this->sex = 0;
    }
  } else if (job == JT_MINSTREL) {
    if (this->sex == 0) {
      this->sex = 1;
    }
  } else if (job == JT_WANDERER) {
    if (this->sex == 1) {
      this->sex = 0;
    }
  } else if (job == JT_MINSTREL_H) {
    if (this->sex == 0) {
      this->sex = 1;
    }
  } else if (job == JT_WANDERER_H) {
    if (this->sex == 1) {
      this->sex = 0;
    }
  } else if (job == JT_TROUBADOUR) {
    if (this->sex == 0) {
      this->sex = 1;
    }
  } else if (job == JT_TROUVERE) {
    if (this->sex == 1) {
      this->sex = 0;
    }
  }

  this->render.shadowScale = g_session.GetNpcLocationRadius(job, this->isRiding == 1);

  if (this->GetMonsterTransformJob() > 0) {
    this->LoadEquipAction(0, g_session.GetMonsterActName(this->GetMonsterTransformJob()).c_str());
    this->LoadEquipSprite(0, g_session.GetMonsterSprName(this->GetMonsterTransformJob()).c_str());
  } else if (StateFlag12(this->stateFlags) == true || job == JT_MARRIED) {
    this->LoadEquipAction(0, g_session.GetRaceBodyResourceName(0, this->appearanceJob, JT_MARRIED, this->sex, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetRaceBodyResourceName(1, this->appearanceJob, JT_MARRIED, this->sex, 0).c_str());
  } else if (StateFlag16(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, g_session.GetBodyActionResourceName(JT_SANTA, this->sex, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_SANTA, this->sex, 0, 0).c_str());
  } else if (StateFlag18(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, g_session.GetBodyActionResourceName(JT_SUMMER, this->sex, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_SUMMER, this->sex, 0, 0).c_str());
  } else if (StateFlag27(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, g_session.GetBodyActionResourceName(JT_HANBOK, this->sex, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_HANBOK, this->sex, 0, 0).c_str());
  } else if (StateFlag28(this->stateFlags) && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, g_session.GetBodyActionResourceName(JT_OKTOBERFEST, this->sex, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_OKTOBERFEST, this->sex, 0, 0).c_str());
  } else if (this->mountedState == 1 && !StateFlag15(this->stateFlags)) {
    this->LoadEquipAction(0, g_session.GetBodyActionResourceName(JT_SUMMER2, this->sex, 0, 0).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(JT_SUMMER2, this->sex, 0, 0).c_str());
  } else if (this->isRiding != 0) {
    this->LoadEquipAction(
      0, g_session.GetBodyActionResourceName(g_session.GetRidingJob(job), this->sex, this->bodyStyle, 0).c_str()
    );
    this->LoadEquipSprite(0, g_session.GetBodySprName(g_session.GetRidingJob(job), this->sex, this->bodyStyle, 0).c_str());
  } else {
    const int effectiveJob = this->GetEffectiveJob(JobNames::BaseJob(job));
    switch (effectiveJob) {
      case JT_MADOGEAR:
      case JT_MADOGEAR_H:
      case JT_MEISTER_MADOGEAR:
        if (this->GetMadogearType() == 2) {
          this->render.shadowScale = 1.5f;
        }
        break;
      case JT_MADOGEAR_B:
        if (this->GetMadogearType() == 2) {
          this->render.shadowScale = 1.12f;
        }
        break;
    }

    this->LoadEquipAction(
      0, g_session.GetBodyActionResourceName(effectiveJob, this->sex, this->bodyStyle, this->GetMadogearType()).c_str()
    );
    this->LoadEquipSprite(0, g_session.GetBodySprName(effectiveJob, this->sex, this->bodyStyle, this->GetMadogearType()).c_str());
  }

  this->SetActionName(this->equipActions[0] ? this->equipActions[0]->GetName() : "");
  this->SetSpriteName(this->equipSprites[0] ? this->equipSprites[0]->GetName() : "");
  this->SetImfFileName();
}

// OFFSET: 0x00DB4E70
// FUNCTION: fn_22653
__declspec(noinline) void CPlayer::UpdateMsgEffect(bool show) {
  if (!StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags) && !StateFlag01(this->stateFlags)) {
    if (show) {
      if (!this->msgEffect) {
        CMsgEffect* effect = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->SpawnMsgEffect(44, &this->render.position, this->render.accountId, true);
        this->AddMsgEffect(effect);
        effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
        this->msgEffect = effect;
      }
      return;
    }
  }

  if (this->msgEffect) {
    this->msgEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
    this->msgEffect = nullptr;
  }
}

// OFFSET: 0x00DB0FC0
// FUNCTION: fn_22634
void CGrannyPc::SetSprActFull(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh) {
  this->SetSexBit(second);
  this->head = third;
  this->weapon = fourth;
  this->accessory = sixth;
  this->accessory2 = seventh;
  this->accessory3 = eighth;
  this->shield = ninth;
  this->secondaryWeapon = fifth;
  this->headPalette = tenth;
  this->render.bodyPalette = eleventh;
  this->SetActorJob(first);
  this->SetSprHead(this->head);
  this->SetSprWeapon(this->weapon, this->secondaryWeapon);
  this->SetSprAccessory(this->accessory);
  this->SetSprAccessory2(this->accessory2);
  this->SetSprAccessory3(this->accessory3);
  this->SetSprShield(this->shield);
  this->SetImfFileName();
  this->SetHeadPaletteName();

  if (this->model) {
    this->ReleaseMeshes();
  }

  std::strcpy(this->modelName, "SWM@hd기본_3.gr2");
  this->model = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->instance = GrannyInstantiateModel(this->model->model);
  for (int mesh = 0; mesh < this->model->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->commands[mesh] = command;
    command->AllocateTriangles(this->model->GetTriangleCount(mesh));
    this->vertices[mesh] = new RenderVertex[this->model->GetTriangleCount(mesh) * 3];
    std::memset(this->vertices[mesh], 0, this->model->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  this->lastClock = GrannyGetSystemSeconds();
  this->animationTime = 0.0f;
  this->actorType = 0;

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
    this->layerCommands[1][mesh] = command;
    command->AllocateTriangles(this->layerModels[1]->GetTriangleCount(mesh));
    this->layerVertices[1][mesh] = new RenderVertex[this->layerModels[1]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[1][mesh], 0, this->layerModels[1]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  tag[1] = 'b';
  tag[2] = 'd';
  this->layerModels[2] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->layerModels[2]->RebindMeshes(*this->model);
  for (int mesh = 0; mesh < this->layerModels[2]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[2][mesh] = command;
    command->AllocateTriangles(this->layerModels[2]->GetTriangleCount(mesh));
    this->layerVertices[2][mesh] = new RenderVertex[this->layerModels[2]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[2][mesh], 0, this->layerModels[2]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  tag[1] = 'p';
  tag[2] = 't';
  this->layerModels[3] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->layerModels[3]->RebindMeshes(*this->model);
  for (int mesh = 0; mesh < this->layerModels[3]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[3][mesh] = command;
    command->AllocateTriangles(this->layerModels[3]->GetTriangleCount(mesh));
    this->layerVertices[3][mesh] = new RenderVertex[this->layerModels[3]->GetTriangleCount(mesh) * 3];
    std::memset(this->layerVertices[3][mesh], 0, this->layerModels[3]->GetTriangleCount(mesh) * 3 * sizeof(RenderVertex));
  }

  tag[1] = 'l';
  tag[2] = 'e';
  this->layerModels[4] = static_cast<C3dGrannyModelRes*>(ResourceMgr::Instance().Load(this->modelName));
  this->layerModels[4]->RebindMeshes(*this->model);
  for (int mesh = 0; mesh < this->layerModels[4]->instance->meshCount; ++mesh) {
    ModelRenderCommand* command = new ModelRenderCommand;
    this->layerCommands[4][mesh] = command;
    command->AllocateTriangles(this->layerModels[4]->GetTriangleCount(mesh));
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
void CPc::SetBodyPaletteName() {
  if (this->render.bodyPalette == 0) {
    return;
  }

  if (this->GetMonsterTransformJob() < 0 && this->IsShapeShifted()) {
    std::string paletteName;
    this->render.name.assign(static_cast<const std::string&>(FormatOwnedString("몸\\%s_%d.pal", this->GetShapeShiftName().c_str(), this->render.bodyPalette)));
    return;
  }

  int job = this->GetEffectiveJob(-1);
  const int costumeJob = this->bodyStyle;
  if (this->isRiding != 0) {
    job = g_session.GetRidingJob(job);
  }

  this->render.name.assign(static_cast<const std::string&>(g_session.GetBodyPalName(job, this->sex, this->render.bodyPalette, costumeJob, this->GetMadogearType(), 0)));
}

// OFFSET: 0x00DB2670
// FUNCTION: fn_22645
void CPc::SetSprShield(int value) {
  this->shield = value;

  if (this->isRiding != 0) {
    this->LoadEquipAction(7, "");
    this->LoadEquipSprite(7, "");
    return;
  }

  const int job = this->GetEffectiveJob(-1);
  this->LoadEquipAction(7, g_session.GetShieldActName(job, this->sex, this->shield, this->bodyStyle).c_str());
  this->LoadEquipSprite(7, g_session.GetShieldSprName(job, this->sex, this->shield, this->bodyStyle).c_str());
}

// OFFSET: 0x00DB6670
// FUNCTION: fn_22668
void CPlayer::ApplyFollowOffset(CGameActor* target, float x, float z, int* row, int* column) {
  int step = target->GetNpcLocationRadius();
  if (target->actorType == 1) {
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
  float adjusted;
  if (target->actorType == 1) {
    adjusted = distance - 20.0f;
  } else if (target->GetNpcLocationRadius() > 0) {
    adjusted = distance - static_cast<float>(target->GetNpcLocationRadius() + 1) * 4.0f;
  } else {
    adjusted = distance;
  }

  return std::max(adjusted, 0.0f);
}

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
  this->ProcessDamageMessages();
  this->UpdateGaugePositions();

  if (this->render.isSingleColor == true && timeGetTime() > this->render.started + 100) {
    this->render.overrideColor = 0xFFFF0000;
    this->render.isSingleColor = false;
  }

  if (this->render.color != 0xFFFFFFFF) {
    this->ModifyArgb(8, 8, 8, 8);
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (this->preMoveOn != 0 && timeGetTime() > (g_session.m_averagePing >> 1) + this->preMoveStartTick) {
    PlayerMoveRequest request;
    mode->WorldSubcell(
      this->render.position.x, this->render.position.z, request.column, request.row, request.subColumn, request.subRow
    );
    request.serverTime = g_session.GetServerTime();
    request.destinationX = this->destCellX;
    request.destinationY = this->destCellZ;
    const long long address = reinterpret_cast<int>(&request);
    this->SendMsg(nullptr, AM_MOVE, address, 0, 0, 0, 0, 0);
  }

  this->UpdatePendingAction();

  const bool result = this->ProcessState();

  this->PlayFrameSounds();

  if (this->isPlayHitWave != 0) {
    const vector3d& player =
      static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
    g_soundMgr->PlayWave(
      this->hitWaveName, this->render.position.x - player.x, 0.0f,
      this->render.position.z - player.z, 250, 40, 1.0f, 0
    );
    this->isPlayHitWave = 0;
  }

  this->UpdateMsgEffect(this->GetMonsterTransformJob() > 0);

  if (g_session.showCameraInfo) {
    UIWindow* window = g_windowMgr.LookupWindow(WID_VIEWCAMERAINFOWND);
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
  switch (this->proceedType) {
  case PT_ATTACK:
  case PT_ATTACK_2: {
    CGameActor* target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(this->proceedTargetGid));
    if (target == nullptr) {
      break;
    }
    if (target->render.state == ST_DEAD) {
      return;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    if (this->GetEffectiveJob(-1) == 16 || this->GetEffectiveJob(-1) == 4017 || this->GetEffectiveJob(-1) == 4039) {
      const int state = static_cast<int>(this->render.state);
      if (state != ST_MOVE && state != ST_STAND && state != ST_DAMAGE && state != ST_SKILL && state != ST_BEGIN_SKILL) {
        return;
      }
    } else if ((this->GetEffectiveJob(-1) >= 4046 && this->GetEffectiveJob(-1) < 4361) || this->GetEffectiveJob(-1) == 24 ||
               this->GetEffectiveJob(-1) == 25) {
      const int state = static_cast<int>(this->render.state);
      if (state != ST_MOVE && state != ST_STAND && state != ST_DAMAGE && state != ST_SKILL && state != ST_BEGIN_SKILL && state < 16) {
        return;
      }
    } else {
      const int state = static_cast<int>(this->render.state);
      if (state != ST_MOVE && state != ST_STAND && state != ST_DAMAGE) {
        return;
      }
    }

    const float distance = this->AdjustFollowDistance(
      target, target->CalcDist(this->render.position.x, this->render.position.y, this->render.position.z)
    );
    const bool inRange = static_cast<float>(g_session.storeItemLimit * 5) + 5.5f >= distance;
    if ((this->destCellX != column || this->destCellZ != row) && !inRange) {
      const std::uint32_t delay = this->proceedType != 5 ? 450 : 1200;
      const bool elapsed = timeGetTime() > this->tickOfMoveForAttack + delay;
      const int state = static_cast<int>(this->render.state);
      int cellX = target->movementState[5];
      int cellY = target->destCellY;
      float x;
      float z;
      if (this->proceedType == 5) {
        x = target->render.position.x;
        z = target->render.position.z;
      } else {
        mode->CellCenter(cellX, cellY, x, z);
      }
      this->ApplyFollowOffset(target, x, z, &cellY, &cellX);
      mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, cellX, cellY, g_session.storeItemLimit);
      if ((this->destCellX != cellX || this->destCellZ != cellY) && state != ST_ATTACK &&
          this->isAttackRequest == 0 && elapsed) {
        this->destCellX = cellX;
        this->destCellZ = cellY;
        this->tickOfMoveForAttack = timeGetTime();
        this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, cellX, cellY, 0, 0, 0, 0);
      }
      return;
    }

    if (this->attackMode == 2) {
      this->SendMsg(nullptr, AM_CANCELATTACK, 0, 0, 0, 0, 0, 0);
    }
    if (this->isAttackRequest != 0 || timeGetTime() <= this->attackReqTime + 1200) {
      return;
    }
    ++this->totalAttackReqCnt;
    this->attackReqTime = timeGetTime();
    this->isAttackRequest = 1;
    if (this->attackMode != 2) {
      this->SendMsg(nullptr, AM_REQ_ATTACK_REPEAT, 0, 0, 0, 0, 0, 0);
    } else {
      this->SendMsg(nullptr, AM_REQ_ATTACK, 0, 0, 0, 0, 0, 0);
    }
    return;
  }
  case PT_SKILL: {
    CGameActor* target;
    if (this->proceedTargetGid == g_session.m_aid) {
      target = this;
    } else {
      target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(this->proceedTargetGid));
    }
    if (target == nullptr) {
      break;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    const float x = target->render.position.x;
    int cellX = target->movementState[5];
    int cellY = target->destCellY;
    const float z = target->render.position.z;
    const int range = mode->GetSkillAttackRange(this->skillId, this->skillUseLevel);
    mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, cellX, cellY, range);
    const int state = static_cast<int>(this->render.state);
    if (state != ST_MOVE && state != ST_STAND && state != ST_DAMAGE && state < 16) {
      return;
    }

    const float distance = this->AdjustFollowDistance(
      target, target->CalcDist(this->render.position.x, this->render.position.y, this->render.position.z)
    );
    const bool inRange = static_cast<float>(range * 5) + 5.5f >= distance;
    if ((this->destCellX == column && this->destCellZ == row) || inRange) {
      this->SendMsg(nullptr, AM_REQ_SKILL, 0, 0, 0, 0, 0, 0);
      break;
    }

    if (this->destCellX == cellX && this->destCellZ == cellY) {
      return;
    }
    if (this->render.state == ST_ATTACK) {
      return;
    }
    this->destCellZ = cellY;
    this->destCellX = cellX;
    this->ApplyFollowOffset(target, x, z, &cellY, &cellX);
    this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, cellX, cellY, 0, 0, 0, 0);
    return;
  }
  case PT_GROUNDSKILL: {
    int groundX = this->gSkillDx;
    int groundY = this->gSkillDy;
    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    const int range = mode->GetSkillAttackRange(this->skillId, this->skillUseLevel);
    float x;
    float z;
    mode->CellCenter(groundX, groundY, x, z);
    mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, groundX, groundY, range);
    const int state = static_cast<int>(this->render.state);
    if (state != ST_MOVE && state != ST_STAND && state != ST_DAMAGE && state < 16) {
      return;
    }

    const float distance = this->CalcDist(x, this->render.position.y, z);
    const bool inRange = static_cast<float>(range * 5) + 5.5f >= distance;
    if ((this->destCellX == column && this->destCellZ == row) || inRange) {
      this->SendMsg(nullptr, AM_REQ_GROUND_SKILL, 0, 0, 0, 0, 0, 0);
      break;
    }

    if (this->destCellX == groundX && this->destCellZ == groundY) {
      return;
    }
    if (this->render.state == ST_ATTACK) {
      return;
    }
    this->destCellZ = groundY;
    this->destCellX = groundX;
    this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, groundX, groundY, 0, 0, 0, 0);
    return;
  }
  case PT_PICKUPITEM: {
    CItem* item = mode->scene->GetItemByAID(this->proceedTargetGid);
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
    const int state = static_cast<int>(this->render.state);
    if (state != ST_STAND && state < 16) {
      return;
    }

    if (mode->IsWithinRange(column, row, targetX, targetY, 1)) {
      this->SendMsg(nullptr, AM_REQ_CHANGE_DIR, targetX, targetY, 0, 0, 0, 0);
      if (!mode->HasPendingItemPickup(this->proceedTargetGid)) {
        mode->AddPendingItemPickup(this->proceedTargetGid);
        mode->SendMsg(MM_ITEMPICKUP, this->proceedTargetGid, 0, 0, 0);
      }
      break;
    }

    if (this->destCellX == itemX && this->destCellZ == itemY) {
      return;
    }
    this->destCellZ = itemY;
    this->destCellX = itemX;
    this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, itemX, itemY, 0, 0, 0, 0);
    return;
  }
  case PT_TOUCH_SKILL: {
    CGameActor* skill = static_cast<CGameActor*>(mode->scene->GetSkillByAID(this->proceedTargetGid));
    if (skill == nullptr) {
      break;
    }

    int column;
    int row;
    mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
    int cellX;
    int cellY;
    mode->WorldCell(skill->render.position.x, skill->render.position.z, cellX, cellY);
    int range = mode->GetSkillAttackRange(this->skillId, this->skillUseLevel);
    if (mode->SendMsg(MM_GET_SKILL_USE_TYPE, 0, 0, 0, 0) == 0 && range < 5 && this->skillId != 124) {
      range = g_session.storeItemLimit > 5 ? g_session.storeItemLimit : 5;
    }
    float x;
    float z;
    mode->CellCenter(cellX, cellY, x, z);
    mode->FindWalkableCellToward(this->render.position.x, this->render.position.z, x, z, cellX, cellY, range);
    const int state = static_cast<int>(this->render.state);
    if (state != ST_MOVE && state != ST_STAND && state != ST_DAMAGE && state < 16) {
      return;
    }

    const float distance = this->CalcDist(x, this->render.position.y, z);
    const bool inRange = static_cast<float>(range * 5) + 5.5f >= distance;
    if ((this->destCellX != column || this->destCellZ != row) && !inRange) {
      if (this->destCellX == cellX && this->destCellZ == cellY) {
        return;
      }
      if (this->render.state == ST_ATTACK && skill->GetEffectiveJob(-1) != UNT_ICEWALL && skill->GetEffectiveJob(-1) != UNT_BLASTMINE &&
          skill->GetEffectiveJob(-1) != UNT_CLAYMORETRAP) {
        return;
      }
      this->destCellZ = cellY;
      this->destCellX = cellX;
      this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, cellX, cellY, 0, 0, 0, 0);
      return;
    }

    switch (skill->GetEffectiveJob(-1)) {
    case UNT_ICEWALL:
    case UNT_BLASTMINE:
    case UNT_CLAYMORETRAP:
    case UNT_REVERBERATION:
    case UNT_NETHERWORLD:
    case UNT_WALLOFTHORN:
    case UNT_DEEPBLINDTRAP:
    case UNT_SOLIDTRAP:
    case UNT_SWIFTTRAP:
    case UNT_FLAMETRAP:
      if (this->skillId == 0) {
        if (this->attackMode == 2) {
          this->SendMsg(nullptr, AM_CANCELATTACK, 0, 0, 0, 0, 0, 0);
        }
        if (this->isAttackRequest != 0 || timeGetTime() <= this->attackReqTime + 1200) {
          return;
        }
        ++this->totalAttackReqCnt;
        this->attackReqTime = timeGetTime();
        this->isAttackRequest = 1;
        if (this->attackMode != 2) {
          this->SendMsg(nullptr, AM_REQ_ATTACK_REPEAT, 0, 0, 0, 0, 0, 0);
        } else {
          this->SendMsg(nullptr, AM_REQ_ATTACK, 0, 0, 0, 0, 0, 0);
        }
        return;
      }
      break;
    }
    this->SendMsg(nullptr, AM_REQ_SKILL, 0, 0, 0, 0, 0, 0);
    break;
  }
  default:
    return;
  }

  this->SendMsg(nullptr, AM_CANCELATTACK, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00DB68B0
// FUNCTION: fn_22670
void CPlayer::ReapplySkillEffects() {
  if (this == nullptr) {
    return;
  }

  g_weatherEffect.StopDarken();
  this->characterLevel = static_cast<std::uint16_t>(g_session.m_level);
  this->UpdateMaxLevelAura();

  CTetraBallEffect* tetraBall;
  for (int id = 0x4EE; id <= 0x4F1; ++id) {
    tetraBall = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Find(this->render.accountId, id));
    if (tetraBall) {
      break;
    }
  }

  for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end(); ++it) {
    const int type = it->id;
    const int value = it->value1;
    const int argument3 = it->value2;
    const int argument4 = it->value3;

    switch (type) {
    case EFST_TWOHANDQUICKEN: {
      CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x82, 0, 0, 0);
      }
      break;
    }
    case EFST_ONEHANDQUICKEN: {
      CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x82, 0, 0, 0);
      }
      break;
    }
    case EFST_NJ_BUNSINJYUTSU: {
      CRagEffect* effect = this->LaunchEffect(EF_BUNSINJYUTSU, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x269, 0, 0, 0);
      }
      break;
    }
    case EFST_OVERTHRUST: {
      CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x80, 0, 0, 0);
      }
      break;
    }
    case EFST_OVERTHRUSTMAX: {
      CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x80, 0, 0, 0);
      }
      break;
    }
    case EFST_ENERGYCOAT: {
      CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xA9, 0, 0, 0);
      }
      break;
    }
    case EFST_SPEARQUICKEN: {
      CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xFA, 0, 0, 0);
      }
      break;
    }
    case EFST_EXPLOSIONSPIRITS:
      this->LaunchEffect(EF_GUMGANG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_STEELBODY:
      this->LaunchEffect(EF_STEELBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_SU_STOOP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SG_SUN_WARM:
    case EFST_SG_MOON_WARM:
    case EFST_SG_STAR_WARM:
      this->LaunchEffect(EF_DOUBLEGUMGANG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_REDLIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_KAITE:
      this->LaunchEffect(EF_REFLECTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_AURABLADE:
      this->LaunchEffect(EF_AURABLADE2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_BERSERK:
      this->LaunchEffect(EF_REDBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_LKCONCENTRATION:
      this->LaunchEffect(EF_LKCONCENTRATION, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_MAGICPOWER:
      this->LaunchEffect(EF_LIGHTBLADE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_MOON:
      this->LaunchEffect(EF_SPHEREWIND2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_DEVIL1: {
      CSkillInfo skill = g_session.m_skillBundle.Lookup(0x1B9);
      int effectId = skill.level;
      if (effectId > 0) {
        CSkillInfo upgrade = g_session.m_skillBundle.Lookup(0xA1D);
        if (upgrade.level > 0) {
          effectId = 5;
        }
        effectId += 0x1DA;
        if (!this->HasId(effectId)) {
          this->LaunchEffect(EF_BLACKDEVIL, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          g_soundMgr->PlayWave("effect\\_blind.wav", 0.0f, -100.0f, 0.0f, 250, 40, 1.0f, 0);
        }
        for (int id = 0x1DB; id < 0x1E5; ++id) {
          if (this->HasId(id) == true) {
            this->SendMsg(nullptr, AM_DELETE_EFFECT, id, 0, 0, 0, 0, 0);
          }
        }
        this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STATUS_STATE, 0, 0, 0, 0, 0);
      this->skillEffect = this->LaunchEffect(EF_STATUS_STATE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_SOULLINK:
      g_weatherEffect.StartDarken();
      this->LaunchEffect(EF_ASURABODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_PROPERTYUNDEAD:
      this->LaunchEffect(EF_UNDEADBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SKE:
      g_weatherEffect.StartDarken();
      break;
    case EFST_DAURA:
      this->LaunchEffect(EF_DOUBLEGUMGANG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_REDLIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_DBARRIER:
      this->LaunchEffect(EF_STEELBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_SU_STOOP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_DA_CONTRACT:
      this->LaunchEffect(EF_M04, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_DUPLELIGHT: {
      while (this->HasId(EF_CHOOKGI_N)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CHOOKGI_N, 0, 0, 0, 0, 0);
      }
      CRagEffect* effect = this->LaunchEffect(EF_CHOOKGI_N, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      vector3d scale{2.0f, 0.0f, 0.0f};
      if (effect) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&scale), 0, 0, 0, 0);
      }
      break;
    }
    case EFST_ORATIO:
      this->LaunchEffect(EF_ORATIO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_BLOOD_SUCKER:
      this->LaunchEffect(EF_REDBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_STRIKING: {
      while (this->HasId(EF_MAKEHALLU2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEHALLU2, 0, 0, 0, 0, 0);
      }
      CRagEffect* effect = this->LaunchEffect(EF_MAKEHALLU2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x39D, 0, 0, 0);
      }
      break;
    }
    case EFST_VENOMIMPRESS:
      this->LaunchEffect(EF_VENOMIMPRESS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_FROSTMISTY:
      this->LaunchEffect(EF_FROSTMISTY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_COLD:
      this->LaunchEffect(EF_COLDSTATUS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_CAMOUFLAGE:
      this->LaunchEffect(EF_CLOAKING2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SHADOWFORM:
      this->LaunchEffect(EF_CLOAKING3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_MANHOLE:
      g_soundMgr->PlayWave("effect\\manhole.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      this->LaunchEffect(EF_MANHOLE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_STEALTHFIELD:
      this->LaunchEffect(EF_CLOAKING2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_CRYSTAL:
      this->LaunchEffect(EF_CRYSTAL_BLUE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SUMMON1:
    case EFST_SUMMON2:
    case EFST_SUMMON3:
    case EFST_SUMMON4:
    case EFST_SUMMON5:
      if (!tetraBall) {
        CTetraBallEffect* ball;
        for (int id = 0x4EE; id <= 0x4F1; ++id) {
          ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Find(this->render.accountId, id));
          if (ball) {
            break;
          }
        }
        if (ball) {
          switch (value) {
          case 0:
            ball->AddBall(0x4EE, type - 0x16B);
            break;
          case 1:
            ball->AddBall(0x4F0, type - 0x16B);
            break;
          case 2:
            ball->AddBall(0x4EF, type - 0x16B);
            break;
          case 3:
            ball->AddBall(0x4F1, type - 0x16B);
            break;
          }
        } else {
          switch (value) {
          case 0:
            ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
            ball->MoveFirstTo(type - 0x16B);
            break;
          case 1:
            ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
            ball->MoveFirstTo(type - 0x16B);
            break;
          case 2:
            ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
            ball->MoveFirstTo(type - 0x16B);
            break;
          case 3:
            ball = dynamic_cast<CTetraBallEffect*>(g_effectMgr->Spawn(this->render.accountId, EF_TETRA_VORTEX_BALL_4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
            ball->MoveFirstTo(type - 0x16B);
            break;
          }
        }
      }
      break;
    case EFST_ROLLINGCUTTER:
      if (value < 10) {
        if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
          g_effectMgr->Spawn(this->render.accountId, value + 0x5BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        } else {
          g_effectMgr->Spawn(this->render.accountId, value + 0x513, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
      }
      break;
    case EFST_MELODYOFSINK:
      if (this->HasId(EF_LEXDIVINA) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEXDIVINA, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_LEXDIVINA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_BEYOND_OF_WARCRY:
      if (this->HasId(EF_STASIS) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STASIS, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_STASIS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SATURDAY_NIGHT_FEVER:
      if (this->HasId(EF_ENERVATION7) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ENERVATION7, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_ENERVATION7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_EQUIPED_FLOOR:
      this->SendMsg(nullptr, AM_STATUS_CHANGE, 0x23B, 1, value, 0, 0, 0);
      break;
    case EFST_ALL_RIDING:
      this->SetRiding(1);
      this->SetSprJob(this->GetEffectiveJob(-1));
      this->SetBodyPalette(this->render.bodyPalette);
      break;
    case EFST_ON_PUSH_CART:
      this->SendMsg(nullptr, AM_STATUS_CHANGE, type, 1, g_session.replayPacketMode, 0, 0, 0);
      break;
    case EFST_TELEKINESIS_INTENSE:
      while (this->HasId(EF_WL_TELEKINESIS_INTENSE)) {
        this->SendMsg(this, AM_DELETE_EFFECT, EF_WL_TELEKINESIS_INTENSE, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_WL_TELEKINESIS_INTENSE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_WIDEWEB:
      while (this->HasId(EF_BOTTOM_SPIDER)) {
        this->SendMsg(this, AM_DELETE_EFFECT, EF_BOTTOM_SPIDER, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_BOTTOM_SPIDER, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_BURNT:
      while (this->HasId(EF_NPC_BURNT)) {
        this->SendMsg(this, AM_DELETE_EFFECT, EF_NPC_BURNT, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_NPC_BURNT, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_CHILL:
      while (this->HasId(EF_NPC_CHILL)) {
        this->SendMsg(this, AM_DELETE_EFFECT, EF_NPC_CHILL, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_NPC_CHILL, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_OFFERTORIUM:
      while (this->HasId(EF_AB_OFFERTORIUM_RING)) {
        this->SendMsg(this, AM_DELETE_EFFECT, EF_AB_OFFERTORIUM_RING, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_AB_OFFERTORIUM_RING, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_ZANGETSU:
      while (this->HasId(EF_BLUELIGHTBODY)) {
        this->SendMsg(this, AM_DELETE_EFFECT, EF_BLUELIGHTBODY, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_BLUELIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_GENSOU:
      g_effectMgr->Retire(this->render.accountId, 0x3ED);
      if (g_effectMgr->HasAllocator(0x3ED)) {
        CEffect* orb = g_effectMgr->Spawn(this->render.accountId, EF_OB_GENSOU, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (orb) {
          static_cast<CEnergyOrbEffect*>(orb)->SetCount(10);
        }
      }
      while (this->HasId(EF_GRAYBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GRAYBODY, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_GRAYBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_KAGEMUSYA:
      while (this->HasId(EF_KAGEMUSYA)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_KAGEMUSYA, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_KAGEMUSYA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_ANTI_M_BLAST:
      g_effectMgr->Retire(this->render.accountId, 0x470);
      if (g_effectMgr->HasAllocator(0x470) == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_RL_ANTI_MATERIAL_BLAST, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_FLOWER_LEAF:
      g_effectMgr->Retire(this->render.accountId, 0x3F5);
      if (g_effectMgr->HasAllocator(0x3F5) == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_CHERRYBLOSSOM, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_DARKCROW:
      while (this->HasId(EF_GC_DARKCROW)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GC_DARKCROW, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_GC_DARKCROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_GC_DARKCROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_GC_DARKCROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_HALLUCINATIONWALK:
    case EFST_NPC_HALLUCINATIONWALK: {
      while (this->HasId(EF_MAKEHALLU)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEHALLU, 0, 0, 0, 0, 0);
      }
      CRagEffect* effect = this->LaunchEffect(EF_MAKEHALLU, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x318, 0, 0, 0);
      }
      break;
    }
    case EFST_ASSUMPTIO:
    case EFST_ASSUMPTIO2:
    case EFST_ASSUMPTIO_BUFF:
      this->LaunchEffect(EF_ASSUMPTIO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SPRITEMABLE:
      g_effectMgr->Retire(this->render.accountId, 0x448);
      if (g_effectMgr->HasAllocator(0x448) == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_SPRITEMABLE, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_SUHIDE:
      this->SetState(0x30, 1);
      this->render.isMotionFreezed = true;
      break;
    case EFST_TUNAPARTY:
      g_effectMgr->Retire(this->render.accountId, 0x449);
      if (g_effectMgr->HasAllocator(0x449) == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_TUNAPARTY, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_SV_ROOTTWIST:
      g_effectMgr->Retire(this->render.accountId, 0x456);
      if (g_effectMgr->HasAllocator(0x456) == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_SU_SV_ROOTTWIST, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_BITESCAR:
      g_effectMgr->Retire(this->render.accountId, 0x45F);
      if (g_effectMgr->HasAllocator(0x45F) == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_BITE_SCAR, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_SOULATTACK:
      this->alternateDoramAppearance[0] = 1;
      break;
    case EFST_IMMUNE_PROPERTY_NOTHING:
    case EFST_IMMUNE_PROPERTY_WATER:
    case EFST_IMMUNE_PROPERTY_GROUND:
    case EFST_IMMUNE_PROPERTY_FIRE:
    case EFST_IMMUNE_PROPERTY_WIND:
    case EFST_IMMUNE_PROPERTY_POISON:
    case EFST_IMMUNE_PROPERTY_SAINT:
    case EFST_IMMUNE_PROPERTY_DARKNESS:
    case EFST_IMMUNE_PROPERTY_TELEKINESIS:
    case EFST_IMMUNE_PROPERTY_UNDEAD:
      g_effectMgr->Spawn(this->render.accountId, type + 0x87, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_GRAVITYCONTROL:
      if (g_effectMgr->Has(this->render.accountId, 0x4B1) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4B1);
      }
      g_effectMgr->Spawn(this->render.accountId, 0x4B1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\t_캐스팅.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case EFST_SUNSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4BE) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BE);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_SUN_STANCE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_LUNARSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4BF) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BF);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_LUNAR_STANCE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_STARSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4C0) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C0);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_STAR_STANCE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_UNIVERSESTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4C1) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C1);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_UNIVERSE_STANCE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_FLASHKICK:
      if (g_effectMgr->Has(this->render.accountId, 0x4C5) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C5);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_FLASH_KICK, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_SOULUNITY:
      if (g_effectMgr->Has(this->render.accountId, 0x4C6) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C6);
      }
      g_effectMgr->Spawn(this->render.accountId, EF_SOUL_UNITY, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\t_바람방출.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case EFST_SOULSHADOW:
    case EFST_SOULFAIRY:
    case EFST_SOULFALCON:
    case EFST_SOULGOLEM:
      g_weatherEffect.StartDarken();
      this->LaunchEffect(EF_ASURABODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_SOUND_OF_DESTRUCTION:
      g_effectMgr->Retire(this->render.accountId, 0x66C);
      g_effectMgr->Spawn(this->render.accountId, EF_NEW_SOUNDOFDESTRUCTION_TARGET, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_DEADLY_DEFEASANCE:
      if (!g_effectMgr->Has(this->render.accountId, 0x6CD)) {
        g_effectMgr->Spawn(this->render.accountId, EF_DEADLY_PROJECTION, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_DEEPBLIND:
      if (this->HasId(EF_BLIND) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BLIND, 0, 0, 0, 0, 0);
      }
      this->LaunchEffect(EF_BLIND, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_overrideColor = 1;
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STATUS_STATE, 0, 0, 0, 0, 0);
      this->skillEffect = this->LaunchEffect(EF_STATUS_STATE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    case EFST_HANDICAPSTATE_DEEPSILENCE:
    case EFST_HANDICAPSTATE_SWOONING:
      this->SendMsg(nullptr, AM_MAKE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
      break;
    case EFST_HANDICAPSTATE_LASSITUDE:
      this->SendMsg(nullptr, AM_MAKE_LASSITUDE_EFFECT, 0, 0, 0, 0, 0, 0);
      break;
    case EFST_HANDICAPSTATE_FROSTBITE:
      g_effectMgr->Spawn(this->render.accountId, EF_STATE_FROSTBITE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_FROSTBITE_SCREEN, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_HANDICAPSTATE_LIGHTNINGSTRIKE:
      g_effectMgr->Retire(this->render.accountId, 0x8DC);
      g_effectMgr->Spawn(this->render.accountId, EF_STATE_LIGHTNINGSTRIKE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_HANDICAPSTATE_CRYSTALLIZATION:
      g_effectMgr->Spawn(this->render.accountId, EF_STATE_CRYSTALLIZATION, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_HANDICAPSTATE_CONFLAGRATION:
      this->SendMsg(nullptr, AM_CHANGE_COLOR, 255, 0, 0, 0, 0, 0);
      g_effectMgr->Retire(this->render.accountId, 0x8DD);
      g_effectMgr->Spawn(this->render.accountId, EF_STATE_CONFLAGRATION, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_HANDICAPSTATE_MISFORTUNE:
      this->SendMsg(nullptr, AM_MAKE_MISFORTUNE_EFFECT, 0, 0, 0, 0, 0, 0);
      break;
    case EFST_HANDICAPSTATE_DEADLYPOISON:
      this->SendMsg(nullptr, AM_CHANGE_COLOR, 170, 0, 170, 0, 0, 0);
      break;
    case EFST_HANDICAPSTATE_DEPRESSION:
      this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 0, 255, 0, 0, 0);
      break;
    case EFST_HANDICAPSTATE_HOLYFLAME:
      this->SendMsg(nullptr, AM_CHANGE_COLOR, 255, 0, 0, 0, 0, 0);
      break;
    case EFST_DRAGONIC_AURA:
      g_effectMgr->Retire(this->render.accountId, 0x6BE);
      g_effectMgr->Retire(this->render.accountId, 0x6BF);
      g_effectMgr->Spawn(this->render.accountId, EF_DRAGONIC_AURA_BUFF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_DRAGONIC_AURA_BUFF_BOTTOM, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_CLIMAX:
      if (!g_effectMgr->Has(this->render.accountId, 0x708)) {
        g_effectMgr->Spawn(this->render.accountId, EF_CLIMAX, this->render.position.x, this->render.position.y, this->render.position.z, 0, value, 0, 0);
      }
      break;
    case EFST_MEDIALE:
      if (!g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF) && !g_effectMgr->Has(this->render.accountId, 0x712)) {
        g_effectMgr->Spawn(this->render.accountId, EF_MEDIALE_VOTUM_LOOP, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_ABYSS_SLAYER:
      if (!g_effectMgr->Has(this->render.accountId, 0x84E)) {
        g_effectMgr->Spawn(this->render.accountId, EF_ABYSS_SLAYER, this->render.position.x, this->render.position.y, this->render.position.z, 0, value, 0, 0);
      }
      break;
    case EFST_FIRST_FAITH_POWER:
      g_effectMgr->Spawn(this->render.accountId, EF_FIRST_FAITH_POWER, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_SECOND_JUDGE:
      g_effectMgr->Spawn(this->render.accountId, EF_JUDGE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_JUDGE_AURA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_THIRD_EXOR_FLAME:
      g_effectMgr->Spawn(this->render.accountId, EF_THIRD_EXOR_FLAME, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(this->render.accountId, EF_THIRD_EXOR_FLAME_AURA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      break;
    case EFST_D_MACHINE:
      if (!g_effectMgr->Has(this->render.accountId, 0x719)) {
        g_effectMgr->Spawn(this->render.accountId, EF_NEW_D_MACHINE, this->render.position.x, this->render.position.y, this->render.position.z, 0, value, 0, 0);
      }
      break;
    case EFST_A_MACHINE:
      g_effectMgr->Spawn(this->render.accountId, EF_A_MACHINE, this->render.position.x, this->render.position.y, this->render.position.z, 0, value, 0, 0);
      break;
    case EFST_COMPETENTIA:
      if (!g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF) && !g_effectMgr->Has(this->render.accountId, 0x73C)) {
        g_effectMgr->Spawn(this->render.accountId, EF_COMPETENTIA_LOOP, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      if (!g_effectMgr->Has(this->render.accountId, 0x73D)) {
        g_effectMgr->Spawn(this->render.accountId, EF_COMPETENTIA_LOOP_BOTTOM, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_WINDSIGN:
      if (!g_effectMgr->Has(this->render.accountId, 0x76C)) {
        g_effectMgr->Spawn(this->render.accountId, EF_WIND_SIGN_LOOP, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_E_SLASH_COUNT:
      if (!g_effectMgr->Has(this->render.accountId, 0x779)) {
        g_effectMgr->Spawn(this->render.accountId, EF_ETERNAL_SLASH_COUNT, this->render.position.x, this->render.position.y, this->render.position.z, 0, value, 0, 0);
      }
      break;
    case EFST_HELLS_PLANT_ARMOR:
      if (!this->HasId(EF_POK_MAGIC)) {
        this->LaunchEffect(EF_POK_MAGIC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SOUNDBLEND:
      if (!g_effectMgr->Has(this->render.accountId, 0x80C)) {
        g_effectMgr->Spawn(this->render.accountId, EF_SOUND_BLEND, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_ABYSSFORCEWEAPON:
      if (!g_effectMgr->Has(this->render.accountId, 0x83D)) {
        CEffect* abyss = g_effectMgr->Spawn(this->render.accountId, 0x83D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (abyss) {
          static_cast<CFromTheAbyssEffect*>(abyss)->SetLevel(value);
        }
      }
      break;
    case EFST_CLOUD_POISON:
      if (!this->HasId(0x8E5)) {
        this->LaunchEffect(0x8E5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SPORE_EXPLOSION_DEBUFF:
      if (!this->HasId(0x8E6)) {
        this->LaunchEffect(0x8E6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BLOCK:
      g_windowMgr.EnsureWindow(WID_DEBUFFREMOVEWND);
      break;
    case EFST_HOLY_OIL:
    case EFST_FIRST_BRAND:
    case EFST_SECOND_BRAND:
      break;
    case EFST_EQUIPPED_DIVINE_ARMOR:
    case EFST_EQUIPPED_HOLY_ARMOR:
      this->SendMsg(nullptr, AM_STATUS_CHANGE, type, 1, 0, 0, 0, 0);
      break;
    default:
      switch (type) {
      case EFST_HAT_EFFECT:
      case EFST_MOONSTAR:
      case EFST_STRANGELIGHTS:
      case EFST_SUPER_STAR:
      case EFST_DECORATION_OF_MUSIC:
      case EFST_FLOWERSMOKE:
      case EFST_FSTONE:
      case EFST_QSCARABA:
      case EFST_LJOSALFAR:
      case EFST_HAPPINESS_STAR:
      case EFST_MAPLE_FALLS:
      case EFST_MERMAID_LONGING:
      case EFST_MAGICAL_FEATHER:
      case EFST_TIME_ACCESSORY:
        this->SendMsg(nullptr, AM_STATUS_CHANGE, type, 1, 0, 0, 0, 0);
        break;
      case EFST_HEAD_EQUIPMENT_EFFECT:
        this->ApplyEffectState(value + 0x9B5, true);
        break;
      default:
        CSkillActionMgr::GetInstance()->OnEfstUpdate(type, value, argument3, argument4, this);
        break;
      }
      break;
    }
  }

  g_statusRefreshSuppressed = 1;
}

// GLOBAL: gl_25218
bool g_moveRequestPending;

struct PlayerSkillRequest {
  int skill;
  int x;
  int y;
  int level;
};

// OFFSET: 0x00DB9A00
// FUNCTION: fn_22672
void CPlayer::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  switch (message) {
  case AM_REQ_GROUND_SKILL: {
    PlayerSkillRequest request;
    request.skill = this->skillId;
    request.x = this->gSkillDx;
    request.y = this->gSkillDy;
    request.level = this->skillUseLevel;
    if (request.skill == 0x7D) {
      g_windowMgr.EnsureWindow(WID_TALKBOXTRAPINPUTWND)->SendMsg(nullptr, UM_SET_SKILL_INFO, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
    } else if (request.skill == 0xDC) {
      g_windowMgr.EnsureWindow(WID_GRAFFISTRBOXWND)->SendMsg(nullptr, UM_SET_SKILL_INFO, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
    } else if (request.skill == 0x108 || request.skill == 0xA04) {
      int metadata = 0;
      int column = 0;
      int row = 0;
      int subColumn = 0;
      int subRow = 0;
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())
        ->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
      MovementRouteInterpolation route;
      if (g_pathFinder.FindPath(timeGetTime(), column, row, subColumn, subRow, this->gSkillDx, this->gSkillDy, 150, route, 15) == true) {
        metadata = route.EndMetadata();
      }
      mode->SendMsg(MM_USE_SKILL_TOGROUND, reinterpret_cast<int>(&request), metadata, 0, 0);
    } else {
      mode->SendMsg(MM_USE_SKILL_TOGROUND, reinterpret_cast<int>(&request), 0, 0, 0);
    }
    break;
  }
  case AM_REQ_SKILL:
    mode->SendMsg(MM_USE_SKILL, this->skillId, this->proceedTargetGid, this->skillUseLevel, 0);
    break;
  case AM_REQ_ATTACK_REPEAT: {
    CRenderObject* target = mode->scene->GetGameActorByAID(this->proceedTargetGid);
    if (target == nullptr || static_cast<CGameActor*>(target)->objectType != 7) {
      mode->SendMsg(MM_REQUEST_ACT, 7, this->proceedTargetGid, 0, 0);
    }
    break;
  }
  case AM_REQ_ATTACK:
    mode->SendMsg(MM_REQUEST_ACT, 0, this->proceedTargetGid, 0, 0);
    break;
  case AM_REQ_SIT:
    if (!StateFlag01(this->stateFlags)) {
      if (this->render.state == ST_STAND) {
        mode->SendMsg(MM_REQUEST_ACT, 2, this->proceedTargetGid, 0, 0);
      } else if (this->render.state == ST_SIT) {
        mode->SendMsg(MM_REQUEST_ACT, 3, this->proceedTargetGid, 0, 0);
      }
    }
    break;
  case AM_REQ_MOVE:
    this->SendMsg(nullptr, AM_CANCELATTACK, 0, 0, 0, 0, 0, 0);
    if (this->render.state != 0x4B) {
      if (this->IsSpecialState(this->render.state)) {
        this->isPreengageStateOfMove = 1;
        this->preengageXOfMove = static_cast<int>(b);
        this->preengageYOfMove = static_cast<int>(c);
      } else {
        this->isPreengageStateOfMove = 0;
        this->preengageXOfMove = -1;
        this->preengageYOfMove = -1;
        this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, b, c, 0, 0, 0, 0);
      }
    }
    break;
  case AM_REQ_MOVE_PRIMITIVE:
    g_moveRequestPending = false;
    if (timeGetTime() > this->standTick + 350) {
      this->standTick = timeGetTime();
      mode->SendMsg(MM_REQUEST_MOVE, static_cast<int>(b), static_cast<int>(c), 0, 0);
    }
    break;
  case AM_REQ_CHANGE_DIR: {
    const std::uint32_t state = this->render.state;
    if (state != ST_STAND && state != ST_DAMAGE && state != ST_SIT) {
      break;
    }
    float targetX;
    float targetZ;
    mode->CellCenter(static_cast<int>(b), static_cast<int>(c), targetX, targetZ);
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
      this->headDir = 0;
      this->render.angle = facing;
    } else if (direction == 1) {
      if (this->headDir == 1) {
        this->headDir = 0;
        this->render.angle = facing;
      } else {
        this->headDir = 1;
      }
    } else {
      if (direction == 2 || direction == 3) {
        this->headDir = 1;
        const float angle = facing + 45.0f;
        this->render.angle = angle;
        if (angle >= 360.0f) {
          this->render.angle = angle - 360.0f;
        }
      }
      if (direction == 7) {
        if (this->headDir == 2) {
          this->headDir = 0;
          this->render.angle = facing;
        } else {
          this->headDir = 2;
        }
      } else if (direction == 5 || direction == 6) {
        const float angle = facing - 45.0f;
        this->headDir = 2;
        this->render.angle = angle;
        if (angle < 0.0f) {
          this->render.angle = angle + 360.0f;
        }
      }
    }
    const int motion = this->headDir;
    this->render.curMotion = motion;
    this->render.angle = static_cast<float>((CPlayer::AngleToDirection(this->render.angle) + 4) * 45);
    mode->SendMsg(MM_CHANGE_DIRECTION, CPlayer::AngleToDirection(this->render.angle), motion, 0, 0);
    break;
  }
  case AM_PREMOVE:
    this->preMoveOn = 1;
    this->preMoveStartTick = timeGetTime();
    this->destCellX = static_cast<int>(b);
    this->destCellZ = static_cast<int>(c);
    break;
  case AM_MOVE: {
    this->preMoveOn = 0;
    const int* route = reinterpret_cast<const int*>(static_cast<std::intptr_t>(b));
    this->routeStartTick = route[0];
    if (this->FindRoute(route[1], route[2], route[3], route[4], route[5], route[6], g_session.m_moveSpeed)) {
      this->movementState[5] = route[5];
      this->destCellY = route[6];
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    } else {
      int column;
      int row;
      int subColumn;
      int subRow;
      mode->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
      if (this->FindRoute(column, row, subColumn, subRow, route[5], route[6], g_session.m_moveSpeed)) {
        this->movementState[5] = route[5];
        this->destCellY = route[6];
        this->SetState(ST_MOVE, 1);
      }
    }
    g_navigationManager.CheckArrival(this->movementState[5], this->destCellY);
    break;
  }
  case AM_FREEZE:
    this->SetState(ST_STAND, 1);
    break;
  case AM_CANCELATTACK:
    this->isAttackRequest = 0;
    this->proceedType = 0;
    mode->SendMsg(MM_SETLOCKONMOUSE, 0, 0, 0, 0);
    break;
  case AM_ATTACKCLICKUP: {
    const int attackMode = this->proceedType;
    if (attackMode != 1 && attackMode != 5) {
      if (attackMode == 3) {
        mode->SendMsg(MM_SETLOCKONMOUSE, 0, 0, 0, 0);
        break;
      }
      if (attackMode != 6) {
        break;
      }
    }
    if (this->attackMode != 0) {
      this->attackMode = 2;
      if (this->totalAttackReqCnt != 0) {
        this->SendMsg(nullptr, AM_CANCELATTACK, 0, 0, 0, 0, 0, 0);
        mode->SendMsg(MM_CANCEL_LOCKON, 0, 0, 0, 0);
      }
    }
    break;
  }
  case AM_SET_SKILL_USE_LEVEL:
    this->skillUseLevel = static_cast<int>(b);
    break;
  case AM_PROCEEDGROUNDSKILL:
    if (this->render.state != ST_SIT &&
        (!StateFlag01(this->stateFlags) || b == 0x89 || b == 0xD4 || b == 0x211 || b == 0x212)) {
      this->proceedType = 4;
      this->skillId = static_cast<int>(b);
      this->gSkillDx = static_cast<int>(c);
      this->gSkillDy = static_cast<int>(d);
      this->proceedTargetGid = 0;
    }
    break;
  case AM_PROCEEDSKILL:
    if (this->render.state != ST_SIT &&
        (!StateFlag01(this->stateFlags) || c == 0x89 || c == 0xD4 || c == 0x211 || c == 0x212 || c == 0x250)) {
      this->proceedType = 3;
      this->proceedTargetGid = static_cast<int>(b);
      this->skillId = static_cast<int>(c);
    }
    break;
  case AM_RESUMEPROCEEDATTACK:
    if (this->render.state != ST_SIT && !StateFlag01(this->stateFlags)) {
      const int attackMode = this->proceedType == 1 ? 5 : 1;
      this->tickOfMoveForAttack = timeGetTime() - 1200;
      this->proceedType = attackMode;
      this->proceedTargetGid = static_cast<int>(b);
      this->isAttackRequest = 0;
      this->totalAttackReqCnt = 0;
      this->destCellX = -1;
      this->destCellZ = -1;
    }
    break;
  case AM_PROCEEDATTACK:
    if (this->render.state != ST_SIT && !StateFlag01(this->stateFlags)) {
      CRenderObject* target = mode->scene->GetGameActorByAID(static_cast<std::uint32_t>(b));
      if (target != nullptr) {
        this->proceedType =
          static_cast<std::uint32_t>(g_session.m_moveSpeed * 4 / 3) <
              static_cast<std::uint32_t>(static_cast<CGameActor*>(target)->moveSpeed)
            ? 5
            : 1;
        this->proceedTargetGid = static_cast<int>(b);
        this->isAttackRequest = 0;
        this->attackMode = static_cast<int>(c);
        this->totalAttackReqCnt = 0;
      }
    }
    break;
  case AM_PROCEEDPICKUPITEM:
    if (this->render.state != ST_SIT && !StateFlag01(this->stateFlags)) {
      this->proceedType = 2;
      this->proceedTargetGid = static_cast<int>(b);
    }
    break;
  case AM_PROCEEDTOUCHSKILL:
    if (this->render.state != ST_SIT && !StateFlag01(this->stateFlags)) {
      const int* skill = reinterpret_cast<const int*>(static_cast<std::intptr_t>(c));
      this->proceedType = 6;
      this->skillId = skill[1];
      this->skillUseLevel = skill[3];
      this->proceedTargetGid = static_cast<int>(b);
      this->attackMode = static_cast<int>(d);
      this->isAttackRequest = 0;
      this->totalAttackReqCnt = 0;
    }
    break;
  case AM_CLEAR_ROUTE:
    this->route.points.clear();
    break;
  default:
    this->CPc::SendMsg(sender, message, b, c, d, e, f, g);
    break;
  }
}

// OFFSET: 0x00DAC750
// FUNCTION: fn_22596
void CGrannyPc::RenderModel(
  const matrix& view, const GrannyPcLight& light, C3dGrannyModelRes* model, int layer, C3dGrannyModelRes* animation
) {
  if (!this->render.visible) {
    return;
  }

  CTexture* texture = nullptr;
  std::uint16_t* indices = nullptr;

  if (this->render.accountId == 0xFFFFFFFF) {
    return;
  }

  this->brightness = (this->alpha << 24) + 0xFFFFFF;
  g_modeMgr.GetGameMode();
  granny_world_pose* pose = nullptr;
  RenderCommand** commands = this->layerCommands[layer];
  if (!model) {
    model = this->model;
    this->worldPose = this->UpdateAnimation(nullptr);
    commands = this->commands;
  } else if (animation) {
    pose = this->UpdateAnimation(animation);
  } else {
    pose = this->worldPose;
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

        g_renderer->viewport.Project(&position.x, world.values, output);
        output.u = vertex.uv[0];
        output.v = vertex.uv[1];
        const float shade = lightY * vertex.normal[1] + lightX * vertex.normal[0] + lightZ * vertex.normal[2];
        const float inverseShade = 1.0f - shade;
        const float upperShade = 1.0f < inverseShade ? 1.0f : inverseShade;
        const float shadeFactor = 0.0f > upperShade ? 0.0f : upperShade;
        const unsigned brightness =
          static_cast<unsigned>(shadeFactor * static_cast<float>(light.intensity) + static_cast<float>(255 - light.intensity));
        this->brightness = brightness;
        output.diffuse = ((light.red + 0x38 + brightness) << 16 & 0xFF0000) + ((light.green + 0x38 + brightness) << 8 & 0xFF00) +
                         static_cast<unsigned char>(light.blue + 0x38 + brightness) + (this->alpha << 24);
      }

      if (layer != 1) {
        this->attachmentPositions.push_back(position);
      }
    }

    command->texture = texture;
    command->vertexCount = vertexCount;
    static_cast<CRendererDX9*>(g_renderer)->Submit(command, 0);
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

// OFFSET: 0x00DB50C0
// FUNCTION: fn_22660
CPlayer::CPlayer() {
  this->Initialize();
}

// OFFSET: 0x00DB5340
// FUNCTION: fn_22665
void CPlayer::Initialize() {
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())
    ->CellCenter(g_session.m_playerX, g_session.m_playerY, this->render.position.x, this->render.position.z);
  this->render.position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain
                              ->Height(this->render.position.x, this->render.position.z);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())
    ->WorldCell(this->render.position.x, this->render.position.z, this->destCellX, this->destCellZ);

  this->gSkillDx = 0;
  this->gSkillDy = 0;
  this->render.angle = static_cast<float>((g_session.m_playerDir + 4) * 45);
  this->render.accountId = g_session.m_aid;
  this->guildId = g_guildInfo.m_guildId;
  this->pathStartCell = -1;
  this->preMoveStartTick = 0;
  this->preMoveOn = 0;
  this->proceedType = 0;
  this->attackMode = 0;
  this->totalAttackReqCnt = 0;
  this->isAttackRequest = 0;
  this->proceedTargetGid = 0;
  this->render.visibilityOptions[1] = 1;
  this->skillId = 0;
  this->skillUseLevel = 0;
  this->tickOfMoveForAttack = timeGetTime() - 1200;
  this->attackReqTime = timeGetTime() - 1200;
  this->render.screenX = g_renderer->viewport.centerX;
  this->render.screenY = g_renderer->viewport.centerY;
  this->isPreengageStateOfMove = 0;
  this->preengageXOfMove = -1;
  this->preengageYOfMove = -1;
  this->standTick = timeGetTime() - 1200;
  this->SetState(ST_STAND, 1);

  this->moveSpeed = g_session.m_moveSpeed;
  this->render.isCharacter = 1;
  this->SetSexBit(g_session.GetCurrentSex());

  const int bodyState = this->bodyState;
  const std::uint32_t stateFlags = this->stateFlags;
  const int effectState = this->effectState;
  this->bodyState = g_session.m_bodyState;
  this->stateFlags = g_session.m_jobFlags;
  this->effectState = g_session.m_effectState;
  this->SetMadogearType(g_session.madogearType);
  this->SetBodyCostume(g_session.m_bodyCostume, 0);
  this->SetSprAct2(
    g_session.GetDisplayJob(), g_session.GetCurrentSex(), g_session.m_head, g_session.m_weapon,
    g_session.m_weaponLeft, g_session.m_accessory,
    g_session.m_accessory2, g_session.m_accessory3,
    g_session.m_headPalette, g_session.m_bodyStyle, g_session.m_jobFlags
  );
  this->SetSprRobe(g_session.m_robe, 0);
  if (g_session.playerAppearanceState != 0) {
    const int value = g_session.playerAppearanceState;
    this->SendMsg(nullptr, AM_STATUS_CHANGE, 856, 1, value, 0, 0, 0);
  }
  this->bodyState = bodyState;
  this->stateFlags = stateFlags;
  this->effectState = effectState;
  this->SetStatusStates(
    g_session.m_bodyState, g_session.m_jobFlags, g_session.m_effectState, false
  );

  this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STATUS_STATE, 0, 0, 0, 0, 0);
  this->skillEffect = this->LaunchEffect(EF_STATUS_STATE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (!g_statusRefreshSuppressed) {
    this->ReapplySkillEffects();
  }

  this->msgEffect = nullptr;
  this->objectType = 0;
  this->SetShapeShift(g_session.GetPlayerSkillMode());
}

// OFFSET: 0x00DB4290
// FUNCTION: fn_22651
void CPc::SetState(int state, int resetPosition) {
  int current = this->render.state;
  if (current == ST_DEAD || this->isTrickDead != 0) {
    return;
  }

  if (state != ST_DEAD && (current == 0x51 || state == -1)) {
    return;
  }

  if (state == ST_DEAD) {
    const int job = this->GetEffectiveJob(JobNames::BaseJob(this->job));
    this->LoadEquipAction(0, g_session.GetBodyActionResourceName(job, this->sex, this->bodyStyle, this->GetMadogearType()).c_str());
    this->LoadEquipSprite(0, g_session.GetBodySprName(job, this->sex, this->bodyStyle, this->GetMadogearType()).c_str());
    current = this->render.state;
  }

  if (current == ST_DAMAGE && static_cast<char>(resetPosition) == 1) {
    this->render.position.x = this->damageDestX;
    this->render.position.z = this->damageDestZ;
  }

  int action;
  if (this->GetMonsterTransformJob() > 0) {
    action = this->ActionBase(state);
  } else {
    action = this->JobActionBase(state);
  }


  switch (state) {
  case ST_BEGIN_SKILL:
    this->RegisterPos();
    this->SetAction(action, 6, 1);
    this->render.isMotionFinished = true;
    break;
  case ST_SIT:
    this->RegisterPos();
    this->SetAction(action, 4, 1);
    this->render.isMotionFinished = true;
    this->render.curMotion = this->headDir;
    break;
  case ST_MOVE:
    this->isCounter = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    this->headDir = 0;
    CGameActor::SetState(ST_MOVE, resetPosition);
    break;
  case ST_STAND:
    CGameActor::SetState(ST_STAND, resetPosition);
    if (g_session.m_aid == this->render.accountId) {
      switch (g_session.m_job) {
      case JT_STAR:
      case JT_STAR_B:
      case JT_STAR_EMPEROR:
      case JT_STAR_EMPEROR_B:
      case JT_SKY_EMPEROR:
        if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.desiredDistance > g_serviceOptionJitterScale) {
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.desiredDistance = g_serviceOptionJitterScale;
        }
        break;
      }
    }
    this->render.curMotion = this->headDir;
    switch (current) {
    case ST_ATTACK:
    case ST_DAMAGE:
    case ST_BEGIN_SKILL:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x3A:
    case 0x3B:
    case 0x3E:
    case 0x3F:
    case 0x40:
    case 0x41:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4C:
    case 0x4D:
    case 0x4E:
    case 0x4F:
    case 0x50:
      this->SetAction(action, 4, 0);
      break;
    default:
      if (this->healthState != 0) {
        this->SetAction(action, 4, 0);
      }
      break;
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
    break;
  case ST_SKILL:
    if (this->render.specialMotionLocked == 1 || this->render.state == ST_SIT) {
      return;
    }
    this->SetAction(action, 6, 1);
    if (this->GetEffectiveJob(-1) == JT_BARD_H || this->GetEffectiveJob(-1) == JT_DANCER_H || this->GetEffectiveJob(-1) == JT_CRUSADER_H ||
        this->GetEffectiveJob(-1) == JT_ROYAL_GUARD || this->GetEffectiveJob(-1) == JT_ROYAL_GUARD_H || this->GetEffectiveJob(-1) == JT_MONK ||
        this->GetEffectiveJob(-1) == JT_MONK_H || this->GetEffectiveJob(-1) == JT_MONK_B || this->GetEffectiveJob(-1) == JT_SURA ||
        this->GetEffectiveJob(-1) == JT_SURA_H || this->GetEffectiveJob(-1) == JT_SURA_B || this->GetEffectiveJob(-1) == JT_INQUISITOR) {
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 400, 0);
    } else if (this->GetEffectiveJob(-1) == JT_NINJA) {
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x60, 2, 5, 0x60, 5, 1, 1000, 0);
    } else if (this->GetEffectiveJob(-1) == JT_GUNSLINGER) {
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x60, 0, 3, 0x60, 3, 1, 1000, 0);
    } else if (this->GetEffectiveJob(-1) == JT_NINJA_B) {
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x60, 2, 5, 0x60, 5, 1, 1000, 0);
    } else if (this->GetEffectiveJob(-1) == JT_GUNSLINGER_B) {
      this->SetSequenceInterval(4);
      this->SetFrameSequence(0x60, 0, 3, 0x60, 3, 1, 1000, 0);
    }
    this->attackMotion = 6.0f;
    break;
  case ST_ATTACK: {
    this->isCounter = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }

    bool weaponAllowed;
    if (this->GetShapeShift() == 1) {
      weaponAllowed = false;
    } else if (this->GetShapeShift() == 2) {
      weaponAllowed = true;
    } else {
      weaponAllowed = IsJobWeaponAllowed(
        this->GetEffectiveJob(-1), this->sex, (static_cast<std::int64_t>(this->secondaryWeapon) << 32) | static_cast<std::uint32_t>(this->weapon)
      );
    }

    int attackState;
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
      if (std::ranges::max(rand() % 10, 0) < 7) {
        weaponAllowed = true;
        attackState = 9;
      } else {
        weaponAllowed = false;
        attackState = 2;
      }
      break;
    default:
      attackState = weaponAllowed ? 9 : 2;
      break;
    }

    CGameActor::SetState(attackState, resetPosition);
    this->attackMotion = AttackMotionLength(this->GetEffectiveJob(-1), this->sex, this->weapon, weaponAllowed);

    int weaponView = this->weapon;
    if (weaponView > 105) {
      weaponView = GetWeaponViewValue(weaponView);
    }
    bool usesArrow = false;
    g_session.m_hotkeyScript->Call("IsItemUsingArrow", "d>b", weaponView, &usesArrow);
    if (usesArrow == true) {
      if (this->IsArrowEffectSuppressed() == true) {
        this->SetArrowEffectSuppressed(false);
      } else {
        CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
        mode->SpawnArrowEffect(&this->render.position, this->hitTargetAid, this->render.speed * this->attackMotion, 0);
      }
      this->attackMotion = 8.0f / this->render.speed + this->attackMotion;
    }

    if (g_session.IsDoramJob(this->GetEffectiveJob(-1)) == true && this->alternateDoramAppearance[0] == 1) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->SpawnArrowEffect(&this->render.position, this->hitTargetAid, this->attackMotion * this->render.speed, this->GetEffectiveJob(-1));
      this->attackMotion = 8.0f / this->render.speed + this->attackMotion;
    }
    break;
  }
  case ST_ATTACK2: {
    this->isCounter = 0;
    if (this->bodyState != 1 && this->bodyState != 2) {
      this->render.isMotionFreezed = false;
    }
    const bool weaponAllowed = IsJobWeaponAllowed(
      this->GetEffectiveJob(-1), this->sex, (static_cast<std::int64_t>(this->secondaryWeapon) << 32) | static_cast<std::uint32_t>(this->weapon)
    );
    CGameActor::SetState(weaponAllowed ? 9 : 2, resetPosition);
    this->attackMotion = AttackMotionLength(this->GetEffectiveJob(-1), this->sex, this->weapon, weaponAllowed);
    if ((this->weapon == 14 && this->secondaryWeapon == 0) || (this->weapon == 13 && this->secondaryWeapon == 0)) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->SpawnArrowEffect(&this->render.position, this->hitTargetAid, this->attackMotion * this->render.speed, 0);
      this->attackMotion = 8.0f / this->render.speed + this->attackMotion;
    }
    break;
  }
  default:
    CGameActor::SetState(state, resetPosition);
    return;
  }

  this->render.previousAnimation = this->Get8Dir(this->render.angle) + this->render.animation;
  this->render.state = state;
  this->render.motionStartTime = timeGetTime();
}

// OFFSET: 0x00DAC400
// FUNCTION: fn_22594
void CGrannyPc::Render(int argument) {
  if (!this->model) {
    return;
  }

  GrannyPcLight light;
  light.red = 200;
  light.direction = g_weatherEffect.GetWindDirection();
  light.green = 200;
  light.blue = 200;
  light.intensity = 0x80;

  const matrix& view = *reinterpret_cast<const matrix*>(argument);
  if (this->renderFrame == 0) {
    this->model->hairPivot = &this->hairPivot;
    this->RenderModel(view, light, nullptr, 0, nullptr);
    for (int layer = 1; layer < 10; ++layer) {
      if (this->layerModels[layer]) {
        if (layer != 1) {
          this->RenderModel(view, light, this->layerModels[layer], layer, nullptr);
        } else {
          this->RenderModel(view, light, this->layerModels[1], 1, this->layerModels[1]);
        }
      }
    }
    this->RenderShadow(argument, vector3d{1800.0f, -1000.0f, 0.0f}, false);
  } else {
    this->RenderCached(view, 0, nullptr);
    if (this->layerModels[1]) {
      this->layerModels[1]->hairPivot = &this->hairPivot;
    }
    for (int layer = 1; layer < 10; ++layer) {
      if (this->layerModels[layer]) {
        this->RenderCached(view, layer, this->layerModels[layer]);
      }
    }
    this->RenderShadow(argument, vector3d{1800.0f, -1000.0f, 0.0f}, true);
  }

  if (++this->renderFrame == 3) {
    this->renderFrame = 0;
  }
}

// OFFSET: 0x00DADA40
// FUNCTION: fn_22600
void CGrannyPc::RenderShadow(int argument, vector3d lightDirection, bool cached) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float sine = ClientSine(static_cast<int>(this->angle) + 90);
  const float cosine = ClientCosine(static_cast<int>(this->angle) + 90);
  lightDirection.x = -18.0f;
  lightDirection.z = -5.0f;
  lightDirection.y = -5.0f;

  int column;
  int row;
  int subColumn;
  int subRow;
  if (!cached) {
    mode->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
    float centerX;
    float centerZ;
    mode->CellCenter(column, row, centerX, centerZ);
    const int offsetX = static_cast<int>(this->render.position.x - centerX);
    const int offsetZ = static_cast<int>(this->render.position.z - centerZ);

    std::memset(g_groundShadowPixels, 0, sizeof(g_groundShadowPixels));
    for (const vector3d& point : this->attachmentPositions) {
      const float x = point.x * 0.35f;
      const float y = point.y * 0.35f;
      const int pixelX = static_cast<int>(x * sine + y * cosine + 17.0f) + offsetX;
      const int pixelY = static_cast<int>(x * cosine - y * sine + 17.0f) + offsetZ;
      if (pixelX >= 0 && pixelX < 34 && pixelY >= 0 && pixelY < 34) {
        const int index = pixelY * 35 + pixelX;
        g_groundShadowPixels[index + 1] |= 0x2F000000;
        g_groundShadowPixels[index + 35] |= 0x2F000000;
        g_groundShadowPixels[index] = 0x5F000000;
      }
    }
    this->attachmentPositions.clear();
    this->shadowTexture->Upload(0, 0, 35, 35, g_groundShadowPixels, 0, 0);
  }

  mode->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
  GroundGeometry* ground = mode->scene->ground;
  column -= 2;
  row -= 2;
  const float groundSine = std::sin(0.0f);
  const float groundCosine = std::cos(0.0f);


  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      const float row0 = static_cast<float>(i) - 2.5f;
      const float row1 = static_cast<float>(i + 1) - 2.5f;
      const float column0 = static_cast<float>(j) - 2.5f;
      const float column1 = static_cast<float>(j + 1) - 2.5f;
      float uv[8];
      uv[0] = (row0 * groundSine + column0 * groundCosine + 2.5f) / 5.0f;
      uv[1] = (row0 * groundCosine - column0 * groundSine + 2.5f) / 5.0f;
      uv[2] = (row0 * groundSine + column1 * groundCosine + 2.5f) / 5.0f;
      uv[3] = (row0 * groundCosine - column1 * groundSine + 2.5f) / 5.0f;
      uv[4] = (row1 * groundSine + column0 * groundCosine + 2.5f) / 5.0f;
      uv[5] = (row1 * groundCosine - column0 * groundSine + 2.5f) / 5.0f;
      uv[6] = (row1 * groundSine + column1 * groundCosine + 2.5f) / 5.0f;
      uv[7] = (row1 * groundCosine - column1 * groundSine + 2.5f) / 5.0f;
      ground->DrawGridDecal(
        reinterpret_cast<const float*>(argument), column + j, row + i, 0x70FFFFFF, *this->shadowTexture, uv);
    }
  }
}

struct PcStandEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int sex;
  /* 000C */ int cellX;
  /* 0010 */ int cellY;
  /* 0014 */ int direction;
  /* 0018 */ int speed;
  /* 001C */ int xSize;
  /* 0020 */ int ySize;
  /* 0024 */ int head;
  /* 0028 */ int weapon;
  /* 002C */ int secondaryWeapon;
  /* 0030 */ int accessory;
  /* 0034 */ int state;
  /* 0038 */ int bodyState;
  /* 003C */ int effectState;
  /* 0040 */ std::uint32_t stateFlags;
  /* 0044 */ int healthState;
  /* 0048 */ int shield;
  /* 004C */ int accessory2;
  /* 0050 */ int accessory3;
  /* 0054 */ int headPalette;
  /* 0058 */ int bodyPalette;
  /* 005C */ int headDir;
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int honor;
  /* 006C */ int virtue;
  /* 0070 */ int robe;
  /* 0074 */ int pkState;
  /* 0078 */ int bodyStyle;
};

struct PcMoveEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int sex;
  /* 000C */ std::uint32_t moveStartTick;
  /* 0010 */ int startX;
  /* 0014 */ int startY;
  /* 0018 */ int startSubX;
  /* 001C */ int startSubY;
  /* 0020 */ int destX;
  /* 0024 */ int destY;
  /* 0028 */ int speed;
  /* 002C */ int head;
  /* 0030 */ int weapon;
  /* 0034 */ int secondaryWeapon;
  /* 0038 */ int accessory;
  /* 003C */ int bodyState;
  /* 0040 */ int effectState;
  /* 0044 */ std::uint32_t stateFlags;
  /* 0048 */ int healthState;
  /* 004C */ int shield;
  /* 0050 */ int accessory2;
  /* 0054 */ int accessory3;
  /* 0058 */ int headPalette;
  /* 005C */ int bodyPalette;
  /* 0060 */ int headDir;
  /* 0064 */ int guildId;
  /* 0068 */ int guildVersion;
  /* 006C */ int honor;
  /* 0070 */ int virtue;
  /* 0074 */ int robe;
  /* 0078 */ int pkState;
  /* 007C */ int bodyStyle;
};

// OFFSET: 0x00DAE890
// FUNCTION: fn_22602
void CPc::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  switch (message) {
  case AM_SET_HEAD_DIR:
    if (this->render.state == ST_STAND || this->render.state == ST_SIT) {
      this->render.curMotion = static_cast<int>(b);
    }
    break;
  case AM_REMOVEGAGE:
    if (this->gaugeWindow) {
      g_windowMgr.Schedule(this->gaugeWindow);
      this->gaugeWindow = nullptr;
    }
    break;
  case AM_UPDATEGAGE:
    if (!this->gaugeWindow) {
      this->gaugeWindow = new UIPcGage();
      this->gaugeWindow->SetSize(60, 5);
      g_windowMgr.AddRenderedFront(this->gaugeWindow);
    }
    static_cast<UIPcGage*>(this->gaugeWindow)->SetValues(static_cast<int>(b), static_cast<int>(c));
    static_cast<UIPcGage*>(this->gaugeWindow)->SetLimits(0, 0);
    this->gaugeWindow->Move(this->render.screenX - 30, ScaleByScreenHeight(12) + this->render.screenY);
    break;
  case AM_STANDENTRY: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    break;
  }
  case AM_STANDENTRY2: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    this->bodyStyle = entry->bodyStyle;
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprAct2(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->headPalette, entry->bodyPalette, entry->stateFlags
    );
    this->SetSprRobe(entry->robe, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    this->pkState = entry->pkState;
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    break;
  }
  case AM_MAKE_CHICKEN:
  case AM_DELETE_CHICKEN:
  case 136: {
    this->SetSprJob(this->GetEffectiveJob(-1));
    this->SetImfFileName();
    const std::int64_t weapon = this->GetWeapon();
    if (weapon >= 1 && weapon <= 104) {
      this->SetSprWeapon(static_cast<int>(weapon), static_cast<int>(weapon >> 32));
    } else {
      this->SetSprWeapon2(static_cast<int>(weapon), static_cast<int>(weapon >> 32));
    }
    this->SetSprShield(this->shield);
    this->SetHeadPaletteName();
    this->SetBodyPaletteName();
    break;
  }
  case AM_INITSTANDENTRY: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    this->render.accountId = entry->aid;
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    gameMode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->moveSpeed = entry->speed;
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    if (c == 2 && !StateFlag06(this->stateFlags)) {
      this->LaunchEffect(EF_ENTRY2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    this->ShowPartyMemberEffect(entry->aid);
    break;
  }
  case AM_INITSTANDENTRY2: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    this->bodyStyle = entry->bodyStyle;
    this->render.accountId = entry->aid;
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    gameMode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->moveSpeed = entry->speed;
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprAct2(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->headPalette, entry->bodyPalette, entry->stateFlags
    );
    this->SetSprRobe(entry->robe, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    if (c == 2 && !StateFlag06(this->stateFlags)) {
      this->LaunchEffect(EF_ENTRY2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    this->ShowPartyMemberEffect(entry->aid);
    this->pkState = entry->pkState;
    break;
  }
  case AM_INITMOVEENTRY: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    mode->CellCenter(entry->startX, entry->startY, this->render.position.x, this->render.position.z);
    this->render.position.x += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubX - 8);
    this->render.position.z += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubY - 8);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    } else {
      this->SetState(ST_STAND, 1);
    }
    this->ShowPartyMemberEffect(entry->aid);
    break;
  }
  case AM_INITMOVEENTRY2: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->bodyStyle = entry->bodyStyle;
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprAct2(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->headPalette, entry->bodyPalette, entry->stateFlags
    );
    this->SetSprRobe(entry->robe, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    mode->CellCenter(entry->startX, entry->startY, this->render.position.x, this->render.position.z);
    this->render.position.x += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubX - 8);
    this->render.position.z += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubY - 8);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    } else {
      this->SetState(ST_STAND, 1);
    }
    this->ShowPartyMemberEffect(entry->aid);
    this->pkState = entry->pkState;
    break;
  }
  case AM_MOVEENTRY: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->bodyStyle = entry->bodyStyle;
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    }
    this->ShowPartyMemberEffect(entry->aid);
    this->pkState = entry->pkState;
    break;
  }
  case AM_MOVEENTRY2: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->bodyStyle = entry->bodyStyle;
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprAct2(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->headPalette, entry->bodyPalette, entry->stateFlags
    );
    this->SetSprRobe(entry->robe, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    }
    this->ShowPartyMemberEffect(entry->aid);
    this->pkState = entry->pkState;
    this->bodyStyle = entry->bodyStyle;
    break;
  }
  case AM_CHANGE_RANK:
    this->pkRank = static_cast<int>(c);
    this->pkTotal = static_cast<int>(b);
    break;
  case AM_MAKE_BIRD: {
    if (this->statusEffects[0] != 0) {
      break;
    }

    int effectId;
    switch (this->bodyStyle) {
    case JT_HUNTER_H:
      effectId = 0x17;
      break;
    case JT_HUNTER_B:
      effectId = 0x18;
      break;
    case JT_GANGSI:
      effectId = 0x20;
      break;
    case JT_DEATHKNIGHT:
      effectId = 0x21;
      break;
    case JT_COLLECTOR:
      effectId = 0x22;
      break;
    case JT_RANGER:
    case JT_RANGER_H:
    case JT_RANGER_B:
    case JT_RANGER_2ND:
      effectId = 0x35;
      break;
    case JT_WINDHAWK:
    case JT_WOLF_WINDHAWK:
      effectId = 0x45;
      break;
    default:
      effectId = 0xE;
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectId, &this->render.position, this->render.accountId, true);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    effect->SetAnimationSpeed(6.0f);
    this->AddMsgEffect(effect);
    this->statusEffects[0] = effect;
    break;
  }
  case AM_MAKE_WOLF: {
    if (this->statusEffects[1] != 0) {
      break;
    }

    int effectId;
    switch (this->bodyStyle) {
    case JT_RANGER:
    case JT_RANGER_H:
    case JT_WOLF_RANGER:
    case JT_WOLF_RANGER_H:
      effectId = 0x23;
      break;
    case JT_RANGER_B:
    case JT_WOLF_RANGER_B:
      effectId = 0x2A;
      break;
    case JT_WINDHAWK:
    case JT_WOLF_WINDHAWK:
      effectId = 0x46;
      break;
    case JT_RANGER_2ND:
      effectId = g_session.IsBabyJob(this->GetEffectiveJob(-1)) == true ? 0x2A : 0x23;
      break;
    default:
      return;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectId, &this->render.position, this->render.accountId, true);
    if (!effect) {
      break;
    }
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->AddMsgEffect(effect);
    this->statusEffects[1] = effect;
    break;
  }
  default:
    CGameActor::SendMsg(sender, message, b, c, d, e, f, g);
    break;
  case 77:
    break;
  }
}

// OFFSET: 0x00DADE30
// FUNCTION: fn_22601
void CGrannyPc::SendMsg(
  CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g
) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  switch (message) {
  case AM_SET_HEAD_DIR:
    if (this->render.state == ST_STAND || this->render.state == ST_SIT) {
      this->render.curMotion = static_cast<int>(b);
    }
    break;
  case AM_REMOVEGAGE:
    if (this->gaugeWindow) {
      g_windowMgr.Schedule(this->gaugeWindow);
      this->gaugeWindow = nullptr;
    }
    break;
  case AM_UPDATEGAGE:
    if (!this->gaugeWindow) {
      this->gaugeWindow = new UIPcGage();
      this->gaugeWindow->SetSize(60, 5);
      g_windowMgr.AddRenderedFront(this->gaugeWindow);
    }
    static_cast<UIPcGage*>(this->gaugeWindow)->SetValues(static_cast<int>(b), static_cast<int>(c));
    static_cast<UIPcGage*>(this->gaugeWindow)->SetLimits(0, 0);
    this->gaugeWindow->Move(this->render.screenX - 30, ScaleByScreenHeight(12) + this->render.screenY);
    break;
  case AM_STANDENTRY: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    break;
  }
  case AM_STANDENTRY2: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    break;
  }
  case AM_MAKE_CHICKEN:
  case AM_DELETE_CHICKEN: {
    this->SetSprJob(this->GetEffectiveJob(-1));
    this->SetImfFileName();
    const std::int64_t weapon = this->GetWeapon();
    this->SetSprWeapon(static_cast<int>(weapon), static_cast<int>(weapon >> 32));
    this->SetSprShield(this->shield);
    this->SetHeadPaletteName();
    this->SetBodyPaletteName();
    break;
  }
  case AM_INITSTANDENTRY:
  case AM_INITSTANDENTRY2: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    this->render.accountId = entry->aid;
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    gameMode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->moveSpeed = entry->speed;
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headDir = entry->headDir;
    this->headType = entry->head;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    switch (entry->state) {
    case PCSTATE_STAND:
      this->SetState(ST_STAND, 1);
      break;
    case PCSTATE_DEAD:
      this->SetState(ST_DEAD, 1);
      break;
    case PCSTATE_SIT:
      this->SetState(ST_SIT, 1);
      break;
    default:
      this->SetState(ST_STAND, 1);
      break;
    }
    if (c == 2 && !StateFlag06(this->stateFlags)) {
      this->LaunchEffect(EF_ENTRY2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    this->ShowPartyMemberEffect(entry->aid);
    break;
  }
  case AM_INITMOVEENTRY: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    mode->CellCenter(entry->startX, entry->startY, this->render.position.x, this->render.position.z);
    this->render.position.x += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubX - 8);
    this->render.position.z += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubY - 8);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    } else {
      this->SetState(ST_STAND, 1);
    }
    this->ShowPartyMemberEffect(entry->aid);
    break;
  }
  case AM_INITMOVEENTRY2: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->stateFlags = entry->stateFlags;
    this->effectState = entry->effectState;
    this->bodyState = entry->bodyState;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    mode->CellCenter(entry->startX, entry->startY, this->render.position.x, this->render.position.z);
    this->render.position.x += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubX - 8);
    this->render.position.z += static_cast<float>(mode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubY - 8);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    } else {
      this->SetState(ST_STAND, 1);
    }
    this->ShowPartyMemberEffect(entry->aid);
    break;
  }
  case AM_MOVEENTRY:
  case AM_MOVEENTRY2: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    this->render.accountId = entry->aid;
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    this->headDir = 0;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprActFull(
      entry->job, entry->sex, entry->head, entry->weapon, entry->secondaryWeapon, entry->accessory, entry->accessory2,
      entry->accessory3, entry->shield, entry->headPalette, entry->bodyPalette
    );
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetHealthState(entry->healthState);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(ST_MOVE, 1);
      this->pathStartCell = 0;
    }
    this->ShowPartyMemberEffect(entry->aid);
    break;
  }
  case AM_CHANGE_RANK:
    this->pkRank = static_cast<int>(c);
    this->pkTotal = static_cast<int>(b);
    break;
  default:
    CGameActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    break;
  case 77:
    break;
  }
}

// OFFSET: 0x00DAC080
// FUNCTION: fn_22592
bool CGrannyPc::ProcessState() {
  if (static_cast<std::uint32_t>(this->freezeEndTick) > 0 && timeGetTime() > static_cast<std::uint32_t>(this->freezeEndTick)) {
    this->render.isMotionFreezed = false;
    this->freezeEndTick = 0;
  }

  if (!this->UpdateFadeOut()) {
    return false;
  }

  g_modeMgr.GetGameMode();
  switch (this->render.state) {
  case ST_BEGIN_SKILL: {
    this->ProcessMotion();
    const int frames = static_cast<int>(this->GetElapsedFrames());
    if (this->hitTargetAid != 0 && frames % 34 == 0) {
      CRenderObject* target = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->hitTargetAid);
      if (target) {
        this->Face(target->render.position.x, target->render.position.z);
      }
    }
    return true;
  }
  case ST_ATTACK:
    this->ProcessMotion();
    this->GetElapsedFrames();
    if (this->render.isMotionFinished) {
      this->SetState(ST_STAND, 1);
    }
    return true;
  case ST_SKILL:
    this->ProcessMotion();
    if (this->render.isMotionFinished) {
      this->SetState(ST_STAND, 1);
    }
    return true;
  case ST_DEAD:
    this->render.frameSequenceActive = 0;
    this->render.animationSequenceActive = 0;
    this->render.timedSequenceActive = 0;
    this->render.isMotionFreezed = false;
  case ST_SIT:
    this->ProcessMotion();
    return true;
  default:
    return CGameActor::ProcessState();
  }
}

// OFFSET: 0x00DAC200
// FUNCTION: fn_22593
bool CPc::ProcessState() {
  if (static_cast<std::uint32_t>(this->freezeEndTick) > 0 && timeGetTime() > static_cast<std::uint32_t>(this->freezeEndTick)) {
    this->render.isMotionFreezed = false;
    this->freezeEndTick = 0;
  }

  if (!this->UpdateFadeOut()) {
    return false;
  }

  g_modeMgr.GetGameMode();
  switch (this->render.state) {
  case ST_BEGIN_SKILL: {
    this->ProcessMotion();
    const int frames = static_cast<int>(this->GetElapsedFrames());
    if (this->hitTargetAid != 0 && frames % 34 == 0) {
      CRenderObject* target = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->hitTargetAid);
      if (target) {
        this->Face(target->render.position.x, target->render.position.z);
      }
    }
    return true;
  }
  case ST_ATTACK:
    this->ProcessMotion();
    this->GetElapsedFrames();
    if (this->render.isMotionFinished) {
      this->SetState(ST_STAND, 1);
    }
    return true;
  case ST_SKILL:
  case ST_AB_SPECIAL:
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
    this->ProcessMotion();
    if (this->render.isMotionFinished) {
      this->SetState(ST_STAND, 1);
    }
    return true;
  case ST_DEAD:
    this->render.frameSequenceActive = 0;
    this->render.animationSequenceActive = 0;
    this->render.timedSequenceActive = 0;
    this->render.isMotionFreezed = false;
  case ST_SIT:
  case 60:
  case 61:
  case 66:
  case 81:
    this->ProcessMotion();
    return true;
  case 51:
    this->ProcessMotion();
    if (this->render.isMotionFinished == true) {
      this->render.state = ST_ATTACK;
    }
    return true;
  default:
    return CGameActor::ProcessState();
  }
}

// OFFSET: 0x00DBA5D0
// FUNCTION: fn_22673
void CPlayer::SetState(int state, int resetPosition) {
  if (this->render.state == ST_DEAD) {
    return;
  }

  if (state != ST_MOVE) {
    this->destCellX = -1;
    this->destCellZ = -1;
  }

  if (this->isPreengageStateOfMove != 0 && this->IsSpecialState(this->render.state)) {
    const int cellX = this->preengageXOfMove;
    const int cellY = this->preengageYOfMove;
    this->isPreengageStateOfMove = 0;
    this->preengageXOfMove = -1;
    this->preengageYOfMove = -1;
    this->SendMsg(nullptr, AM_REQ_MOVE_PRIMITIVE, cellX, cellY, 0, 0, 0, 0);
  }

  if (state == ST_DEAD) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    mode->SendMsg(MM_HIDEMOUSEPOINTER, 0, 0, 0, 0);
    mode->LaunchDummyEffect(&this->render.position, 0x174, 300, 0);
    g_screenShakeDuration = 0;
  }

  CPc::SetState(state, resetPosition);
}
