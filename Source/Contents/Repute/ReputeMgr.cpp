#include "Contents/Repute/ReputeMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Application/ErrorDialog.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Resource/Json/JsonCppLoader.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstring>
#include <utility>

// OFFSET: 0x0060DC70
// FUNCTION: fn_04294
bool CReputeMgr::LoadReputeGroupData(std::map<std::uint64_t, ReputeGroup>& groups) {
  std::string path = "ContentData\\Repute\\ReputeGroupData.bson";
  CFile file;
  if (!file.Open(path.c_str(), 0, true, false)) {
    return false;
  }

  JsonCppLoader loader;
  const auto* data = static_cast<const unsigned char*>(file.GetData());
  if (!loader.Load(std::vector<unsigned char>(data, data + file.GetSize()), true)) {
    return false;
  }

  std::map<int, ReputeGroupRecord> records;
  if (!loader.GetReputeGroupData(&records)) {
    return false;
  }

  groups.clear();
  std::for_each(records.begin(), records.end(), GroupInserter{this, &groups});
  return true;
}

// OFFSET: 0x0060E090
// FUNCTION: fn_04295
bool CReputeMgr::LoadReputeInfoData(std::map<std::uint64_t, ReputeInfo>& infos) {
  std::string path = "ContentData\\Repute\\ReputeInfoData.bson";
  CFile file;
  if (!file.Open(path.c_str(), 0, true, false)) {
    return false;
  }

  JsonCppLoader loader;
  const auto* data = static_cast<const unsigned char*>(file.GetData());
  if (!loader.Load(std::vector<unsigned char>(data, data + file.GetSize()), true)) {
    return false;
  }

  std::map<int, ReputeInfoRecord> records;
  if (!loader.GetReputeInfoData(&records)) {
    return false;
  }

  infos.clear();
  std::for_each(records.begin(), records.end(), InfoInserter{this, &infos});
  return true;
}

// OFFSET: 0x0060C5D0
// FUNCTION: fn_04283
CReputeMgr::CReputeMgr() {
}

// OFFSET: 0x0060D8B0
// FUNCTION: fn_04290
std::vector<std::string> CReputeMgr::GetGroupNames() const {
  std::vector<std::string> names;
  std::for_each(this->m_groups.begin(), this->m_groups.end(), [&names](const std::pair<const std::uint64_t, ReputeGroup>& entry) {
    std::pair<const std::uint64_t, ReputeGroup> group = entry;
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

        const ReputeVisibility visibility = found->second.visibility;
        if (REPUTE_VISIBLE == visibility || (visibility == REPUTE_VISIBLE_WITH_POINT && 0 != found->second.point)) {
          reputes.push_back(&found->second);
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

      const ReputeVisibility visibility = found->second.visibility;
      if (REPUTE_VISIBLE == visibility || (visibility == REPUTE_VISIBLE_WITH_POINT && 0 != found->second.point)) {
        reputes.push_back(&found->second);
      }
    }
  }

  return reputes;
}

// OFFSET: 0x0060DC30
// FUNCTION: fn_46721
bool CReputeMgr::Load(std::map<std::uint64_t, ReputeInfo>& infos, std::map<std::uint64_t, ReputeGroup>& groups) {
  if (!this->LoadReputeInfoData(infos)) {
    ShowErrorDialog("Failed : LoadReputeInfoData", 0);
    return false;
  }
  if (!this->LoadReputeGroupData(groups)) {
    ShowErrorDialog("Failed : LoadReputeInfoData", 0);
    return false;
  }
  return true;
}

// OFFSET: 0x0060DBF0
// FUNCTION: fn_04293
bool CReputeMgr::Load() {
  return this->Load(this->m_infos, this->m_groups);
}

// OFFSET: 0x0060E4A0
// FUNCTION: fn_04296
void CReputeMgr::OpenRepute(std::uint64_t groupId, std::uint64_t reputeId) {
  UIWindow* window = g_windowMgr.EnsureWindow(WID_REPUTEWND);
  if (!window) {
    return;
  }

  const unsigned int groupIdLow = static_cast<unsigned int>(groupId);
  const unsigned int groupIdHigh = static_cast<unsigned int>(groupId >> 32);

  if (groupIdLow == 0 && groupIdHigh == 0) {
    window->SendMsg(nullptr, UM_TAB, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REPUTE_TOTAL)), 0, 0, 0);
  } else {
    auto group = this->m_groups.find(groupId);
    if (group != this->m_groups.end()) {
      window->SendMsg(nullptr, UM_TAB, reinterpret_cast<std::intptr_t>(group->second.name.c_str()), 0, 0, 0);
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
    if (entry->second.name.find(keyword) == std::string::npos) {
      continue;
    }

    if (REPUTE_VISIBLE == entry->second.visibility || (REPUTE_VISIBLE_WITH_POINT == entry->second.visibility && 0 != entry->second.point)) {
      reputes.push_back(&entry->second);
    }
  }

  return reputes;
}

// OFFSET: 0x0060E830
// FUNCTION: fn_04299
void CReputeMgr::SetPointsReceived(bool received) {
  this->m_waitingForPoints = !received;
  if (received == true) {
    UIWindow* window = g_windowMgr.LookupWindow(WID_REPUTEWND);
    if (window) {
      window->SendMsg(nullptr, UM_REFRESH_REPUTE_FILTER, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0060D440
// FUNCTION: fn_04287
void CReputeMgr::InfoInserter::operator()(const std::pair<int, ReputeInfoRecord>& entry) const {
  ReputeInfo info;
  info.name = static_cast<const std::string&>(FromUtf8(entry.second.name, g_textCodePage));
  info.maxPointPositive = entry.second.maxPointPositive;
  info.maxPointNegative = entry.second.maxPointNegative;
  const std::string invisible = FromUtf8(entry.second.invisible, g_textCodePage);
  ReputeVisibility visibility;
  const uint32_t* words = reinterpret_cast<const uint32_t*>(invisible.data());
  if (invisible.size() == 12 && words[0] == 0x49534956 && words[1] == 0x5f454c42 && words[2] == 0x45555254) {
    visibility = REPUTE_VISIBLE;
  } else if (invisible == "VISIBLE_FALSE") {
    visibility = REPUTE_HIDDEN;
  } else {
    visibility = REPUTE_VISIBLE_WITH_POINT;
  }
  info.visibility = visibility;

  (*this->dest)[entry.first] = info;
}

// OFFSET: 0x0060D060
// FUNCTION: fn_04286
void CReputeMgr::GroupInserter::operator()(const std::pair<int, ReputeGroupRecord>& entry) const {
  ReputeGroup group;
  group.name = static_cast<const std::string&>(FromUtf8(entry.second.name, g_textCodePage));
  group.reputes = std::vector<std::uint64_t>(entry.second.reputes.begin(), entry.second.reputes.end());
  (*this->dest)[static_cast<std::uint64_t>(entry.first)] = group;
}
