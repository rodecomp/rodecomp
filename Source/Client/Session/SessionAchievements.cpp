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

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::string& std::string::assign(const char*, const std::size_t);
template __declspec(noinline) SessionRecord6178Entry* std::_Uninitialized_copy<SessionRecord6178Entry*, std::allocator<SessionRecord6178Entry>>(SessionRecord6178Entry*, SessionRecord6178Entry*, SessionRecord6178Entry*, std::allocator<SessionRecord6178Entry>&);
template __declspec(noinline) std::pair<std::_Tree_node<std::pair<const int, SessionRecord6144>, void*>*, bool> std::map<int, SessionRecord6144>::_Mybase::_Emplace<std::pair<int, SessionRecord6144>>(std::pair<int, SessionRecord6144>&&);

struct JobTitleLess {
  bool operator()(int left, int right) const;
};

// OFFSET: 0x00E063F0
// FUNCTION: fn_23735
bool JobTitleLess::operator()(int left, int right) const {
  std::string leftTitle = g_session.GetJobTitleString(left);
  std::string rightTitle = g_session.GetJobTitleString(right);
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

  SessionRecord6144 initial = SessionRecord6144();
  achievements.insert(std::make_pair(id, initial));
  achievements[id].registered = 1;
  return &achievements[id];
}

// OFFSET: 0x00E06580
// FUNCTION: fn_23737
bool CSession::ToggleTrackedAchievement(int id) {
  std::vector<SessionRecord6178>& tracked = this->trackedAchievements;
  auto found = std::find_if(tracked.begin(), tracked.end(), [id](SessionRecord6178 record) { return id == record.id; });

  SessionRecord6144* progress = FindOrRegisterAchievement(this->achievements, id);

  if (!progress) {
    return false;
  }

  if (found != tracked.end()) {
    std::copy(found + 1, tracked.end(), found);
    tracked.pop_back();
  } else {
    if (tracked.size() >= 3) {
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
