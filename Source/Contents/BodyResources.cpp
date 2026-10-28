#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"

#include <string>

// OFFSET: 0x00BABEF0
// FUNCTION: fn_19754
std::string EquipmentResourceMgr::GetBodyRacePrefix(int job) {
  if (g_session.IsReducedSkill(job) == true) {
    return "도람족";
  }
  return "인간족";
}
