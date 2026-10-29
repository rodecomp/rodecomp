#include "Script/LuaFileOperations.h"
#include "File/File.h"
#include "File/FileCreation.h"

extern "C" {
#include <lauxlib.h>
}

#include <cstdio>
#include <io.h>
#include <regex>
#include <string>

// OFFSET: 0x00AFE7D0
// FUNCTION: fn_18339
int LuaChangeDirectoryOfDatas(lua_State* state) {
  std::string name = luaL_checkstring(state, 1);
  std::string sourceDirectory = luaL_checkstring(state, 2);
  std::string targetDirectory = luaL_checkstring(state, 3);
  std::string source = sourceDirectory + "\\" + name;
  std::string target = targetDirectory + "\\" + name;

  if (_access(target.c_str(), 0) != 0 && _access(source.c_str(), 0) == 0) {
    if (_access(target.c_str(), 0) == 0 || CreateDirectoryPath(targetDirectory)) {
      std::rename(source.c_str(), target.c_str());
    }
  }

  return 0;
}

// OFFSET: 0x00AF6580
// FUNCTION: fn_18254
bool LoadLuaResource(lua_State* state, const std::string& name, unsigned char allowLuaFallback) {
  std::string resolved = name;
  CFile file;
  if (!file.Open(name.c_str(), 0, false, false)) {
    if (allowLuaFallback != 1) {
      return false;
    }

    resolved = std::regex_replace(name, std::regex("\\.lub$"), ".lua");
    if (!file.Open(resolved.c_str(), 0, false, false)) {
      return false;
    }
  }

  const bool loaded = luaL_loadbuffer(state, static_cast<const char*>(file.GetData()), file.GetSize(), resolved.c_str()) == 0;
  file.Close();
  return loaded;
}
