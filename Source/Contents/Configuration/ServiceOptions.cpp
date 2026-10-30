#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ScriptOptions.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/World/World.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Script/LuaInstance.h"
#include "Script/LuaScript.h"
#include "Utility/Singleton.h"
#include "Utility/Text/StringUtil.h"

extern "C" {
#include <lua.h>
}

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

void ApplyServiceOption(int id, unsigned char value, bool notify, int enabledMessage, int disabledMessage);

// OFFSET: 0x006BFE60
// FUNCTION: fn_06299
CGameSettingsMgr::CGameSettingsMgr() {
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

// OFFSET: 0x006C5FC0

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
