#include "Utility/JsonCppLoader.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

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

// OFFSET: 0x00509940
// FUNCTION: fn_00891
bool JsonCppLoader::GetMapStringBoolValue(const JsonValue& node, const std::string& key, std::map<std::string, bool>* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::OBJECT_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject& members = value.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    if (member->second.GetType() != JsonValue::BOOL_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. boolValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    if (!out->insert(std::make_pair(member->first, member->second.AsBool())).second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00508570
// FUNCTION: fn_00886
bool JsonCppLoader::GetIntValue(const JsonValue& node, const std::string& key, int* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::INT64_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.AsInt();
  return true;
}

// OFFSET: 0x005086D0
// FUNCTION: fn_00887
bool JsonCppLoader::GetStringValue(const JsonValue& node, const std::string& key, std::string* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::STRING_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.AsString();
  return true;
}

// OFFSET: 0x005087D0
// FUNCTION: fn_00888
bool JsonCppLoader::GetMapStringIntValue(const JsonValue& node, const std::string& key, std::map<std::string, int>* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::OBJECT_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject& members = value.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    std::string name = member->first;
    JsonValue element = member->second;
    if (element.GetType() != JsonValue::INT64_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    if (!out->insert(std::make_pair(name, element.AsInt())).second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00508BD0
// FUNCTION: fn_00889
bool JsonCppLoader::GetMapStringStringValue(const JsonValue& node, const std::string& key, std::map<std::string, std::string>* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::OBJECT_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject& members = value.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    std::string name = member->first;
    JsonValue element = member->second;
    if (element.GetType() != JsonValue::STRING_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. stringValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    if (!out->insert(std::make_pair(name, element.AsString())).second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00509130
// FUNCTION: fn_00890
bool JsonCppLoader::GetMapStringIntVectorValue(const JsonValue& node, const std::string& key, std::map<std::string, std::vector<int> >* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::OBJECT_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject& members = value.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    std::string name = member->first;
    JsonValue element = member->second;
    if (element.GetType() != JsonValue::ARRAY_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. arrayValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    std::vector<int> values;
    const JsonArray& items = element.ArrayRange();
    for (JsonArray::const_iterator item = items.begin(); item != items.end(); ++item) {
      if (item->GetType() != JsonValue::INT64_VALUE) {
        this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
        return false;
      }

      values.push_back(item->AsInt());
    }

    if (!out->insert(std::make_pair(name, values)).second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00509D40
// FUNCTION: fn_00892
bool JsonCppLoader::GetIntVectorValue(const JsonValue& node, const std::string& key, std::vector<int>* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::ARRAY_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonArray& items = value.ArrayRange();
  for (JsonArray::const_iterator item = items.begin(); item != items.end(); ++item) {
    if (item->GetType() != JsonValue::INT64_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    out->push_back(item->AsInt());
  }

  return true;
}

// OFFSET: 0x00509F70
// FUNCTION: fn_00893
bool JsonCppLoader::GetBoolValue(const JsonValue& node, const std::string& key, bool* out) {
  JsonValue value;
  this->GetValue(value, key, node, JsonValue::BOOL_VALUE);
  if (value.IsNull()) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.AsBool();
  return true;
}
