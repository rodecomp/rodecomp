#include "Contents/Actors/MercenaryAI.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>
#include <timeapi.h>

namespace {

struct SkillOwnerPacket {
  /* 0000 */ std::int16_t opcode;
  /* 0002 */ std::uint32_t aid;
};

#pragma pack(push, 1)
struct ActionRequestPacket {
  /* 0000 */ std::int16_t opcode;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint32_t target;
  /* 000A */ unsigned char action;
};
#pragma pack(pop)

static_assert(sizeof(ActionRequestPacket) == 11);

// INLINED
bool IsHomunAid(std::uint32_t aid) {
  return aid != 0xFFFFFFFF && aid == *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
}

// INLINED
bool IsMercenaryAid(std::uint32_t aid) {
  return aid != 0xFFFFFFFF && aid == *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
}

// INLINED
bool IsLocalPlayerAid(std::uint32_t aid) {
  CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  if (!player) {
    return false;
  }
  return player->Slot50() == static_cast<int>(aid);
}

// INLINED
bool IsFirstSkillRange(unsigned id) {
  return id - 8000u <= 60;
}

// INLINED
bool IsOtherSkillRange(unsigned id) {
  return id - 8200u <= 41 || id - 8400u <= 57;
}

}

// OFFSET: 0x00D9E3F0
// FUNCTION: fn_22327
CMercenaryAI::CMercenaryAI(int kind) : state(nullptr), actionStarted(0), actionDuration(0), kind(kind), lastTick(0) {
}

// OFFSET: 0x00D9E430
// FUNCTION: fn_22328
CMercenaryAI::~CMercenaryAI() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}

// OFFSET: 0x00D9E500
// FUNCTION: fn_22330
int ClampTargetAttackRange(std::uint32_t aid, int distance) {
  CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid));
  if (actor) {
    CGameActor* target = static_cast<CGameActor*>(
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(&actor->unknown_0318[5]))
    );
    if (target) {
      return distance < target->Slot42() ? target->Slot42() : distance;
    }
  }
  return distance;
}

// OFFSET: 0x00D9E570
// FUNCTION: fn_22331
void CMercenaryAI::Shutdown() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}

// OFFSET: 0x00D9E590
// FUNCTION: fn_22332
int CMercenaryAI::Load(const char* path) {
  if (!path) {
    return 1;
  }

  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }

  this->state = luaL_newstate();
  if (!this->state) {
    return 2;
  }

  luaL_openlibs(this->state);
  this->RegisterFunctions();

  if (luaL_loadfile(this->state, path) == 0) {
    if (lua_pcall(this->state, 0, 0, 0) == 0) {
      return 0;
    }
  }

  char message[1024];
  sprintf(message, "%s\n", lua_tostring(this->state, -1));
  MessageBoxA(nullptr, message, "AI.lua error", 0);
  return 3;
}

// OFFSET: 0x00D9E690
// FUNCTION: fn_22333
int CMercenaryAI::Process(std::uint32_t aid) {
  char message[1024];

  CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid));
  if (actor->render.unknown_0070 == 2 || actor->render.unknown_0070 == 7) {
    if (!actor->render.isMotionFinished) {
      return 0;
    }
    actor->Slot15(0, 1);
  }

  if (this->actionStarted == 0) {
    int actionState = 0;
    if (this->kind == 0) {
      actionState = g_session.m_homunActionState;
    } else if (this->kind == 1) {
      actionState = g_session.m_mercenaryActionState;
    }

    if (actionState != 0 && this->kind == 0) {
      this->actionStarted = GetTickCount();
      if (actionState == 29) {
        if (g_session.m_homunJob >= 6048 && g_session.m_homunJob <= 6052) {
          actor->SetAction(0x30, 4, 0);
        } else {
          actor->SetAction(0x38, 4, 0);
        }
        this->actionDuration = 3000;
      }
      return 0;
    }
  } else if (GetTickCount() - this->actionStarted > this->actionDuration) {
    actor->Slot15(0, 1);
    if (this->kind == 0) {
      g_session.m_homunActionState = 0;
    } else if (this->kind == 1) {
      g_session.m_mercenaryActionState = 0;
    }
    this->actionStarted = 0;
    this->actionDuration = 0;
  }

  std::uint32_t now = GetTickCount();
  if (now - this->lastTick < 140) {
    return 0;
  }

  this->lastTick = now;
  if (!this->state) {
    return 0;
  }

  lua_getglobal(this->state, "AI");
  lua_pushnumber(this->state, aid);
  if (lua_pcall(this->state, 1, 0, 0) == 0) {
    return 0;
  }

  sprintf(message, "%s\n", lua_tostring(this->state, -1));
  MessageBoxA(nullptr, message, "AI.lua error", 0);
  this->Shutdown();
  return 1;
}

int AiGetActors(lua_State* state);
int AiTrace(lua_State* state);
int AiGetTick(lua_State* state);
int AiSkillGround(lua_State* state);
int AiIsMonster(lua_State* state);
int AiTraceAI(lua_State* state);
int AiMoveToOwner(lua_State* state);
int AiMove(lua_State* state);
int AiGetMsg(lua_State* state);
int AiGetResMsg(lua_State* state);
int AiSkillObject(lua_State* state);
int AiHomunGetV(lua_State* state);
int AiMercenaryGetV(lua_State* state);
int AiHomunAttack(lua_State* state);
int AiMercenaryAttack(lua_State* state);
int AiTraceValue(lua_State* state);

// OFFSET: 0x00D9E890
// FUNCTION: fn_22334
void CMercenaryAI::RegisterFunctions() {
  lua_register(this->state, "GetActors", AiGetActors);
  lua_register(this->state, "Trace", AiTrace);
  lua_register(this->state, "GetTick", AiGetTick);
  lua_register(this->state, "SkillGround", AiSkillGround);
  lua_register(this->state, "IsMonster", AiIsMonster);
  lua_register(this->state, "TraceAI", AiTraceAI);
  lua_register(this->state, "MoveToOwner", AiMoveToOwner);
  lua_register(this->state, "Move", AiMove);
  lua_register(this->state, "GetMsg", AiGetMsg);
  lua_register(this->state, "GetResMsg", AiGetResMsg);
  lua_register(this->state, "SkillObject", AiSkillObject);

  if (this->kind == 0) {
    lua_register(this->state, "GetV", AiHomunGetV);
    lua_register(this->state, "Attack", AiHomunAttack);
  } else if (this->kind == 1) {
    lua_register(this->state, "GetV", AiMercenaryGetV);
    lua_register(this->state, "Attack", AiMercenaryAttack);
  }

  lua_register(this->state, "TraceValue", AiTraceValue);
}

// OFFSET: 0x00D9EAF0
// FUNCTION: fn_22335
int AiHomunAttack(lua_State* state) {
  if (g_session.GetOption(17) > 0) {
    return 0;
  }

  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  std::uint32_t target = static_cast<std::uint32_t>(lua_tonumber(state, 2));
  if (!IsHomunAid(aid)) {
    return 0;
  }
  if (*reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]) != 0 && IsMercenaryAid(target)) {
    return 0;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  CGameActor* targetActor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(target));
  if (!actor || !targetActor) {
    return 0;
  }
  if (StateFlag06(targetActor->Slot13()) || StateFlag01(targetActor->Slot13()) || StateFlag02(targetActor->Slot13())) {
    return 0;
  }

  // GLOBAL: gl_24908
  static std::uint32_t lastAttackTime;
  if (*reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[0x48]) + lastAttackTime < timeGetTime()) {
    ActionRequestPacket packet;
    packet.opcode = ID_CZ_REQUEST_ACTNPC;
    packet.aid = aid;
    packet.target = target;
    packet.action = 0;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.opcode), &packet);
    lastAttackTime = timeGetTime();
  }

  return 0;
}

// OFFSET: 0x00D9ECA0
// FUNCTION: fn_22336
int AiMercenaryAttack(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  std::uint32_t target = static_cast<std::uint32_t>(lua_tonumber(state, 2));
  if (!IsMercenaryAid(aid)) {
    return 0;
  }
  if (*reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]) != 0 && IsHomunAid(target)) {
    return 0;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  CGameActor* targetActor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(target));
  if (!actor || !targetActor) {
    return 0;
  }
  if (StateFlag06(targetActor->Slot13()) || StateFlag01(targetActor->Slot13()) || StateFlag02(targetActor->Slot13())) {
    return 0;
  }

  // GLOBAL: gl_24909
  static std::uint32_t lastAttackTime;
  if (static_cast<std::uint32_t>(g_session.m_mercenaryStats[7]) + lastAttackTime < timeGetTime()) {
    ActionRequestPacket packet;
    packet.opcode = ID_CZ_REQUEST_ACTNPC;
    packet.aid = aid;
    packet.target = target;
    packet.action = 0;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.opcode), &packet);
    lastAttackTime = timeGetTime();
  }

  return 0;
}

// OFFSET: 0x00D9EE10
// FUNCTION: fn_22337
int AiGetActors(lua_State* state) {
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  lua_newtable(state);

  int index = 1;
  for (std::list<CRenderObject*>::iterator it = world->actors.begin(); it != world->actors.end(); ++it) {
    int accountId = static_cast<int>((*it)->render.accountId);
    lua_pushnumber(state, index++);
    lua_pushnumber(state, accountId);
    lua_settable(state, -3);
  }

  int playerAid = static_cast<int>(g_session.m_aid);
  lua_pushnumber(state, index);
  lua_pushnumber(state, playerAid);
  lua_settable(state, -3);
  return 1;
}

// OFFSET: 0x00D9F270
// FUNCTION: fn_22340
int AiGetTick(lua_State* state) {
  lua_pushnumber(state, GetTickCount());
  return 1;
}

// OFFSET: 0x00D9F2B0
// FUNCTION: fn_22341
int AiHomunGetV(lua_State* state) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int property = static_cast<int>(lua_tonumber(state, 1));
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 2));

  CGameActor* actor = nullptr;
  if (aid == g_session.m_aid) {
    actor = mode->scene->player;
  } else {
    actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  }

  double result = 0.0;
  switch (property) {
  case 0:
    result = static_cast<double>(g_session.m_aid);
    break;

  case 1:
  case 13:
    if (actor) {
      if (aid != g_session.m_aid) {
        if (StateFlag06(actor->Slot13()) || StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
          lua_pushnumber(state, -1.0);
          lua_pushnumber(state, -1.0);
          return 2;
        }
      }

      int cellX = 0, cellY = 0;
      mode->WorldCell(actor->render.position.x, actor->render.position.z, cellX, cellY);

      int skillId = 0;
      int skillLevel = 0;
      if (property == 13) {
        skillId = static_cast<int>(lua_tonumber(state, 3));
        skillLevel = static_cast<int>(lua_tonumber(state, 4));
      }

      std::uint32_t homunAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
      if (actor->render.accountId != g_session.m_aid && !g_session.sub_E1DE30(actor->render.accountId)) {
        CGameActor* targetActor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(homunAid));
        if (targetActor) {
          int targetX = 0, targetY = 0;
          mode->WorldCell(targetActor->render.position.x, targetActor->render.position.z, targetX, targetY);

          int range = 0;
          if (skillId != 0) {
            range = mode->GetSkillAttackRange(skillId, skillLevel);
          }

          int clamped = ClampTargetAttackRange(homunAid, range);
          if (clamped > 0) {
            int dx = targetX - cellX;
            float dist = std::sqrt(static_cast<float>(dx * dx + (targetY - cellY) * (targetY - cellY)));
            double angle = std::acos((1.0f / dist) * static_cast<float>(dx));
            cellX = static_cast<int>(static_cast<float>(cellX) + std::cos(static_cast<float>(angle)) * static_cast<float>(clamped));
            cellY = static_cast<int>(static_cast<float>(cellY) + -std::sin(static_cast<float>(angle)) * static_cast<float>(clamped));

            if (!mode->IsValidCell(cellX, cellY)) {
              if (!mode->GetClosestValidCell(&cellX, &cellY)) {
                cellX = targetX;
                cellY = targetY;
              }
            }
          }
        }
      }

      lua_pushnumber(state, cellX);
      lua_pushnumber(state, cellY);
      return 2;
    }

    lua_pushnumber(state, -1.0);
    lua_pushnumber(state, -1.0);
    return 2;

  case 2:
    return 1;

  case 3:
    if (!actor) {
      result = -1.0;
    } else {
      result = static_cast<double>(actor->render.unknown_0070);
    }
    break;

  case 4: {
    int range = *reinterpret_cast<const int*>(&g_session.unknown_5620[4]);
    if (!actor || range == 0) {
      result = -1.0;
    } else {
      result = static_cast<double>(range);
    }
    break;
  }

  case 5:
    if (!actor) {
      result = -1.0;
    } else {
      result = static_cast<double>(actor->hitTargetAid);
    }
    break;

  case 6:
  case 14: {
    std::uint32_t skillId = static_cast<std::uint32_t>(lua_tonumber(state, 3));
    ClientSkills skills{
      g_session.m_skillBundle,
      g_session.m_homunSkills,
      g_session.m_mercSkills,
      g_session.m_additionalSkills,
      reinterpret_cast<const int&>(g_session.m_jobFlags),
      g_activeSkillEffects
    };
    CSkillInfo skill = skills.LookupFirst(skillId);
    if (!skill.available) {
      result = 1.0;
    } else {
      int range = skill.unknown_001C;
      if (property == 14) {
        int level = static_cast<int>(lua_tonumber(state, 4));
        range = mode->GetSkillAttackRange(skillId, level);
      }
      std::uint32_t homunAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]);
      result = static_cast<double>(ClampTargetAttackRange(homunAid, range));
    }
    lua_pushnumber(state, result);
    return 1;
  }

  case 7:
    if (!actor) {
      result = -1.0;
    } else {
      result = static_cast<double>(actor->GetEffectiveJob(-1) % 6000);
    }
    break;

  case 8:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(g_session.unknown_5568);
    } else if (!g_session.IsHomunAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.m_homunHp);
    }
    break;

  case 9:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(*reinterpret_cast<const int*>(&g_session.unknown_5570[0]));
    } else if (!g_session.IsHomunAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.m_homunSp);
    }
    break;

  case 10:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(g_session.unknown_556C);
    } else if (!g_session.IsHomunAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(*reinterpret_cast<const int*>(&g_session.unknown_55C8[0]));
    }
    break;

  case 11:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(*reinterpret_cast<const int*>(&g_session.unknown_5570[4]));
    } else if (!g_session.IsHomunAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.unknown_55D0);
    }
    break;

  default:
    return 0;
  }

  lua_pushnumber(state, result);
  return 1;
}

// OFFSET: 0x00D9F8E0
// FUNCTION: fn_22342
int AiMercenaryGetV(lua_State* state) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int property = static_cast<int>(lua_tonumber(state, 1));
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 2));

  CGameActor* actor = nullptr;
  if (aid == g_session.m_aid) {
    actor = mode->scene->player;
  } else {
    actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  }

  double result = 0.0;
  switch (property) {
  case 0:
    result = static_cast<double>(g_session.m_aid);
    break;

  case 1:
  case 13:
    if (actor) {
      if (aid != g_session.m_aid) {
        if (StateFlag06(actor->Slot13()) || StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
          lua_pushnumber(state, -1.0);
          lua_pushnumber(state, -1.0);
          return 2;
        }
      }

      int cellX = 0, cellY = 0;
      mode->WorldCell(actor->render.position.x, actor->render.position.z, cellX, cellY);

      int skillId = 0;
      int skillLevel = 0;
      if (property == 13) {
        skillId = static_cast<int>(lua_tonumber(state, 3));
        skillLevel = static_cast<int>(lua_tonumber(state, 4));
      }

      std::uint32_t mercAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
      if (actor->render.accountId != g_session.m_aid && !g_session.sub_E1DE30(actor->render.accountId)) {
        CGameActor* targetActor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(mercAid));
        if (targetActor) {
          int targetX = 0, targetY = 0;
          mode->WorldCell(targetActor->render.position.x, targetActor->render.position.z, targetX, targetY);

          int range = 0;
          if (skillId != 0) {
            range = mode->GetSkillAttackRange(skillId, skillLevel);
          }

          int clamped = ClampTargetAttackRange(*reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]), range);
          if (clamped > 0) {
            int dx = targetX - cellX;
            float dist = std::sqrt(static_cast<float>(dx * dx + (targetY - cellY) * (targetY - cellY)));
            double angle = std::acos((1.0f / dist) * static_cast<float>(dx));
            cellX = static_cast<int>(static_cast<float>(cellX) + std::cos(static_cast<float>(angle)) * static_cast<float>(clamped));
            cellY = static_cast<int>(static_cast<float>(cellY) + -std::sin(static_cast<float>(angle)) * static_cast<float>(clamped));

            if (!mode->IsValidCell(cellX, cellY)) {
              if (!mode->GetClosestValidCell(&cellX, &cellY)) {
                cellX = targetX;
                cellY = targetY;
              }
            }
          }
        }
      }

      lua_pushnumber(state, cellX);
      lua_pushnumber(state, cellY);
      return 2;
    }

    lua_pushnumber(state, -1.0);
    lua_pushnumber(state, -1.0);
    return 2;

  case 2:
    return 1;

  case 3:
    if (!actor) {
      result = -1.0;
    } else {
      result = static_cast<double>(actor->render.unknown_0070);
    }
    break;

  case 4: {
    int range = *reinterpret_cast<const int*>(&g_session.unknown_5690[0]);
    if (!actor || range == 0) {
      result = -1.0;
    } else {
      result = static_cast<double>(range);
    }
    break;
  }

  case 5:
    if (!actor || actor->render.unknown_0070 == 3) {
      result = -1.0;
    } else {
      result = static_cast<double>(actor->hitTargetAid);
    }
    break;

  case 6:
  case 14: {
    std::uint32_t skillId = static_cast<std::uint32_t>(lua_tonumber(state, 3));
    ClientSkills skills{
      g_session.m_skillBundle,
      g_session.m_homunSkills,
      g_session.m_mercSkills,
      g_session.m_additionalSkills,
      reinterpret_cast<const int&>(g_session.m_jobFlags),
      g_activeSkillEffects
    };
    CSkillInfo skill = skills.LookupOther(skillId);
    if (!skill.available) {
      result = 1.0;
    } else {
      int range = skill.unknown_001C;
      if (property == 14) {
        int level = static_cast<int>(lua_tonumber(state, 4));
        range = mode->GetSkillAttackRange(skillId, level);
      }
      std::uint32_t mercAid = *reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5620[8]);
      result = static_cast<double>(ClampTargetAttackRange(mercAid, range));
    }
    lua_pushnumber(state, result);
    return 1;
  }

  case 8:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(g_session.unknown_5568);
    } else if (!g_session.IsMercenaryAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.m_mercenaryHp);
    }
    break;

  case 9:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(*reinterpret_cast<const int*>(&g_session.unknown_5570[0]));
    } else if (!g_session.IsMercenaryAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.m_mercenarySp);
    }
    break;

  case 10:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(g_session.unknown_556C);
    } else if (!g_session.IsMercenaryAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.m_mercenaryMaxHp);
    }
    break;

  case 11:
    if (aid == g_session.m_aid) {
      result = static_cast<double>(*reinterpret_cast<const int*>(&g_session.unknown_5570[4]));
    } else if (!g_session.IsMercenaryAid(aid)) {
      result = -1.0;
    } else {
      result = static_cast<double>(g_session.m_mercenaryMaxSp);
    }
    break;

  case 12:
    if (!actor) {
      result = -1.0;
    } else {
      int job = actor->GetEffectiveJob(-1);
      if (g_session.IsSkillIdInRange6018(job)) {
        result = static_cast<double>(job % 6017 + 1);
      } else {
        result = 1.0;
      }
    }
    break;

  default:
    return 0;
  }

  lua_pushnumber(state, result);
  return 1;
}

// OFFSET: 0x00DA0630
// FUNCTION: fn_22348
int AiTrace(lua_State* state) {
  std::string text = luaL_checkstring(state, 1);
  return 0;
}

// OFFSET: 0x00DA0720
// FUNCTION: fn_22349
int AiTraceAI(lua_State* state) {
  if (!g_session.m_traceAi) {
    return 0;
  }

  const char* text = luaL_checkstring(state, 1);
  if (!text) {
    return 0;
  }

  // GLOBAL: gl_24844
  static std::string lastTraceMessage;
  if (lastTraceMessage == text) {
    return 0;
  }

  lastTraceMessage = text;
  std::string line = text;
  line.append("\n", 1);
  OutputDebugStringA(line.c_str());

  SYSTEMTIME localTime;
  GetLocalTime(&localTime);
  std::uint32_t date = ((localTime.wYear << 8 | static_cast<short>(localTime.wMonth)) << 8) | static_cast<short>(localTime.wDay);
  std::uint32_t time = ((localTime.wHour << 8 | static_cast<short>(localTime.wMinute)) << 8) | static_cast<short>(localTime.wSecond);

  CFile file;
  char path[256];
  GetCurrentDirectoryA(sizeof(path), path);
  char* data = std::strstr(path, "\\data");
  int size = 0;
  if (data) {
    std::memcpy(path, path, data - path);
  }
  std::strcat(path, "\\TraceAI.txt");

  char* buffer = const_cast<char*>(text);
  if (file.Open(path, 0, true, false)) {
    size = file.GetSize();
    buffer = new char[size];
    file.Read(buffer, size);
    file.Close();
  }

  file.Open(path, 0x1001, true, false);
  if (size) {
    file.Write(buffer, size);
  }

  char stamp[256];
  sprintf(stamp, "%d/%d/%d - %d:%d:%d", date >> 16, (date >> 8) & 0xFF, date & 0xFF, (time >> 16) & 0xFF, (time >> 8) & 0xFF, time & 0xFF);
  file.Write(stamp, std::strlen(stamp));
  file.Write(text, std::strlen(text));
  file.Write("\r\n", 2);
  file.Close();
  if (size) {
    delete[] buffer;
  }

  return 0;
}

// OFFSET: 0x00DA0C70
// FUNCTION: fn_22350
int AiTraceValue(lua_State* state) {
  std::string text = luaL_checkstring(state, 1);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 4, 0, 0);
  return 1;
}

// OFFSET: 0x00DA0D90
// FUNCTION: fn_22351
void AiSetTableNumber(lua_State* state, int key, int value) {
  lua_pushnumber(state, key);
  lua_pushnumber(state, value);
  lua_settable(state, -3);
}

namespace {

struct AiMessage {
  /* 0000 */ int type;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ int level;
  /* 0010 */ int skill;
  /* 0014 */ int target;
};

static_assert(sizeof(AiMessage) == 24);

#pragma pack(push, 1)
struct MoveToOwnerPacket {
  /* 0000 */ std::int16_t opcode;
  /* 0002 */ std::uint32_t aid;
};
#pragma pack(pop)

static_assert(sizeof(MoveToOwnerPacket) == 6);

}

// OFFSET: 0x00D9EEF0
// FUNCTION: fn_22338
int AiGetMsg(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  int index;
  if (g_session.IsHomunAid(aid)) {
    index = 0;
  } else if (g_session.IsMercenaryAid(aid)) {
    index = 1;
  } else {
    return 0;
  }

  CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid));
  if (!actor) {
    return 0;
  }

  AiMessage message = *reinterpret_cast<const AiMessage*>(actor->Slot51(index));
  lua_newtable(state);
  lua_pushnumber(state, 1);
  lua_pushnumber(state, message.type);
  lua_settable(state, -3);
  switch (message.type) {
  case 1:
  case 4:
  case 5:
    AiSetTableNumber(state, 2, message.x);
    AiSetTableNumber(state, 3, message.y);
    break;
  case 3:
    AiSetTableNumber(state, 2, message.target);
    break;
  case 7:
    AiSetTableNumber(state, 2, message.level);
    AiSetTableNumber(state, 3, message.skill);
    AiSetTableNumber(state, 4, message.target);
    break;
  case 8:
    AiSetTableNumber(state, 2, message.level);
    AiSetTableNumber(state, 3, message.skill);
    AiSetTableNumber(state, 4, message.x);
    AiSetTableNumber(state, 5, message.y);
    break;
  }

  message.type = 0;
  actor->SetUnknown0318(reinterpret_cast<const float*>(&message), index);
  return 1;
}

// OFFSET: 0x00D9F0B0
// FUNCTION: fn_22339
int AiGetResMsg(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  int index;
  if (g_session.IsHomunAid(aid)) {
    index = 0;
  } else if (g_session.IsMercenaryAid(aid)) {
    index = 1;
  } else {
    return 0;
  }

  CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid));
  if (!actor) {
    return 0;
  }

  AiMessage message = *reinterpret_cast<const AiMessage*>(actor->Slot53(index));
  lua_newtable(state);
  lua_pushnumber(state, 1);
  lua_pushnumber(state, message.type);
  lua_settable(state, -3);
  switch (message.type) {
  case 1:
  case 4:
  case 5:
    AiSetTableNumber(state, 2, message.x);
    AiSetTableNumber(state, 3, message.y);
    break;
  case 3:
    AiSetTableNumber(state, 2, message.target);
    break;
  case 7:
    AiSetTableNumber(state, 2, message.level);
    AiSetTableNumber(state, 3, message.skill);
    AiSetTableNumber(state, 4, message.target);
    break;
  case 8:
    AiSetTableNumber(state, 2, message.level);
    AiSetTableNumber(state, 3, message.skill);
    AiSetTableNumber(state, 4, message.x);
    AiSetTableNumber(state, 5, message.y);
    break;
  }

  message.type = 0;
  actor->SetUnknown0330(reinterpret_cast<const float*>(&message), index);
  return 1;
}

// OFFSET: 0x00D9FF50
// FUNCTION: fn_22343
int AiIsMonster(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  double result = 0.0;
  if (actor) {
    int job = actor->GetEffectiveJob(-1);
    if (((job > 1000 && job < 3999) || static_cast<unsigned>(job - 20021) <= 11978) && actor->unknown_0314 != 7) {
      switch (actor->unknown_0314) {
      case 9:
      case 10:
      case 13:
      case 14:
        result = 0.0;
        break;
      default:
        if (StateFlag06(actor->Slot13()) || StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
          result = 0.0;
        } else {
          result = 1.0;
        }
        break;
      }
    } else if (g_session.IsPlayerJobId(job)) {
      CWorld* world = mode->scene;
      bool ally;
      if (world->unknown_0048 || world->unknown_0054) {
        ally = g_session.IsPartyMember(actor->render.accountId, true);
      } else if (world->unknown_004C) {
        ally = g_session.IsLocalPlayerAid(actor->Slot50()) != 0;
      } else {
        ally = true;
      }
      if (!ally) {
        if (StateFlag06(actor->Slot13()) || StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
          result = 0.0;
        } else {
          result = 1.0;
        }
      }
    }
  }

  lua_pushnumber(state, result);
  return 1;
}

// OFFSET: 0x00DA00D0
// FUNCTION: fn_22344
int AiMove(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  int x = static_cast<int>(lua_tonumber(state, 2));
  int y = static_cast<int>(lua_tonumber(state, 3));

  if (!g_session.IsHomunAid(aid) && !g_session.IsMercenaryAid(aid)) {
    return 0;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  if (!actor) {
    return 0;
  }

  if (!mode->IsValidCell(x, y)) {
    if (!mode->GetClosestValidCell(&x, &y)) {
      return 0;
    }
  }

  int actorX = 0, actorY = 0;
  mode->WorldCell(actor->render.position.x, actor->render.position.z, actorX, actorY);

  if (std::abs(x - actorX) + std::abs(y - actorY) < 25) {
#pragma pack(push, 1)
    struct MoveNpcPacket {
      /* 0000 */ std::int16_t opcode;
      /* 0002 */ std::uint32_t aid;
      /* 0006 */ unsigned char dest[3];
    };
#pragma pack(pop)
    static_assert(sizeof(MoveNpcPacket) == 9);

    MoveNpcPacket packet;
    packet.opcode = ID_CZ_REQUEST_MOVENPC;
    packet.aid = aid;
    packet.dest[0] = static_cast<unsigned char>(x >> 2);
    packet.dest[1] = static_cast<unsigned char>(((y >> 4) & 0x3F) | (x << 6));
    packet.dest[2] = static_cast<unsigned char>(y << 4);

    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.opcode), &packet);
  }

  return 0;
}

// OFFSET: 0x00DA0260
// FUNCTION: fn_22345
int AiMoveToOwner(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  if (!g_session.IsHomunAid(aid) && !g_session.IsMercenaryAid(aid)) {
    return 0;
  }

  if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid)) {
    MoveToOwnerPacket packet;
    packet.opcode = ID_CZ_REQUEST_MOVETOOWNER;
    packet.aid = aid;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.opcode), &packet);
  }

  return 0;
}

// OFFSET: 0x00DA0300
// FUNCTION: fn_22346
int AiSkillGround(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  int skillArgs[4];
  skillArgs[0] = static_cast<int>(lua_tonumber(state, 3));
  skillArgs[1] = static_cast<int>(lua_tonumber(state, 4));
  skillArgs[2] = static_cast<int>(lua_tonumber(state, 5));
  skillArgs[3] = static_cast<int>(lua_tonumber(state, 2));

  if (!IsHomunAid(aid) && !IsMercenaryAid(aid)) {
    return 0;
  }
  if (!IsFirstSkillRange(static_cast<unsigned>(skillArgs[0])) && !IsOtherSkillRange(static_cast<unsigned>(skillArgs[0]))) {
    return 0;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  if (actor->render.unknown_0070 == 1 || actor->render.unknown_0070 == 0 || actor->render.unknown_0070 == 4) {
    mode->SendMsg(0x46, reinterpret_cast<int>(skillArgs), 0, 0, 0);
  }
  mode->SendMsg(0x1a, 0, 0, 0, 0);
  return 0;
}

// OFFSET: 0x00DA0420
// FUNCTION: fn_22347
int AiSkillObject(lua_State* state) {
  std::uint32_t aid = static_cast<std::uint32_t>(lua_tonumber(state, 1));
  unsigned int level = static_cast<unsigned int>(lua_tonumber(state, 2));
  std::uint32_t skillId = static_cast<std::uint32_t>(lua_tonumber(state, 3));
  std::uint32_t target = static_cast<std::uint32_t>(lua_tonumber(state, 4));

  ClientSkills skills{
    g_session.m_skillBundle,
    g_session.m_homunSkills,
    g_session.m_mercSkills,
    g_session.m_additionalSkills,
    reinterpret_cast<const int&>(g_session.m_jobFlags),
    g_activeSkillEffects
  };
  CSkillInfo skill = skills.Lookup(skillId);
  if (!skill.available) {
    return 0;
  }

  if (!g_session.IsHomunAid(aid) && !g_session.IsMercenaryAid(aid)) {
    return 0;
  }

  if (!IsFirstSkillRange(skillId) && !IsOtherSkillRange(skillId)) {
    return 0;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* targetActor = nullptr;
  if (target == g_session.m_aid) {
    targetActor = mode->scene->player;
  } else {
    targetActor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(target));
    if (targetActor) {
      if (StateFlag06(targetActor->Slot13()) || StateFlag01(targetActor->Slot13()) || StateFlag02(targetActor->Slot13())) {
        return 0;
      }
    }
  }

  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  if (targetActor && (actor->render.unknown_0070 == 1 || actor->render.unknown_0070 == 0 || actor->render.unknown_0070 == 4)) {
    mode->SendMsg(0x45, skillId, target, level, 0);
  }
  mode->SendMsg(0x1a, 0, 0, 0, 0);

  if (IsSupportSkill(skillId)) {
    lua_pushnumber(state, 1.0);
    return 1;
  }

  return 0;
}
