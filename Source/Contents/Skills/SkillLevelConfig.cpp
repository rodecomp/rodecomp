#include "Client/Session.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"

#include <json/value.h>

#include <cstdint>

// OFFSET: 0x005E6A10
// FUNCTION: fn_03930
void CCharacterLinkedUserDataMgr::LoadUseSkillLevels(Json::Value& data) {
  Json::Value values = data["UseSkillInfo"];
  if (!values.isNull()) {
    for (auto it = values.begin(); it != values.end(); it++) {
      if ((*it).isObject()) {
        int id = (*it).get("SKID", 0).asInt();
        int level = (*it).get("SKLv", 0).asInt();
        g_session.m_skillBundle.SetSkillValue(id, level);
      }
    }
  }
}

// OFFSET: 0x005E63B0
// FUNCTION: fn_03928
void CCharacterLinkedUserDataMgr::LoadHomunculusSkillLevels(Json::Value& data) {
  Json::Value values = data["HomunSkillInfo"];
  if (!values.isNull()) {
    for (auto it = values.begin(); it != values.end(); it++) {
      if ((*it).isObject()) {
        int id = (*it).get("HomunSKID", 0).asInt();
        int level = (*it).get("HomunSKLv", 0).asInt();

        g_session.SetHomunSkillLevel(id, level);
      }
    }
  }
}
