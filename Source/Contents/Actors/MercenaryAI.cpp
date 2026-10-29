#include "Contents/Actors/MercenaryAI.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "UI/Window/UIWindowMgr.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>

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
