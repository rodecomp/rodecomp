#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLTREESCRIPT_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLTREESCRIPT_H

struct lua_State;
class CPlayerSkillBundle;

// Source-side binding for the two recovered callbacks, not the full original
// LuaScript initializer. The bundle must outlive this state's callbacks.
// Skill names remain borrowed from Lua strings, as in the reference.
// Registers c_AddSkillList and c_AddNeedSkillList only. Both return the existing
// top Lua argument (one result, no push), preserve numeric truncation and -1
// filters, and associate prerequisites with the first available grouped match.
void RegisterSkillTreeCallbacks(lua_State* state, CPlayerSkillBundle& bundle);

#endif
