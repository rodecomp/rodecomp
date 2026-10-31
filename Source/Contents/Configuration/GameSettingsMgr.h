#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_GAMESETTINGSMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_GAMESETTINGSMGR_H

#include "Contents/Skills/PlayerSkillBundle.h"
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

struct ScriptOptions;

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

#endif
