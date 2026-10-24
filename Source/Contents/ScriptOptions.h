#ifndef RAGNAROK_SOURCE_CONTENTS_SCRIPTOPTIONS_H
#define RAGNAROK_SOURCE_CONTENTS_SCRIPTOPTIONS_H

#include <string>
#include <windows.h>

class LuaScript;

// Reads through the original settings callback l_GetOptionValue. The script
// state is borrowed from the application owner, not replaced by a second map.
struct ScriptOptions {
  LuaScript& script;

  int Integer(const char* key, int fallback);
  std::string String(const char* key, const char* fallback);
  GUID Guid(const char* key, const GUID& fallback);
};

#endif
