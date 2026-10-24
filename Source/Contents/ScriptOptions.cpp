#include "Contents/ScriptOptions.h"
#include "Script/LuaScript.h"
#include "Utility/GuidParse.h"

#include <cstring>

// OFFSET: 0x00E0A890
// FUNCTION: fn_23800
int ScriptOptions::Integer(const char* key, int fallback) {
  int value = -1;
  this->script.Call("l_GetOptionValue", "s>d", key, &value);
  return value == -1 ? fallback : value;
}

// OFFSET: 0x00E0A950
// FUNCTION: fn_23801
std::string ScriptOptions::String(const char* key, const char* fallback) {
  const char* value = nullptr;
  this->script.Call("l_GetOptionValue", "s>s", key, &value);
  return !value || std::strcmp(value, "-1") == 0 ? fallback : value;
}

// OFFSET: 0x00E0A760
// FUNCTION: fn_23799
GUID ScriptOptions::Guid(const char* key, const GUID& fallback) {
  const char* value = nullptr;
  this->script.Call("l_GetOptionValue", "s>s", key, &value);

  if (value && std::strcmp(value, "-1") != 0) {
    GUID parsed;

    if (ParseGuid(value, std::strlen(value), parsed, true) == 1) {
      return parsed;
    }
  }

  return fallback;
}
