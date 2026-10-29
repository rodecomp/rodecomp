#ifndef RAGNAROK_SOURCE_CONTENTS_RENEWQUESTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_RENEWQUESTMGR_H

#include <list>
#include <map>
#include <set>
#include <string>
#include <string_view>

struct lua_State;

struct RenewQuestOngoingInfo {
  /* 0000 */ unsigned char m_state;
  /* 0001 */ unsigned char unknown_0001[0xB3];
  /* 00B4 */ bool m_simpleView;
  /* 00B5 */ unsigned char unknown_00B5[0xF];
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
  /* 01B8 */ unsigned char unknown_01B8[0x48];

  std::string GetDateString() const;
};

struct RenewQuestRecommendedInfo {
  /* 0000 */ bool m_active;
  /* 0001 */ unsigned char unknown_0001[0xEF];
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
  std::string GetIconTooltip(std::string_view icon);
  RenewQuestOngoingInfo* FindOngoingQuest(unsigned int id);
  RenewQuestRecommendedInfo* FindRecommendedQuest(unsigned int id);
  void WriteOngoingQuests(char* buffer);
  int GetOngoingQuestsSize();
  void AddNewQuest(unsigned int id);
  bool IsNewQuest(unsigned int id);
  bool IsInFilter(int filter, std::string_view icon);
  void SendQuestStatusRequest();
  bool SetOngoingQuestState(unsigned int id, unsigned char state);
  bool SetRecommendedQuestActive(unsigned int id, bool active);
  bool SetOngoingSimpleView(unsigned int id, bool simpleView);
  void SetParameter(int key, int value);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestOngoingInfo) == 0x200);
static_assert(sizeof(RenewQuestRecommendedInfo) == 0xF0);
static_assert(sizeof(RenewQuestFilter) == 0x20);
static_assert(sizeof(CRenewQuestMgr) == 0x3C);
#endif

extern CRenewQuestMgr* g_renewQuestMgr;

#endif
