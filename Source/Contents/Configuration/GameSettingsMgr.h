#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_GAMESETTINGSMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_GAMESETTINGSMGR_H

#include "Script/LuaInstance.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct GameSettingsOption {
  /* 0000 */ int id = 300;
  /* 0004 */ int tab;
  /* 0008 */ int type;
  /* 000C */ std::string title;
  /* 0024 */ std::string tooltip;
  /* 003C */ int tipBoxId = 0;
  /* 0040 */ std::string description;
  /* 0058 */ bool defaultValue;
  /* 005C */ int enabledMessage = 4698;
  /* 0060 */ int disabledMessage = 4698;
};

class CGameSettingsMgr : public Singleton<CGameSettingsMgr> {
public:
  CGameSettingsMgr();
  ~CGameSettingsMgr() override;

  /* 0004 */ CLuaInstance script;
  /* 000C */ std::vector<GameSettingsOption> options;
  /* 0018 */ std::unordered_map<int, std::function<void(int, bool, bool, int, int)>> actions;
  /* 0038 */ std::unordered_map<int, std::function<void()>> functions;

  bool LoadSettings();
  bool ReadOptionTable();
  void ResetToDefaults();
  void ToggleOption(int id);
  void SetOption(int id, bool value, bool notify);
  void ApplyScriptOption(int id, const char* key);
  bool RunFunction(int id);
  std::optional<GameSettingsOption*> FindOption(int id);
  std::list<int> GetOptionIds(int tab);

private:
  void Init_Func();
};

using GameSettings = CGameSettingsMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GameSettingsOption) == 0x64);
static_assert(offsetof(GameSettingsOption, defaultValue) == 0x58);
static_assert(offsetof(GameSettings, options) == 0x0C);
static_assert(sizeof(GameSettings) == 0x58);
#endif

#endif
