#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_AGENCYREQUESTCONTEXT_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_AGENCYREQUESTCONTEXT_H

#include <cstdint>
#include <string>

class CAsyncWorkMgr;
class LuaScript;
class WindowManagerWindows;

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
