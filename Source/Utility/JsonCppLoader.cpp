#include "Utility/JsonCppLoader.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
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


#include <iosfwd>

struct JsonCppLoaderBsonOptions {
  unsigned char storage[16];
};

struct JsonCppLoaderJsonOptions {
  unsigned char storage[248];
};

extern "C" void fn_00198();

struct JsonCppLoaderJsonHandler {
  unsigned char storage[40];

  // INLINED
  JsonCppLoaderJsonHandler() {
    reinterpret_cast<void(__thiscall*)(JsonCppLoaderJsonHandler*, JsonCppLoaderJsonHandler*)>(fn_00198)(this, this);
  }
};

struct JsonCppLoaderFileStream {
  unsigned char unknown_0000[0x5C];
  void* file;
  unsigned char unknown_0060[0x30];
};
static_assert(sizeof(JsonCppLoaderFileStream) == 0x90);

extern "C" {
void fn_00318();
void fn_00414();
void fn_00646();
void fn_00349();
void fn_00321();
void fn_00368();
void fn_00198();
void* __cdecl fn_00272(JsonValue* out, std::ifstream* file, JsonCppLoaderBsonOptions* options);
void* __cdecl fn_00273(JsonValue* out, const std::vector<unsigned char>* buffer, JsonCppLoaderBsonOptions* options);
void* __cdecl fn_00733(JsonValue* out, std::ifstream* file, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
void* __cdecl fn_00734(JsonValue* out, const unsigned char* data, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
void fn_00769();
void fn_00367();
}

// OFFSET: 0x004F3CC0

// OFFSET: 0x004F3C70
// FUNCTION: fn_00486
bool JsonCppLoader::sub_4F3C70(std::vector<unsigned char>& buffer, bool bson) {
  if (buffer.empty()) {
    this->Fail("error : JsonCppLoader::Load failed (reason : 비어있는 버퍼가 전달되었습니다.)");
    return false;
  }

  bool parsed;
  if (bson) {
    parsed = this->sub_4F3DF0(buffer);
  } else {
    parsed = this->sub_4F4040(buffer);
  }

  if (parsed) {
    this->loaded = true;
    return true;
  }

  return false;
}

// OFFSET: 0x004F3DF0

// OFFSET: 0x004F3ED0

// OFFSET: 0x004F4040
// FUNCTION: fn_00490
__declspec(noinline) bool JsonCppLoader::sub_4F4040(std::vector<unsigned char>& buffer) {
  if (buffer.empty()) {
    this->Fail("비어있는 버퍼가 전달되었습니다.");
    return false;
  }

  if (buffer.back() != '\0') {
    this->Fail("문자열 끝이 존재하지 않습니다.");
    return false;
  }

  JsonCppLoaderJsonOptions optionStorage{};
  JsonCppLoaderJsonOptions* options = reinterpret_cast<JsonCppLoaderJsonOptions*(__thiscall*)(JsonCppLoaderJsonOptions*, int)>(fn_00321)(&optionStorage, 1);

  JsonValue parsed{};
  JsonValue* result;
  try {
    result = static_cast<JsonValue*>(fn_00734(&parsed, buffer.data(), options, JsonCppLoaderJsonHandler{}));
    if (&this->document != result) {
      reinterpret_cast<void(__thiscall*)(JsonValue*, JsonValue*)>(fn_00769)(&this->document, result);
    }
  } catch (const std::exception& exception) {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(fn_00367)(&parsed);
    reinterpret_cast<void(__thiscall*)(JsonCppLoaderJsonOptions*)>(fn_00368)(reinterpret_cast<JsonCppLoaderJsonOptions*>(reinterpret_cast<unsigned char*>(options) + 8));
    this->Fail("error : JsonCppLoader::LoadJson failed (reason : %s)", exception.what());
    return false;
  }
  reinterpret_cast<void(__thiscall*)(JsonValue*)>(fn_00367)(&parsed);
  reinterpret_cast<void(__thiscall*)(JsonCppLoaderJsonOptions*)>(fn_00368)(reinterpret_cast<JsonCppLoaderJsonOptions*>(reinterpret_cast<unsigned char*>(options) + 8));
  return true;
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
