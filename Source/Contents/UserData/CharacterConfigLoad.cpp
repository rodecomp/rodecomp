#include "Client/Session.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Skills/SkillLevelRegistry.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/value.h>

// OFFSET: 0x005E6090
// FUNCTION: fn_03927
void CCharacterLinkedUserDataMgr::CompleteLoad(
  AsyncDiagnostic diagnostic, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string response
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
    LoadSkillUseLevelRegistry();
    g_session.SetOption(7, 1, 0);
    g_session.SetOption(8, 1, 0);
    this->m_loaded = true;
  } else if (type == 1) {
    this->LoadUseSkillLevels(root["data"]);
    this->LoadHomunculusSkillLevels(root["data"]);

    const Json::Value comparison = root["data"]["ItemComparison"];
    g_serviceOptions.Set(264, comparison.isNull() ? true : comparison.asBool());

    const Json::Value sorting = root["data"]["SortPrivateTab"];
    g_sortPrivateTab = sorting.isNull() ? false : sorting.asBool();

    this->LoadNormalItemSlots(root["data"]);
    this->m_loaded = true;
  }

  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_accountName = accountName;
}

// OFFSET: 0x005E6E20
// FUNCTION: fn_03932
void CCharacterLinkedUserDataMgr::Load(std::uint32_t accountId, std::uint32_t characterId, std::string accountName) {
  if (accountId == 0 || characterId == 0) {
    return;
  }

  g_asyncWorkMgr->LoadCharacter(
    accountId, characterId, accountName, this->LoadUrl(), std::string(reinterpret_cast<const char*>(&g_session.unknown_807C[0x08]), 16)
  );
}
