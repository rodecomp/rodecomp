#include "Contents/UserData/AccountService.h"
#include "Contents/Party/AdventurerAgencyMgr.h"

#include <json/value.h>
#include <new>

std::uint32_t CAdventurerAgencyMgr::Id(double value) {
  if (!(value < 4294967296.0) || value <= -1.0) {
    return 0xFFFFFFFFu;
  }

  return static_cast<std::uint32_t>(value);
}

// STATIC: 0x01389848
// GLOBAL: gl_24352
CAdventurerAgencyMgr* g_adventurerAgencyMgr;

// OFFSET: 0x005CA710
// FUNCTION: fn_03648
CAdventurerAgencyMgr::CAdventurerAgencyMgr() : enabled(true), registered(false), unknown_0008(0), pending(0) {
}

// OFFSET: 0x005CA880
// FUNCTION: fn_03650
// OFFSET: 0x005CA8D0
// FUNCTION: fn_03651
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

// OFFSET: 0x005CAA50
// FUNCTION: fn_03655
std::string
CAdventurerAgencyMgr::ServiceUrl(int operation, LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) const {
  std::string url = ResolveAccountServiceBase(settings, tables, host, port, overrideKey);

  if (url.empty()) {
    return url;
  }

  switch (operation) {
  case 0:
  case 2:
    url += "/party/add";
    break;
  case 1:
    url += "/party/list";
    break;
  case 3:
    url += "/party/search";
    break;
  case 4:
    url += "/party/del";
    break;
  case 5:
    url += "/party/get";
    break;
  case 6:
    url += "/party/change";
    break;
  case 7:
    url += "/party/info";
    break;
  default:
    break;
  }

  return url;
}

// OFFSET: 0x005CAC00
// FUNCTION: fn_03656
void CAdventurerAgencyMgr::LoadList(const Json::Value& value) {
  this->entries.clear();

  for (auto entry = value.begin(); entry != value.end(); ++entry) {
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
