#include "Client/Session/Session.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Session/Session.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/Configuration/ServiceOptions.h"

#include <json/value.h>
#include <json/writer.h>

// OFFSET: 0x00DE0E40
// FUNCTION: fn_23474
int CSession::UseLevel(int id) const {
  const auto key = static_cast<std::uint32_t>(id);

  if (key - 1u <= 0x1F3D) {
    return this->m_skillBundle.skillValues[key];
  }

  if (id > 10000) {
    return this->extendedSkillLevels[key - 10000u];
  }

  if (this->IsHomunculusSkill(key)) {
    const int index = id - 8000;

    if (index > -1) {
      return this->m_homunSkillLevels[index];
    }
  }

  return 0;
}

// INLINED
inline void CCharacterLinkedUserDataMgr::SaveUseSkillLevel(Json::Value& data) {
  if (static_cast<int>(g_session.m_options.Get(7)) != 0) {
    Json::Value values;

    struct UseSkillLevelAppender {
      Json::Value* values;

      // OFFSET: 0x005E7AE0
      // FUNCTION: fn_03940
      bool operator()(CSkillInfo& skill) const {
        const int level = g_session.UseLevel(skill.id);
        if (level != 0) {
          Json::Value value;
          value["SKID"] = skill.id;
          value["SKLv"] = level;
          this->values->append(value);
        }

        return true;
      }
    };

    g_session.m_skillBundle.ForEachGrouped(UseSkillLevelAppender{&values});
    data["UseSkillInfo"] = values;
  }
}

// INLINED
inline void CCharacterLinkedUserDataMgr::SaveHomunSkillLevel(Json::Value& data) {
  if (static_cast<int>(g_session.m_options.Get(8)) != 0) {
    Json::Value values;

    for (auto skillIt = g_session.m_homunSkills.begin(); skillIt != g_session.m_homunSkills.end(); ++skillIt) {
      const CSkillInfo& skill = *skillIt;
      const int level = g_session.HomunSkillLevel(skill.id);

      if (level != 0) {
        Json::Value value;
        value["HomunSKID"] = skill.id;
        value["HomunSKLv"] = level;
        values.append(value);
      }
    }

    data["HomunSkillInfo"] = values;
  }
}

// OFFSET: 0x005E6EF0
// FUNCTION: fn_03933
void CCharacterLinkedUserDataMgr::Save() {
  if (!this->m_loaded) {
    return;
  }

  Json::Value root;
  Json::Value data;

  this->SaveUseSkillLevel(data);
  this->SaveHomunSkillLevel(data);

  data["ItemComparison"] = g_serviceOptions.Get(TT_ITEM_COMPARE_ON_OFF);
  data["SortPrivateTab"] = g_session.m_sortPrivateTab;
  this->ExportNormalItemSlots(data);

  if (data.isNull() != true) {
    root["data"] = data;
    Json::StyledWriter writer;
    const std::string response = writer.write(root);
    CAsyncWorkMgr* mgr = g_asyncWorkMgr;
    mgr->SaveCharacter(this->m_accountId, this->m_characterId, this->m_accountName, this->SaveUrl(),
                       std::string(g_session.m_webAuthToken, 16), response);

    ++this->m_pendingSaves;
  }
}

// OFFSET: 0x00DDD0D0
// FUNCTION: fn_23379
int CSession::HomunSkillLevel(int skillId) const {
  skillId -= 8000;
  if (skillId > -1) {
    return this->m_homunSkillLevels[skillId];
  }

  return 0;
}
