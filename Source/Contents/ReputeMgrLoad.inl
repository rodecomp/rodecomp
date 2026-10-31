#ifndef RAGNAROK_SOURCE_CONTENTS_REPUTEMGRLOAD_INL
#define RAGNAROK_SOURCE_CONTENTS_REPUTEMGRLOAD_INL

#include "Contents/ReputeMgr.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Utility/JsonCppLoader.h"

#include <algorithm>
#include <cstring>
#include <utility>

// OFFSET: 0x0050AE30
// FUNCTION: fn_00894
bool JsonCppLoader::sub_50AE30(std::map<int, ReputeGroupRecord>* out) {
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
bool JsonCppLoader::sub_50B830(std::map<int, ReputeInfoRecord>* out) {
  if (!this->loaded) {
    this->Fail("초기화가 되어야 합니다.");
    return false;
  }

  JsonValue infos;
  this->GetValue(infos, "reputeInfo", this->document, JsonValue::OBJECT_VALUE);
  if (infos.IsNull()) {
    this->Fail("키에 해당하는 데이터가 없습니다. (key : %s)", "reputeInfo");
    return false;
  }

  const JsonObject& members = infos.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    JsonValue nameValue;
    JsonValue maxPositive;
    JsonValue maxNegative;
    JsonValue invisibleValue;
    this->GetValue(nameValue, "Name", member->second, JsonValue::STRING_VALUE);
    if (nameValue.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "Name");
    }
    this->GetValue(maxPositive, "MaxPoint_Positive", member->second, JsonValue::INT64_VALUE);
    if (maxPositive.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "MaxPoint_Positive");
    }
    this->GetValue(maxNegative, "MaxPoint_Negative", member->second, JsonValue::INT64_VALUE);
    if (maxNegative.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "MaxPoint_Negative");
    }
    this->GetValue(invisibleValue, "Invisible", member->second, JsonValue::STRING_VALUE);
    if (invisibleValue.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "Invisible");
    }

    ReputeInfoRecord record;
    if (!nameValue.IsNull()) {
      record.name = nameValue.AsString();
    }
    record.maxPointPositive = maxPositive.IsNull() ? 0 : maxPositive.AsInt();
    record.maxPointNegative = maxNegative.IsNull() ? 0 : maxNegative.AsInt();
    if (!invisibleValue.IsNull()) {
      record.invisible = invisibleValue.AsString();
    }

    if (!out->insert(std::make_pair(std::stoi(member->first), record)).second) {
      this->Fail("중복키가 입력되었습니다. (key : %s)", member->first.c_str());
      return false;
    }
  }

  return this->succeeded;
}

// OFFSET: 0x0060D060

// OFFSET: 0x0060D440

// OFFSET: 0x0060DC70
// FUNCTION: fn_04294
bool CReputeMgr::sub_60DC70(std::map<std::uint64_t, ReputeGroup>& groups) {
  CFile file;
  if (!file.Open("ContentData\\Repute\\ReputeGroupData.bson", 0, true, false)) {
    return false;
  }

  const auto* data = static_cast<const unsigned char*>(file.GetData());
  std::vector<unsigned char> buffer(data, data + file.GetSize());

  JsonCppLoader loader;
  if (!loader.sub_4F3C70(buffer, true)) {
    return false;
  }

  std::map<int, ReputeGroupRecord> records;
  if (!loader.sub_50AE30(&records)) {
    return false;
  }

  groups.clear();
  std::for_each(records.begin(), records.end(), GroupInserter{this, &groups});
  return true;
}

// OFFSET: 0x0060E090
// FUNCTION: fn_04295
bool CReputeMgr::sub_60E090(std::map<std::uint64_t, ReputeInfo>& infos) {
  CFile file;
  if (!file.Open("ContentData\\Repute\\ReputeInfoData.bson", 0, true, false)) {
    return false;
  }

  const auto* data = static_cast<const unsigned char*>(file.GetData());
  std::vector<unsigned char> buffer(data, data + file.GetSize());

  JsonCppLoader loader;
  if (!loader.sub_4F3C70(buffer, true)) {
    return false;
  }

  std::map<int, ReputeInfoRecord> records;
  if (!loader.sub_50B830(&records)) {
    return false;
  }

  infos.clear();
  std::for_each(records.begin(), records.end(), InfoInserter{this, &infos});
  return true;
}

#endif
