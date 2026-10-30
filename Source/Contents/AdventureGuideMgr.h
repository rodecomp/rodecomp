#ifndef RAGNAROK_SOURCE_CONTENTS_ADVENTUREGUIDEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ADVENTUREGUIDEMGR_H

#include "Script/LuaInstance.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct AdventureGuideDetail {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ std::string text;
};

struct AdventureGuideIntro {
  /* 0000 */ int unknown_0000;
  /* 0004 */ std::string title;
  /* 001C */ std::string text;
  /* 0034 */ std::map<int, AdventureGuideDetail> details;
};

struct AdventureGuideText {
  /* 0000 */ std::string first;
  /* 0018 */ std::string second;
};

struct AdventureGuideEpisodeSource {
  /* 0000 */ const char* name;
  /* 0004 */ int order;
  /* 0008 */ int unknown_0008;
  /* 000C */ std::vector<std::string> values;
};

struct AdventureGuideEpisode {
  AdventureGuideEpisode(AdventureGuideEpisodeSource&& source);

  /* 0000 */ std::string name;
  /* 0018 */ int order;
  /* 001C */ int unknown_001C;
  /* 0020 */ std::vector<std::string> values;
};

struct AdventureGuideContents {
  /* 0000 */ std::uint8_t unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ std::string first;
  /* 0028 */ std::string second;
  /* 0040 */ std::vector<std::string> values;
};

struct AdventureGuideEntry {
  bool IsAvailable(int itemId) const;

  /* 0000 */ std::uint8_t unknown_0000;
  /* 0001 */ std::uint8_t unknown_0001;
  /* 0002 */ std::uint8_t unknown_0002;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ std::string text;
  /* 0024 */ std::vector<std::string> values;
};

struct AdventureGuideResult {
  AdventureGuideResult(bool ok, const char* message) : ok(ok), message(message) {
  }

  AdventureGuideResult(bool ok, const std::string& message) : ok(ok), message(message) {
  }

  /* 0000 */ bool ok;
  /* 0004 */ std::string message;
};

// VTABLE: vt_00350
class CAdventureGuideMgr {
public:
  static bool Create();
  static void Destroy();

  /* 0004 */ CLuaInstance m_lua;
  /* 000C */ std::map<int, AdventureGuideIntro> m_intros;
  /* 0014 */ std::string unknown_0014;
  /* 002C */ std::map<int, AdventureGuideText> unknown_002C;
  /* 0034 */ std::list<AdventureGuideEpisode> m_episodes;
  /* 003C */ std::map<int, std::map<int, AdventureGuideContents>> unknown_003C;
  /* 0044 */ std::map<int, std::map<int, AdventureGuideContents>> unknown_0044;
  /* 004C */ std::map<int, std::map<int, AdventureGuideContents>> unknown_004C;
  /* 0054 */ std::map<int, std::map<int, AdventureGuideEntry>> unknown_0054;
  /* 005C */ std::map<int, std::map<int, AdventureGuideEntry>> unknown_005C;
  /* 0064 */ bool m_initialized;

  CAdventureGuideMgr();
  virtual ~CAdventureGuideMgr();

  bool IsAvailable(int type, int id) const;
  void Init();
  bool Load();
  void GetEpisodeList(std::list<AdventureGuideEpisode>& out);
  void GetContentsList(std::list<std::pair<int, AdventureGuideContents>>& out, int type, int id);
  void GetEntryList(std::list<std::pair<int, AdventureGuideEntry>>& out, int type, int id);

  AdventureGuideResult SetIntroTitle(std::string_view title) {
    unknown_0014 = title;
    return AdventureGuideResult(true, "good");
  }

  __declspec(noinline) AdventureGuideResult InsertGuideMenu(int type, int id, std::string_view name, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertIntroContents(int id, std::string_view title, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertEpisodeSubMenu(int id, int minLevel, int maxLevel, int unknown, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertEpisodeContents(int id, int menu, int minLevel, int maxLevel, int unknown, bool flag, std::string_view first, std::string_view second);
  __declspec(noinline) AdventureGuideResult InsertEpisodeReward(int id, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertQuestSubMenu(int id, int minLevel, int maxLevel, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertQuestContents(int id, int menu, int minLevel, int maxLevel, bool flag, std::string_view first, std::string_view second);
  __declspec(noinline) AdventureGuideResult InsertQuestReward(int id, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertDungeonSubMenu(int id, int minLevel, int maxLevel, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertDungeonContents(int id, int menu, int minLevel, int maxLevel, bool flag, std::string_view first, std::string_view second);
  __declspec(noinline) AdventureGuideResult InsertDungeonReward(int id, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertEntrySubMenu(int type, int id, int minLevel, int maxLevel, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertEntryContents(int type, int id, int menu, int minLevel, int maxLevel, bool flag0, bool flag1, bool flag2, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertObtainGuide(int type, int id, std::string_view text);
  __declspec(noinline) AdventureGuideResult InsertMonsterContents(std::string_view name, int minLevel, int maxLevel);
  __declspec(noinline) AdventureGuideResult InsertMonsterReward(std::string_view name, std::string_view text);
};

extern CAdventureGuideMgr* g_adventureGuideMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AdventureGuideDetail) == 0x24);
static_assert(sizeof(AdventureGuideIntro) == 0x3C);
static_assert(offsetof(AdventureGuideIntro, details) == 0x34);
static_assert(sizeof(AdventureGuideText) == 0x30);
static_assert(sizeof(AdventureGuideEpisodeSource) == 0x18);
static_assert(sizeof(AdventureGuideEpisode) == 0x2C);
static_assert(sizeof(AdventureGuideResult) == 0x1C);
static_assert(offsetof(AdventureGuideEpisode, order) == 0x18);
static_assert(offsetof(AdventureGuideEpisode, values) == 0x20);
static_assert(sizeof(AdventureGuideContents) == 0x4C);
static_assert(offsetof(AdventureGuideContents, values) == 0x40);
static_assert(sizeof(AdventureGuideEntry) == 0x30);
static_assert(offsetof(AdventureGuideEntry, text) == 0xC);
static_assert(offsetof(AdventureGuideEntry, values) == 0x24);
static_assert(offsetof(CAdventureGuideMgr, m_lua) == 0x4);
static_assert(offsetof(CAdventureGuideMgr, m_intros) == 0xC);
static_assert(offsetof(CAdventureGuideMgr, unknown_0014) == 0x14);
static_assert(offsetof(CAdventureGuideMgr, unknown_002C) == 0x2C);
static_assert(offsetof(CAdventureGuideMgr, m_episodes) == 0x34);
static_assert(offsetof(CAdventureGuideMgr, unknown_003C) == 0x3C);
static_assert(offsetof(CAdventureGuideMgr, unknown_0054) == 0x54);
static_assert(offsetof(CAdventureGuideMgr, m_initialized) == 0x64);
static_assert(sizeof(CAdventureGuideMgr) == 0x68);
#endif

#endif
