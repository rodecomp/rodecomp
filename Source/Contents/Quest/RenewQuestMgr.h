#ifndef RAGNAROK_SOURCE_CONTENTS_QUEST_RENEWQUESTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_QUEST_RENEWQUESTMGR_H

#include <windows.h>

#include <list>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct lua_State;

struct RenewQuestInfo {
  union {
    /* 0000 */ unsigned char m_state;
    /* 0000 */ bool m_active;
  };
  /* 0004 */ std::string m_title;
  /* 001C */ std::string m_icon;
  /* 0034 */ std::string m_iconTooltip;
  /* 004C */ std::string summary;
  /* 0064 */ std::string padding_0064;
  /* 007C */ std::string npcSprite;
  /* 0094 */ std::string destinationMap;
  /* 00AC */ int destinationX;
  /* 00B0 */ int destinationY;

  RenewQuestInfo();
};

struct RenewQuestOngoingInfo : RenewQuestInfo {
  /* 00B4 */ bool m_simpleView;
  /* 00B8 */ std::vector<std::string> m_descriptions;
  /* 00C4 */ short m_year;
  /* 00C6 */ short m_month;
  /* 00C8 */ short m_day;
  /* 00CA */ short m_hour;
  /* 00CC */ unsigned short m_minute;
  /* 00CE */ short padding_00CE;
  /* 00D0 */ short padding_00D0;
  /* 00D2 */ char m_huntCount;
  /* 00D4 */ int m_huntMobId[3];
  /* 00E0 */ short m_huntKillCount[3];
  /* 00E6 */ char m_huntName[3][0x33];
  /* 0180 */ short m_huntMaxCount[3];
  /* 0188 */ __int64 m_huntId[3];
  /* 01A0 */ int m_huntConditionFlags[3];
  /* 01AC */ short m_huntMinLevel[3];
  /* 01B2 */ short m_huntMaxLevel[3];
  /* 01B8 */ std::string baseExpReward;
  /* 01D0 */ std::string jobExpReward;
  /* 01E8 */ std::vector<std::pair<unsigned int, unsigned int>> m_rewards;
  /* 01F8 */ __int64 questType;

  // OFFSET: 0x00676110
  // FUNCTION: fn_05417
  RenewQuestOngoingInfo() : m_simpleView(true), baseExpReward(""), jobExpReward("") {
    this->m_descriptions.clear();
    this->m_rewards.clear();
    for (unsigned int index = 0; index < 3; ++index) {
      this->m_huntMobId[index] = 31999;
      this->m_huntMinLevel[index] = -1;
      this->m_huntMaxLevel[index] = -1;
    }

    SecureZeroMemory(this->m_huntName, sizeof(this->m_huntName));
  }

  std::string GetDateString() const;
  std::string GetHuntText(int index) const;
};

struct RenewQuestRecommendedInfo : RenewQuestInfo {
  /* 00B4 */ std::string backgroundImage;
  /* 00CC */ std::vector<std::string> m_descriptions[3];

  RenewQuestRecommendedInfo();
};

struct RenewQuestFilter {
  /* 0000 */ std::string m_name;
  /* 0018 */ std::list<std::string> m_icons;
};

class CRenewQuestMgr {
public:
  static bool Create();
  static void Destroy();
  static int L_AddQuestFilterInfo(lua_State* state);
  static int L_AddQuestIconTooltip(lua_State* state);
  static int L_SetRecommendedQuestActive(lua_State* state);
  static int L_AddOngoingDescription(lua_State* state);
  static int L_AddOngoingRewardInfo(lua_State* state);
  static int L_AddRecommendedQuestInfo(lua_State* state);
  static int L_InsertRecommendedQuestInfo(lua_State* state);

  /* 0004 */ std::map<unsigned int, RenewQuestOngoingInfo> m_ongoingQuests;
  /* 000C */ std::map<unsigned int, RenewQuestRecommendedInfo> m_recommendedQuests;
  /* 0014 */ bool m_initialized;
  /* 0018 */ int m_parameter;
  /* 001C */ std::set<unsigned int> m_newQuests;
  /* 0024 */ std::set<unsigned int> m_statusRequests;
  /* 002C */ std::map<std::string, std::string> m_iconTooltips;
  /* 0034 */ std::map<int, RenewQuestFilter> m_filters;

  virtual ~CRenewQuestMgr() = default;

  CRenewQuestMgr();
  bool Clear();
  void LoadQuestInfo();
  void AddOngoingQuest(unsigned int id, RenewQuestOngoingInfo& info);
  void ReadOngoingQuests(const char* buffer, unsigned int size);
  void RemoveStatusRequest(unsigned int id);
  void AddStatusRequest(unsigned int id);
  void AddIconTooltip(std::string_view icon, std::string_view tooltip);
  std::string GetFilterName(int filter);
  std::string GetMonsterConditionText(unsigned int flags, int minLevel, int maxLevel);
  std::string GetIconTooltip(std::string_view icon);
  const RenewQuestOngoingInfo& GetOngoingQuest(unsigned int id);
  RenewQuestOngoingInfo* FindOngoingQuest(unsigned int id);
  const RenewQuestRecommendedInfo& GetRecommendedQuest(unsigned int id);
  RenewQuestRecommendedInfo* FindRecommendedQuest(unsigned int id);
  void WriteOngoingQuests(char* buffer);
  int GetOngoingQuestsSize();
  void AddNewQuest(unsigned int id);
  void AddRecommendedQuest(unsigned int id, const RenewQuestRecommendedInfo& info);
  bool IsNewQuest(unsigned int id);
  bool IsInFilter(int filter, std::string_view icon);
  void SendQuestStatusRequest();
  bool SetOngoingQuestState(unsigned int id, unsigned char state);
  bool SetRecommendedQuestActive(unsigned int id, bool active);
  bool SetOngoingSimpleView(unsigned int id, bool simpleView);
  void SetParameter(int key, int value);
  void SaveOngoingSimpleViewList();
  void SaveRecommendedActiveList();
  void ShowOngoingQuestInfo(unsigned int questId);
  void ShowQuestInfo(unsigned int questId);
  void UpdateOngoingQuestInfo();
  void UpdateQuestWindows();
  void SetOngoingQuest(unsigned int id, const RenewQuestOngoingInfo& info);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestInfo) == 0xB4);
static_assert(sizeof(RenewQuestOngoingInfo) == 0x200);
static_assert(sizeof(RenewQuestRecommendedInfo) == 0xF0);
static_assert(sizeof(RenewQuestFilter) == 0x20);
static_assert(sizeof(CRenewQuestMgr) == 0x3C);
#endif

extern CRenewQuestMgr* g_renewQuestMgr;

#endif
