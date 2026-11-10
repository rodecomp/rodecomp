#ifndef RAGNAROK_SOURCE_CONTENTS_ACHIEVEMENT_ACHIEVEMENTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ACHIEVEMENT_ACHIEVEMENTMGR_H

#include <map>
#include <string>
#include <utility>
#include <vector>

struct lua_State;

struct AchievementReward {
  // INLINED
  AchievementReward(std::string text, int targetCount, int conditionId) {
    this->text = text;
    this->targetCount = targetCount;
    this->conditionId = conditionId;
  }

  /* 0000 */ std::string text;
  /* 0018 */ int targetCount;
  /* 001C */ int conditionId;
};

struct AchievementInfo {
  // INLINED
  AchievementInfo() {
  }
  // INLINED
  AchievementInfo(std::string title, std::string summaryText, std::string description, int points)
      : title(title), summaryText(summaryText), description(description), points(points) {
  }

  /* 0000 */ std::string title;
  /* 0018 */ std::string summaryText;
  /* 0030 */ std::string description;
  /* 0048 */ int points;
  /* 004C */ int rewardItemId = -1;
  /* 0050 */ int rewardTitleId = -1;
  /* 0054 */ int rewardBuffId = -1;
  /* 0058 */ std::vector<AchievementReward> rewards;
  /* 0064 */ int progressMode;
  /* 0068 */ std::string iconName;
};

static_assert(sizeof(AchievementInfo) == 0x80);

class CAchievementMgr {
public:
  // INLINED
  CAchievementMgr() : enabled(true), groupWidth(10) {
  }
  // INLINED
  virtual ~CAchievementMgr() {
    this->achievements.clear();
    this->groups.clear();
  }

  static bool CreateInstance();
  static void DestroyInstance();
  bool LoadAchievementInfo();

  /* 0004 */ bool enabled;
  /* 0008 */ int groupWidth;
  /* 000C */ std::map<int, AchievementInfo> achievements;
  /* 0014 */ std::map<int, std::vector<int>> groups;

  bool GetAchievement(AchievementInfo& out, int id);
  std::string GetIconName(int id);
  bool GetGroupAchievements(std::map<int, AchievementInfo>& out, int group, int index);
  int GetPoints(int id);
  std::string GetSummaryText(int id);
  int GetRewardTitleId(int id);
  int GetAchievementCount(int group, int index);
  int GetCompletedCount(int group, int index);
  bool FindTab(int id, int& group, int& index);
  std::pair<bool, std::string> InsertAchieveInfo(int id, std::string title, std::string padding_0018, std::string description, int points);
  std::pair<bool, std::string> InsertAchieveResource(int id, std::string text, int padding_0018, int padding_001C);
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
