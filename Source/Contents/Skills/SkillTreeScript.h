#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLTREESCRIPT_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLTREESCRIPT_H

struct lua_State;
class CPlayerSkillBundle;

int AddSkill(lua_State* state);
int AddNeededSkill(lua_State* state);
void RegisterSkillTreeCallbacks(lua_State* state, CPlayerSkillBundle& bundle);

#endif
