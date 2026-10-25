#include "Client/ClientOptions.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/UserData/CharacterConfigLoad.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Skills/SkillLevelConfig.h"
#include "Contents/Skills/SkillLevelRegistry.h"
#include "UI/Shortcuts/NormalItemSlotLoad.h"
#include "Utility/Async/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005E6090
// FUNCTION: fn_03927
void CCharacterLinkedUserDataMgr::CompleteLoad(
  AsyncDiagnostic diagnostic, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string response, CharacterConfigLoad& context
) {
  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;
  const std::string document(response.c_str());

  if (!reader.parse(document, root, true)) {
    return;
  }

  const int type = root["Type"].asInt();

  if (type == 4) {
    context.registry.Load();
    context.options.Set(7, 1, 0);
    context.options.Set(8, 1, 0);
    this->m_loaded = true;
  } else if (type == 1) {
    Json::Value& data = root["data"];
    LoadUseSkillLevels(data, context.useLevels);
    LoadHomunculusSkillLevels(data, context.homunculusLevels);

    const Json::Value comparison = data["ItemComparison"];
    context.serviceOptions.Set(264, comparison.isNull() ? true : comparison.asBool());

    const Json::Value sorting = data["SortPrivateTab"];
    context.sortPrivate = sorting.isNull() ? false : sorting.asBool();

    LoadNormalItemSlots(data, context.items, context.saved, context.itemSlotActive, context.windows, context.visibility);
    this->m_loaded = true;
  }

  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_accountName = accountName;
}

// OFFSET: 0x005E6E20
// FUNCTION: fn_03932
void CCharacterLinkedUserDataMgr::Load(
  std::uint32_t accountId, std::uint32_t characterId, std::string accountName, const char* token16, CAsyncWorkMgr& workManager, CharacterConfigLoad& context
) {
  if (accountId == 0 || characterId == 0) {
    return;
  }

  const std::string token(token16, 16);
  const std::string url = this->LoadUrl();
  workManager.LoadCharacter(accountId, characterId, accountName, url, token, context);
}
