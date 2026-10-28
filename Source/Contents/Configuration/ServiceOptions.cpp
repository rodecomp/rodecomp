#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ScriptOptions.h"

#include <cstddef>
#include <vector>

struct GameSettingsOption {
  /* 0000 */ int id;
  /* 0004 */ unsigned char unknown_0004[0x54];
  /* 0058 */ unsigned char defaultValue;
  /* 0059 */ unsigned char unknown_0059[0xB];
};

struct GameSettings {
  /* 0000 */ unsigned char unknown_0000[0xC];
  /* 000C */ std::vector<GameSettingsOption> options;

  void ApplyScriptOption(int id, const char* key, ScriptOptions& script);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GameSettingsOption) == 0x64);
static_assert(offsetof(GameSettingsOption, defaultValue) == 0x58);
static_assert(offsetof(GameSettings, options) == 0x0C);
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
