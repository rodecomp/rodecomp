#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00DDBB60
// FUNCTION: fn_23360
std::string CSession::ResolveAccountServiceBase() {
  std::string address = this->m_hotkeyScript->GlobalString("AssistAddr");
  if (!address.empty()) {
    return address;
  }

  std::string key = std::string(g_serviceHost) + ":" + g_servicePort;
  if (g_serviceOverrideKey && *g_serviceOverrideKey) {
    key = g_serviceOverrideKey;
  }

  const char* resolved = nullptr;
  if (this->m_hotkeyScript->Call("GetTableStringValueForC", "ss>s", "AssistAddrTbl", key.c_str(), &resolved) && resolved && *resolved) {
    return resolved;
  }

  return {};
}
