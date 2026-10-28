#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Script/LuaScript.h"

#include <string>

// OFFSET: 0x00BAC770
// FUNCTION: fn_19756
std::string EquipmentResourceMgr::GetRobeSprName(int robe, int) {
  const char* name = nullptr;
  if (g_session.m_hotkeyScript->Call("ReqRobSprName", "d>s", robe, &name)) {
    return name;
  }
  return name;
}
