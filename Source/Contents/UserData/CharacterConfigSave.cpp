#include "Client/ClientOptions.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/UserData/CharacterConfigSave.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/Configuration/ServiceOptions.h"

#include <json/value.h>
#include <json/writer.h>

// OFFSET: 0x00DE0E40
// FUNCTION: fn_23474
int CharacterConfigSave::UseLevel(int id) const {
  const auto key = static_cast<std::uint32_t>(id);

  if (key - 1u <= 0x1F3D) {
    return this->useLevels[key];
  }

  if (id > 10000) {
    return this->additionalLevels[key - 10000u];
  }

  if (key - 8000u <= 60) {
    return this->homunculusLevels[key - 8000u];
  }

  return 0;
}

// OFFSET: 0x005E6EF0
// FUNCTION: fn_03933
void CCharacterLinkedUserDataMgr::Save(CharacterConfigSave& context, const char* token16, CAsyncWorkMgr& workManager) {
  if (!this->m_loaded) {
    return;
  }

  Json::Value root;
  Json::Value data;

  if (context.options.Get(7) != 0) {
    Json::Value values;

    context.skills.ForEachGrouped([&](CSkillInfo* skill) {
      const int id = static_cast<int>(skill->id);
      const int level = context.UseLevel(id);

      if (level != 0) {
        Json::Value value;
        value["SKID"] = id;
        value["SKLv"] = level;
        values.append(value);
      }

      return true;
    });
    data["UseSkillInfo"] = values;
  }

  if (context.options.Get(8) != 0) {
    Json::Value values;

    for (const CSkillInfo& skill : context.homunculusSkills) {
      const int id = static_cast<int>(skill.id);
      const auto index = skill.id - 8000u;
      const int level = static_cast<int>(index) < 0 ? 0 : context.homunculusLevels[index];

      if (level != 0) {
        Json::Value value;
        value["HomunSKID"] = id;
        value["HomunSKLv"] = level;
        values.append(value);
      }
    }

    data["HomunSkillInfo"] = values;
  }

  data["ItemComparison"] = context.serviceOptions.Get(264) != 0;
  data["SortPrivateTab"] = context.sortPrivate;
  ExportNormalItemSlots(data, context.items, context.saved, context.itemSlotActive);

  if (!data.isNull()) {
    root["data"] = data;
    Json::StyledWriter writer;
    const std::string response = writer.write(root);
    const std::string token(token16, 16);
    const std::string url = this->SaveUrl();
    workManager.SaveCharacter(this->m_accountId, this->m_characterId, this->m_accountName, url, token, response);

    ++this->m_pendingSaves;
  }
}
