#include "Contents/ReputeMgr.h"
#include "Contents/MessageTable.h"
#include "Network/TextEncoding.h"
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

// OFFSET: 0x0060E4A0
// FUNCTION: fn_04296
void CReputeMgr::OpenRepute(std::uint64_t groupId, std::uint64_t reputeId) {
  UIWindow* window = g_windowMgr.EnsureWindow(346);
  if (!window) {
    return;
  }

  if (groupId == 0) {
    window->SendMsg(nullptr, 22, reinterpret_cast<std::intptr_t>(LookupMessage(3829)), 0, 0, 0);
  } else {
    auto group = this->m_groups.find(groupId);
    if (group != this->m_groups.end()) {
      window->SendMsg(nullptr, 22, reinterpret_cast<std::intptr_t>(group->second.name.c_str()), 0, 0, 0);
    }
  }

  auto info = this->m_infos.find(reputeId);
  if (info != this->m_infos.end()) {
    info->second.updated = true;
    window->Invalidate();
  }
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

// OFFSET: 0x0060D440
// FUNCTION: fn_04287
void CReputeMgr::InfoInserter::operator()(const std::pair<const int, ReputeInfoRecord>& entry) const {
  ReputeInfo info{};
  info.name = FromUtf8(entry.second.name, g_textCodePage);
  info.unknown_0018 = entry.second.maxPointPositive;
  info.unknown_0020 = entry.second.maxPointNegative;
  const std::string& invisible = entry.second.invisible;
  if (invisible.size() == 12 && invisible[0] == 'V' && invisible[1] == 'I' && invisible[2] == 'S' && invisible[3] == 'I' &&
      invisible[4] == 'B' && invisible[5] == 'L' && invisible[6] == 'E' && invisible[7] == '_' && invisible[8] == 'T' &&
      invisible[9] == 'R' && invisible[10] == 'U' && invisible[11] == 'E') {
    info.visibility = REPUTE_VISIBLE;
  } else if (invisible.size() == 13 && invisible[0] == 'V' && invisible[1] == 'I' && invisible[2] == 'S' && invisible[3] == 'I' &&
             invisible[4] == 'B' && invisible[5] == 'L' && invisible[6] == 'E' && invisible[7] == '_' && invisible[8] == 'F' &&
             invisible[9] == 'A' && invisible[10] == 'L' && invisible[11] == 'S' && invisible[12] == 'E') {
    info.visibility = REPUTE_HIDDEN;
  } else {
    info.visibility = REPUTE_VISIBLE_WITH_POINT;
  }

  (*this->dest)[static_cast<std::uint64_t>(entry.first)] = std::move(info);
}
