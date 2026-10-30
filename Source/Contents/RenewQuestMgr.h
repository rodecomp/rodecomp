#ifndef RAGNAROK_SOURCE_CONTENTS_RENEWQUESTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_RENEWQUESTMGR_H

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
  /* 004C */ std::string unknown_004C;
  /* 0064 */ std::string unknown_0064;
  /* 007C */ std::string unknown_007C;
  /* 0094 */ std::string unknown_0094;
  /* 00AC */ int unknown_00AC;
  /* 00B0 */ int unknown_00B0;

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
  /* 00CE */ short unknown_00CE;
  /* 00D0 */ short unknown_00D0;
  /* 00D2 */ char m_huntCount;
  /* 00D4 */ int m_huntUnknown00D4[3];
  /* 00E0 */ short m_huntUnknown00E0[3];
  /* 00E6 */ char m_huntName[3][0x33];
  /* 0180 */ short m_huntUnknown0180[3];
  /* 0188 */ __int64 m_huntId[3];
  /* 01A0 */ int m_huntUnknown01A0[3];
  /* 01AC */ short m_huntUnknown01AC[3];
  /* 01B2 */ short m_huntUnknown01B2[3];
  /* 01B8 */ std::string unknown_01B8;
  /* 01D0 */ std::string unknown_01D0;
  /* 01E8 */ std::vector<std::pair<unsigned int, unsigned int>> m_rewards;
  /* 01F8 */ __int64 unknown_01F8;

  RenewQuestOngoingInfo();
  std::string GetDateString() const;
  std::string GetHuntText(int index) const;
};

struct RenewQuestRecommendedInfo : RenewQuestInfo {
  /* 00B4 */ std::string unknown_00B4;
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
  void RemoveStatusRequest(unsigned int id);
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
  void sub_67B620(unsigned int questId);
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
