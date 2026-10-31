#ifndef RAGNAROK_SOURCE_SCRIPT_LUASCRIPT_H
#define RAGNAROK_SOURCE_SCRIPT_LUASCRIPT_H

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <vector>

struct lua_State;

class LuaScript {
public:
  LuaScript();
  ~LuaScript();
  void Open();
  bool Call(std::string function, const char* signature, ...);
  bool CallTable(std::string table, std::string function, const char* signature, ...);
  bool Execute(const void* bytes, std::size_t size);
  bool Load(const char* name, bool resourcePath, bool looseOnly);
  int GlobalInteger(const char* name);
  std::string GlobalString(const char* name);
  bool HasTable(const char* name);
  std::list<int> TableKeys(const char* name, int unused);
  int TableInteger(std::string table, int keyType, std::intptr_t key);
  void Close();
  void Register(std::string name, int (*function)(lua_State*));
  // INLINED
  lua_State* GetState() const {
    return this->m_state;
  }

protected:
  /* 0000 */ lua_State* m_state;

private:
  void Error(const char* caption, const char* format, ...);
  bool TableField(const char* signature, ...);
  void Register();
};

extern LuaScript* g_luaScript;
extern std::map<std::string, std::vector<std::string>> g_kafraServiceMapList;

struct MiniMapPosition;
extern std::map<std::string, MiniMapPosition> g_exceptionMiniMapPosList;

#endif
