#include "Contents/AdventureGuideMgr.h"

#include "Client/Session.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <iterator>
#include <new>
#include <optional>
#include <windows.h>

// GLOBAL: gl_24502
CAdventureGuideMgr* g_adventureGuideMgr;

// OFFSET: 0x0066A120
// FUNCTION: fn_05278
int InsertIntro(lua_State* state) {
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must string");
    return 2;
  }

  const double id = lua_tonumber(state, -4);
  const std::string_view name = lua_tostring(state, -3);
  const std::string_view title = lua_tostring(state, -2);
  const std::string_view text = lua_tostring(state, -1);
  g_adventureGuideMgr->SetIntroTitle(title);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(0, static_cast<int>(id), name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertInfo : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066A3C0
// FUNCTION: fn_05279
int InsertIntroContents(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string_view title = lua_tostring(state, -2);
  const std::string_view text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertIntroContents(id, title, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertIntroContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x006697A0
// FUNCTION: fn_05274
int InsertEpisode(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(1, id, name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertEpisode : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00669F10
// FUNCTION: fn_05277
int InsertEpisodeSubMenu(lua_State* state) {
  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -5));
  const int minLevel = static_cast<int>(lua_tonumber(state, -4));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -3));
  const int unknown = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEpisodeSubMenu(id, minLevel, maxLevel, unknown, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertEpisodeSubMenu : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00669A40
// FUNCTION: fn_05275
int InsertEpisodeContents(lua_State* state) {
  if (!lua_isnumber(state, -8)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 6th is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 7th is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 8th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -8));
  const int menu = static_cast<int>(lua_tonumber(state, -7));
  const int minLevel = static_cast<int>(lua_tonumber(state, -6));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -5));
  const int unknown = static_cast<int>(lua_tonumber(state, -4));
  const bool flag = lua_tonumber(state, -3) != 0;
  const std::string first = lua_tostring(state, -2);
  const std::string second = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEpisodeContents(id, menu, minLevel, maxLevel, unknown, flag, first, second);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertEpisodeContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00669D40
// FUNCTION: fn_05276
int InsertEpisodeReward(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEpisodeReward(id, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertEpisodeReward : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066A550
// FUNCTION: fn_05280
int InsertMonster(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(2, id, name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertMonster : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066A7F0
// FUNCTION: fn_05281
int InsertMonsterContents(lua_State* state) {
  if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must string");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  const std::string name = lua_tostring(state, -3);
  const int minLevel = static_cast<int>(lua_tonumber(state, -2));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -1));
  const AdventureGuideResult result = g_adventureGuideMgr->InsertMonsterContents(name, minLevel, maxLevel);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertMonsterContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066AA00
// FUNCTION: fn_05282
int InsertMonsterReward(lua_State* state) {
  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertMonsterReward(name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertMonsterReward : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066AC70
// FUNCTION: fn_05283
int InsertQuest(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(3, id, name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertQuest : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066B3A0
// FUNCTION: fn_05286
int InsertQuestSubMenu(lua_State* state) {
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -4));
  const int minLevel = static_cast<int>(lua_tonumber(state, -3));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertQuestSubMenu(id, minLevel, maxLevel, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertQuestSubMenu : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066AF10
// FUNCTION: fn_05284
int InsertQuestContents(lua_State* state) {
  if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 6th is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 7th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -7));
  const int menu = static_cast<int>(lua_tonumber(state, -6));
  const int minLevel = static_cast<int>(lua_tonumber(state, -5));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -4));
  const bool flag = lua_tonumber(state, -3) != 0;
  const std::string first = lua_tostring(state, -2);
  const std::string second = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertQuestContents(id, menu, minLevel, maxLevel, flag, first, second);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertQuestContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066B1D0
// FUNCTION: fn_05285
int InsertQuestReward(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertQuestReward(id, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertQuestReward : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00668EA0
// FUNCTION: fn_05270
int InsertDungeon(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(4, id, name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertDungeon : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x006695D0
// FUNCTION: fn_05273
int InsertDungeonSubMenu(lua_State* state) {
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -4));
  const int minLevel = static_cast<int>(lua_tonumber(state, -3));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertDungeonSubMenu(id, minLevel, maxLevel, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertDungeonSubMenu : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00669140
// FUNCTION: fn_05271
int InsertDungeonContents(lua_State* state) {
  if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 6th is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 7th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -7));
  const int menu = static_cast<int>(lua_tonumber(state, -6));
  const int minLevel = static_cast<int>(lua_tonumber(state, -5));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -4));
  const bool flag = lua_tonumber(state, -3) != 0;
  const std::string first = lua_tostring(state, -2);
  const std::string second = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertDungeonContents(id, menu, minLevel, maxLevel, flag, first, second);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertDungeonContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00669400
// FUNCTION: fn_05272
int InsertDungeonReward(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertDungeonReward(id, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertDungeonReward : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066B570
// FUNCTION: fn_05287
int InsertWeapon(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(5, id, name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertWeapon : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066BCF0
// FUNCTION: fn_05290
int InsertWeaponSubMenu(lua_State* state) {
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -4));
  const int minLevel = static_cast<int>(lua_tonumber(state, -3));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEntrySubMenu(5, id, minLevel, maxLevel, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertWeaponSubMenu : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066B810
// FUNCTION: fn_05288
int InsertWeaponContents(lua_State* state) {
  if (!lua_isnumber(state, -8)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 6th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 7th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 8th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -8));
  const int menu = static_cast<int>(lua_tonumber(state, -7));
  const int minLevel = static_cast<int>(lua_tonumber(state, -6));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -5));
  const bool flag0 = lua_tonumber(state, -4) != 0;
  const bool flag1 = lua_tonumber(state, -3) != 0;
  const bool flag2 = lua_tonumber(state, -2) != 0;
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEntryContents(5, id, menu, minLevel, maxLevel, flag0, flag1, flag2, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertWeaponContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x0066BB10
// FUNCTION: fn_05289
int InsertWeaponObtainGuide(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertObtainGuide(5, id, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertWeaponObtainGuide : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00668550
// FUNCTION: fn_05266
int InsertArmor(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertGuideMenu(6, id, name, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertArmor : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00668CD0
// FUNCTION: fn_05269
int InsertArmorSubMenu(lua_State* state) {
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -4));
  const int minLevel = static_cast<int>(lua_tonumber(state, -3));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEntrySubMenu(6, id, minLevel, maxLevel, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertArmorSubMenu : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x006687F0
// FUNCTION: fn_05267
int InsertArmorContents(lua_State* state) {
  if (!lua_isnumber(state, -8)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must number");
    return 2;
  }

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 6th is must number");
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 7th is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 8th is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -8));
  const int menu = static_cast<int>(lua_tonumber(state, -7));
  const int minLevel = static_cast<int>(lua_tonumber(state, -6));
  const int maxLevel = static_cast<int>(lua_tonumber(state, -5));
  const bool flag0 = lua_tonumber(state, -4) != 0;
  const bool flag1 = lua_tonumber(state, -3) != 0;
  const bool flag2 = lua_tonumber(state, -2) != 0;
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertEntryContents(6, id, menu, minLevel, maxLevel, flag0, flag1, flag2, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertArmorContents : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x00668AF0
// FUNCTION: fn_05268
int InsertArmorObtainGuide(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1st is must number");
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2nd is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  const std::string text = lua_tostring(state, -1);
  const AdventureGuideResult result = g_adventureGuideMgr->InsertObtainGuide(6, id, text);

  if (!result.ok) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatString("InsertArmorObtainGuide : %s", result.message.c_str()));
  } else {
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

using AdventureGuideEntryTable = std::map<int, std::map<int, AdventureGuideEntry>>;
using AdventureGuideContentsTable = std::map<int, std::map<int, AdventureGuideContents>>;

// OFFSET: 0x0065F680
// FUNCTION: fn_05189
static std::optional<AdventureGuideEntryTable::mapped_type*> FindMapped(AdventureGuideEntryTable& map, const int& key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return std::nullopt;
  }
  return &it->second;
}

// OFFSET: 0x0065F6D0
// FUNCTION: fn_05190
static std::optional<AdventureGuideContentsTable::mapped_type*> FindMapped(AdventureGuideContentsTable& map, const int& key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return std::nullopt;
  }
  return &it->second;
}

// OFFSET: 0x00660F70
// FUNCTION: fn_05222
CAdventureGuideMgr::CAdventureGuideMgr() : m_initialized(true) {
}

// OFFSET: 0x006611F0
// FUNCTION: fn_05224
void CAdventureGuideMgr::Init() {
}

// OFFSET: 0x006629D0
// FUNCTION: fn_05227
CAdventureGuideMgr::~CAdventureGuideMgr() {
}

// OFFSET: 0x006635C0
// FUNCTION: fn_05234
bool CAdventureGuideMgr::Create() {
  if (g_adventureGuideMgr) {
    return false;
  }

  g_adventureGuideMgr = new (std::nothrow) CAdventureGuideMgr();
  if (!g_adventureGuideMgr) {
    return false;
  }

  if (g_adventureGuideMgr->m_initialized) {
    if (g_adventureGuideMgr->Load()) {
      return true;
    }
    if (g_adventureGuideMgr) {
      delete g_adventureGuideMgr;
    }
  } else {
    delete g_adventureGuideMgr;
  }

  g_adventureGuideMgr = nullptr;
  return false;
}

// OFFSET: 0x00663680
// FUNCTION: fn_05235
void CAdventureGuideMgr::Destroy() {
  if (g_adventureGuideMgr) {
    delete g_adventureGuideMgr;
    g_adventureGuideMgr = nullptr;
  }
}

// OFFSET: 0x00663890
// FUNCTION: fn_05237
void CAdventureGuideMgr::GetEpisodeList(std::list<AdventureGuideEpisode>& out) {
  out.clear();

  for (const AdventureGuideEpisode& episode : m_episodes) {
    if (episode.order <= g_session.m_level && g_session.m_level <= episode.unknown_001C) {
      out.push_back(episode);
    }
  }
}

// OFFSET: 0x006639D0
// FUNCTION: fn_05238
void CAdventureGuideMgr::GetContentsList(std::list<std::pair<int, AdventureGuideContents>>& out, int type, int id) {
  out.clear();

  std::map<int, std::map<int, AdventureGuideContents>>* table;
  switch (type) {
  case 1:
    table = &unknown_003C;
    break;
  case 3:
    table = &unknown_0044;
    break;
  case 4:
    table = &unknown_004C;
    break;
  default:
    return;
  }

  std::optional<std::map<int, AdventureGuideContents>*> contents = FindMapped(*table, id);
  if (!contents) {
    return;
  }

  std::copy_if((*contents)->begin(), (*contents)->end(), std::back_inserter(out),
               [](const std::pair<const int, AdventureGuideContents>& entry) {
                 return entry.second.unknown_0004 <= g_session.m_level && g_session.m_level <= entry.second.unknown_0008;
               });
  out.sort([](const std::pair<int, AdventureGuideContents>& lhs, const std::pair<int, AdventureGuideContents>& rhs) {
    return lhs.second.unknown_0004 < rhs.second.unknown_0004;
  });
}

// OFFSET: 0x006636A0
// FUNCTION: fn_05236
void CAdventureGuideMgr::GetEntryList(std::list<std::pair<int, AdventureGuideEntry>>& out, int type, int id) {
  out.clear();

  std::map<int, std::map<int, AdventureGuideEntry>>* table;
  switch (type) {
  case 5:
    table = &unknown_0054;
    break;
  case 6:
    table = &unknown_005C;
    break;
  default:
    return;
  }

  std::optional<std::map<int, AdventureGuideEntry>*> entries = FindMapped(*table, id);
  if (!entries) {
    return;
  }

  std::copy((*entries)->begin(), (*entries)->end(), std::back_inserter(out));
  out.sort([](const std::pair<int, AdventureGuideEntry>& lhs, const std::pair<int, AdventureGuideEntry>& rhs) {
    return lhs.second.unknown_0004 < rhs.second.unknown_0004;
  });
}

// OFFSET: 0x00664EE0
// FUNCTION: fn_05240
AdventureGuideResult CAdventureGuideMgr::InsertDungeonContents(int id, int menu, int minLevel, int maxLevel, bool flag, std::string_view first, std::string_view second) {
  const AdventureGuideContents contents{flag, minLevel, maxLevel, 0, std::string(first), std::string(second)};
  unknown_004C[menu].try_emplace(id, contents);
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00665150
// FUNCTION: fn_05241
AdventureGuideResult CAdventureGuideMgr::InsertDungeonReward(int id, std::string_view text) {
  const auto it = std::find_if(unknown_004C.begin(), unknown_004C.end(), [&](const auto& entry) {
    return entry.second.find(id) != entry.second.end();
  });
  if (it == unknown_004C.end()) {
    return AdventureGuideResult(false, FormatOwnedString("[%d] cannot be find", id));
  }

  it->second[id].values.emplace_back(text.data());
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00665340
// FUNCTION: fn_05242
AdventureGuideResult CAdventureGuideMgr::InsertDungeonSubMenu(int id, int minLevel, int maxLevel, std::string_view text) {
  const AdventureGuideDetail detail{minLevel, maxLevel, 0, std::string(text)};
  for (auto& [key, intro] : m_intros) {
    if (intro.unknown_0000 == 4) {
      intro.details.try_emplace(id, detail);
      break;
    }
  }

  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x006654E0
// FUNCTION: fn_05243
AdventureGuideResult CAdventureGuideMgr::InsertEpisodeContents(int id, int menu, int minLevel, int maxLevel, int unknown, bool flag, std::string_view first, std::string_view second) {
  const AdventureGuideContents contents{flag, minLevel, maxLevel, unknown, std::string(first), std::string(second)};
  unknown_003C[menu].try_emplace(id, contents);
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00665750
// FUNCTION: fn_05244
AdventureGuideResult CAdventureGuideMgr::InsertEpisodeReward(int id, std::string_view text) {
  const auto it = std::find_if(unknown_003C.begin(), unknown_003C.end(), [&](const auto& entry) {
    return entry.second.find(id) != entry.second.end();
  });
  if (it == unknown_003C.end()) {
    return AdventureGuideResult(false, FormatOwnedString("[%d] cannot be find", id));
  }

  it->second[id].values.emplace_back(text.data());
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00665940
// FUNCTION: fn_05245
AdventureGuideResult CAdventureGuideMgr::InsertEpisodeSubMenu(int id, int minLevel, int maxLevel, int unknown, std::string_view text) {
  const AdventureGuideDetail detail{minLevel, maxLevel, unknown, std::string(text)};
  for (auto& [key, intro] : m_intros) {
    if (intro.unknown_0000 == 1) {
      intro.details.try_emplace(id, detail);
      break;
    }
  }

  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00665AD0
// FUNCTION: fn_05246
AdventureGuideResult CAdventureGuideMgr::InsertEntryContents(int type, int id, int menu, int minLevel, int maxLevel, bool flag0, bool flag1, bool flag2, std::string_view text) {
  const AdventureGuideEntry entry{flag0, flag1, flag2, minLevel, maxLevel, std::string(text)};
  switch (type) {
  case 5:
    unknown_0054[menu].try_emplace(id, entry);
    break;
  case 6:
    unknown_005C[menu].try_emplace(id, entry);
    break;
  }

  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00665CE0
// FUNCTION: fn_05247
AdventureGuideResult CAdventureGuideMgr::InsertObtainGuide(int type, int id, std::string_view text) {
  std::map<int, std::map<int, AdventureGuideEntry>>::iterator it{};
  const auto contains = [&](const auto& entry) {
    return entry.second.find(id) != entry.second.end();
  };

  if (type == 5) {
    it = std::find_if(unknown_0054.begin(), unknown_0054.end(), contains);
    if (it == unknown_0054.end()) {
      return AdventureGuideResult(false, FormatOwnedString("[%d] cannot be find", id));
    }
  } else if (type == 6) {
    it = std::find_if(unknown_005C.begin(), unknown_005C.end(), contains);
    if (it == unknown_005C.end()) {
      return AdventureGuideResult(false, FormatOwnedString("[%d] cannot be find", id));
    }
  }

  it->second[id].values.emplace_back(text.data());
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666080
// FUNCTION: fn_05248
AdventureGuideResult CAdventureGuideMgr::InsertEntrySubMenu(int type, int id, int minLevel, int maxLevel, std::string_view text) {
  const AdventureGuideDetail detail{minLevel, maxLevel, 0, std::string(text)};
  for (auto& [key, intro] : m_intros) {
    if (intro.unknown_0000 == type) {
      intro.details.try_emplace(id, detail);
      break;
    }
  }

  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666220
// FUNCTION: fn_05249
AdventureGuideResult CAdventureGuideMgr::InsertIntroContents(int id, std::string_view title, std::string_view text) {
  const AdventureGuideText entry{std::string(title), std::string(text)};
  unknown_002C.try_emplace(id, entry);
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666540
// FUNCTION: fn_05250
AdventureGuideResult CAdventureGuideMgr::InsertGuideMenu(int type, int id, std::string_view name, std::string_view text) {
  if (type != id) {
    return AdventureGuideResult(false, "GuideType Error");
  }

  const AdventureGuideIntro intro{type, std::string(name), std::string(text)};
  m_intros.try_emplace(id, intro);
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666860
// FUNCTION: fn_05251
AdventureGuideResult CAdventureGuideMgr::InsertMonsterContents(std::string_view name, int minLevel, int maxLevel) {
  m_episodes.emplace_back(AdventureGuideEpisodeSource{name.data(), minLevel, maxLevel});
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666A50
// FUNCTION: fn_05252
AdventureGuideResult CAdventureGuideMgr::InsertMonsterReward(std::string_view name, std::string_view text) {
  if (m_episodes.empty()) {
    return AdventureGuideResult(false, "Contents Empty");
  }

  AdventureGuideEpisode& episode = m_episodes.back();
  if (name != episode.name) {
    return AdventureGuideResult(false, FormatOwnedString("[%s] cannot be find", name));
  }

  episode.values.emplace_back(text.data());
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666C00
// FUNCTION: fn_05253
AdventureGuideResult CAdventureGuideMgr::InsertQuestContents(int id, int menu, int minLevel, int maxLevel, bool flag, std::string_view first, std::string_view second) {
  const AdventureGuideContents contents{flag, minLevel, maxLevel, 0, std::string(first), std::string(second)};
  unknown_0044[menu].try_emplace(id, contents);
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00666E70
// FUNCTION: fn_05254
AdventureGuideResult CAdventureGuideMgr::InsertQuestReward(int id, std::string_view text) {
  const auto it = std::find_if(unknown_0044.begin(), unknown_0044.end(), [&](const auto& entry) {
    return entry.second.find(id) != entry.second.end();
  });
  if (it == unknown_0044.end()) {
    return AdventureGuideResult(false, FormatOwnedString("[%d] cannot be find", id));
  }

  it->second[id].values.emplace_back(text.data());
  return AdventureGuideResult(true, "good");
}

// OFFSET: 0x00667060
// FUNCTION: fn_05255
AdventureGuideResult CAdventureGuideMgr::InsertQuestSubMenu(int id, int minLevel, int maxLevel, std::string_view text) {
  const AdventureGuideDetail detail{minLevel, maxLevel, 0, std::string(text)};
  for (auto& [key, intro] : m_intros) {
    if (intro.unknown_0000 == 3) {
      intro.details.try_emplace(id, detail);
      break;
    }
  }

  return AdventureGuideResult(true, "good");
}

// INLINED
static bool CallGuideFunction(CLuaInstance& lua, const std::string& function) {
  lua_State* state = lua.GetState();
  if (!state) {
    return false;
  }

  lua_getglobal(state, function.c_str());
  if (lua_pcall(state, 0, 2, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(state, -1), "CAdventureGuideMgr", MB_OK);
    return false;
  }

  if (lua_type(state, -2) != LUA_TBOOLEAN) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "1th argument must be boolean");
    return false;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "2th argument must be string");
    return false;
  }

  const bool ok = lua_toboolean(state, -2) != 0;
  const std::string message = lua_tostring(state, -1);
  if (!ok) {
    MessageBoxA(nullptr, message.c_str(), "CAdventureGuideMgr", MB_OK);
    return false;
  }

  return true;
}

// OFFSET: 0x00663BD0
// FUNCTION: fn_05239
bool CAdventureGuideMgr::Load() {
  m_lua.SetGlobal("InsertIntro", ::InsertIntro);
  m_lua.SetGlobal("InsertIntroContents", ::InsertIntroContents);
  m_lua.SetGlobal("InsertEpisode", ::InsertEpisode);
  m_lua.SetGlobal("InsertEpisodeSubMenu", ::InsertEpisodeSubMenu);
  m_lua.SetGlobal("InsertEpisodeContents", ::InsertEpisodeContents);
  m_lua.SetGlobal("InsertEpisodeReward", ::InsertEpisodeReward);
  m_lua.SetGlobal("InsertMonster", ::InsertMonster);
  m_lua.SetGlobal("InsertMonsterContents", ::InsertMonsterContents);
  m_lua.SetGlobal("InsertMonsterReward", ::InsertMonsterReward);
  m_lua.SetGlobal("InsertQuest", ::InsertQuest);
  m_lua.SetGlobal("InsertQuestSubMenu", ::InsertQuestSubMenu);
  m_lua.SetGlobal("InsertQuestContents", ::InsertQuestContents);
  m_lua.SetGlobal("InsertQuestReward", ::InsertQuestReward);
  m_lua.SetGlobal("InsertDungeon", ::InsertDungeon);
  m_lua.SetGlobal("InsertDungeonSubMenu", ::InsertDungeonSubMenu);
  m_lua.SetGlobal("InsertDungeonContents", ::InsertDungeonContents);
  m_lua.SetGlobal("InsertDungeonReward", ::InsertDungeonReward);
  m_lua.SetGlobal("InsertWeapon", ::InsertWeapon);
  m_lua.SetGlobal("InsertWeaponSubMenu", ::InsertWeaponSubMenu);
  m_lua.SetGlobal("InsertWeaponContents", ::InsertWeaponContents);
  m_lua.SetGlobal("InsertWeaponObtainGuide", ::InsertWeaponObtainGuide);
  m_lua.SetGlobal("InsertArmor", ::InsertArmor);
  m_lua.SetGlobal("InsertArmorSubMenu", ::InsertArmorSubMenu);
  m_lua.SetGlobal("InsertArmorContents", ::InsertArmorContents);
  m_lua.SetGlobal("InsertArmorObtainGuide", ::InsertArmorObtainGuide);

  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\AdventureGuide_F.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Intro.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Episode.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Monster.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Quest.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Dungeon.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Weapon.lub")) {
    return false;
  }
  if (!m_lua.LoadFile("luafiles514\\Lua Files\\AdventureGuide\\Guide_Armor.lub")) {
    return false;
  }

  if (!CallGuideFunction(m_lua, "main")) {
    return false;
  }

  m_episodes.sort([](const AdventureGuideEpisode& lhs, const AdventureGuideEpisode& rhs) {
    return lhs.order < rhs.order;
  });
  return true;
}
