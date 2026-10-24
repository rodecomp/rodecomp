#include "Contents/PlayerSkillBundle.h"
#include "Contents/SkillTreeScript.h"

extern "C" {
#include <lauxlib.h>
}

#include <cstdint>

namespace {
  CPlayerSkillBundle& Bundle(lua_State* state) {
    return *static_cast<CPlayerSkillBundle*>(lua_touserdata(state, lua_upvalueindex(1)));
  }

  // OFFSET: 0x00AFEE00
  // FUNCTION: fn_18349
  int AddSkill(lua_State* state) {
    const double group = luaL_checknumber(state, 1);
    const double id = luaL_checknumber(state, 2);
    const char* name = luaL_checkstring(state, 3);
    const int position = static_cast<int>(luaL_checknumber(state, 4));
    const int value = static_cast<int>(luaL_checknumber(state, 5));
    const double last = luaL_checknumber(state, 6);

    if (*name && position != -1 && value != -1) {
      CSkillInfo skill(0);
      skill.available = 1;
      skill.name = name;
      skill.unknown_0024 = position;
      skill.unknown_0028 = value;
      skill.id = static_cast<std::uint32_t>(static_cast<int>(id));
      skill.level = 0;
      skill.unknown_0014 = 0;
      skill.unknown_0018 = 0;
      skill.unknown_001C = 0;
      skill.unknown_0034 = 0;
      skill.unknown_002C = static_cast<int>(last);
      Bundle(state).AddGrouped(static_cast<int>(group), skill);
    }

    // The original returns one existing stack value, without pushing a result.
    return 1;
  }

  // OFFSET: 0x00AFED50
  // FUNCTION: fn_18348
  int AddNeededSkill(lua_State* state) {
    const int id = static_cast<int>(luaL_checknumber(state, 1));
    const int needed = static_cast<int>(luaL_checknumber(state, 2));
    const int level = static_cast<int>(luaL_checknumber(state, 3));

    if (id != -1 && needed != -1 && level != -1) {
      CSkillInfo* skill = Bundle(state).FindGrouped(static_cast<std::uint32_t>(id));
      if (skill && skill->available) {
        skill->entries.push_back({static_cast<std::uint32_t>(needed), static_cast<std::uint32_t>(level)});
      }
    }

    return 1;
  }
}

void RegisterSkillTreeCallbacks(lua_State* state, CPlayerSkillBundle& bundle) {
  lua_pushlightuserdata(state, &bundle);
  lua_pushcclosure(state, AddSkill, 1);
  lua_setglobal(state, "c_AddSkillList");

  lua_pushlightuserdata(state, &bundle);
  lua_pushcclosure(state, AddNeededSkill, 1);
  lua_setglobal(state, "c_AddNeedSkillList");
}
