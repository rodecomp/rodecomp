#ifndef RAGNAROK_SOURCE_CONTENTS_ACCOUNTSERVICE_H
#define RAGNAROK_SOURCE_CONTENTS_ACCOUNTSERVICE_H

#include <string>

class LuaScript;

// Explicit state: settings script, table script, host/port/override pointers.
std::string ResolveAccountServiceBase(
  LuaScript& settings,
  LuaScript& tables,
  const char* host,
  const char* port,
  const char* overrideKey
);

#endif
