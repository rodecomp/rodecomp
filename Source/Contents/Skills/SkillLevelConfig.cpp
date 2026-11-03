#include "Client/Session.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"

#include <json/value.h>

#include <cstdint>

// OFFSET: 0x005E6A10
// FUNCTION: fn_03930
void CCharacterLinkedUserDataMgr::LoadUseSkillLevels(Json::Value& data) {
  Json::Value values = data["UseSkillInfo"];
  if (!values.isNull()) {
    for (auto it = values.begin(); it != values.end(); ++it) {
      Json::Value& value = *it;
      if (value.isObject()) {
        int id = value.get("SKID", 0).asInt();
        int level = value.get("SKLv", 0).asInt();
        g_session.m_skillBundle.SetUnknownValue(id, level);
      }
    }
  }
}

// OFFSET: 0x005E63B0
// FUNCTION: fn_03928
void CCharacterLinkedUserDataMgr::LoadHomunculusSkillLevels(Json::Value& data) {
  Json::Value values = data["HomunSkillInfo"];
  if (!values.isNull()) {
    for (auto it = values.begin(); it != values.end(); ++it) {
      Json::Value& value = *it;
      if (value.isObject()) {
        int id = value.get("HomunSKID", 0).asInt();
        int level = value.get("HomunSKLv", 0).asInt();

        g_session.SetHomunSkillLevel(id, level);
      }
    }
  }
}
