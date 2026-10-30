#include "Contents/ReputeMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <utility>

int sub_AD6360(const char* text, unsigned int type);

// OFFSET: 0x0060C5D0
// FUNCTION: fn_04283
CReputeMgr::CReputeMgr() {
}

// OFFSET: 0x0060D8B0
// FUNCTION: fn_04290
std::vector<std::string> CReputeMgr::GetGroupNames() const {
  std::vector<std::string> names;
  std::for_each(this->m_groups.begin(), this->m_groups.end(), [&names](std::pair<const std::uint64_t, ReputeGroup> group) {
    names.push_back(group.second.name);
  });
  return names;
}

// OFFSET: 0x0060D930
// FUNCTION: fn_04291
std::vector<const ReputeInfo*> CReputeMgr::GetGroupReputes(const std::string& groupName) const {
  std::vector<const ReputeInfo*> reputes;
  for (auto group = this->m_groups.begin(); group != this->m_groups.end(); ++group) {
    if (group->second.name == groupName) {
      for (auto id = group->second.reputes.begin(); id != group->second.reputes.end(); ++id) {
        auto found = this->m_infos.find(*id);
        if (found == this->m_infos.end()) {
          continue;
        }

        const ReputeInfo& info = found->second;
        if (info.visibility == REPUTE_VISIBLE || (info.visibility == REPUTE_VISIBLE_WITH_POINT && info.point != 0)) {
          reputes.push_back(&info);
        }
      }
    }
  }

  return reputes;
}

// OFFSET: 0x0060DAB0
// FUNCTION: fn_04292
std::vector<const ReputeInfo*> CReputeMgr::GetAllReputes() const {
  std::vector<const ReputeInfo*> reputes;
  for (auto group = this->m_groups.begin(); group != this->m_groups.end(); ++group) {
    for (auto id = group->second.reputes.begin(); id != group->second.reputes.end(); ++id) {
      auto found = this->m_infos.find(*id);
      if (found == this->m_infos.end()) {
        continue;
      }

      const ReputeInfo& info = found->second;
      if (info.visibility == REPUTE_VISIBLE || (info.visibility == REPUTE_VISIBLE_WITH_POINT && info.point != 0)) {
        reputes.push_back(&info);
      }
    }
  }

  return reputes;
}

// OFFSET: 0x0060DBF0
// FUNCTION: fn_04293
bool CReputeMgr::sub_60DBF0() {
  if (this->sub_60E090(this->m_infos) && this->sub_60DC70(this->m_groups)) {
    return true;
  }

  sub_AD6360("Failed : LoadReputeInfoData", 0);
  return false;
}

// OFFSET: 0x0060E5B0
// FUNCTION: fn_04297
void CReputeMgr::SetPoint(std::uint64_t id, std::int64_t point) {
  auto found = this->m_infos.find(id);
  if (found != this->m_infos.end()) {
    found->second.point = point;
  }
}

// OFFSET: 0x0060E610
// FUNCTION: fn_04298
std::vector<const ReputeInfo*> CReputeMgr::FindReputes(const std::string& keyword) const {
  std::vector<const ReputeInfo*> reputes;
  for (auto entry = this->m_infos.begin(); entry != this->m_infos.end(); ++entry) {
    const ReputeInfo& info = entry->second;
    if (info.name.find(keyword) == std::string::npos) {
      continue;
    }

    if (info.visibility == REPUTE_VISIBLE || (info.visibility == REPUTE_VISIBLE_WITH_POINT && info.point != 0)) {
      reputes.push_back(&info);
    }
  }

  return reputes;
}

// OFFSET: 0x0060E830
// FUNCTION: fn_04299
void CReputeMgr::SetPointsReceived(bool received) {
  this->m_waitingForPoints = !received;
  if (received == true) {
    UIWindow* window = g_windowMgr.LookupWindow(346);
    if (window) {
      window->SendMsg(nullptr, 136, 0, 0, 0, 0);
    }
  }
}
