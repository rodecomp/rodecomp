#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ScriptOptions.h"
#include "Contents/MessageTable.h"
#include "Client/Session.h"

extern "C" {
#include <lua.h>
}

#include <cstddef>
#include <format>
#include <functional>
#include <string>
#include <vector>
#include <windows.h>

struct GameSettingsOption {
  /* 0000 */ int id;
  /* 0004 */ int tab;
  /* 0008 */ int type;
  /* 000C */ std::string title;
  /* 0024 */ std::string tooltip;
  /* 003C */ int tipBoxId;
  /* 0040 */ std::string description;
  /* 0058 */ unsigned char defaultValue;
  /* 0059 */ unsigned char unknown_0059[3];
  /* 005C */ int unknown_005c;
  /* 0060 */ int unknown_0060;
};

struct GameSettings {
  /* 0000 */ void* vtable;
  /* 0004 */ void* luaInstanceVtable;
  /* 0008 */ lua_State* state;
  /* 000C */ std::vector<GameSettingsOption> options;
  /* 0018 */ std::unordered_map<std::uint32_t, std::function<void(int, unsigned char, bool, int, int)>, SkillIdHash> actions;
  /* 0038 */ std::unordered_map<std::uint32_t, std::function<void()>, SkillIdHash> functions;

  bool Load();
  void ResetToDefaults();
  void ToggleOption(int id);
  void sub_6C0C40(int id, unsigned char value, bool notify);
  void ApplyScriptOption(int id, const char* key, ScriptOptions& script);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GameSettingsOption) == 0x64);
static_assert(offsetof(GameSettingsOption, defaultValue) == 0x58);
static_assert(offsetof(GameSettings, options) == 0x0C);
static_assert(sizeof(GameSettings) == 0x58);
#endif

// GLOBAL: gl_24579
ServiceOptions g_serviceOptions;

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

void sub_6C4270(int id, unsigned char value, bool notify, int unknown_005c, int unknown_0060);
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
