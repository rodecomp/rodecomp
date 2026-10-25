#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SCRIPTOPTIONS_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SCRIPTOPTIONS_H

#include <string>
#include <windows.h>

class LuaScript;

struct ScriptOptions {
  LuaScript& script;

  int Integer(const char* key, int fallback);
  std::string String(const char* key, const char* fallback);
  GUID Guid(const char* key, const GUID& fallback);
};

#endif
