#ifndef RAGNAROK_SOURCE_CONTENTS_ACHIEVEMENTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ACHIEVEMENTMGR_H

#include <map>
#include <string>
#include <vector>

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
  static void DestroyInstance();

  /* 0004 */ bool unknown_0004;
  /* 0008 */ int groupWidth;
  /* 000C */ std::map<int, AchievementInfo> achievements;
  /* 0014 */ std::map<int, std::vector<int>> groups;

  // INLINED
  virtual ~CAchievementMgr() {
    this->achievements.clear();
    this->groups.clear();
  }

  bool GetAchievement(AchievementInfo& out, int id);
  std::string GetUnknown0068(int id);
  bool GetGroupAchievements(std::map<int, AchievementInfo>& out, int group, int index);
};

static_assert(sizeof(CAchievementMgr) == 0x1C);

extern CAchievementMgr* g_achievementMgr;

#endif
