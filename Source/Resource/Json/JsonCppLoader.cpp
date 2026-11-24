#include "Resource/Json/JsonCppLoader.h"

#include <jsoncons_ext/bson/bson.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fstream>

namespace {
// INLINED
  jsoncons::json_type NormalizeValueType(const jsoncons::json& value) {
    const jsoncons::json_type type = value.type();
    if (type == jsoncons::json_type::uint64_value) {
      return jsoncons::json_type::int64_value;
    }

    return type;
  }
}

// OFFSET: 0x004F3690
// FUNCTION: fn_00483
jsoncons::json JsonCppLoader::GetValue(const std::string& key, const jsoncons::json& node, jsoncons::json_type type) {
  if (node.is_null() == true) {
    return jsoncons::json::null();
  }

  if (!node.is_object()) {
    return jsoncons::json::null();
  }

  if (node.object_range().end() == node.find(key)) {
    return jsoncons::json::null();
  }

  jsoncons::json value = node.at(key);
  if (NormalizeValueType(value) != type) {
    return jsoncons::json::null();
  }
  return value;
}

// OFFSET: 0x004F3820
// FUNCTION: fn_00484
__declspec(noinline) jsoncons::json_type JsonCppLoader::GetValueType(const jsoncons::json& value) {
  return NormalizeValueType(value);
}

// OFFSET: 0x00509940
// FUNCTION: fn_00891
bool JsonCppLoader::GetMapStringBoolValue(const jsoncons::json& node, const std::string& key, std::map<std::string, bool>* out) {
  jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::object_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  for (auto& member : value.object_range()) {
    std::string name = member.key();
    jsoncons::json element = member.value();
    if (this->GetValueType(element) != jsoncons::json_type::bool_value) {
      this->Fail("잘못된 데이터가 입력되었습니다. boolValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    const auto inserted = out->insert(std::pair<std::string, bool>(name, element.as<bool>()));
    if (!inserted.second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x005086D0
// FUNCTION: fn_00887
bool JsonCppLoader::GetStringValue(const jsoncons::json& node, const std::string& key, std::string* out) {
  const jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::string_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.as<std::string>();
  return true;
}

// OFFSET: 0x004F3CC0
// FUNCTION: fn_00487
bool JsonCppLoader::LoadBson(const std::string& path) {
  try {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) {
      return false;
    }

    this->document = jsoncons::bson::decode_bson<jsoncons::json>(file);
    file.close();
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadBson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004F3ED0
// FUNCTION: fn_00489
bool JsonCppLoader::LoadJson(const std::string& path) {
  try {
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
      return false;
    }

    this->document = jsoncons::json::parse(file);
    file.close();
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadJson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004F3C70
// FUNCTION: fn_00486
bool JsonCppLoader::Load(const std::vector<unsigned char>& buffer, bool bson) {
  if (buffer.empty()) {
    this->Fail("error : JsonCppLoader::Load failed (reason : 비어있는 버퍼가 전달되었습니다.)");
    return false;
  }

  bool parsed;
  if (bson == true) {
    parsed = this->LoadBson(buffer);
  } else {
    parsed = this->LoadJson(buffer);
  }

  if (parsed) {
    this->loaded = true;
    return true;
  }

  return false;
}

// OFFSET: 0x004F3DF0
// FUNCTION: fn_00488
__declspec(noinline) bool JsonCppLoader::LoadBson(const std::vector<unsigned char>& buffer) {
  if (buffer.empty()) {
    this->Fail("비어있는 버퍼가 전달되었습니다.");
    return false;
  }

  try {
    this->document = jsoncons::bson::decode_bson<jsoncons::json>(buffer);
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadBson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004F4040
// FUNCTION: fn_00490
__declspec(noinline) bool JsonCppLoader::LoadJson(const std::vector<unsigned char>& buffer) {
  if (buffer.empty()) {
    this->Fail("비어있는 버퍼가 전달되었습니다.");
    return false;
  }

  if (buffer.back() != '\0') {
    this->Fail("문자열 끝이 존재하지 않습니다.");
    return false;
  }

  try {
    this->document = jsoncons::json::parse(reinterpret_cast<const char*>(buffer.data()));
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadJson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004EEC60
// FUNCTION: fn_00337
JsonCppLoader::JsonCppLoader() : document(jsoncons::json::null()), loaded(false), succeeded(true) {
}

// OFFSET: 0x004F2840
// FUNCTION: fn_00446
JsonCppLoader::~JsonCppLoader() {
}

// OFFSET: 0x004F4180
// FUNCTION: fn_00491
void JsonCppLoader::Fail(const char* format, ...) {
  char buffer[0x800];
  std::memset(buffer, 0, sizeof(buffer));

  va_list arguments;
  va_start(arguments, format);
  vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, arguments);
  va_end(arguments);

  this->error.assign(buffer, std::strlen(buffer));
  this->succeeded = false;
}

// OFFSET: 0x00508570
// FUNCTION: fn_00886
bool JsonCppLoader::GetIntValue(const jsoncons::json& node, const std::string& key, int* out) {
  const jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::int64_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.as<int>();
  return true;
}

// OFFSET: 0x005087D0
// FUNCTION: fn_00888
bool JsonCppLoader::GetMapStringIntValue(const jsoncons::json& node, const std::string& key, std::map<std::string, int>* out) {
  jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::object_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  for (auto& member : value.object_range()) {
    std::string name = member.key();
    jsoncons::json element = member.value();
    if (this->GetValueType(element) != jsoncons::json_type::int64_value) {
      this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    const auto inserted = out->insert(std::map<std::string, int>::value_type(name, element.as<int>()));
    if (!inserted.second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00508BD0
// FUNCTION: fn_00889
bool JsonCppLoader::GetMapStringStringValue(const jsoncons::json& node, const std::string& key, std::map<std::string, std::string>* out) {
  jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::object_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  for (auto& member : value.object_range()) {
    std::string name = member.key();
    jsoncons::json element = member.value();
    if (this->GetValueType(element) != jsoncons::json_type::string_value) {
      this->Fail("잘못된 데이터가 입력되었습니다. stringValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    const auto inserted = out->insert(std::map<std::string, std::string>::value_type(name, element.as<std::string>()));
    if (!inserted.second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00509130
// FUNCTION: fn_00890
bool JsonCppLoader::GetMapStringIntVectorValue(const jsoncons::json& node, const std::string& key, std::map<std::string, std::vector<unsigned int> >* out) {
  jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::object_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  for (auto& member : value.object_range()) {
    std::string name = member.key();
    jsoncons::json element = member.value();
    if (this->GetValueType(element) != jsoncons::json_type::array_value) {
      this->Fail("잘못된 데이터가 입력되었습니다. arrayValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    std::vector<unsigned int> values;
    for (const jsoncons::json& item : element.array_range()) {
      if (this->GetValueType(item) != jsoncons::json_type::int64_value) {
        this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
        return false;
      }

      values.push_back(item.as<int>());
    }

    const auto inserted = out->insert(std::pair<std::string, std::vector<unsigned int> >(name, values));
    if (!inserted.second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00509D40
// FUNCTION: fn_00892
__declspec(noinline) bool JsonCppLoader::GetIntVectorValue(const jsoncons::json& node, const std::string& key, std::vector<int>* out) {
  const jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::array_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  auto range = value.array_range();
  for (auto it = range.begin(); it != range.end(); ++it) {
    const jsoncons::json& item = *it;
    if (this->GetValueType(item) != jsoncons::json_type::int64_value) {
      this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    out->emplace_back(static_cast<unsigned int>(item.as<int>()));
  }

  return true;
}

// OFFSET: 0x00509F70
// FUNCTION: fn_00893
bool JsonCppLoader::GetBoolValue(const jsoncons::json& node, const std::string& key, bool* out) {
  const jsoncons::json value = this->GetValue(key, node, jsoncons::json_type::bool_value);
  if (value.is_null() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.as_bool();
  return true;
}

// OFFSET: 0x0050AE30
// FUNCTION: fn_00894
bool JsonCppLoader::GetReputeGroupData(std::map<int, ReputeGroupRecord>* out) {
  if (!this->loaded) {
    this->Fail("초기화가 되어야 합니다.");
    return false;
  }

  if (this->document.object_range().end() == this->document.find("ReputeGroup")) {
    this->Fail("키에 해당하는 데이터가 없습니다. (key : %s)", "ReputeGroup");
    return false;
  }

  for (const auto& member : this->document["ReputeGroup"].object_range()) {
    const std::string name = member.key();
    const jsoncons::json node = member.value();
    ReputeGroupRecord record;
    this->GetStringValue(node, "ID", &record.id);
    this->GetStringValue(node, "Name", &record.name);
    this->GetIntVectorValue(node, "ReputeList", &record.reputes);

    if (!out->insert({std::stoi(name), record}).second) {
      this->Fail("중복키가 입력되었습니다. (name : %s)", name.c_str());
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

  if (this->document.object_range().end() == this->document.find("reputeInfo")) {
    this->Fail("키에 해당하는 데이터가 없습니다. (key : %s)", "reputeInfo");
    return false;
  }

  for (const auto& member : this->document["reputeInfo"].object_range()) {
    const std::string name = member.key();
    const jsoncons::json node = member.value();
    ReputeInfoRecord record;
    this->GetStringValue(node, "Name", &record.name);
    this->GetIntValue(node, "MaxPoint_Positive", &record.maxPointPositive);
    this->GetIntValue(node, "MaxPoint_Negative", &record.maxPointNegative);
    this->GetStringValue(node, "Invisible", &record.invisible);

    if (!out->insert(std::pair<int, ReputeInfoRecord>(std::stoi(name), record)).second) {
      this->Fail("중복키가 입력되었습니다. (key : %s)", name.c_str());
      return false;
    }
  }

  return this->succeeded;
}
