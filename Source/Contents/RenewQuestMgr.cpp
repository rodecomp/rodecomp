#include "Contents/RenewQuestMgr.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <new>
#include <utility>

#pragma pack(push, 1)
struct RenewQuestOngoingRecord {
  /* 0000 */ unsigned int questId;
  /* 0004 */ unsigned short state;
  /* 0006 */ short huntCount;
  /* 0008 */ short year;
  /* 000A */ short month;
  /* 000C */ short day;
  /* 000E */ short hour;
  /* 0010 */ unsigned __int64 minute;
};

struct RenewQuestHuntRecord {
  /* 0000 */ int huntId;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ short unknown_000C;
  /* 000E */ short unknown_000E;
  /* 0010 */ short unknown_0010;
  /* 0012 */ short unknown_0012;
  /* 0014 */ char name[0x33];
};

struct PACKET_CZ_QUEST_STATUS_REQ {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t packetLength;
};

struct PACKET_CZ_QUEST_STATUS_REQ_BUFFER {
  /* 0000 */ PACKET_CZ_QUEST_STATUS_REQ header;
  /* 0004 */ unsigned int questIds[511];
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestOngoingRecord) == 0x18);
static_assert(sizeof(RenewQuestHuntRecord) == 0x47);
static_assert(sizeof(PACKET_CZ_QUEST_STATUS_REQ_BUFFER) == 0x800);
#endif

// GLOBAL: gl_24837
CRenewQuestMgr* g_renewQuestMgr;

// OFFSET: 0x00675990
// FUNCTION: fn_05414
CRenewQuestMgr::CRenewQuestMgr() : m_initialized(false) {
  this->m_initialized = this->Clear();
  this->m_parameter = 24;
}

// OFFSET: 0x00677C00
// FUNCTION: fn_05433
bool CRenewQuestMgr::Clear() {
  this->m_ongoingQuests.clear();
  this->m_recommendedQuests.clear();
  return true;
}

// OFFSET: 0x00677C50
// FUNCTION: fn_05434
bool CRenewQuestMgr::Create() {
  if (g_renewQuestMgr == nullptr) {
    g_renewQuestMgr = new (std::nothrow) CRenewQuestMgr();
    if (g_renewQuestMgr != nullptr) {
      if (g_renewQuestMgr->m_initialized) {
        return true;
      }

      delete g_renewQuestMgr;
      g_renewQuestMgr = nullptr;
    }
  }

  return false;
}

// OFFSET: 0x00677D00
// FUNCTION: fn_05436
void CRenewQuestMgr::Destroy() {
  if (g_renewQuestMgr != nullptr) {
    delete g_renewQuestMgr;
    g_renewQuestMgr = nullptr;
  }
}

// OFFSET: 0x00677D20
// FUNCTION: fn_05437
void CRenewQuestMgr::RemoveStatusRequest(unsigned int id) {
  const auto found = this->m_statusRequests.find(id);
  if (found != this->m_statusRequests.end()) {
    this->m_statusRequests.erase(found);
  }
}

// OFFSET: 0x00677D80
// FUNCTION: fn_05438
std::string RenewQuestOngoingInfo::GetDateString() const {
  if (this->m_year < 1971) {
    return std::string();
  }

  return std::format("{0} [{1:04}/{2:02}/{3:02} {4:02}:{5:02}]", LookupMessage(2195), this->m_year, this->m_month,
                     this->m_day, this->m_hour, this->m_minute);
}

// OFFSET: 0x006779A0
// FUNCTION: fn_05432
void CRenewQuestMgr::AddIconTooltip(std::string_view icon, std::string_view tooltip) {
  const std::string text(tooltip);
  this->m_iconTooltips[std::string(icon)] = text;
}

// OFFSET: 0x00677F40
// FUNCTION: fn_05439
std::string CRenewQuestMgr::GetFilterName(int filter) {
  const auto found = this->m_filters.find(filter);
  return found == this->m_filters.end() ? std::string() : found->second.m_name;
}

// OFFSET: 0x006792A0
// FUNCTION: fn_05441
std::string CRenewQuestMgr::GetIconTooltip(std::string_view icon) {
  const auto found = this->m_iconTooltips.find(std::string(icon));
  return found == this->m_iconTooltips.end() ? std::string("QUEST") : found->second;
}

// OFFSET: 0x00679A00
// FUNCTION: fn_05444
RenewQuestOngoingInfo* CRenewQuestMgr::FindOngoingQuest(unsigned int id) {
  const auto found = this->m_ongoingQuests.find(id);
  if (found != this->m_ongoingQuests.end()) {
    return &found->second;
  }

  return nullptr;
}

// OFFSET: 0x00679BD0
// FUNCTION: fn_05446
RenewQuestRecommendedInfo* CRenewQuestMgr::FindRecommendedQuest(unsigned int id) {
  const auto found = this->m_recommendedQuests.find(id);
  if (found != this->m_recommendedQuests.end()) {
    return &found->second;
  }

  return nullptr;
}

// OFFSET: 0x00679C20
// FUNCTION: fn_05447
void CRenewQuestMgr::WriteOngoingQuests(char* buffer) {
  int offset = 4;
  *reinterpret_cast<unsigned int*>(buffer) = this->m_ongoingQuests.size();

  for (auto it = this->m_ongoingQuests.begin(); it != this->m_ongoingQuests.end(); ++it) {
    const RenewQuestOngoingInfo& info = it->second;
    const char huntCount = info.m_huntCount;

    RenewQuestOngoingRecord* record = reinterpret_cast<RenewQuestOngoingRecord*>(buffer + offset);
    record->questId = it->first;
    record->state = info.m_state;
    record->huntCount = huntCount;
    record->year = info.m_year;
    record->month = info.m_month;
    record->day = info.m_day;
    record->hour = info.m_hour;
    record->minute = info.m_minute;
    offset += sizeof(RenewQuestOngoingRecord);

    for (int index = 0; index < huntCount; ++index) {
      RenewQuestHuntRecord hunt = {};
      hunt.huntId = static_cast<int>(info.m_huntId[index]);
      hunt.unknown_0004 = info.m_huntUnknown01A0[index];
      hunt.unknown_0008 = info.m_huntUnknown00D4[index];
      hunt.unknown_000C = info.m_huntUnknown01AC[index];
      hunt.unknown_000E = info.m_huntUnknown01B2[index];
      hunt.unknown_0010 = info.m_huntUnknown00E0[index];
      hunt.unknown_0012 = info.m_huntUnknown0180[index];
      strncpy_s(hunt.name, sizeof(hunt.name), info.m_huntName[index], _TRUNCATE);
      *reinterpret_cast<RenewQuestHuntRecord*>(buffer + offset) = hunt;
      offset += sizeof(RenewQuestHuntRecord);
    }
  }
}

// OFFSET: 0x00679E00
// FUNCTION: fn_05448
int CRenewQuestMgr::GetOngoingQuestsSize() {
  int size = 4;
  for (auto it = this->m_ongoingQuests.begin(); it != this->m_ongoingQuests.end(); ++it) {
    size += it->second.m_huntCount * static_cast<int>(sizeof(RenewQuestHuntRecord));
  }

  return size + this->m_ongoingQuests.size() * sizeof(RenewQuestOngoingRecord);
}

// OFFSET: 0x0067A500
// FUNCTION: fn_05450
void CRenewQuestMgr::AddNewQuest(unsigned int id) {
  this->m_newQuests.insert(id);
  const auto found = this->m_statusRequests.find(id);
  if (found != this->m_statusRequests.end()) {
    this->m_statusRequests.erase(found);
  }
}

// OFFSET: 0x0067AEB0
// FUNCTION: fn_05453
bool CRenewQuestMgr::IsNewQuest(unsigned int id) {
  return this->m_newQuests.find(id) != this->m_newQuests.end();
}

// OFFSET: 0x0067AF00
// FUNCTION: fn_05454
bool CRenewQuestMgr::IsInFilter(int filter, std::string_view icon) {
  switch (filter) {
  case 0:
    return true;
  case 1:
    for (auto it = this->m_filters.begin(); it != this->m_filters.end(); ++it) {
      const std::list<std::string>& icons = it->second.m_icons;
      if (std::find(icons.begin(), icons.end(), icon) != icons.end()) {
        return false;
      }
    }
    return true;
  default: {
    const auto found = this->m_filters.find(filter);
    if (found == this->m_filters.end()) {
      return false;
    }

    const std::list<std::string>& icons = found->second.m_icons;
    return std::find(icons.begin(), icons.end(), icon) != icons.end();
  }
  }
}

// OFFSET: 0x0067B030
// FUNCTION: fn_05455
int CRenewQuestMgr::L_AddQuestFilterInfo(lua_State* state) {
  const int id = static_cast<int>(lua_tonumber(state, -3));
  const std::string name = lua_tostring(state, -2);
  std::list<std::string> icons;
  if (lua_type(state, -1) != LUA_TNIL) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      icons.push_back(lua_tostring(state, -1));
      lua_pop(state, 1);
    }
  }

  g_renewQuestMgr->m_filters.try_emplace(id, RenewQuestFilter{name, std::move(icons)});
  return 0;
}

// OFFSET: 0x0067B460
// FUNCTION: fn_05456
int CRenewQuestMgr::L_AddQuestIconTooltip(lua_State* state) {
  const std::string icon = lua_tostring(state, -2);
  const std::string tooltip = lua_tostring(state, -1);
  g_renewQuestMgr->AddIconTooltip(icon, tooltip);
  return 0;
}

// OFFSET: 0x0067B6E0
// FUNCTION: fn_05461
void CRenewQuestMgr::SendQuestStatusRequest() {
  PACKET_CZ_QUEST_STATUS_REQ_BUFFER packet;
  const unsigned int length = this->m_statusRequests.size() * sizeof(unsigned int) + sizeof(PACKET_CZ_QUEST_STATUS_REQ);
  std::memset(packet.questIds, 0, sizeof(packet.questIds));
  if (length > sizeof(packet)) {
    return;
  }

  PACKET_CZ_QUEST_STATUS_REQ header;
  header.packetType = ID_CZ_QUEST_STATUS_REQ;
  header.packetLength = static_cast<std::int16_t>(length);
  packet.header = header;

  unsigned int* questId = packet.questIds;
  for (auto it = this->m_statusRequests.begin(); it != this->m_statusRequests.end(); ++it) {
    memcpy_s(questId, sizeof(unsigned int), &*it, sizeof(unsigned int));
    ++questId;
  }

  GetRagConnection().Send(length, &packet);
}

// OFFSET: 0x0067B800
// FUNCTION: fn_05462
bool CRenewQuestMgr::SetOngoingQuestState(unsigned int id, unsigned char state) {
  const auto found = this->m_ongoingQuests.find(id);
  if (found != this->m_ongoingQuests.end()) {
    found->second.m_state = state;
    return true;
  }

  return false;
}

// OFFSET: 0x0067B850
// FUNCTION: fn_05463
bool CRenewQuestMgr::SetRecommendedQuestActive(unsigned int id, bool active) {
  const auto found = this->m_recommendedQuests.find(id);
  if (found != this->m_recommendedQuests.end()) {
    found->second.m_active = active;
    return true;
  }

  return false;
}

// OFFSET: 0x0067B8A0
// FUNCTION: fn_05464
bool CRenewQuestMgr::SetOngoingSimpleView(unsigned int id, bool simpleView) {
  const auto found = this->m_ongoingQuests.find(id);
  if (found != this->m_ongoingQuests.end()) {
    found->second.m_simpleView = simpleView;
    return true;
  }

  return false;
}

// OFFSET: 0x0067BF90
// FUNCTION: fn_05466
void CRenewQuestMgr::SetParameter(int key, int value) {
  if (key == 3502) {
    this->m_parameter = value;
  }
}

// OFFSET: 0x0067E9F0
// FUNCTION: fn_05497
int CRenewQuestMgr::L_SetRecommendedQuestActive(lua_State* state) {
  const unsigned int id = static_cast<unsigned int>(luaL_checknumber(state, 1));
  CRenewQuestMgr* manager = g_renewQuestMgr;
  const auto found = manager->m_recommendedQuests.find(id);
  if (found != manager->m_recommendedQuests.end()) {
    found->second.m_active = false;
  }

  return 0;
}
