#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <string>
#include <vector>

struct JobTitleLess {
  bool operator()(int left, int right) const;
};

// OFFSET: 0x00E063F0
// FUNCTION: fn_23735
bool JobTitleLess::operator()(int left, int right) const {
  const int& leftId = left;
  const int& rightId = right;
  std::string leftTitle = g_session.GetJobTitleString(leftId);
  std::string rightTitle = g_session.GetJobTitleString(rightId);
  return leftTitle < rightTitle;
}

// OFFSET: 0x00E064C0
// FUNCTION: fn_23736
int CSession::AddAchievementJobTitle(int id) {
  int value = g_achievementMgr->GetRewardTitleId(id);
  if (value == -1) {
    return -1;
  }

  if (std::find(this->jobTitles.begin(), this->jobTitles.end(), value) != this->jobTitles.end()) {
    return -1;
  }

  this->jobTitles.push_back(value);
  std::sort(this->jobTitles.begin(), this->jobTitles.end(), JobTitleLess());

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_EQUIPWND)) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }

  return value;
}

// INLINED
static SessionRecord6144* FindOrRegisterAchievement(std::map<int, SessionRecord6144>& achievements, int id) {
  auto record = achievements.find(id);
  if (record != achievements.end()) {
    return &record->second;
  }

  SessionRecord6144 initial;
  achievements.insert(std::make_pair(id, initial));
  achievements[id].registered = 1;
  return &achievements[id];
}

// OFFSET: 0x00E06580
// FUNCTION: fn_23737
bool CSession::ToggleTrackedAchievement(int id) {
  auto found = std::find_if(this->trackedAchievements.begin(), this->trackedAchievements.end(), [id](SessionRecord6178 record) { return id == record.id; });

  SessionRecord6144* progress = FindOrRegisterAchievement(this->achievements, id);

  if (!progress) {
    return false;
  }

  if (found != this->trackedAchievements.end()) {
    this->trackedAchievements.erase(found);
  } else {
    if (this->trackedAchievements.size() >= 3) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAIL_ADD_ACHIEVEMENT_TRACING)), 0x64FFFF, 0, 0, 0);
      return false;
    }

    AchievementInfo info;
    g_achievementMgr->GetAchievement(info, id);

    SessionRecord6178 entry;
    entry.id = id;
    entry.completed = progress->completed;
    entry.title = info.title;

    for (unsigned int i = 0; i < info.rewards.size(); ++i) {
      SessionRecord6178Entry objective;

      if (info.progressMode == 0) {
        if (info.rewards[i].text.size() > 40) {
          objective.text = static_cast<const std::string&>(FormatOwnedString("- %s...", info.rewards[i].text.substr(0, 40).c_str()));
        } else {
          objective.text = static_cast<const std::string&>(FormatOwnedString("- %s", info.rewards[i].text.c_str()));
        }

        objective.completed = static_cast<int>(progress->objectives[i]) > 0;
      } else if (info.progressMode == 1) {
        std::string count = FormatOwnedString("(%d/%d)", progress->objectives[i], info.rewards[i].targetCount);
        if (info.rewards[i].text.size() + count.size() > 40) {
          objective.text = static_cast<const std::string&>(
              FormatOwnedString("- %s...%s", info.rewards[i].text.substr(0, 40 - count.size()).c_str(), count.c_str()));
        } else {
          objective.text = static_cast<const std::string&>(FormatOwnedString("- %s %s", info.rewards[i].text.c_str(), count.c_str()));
        }

        objective.completed = static_cast<int>(progress->objectives[i]) == info.rewards[i].targetCount;
      }

      entry.objectives.push_back(objective);
    }

    this->trackedAchievements.push_back(entry);
  }

  if (this->trackedAchievements.size() == 0) {
    g_windowMgr.CloseWindow(WID_ACHTRACINGWND);
    return false;
  }

  return true;
}
