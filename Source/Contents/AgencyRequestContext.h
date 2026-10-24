#ifndef RAGNAROK_SOURCE_CONTENTS_AGENCYREQUESTCONTEXT_H
#define RAGNAROK_SOURCE_CONTENTS_AGENCYREQUESTCONTEXT_H

#include <cstdint>
#include <string>

class CAsyncWorkMgr;
class LuaScript;
class WindowManagerWindows;

// Original client identity, world, 16-byte token and service configuration.
// Borrows existing owners; does not duplicate registration or pending state.
struct AgencyRequestContext {
  CAsyncWorkMgr& work;
  LuaScript& settings;
  LuaScript& tables;
  WindowManagerWindows& windows;
  const std::uint32_t& accountId;
  const std::uint32_t& characterId;
  const std::string& world;
  const std::string& characterName;
  const char* token16;
  const char* host;
  const char* port;
  const char* overrideKey;
};

#endif
