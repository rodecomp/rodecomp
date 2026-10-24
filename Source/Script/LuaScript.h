#ifndef RAGNAROK_SOURCE_SCRIPT_LUASCRIPT_H
#define RAGNAROK_SOURCE_SCRIPT_LUASCRIPT_H

extern "C" {
#include <lua.h>
}

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <list>
#include <string>

// Reconstructed name for the original one-pointer script wrapper. This does not
// claim the unreconstructed application's native-registration initializer.
class LuaScript {
public:
  LuaScript();
  ~LuaScript();
  bool Call(std::string function, const char* signature, ...);
  bool CallTable(std::string table, std::string function, const char* signature, ...);
  bool Execute(const void* bytes, std::size_t size);
  int GlobalInteger(const char* name);
  std::string GlobalString(const char* name);
  bool HasTable(const char* name);
  std::list<int> TableKeys(const char* name, int unused);
  int TableInteger(std::string table, int keyType, std::intptr_t key);
  void Close();

protected:
  /* 0000 */ lua_State* m_state;

private:
  void Error(const char* caption, const char* format, ...);
};

#endif
