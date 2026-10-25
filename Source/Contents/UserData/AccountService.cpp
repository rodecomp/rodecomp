#include "Contents/UserData/AccountService.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00DDBB60
// FUNCTION: fn_23360
std::string ResolveAccountServiceBase(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) {
  std::string address = settings.GlobalString("AssistAddr");
  if (!address.empty()) {
    return address;
  }

  std::string key = std::string(host) + ":" + port;
  if (overrideKey && *overrideKey) {
    key = overrideKey;
  }

  const char* resolved = nullptr;
  if (tables.Call("GetTableStringValueForC", "ss>s", "AssistAddrTbl", key.c_str(), &resolved) && resolved && *resolved) {
    return resolved;
  }

  return {};
}
