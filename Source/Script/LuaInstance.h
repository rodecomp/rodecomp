#ifndef RAGNAROK_SOURCE_SCRIPT_LUAINSTANCE_H
#define RAGNAROK_SOURCE_SCRIPT_LUAINSTANCE_H

#if __has_include(<lua.h>)
extern "C" {
#include <lua.h>
}
#else
struct lua_State;
typedef int (*lua_CFunction)(lua_State* state);
#endif

#include <map>
#include <string>
#include <vector>

class CLuaInstance {
public:
  CLuaInstance();
  virtual ~CLuaInstance();
  bool Execute(const std::string& name);
  bool LoadFile(const std::string& path);
  bool LoadFile(const std::string& path, bool showError);
  void SetGlobal(const std::string& name, bool value);
  void SetGlobal(const std::string& name, lua_CFunction function);
  void SetGlobal(const std::string& name, double value);
  int ReadStrings(std::vector<std::string>& values, const char* global);
  int GetFieldInt(int& value, const char* name);
  int GetFieldUIntArray(std::vector<unsigned int>& values, const char* name);
  int GetFieldString(std::string& value, const char* name);
  int GetFieldStringArray(std::vector<std::string>& values, const char* name);
  int GetFieldUIntMap(std::map<unsigned int, unsigned int>& values, const char* name);
  int GetFieldStringMap(std::map<unsigned int, std::string>& values, const char* name);
  int GetInt(int& value, int index);
  lua_State* GetState() const { return this->m_state; }

protected:
  /* 0004 */ lua_State* m_state;

private:
  static int LuaMessageBox(lua_State* state);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CLuaInstance) == 8);
#endif

#endif
