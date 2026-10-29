#ifndef RAGNAROK_SOURCE_SCRIPT_LUAFILEOPERATIONS_H
#define RAGNAROK_SOURCE_SCRIPT_LUAFILEOPERATIONS_H

#include <string>

struct lua_State;

int LuaChangeDirectoryOfDatas(lua_State* state);
bool LoadLuaResource(lua_State* state, const std::string& name, unsigned char allowLuaFallback);

#endif
