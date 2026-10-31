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
