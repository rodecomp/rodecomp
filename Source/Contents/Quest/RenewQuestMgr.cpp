#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Script/LuaInstance.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextEscapes.h"
#include "UI/Quest/UIOngoingQuestInfo.h"
#include "UI/Quest/UIRenewQuestUI.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Session/Session.h"

#include "Resource/MSGSTRINGID.h"
#include "UI/Quest/UIQuestDisplay.h"
#include "UI/Status/UIBasicInfoWnd.h"
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

template __declspec(noinline) bool std::string_view::_Equal(const std::string_view) const noexcept;
template __declspec(noinline) std::list<std::string>::const_iterator std::find<std::list<std::string>::const_iterator, std::string_view>(std::list<std::string>::const_iterator, const std::list<std::string>::const_iterator, const std::string_view&);

#pragma pack(push, 1)
struct RenewQuestReplayHeader {
  /* 0000 */ unsigned int count;
};

struct RenewQuestOngoingRecord {
  /* 0000 */ unsigned int questId;
  /* 0004 */ unsigned char state;
  /* 0005 */ unsigned char padding_0005;
  /* 0006 */ short huntCount;
  /* 0008 */ short year;
  /* 000A */ short month;
  /* 000C */ short day;
  /* 000E */ short hour;
  /* 0010 */ unsigned short minute;
  /* 0012 */ unsigned short padding_0012;
  /* 0014 */ unsigned int padding_0014;
};

struct RenewQuestHuntRecord {
  /* 0000 */ int huntId;
  /* 0004 */ int monsterCondition;
  /* 0008 */ int monsterId;
  /* 000C */ short minLevel;
  /* 000E */ short maxLevel;
  /* 0010 */ short killCount;
  /* 0012 */ short killGoal;
  /* 0014 */ char name[0x33];
};

struct RenewQuestShortHuntRecord {
  /* 0000 */ int huntId;
  /* 0004 */ int monsterCondition;
  /* 0008 */ int monsterId;
  /* 000C */ short minLevel;
  /* 000E */ short maxLevel;
  /* 0010 */ short killCount;
  /* 0012 */ short killGoal;
  /* 0014 */ char name[0x18];
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
static_assert(sizeof(RenewQuestReplayHeader) == 4);
static_assert(sizeof(RenewQuestOngoingRecord) == 0x18);
static_assert(sizeof(RenewQuestHuntRecord) == 0x47);
static_assert(sizeof(RenewQuestShortHuntRecord) == 0x2C);
static_assert(sizeof(PACKET_CZ_QUEST_STATUS_REQ_BUFFER) == 0x800);
#endif

// GLOBAL: gl_24837
CRenewQuestMgr* g_renewQuestMgr;

// OFFSET: 0x006765C0
// FUNCTION: fn_05420
RenewQuestInfo::RenewQuestInfo()
    : m_state(1), m_title(""), m_icon(""), m_iconTooltip(""), summary(""), padding_0064(""), npcSprite(""), destinationMap(""),
      destinationX(0), destinationY(0) {}

// OFFSET: 0x00676770
// FUNCTION: fn_05421
RenewQuestRecommendedInfo::RenewQuestRecommendedInfo() : backgroundImage("") {
  for (int index = 0; index < 3; ++index) {
    this->m_descriptions[index].clear();
  }
}

// OFFSET: 0x00675990
// FUNCTION: fn_05414
CRenewQuestMgr::CRenewQuestMgr() {
  this->m_initialized = false;
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
      if (!g_renewQuestMgr->m_initialized) {
        delete g_renewQuestMgr;
        g_renewQuestMgr = nullptr;
        return false;
      }

      return true;
    }
  }

  return false;
}

// OFFSET: 0x00677CF0
// FUNCTION: fn_05435
void Nullsub212() {
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
  const auto found = this->m_statusRequests.find(static_cast<unsigned int>(id));
  if (found != this->m_statusRequests.end()) {
    this->m_statusRequests.erase(found);
  }
}

// OFFSET: 0x00677D80
// FUNCTION: fn_05438
std::string RenewQuestOngoingInfo::GetDateString() const {
  return 1970 < this->m_year ? std::format("{0} [{1:04}/{2:02}/{3:02} {4:02}:{5:02}]", LookupMessage(MSI_NAVIGATION_ROUTEINFO), this->m_year,
                                           this->m_month, this->m_day, this->m_hour, this->m_minute)
                              : std::string("");
}

// OFFSET: 0x006779A0
// FUNCTION: fn_05432
void CRenewQuestMgr::AddIconTooltip(std::string_view icon, std::string_view tooltip) {
  this->m_iconTooltips[std::string(icon)] = std::string(tooltip);
}

// OFFSET: 0x00677F40
// FUNCTION: fn_05439
std::string CRenewQuestMgr::GetFilterName(int filter) {
  const std::map<int, RenewQuestFilter>::const_iterator found = this->m_filters.find(filter);
  return this->m_filters.end() != found ? found->second.m_name : std::string("");
}

// OFFSET: 0x006780B0
// FUNCTION: fn_05440
std::string CRenewQuestMgr::GetMonsterConditionText(unsigned int flags, int minLevel, int maxLevel) {
  std::string result;
  std::string separator = " ";

  if (flags != 0) {
    if (flags & 0x20000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY4));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x40000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY7));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x80000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY5));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x100000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY6));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x200000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY0));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x400000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY9));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x800000) {
      std::string text(LookupMessage(MSI_NAVIGATION_PROPERTY8));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x1000000) {
      std::string text(LookupMessage(MSI_NAVIGATION_SCALE1));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x2000000) {
      std::string text(LookupMessage(MSI_NAVIGATION_SCALE0));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x4000000) {
      std::string text(LookupMessage(MSI_NAVIGATION_SCALE2));
      text.erase(text.size() - 2, text.size());
      result = result + text + separator;
    }

    if (flags & 0x10) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE3)) + separator;
    }

    if (flags & 0x20) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE1)) + separator;
    }

    if (flags & 0x40) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE2)) + separator;
    }

    if (flags & 0x80) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE0)) + separator;
    }

    if (flags & 0x100) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE5)) + separator;
    }

    if (flags & 0x200) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE7)) + separator;
    }

    if (flags & 0x400) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE8)) + separator;
    }

    if (flags & 0x800) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE6)) + separator;
    }

    if (flags & 0x1000) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE9)) + separator;
    }

    if (flags & 0x2000) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_MOVECLICK)) + separator;
    }

    if (flags & 0x4000) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_RACE4)) + separator;
    }

    if (flags & 0x8000) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_NPCCLICK)) + separator;
    }

    if (flags & 0x10000) {
      result = result + std::string(LookupMessage(MSI_NAVIGATION_NPCMOVE)) + separator;
    }
  }

  if (minLevel == 0) {
    if (maxLevel > 0) {
      result = result + " Lv." + IntToString(maxLevel, 0) + "▼";
    }
  } else if (minLevel > 0) {
    if (maxLevel == 0) {
      result = result + " Lv." + IntToString(minLevel, 0) + "▲";
    } else if (minLevel < maxLevel) {
      result = result + " Lv." + IntToString(minLevel, 0) + "~Lv." + IntToString(maxLevel, 0);
    }
  }

  return result;
}

// OFFSET: 0x006792A0
// FUNCTION: fn_05441
std::string CRenewQuestMgr::GetIconTooltip(std::string_view icon) {
  const std::map<std::string, std::string>::const_iterator found = this->m_iconTooltips.find(std::string(icon));
  return this->m_iconTooltips.end() != found ? found->second : std::string("QUEST");
}

// OFFSET: 0x006794D0
// FUNCTION: fn_05442
std::string RenewQuestOngoingInfo::GetHuntText(int index) const {
  if (index >= 3 || this->m_huntCount <= index) {
    return std::string("");
  }

  std::string text;
  const short minLevel = this->m_huntMinLevel[index];
  const short maxLevel = this->m_huntMaxLevel[index];
  if (this->m_huntMobId[index] == 31999) {
    text = static_cast<const std::string&>(g_renewQuestMgr->GetMonsterConditionText(this->m_huntConditionFlags[index], minLevel, maxLevel));
  } else {
    const char* name = this->m_huntName[index];
    if (this->m_huntMinLevel[index] >= 0 && this->m_huntMaxLevel[index] >= 0 && (minLevel != 0 || this->m_huntMaxLevel[index] != 0)) {
      if (minLevel == 0) {
        text = std::format("{} Lv.{}▼", name, maxLevel);
      } else if (this->m_huntMaxLevel[index] == 0) {
        text = std::format("{} Lv.{}▲", name, minLevel);
      } else {
        text = std::format("{} Lv.{}~Lv.{}", name, minLevel, maxLevel);
      }
    } else {
      text = name;
    }
  }

  const short goal = this->m_huntMaxCount[index];
  if (goal > 0) {
    const short count = this->m_huntKillCount[index];
    if (count == goal) {
      text = std::format("{} ( {} )", text, LookupMessage(MSI_COMPLETE_HUNTING_QUEST));
    } else {
      text = std::format("{} ( {} / {} )", text, count, goal);
    }
  }

  return text;
}

// OFFSET: 0x00679920
// FUNCTION: fn_05443
const RenewQuestOngoingInfo& CRenewQuestMgr::GetOngoingQuest(unsigned int id) {
  const auto found = this->m_ongoingQuests.find(id);
  if (this->m_ongoingQuests.end() != found) {
    return found->second;
  }

  // GLOBAL: gl_24857
  static RenewQuestOngoingInfo emptyQuest;
  return emptyQuest;
}

// OFFSET: 0x00679A00
// FUNCTION: fn_05444
RenewQuestOngoingInfo* CRenewQuestMgr::FindOngoingQuest(unsigned int id) {
  const auto found = this->m_ongoingQuests.find(id);
  if (this->m_ongoingQuests.end() != found) {
    return &found->second;
  }

  return nullptr;
}

// OFFSET: 0x00679AF0
// FUNCTION: fn_05445
const RenewQuestRecommendedInfo& CRenewQuestMgr::GetRecommendedQuest(unsigned int id) {
  const auto found = this->m_recommendedQuests.find(id);
  if (this->m_recommendedQuests.end() != found) {
    return found->second;
  }

  // GLOBAL: gl_24858
  static RenewQuestRecommendedInfo empty;
  return empty;
}

// OFFSET: 0x00679BD0
// FUNCTION: fn_05446
RenewQuestRecommendedInfo* CRenewQuestMgr::FindRecommendedQuest(unsigned int id) {
  const auto found = this->m_recommendedQuests.find(id);
  if (this->m_recommendedQuests.end() != found) {
    return &found->second;
  }

  return nullptr;
}

// OFFSET: 0x00679C20
// FUNCTION: fn_05447
void CRenewQuestMgr::WriteOngoingQuests(char* buffer) {
  int offset = 4;
  reinterpret_cast<RenewQuestReplayHeader*>(buffer)->count = this->m_ongoingQuests.size();

  for (auto it = this->m_ongoingQuests.begin(); it != this->m_ongoingQuests.end(); ++it) {
    const RenewQuestOngoingInfo& info = it->second;
    const char huntCount = info.m_huntCount;

    RenewQuestOngoingRecord record = {};
    record.state = info.m_state;
    record.huntCount = huntCount;
    record.year = info.m_year;
    record.month = info.m_month;
    record.day = info.m_day;
    record.hour = info.m_hour;
    record.minute = info.m_minute;
    record.questId = it->first;
    *reinterpret_cast<RenewQuestOngoingRecord*>(buffer + offset) = record;
    offset += sizeof(RenewQuestOngoingRecord);

    for (int index = 0; index < huntCount; ++index) {
      RenewQuestHuntRecord hunt;
      SecureZeroMemory(&hunt, sizeof(hunt));
      hunt.huntId = static_cast<int>(info.m_huntId[index]);
      hunt.monsterCondition = info.m_huntConditionFlags[index];
      hunt.monsterId = info.m_huntMobId[index];
      hunt.minLevel = info.m_huntMinLevel[index];
      hunt.maxLevel = info.m_huntMaxLevel[index];
      hunt.killCount = info.m_huntKillCount[index];
      hunt.killGoal = info.m_huntMaxCount[index];
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

// OFFSET: 0x00679E50
// FUNCTION: fn_05449
void CRenewQuestMgr::LoadQuestInfo() {
  {
    CLuaInstance lua;
    lua.SetGlobal("AddQuestIconTooltip", L_AddQuestIconTooltip);
    lua.SetGlobal("AddQuestFilterInfo", L_AddQuestFilterInfo);
    if (!lua.LoadFile("System\\QuestClassificationInfo.lub", true)) {
      return;
    }

    lua.Execute("LoadQuestIconTooltip");
    lua.Execute("LoadQuestFilterInfo");
  }

  g_session.m_hotkeyScript->Register("AddOngoingDescription", L_AddOngoingDescription);
  g_session.m_hotkeyScript->Register("AddOngoingRewardInfo", L_AddOngoingRewardInfo);
  g_session.m_hotkeyScript->Register("InsertRecommededQuestInfo", L_InsertRecommendedQuestInfo);
  g_session.m_hotkeyScript->Register("AddRecommendedQuestInfo", L_AddRecommendedQuestInfo);
  g_session.m_hotkeyScript->Register("SetRecommendedQuestActive", L_SetRecommendedQuestActive);
  g_session.m_hotkeyScript->Load("Lua Files\\DataInfo\\QuestInfo_f", true, false);
  g_session.m_hotkeyScript->Load("savedata\\OngoingSimpleViewList", false, true);
  g_session.m_hotkeyScript->Load("savedata\\RecommendedActiveList", false, true);
  if (g_serviceMode == ServerSakray) {
    g_session.m_hotkeyScript->Load("system\\OngoingQuestInfoList_Sakray", false, true);
    g_session.m_hotkeyScript->Load("system\\RecommendedQuestInfoList_Sakray", false, true);
  } else {
    g_session.m_hotkeyScript->Load("system\\OngoingQuestInfoList_True", false, true);
    g_session.m_hotkeyScript->Load("system\\RecommendedQuestInfoList_True", false, true);
  }

  g_session.m_hotkeyScript->Call("RecommendedQuestInfoLoad", "");
  g_session.m_hotkeyScript->Call("RecommendedQuestActiveLoad", "");
}

// OFFSET: 0x0067A500
// FUNCTION: fn_05450
void CRenewQuestMgr::AddNewQuest(unsigned int id) {
  this->m_newQuests.insert(static_cast<unsigned int>(id));
  const auto found = this->m_statusRequests.find(static_cast<unsigned int>(id));
  if (found != this->m_statusRequests.end()) {
    this->m_statusRequests.erase(found);
  }
}

// OFFSET: 0x0067A630
// FUNCTION: fn_05451
void CRenewQuestMgr::AddOngoingQuest(unsigned int id, RenewQuestOngoingInfo& info) {
  bool simpleView = true;
  const char* title = nullptr;
  const char* icon = nullptr;
  const char* text3 = nullptr;
  const char* text4 = nullptr;
  const char* text5 = nullptr;
  const char* text6 = nullptr;
  const char* text7 = nullptr;
  int destinationX = 0;
  int destinationY = 0;
  int type = 0;
  if (g_session.m_hotkeyScript->Call("GetOngoingQuestInfoByID", "d>bsssssssddd", id, &simpleView, &title, &icon, &text3, &text4, &text5, &text6,
                        &text7, &destinationX, &destinationY, &type)) {
    info.m_title = title ? title : "";
    info.m_icon = icon ? icon : "";
    info.summary = text3 ? text3 : "";
    info.npcSprite = text4 ? text4 : "";
    info.destinationMap = text5 ? text5 : "";
    info.baseExpReward = text6 ? text6 : "";
    info.jobExpReward = text7 ? text7 : "";
    info.destinationX = destinationX;
    info.destinationY = destinationY;
    info.m_simpleView = simpleView;
    info.questType = type;
    if (type == 1) {
      info.m_simpleView = false;
    }

    info.m_iconTooltip = this->GetIconTooltip(info.m_icon);
  }

  UIWindow* window = static_cast<UIWindow*>(g_windowMgr.basicInfoWnd);
  if (window != nullptr) {
    info.m_title = window->FitDisplayText(250, info.m_title.c_str(), 0, 12, 1);
    info.summary = window->FitDisplayText(250, info.summary.c_str(), 0, 12, 0);
  }

  this->m_ongoingQuests.erase(id);
  this->m_ongoingQuests.insert(std::pair<const unsigned int, RenewQuestOngoingInfo>(id, info));
  g_session.m_hotkeyScript->Call("GetOngoingDescription", "d", id);
  g_session.m_hotkeyScript->Call("GetOngoingRewardInfo", "d", id);
}

// OFFSET: 0x0067ACE0
// FUNCTION: fn_05452
void CRenewQuestMgr::AddRecommendedQuest(unsigned int id, const RenewQuestRecommendedInfo& info) {
  this->m_recommendedQuests.erase(id);
  this->m_recommendedQuests.insert(std::pair<const unsigned int, RenewQuestRecommendedInfo>(id, info));
}

// OFFSET: 0x0067AEB0
// FUNCTION: fn_05453
bool CRenewQuestMgr::IsNewQuest(unsigned int id) {
  return this->m_newQuests.find(static_cast<unsigned int>(id)) != this->m_newQuests.end();
}

// OFFSET: 0x0067AF00
// FUNCTION: fn_05454
bool CRenewQuestMgr::IsInFilter(int filter, std::string_view icon) {
  switch (filter) {
  case 0:
    return true;
  case 1:
    for (auto& entry : this->m_filters) {
      std::list<std::string>& icons = entry.second.m_icons;
      if (icons.end() != std::find(icons.begin(), icons.end(), icon)) {
        return false;
      }
    }
    return true;
  default: {
    const auto found = this->m_filters.find(filter);
    if (this->m_filters.end() == found) {
      return false;
    }

    const std::list<std::string>& icons = found->second.m_icons;
    return icons.end() != std::find(icons.begin(), icons.end(), icon);
  }
  }
}

// OFFSET: 0x0067B030
// FUNCTION: fn_05455
int CRenewQuestMgr::L_AddQuestFilterInfo(lua_State* state) {
  const lua_Number id = lua_tonumber(state, -3);
  const std::string name = lua_tostring(state, -2);
  std::list<std::string> icons;
  if (lua_type(state, -1) != LUA_TNIL) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      icons.emplace_back(lua_tostring(state, -1));
      lua_pop(state, 1);
    }
  }

  g_renewQuestMgr->m_filters.try_emplace(static_cast<int>(id), RenewQuestFilter{std::string(name.c_str(), name.size()), std::move(icons)});
  return 0;
}

// OFFSET: 0x0067B460
// FUNCTION: fn_05456
int CRenewQuestMgr::L_AddQuestIconTooltip(lua_State* state) {
  const std::string icon = lua_tostring(state, -2);
  const std::string tooltip = lua_tostring(state, -1);
  CRenewQuestMgr* manager = g_renewQuestMgr;
  manager->AddIconTooltip(icon, tooltip);
  return 0;
}

// OFFSET: 0x0067B620
// FUNCTION: fn_05457
void CRenewQuestMgr::ShowOngoingQuestInfo(unsigned int questId) {
  if (auto* window = static_cast<CUIOngoingQuestInfo*>(g_windowMgr.EnsureWindow(WID_ONGOINGQUESTINFO))) {
    window->SetQuest(questId);
  }
}

// OFFSET: 0x0067B650
// FUNCTION: fn_05458
void CRenewQuestMgr::ShowQuestInfo(unsigned int questId) {
  if (auto* window = static_cast<CUIRenewQuestUI*>(g_windowMgr.EnsureWindow(WID_RENEWQUESTUI))) {
    window->UpdateQuestLists(0xF);
    window->SelectQuest(questId);
  }
}

// OFFSET: 0x0067B690
// FUNCTION: fn_05459
void CRenewQuestMgr::UpdateOngoingQuestInfo() {
  if (auto* window = static_cast<CUIOngoingQuestInfo*>(g_windowMgr.LookupWindow(WID_ONGOINGQUESTINFO))) {
    window->UpdateHuntInfo();
  }
}

// OFFSET: 0x0067B6B0
// FUNCTION: fn_05460
void CRenewQuestMgr::UpdateQuestWindows() {
  if (g_windowMgr.questDisplayWnd != nullptr) {
    g_windowMgr.questDisplayWnd->Invalidate();
  }

  if (auto* window = static_cast<CUIRenewQuestUI*>(g_windowMgr.LookupWindow(WID_RENEWQUESTUI))) {
    window->UpdateQuestLists(0xD);
  }
}

// OFFSET: 0x0067B6E0
// FUNCTION: fn_05461
void CRenewQuestMgr::SendQuestStatusRequest() {
  PACKET_CZ_QUEST_STATUS_REQ header;
  header.packetType = ID_CZ_QUEST_STATUS_REQ;
  PACKET_CZ_QUEST_STATUS_REQ_BUFFER packet;
  const unsigned int length = this->m_statusRequests.size() * sizeof(unsigned int) + sizeof(PACKET_CZ_QUEST_STATUS_REQ);
  std::memset(packet.questIds, 0, sizeof(packet.questIds));
  if (length > sizeof(packet)) {
    return;
  }

  header.packetLength = static_cast<std::int16_t>(length);
  packet.header = header;

  unsigned int* questId = packet.questIds;
  for (const unsigned int& id : this->m_statusRequests) {
    memcpy_s(questId, sizeof(unsigned int), &id, sizeof(unsigned int));
    ++questId;
  }

  GetRagConnection().Send(length, &packet);
}

// OFFSET: 0x0067B800
// FUNCTION: fn_05462
bool CRenewQuestMgr::SetOngoingQuestState(unsigned int id, unsigned char state) {
  const auto found = this->m_ongoingQuests.find(id);
  if (this->m_ongoingQuests.end() != found) {
    found->second.m_state = state;
    return true;
  }

  return false;
}

// OFFSET: 0x0067B850
// FUNCTION: fn_05463
bool CRenewQuestMgr::SetRecommendedQuestActive(unsigned int id, bool active) {
  const auto found = this->m_recommendedQuests.find(id);
  if (this->m_recommendedQuests.end() != found) {
    found->second.m_active = active;
    return true;
  }

  return false;
}

// OFFSET: 0x0067B8A0
// FUNCTION: fn_05464
bool CRenewQuestMgr::SetOngoingSimpleView(unsigned int id, bool simpleView) {
  const auto found = this->m_ongoingQuests.find(id);
  if (this->m_ongoingQuests.end() == found) {
    return false;
  }

  found->second.m_simpleView = simpleView;
  return true;
}

// OFFSET: 0x0067B8F0
// FUNCTION: fn_05465
void CRenewQuestMgr::ReadOngoingQuests(const char* buffer, unsigned int) {
  int offset = 4;
  const unsigned int count = reinterpret_cast<const RenewQuestReplayHeader*>(buffer)->count;
  for (unsigned int index = 0; index < count; ++index) {
    const RenewQuestOngoingRecord record = *reinterpret_cast<const RenewQuestOngoingRecord*>(buffer + offset);
    offset += sizeof(RenewQuestOngoingRecord);
    if (!this->SetOngoingQuestState(record.questId, static_cast<unsigned char>(record.state))) {
      RenewQuestOngoingInfo info;
      info.m_state = static_cast<unsigned char>(record.state);
      info.m_year = record.year;
      info.m_month = record.month;
      info.m_day = record.day;
      info.m_hour = record.hour;
      info.m_minute = static_cast<unsigned short>(record.minute);
      info.m_huntCount = static_cast<char>(record.huntCount);
      for (int hunt = 0; hunt < record.huntCount; ++hunt) {
        if (this->m_parameter == 0x33) {
          RenewQuestHuntRecord huntRecord;
          memcpy_s(&huntRecord, sizeof(huntRecord), buffer + offset, sizeof(huntRecord));
          info.m_huntId[hunt] = huntRecord.huntId;
          info.m_huntConditionFlags[hunt] = huntRecord.monsterCondition;
          info.m_huntMobId[hunt] = huntRecord.monsterId;
          info.m_huntKillCount[hunt] = huntRecord.killCount;
          info.m_huntMaxCount[hunt] = huntRecord.killGoal;
          info.m_huntMinLevel[hunt] = huntRecord.minLevel;
          info.m_huntMaxLevel[hunt] = huntRecord.maxLevel;
          strncpy_s(info.m_huntName[hunt], sizeof(info.m_huntName[hunt]), huntRecord.name, _TRUNCATE);
          offset += sizeof(RenewQuestHuntRecord);
        } else {
          RenewQuestShortHuntRecord huntRecord;
          memcpy_s(&huntRecord, sizeof(huntRecord), buffer + offset, sizeof(huntRecord));
          info.m_huntId[hunt] = huntRecord.huntId;
          info.m_huntConditionFlags[hunt] = huntRecord.monsterCondition;
          info.m_huntMobId[hunt] = huntRecord.monsterId;
          info.m_huntKillCount[hunt] = huntRecord.killCount;
          info.m_huntMaxCount[hunt] = huntRecord.killGoal;
          info.m_huntMinLevel[hunt] = huntRecord.minLevel;
          info.m_huntMaxLevel[hunt] = huntRecord.maxLevel;
          strncpy_s(info.m_huntName[hunt], sizeof(info.m_huntName[hunt]), huntRecord.name, sizeof(huntRecord.name));
          offset += sizeof(RenewQuestShortHuntRecord);
        }
      }

      this->AddOngoingQuest(record.questId, info);
    }
  }

  if (g_windowMgr.questDisplayWnd != nullptr) {
    g_windowMgr.questDisplayWnd->Invalidate();
  }

  if (auto* window = static_cast<CUIRenewQuestUI*>(g_windowMgr.LookupWindow(WID_RENEWQUESTUI))) {
    window->UpdateQuestLists(0xD);
  }
}

// OFFSET: 0x0067BF90
// FUNCTION: fn_05466
void CRenewQuestMgr::SetParameter(int key, int value) {
  if (key == 3502) {
    this->m_parameter = value;
  }
}

// OFFSET: 0x0067BFB0
// FUNCTION: fn_05467
void CRenewQuestMgr::SaveOngoingSimpleViewList() {
  std::string path = "SaveData\\OngoingSimpleViewList.lua";
  CFile file;
  if (file.Open(path.c_str(), 0x1001, true, true)) {
    std::string text = "OngoingSimpleViewList = {}\r\n\r\n";
    file.Write(text.c_str(), text.size());
    text = "OngoingSimpleViewList = \r\n{\r\n";
    file.Write(text.c_str(), text.size());

    for (auto it = this->m_ongoingQuests.begin(); it != this->m_ongoingQuests.end(); ++it) {
      if (!it->second.m_simpleView) {
        text = '\t' + IntToString(it->first, 0) + ",\r\n";
        file.Write(text.c_str(), text.size());
      }
    }

    text = "}";
    file.Write(text.c_str(), text.size());
    file.Close();
  }
}

// OFFSET: 0x0067C5A0
// FUNCTION: fn_05468
void CRenewQuestMgr::SaveRecommendedActiveList() {
  std::string path = "SaveData\\RecommendedActiveList.lua";
  CFile file;
  if (file.Open(path.c_str(), 0x1001, true, true)) {
    std::string text = "RecommendedActiveList = {}\r\n\r\n";
    file.Write(text.c_str(), text.size());
    text = "RecommendedActiveList = \r\n{\r\n";
    file.Write(text.c_str(), text.size());

    for (auto it = this->m_recommendedQuests.begin(); it != this->m_recommendedQuests.end(); ++it) {
      if (!it->second.m_active) {
        text = '\t' + IntToString(it->first, 0) + ",\r\n";
        file.Write(text.c_str(), text.size());
      }
    }

    text = "}";
    file.Write(text.c_str(), text.size());
    file.Close();
  }
}

// OFFSET: 0x0067E370
// FUNCTION: fn_05493
int CRenewQuestMgr::L_AddOngoingDescription(lua_State* state) {
  const unsigned int id = static_cast<unsigned int>(luaL_checknumber(state, 1));
  std::string description = luaL_checkstring(state, 2);
  DecodeTextEscapes(description);
  g_renewQuestMgr->m_ongoingQuests[static_cast<unsigned int>(id)].m_descriptions.push_back(description);
  return 0;
}

// OFFSET: 0x0067E4C0
// FUNCTION: fn_05494
int CRenewQuestMgr::L_AddOngoingRewardInfo(lua_State* state) {
  const unsigned int id = static_cast<unsigned int>(luaL_checknumber(state, 1));
  const unsigned int item = static_cast<unsigned int>(luaL_checknumber(state, 2));
  const unsigned int count = static_cast<unsigned int>(luaL_checknumber(state, 3));
  g_renewQuestMgr->m_ongoingQuests[static_cast<unsigned int>(id)].m_rewards.push_back(std::make_pair(item, count));
  return 0;
}

// OFFSET: 0x0067E570
// FUNCTION: fn_05495
int CRenewQuestMgr::L_AddRecommendedQuestInfo(lua_State* state) {
  const unsigned int id = static_cast<unsigned int>(luaL_checknumber(state, 1));
  const int index = static_cast<int>(luaL_checknumber(state, 2));
  std::string description = luaL_checkstring(state, 3);
  DecodeTextEscapes(description);
  g_renewQuestMgr->m_recommendedQuests[static_cast<unsigned int>(id)].m_descriptions[index].push_back(description);
  return 0;
}

// OFFSET: 0x0067E770
// FUNCTION: fn_05496
int CRenewQuestMgr::L_InsertRecommendedQuestInfo(lua_State* state) {
  RenewQuestRecommendedInfo info;
  const unsigned int id = static_cast<unsigned int>(luaL_checknumber(state, 1));
  info.m_active = true;
  info.m_title = luaL_checkstring(state, 2);
  info.m_icon = luaL_checkstring(state, 3);
  info.summary = luaL_checkstring(state, 4);
  info.backgroundImage = luaL_checkstring(state, 5);
  info.npcSprite = luaL_checkstring(state, 6);
  info.destinationMap = luaL_checkstring(state, 7);
  info.destinationX = static_cast<int>(luaL_checknumber(state, 8));
  info.destinationY = static_cast<int>(luaL_checknumber(state, 9));
  info.m_iconTooltip = g_renewQuestMgr->GetIconTooltip(info.m_icon);
  g_renewQuestMgr->AddRecommendedQuest(id, info);
  return 0;
}

// OFFSET: 0x0067E9F0
// FUNCTION: fn_05497
int CRenewQuestMgr::L_SetRecommendedQuestActive(lua_State* state) {
  const unsigned int id = static_cast<unsigned int>(luaL_checknumber(state, 1));
  CRenewQuestMgr* manager = g_renewQuestMgr;
  const auto found = manager->m_recommendedQuests.find(id);
  if (manager->m_recommendedQuests.end() != found) {
    found->second.m_active = false;
  }

  return 0;
}
