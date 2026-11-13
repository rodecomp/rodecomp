#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorNameCard.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/MercenaryAI.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"
#include "Render/Renderer.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Resource/MSGSTRINGID.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// INLINED
static vector3d& operator+=(vector3d& target, const vector3d& offset) {
  target.x += offset.x;
  target.y += offset.y;
  target.z += offset.z;
  return target;
}

// OFFSET: 0x00CB1930
// FUNCTION: fn_21099
void CGameActor::ProcessDamageMessages() {
  for (auto it = this->willBeAttackedList.begin(); it != this->willBeAttackedList.end();) {
    if (timeGetTime() >= it->time) {
      if (it->damage > 0) {
        const std::uint32_t targetAid = it->targetAid;
        const std::uint32_t attackerAid = it->attackerAid;
        char partyNotice[500];
        char notice[500];
        notice[0] = '\0';
        partyNotice[0] = '\0';

        std::string targetName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(targetAid);
        std::string attackerName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(attackerAid);

        std::size_t position = targetName.find('#');
        if (position != std::string::npos) {
          targetName.resize(position);
        }

        position = attackerName.find('#');
        if (position != std::string::npos) {
          attackerName.resize(position);
        }

        if (targetName.empty()) {
          const CActorNameCard& card = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LookupNameCard(targetAid);
          targetName.assign(card.displayName.empty() ? std::string(LookupMessage(MSI_WHO_IS)) : card.displayName);
        }

        if (attackerName.empty()) {
          const CActorNameCard& card = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LookupNameCard(attackerAid);
          attackerName.assign(card.displayName.empty() ? std::string(LookupMessage(MSI_WHO_IS)) : card.displayName);
        }

        if (IsPlayerJob(this->GetEffectiveJob(-1))) {
          if (g_session.m_aid == targetAid) {
            std::sprintf(notice, LookupMessage(MSI_I_RECEIVED_DAMAGE), attackerName.c_str(), it->damage);
          } else if (g_session.IsPartyMember(targetAid, true)) {
            std::sprintf(partyNotice, LookupMessage(MSI_PARTY_RECEIVED_DAMAGE), targetName.c_str(), attackerName.c_str(), it->damage);
          }
        } else {
          if (g_session.m_aid == attackerAid) {
            std::sprintf(notice, LookupMessage(MSI_I_GAVE_DAMAGE), targetName.c_str(), it->damage);
          } else if (g_session.IsPartyMember(attackerAid, true)) {
            std::sprintf(partyNotice, LookupMessage(MSI_PARTY_GAVE_DAMAGE), attackerName.c_str(), targetName.c_str(), it->damage);
          }
        }

        if (std::strlen(notice) != 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice), 0xFFFF, 16, 0, 0);
        }

        if (std::strlen(partyNotice) != 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(partyNotice), 0xC8C8FF, 17, 0, 0);
        }
      }

      ActorDamageMessage& message = *it;
      this->SendMsg(nullptr, message.action, reinterpret_cast<std::uint32_t>(&message.targetAid), 0, 0, 0, 0, 0);
      it = this->willBeAttackedList.erase(it);
    } else {
      ++it;
    }
  }
}

// OFFSET: 0x00CB2040
// FUNCTION: fn_21100
void CGameActor::RegisterPos() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int column;
  int row;
  mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
  mode->SendMsg(MM_ADDREGISTERPOS, this->render.accountId, column, row, 0);
}

// OFFSET: 0x00CB0FD0
// FUNCTION: fn_21097
#pragma auto_inline(off)
bool CGameActor::ProcessState() {
  if (static_cast<std::uint32_t>(this->freezeEndTick) > 0 && timeGetTime() > static_cast<std::uint32_t>(this->freezeEndTick)) {
    this->render.isMotionFreezed = false;
    this->freezeEndTick = 0;
  }

  if (static_cast<std::uint32_t>(this->vanishTime) > 0) {
    if (timeGetTime() > static_cast<std::uint32_t>(this->vanishTime)) {
      return false;
    }
    const unsigned int alpha = (static_cast<std::uint32_t>(this->vanishTime) - timeGetTime()) >> 1;
    reinterpret_cast<unsigned char*>(&this->render.color)[3] = alpha > 255 ? 255 : static_cast<unsigned char>(alpha);
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CWorld* world = mode->scene;
  const std::uint32_t serverTime = g_session.GetServerTime();
  int moved = 0;

  switch (this->render.state) {
  case ST_AUTOCOUNTER:
    if (this->isCounter == 0) {
      this->isCounter = 1;
      if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) != 0) {
        if (IsJobWeaponAllowed(this->GetEffectiveJob(-1), this->sex, this->GetWeapon())) {
          this->SetAction(0x58, 4, 1);
        } else {
          this->SetAction(0x50, 4, 1);
        }
      } else {
        this->SetAction(0x10, 4, 1);
      }
      this->render.isMotionFreezed = true;
    }
    if (timeGetTime() >= this->skillRechargeEndTick) {
      this->isCounter = 0;
      this->render.isMotionFreezed = false;
      this->SetState(0, 1);
      this->SendMsg(nullptr, AM_DELETE_SKILL_RECHARGEGAGE, 0, 0, 0, 0, 0, 0);
    }
    this->ProcessMotion();
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  case ST_STAND:
    if (this->render.motionType == 1 && this->render.isMotionFinished) {
      this->SetState(0, 1);
    } else if (this->objectType == 7 && this->render.accountId == g_session.petGid && static_cast<std::uint32_t>(this->petEmotionStartTick) > 0 &&
               timeGetTime() > static_cast<std::uint32_t>(this->petEmotionStartTick) + 3000) {
      if (rand() % 100 < 25) {
        const int hungry = g_session.GetPetHungryState(g_session.petHunger);
        const int friendly = g_session.GetPetFriendlyState(g_session.m_petFriendly);
        const int action = g_session.GetPetActionNumber(hungry, friendly, 4);
        const int emotion = g_session.GetPetEmotionNumber(g_session.petJob, 4, hungry);
        mode->SendMsg(MM_PET_ACT, emotion, 0, 0, 0);
        mode->SendMsg(MM_PET_ACT, action, 0, 0, 0);
      }
      this->petEmotionStartTick = 0;
    }
    this->ProcessMotion();
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  case ST_MOVE:
    if (this->route.Empty()) {
      break;
    } else {
      const vector3d old = this->render.position;
      const int previousIndex = this->pathStartCell;
      std::uint32_t nextTick;
      float cellX;
      float cellY;
      this->pathStartCell = this->route.NextPoint(serverTime, &nextTick, &cellX, &cellY);
      float destX;
      float destZ;
      mode->CellCenter(static_cast<int>(cellX), static_cast<int>(cellY), destX, destZ);
      int dtNext = static_cast<int>(nextTick) - static_cast<int>(this->lastServerTime);
      int dtNow = static_cast<int>(serverTime) - static_cast<int>(this->lastServerTime);
      if (dtNext < 0) {
        dtNext = 0;
      }
      if (dtNow < 0) {
        dtNow = 0;
      }
      if (dtNext > 0 && dtNow > 0) {
        moved = 1;
        const float ratio = static_cast<float>(dtNow) / static_cast<float>(dtNext);
        this->render.position.x = (destX - old.x) * ratio + old.x;
        this->render.position.z = (destZ - old.z) * ratio + old.z;
      }
      if (this->isNeverAnimation == 0) {
        float angle;
        if (old.x == this->render.position.x && old.z == this->render.position.z) {
          angle = this->render.angle;
        } else {
          angle = (static_cast<float>(std::atan2(static_cast<double>(this->render.position.x - old.x),
                                                 static_cast<double>(-(this->render.position.z - old.z)))) /
                   3.141592f) *
                  180.0f;
          if (angle >= 360.0f) {
            angle -= 360.0f;
          }
          if (angle < 0.0f) {
            angle += 360.0f;
          }
          this->render.angle = angle;
        }
        this->render.previousAnimation = this->Get8Dir(angle) + this->render.animation;
      }
      const float movedX = this->render.position.x - old.x;
      this->dist += SquareRoot(movedX * movedX + (this->render.position.z - old.z) * (this->render.position.z - old.z));
      if (this->isNeverAnimation == 0) {
        this->ProcessMotionWithDist();
      }
      if (this->pathStartCell != -1 || serverTime < nextTick) {
        if (previousIndex != this->pathStartCell) {
          std::int32_t pointX = 0;
          std::int32_t pointY = 0;
          if (this->route.Point(previousIndex, &pointX, &pointY)) {
            float notifyX = 0.0f;
            float notifyZ = 0.0f;
            mode->CellCenter(pointX, pointY, notifyX, notifyZ);
            this->NotifyCellMove(&notifyX, &notifyZ);
          }
        }
        break;
      }
      this->render.position.x = destX;
      this->render.position.z = destZ;
      this->NotifyCellMove(&this->render.position.x, &this->render.position.z);
      this->SetState(0, 1);
      this->isNeverAnimation = 0;
      this->render.isMotionFreezed = false;
      if (this->isAsuraAttack != 0) {
        this->isAsuraAttack = 0;
        this->SetFrameSequence(0x60, 2, 2, 0x60, 2, 1, 3000, 0);
      }
      mode->SetFollowTicks(0, 0);
      this->lastProcessStateTime = timeGetTime();
      this->lastServerTime = serverTime;
      goto applyHeight;
    }

  case ST_ATTACK:
  case ST_ATTACK2:
  case ST_THROW:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x38:
  case 0x39:
  case 0x3A:
  case 0x3B:
    this->ProcessMotion();
    if (this->render.isMotionFinished) {
      this->SetState(0, 1);
    }
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  case ST_BEGIN_SKILL:
    this->isCounter = 0;
    this->ProcessMotion();
    {
      const int frames = static_cast<int>(this->GetElapsedFrames());
      if (this->hitTargetAid != 0) {
        if (frames % 34 == 0) {
          CRenderObject* target = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->hitTargetAid);
          if (target != nullptr) {
            this->Face(target->render.position.x, target->render.position.z);
          }
        }
      }
    }
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  case ST_PICKUP:
  case ST_SKILL:
  case ST_AB_SPECIAL:
    this->isCounter = 0;
    this->ProcessMotion();
    if (this->render.isMotionFinished) {
      this->SetState(0, 1);
    }
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  case ST_DAMAGE:
    this->isCounter = 0;
    this->ProcessMotion();
    {
      const vector3d old = this->render.position;
      const float deltaX = this->damageDestX - this->render.position.x;
      const float deltaZ = this->damageDestZ - this->render.position.z;
      const unsigned int remaining = this->render.motionStartTime - timeGetTime() + 200;
      const unsigned int elapsed = timeGetTime() - this->lastProcessStateTime;
      if (remaining != 0) {
        moved = 1;
        const float elapsedTime = static_cast<float>(elapsed);
        const float totalTime = static_cast<float>(remaining);
        this->render.position.x = elapsedTime * deltaX / totalTime + old.x;
        this->render.position.z = deltaZ * elapsedTime / totalTime + old.z;
      }
      if (this->render.isMotionFinished) {
        this->render.position.x = this->damageDestX;
        this->render.position.z = this->damageDestZ;
        moved = 1;
        this->SetState(0, 1);
      }
    }
    break;

  case ST_DEAD:
    this->render.frameSequenceActive = 0;
    this->render.animationSequenceActive = 0;
    this->render.timedSequenceActive = 0;
    this->render.isMotionFreezed = false;
    this->isCounter = 0;
    this->ProcessMotion();
    if (this->render.isMotionFinished && this->willBeAttackedList.size() == 0) {
      mode->SendMsg(MM_DELETEREGISTERPOS, static_cast<int>(this->render.accountId), 0, 0, 0);
      const int job = this->GetEffectiveJob(-1);
      if ((job > 31 && job < 1000) || g_session.IsMonsterClass(job) || this->objectType == 9 ||
          g_session.IsCompanionJob(job)) {
        this->MakeCorpse();
        return false;
      }
    }
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  case ST_SLEEP:
  case ST_DANCE:
  case ST_SING:
  case ST_READYSHOT:
  case ST_READYJUMP:
  case ST_LANDJUMP:
  case ST_R_STORM:
  case ST_R_DOWN:
  case ST_R_TURN:
  case ST_R_COUNTER:
  case ST_R_DODGE:
  case ST_A_COUNTER:
  case ST_A_JUMPKICK:
  case ST_THROW2:
  case ST_A_STORM:
  case ST_A_DOWN:
  case ST_A_TURN:
  case ST_SG_DOWN:
  case ST_SG_TURN:
  case ST_NINJASKILL1:
  case ST_NINJASKILL2:
  case ST_NINJAREADY:
  case ST_GUNSKILL1:
  case ST_GUNSKILL2:
  case ST_GUNSKILL3:
  case ST_GUNREADY:
  case ST_GUNREADY2:
  case ST_MADOSKILL:
  case ST_MADOSKILL2:
  case 0x2F:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x37:
    this->isCounter = 0;
    this->ProcessMotion();
    this->lastProcessStateTime = timeGetTime();
    this->lastServerTime = serverTime;
    return true;

  default:
    break;
  }

  this->lastProcessStateTime = timeGetTime();
  this->lastServerTime = serverTime;
  if (moved != 0) {
  applyHeight:
    this->render.position.y = world->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.position += this->positionDelta;
  }
  return true;
}
#pragma auto_inline(on)

// OFFSET: 0x00CA83A0
// FUNCTION: fn_20988
CGameActor::CGameActor()
    : accel(), damageNumberCount(0), lastDamage(0), homunMsg(), homunResMsg(), merMsg(), merResMsg(),
      positionDelta(), shapeShift(0), savedShapeShift(0) {
  this->ResetState();
}

// OFFSET: 0x00CAF4F0
// FUNCTION: fn_21084
void CGameActor::ResetState() {
  this->statusEffects[11] = 0;
  this->statusEffects[2] = 0;
  this->statusEffects[3] = 0;
  this->statusEffects[1] = 0;
  this->statusEffects[0] = 0;
  this->statusEffects[6] = 0;
  this->statusEffects[4] = 0;
  this->statusEffects[5] = 0;
  this->statusEffects[9] = 0;
  this->statusEffects[7] = 0;
  this->statusEffects[8] = 0;
  this->statusEffects[10] = 0;
  this->statusEffects[12] = 0;
  this->render.position.x = 0.0f;
  this->render.position.y = 0.0f;
  this->render.position.z = 0.0f;
  this->render.angle = 0.0f;
  this->render.animation = 0;
  this->render.previousAnimation = 0;
  this->render.curMotion = 0;
  this->render.state = 0;
  this->render.motionType = 0;
  this->render.speed = 1.0f;
  this->render.visibilityOptions[0] = 1;
  this->accel.x = 0.0f;
  this->accel.y = 0.0f;
  this->accel.z = 0.0f;
  this->dist = 0;
  this->render.accountId = 0xFFFFFFFF;
  this->lastServerTime = g_session.GetServerTime();
  this->lastProcessStateTime = timeGetTime();
  this->render.screenX = 0;
  this->render.screenY = 0;
  this->actorWindows[0] = 0;
  this->actorWindows[1] = 0;
  this->actorWindows[2] = 0;
  this->actorWindows[3] = 0;
  this->chatWidth = 0;
  this->nameWidth = 0;
  this->render.isMotionFinished = false;
  this->hitTargetAid = 0;
  this->attackMotion = -1.0f;
  this->pathStartCell = -1;
  this->render.visibilityOptions[1] = 1;
  this->render.motionStartTime = timeGetTime();
  this->petEmotionStartTick = 0;
  this->skillRechargeEndTick = timeGetTime() - 5000;
  this->skillRechargeStartTick = timeGetTime() - 10000;
  this->effectLaunchCnt = 3600;
  this->render.color = 0xFFFFFFFF;
  this->render.visible = 1;
  this->render.flatDepth = 0;
  this->willBeDead = 0;
  this->bodyState = 0;
  this->stateFlags = 0;
  this->SetRiding(0);
  this->effectState = 0;
  this->healthState = 0;
  this->vanishTime = 0;
  this->levelAuraType = 0;
  this->hasLevelAura = 0;
  this->characterLevel = 0;
  this->isCounter = 0;
  this->isTrickDead = 0;
  this->isPlayHitWave = 0;
  std::memset(this->hitWaveName, 0, sizeof(this->hitWaveName));
  this->guildId = 0;
  this->freezeEndTick = 0;
  this->actorType = 0;
  this->sceneNameWindow = nullptr;
  this->actorTitleWindow = nullptr;
  this->isNeverAnimation = 0;
  this->isAsuraAttack = 0;
  this->isBladeStop = 0;
  this->homunAI = nullptr;
  this->merAI = nullptr;
  this->objectType = 4;
  this->actorWindows[4] = 0;
  this->transformJobs[0] = -1;
  this->transformJobs[1] = -1;
  this->transformJobs[2] = -1;
  this->monsterTransformJob = -1;
  this->transformPriority = 0;
  this->charmType = 0;
  this->charmCount = 0;
  this->movementState[0] = 0;
  this->movementState[1] = 0;
  this->movementState[2] = 0;
  this->movementState[3] = 0;
  this->movementState[4] = 0;
  this->movementState[5] = 0;
  this->hasHp = 0;
  this->maxHp = 0;
  this->hpGaugeWindow = nullptr;
  this->hpGaugeFlags = 0;
  this->hasSp = 0;
  this->spGaugeValue = 0;
  this->mountedState = 0;
  this->actorFlags = 0;
  this->emblemVersion = 0;
  this->firstFollowerEffect = nullptr;
  this->secondFollowerEffect = nullptr;

  this->maximumValue = 0;
}

// OFFSET: 0x00CAFF00
// FUNCTION: fn_21091
void CGameActor::UpdateGaugePositions() {
  // GLOBAL: gl_24991
  static float s_gaugeOffsetY = static_cast<float>(g_renderer->viewport.height * 81 / 480);

  if (this->actorWindows[0] != 0) {
    if (timeGetTime() < this->chatTick + 5000) {
      UIWindow* window = this->actorWindows[0];
      int x = this->render.screenX - this->chatWidth / 2;
      if (x < -3) {
        x = -3;
      }
      const int maximumX = g_renderer->viewport.width - window->geometry.width + 3;
      if (x > maximumX) {
        x = maximumX;
      }
      int y = static_cast<int>(
        static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.distanceScale - static_cast<float>(this->chatHeight)
      );
      if (y < -3) {
        y = -3;
      }
      const int maximumY = g_renderer->viewport.height - window->geometry.height + 3;
      if (y > maximumY) {
        y = maximumY;
      }
      window->Move(x, y);
    } else {
      g_windowMgr.Schedule(this->actorWindows[0]);
      this->actorWindows[0] = 0;
    }
  }

  if (this->actorWindows[1] != 0) {
    this->actorWindows[1]->Move(
      this->render.screenX - 70,
      static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.distanceScale - 17.0f)
    );
  }

  if (this->actorWindows[2] != 0) {
    this->actorWindows[2]->Move(
      this->render.screenX - 70,
      static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.distanceScale - 17.0f - 17.0f)
    );
  }

  if (this->actorWindows[3] != 0) {
    if (timeGetTime() > this->skillRechargeEndTick) {
      this->SendMsg(nullptr, AM_DELETE_SKILL_RECHARGEGAGE, 0, 0, 0, 0, 0, 0);
      if (this->render.state == 8) {
        this->SetState(0, 1);
      }
    } else {
      this->actorWindows[3]->Move(
        this->render.screenX - 30,
        static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.distanceScale)
      );
      const std::uint32_t total = this->skillRechargeEndTick - this->skillRechargeStartTick;
      std::uint32_t elapsed = timeGetTime() - this->skillRechargeStartTick;
      if (elapsed > total) {
        elapsed = total;
      }
      static_cast<UIRechargeGage*>(this->actorWindows[3])->SetValues(elapsed, total);
    }
  }

  if (this->actorWindows[4] != 0) {
    this->actorWindows[4]->Move(
      this->render.screenX - 70,
      static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.distanceScale - 17.0f - 17.0f)
    );
  }
}

// OFFSET: 0x00CA8D70
// FUNCTION: fn_20994
CGameActor::~CGameActor() {
  this->ReleaseAttachments();
}

// OFFSET: 0x00CAF260
// FUNCTION: fn_21079
void CGameActor::ReleaseAttachments() {
  if (this->homunAI && this->homunAI->state) {
    this->homunAI->Shutdown();
    delete this->homunAI;
    this->homunAI = nullptr;
  }

  if (this->merAI && this->merAI->state) {
    this->merAI->Shutdown();
    delete this->merAI;
    this->merAI = nullptr;
  }

  if (this->actorWindows[0]) {
    g_windowMgr.Schedule(this->actorWindows[0]);
    this->actorWindows[0] = 0;
  }

  if (this->actorWindows[1]) {
    g_windowMgr.Schedule(this->actorWindows[1]);
    this->actorWindows[1] = 0;
  }

  if (this->actorWindows[2]) {
    g_windowMgr.Schedule(this->actorWindows[2]);
    this->actorWindows[2] = 0;
  }

  if (this->actorWindows[3]) {
    g_windowMgr.Schedule(this->actorWindows[3]);
    this->actorWindows[3] = 0;
  }

  if (this->actorWindows[4]) {
    g_windowMgr.Schedule(this->actorWindows[4]);
    this->actorWindows[4] = 0;
  }

  if (this->hpGaugeWindow) {
    g_windowMgr.Schedule(this->hpGaugeWindow);
    this->hpGaugeWindow = nullptr;
  }

  for (std::list<CGameObject*>::iterator it = this->msgEffectList.begin(); it != this->msgEffectList.end(); ++it) {
    (*it)->SendMsg(nullptr, AM_MSGMASTER_DEAD, 0, 0, 0, 0, 0, 0);
  }
  this->msgEffectList.erase(this->msgEffectList.begin(), this->msgEffectList.end());

  if (g_session.GetSiegeMode() && this->sceneNameWindow) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->RemoveNotificationWindow(reinterpret_cast<int>(this->sceneNameWindow));
    g_windowMgr.Schedule(this->sceneNameWindow);
    this->sceneNameWindow = nullptr;
  }

  if (this->actorTitleWindow) {
    g_windowMgr.Schedule(this->actorTitleWindow);
    this->actorTitleWindow = nullptr;
  }
}

// OFFSET: 0x00CB0220
// FUNCTION: fn_21092
void CGameActor::UpdateStateEffects() {
  if (g_updateTick > 0 && this->render.effectPhaseBytes[0] == 1) {
    this->render.flags = 0;
    this->render.effectPhaseBytes[0] = 0;
  }

  if (StateFlagsClear(this->stateFlags)) {
    this->effectLaunchCnt = 36000;
    return;
  }

  const std::uint64_t now = timeGetTime();
  if (StateFlag00(this->stateFlags) && static_cast<unsigned>(this->effectLaunchCnt) >= static_cast<unsigned>(EffectLifetime(22))) {
    this->effectLaunchCnt = 0;
    CRagEffect* effect = this->LaunchEffect(22, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, AM_SETCOUNT, 0, static_cast<std::int64_t>((now >> 4) % 10 + 1), 0, 0, 0, 0);
    }
  }

  if (StateFlag13(this->stateFlags) && static_cast<unsigned>(this->effectLaunchCnt) >= static_cast<unsigned>(EffectLifetime(33))) {
    this->effectLaunchCnt = 0;
    CRagEffect* effect = this->LaunchEffect(33, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, AM_SETCOUNT, 0, static_cast<std::int64_t>((now >> 4) % 10 + 1), 0, 0, 0, 0);
    }
  }

  ++this->effectLaunchCnt;
}

// OFFSET: 0x00CB03E0
// FUNCTION: fn_21093
void CGameActor::UpdateHpGauge() {
  if (this->hpGaugeWindow == nullptr) {
    return;
  }

  if (g_serviceOptions.Get(0xD5) && this->hpGaugeWindow->displayMode != 0 &&
      !StateFlag01(this->stateFlags) && !StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags) &&
      g_windowMgr.windowVisibilityState != 2) {
    this->hpGaugeWindow->Show(1);
    if (this->render.state == 3 && this->hpGaugeWindow->current != 0) {
      this->hpGaugeWindow->SetValues(0, this->hpGaugeWindow->maximum);
    }
    const int y = static_cast<int>(static_cast<float>(ScaleByScreenHeight(12)) * this->render.distanceScale + static_cast<float>(this->render.screenY));
    this->hpGaugeWindow->Move(
      this->render.screenX - this->hpGaugeWindow->geometry.width / 2, y);
    this->hpGaugeWindow->Invalidate();
    return;
  }

  this->hpGaugeWindow->Show(0);
}

// OFFSET: 0x00CB0A90
// FUNCTION: fn_21095
void CGameActor::ProcessMotionWithDist() {
  const int frames = static_cast<int>(this->render.action->FrameCount(this->render.previousAnimation));
  this->render.curMotion = static_cast<int>(this->dist * 0.37f * 4.0f / this->render.speed) % frames;
}

// OFFSET: 0x00CB18E0
// FUNCTION: fn_21098
bool CGameActor::UpdateFadeOut() {
  if (static_cast<std::uint32_t>(this->vanishTime) > 0) {
    if (timeGetTime() > static_cast<std::uint32_t>(this->vanishTime)) {
      return false;
    }

    const std::uint32_t remaining = (static_cast<std::uint32_t>(this->vanishTime) - timeGetTime()) / 2;
    reinterpret_cast<unsigned char*>(&this->render.color)[3] = static_cast<unsigned char>(remaining > 0xFF ? 0xFF : remaining);
  }

  return true;
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
    if (StateFlag21(flags) == true) {
      return 0x10b6;
    }
  } else if (job == 0x109d) {
    if (StateFlag22(flags) == true) {
      return 0x10b7;
    }
  } else if (job == 0x109c) {
    if (StateFlag19(flags) == true) {
      return 0x10b8;
    }
  } else if (job == 0x10a2) {
    if (StateFlag05(flags) == true) {
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
CGameObject* CGameActor::GetEffectCounter(int effectId) {
  switch (effectId) {
  case HT_BLITZBEAT:
  case SN_FALCONASSAULT:
  case WH_HAWKRUSH:
  case WH_HAWKBOOMERANG:
  case WH_WILD_WALK:
    return this->statusEffects[0];
  }
  switch (effectId) {
  case RA_WUGSTRIKE:
  case RA_WUGBITE:
    return this->statusEffects[1];
  }
  return nullptr;
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
      scale = ((angle >= 90 ? g_sinTable[angle] : g_sinTable[90]) + 1.0f) * 0.25f + g_depthAverageHalf;
    }
  }

  switch (this->GetEffectiveJob(-1)) {
  case JT_CHICKEN:
  case JT_CHICKEN2:
  case JT_CHICKEN_H:
  case JT_CHICKEN2_H:
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
  case JT_WOLF_WINDHAWK:
  case JT_MEISTER_MADOGEAR:
  case JT_DRAGON_KNIGHT_CHICKEN:
  case JT_IMPERIAL_GUARD_CHICKEN:
    return scale * 6.0f;
  case JT_CHICKEN_B:
  case JT_CHICKEN2_B:
  case JT_RUNE_CHICKEN_B:
  case JT_ROYAL_CHICKEN_B:
  case JT_WOLF_RANGER_B:
  case JT_MADOGEAR_B:
    return scale * 4.0f;
  default:
    break;
  }

  if (!g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) && !g_session.IsNpcJob(this->GetEffectiveJob(-1)) &&
      this->render.accountId != g_session.m_aid) {
    return 0.0f;
  }

  if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
    if (this->isRiding) {
      return 4.0f;
    }
    return -1.0f;
  }

  if (g_session.IsDoramJob(this->GetEffectiveJob(-1))) {
    if (this->isRiding) {
      return scale * -3.0f;
    }
    return scale * -2.0f;
  }

  if (!this->isRiding) {
    return 0.0f;
  }

  CPc* pc = dynamic_cast<CPc*>(this);
  if (!pc) {
    return scale * 6.0f;
  }

  switch (static_cast<unsigned int>(pc->appearanceJob)) {
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_NIGHT_WATCH:
    return 0.0f;
  case JT_NOVICE:
  case JT_SUPERNOVICE:
  case JT_NOVICE_H:
  case JT_SUPERNOVICE2:
  case JT_HYPER_NOVICE:
  case JT_DRUID:
  case JT_KARNOS:
  case JT_ALITEA:
    return scale + scale;
  default:
    return scale * 6.0f;
  }
}

// OFFSET: 0x00CAD780
// FUNCTION: fn_21054
float CGameActor::GetNameHeight() {
  switch (this->GetEffectiveJob(-1)) {
  case JT_CHICKEN:
  case JT_CHICKEN2:
  case JT_CHICKEN_H:
  case JT_CHICKEN2_H:
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
  case JT_WOLF_WINDHAWK:
  case JT_MEISTER_MADOGEAR:
  case JT_DRAGON_KNIGHT_CHICKEN:
  case JT_IMPERIAL_GUARD_CHICKEN:
    return 23.0f;
  case JT_CHICKEN_B:
  case JT_CHICKEN2_B:
  case JT_RUNE_CHICKEN_B:
  case JT_ROYAL_CHICKEN_B:
  case JT_WOLF_RANGER_B:
  case JT_MADOGEAR_B:
    return 19.0f;
  default:
    break;
  }

  if (!g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) && !g_session.IsNpcJob(this->GetEffectiveJob(-1)) &&
      this->render.accountId != g_session.m_aid) {
    return 18.0f;
  }

  if (g_session.IsDoramJob(this->GetEffectiveJob(-1)) || !this->isRiding) {
    return 18.0f;
  }

  CPc* pc = dynamic_cast<CPc*>(this);
  if (!pc) {
    return 23.0f;
  }

  switch (static_cast<unsigned int>(pc->appearanceJob)) {
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_NIGHT_WATCH:
    return 18.0f;
  case JT_NOVICE:
  case JT_SUPERNOVICE:
  case JT_NOVICE_H:
  case JT_SUPERNOVICE2:
  case JT_HYPER_NOVICE:
  case JT_DRUID:
  case JT_KARNOS:
  case JT_ALITEA:
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
__declspec(noinline) float CGameActor::GetElapsedFrames() {
  return static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
}

// OFFSET: 0x00CADAF0
// FUNCTION: fn_21058
int CGameActor::GetGaugeCurrent() {
  if (this->hpGaugeWindow == nullptr) {
    return -1;
  }
  return this->hpGaugeWindow->current;
}

// OFFSET: 0x00CADB10
// FUNCTION: fn_21059
int CGameActor::GetGaugeMaximum() {
  if (this->hpGaugeWindow == nullptr) {
    return -1;
  }
  return this->hpGaugeWindow->maximum;
}

// OFFSET: 0x00CADB30
// FUNCTION: fn_21060
void CGameActor::ResetTransformJobs() {
  this->transformJobs[0] = -1;
  this->transformJobs[1] = -1;
  this->transformJobs[2] = -1;
  this->monsterTransformJob = -1;
  this->transformPriority = 0;
}

// OFFSET: 0x00CADC90
// FUNCTION: fn_21065
__declspec(noinline) bool CSession::IsMonsterClass(int job) {
  if ((job > 1000 && job < 3999) || static_cast<unsigned int>(job - 20021) <= 11978) {
    return true;
  }
  return false;
}

// OFFSET: 0x00CADCC0
// FUNCTION: fn_21066
bool CGameActor::HasPendingDamage() {
  return this->willBeAttackedList.size() > 0;
}

// OFFSET: 0x00CADCF0
// FUNCTION: fn_21067
bool CGameActor::UpdateMaximumValue(std::int64_t value) {
  bool result = false;
  if (this->maximumValue < value) {
    result = true;
  }
  this->maximumValue = value;
  return result;
}

// OFFSET: 0x00CADD30
// FUNCTION: fn_21068
int CGameActor::IsPlayerClass(int job) {
  return IsPlayerJob(job);
}

// OFFSET: 0x00CAE1B0
// FUNCTION: fn_21072
void CGameActor::SetStackedEffect(int effectId, bool on) {
  if (on) {
    for (const int id : this->exclusiveEffects) {
      this->ApplyEffectState(id, false);
    }
    this->exclusiveEffects.insert(effectId);
    for (const int id : this->exclusiveEffects) {
      this->ApplyEffectState(id, true);
    }
  } else {
    this->exclusiveEffects.erase(effectId);
    this->ApplyEffectState(effectId, false);
  }
}

// OFFSET: 0x00CAE3B0
// FUNCTION: fn_21073
void __stdcall CreateHallucinationPc(
  int job, int sex, int head, const vector3d* position, int alpha, int weapon, int shield, int bodyPalette
) {
  CPc* pc = new (std::nothrow) CPc;
  if (pc == nullptr) {
    return;
  }

  pc->sex = sex & 1;
  pc->SetActorJob(job);
  pc->SetSprHead(head);
  pc->SetImfFileName();
  pc->SetBodyPalette(weapon);
  pc->SetHeadPalette(shield);
  pc->SetBodyCostume(bodyPalette, 0);

  CHalluPC* hallu = new CHalluPC(pc);
  hallu->render.position.x = position->x;
  hallu->render.position.y = position->y;
  hallu->render.position.z = position->z;
  unsigned char* const argb = reinterpret_cast<unsigned char*>(&hallu->render.color);
  if (alpha != -1) {
    argb[3] = static_cast<unsigned char>(alpha);
  }
  *reinterpret_cast<std::uint16_t*>(argb + 1) = 0xFFFF;
  argb[0] = 0xFF;

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(hallu);
  delete pc;
}

// OFFSET: 0x00AA20D0
// FUNCTION: fn_17210
bool CGameActor::HasActiveMercenaryAI() {
  return this->merAI && this->merAI->state;
}
