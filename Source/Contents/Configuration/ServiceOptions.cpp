#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ScriptOptions.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Configuration/OptionWriter.h"
#include "Contents/SkinMgr.h"
#include "File/File.h"
#include "Contents/Sound/SoundMgr.h"
#include "File/ResourcePath.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourcePreload.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/World/World.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Network/ClientOptionPacket.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Script/LuaInstance.h"
#include "Script/LuaScript.h"
#include "Utility/Singleton.h"
#include "Utility/Text/StringUtil.h"

extern "C" {
#include <lua.h>
}

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <functional>
#include <list>
#include <optional>
#include <utility>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

struct GameSettingsOption {
  /* 0000 */ int id = 300;
  /* 0004 */ int tab;
  /* 0008 */ int type;
  /* 000C */ std::string title;
  /* 0024 */ std::string tooltip;
  /* 003C */ int tipBoxId = 0;
  /* 0040 */ std::string description;
  /* 0058 */ unsigned char defaultValue;
  /* 0059 */ unsigned char unknown_0059[3];
  /* 005C */ int unknown_005c = 4698;
  /* 0060 */ int unknown_0060 = 4698;
};

class CGameSettingsMgr : public Singleton<CGameSettingsMgr>, public CLuaInstance {
public:
  CGameSettingsMgr();
  ~CGameSettingsMgr() override;

  /* 000C */ std::vector<GameSettingsOption> options;
  /* 0018 */ std::unordered_map<std::uint32_t, std::function<void(int, unsigned char, bool, int, int)>, SkillIdHash> actions;
  /* 0038 */ std::unordered_map<std::uint32_t, std::function<void()>, SkillIdHash> functions;

  bool Load();
  bool ReadOptionTable();
  void ResetToDefaults();
  void ToggleOption(int id);
  void sub_6C0C40(int id, unsigned char value, bool notify);
  void ApplyScriptOption(int id, const char* key, ScriptOptions& script);
  bool RunFunction(int id);
  std::optional<GameSettingsOption*> FindOption(int id);
  std::list<int> GetOptionIds(int tab);
};

using GameSettings = CGameSettingsMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GameSettingsOption) == 0x64);
static_assert(offsetof(GameSettingsOption, defaultValue) == 0x58);
static_assert(offsetof(GameSettings, options) == 0x0C);
static_assert(sizeof(GameSettings) == 0x58);
#endif

// GLOBAL: gl_24579
ServiceOptions g_serviceOptions;

// GLOBAL: gl_24791
LuaScript* g_luaScript;

// GLOBAL: gl_24569
const float g_jitterDistanceScale = 400.0f;

// GLOBAL: gl_25022
float g_serviceOptionJitterScale = 400.0f;

void ApplyServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage);

// OFFSET: 0x006C48C0
// FUNCTION: fn_45461
void GreetFriends() {
  g_session.sub_DE3D90(nullptr);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("[ To Friends ] : (Hi) *^_^*"), 0x1EFFFF, 0, 0, 0);
}

// OFFSET: 0x006BFE60
// FUNCTION: fn_06299
CGameSettingsMgr::CGameSettingsMgr() {
}

// OFFSET: 0x006C01A0
// FUNCTION: fn_06302
void Noop() {
}

// OFFSET: 0x006C0B80
// FUNCTION: fn_06311
CGameSettingsMgr::~CGameSettingsMgr() {
}

// OFFSET: 0x006C0C40
// FUNCTION: fn_06312
void GameSettings::sub_6C0C40(int id, unsigned char value, bool notify) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    if (option.type != 0) {
      return;
    }

    const auto entry = this->actions.find(static_cast<std::uint32_t>(id));
    if (entry == this->actions.end()) {
      ApplyServiceOption(id, value, notify, option.unknown_005c, option.unknown_0060);
      return;
    }

    const std::function<void(int, unsigned char, bool, int, int)> action = entry->second;
    if (action) {
      action(id, value, notify, option.unknown_005c, option.unknown_0060);
    }
    return;
  }
}

// OFFSET: 0x006C0DD0
// FUNCTION: fn_06313
bool GameSettings::RunFunction(int id) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    if (option.type != 1) {
      return false;
    }

    const auto& functions = this->functions;
    const auto entry = functions.find(static_cast<std::uint32_t>(id));
    if (entry == functions.end()) {
      return false;
    }

    const std::function<void()> function = entry->second;
    if (function) {
      function();
      return true;
    }
    return false;
  }
  return false;
}

// OFFSET: 0x006C1030
// FUNCTION: fn_06315
std::optional<GameSettingsOption*> GameSettings::FindOption(int id) {
  auto option = this->options.begin();
  while (option != this->options.end() && id != option->id) {
    ++option;
  }
  if (option == this->options.end()) {
    return std::nullopt;
  }
  return &*option;
}

// OFFSET: 0x006C1070
// FUNCTION: fn_06316
std::list<int> GameSettings::GetOptionIds(int tab) {
  std::list<int> ids;
  for (const GameSettingsOption& option : this->options) {
    if (option.tab == tab) {
      ids.push_back(option.id);
    }
  }
  return ids;
}

// OFFSET: 0x006C17A0
// FUNCTION: fn_06318
unsigned char ServiceOptions::Get(int id) const {
  const auto entry = this->values.find(static_cast<std::uint32_t>(id));
  return entry == this->values.end() ? 0 : entry->second;
}

// OFFSET: 0x006C1820
// FUNCTION: fn_06319
bool IsServiceOption(int id) {
  switch (id) {
  case 106:
  case 107:
  case 108:
  case 231:
  case 269:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x006C19D0
// FUNCTION: fn_06321
bool GameSettings::ReadOptionTable() {
  lua_State* state = this->m_state;
  if (!state) {
    return false;
  }

  lua_getfield(state, LUA_GLOBALSINDEX, "OptionTbl");
  if (lua_type(state, -1) != LUA_TTABLE) {
    return false;
  }

  this->options.resize(lua_objlen(state, -1));
  lua_pushnil(state);
  while (lua_next(state, -2)) {
    GameSettingsOption option;
    const int index = lua_tointeger(state, -2);

    lua_pushstring(state, "ID");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.id = lua_tointeger(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].ID가 선언되지 않았습니다. 클라이언트로 문의 부탁드립니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Tab");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.tab = lua_tointeger(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Tab이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Type");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.type = lua_tointeger(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Type이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    if (option.type == 0) {
      lua_pushstring(state, "Default");
      lua_gettable(state, -2);
      if (lua_type(state, -1) > LUA_TNIL && lua_isboolean(state, -1)) {
        option.defaultValue = (lua_toboolean(state, -1) != 0) ^ IsServiceOption(option.id);
      } else {
        MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Default가 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
      }
      lua_pop(state, 1);

      lua_pushstring(state, "MSGs");
      lua_gettable(state, -2);
      if (lua_type(state, -1) > LUA_TNIL && lua_istable(state, -1)) {
        lua_pushnumber(state, 1.0);
        lua_gettable(state, -2);
        const int enabledMessage = FindMessageId(lua_tostring(state, -1));
        lua_pop(state, 1);
        lua_pushnumber(state, 2.0);
        lua_gettable(state, -2);
        const int disabledMessage = FindMessageId(lua_tostring(state, -1));
        lua_pop(state, 1);
        option.unknown_005c = enabledMessage;
        option.unknown_0060 = disabledMessage;
      }
      lua_pop(state, 1);

      g_serviceOptions.values[static_cast<std::uint32_t>(option.id)] = option.defaultValue;
    }

    lua_pushstring(state, "Title");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isstring(state, -1)) {
      option.title = lua_tostring(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Title이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Tooltip");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isstring(state, -1)) {
      option.tooltip = lua_tostring(state, -1);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "TipBoxID");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isnumber(state, -1)) {
      option.tipBoxId = lua_tointeger(state, -1);
    }
    lua_pop(state, 1);

    lua_pushstring(state, "Description");
    lua_gettable(state, -2);
    if (lua_type(state, -1) > LUA_TNIL && lua_isstring(state, -1)) {
      option.description = lua_tostring(state, -1);
    } else {
      MessageBoxA(nullptr, std::format("OptionTbl[ {} ].Description이 잘못되었습니다.", index).c_str(), "GameSettings.lua", MB_OK);
    }
    lua_pop(state, 1);

    this->options[index - 1] = std::move(option);
    lua_pop(state, 1);
  }

  lua_pop(state, 1);
  return lua_gettop(state) == 0;
}

void sub_AAB070(int id);

// OFFSET: 0x006C2610
// FUNCTION: fn_06322
void GameSettings::ResetToDefaults() {
  for (const GameSettingsOption& option : this->options) {
    if (option.type == 0) {
      this->sub_6C0C40(option.id, option.defaultValue, true);
    }
  }
}

// OFFSET: 0x006C27D0
// FUNCTION: fn_06323
void GameSettings::ToggleOption(int id) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    if (option.type == 0) {
      const unsigned char current = g_serviceOptions.Get(id);
      this->sub_6C0C40(id, current == 0, true);
      sub_AAB070(id);
    } else if (option.type == 1) {
      const auto entry = this->functions.find(id);
      if (entry != this->functions.end() && entry->second) {
        entry->second();
      }
    }
    break;
  }
}

struct SessionCommandTarget {
  int sub_DE1010(const char* name, int* optionId, std::string* strings);
};

struct ServiceOptionActorRefresh {
  void sub_CD6DD0(char all);
};

// OFFSET: 0x006C29A0
// FUNCTION: fn_06324
int SetOption(const char* name, unsigned char value) {
  if (!name || name[0] == '\0') {
    return 3;
  }

  int optionId = 3;
  std::string strings[3];
  reinterpret_cast<SessionCommandTarget*>(&g_session)->sub_DE1010(name, &optionId, strings);
  if (optionId != 3) {
    g_serviceOptions.values[static_cast<std::uint32_t>(optionId)] = value;
  }
  return optionId;
}

// OFFSET: 0x006C0F00
// FUNCTION: fn_06314
std::pair<int, unsigned char> FindServiceOption(const char* name) {
  if (IsNullOrEmpty(name)) {
    return {3, 0};
  }

  int optionId = 3;
  std::string strings[3];
  reinterpret_cast<SessionCommandTarget*>(&g_session)->sub_DE1010(name, &optionId, strings);
  const auto entry = g_serviceOptions.values.find(static_cast<std::uint32_t>(optionId));
  if (entry == g_serviceOptions.values.end()) {
    return {3, 0};
  }
  return {optionId, entry->second};
}

// OFFSET: 0x006C2A80
// FUNCTION: fn_06325
bool ServiceOptions::Set(int id, unsigned char value) {
  const auto entry = this->values.find(static_cast<std::uint32_t>(id));

  if (entry == this->values.end()) {
    return false;
  }

  entry->second = value;
  return true;
}

// OFFSET: 0x006C1900
// FUNCTION: fn_06320
void GameSettings::ApplyScriptOption(int id, const char* key, ScriptOptions& script) {
  for (const GameSettingsOption& option : this->options) {
    if (option.id != id) {
      continue;
    }

    g_serviceOptions.Set(id, script.Integer(key, option.defaultValue) != 0);
    return;
  }
}

namespace {
#pragma pack(push, 1)
struct ServiceOptionPacket {
  std::int16_t id;
};

struct WhisperBlockPacket {
  std::int16_t id;
  unsigned char enabled;
};

struct ConfigPacket {
  std::int16_t id;
  int type;
  int value;
};
#pragma pack(pop)
}

// OFFSET: 0x006C5DB0
// FUNCTION: fn_06368
void SendServiceOptionPacket() {
  ServiceOptionPacket packet;
  packet.id = 0xAE8;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C5E70
// FUNCTION: fn_06370
void NotifyServiceOption(unsigned char enabled, int enabledMessage, int disabledMessage, int color) {
  std::string text;
  if (enabled != 0) {
    if (enabledMessage != 0x125A) {
      text = LookupMessage(enabledMessage);
    }
  } else if (disabledMessage != 0x125A) {
    text = LookupMessage(disabledMessage);
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0, 0);
  }
}

// OFFSET: 0x006C6050
// FUNCTION: fn_06372
void NotifyGameModeOption() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x11F, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C6080
// FUNCTION: fn_06373
void NotifyGameModeOptionAlt() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x11F, 2, 0, 0, 0);
  }
}

// OFFSET: 0x006C60B0
// FUNCTION: fn_06374
void NotifyGameModeMenu() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x4B, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C40C0
// FUNCTION: fn_06350
void RotatePlayerDirection(bool clockwise) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  CPlayer* player = mode->scene->player;
  if (!player) {
    return;
  }

  float angle = static_cast<float>((8 - player->Get8Dir(player->render.angle)) * 45) - mode->camera->camera.yaw;
  if (clockwise) {
    angle -= 45.0f;
    player->render.angle = angle;
    if (angle < 0.0f) {
      angle += 360.0f;
      player->render.angle = angle;
    }
  } else {
    angle += 45.0f;
    player->render.angle = angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
      player->render.angle = angle;
    }
  }

  mode->SendMsg(0x8B, (static_cast<int>(angle) + 180) / 45 % 8, 0, 0, 0);
}

// OFFSET: 0x006C41B0
// FUNCTION: fn_06351
void SendConfigServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    ConfigPacket packet;
    packet.id = 0x2D8;
    packet.type = 1;
    packet.value = value;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x006C4220
// FUNCTION: fn_06352
void ApplyServiceOptionFlags(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  SetServiceFlags(value, value);
}

// OFFSET: 0x006C4270
// FUNCTION: fn_06353
void ApplyServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }
}

// OFFSET: 0x006C42B0
// FUNCTION: fn_06354
void ToggleSitStand() {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == 933) {
      return;
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  CPlayer* player = mode->scene->player;
  if (!player) {
    return;
  }

  const int motion = (player->render.curMotion != 2) + 1;
  player->render.curMotion = motion;
  player->render.angle = static_cast<float>(std::ceil(player->render.angle));
  mode->SendMsg(0x8B, (static_cast<int>(player->render.angle) + 180) / 45 % 8, motion, 0, 0);

  if (player->render.unknown_0070 == 6) {
    std::list<unsigned long>& times = *reinterpret_cast<std::list<unsigned long>*>(mode->unknown_05D4);
    times.pop_front();
    times.push_back(timeGetTime());
    const unsigned long elapsed = times.back() - times.front();
    if (elapsed - 1501 <= 1498) {
      mode->SendMsg(0xA9, 0, 0, 0, 0);
      for (unsigned long& time : times) {
        time = 0;
      }
    }
  }
}

// OFFSET: 0x006C44A0
// FUNCTION: fn_06355
void ApplyServiceOptionXmasWeather(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  const std::string mapName = mode->MapName();
  if (mapName == "xmas_fild01.rsw" || mapName == "xmas.rsw") {
    if (g_serviceOptions.Get(8)) {
      g_weatherEffect.Snow();
    } else {
      g_weatherEffect.ExtendEffect();
    }
  }
}

// OFFSET: 0x006C4660
// FUNCTION: fn_06357
void ApplyServiceOptionMapFog(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ApplyMapFog(value);
}

// OFFSET: 0x006C46C0
// FUNCTION: fn_06358
void ApplyServiceOptionWithNotice(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (g_serviceOptions.Get(107)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBB4)), 0x64FFFF, 0, 0, 0);
      return;
    }

    reinterpret_cast<ServiceOptionActorRefresh*>(g_modeMgr.GetGameMode())->sub_CD6DD0(0);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }
}

// OFFSET: 0x006C48F0
// FUNCTION: fn_06360
void ApplyServiceOptionActorRefresh(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  reinterpret_cast<ServiceOptionActorRefresh*>(g_modeMgr.GetGameMode())->sub_CD6DD0(0);
}

// OFFSET: 0x006C4750
// FUNCTION: fn_06359
void ShowHelpMessages() {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  int count = 0;
  g_luaScript->Call("GetTableSizeForC", "s>d", "HelpMsgStr", &count);
  for (int index = 1; index <= count; ++index) {
    const char* text = nullptr;
    if (g_luaScript->Call("getHelpMsg", "d>s", index, &text) && std::strlen(text) != 0) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF64, 0, 0, 0);
    }
  }
}

// OFFSET: 0x006C5D10
// FUNCTION: fn_06366
void ApplyServiceOptionSessionFlag(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  *reinterpret_cast<int*>(g_session.unknown_5454) = value;
}

// OFFSET: 0x006C5D60
// FUNCTION: fn_06367
void ApplyServiceOptionCursorClip(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (value) {
    ClipApplicationCursor(hWnd);
  } else {
    ClipCursor(nullptr);
  }
}

// OFFSET: 0x006C5DF0
// FUNCTION: fn_06369
void ApplyServiceOptionAskOnDisable(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(0x128) == value) {
    return;
  }

  if (!value &&
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x1104), 2, true, false, 0, "Message", 0x118, 0x78, 0) == 0xBC) {
    g_serviceOptions.Set(0x128, 1);
    sub_AAB070(0x128);
    return;
  }

  SendClientOption(value, GetRagConnection(), g_packetLengths);
}

// OFFSET: 0x006C5FC0
// FUNCTION: fn_06371
void ApplyServiceOptionWindow159(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0x9F)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C60F0
// FUNCTION: fn_06375
void ApplyServiceOptionWindow190(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(0xBE)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C6150
// FUNCTION: fn_06376
void ApplyServiceOptionWindow267(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(0x10B)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C61B0
// FUNCTION: fn_06377
void ApplyServiceOptionWindows189(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  g_windowMgr.SetWindows189Visible(value);
}

// OFFSET: 0x006C6200
// FUNCTION: fn_06378
void ApplyServiceOptionNotifyWindow(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01C0)) {
    window->SendMsg(nullptr, 6, 0x1F6, 0, 0, 0);
  }
}

// OFFSET: 0x006C6260
// FUNCTION: fn_06379
void ApplyServiceOptionWindow268(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(0x10C)) {
    window->Show(value);
  }
}

// OFFSET: 0x006C6300
// FUNCTION: fn_06380
void ApplyServiceOptionHideWindow(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode && mode->unknown_05D0 && !g_serviceOptions.Get(id)) {
    mode->unknown_05D0->Show(0);
  }
}

// OFFSET: 0x006C6380
// FUNCTION: fn_06381
void EnableWhisperBlock() {
  g_whisperBlockMgr->Clear();
  WhisperBlockPacket packet;
  packet.id = 0xD0;
  packet.enabled = 1;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C63D0
// FUNCTION: fn_06382
void DisableWhisperBlock() {
  WhisperBlockPacket packet;
  packet.id = 0xD0;
  packet.enabled = 0;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C6410
// FUNCTION: fn_06383
void SendWhisperBlockPacket() {
  ServiceOptionPacket packet;
  packet.id = 0xD3;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x006C6450
// FUNCTION: fn_06384
void ApplyServiceOptionAndSnap(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  g_windowMgr.UpdateSnapRectangles();
}

// OFFSET: 0x006C5A80
// FUNCTION: fn_06363
void ApplyServiceOptionWeatherFlag(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  g_weatherEffect.unknown_0001 = value ^ 1;
}

// OFFSET: 0x006C5AD0
// FUNCTION: fn_06364
void ApplyServiceOptionWindow27(int, unsigned char, bool, int, int) {
  if (g_windowMgr.unknown_0304 != 0) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x24B), 0, true, false, 0, "Message", 0x118, 0x78, 0);
    return;
  }

  if (g_windowMgr.unknown_02FC != 0) {
    CMode* mode = g_modeMgr.GetGameMode();
    if (mode) {
      mode->SendMsg(0x52, 0, 0, 0, 0);
    }
    return;
  }

  reinterpret_cast<WindowManagerWindows*>(&g_windowMgr)->EnsureWindow(0x1B);
}

// GLOBAL: gl_24912
template <>
CGameSettingsMgr* Singleton<CGameSettingsMgr>::s_instance = nullptr;

// OFFSET: 0x00AB1040
// FUNCTION: fn_17330
template <>
CGameSettingsMgr* Singleton<CGameSettingsMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CGameSettingsMgr;
  }

  return s_instance;
}

// OFFSET: 0x006C45F0
// FUNCTION: fn_06356
void ApplyServiceOptionJitterScale(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage) {
  if (g_serviceOptions.Get(id) != value) {
    g_serviceOptions.Set(id, value);
    if (notify) {
      NotifyServiceOption(value, enabledMessage, disabledMessage, 0x64FFFF);
    }
  }

  if (value) {
    g_serviceOptionJitterScale = 730.0f;
  } else {
    g_serviceOptionJitterScale = g_jitterDistanceScale;
  }
}

// OFFSET: 0x006C45D0
// FUNCTION: fn_45463
void RequestModeAction48() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x30, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C5A60
// FUNCTION: fn_45464
void RequestModeAction61() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x3D, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C6020
// FUNCTION: fn_45465
void NotifyGameModeOptionFirst() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(0x11F, 1, 0, 0, 0);
  }
}

// OFFSET: 0x006C62C0
// FUNCTION: fn_45462
void RequestPlayerAction31() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (CPlayer* player = mode->scene->player) {
    player->SendMsg(nullptr, 0x1F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006C5B40
// FUNCTION: fn_06365
void ShowPlayerLocation() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  const auto* player = reinterpret_cast<const CRenderObject*>(mode->scene->player);
  if (!player) {
    return;
  }

  int column = 0;
  int row = 0;
  mode->WorldCell(player->render.position.x, player->render.position.z, column, row);

  std::string name = mode->MapName();
  if (name.size() > 4) {
    name.resize(name.size() - 4);
  }

  const std::string text = std::format("{}({}) : {}, {}", g_session.GetMapDisplayName(mode->MapName().c_str()), name.c_str(), column, row);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0x64FFFF, 0, 0, 0);
  if (g_serviceType != 10) {
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text.c_str()), 0x64FFFF, 0, 0, 0);
  }
}


// GLOBAL: gl_25208
bool g_showBattleFieldIcon;

// GLOBAL: gl_25209
int g_configuredOldWidth;

// GLOBAL: gl_25210
int g_configuredOldHeight;

// GLOBAL: gl_25211
int g_configuredDeviceCount;

// GLOBAL: gl_25212
int g_configuredModeCount;

// GLOBAL: gl_25213
int g_renderSystemReset;

// GLOBAL: gl_25214
GUID g_configuredDeviceGuid;

// GLOBAL: gl_25215
char g_configuredDeviceName[0x20];

// GLOBAL: gl_25216
int g_viewportWidth = 1024;

// GLOBAL: gl_25217
int g_viewportHeight = 768;

// GLOBAL: gl_24767
extern const GUID g_defaultDrawGuidDX7 = {};

// OFFSET: 0x00DFE4E0
// FUNCTION: fn_23604
void CSession::LoadOptionInfo() {
  ScriptOptions& options = reinterpret_cast<ScriptOptions&>(*this);

  g_luaScript->Call("SetCmdOnOffList", "");
  ResourceExists("OptionInfo.lua", 1);
  g_textureFiltering = options.Integer("Trilinear", 0);

  const int effectVolume = options.Integer("Effect_Volume", 100);
  CSoundMgr::SetVolume(g_soundMgr, options.Integer("Bgm_Volume", 100));
  g_soundMgr->SetTargetVolume(CSoundMgr::Volume(g_soundMgr));
  g_soundMgr->SetSampleVolume(effectVolume);
  g_soundMgr->Set3DVolume(effectVolume);

  this->m_autoOpenWhisperWindow = options.Integer("AutoOpen1to1Window", 1);
  this->m_autoOpenFriendWhisperWindow = options.Integer("AutoOpen1to1Window_Friend", 1);
  this->m_whisperOpenSound = options.Integer("PlaySound_Open1to1Window", 1);
  this->m_simplifiedSkillList = options.Integer("Simplicity_SkillList", 0);
  this->m_showSkillDescription = options.Integer("Show_SkillDescript", 0);
  g_windowCloseByte = options.Integer("ChangeChatMode", 1) != 0;
  this->m_lockMouse = options.Integer("LockMouse", 0) != 0;
  this->m_channelCopId = options.Integer("ChannelCopID", 0);

  g_outdoorCameraPitch = this->GetOptionFloat("Outdoor_ViewLatitude", -45.0f);
  g_outdoorCameraDistance = this->GetOptionFloat("Outdoor_ViewDistance", 300.0f);
  g_indoorCameraPitch = this->GetOptionFloat("Indoor_ViewLatitude", -45.0f);
  g_indoorCameraDistance = this->GetOptionFloat("Indoor_ViewDistance", 300.0f);

  if (g_indoorCameraPitch < -55.0f) {
    g_indoorCameraPitch = -55.0f;
  } else if (g_indoorCameraPitch > -35.0f) {
    g_indoorCameraPitch = -35.0f;
  }

  if (g_indoorCameraDistance < g_cameraMinDistance) {
    g_indoorCameraDistance = g_cameraMinDistance;
  }

  if (g_indoorCameraDistance > g_cameraBaseDistance) {
    g_indoorCameraDistance = g_cameraBaseDistance;
  }

  Singleton<CGameSettingsMgr>::GetInstance();
  ::SetOption(LookupMessage(0xDB9), options.Integer(LookupMessage(0xDB9), 0) != 0);
  g_serviceOptionJitterScale = g_serviceOptions.Get(0xF1) ? 730.0f : g_jitterDistanceScale;

  if (g_outdoorCameraPitch < -65.0f) {
    g_outdoorCameraPitch = -65.0f;
  } else if (g_outdoorCameraPitch > -25.0f) {
    g_outdoorCameraPitch = -25.0f;
  }

  if (g_outdoorCameraDistance < g_cameraMinDistance) {
    g_outdoorCameraDistance = g_cameraMinDistance;
  }

  if (g_outdoorCameraDistance > g_serviceOptionJitterScale) {
    g_outdoorCameraDistance = g_serviceOptionJitterScale;
  }

  g_skinInfo.m_name = options.String("SkinName", "<Basic Skin>");
  this->unknown_1708 = g_serviceOptions.Get(0x2E);
  g_showBattleFieldIcon = options.Integer("ShowBattleFieldIcon", 1) != 0;
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(ConfineCursor, "MouseExclusive", options);

  const int resetPolicy = this->RenderSystemResetPolicy();
  const int defaultBackend = this->DefaultRenderBackend();
  g_renderSystemReset = options.Integer("RenderSystemReset", 1);

  if (resetPolicy != g_renderSystemReset) {
    g_configuredRenderBackend = defaultBackend;
    g_renderSystemReset = resetPolicy;
  } else {
    g_configuredRenderBackend = options.Integer("RENDERSYSTEM", defaultBackend);
  }

  g_configuredDeviceGuid = options.Guid("DX9DEVICEID", g_defaultDrawGuidDX7);
  strncpy_s(g_configuredDeviceName, sizeof(g_configuredDeviceName), options.String("DX9DEVICENAME", "").c_str(), _TRUNCATE);

  g_configuredFullscreen = options.Integer("ISFULLSCREENMODE", 0);
  g_configuredScreenWidth = options.Integer("WIDTH", 1024);
  g_configuredScreenHeight = options.Integer("HEIGHT", 768);
  g_configuredOldWidth = options.Integer("OLD_WIDTH", 0);
  g_configuredOldHeight = options.Integer("OLD_HEIGHT", 0);

  if (static_cast<unsigned int>(g_configuredScreenHeight) < 664) {
    g_configuredScreenWidth = 1024;
    g_configuredScreenHeight = 768;
  }

  g_viewportWidth = g_configuredScreenWidth;
  g_viewportHeight = g_configuredScreenHeight;
  g_configuredBitsPerPixel = options.Integer("BITPERPIXEL", 16);

  OSVERSIONINFOA version = {sizeof(version)};
  GetVersionExA(&version);

  if (version.dwMajorVersion >= 6 && version.dwMinorVersion >= 2) {
    g_configuredBitsPerPixel = 32;
  }

  g_configuredDeviceCount = options.Integer("DEVICECNT", 0);
  g_configuredModeCount = options.Integer("MODECNT", 0);

  g_effectDetailLevel = options.Integer("SPRITEMODE", 2);
  switch (g_effectDetailLevel) {
  case 0:
    g_spriteHalfWidth = 1;
    [[fallthrough]];
  case 1:
    g_spriteHalfHeight = 1;
    break;
  default:
    g_spriteHalfWidth = 0;
    g_spriteHalfHeight = 0;
    break;
  }

  g_textureDetailLevel = options.Integer("TEXTUREMODE", 2);
  switch (g_textureDetailLevel) {
  case 0:
    g_textureReduction = 4;
    break;
  case 1:
    g_textureReduction = 2;
    break;
  default:
    g_textureReduction = 1;
    break;
  }

  this->m_lockItemDrop = options.Integer("bLockItemDropFromItemWnd", 0) != 0;
  this->m_skipPriceWarning = options.Integer("bWarningPriceCheck", 0) == 0;
  this->m_muteSellSound = options.Integer("bSoundEffectInItemSellWnd", 0) == 0;
  this->m_guildMemberListSort = options.Integer("bGuildMemberListSort", 1) != 0;
  this->m_blockPrivateTab = options.Integer("bBlockPrivateTab", 0) != 0;
  this->m_itemDefaultSort = options.Integer("bItemDefaultSort", 0) != 0;
  this->m_itemAscendingOrder = options.Integer("bItemAscendingOrder", 0) != 0;
  this->m_itemDescendingOrder = options.Integer("bItemDescendingOrder", 0) != 0;
  this->m_showMenuIcon = options.Integer("bShowMenuIcon", 1) != 0;

  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0xF3, LookupMessage(0xDC6), options);
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10B, "bExtendDamage", options);
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10C, "bShowMapName", options);
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10D, "bShowOnlyMineFootprint", options);

  this->m_damageSkinIndex = options.Integer("DamageSkin", 0);
  this->m_damageSkinMode = options.Integer("DamageDir", 0);
  this->m_priorityClass = options.Integer("PriorityClass", 32);

  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10A, "bSummonTranslucent", options);
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x109, "bHidePlayer", options);
  Singleton<CGameSettingsMgr>::GetInstance()->ApplyScriptOption(0x10E, "bQuickSlot", options);
}

// OFFSET: 0x00E01770
