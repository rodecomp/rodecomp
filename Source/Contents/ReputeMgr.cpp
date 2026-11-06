#include "Contents/ReputeMgr.h"
#include "Client/Application/ErrorDialog.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Contents/MessageTable.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/JsonCppLoader.h"

#include <algorithm>
#include <cstring>
#include <utility>

// OFFSET: 0x0050AE30
// FUNCTION: fn_00894
bool JsonCppLoader::GetReputeGroupData(std::map<int, ReputeGroupRecord>* out) {
  if (!this->loaded) {
    this->Fail("초기화가 되어야 합니다.");
    return false;
  }

  JsonValue groups;
  this->GetValue(groups, "ReputeGroup", this->document, JsonValue::OBJECT_VALUE);
  if (groups.IsNull()) {
    this->Fail("키에 해당하는 데이터가 없습니다. (key : %s)", "ReputeGroup");
    return false;
  }

  const JsonObject& members = groups.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    JsonValue idValue;
    JsonValue nameValue;
    JsonValue listValue;
    this->GetValue(idValue, "ID", member->second, JsonValue::STRING_VALUE);
    if (idValue.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "ID");
    }
    this->GetValue(nameValue, "Name", member->second, JsonValue::STRING_VALUE);
    if (nameValue.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "Name");
    }
    this->GetValue(listValue, "ReputeList", member->second, JsonValue::ARRAY_VALUE);
    if (listValue.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "ReputeList");
    }

    ReputeGroupRecord record;
    if (!idValue.IsNull()) {
      record.id = idValue.AsString();
    }
    if (!nameValue.IsNull()) {
      record.name = nameValue.AsString();
    }
    if (!listValue.IsNull()) {
      const JsonArray& ids = listValue.GetArray();
      for (JsonArray::const_iterator id = ids.begin(); id != ids.end(); ++id) {
        if (id->GetType() != JsonValue::INT64_VALUE) {
          this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", "ReputeList");
          break;
        }
        record.reputes.push_back(id->AsInt());
      }
    }

    if (!out->insert(std::make_pair(std::stoi(member->first), record)).second) {
      this->Fail("중복키가 입력되었습니다. (name : %s)", member->first.c_str());
      return false;
    }
  }

  return this->succeeded;
}

// OFFSET: 0x0050B830
// FUNCTION: fn_00895
bool JsonCppLoader::GetReputeInfoData(std::map<int, ReputeInfoRecord>* out) {
  if (!this->loaded) {
    this->Fail("초기화가 되어야 합니다.");
    return false;
  }

  const JsonValue* infos = nullptr;
  if (const JsonObject* root = this->document.ObjectRange()) {
    for (const JsonMember& entry : *root) {
      if (entry.first == "reputeInfo") {
        infos = &entry.second;
        break;
      }
    }
  }

  if (infos == nullptr) {
    this->Fail("키에 해당하는 데이터가 없습니다. (key : %s)", "reputeInfo");
    return false;
  }

  const JsonObject* members = infos->ObjectRange();
  const JsonObject none;
  for (const JsonMember& member : members ? *members : none) {
    const std::string name = member.first;
    const JsonValue node = member.second;
    ReputeInfoRecord record;
    this->GetStringValue(node, "Name", &record.name);
    this->GetIntValue(node, "MaxPoint_Positive", &record.maxPointPositive);
    this->GetIntValue(node, "MaxPoint_Negative", &record.maxPointNegative);
    this->GetStringValue(node, "Invisible", &record.invisible);

    if (!out->insert(std::make_pair(std::stoi(name), record)).second) {
      this->Fail("중복키가 입력되었습니다. (key : %s)", name.c_str());
      return false;
    }
  }

  return this->succeeded;
}

// OFFSET: 0x0060DC70
// FUNCTION: fn_04294
bool CReputeMgr::LoadReputeGroupData(std::map<std::uint64_t, ReputeGroup>& groups) {
  std::string path = "ContentData\\Repute\\ReputeGroupData.bson";
  CFile file;
  if (!file.Open(path.c_str(), 0, true, false)) {
    return false;
  }

  const auto* data = static_cast<const unsigned char*>(file.GetData());
  std::vector<unsigned char> buffer(data, data + file.GetSize());

  JsonCppLoader loader;
  if (!loader.Load(buffer, true)) {
    return false;
  }

  std::map<int, ReputeGroupRecord> records;
  if (!loader.GetReputeGroupData(&records)) {
    return false;
  }

  groups.clear();
  for (std::pair<const int, ReputeGroupRecord> record : records) {
    GroupInserter{this, &groups}(record);
  }
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

  const auto* data = static_cast<const unsigned char*>(file.GetData());
  std::vector<unsigned char> buffer(data, data + file.GetSize());

  JsonCppLoader loader;
  if (!loader.Load(buffer, true)) {
    return false;
  }

  std::map<int, ReputeInfoRecord> records;
  if (!loader.GetReputeInfoData(&records)) {
    return false;
  }

  infos.clear();
  for (std::pair<const int, ReputeInfoRecord> record : records) {
    InfoInserter{this, &infos}(record);
  }
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
bool CReputeMgr::Load() {
  if (this->LoadReputeInfoData(this->m_infos) && this->LoadReputeGroupData(this->m_groups)) {
    return true;
  }

  ShowErrorDialog(GetRenderSystemHolder()->system, "Failed : LoadReputeInfoData", 0);
  return false;
}

// OFFSET: 0x0060E4A0
// FUNCTION: fn_04296
void CReputeMgr::OpenRepute(std::uint64_t groupId, std::uint64_t reputeId) {
  UIWindow* window = g_windowMgr.EnsureWindow(346);
  if (!window) {
    return;
  }

  const unsigned int groupIdLow = static_cast<unsigned int>(groupId);
  const unsigned int groupIdHigh = static_cast<unsigned int>(groupId >> 32);

  if (groupIdLow == 0 && groupIdHigh == 0) {
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
  ReputeInfo info;
  info.point = 0;
  info.name = FromUtf8(entry.second.name, g_textCodePage);
  info.unknown_0018 = entry.second.maxPointPositive;
  info.unknown_0020 = entry.second.maxPointNegative;
  const std::string invisible = FromUtf8(entry.second.invisible, g_textCodePage);
  const char* text = invisible.c_str();
  if (invisible.size() == 12 && text[0] == 'V' && text[1] == 'I' && text[2] == 'S' && text[3] == 'I' &&
      text[4] == 'B' && text[5] == 'L' && text[6] == 'E' && text[7] == '_' && text[8] == 'T' &&
      text[9] == 'R' && text[10] == 'U' && text[11] == 'E') {
    info.visibility = REPUTE_VISIBLE;
  } else if (invisible == "VISIBLE_FALSE") {
    info.visibility = REPUTE_HIDDEN;
  } else {
    info.visibility = REPUTE_VISIBLE_WITH_POINT;
  }

  (*this->dest)[entry.first] = info;
}

// OFFSET: 0x0060D060
// FUNCTION: fn_04286
void CReputeMgr::GroupInserter::operator()(const std::pair<const int, ReputeGroupRecord>& entry) const {
  ReputeGroup group;
  group.name = static_cast<const std::string&>(FromUtf8(entry.second.name, g_textCodePage));
  group.reputes = std::vector<std::uint64_t>(entry.second.reputes.begin(), entry.second.reputes.end());
  (*this->dest)[static_cast<std::uint64_t>(entry.first)] = group;
}
