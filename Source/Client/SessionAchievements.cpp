#include "Client/Session.h"
#include "Contents/AchievementMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/MessageTable.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <string>
#include <vector>

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
int CSession::AddUnknown6160(int id) {
  int value = g_achievementMgr->GetUnknown0050(id);
  if (value == -1) {
    return -1;
  }

  for (std::vector<int>::iterator it = this->unknown_6160.begin(); it != this->unknown_6160.end(); ++it) {
    if (*it == value) {
      return -1;
    }
  }

  this->unknown_6160.push_back(value);
  std::sort(this->unknown_6160.begin(), this->unknown_6160.end());

  if (UIWindow* window = g_windowMgr.LookupWindow(10)) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }

  return value;
}

// OFFSET: 0x00E06580
// FUNCTION: fn_23737
bool CSession::ToggleTrackedAchievement(int id) {
  std::vector<SessionRecord6178>& tracked = this->unknown_6178;
  auto found = std::find_if(tracked.begin(), tracked.end(), [id](SessionRecord6178 record) { return id == record.id; });

  SessionRecord6144* progress;
  auto record = this->unknown_6144.find(id);
  if (record != this->unknown_6144.end()) {
    progress = &record->second;
  } else {
    this->unknown_6144.insert(std::make_pair(id, SessionRecord6144()));
    reinterpret_cast<unsigned char*>(&this->unknown_6144[id].unknown_0000)[0] = 1;
    progress = &this->unknown_6144[id];
  }

  if (!progress) {
    return false;
  }

  if (found != tracked.end()) {
    tracked.erase(found);
  } else {
    if (tracked.size() >= 3) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA78)), 0x64FFFF, 0, 0, 0);
      return false;
    }

    AchievementInfo info;
    g_achievementMgr->GetAchievement(info, id);

    SessionRecord6178 entry;
    entry.id = id;
    entry.unknown_0001 = reinterpret_cast<const unsigned char*>(&progress->unknown_0000)[1];
    entry.unknown_0008 = info.title;

    for (unsigned int i = 0; i < info.rewards.size(); ++i) {
      const AchievementReward& reward = info.rewards[i];
      std::string text;
      bool completed = false;

      if (info.unknown_0064 == 0) {
        if (reward.text.size() > 40) {
          text = FormatOwnedString("- %s...", reward.text.substr(0, 40).c_str());
        } else {
          text = FormatOwnedString("- %s", reward.text.c_str());
        }

        completed = static_cast<int>(progress->unknown_0004[i]) > 0;
      } else if (info.unknown_0064 == 1) {
        std::string count = FormatOwnedString("(%d/%d)", progress->unknown_0004[i], reward.unknown_0018);
        if (reward.text.size() + count.size() > 40) {
          text = FormatOwnedString("- %s...%s", reward.text.substr(0, 40 - count.size()).c_str(), count.c_str());
        } else {
          text = FormatOwnedString("- %s %s", reward.text.c_str(), count.c_str());
        }

        completed = static_cast<int>(progress->unknown_0004[i]) == reward.unknown_0018;
      }

      entry.unknown_0020.emplace_back(text, completed);
    }

    tracked.push_back(entry);
  }

  if (tracked.size() == 0) {
    g_windowMgr.CloseWindow(0x10F);
    return false;
  }

  return true;
}
