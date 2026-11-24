#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Script/LuaScript.h"

#include <json/value.h>
#include <new>

// INLINED
std::uint32_t CAdventurerAgencyMgr::Id(double value) {
  return static_cast<std::uint32_t>(value);
}

// STATIC: 0x01389848
// GLOBAL: gl_24352
CAdventurerAgencyMgr* g_adventurerAgencyMgr;

// OFFSET: 0x005CA660
// FUNCTION: fn_03647
CAdvenPartyInfo::CAdvenPartyInfo()
    : tanker(false), healer(false), dealer(false), assist(false), minimumLevel(0), maximumLevel(999), type(0), accountId(0), characterId(0) {
}

// OFFSET: 0x005CA710
// FUNCTION: fn_03648
CAdventurerAgencyMgr::CAdventurerAgencyMgr() : registered(false), masterAccountId(0), pending(0) {
  this->enabled = true;
}

// OFFSET: 0x005CA880
// FUNCTION: fn_03650
CAdventurerAgencyMgr::~CAdventurerAgencyMgr() = default;

// OFFSET: 0x005CA940
// FUNCTION: fn_03652
bool CAdventurerAgencyMgr::Initialize() {
  if (g_adventurerAgencyMgr) {
    return false;
  }

  g_adventurerAgencyMgr = new (std::nothrow) CAdventurerAgencyMgr;

  if (!g_adventurerAgencyMgr) {
    return false;
  }

  if (!g_adventurerAgencyMgr->enabled) {
    delete g_adventurerAgencyMgr;
    g_adventurerAgencyMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x005CAA20
// FUNCTION: fn_03653
void CAdventurerAgencyMgr::Shutdown() {
  if (g_adventurerAgencyMgr) {
    delete g_adventurerAgencyMgr;
    g_adventurerAgencyMgr = nullptr;
  }
}

// OFFSET: 0x005CAA40
// FUNCTION: fn_03654
const std::vector<CAdvenPartyInfo>& CAdventurerAgencyMgr::Entries() const {
  return this->entries;
}

// OFFSET: 0x005CAC00
// FUNCTION: fn_03656
void CAdventurerAgencyMgr::LoadList(Json::Value& value) {
  this->entries.clear();

  for (auto entry = value.begin(); entry != value.end(); entry++) {
    CAdvenPartyInfo info;
    info.characterName = (*entry)["CharName"].asString();
    info.accountId = Id((*entry).get("AID", 0).asDouble());
    info.characterId = Id((*entry).get("GID", 0).asDouble());
    info.assist = (*entry).get("Assist", 1).asInt() != 0;
    info.healer = (*entry).get("Healer", 1).asInt() != 0;
    info.dealer = (*entry).get("Dealer", 1).asInt() != 0;
    info.tanker = (*entry).get("Tanker", 1).asInt() != 0;
    info.minimumLevel = (*entry).get("MinLV", 0).asInt();
    info.maximumLevel = (*entry).get("MaxLV", 999).asInt();
    info.type = (*entry).get("Type", 1).asInt();
    info.memo = (*entry).get("Memo", "").asString();
    this->entries.push_back(info);
  }
}
