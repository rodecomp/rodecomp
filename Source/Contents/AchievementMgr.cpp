#include "Contents/AchievementMgr.h"

#include <new>
#include <utility>

// GLOBAL: gl_24856
CAchievementMgr* g_achievementMgr;

// OFFSET: 0x0066D3A0
// FUNCTION: fn_05311
bool CAchievementMgr::CreateInstance() {
  if (g_achievementMgr != nullptr) {
    return false;
  }

  g_achievementMgr = new (std::nothrow) CAchievementMgr();
  if (g_achievementMgr == nullptr) {
    return false;
  }

  if (!g_achievementMgr->unknown_0004) {
    delete g_achievementMgr;
    g_achievementMgr = nullptr;
    return false;
  }

  if (!g_achievementMgr->sub_66EAA0()) {
    if (g_achievementMgr != nullptr) {
      delete g_achievementMgr;
    }
    g_achievementMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x0066D500
// FUNCTION: fn_05312
void CAchievementMgr::DestroyInstance() {
  if (g_achievementMgr) {
    delete g_achievementMgr;
    g_achievementMgr = nullptr;
  }
}

// OFFSET: 0x0066D540
// FUNCTION: fn_05313
bool CAchievementMgr::GetAchievement(AchievementInfo& out, int id) {
  if (this->achievements.find(id) == this->achievements.end()) {
    return false;
  }

  out = this->achievements[id];
  return true;
}

// OFFSET: 0x0066D720
// FUNCTION: fn_05314
std::string CAchievementMgr::GetUnknown0068(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].unknown_0068;
  }
  return "";
}

// OFFSET: 0x0066D800
// FUNCTION: fn_05315
bool CAchievementMgr::GetGroupAchievements(std::map<int, AchievementInfo>& out, int group, int index) {
  out.clear();

  if (index == -1) {
    for (int i = 0; i < this->groupWidth; ++i) {
      int key = this->groupWidth * group + i;
      for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
        if (!out.insert(std::make_pair(*it, this->achievements[*it])).second) {
          return false;
        }
      }
    }
    return true;
  }

  int key = this->groupWidth * group + index;
  for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
    if (!out.insert(std::make_pair(*it, this->achievements[*it])).second) {
      return false;
    }
  }
  return true;
}
