#ifndef RAGNAROK_SOURCE_SCRIPT_LUAINSTANCE_H
#define RAGNAROK_SOURCE_SCRIPT_LUAINSTANCE_H

extern "C" {
#include <lua.h>
}

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
