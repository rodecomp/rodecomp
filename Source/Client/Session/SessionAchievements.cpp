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
#include "Utility/StdStringCompare.h"

struct JobTitleLess {
  bool operator()(int left, int right) const;
};

// OFFSET: 0x00E063F0
// FUNCTION: fn_23735
bool JobTitleLess::operator()(int left, int right) const {
  std::string leftTitle = GetJobTitleString(left);
  std::string rightTitle = GetJobTitleString(right);
  return leftTitle < rightTitle;
}

// OFFSET: 0x00E064C0
// FUNCTION: fn_23736
int CSession::AddAchievementJobTitle(int id) {
  int value = g_achievementMgr->GetRewardTitleId(id);
  if (value == -1) {
    return -1;
  }

  for (std::vector<int>::iterator it = this->jobTitles.begin(); it != this->jobTitles.end(); ++it) {
    if (*it == value) {
      return -1;
    }
  }

  this->jobTitles.push_back(value);
  std::sort(this->jobTitles.begin(), this->jobTitles.end(), JobTitleLess());

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_EQUIPWND)) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }

  return value;
}

// OFFSET: 0x00E06580
// FUNCTION: fn_23737
bool CSession::ToggleTrackedAchievement(int id) {
  std::vector<SessionRecord6178>& tracked = this->trackedAchievements;
  auto found = tracked.begin();
  while (found != tracked.end() && found->id != id) {
    ++found;
  }

  SessionRecord6144* progress;
  auto record = this->achievements.find(id);
  if (record != this->achievements.end()) {
    progress = &record->second;
  } else {
    this->achievements.insert(std::make_pair(id, SessionRecord6144()));
    reinterpret_cast<unsigned char*>(&this->achievements[id].completionFlags)[0] = 1;
    progress = &this->achievements[id];
  }

  if (!progress) {
    return false;
  }

  if (found != tracked.end()) {
    tracked.erase(found);
  } else {
    if (tracked.size() >= 3) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GET_ROULLETE_PRIZE)), 0x64FFFF, 0, 0, 0);
      return false;
    }

    AchievementInfo info;
    g_achievementMgr->GetAchievement(info, id);

    SessionRecord6178 entry;
    entry.id = id;
    entry.completed = reinterpret_cast<const unsigned char*>(&progress->completionFlags)[1];
    entry.title = info.title;

    for (unsigned int i = 0; i < info.rewards.size(); ++i) {
      const AchievementReward& reward = info.rewards[i];
      std::string text;
      bool completed = false;

      if (info.progressMode == 0) {
        if (reward.text.size() > 40) {
          text = static_cast<const std::string&>(FormatOwnedString("- %s...", reward.text.substr(0, 40).c_str()));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString("- %s", reward.text.c_str()));
        }

        completed = static_cast<int>(progress->objectives[i]) > 0;
      } else if (info.progressMode == 1) {
        std::string count = FormatOwnedString("(%d/%d)", progress->objectives[i], reward.targetCount);
        if (reward.text.size() + count.size() > 40) {
          text = static_cast<const std::string&>(FormatOwnedString("- %s...%s", reward.text.substr(0, 40 - count.size()).c_str(), count.c_str()));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString("- %s %s", reward.text.c_str(), count.c_str()));
        }

        completed = static_cast<int>(progress->objectives[i]) == reward.targetCount;
      }

      entry.objectives.emplace_back(text, completed);
    }

    tracked.push_back(entry);
  }

  if (tracked.size() == 0) {
    g_windowMgr.CloseWindow(WID_ACHTRACINGWND);
    return false;
  }

  return true;
}
