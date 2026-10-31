#include "Contents/Actors/GameActor.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/ActorNameCard.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/MercenaryAI.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"
#include "Render/Renderer.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Effects/RagEffect.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CB1930
void CGameActor::ProcessDamageMessages() {
  for (auto it = this->unknown_02A8.begin(); it != this->unknown_02A8.end();) {
    if (timeGetTime() < it->time) {
      ++it;
      continue;
    }

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
        targetName.assign(card.unknown_007C.empty() ? std::string(LookupMessage(1604)) : card.unknown_007C);
      }

      if (attackerName.empty()) {
        const CActorNameCard& card = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LookupNameCard(attackerAid);
        attackerName.assign(card.unknown_007C.empty() ? std::string(LookupMessage(1604)) : card.unknown_007C);
      }

      if (IsPlayerJob(this->GetEffectiveJob(-1))) {
        if (g_session.m_aid == targetAid) {
          std::sprintf(notice, LookupMessage(1605), attackerName.c_str(), it->damage);
        } else if (g_session.IsPartyMember(targetAid, true)) {
          std::sprintf(partyNotice, LookupMessage(1606), targetName.c_str(), attackerName.c_str(), it->damage);
        }
      } else {
        if (g_session.m_aid == attackerAid) {
          std::sprintf(notice, LookupMessage(1607), targetName.c_str(), it->damage);
        } else if (g_session.IsPartyMember(attackerAid, true)) {
          std::sprintf(partyNotice, LookupMessage(1608), attackerName.c_str(), targetName.c_str(), it->damage);
        }
      }

      if (std::strlen(notice) != 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(notice), 0xFFFF, 16, 0, 0);
      }

      if (std::strlen(partyNotice) != 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(partyNotice), 0xC8C8FF, 17, 0, 0);
      }
    }

    ActorDamageMessage& message = *it;
    this->SendMsg(
      nullptr, message.action, message.action >> 31, reinterpret_cast<int>(&message.targetAid), 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0);
    it = this->unknown_02A8.erase(it);
  }
}

// OFFSET: 0x00CB2040
// FUNCTION: fn_21100
void CGameActor::UpdateWorldCell() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int column;
  int row;
  mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
  mode->SendMsg(0xC, this->render.accountId, column, row, 0);
}

// OFFSET: 0x00CB0FD0
// FUNCTION: fn_21097
#pragma auto_inline(off)
bool CGameActor::OnProcess() {
  if (this->unknown_0278 != 0 && timeGetTime() > static_cast<std::uint32_t>(this->unknown_0278)) {
    this->render.isMotionFreezed = false;
    this->unknown_0278 = 0;
  }

  if (this->unknown_02DC != 0) {
    if (timeGetTime() > static_cast<std::uint32_t>(this->unknown_02DC)) {
      return false;
    }
    unsigned int alpha = (static_cast<std::uint32_t>(this->unknown_02DC) - timeGetTime()) >> 1;
    if (alpha > 255) {
      alpha = 255;
    }
    reinterpret_cast<unsigned char*>(&this->render.color)[3] = static_cast<unsigned char>(alpha);
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CWorld* world = mode->scene;
  const std::uint32_t serverTime = g_session.GetServerTime();
  int moved = 0;

  switch (this->render.unknown_0070) {
  case 0:
    if (this->render.unknown_006C == 1 && this->render.isMotionFinished) {
      this->Slot15(0, 1);
    } else if (this->unknown_0314 == 7 && this->render.accountId == g_session.unknown_1010 && this->unknown_027C != 0 &&
               timeGetTime() > static_cast<std::uint32_t>(this->unknown_027C) + 3000) {
      if (rand() % 100 < 25) {
        const int hungry = g_session.GetPetHungryState(g_session.unknown_1044);
        const int friendly = g_session.GetPetFriendlyState(g_session.m_petFriendly);
        const int action = g_session.GetPetActionNumber(hungry, friendly, 4);
        const int emotion = g_session.GetPetEmotionNumber(g_session.unknown_103C, 4, hungry);
        mode->SendMsg(0x94, emotion, 0, 0, 0);
        mode->SendMsg(0x94, action, 0, 0, 0);
      }
      this->unknown_027C = 0;
    }
    this->Slot08();
    this->unknown_0244 = timeGetTime();
    this->unknown_0258 = serverTime;
    return true;

  case 1:
    if (this->route.Empty()) {
      break;
    } else {
      const float oldX = this->render.position.x;
      const float oldZ = this->render.position.z;
      const int previousIndex = this->unknown_023C;
      std::uint32_t nextTick = 0;
      float cellX = 0.0f;
      float cellY = 0.0f;
      this->unknown_023C = this->route.NextPoint(serverTime, &nextTick, &cellX, &cellY);
      float destX = 0.0f;
      float destZ = 0.0f;
      reinterpret_cast<const TerrainHeight*>(mode)->CellCenter(static_cast<int>(cellX), static_cast<int>(cellY), destX, destZ);
      int dtNext = static_cast<int>(nextTick) - static_cast<int>(this->unknown_0258);
      int dtNow = static_cast<int>(serverTime) - static_cast<int>(this->unknown_0258);
      if (dtNext < 0) {
        dtNext = 0;
      }
      if (dtNow < 0) {
        dtNow = 0;
      }
      if (dtNext > 0 && dtNow > 0) {
        moved = 1;
        const float ratio = static_cast<float>(dtNow) / static_cast<float>(dtNext);
        this->render.position.x = (destX - oldX) * ratio + oldX;
        this->render.position.z = (destZ - oldZ) * ratio + oldZ;
      }
      if (this->unknown_0238 == 0) {
        float angle = this->render.angle;
        if (oldX != this->render.position.x || oldZ != this->render.position.z) {
          angle = (static_cast<float>(std::atan2(static_cast<double>(this->render.position.x - oldX),
                                                 static_cast<double>(-(this->render.position.z - oldZ)))) /
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
      this->unknown_0240 += SquareRoot((this->render.position.x - oldX) * (this->render.position.x - oldX) +
                                       (this->render.position.z - oldZ) * (this->render.position.z - oldZ));
      if (this->unknown_0238 == 0) {
        this->Slot36();
      }
      if (this->unknown_023C != -1 || serverTime < nextTick) {
        if (previousIndex != this->unknown_023C) {
          std::int32_t pointX = 0;
          std::int32_t pointY = 0;
          if (this->route.Point(previousIndex, &pointX, &pointY)) {
            float notifyX = 0.0f;
            float notifyZ = 0.0f;
            reinterpret_cast<const TerrainHeight*>(mode)->CellCenter(pointX, pointY, notifyX, notifyZ);
            this->NotifyCellMove(&notifyX, &notifyZ);
          }
        }
        break;
      }
      this->render.position.x = destX;
      this->render.position.z = destZ;
      this->NotifyCellMove(&this->render.position.x, &this->render.position.z);
      this->Slot15(0, 1);
      this->unknown_0238 = 0;
      this->render.isMotionFreezed = false;
      if (this->unknown_0184 != 0) {
        this->unknown_0184 = 0;
        this->SetFrameSequence(0x60, 2, 2, 0x60, 2, 1, 3000, 0);
      }
      mode->SetFollowTicks(0, 0);
      this->unknown_0244 = timeGetTime();
      this->unknown_0258 = serverTime;
      this->render.position.y = reinterpret_cast<const TerrainHeight*>(world->terrain)->Height(this->render.position.x, this->render.position.z);
      this->render.position.x += this->unknown_03F0.x;
      this->render.position.y += this->unknown_03F0.y;
      this->render.position.z += this->unknown_03F0.z;
      return true;
    }

  case 3:
    this->render.unknown_00C8 = 0;
    this->render.unknown_00D8 = 0;
    this->render.unknown_00DC = 0;
    this->render.isMotionFreezed = false;
    this->unknown_0178 = 0;
    this->Slot08();
    if (this->render.isMotionFinished && this->unknown_02A8.size() == 0) {
      mode->SendMsg(0xD, static_cast<int>(this->render.accountId), 0, 0, 0);
      const int job = this->GetEffectiveJob(-1);
      if ((job > 31 && job < 1000) || reinterpret_cast<CGameActor*>(&g_session)->IsMonsterClass(job) || this->unknown_0314 == 9 ||
          g_session.IsSkillIdInRange6001(job)) {
        this->Slot38();
        return false;
      }
    }
    this->unknown_0244 = timeGetTime();
    this->unknown_0258 = serverTime;
    return true;

  case 4:
    this->unknown_0178 = 0;
    this->Slot08();
    {
      const float oldX = this->render.position.x;
      const float oldZ = this->render.position.z;
      const float deltaX = this->unknown_02D0 - this->render.position.x;
      const float deltaZ = this->unknown_02D4 - this->render.position.z;
      const unsigned int remaining = this->render.unknown_008C - timeGetTime() + 200;
      const unsigned int elapsed = timeGetTime() - this->unknown_0244;
      if (remaining != 0) {
        moved = 1;
        this->render.position.x = deltaX * static_cast<float>(elapsed) / static_cast<float>(remaining) + oldX;
        this->render.position.z = deltaZ * static_cast<float>(elapsed) / static_cast<float>(remaining) + oldZ;
      }
      if (this->render.isMotionFinished) {
        this->render.position.x = this->unknown_02D0;
        this->render.position.z = this->unknown_02D4;
        moved = 1;
        this->Slot15(0, 1);
      }
    }
    break;

  case 5:
  case 7:
  case 0x2D:
    this->unknown_0178 = 0;
  case 2:
  case 9:
  case 0xC:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x38:
  case 0x39:
  case 0x3A:
  case 0x3B:
    this->Slot08();
    if (this->render.isMotionFinished) {
      this->Slot15(0, 1);
    }
    this->unknown_0244 = timeGetTime();
    this->unknown_0258 = serverTime;
    return true;

  case 8:
    this->unknown_0178 = 0;
    this->Slot08();
    {
      const int frames = static_cast<int>(this->GetElapsedFrames());
      if (this->hitTargetAid != 0) {
        if (frames == (frames / 34) * 34) {
          CRenderObject* target = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->hitTargetAid);
          if (target != nullptr) {
            this->Face(target->render.position.x, target->render.position.z);
          }
        }
      }
    }
    this->unknown_0244 = timeGetTime();
    this->unknown_0258 = serverTime;
    return true;

  case 10:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1A:
  case 0x1B:
  case 0x1C:
  case 0x1E:
  case 0x1F:
  case 0x20:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x27:
  case 0x28:
  case 0x29:
  case 0x2A:
  case 0x2B:
  case 0x2C:
  case 0x2F:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x37:
    this->unknown_0178 = 0;
    this->Slot08();
    this->unknown_0244 = timeGetTime();
    this->unknown_0258 = serverTime;
    return true;

  case 0xD:
    if (this->unknown_0178 == 0) {
      this->unknown_0178 = 1;
      if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1)) == 0) {
        this->SetAction(0x10, 4, 1);
      } else if (IsJobWeaponAllowed(this->GetEffectiveJob(-1), this->unknown_0260, this->Slot35()) == false) {
        this->SetAction(0x50, 4, 1);
      } else {
        this->SetAction(0x58, 4, 1);
      }
      this->render.isMotionFreezed = true;
    }
    if (timeGetTime() >= this->unknown_0280) {
      this->unknown_0178 = 0;
      this->render.isMotionFreezed = false;
      this->Slot15(0, 1);
      this->SendMsg(nullptr, 0x53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    this->Slot08();
    this->unknown_0244 = timeGetTime();
    this->unknown_0258 = serverTime;
    return true;

  default:
    break;
  }

  this->unknown_0244 = timeGetTime();
  this->unknown_0258 = serverTime;
  if (moved != 0) {
    this->render.position.y = reinterpret_cast<const TerrainHeight*>(world->terrain)->Height(this->render.position.x, this->render.position.z);
    this->render.position.x += this->unknown_03F0.x;
    this->render.position.y += this->unknown_03F0.y;
    this->render.position.z += this->unknown_03F0.z;
  }
  return true;
}
#pragma auto_inline(on)

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

// OFFSET: 0x00CAFF00
// FUNCTION: fn_21091
void CGameActor::UpdateGaugePositions() {
  // GLOBAL: gl_24991
  static float s_gaugeOffsetY = static_cast<float>(g_renderer->viewport.height * 81 / 480);

  if (this->unknown_0264[0] != 0) {
    if (timeGetTime() < this->unknown_0248 + 5000) {
      UIWindow* window = reinterpret_cast<UIWindow*>(this->unknown_0264[0]);
      int x = this->render.screenX - this->unknown_0288 / 2;
      if (x < -3) {
        x = -3;
      }
      const int maximumX = g_renderer->viewport.width - window->geometry.width + 3;
      if (x > maximumX) {
        x = maximumX;
      }
      int y = static_cast<int>(
        static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.unknown_005C - static_cast<float>(this->unknown_028C)
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
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[0]));
      this->unknown_0264[0] = 0;
    }
  }

  if (this->unknown_0264[1] != 0) {
    reinterpret_cast<UIWindow*>(this->unknown_0264[1])->Move(
      this->render.screenX - 70,
      static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.unknown_005C - 17.0f)
    );
  }

  if (this->unknown_0264[2] != 0) {
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->Move(
      this->render.screenX - 70,
      static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.unknown_005C - 17.0f - 17.0f)
    );
  }

  if (this->unknown_0264[3] != 0) {
    if (timeGetTime() > this->unknown_0280) {
      this->SendMsg(nullptr, 0x53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (this->render.unknown_0070 == 8) {
        this->Slot15(0, 1);
      }
    } else {
      reinterpret_cast<UIWindow*>(this->unknown_0264[3])->Move(
        this->render.screenX - 30,
        static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.unknown_005C)
      );
      const std::uint32_t total = this->unknown_0280 - this->unknown_0284;
      std::uint32_t elapsed = timeGetTime() - this->unknown_0284;
      if (elapsed > total) {
        elapsed = total;
      }
      reinterpret_cast<UIRechargeGage*>(this->unknown_0264[3])->SetValues(elapsed, total);
    }
  }

  if (this->unknown_0264[4] != 0) {
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->Move(
      this->render.screenX - 70,
      static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.unknown_005C - 17.0f - 17.0f)
    );
  }
}

// OFFSET: 0x00CAF260
// FUNCTION: fn_21079
void CGameActor::ReleaseAttachments() {
  if (this->unknown_030C && reinterpret_cast<CMercenaryAI*>(this->unknown_030C)->state) {
    reinterpret_cast<CMercenaryAI*>(this->unknown_030C)->Shutdown();
    delete reinterpret_cast<CMercenaryAI*>(this->unknown_030C);
    this->unknown_030C = 0;
  }

  if (this->unknown_0310 && reinterpret_cast<CMercenaryAI*>(this->unknown_0310)->state) {
    reinterpret_cast<CMercenaryAI*>(this->unknown_0310)->Shutdown();
    delete reinterpret_cast<CMercenaryAI*>(this->unknown_0310);
    this->unknown_0310 = 0;
  }

  for (int index = 0; index < 5; ++index) {
    if (this->unknown_0264[index]) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[index]));
      this->unknown_0264[index] = 0;
    }
  }

  if (this->unknown_0300) {
    g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0300));
    this->unknown_0300 = 0;
  }

  for (CGameObject* object : this->unknown_03B0) {
    object->SendMsg(nullptr, 0x31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  this->unknown_03B0.clear();

  if (g_session.GetSceneUnknown004C() && this->unknown_0188) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->RemoveUnknown0604(this->unknown_0188);
    g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0188));
    this->unknown_0188 = 0;
  }

  if (this->unknown_018C) {
    g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_018C));
    this->unknown_018C = 0;
  }
}

// OFFSET: 0x00CB0220
// FUNCTION: fn_21092
void CGameActor::UpdateStateEffects() {
  if (g_updateTick > 0 && this->render.unknown_00ECBytes[0] == 1) {
    this->render.flags = 0;
    this->render.unknown_00ECBytes[0] = 0;
  }

  if (StateFlagsClear(this->stateFlags)) {
    this->unknown_02D8 = 36000;
    return;
  }

  const DWORD now = timeGetTime();
  if (StateFlag00(this->stateFlags) && static_cast<unsigned>(this->unknown_02D8) >= static_cast<unsigned>(EffectLifetime(22))) {
    this->unknown_02D8 = 0;
    CRagEffect* effect = this->LaunchEffect(22, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, static_cast<std::int64_t>((static_cast<std::uint64_t>(now) >> 4) % 10 + 1), 0, 0, 0, 0);
    }
  }

  if (StateFlag13(this->stateFlags) && static_cast<unsigned>(this->unknown_02D8) >= static_cast<unsigned>(EffectLifetime(33))) {
    this->unknown_02D8 = 0;
    CRagEffect* effect = this->LaunchEffect(33, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, static_cast<std::int64_t>((static_cast<std::uint64_t>(now) >> 4) % 10 + 1), 0, 0, 0, 0);
    }
  }

  ++this->unknown_02D8;
}

// OFFSET: 0x00CB03E0
// FUNCTION: fn_21093
void CGameActor::UpdateHpGauge() {
  if (this->unknown_0300 == 0) {
    return;
  }

  if (g_serviceOptions.Get(0xD5) && reinterpret_cast<UIPcGage*>(this->unknown_0300)->unknown_009C != 0 &&
      !StateFlag01(this->stateFlags) && !StateFlag02(this->stateFlags) && !StateFlag06(this->stateFlags) &&
      g_windowMgr.unknown_0508 != 2) {
    reinterpret_cast<UIPcGage*>(this->unknown_0300)->Show(1);
    if (this->render.unknown_0070 == 3 && reinterpret_cast<UIPcGage*>(this->unknown_0300)->current != 0) {
      reinterpret_cast<UIPcGage*>(this->unknown_0300)->SetValues(0, reinterpret_cast<UIPcGage*>(this->unknown_0300)->maximum);
    }
    reinterpret_cast<UIPcGage*>(this->unknown_0300)->Move(
      this->render.screenX - reinterpret_cast<UIPcGage*>(this->unknown_0300)->geometry.width / 2,
      static_cast<int>(static_cast<float>(ScaleByScreenHeight(12)) * this->render.unknown_005C + static_cast<float>(this->render.screenY))
    );
    reinterpret_cast<UIPcGage*>(this->unknown_0300)->Invalidate();
    return;
  }

  reinterpret_cast<UIPcGage*>(this->unknown_0300)->Show(0);
}

// OFFSET: 0x00CB0A90
// FUNCTION: fn_21095
void CGameActor::UpdateMotionFrame() {
  const int frames = static_cast<int>(this->render.action->FrameCount(this->render.previousAnimation));
  this->render.curMotion = static_cast<int>(this->unknown_0240 * 0.37f * 4.0f / this->render.speed) % frames;
}

// OFFSET: 0x00CB18E0
// FUNCTION: fn_21098
bool CGameActor::UpdateFadeOut() {
  if (static_cast<std::uint32_t>(this->unknown_02DC) > 0) {
    if (timeGetTime() > static_cast<std::uint32_t>(this->unknown_02DC)) {
      return false;
    }

    const std::uint32_t remaining = (static_cast<std::uint32_t>(this->unknown_02DC) - timeGetTime()) / 2;
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
      scale = (g_sinTable[angle >= 90 ? angle : 90] + 1.0f) * 0.25f + g_depthAverageHalf;
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

// OFFSET: 0x00CADAF0
// FUNCTION: fn_21058
int CGameActor::GetGaugeCurrent() {
  if (this->unknown_0300 == 0) {
    return -1;
  }
  return reinterpret_cast<UIPcGage*>(this->unknown_0300)->current;
}

// OFFSET: 0x00CADB10
// FUNCTION: fn_21059
int CGameActor::GetGaugeMaximum() {
  if (this->unknown_0300 == 0) {
    return -1;
  }
  return reinterpret_cast<UIPcGage*>(this->unknown_0300)->maximum;
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
  return this->unknown_02A8.size() > 0;
}

// OFFSET: 0x00CADCF0
// FUNCTION: fn_21067
bool CGameActor::UpdateUnknown03E8(std::int64_t value) {
  bool result = false;
  const int high = static_cast<int>(value >> 32);
  const unsigned int low = static_cast<unsigned int>(value);
  if (this->unknown_03EC <= high && (this->unknown_03EC < high || static_cast<unsigned int>(this->unknown_03E8) < low)) {
    result = true;
  }
  this->unknown_03EC = high;
  this->unknown_03E8 = static_cast<int>(low);
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

// OFFSET: 0x00AA20D0
// FUNCTION: fn_17210
bool CGameActor::HasUnknown0310Entry() {
  const int* entry = reinterpret_cast<const int*>(this->unknown_0310);
  return entry && entry[1] != 0;
}
