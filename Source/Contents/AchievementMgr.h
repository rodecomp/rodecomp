#ifndef RAGNAROK_SOURCE_CONTENTS_ACHIEVEMENTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ACHIEVEMENTMGR_H

#include <map>
#include <string>
#include <utility>
#include <vector>

struct lua_State;

struct AchievementReward {
  /* 0000 */ std::string text;
  /* 0018 */ int unknown_0018;
  /* 001C */ int unknown_001C;
};

struct AchievementInfo {
  /* 0000 */ std::string title;
  /* 0018 */ std::string unknown_0018;
  /* 0030 */ std::string unknown_0030;
  /* 0048 */ int unknown_0048;
  /* 004C */ int unknown_004C;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;
  /* 0058 */ std::vector<AchievementReward> rewards;
  /* 0064 */ int unknown_0064;
  /* 0068 */ std::string unknown_0068;
};

static_assert(sizeof(AchievementInfo) == 0x80);

class CAchievementMgr {
public:
  // INLINED
  CAchievementMgr() : unknown_0004(true), groupWidth(10) {
  }
  // INLINED
  virtual ~CAchievementMgr() {
    this->achievements.clear();
    this->groups.clear();
  }

  static bool CreateInstance();
  static void DestroyInstance();
  bool sub_66EAA0();

  /* 0004 */ bool unknown_0004;
  /* 0008 */ int groupWidth;
  /* 000C */ std::map<int, AchievementInfo> achievements;
  /* 0014 */ std::map<int, std::vector<int>> groups;

  bool GetAchievement(AchievementInfo& out, int id);
  std::string GetUnknown0068(int id);
  bool GetGroupAchievements(std::map<int, AchievementInfo>& out, int group, int index);
  int GetUnknown0048(int id);
  std::string GetUnknown0018(int id);
  int GetUnknown0050(int id);
  int GetAchievementCount(int group, int index);
  int GetCompletedCount(int group, int index);
  bool FindTab(int id, int& group, int& index);
  std::pair<bool, std::string> InsertAchieveInfo(int id, std::string title, std::string unknown0018, std::string unknown0030, int unknown0048);
  std::pair<bool, std::string> InsertAchieveResource(int id, std::string text, int unknown0018, int unknown001C);
  std::pair<bool, std::string> InsertAchieveRewardBuff(int id, int value);
  std::pair<bool, std::string> InsertAchieveRewardItem(int id, int value);
  std::pair<bool, std::string> InsertAchieveRewardTitle(int id, int value);
  std::pair<bool, std::string> InsertAchieveUIType(int id, int type, std::string text);
  bool HasReward(int id);
  std::pair<bool, std::string> SetAchieveTab(int id, int group, int index);

  static int L_InsertAchieveInfo(lua_State* state);
  static int L_InsertAchieveResource(lua_State* state);
  static int L_InsertAchieveRewardBuff(lua_State* state);
  static int L_InsertAchieveRewardItem(lua_State* state);
  static int L_InsertAchieveRewardTitle(lua_State* state);
  static int L_InsertAchieveUIType(lua_State* state);
  static int L_SetAchieveIDByTab(lua_State* state);
};

static_assert(sizeof(CAchievementMgr) == 0x1C);

extern CAchievementMgr* g_achievementMgr;

#endif
