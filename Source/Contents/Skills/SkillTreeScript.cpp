#include "Client/Session.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/Skills/SkillTreeScript.h"

extern "C" {
#include <lauxlib.h>
}

#include <cstdint>
#include <cstring>

template std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, std::list<CSkillInfo>>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, std::list<CSkillInfo>>>>, std::_Iterator_base0>::operator++() noexcept;

// OFFSET: 0x00AFEE00
// FUNCTION: fn_18349
int AddSkill(lua_State* state) {
  CSkillInfo skill(0);
  const double group = luaL_checknumber(state, 1);
  const double id = luaL_checknumber(state, 2);
  const char* name = luaL_checkstring(state, 3);
  const int position = static_cast<int>(luaL_checknumber(state, 4));
  const int value = static_cast<int>(luaL_checknumber(state, 5));
  const double last = luaL_checknumber(state, 6);

  if (std::strcmp(name, "") != 0 && position != -1 && value != -1) {
    skill.level = 0;
    skill.spCost = 0;
    skill.range = 0;
    skill.upgradable = 0;
    skill.available = 1;
    skill.name = name;
    skill.treePosition = position;
    skill.maxLevel = value;
    skill.id = static_cast<std::uint32_t>(static_cast<int>(id));
    skill.apCost = 0;
    skill.canPlanLevel = static_cast<int>(last);
    g_session.m_skillBundle.AddGrouped(static_cast<int>(group), skill);
  }

  return 1;
}

// OFFSET: 0x00AFED50
// FUNCTION: fn_18348
int AddNeededSkill(lua_State* state) {
  const int id = static_cast<int>(luaL_checknumber(state, 1));
  const int needed = static_cast<int>(luaL_checknumber(state, 2));
  const int level = static_cast<int>(luaL_checknumber(state, 3));

  if (id != -1 && needed != -1 && level != -1) {
    CSkillInfo* skill = g_session.FindGroupedSkill(static_cast<std::uint32_t>(id));
    if (skill && skill->available) {
      skill->entries.push_back({static_cast<std::uint32_t>(needed), static_cast<std::uint32_t>(level)});
    }
  }

  return 1;
}

// INLINED
void RegisterSkillTreeCallbacks(lua_State* state, CPlayerSkillBundle& bundle) {
  lua_pushlightuserdata(state, &bundle);
  lua_pushcclosure(state, AddSkill, 1);
  lua_setglobal(state, "c_AddSkillList");

  lua_pushlightuserdata(state, &bundle);
  lua_pushcclosure(state, AddNeededSkill, 1);
  lua_setglobal(state, "c_AddNeedSkillList");
}
