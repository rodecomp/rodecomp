#include "Resource/Json/JsonCppLoader.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

extern "C" void JsonDestroy();

struct JsonValueLocal {
  JsonValue value;

  // INLINED
  ~JsonValueLocal() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->value);
  }
};

// OFFSET: 0x00509940
// FUNCTION: fn_00891
bool JsonCppLoader::GetMapStringBoolValue(const JsonValue& node, const std::string& key, std::map<std::string, bool>* out) {
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::OBJECT_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject& members = value.value.GetObject();
  for (JsonObject::const_iterator member = members.begin(); member != members.end(); ++member) {
    std::string name = member->first;
    JsonValue element = member->second;
    if (element.GetType() != JsonValue::BOOL_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. boolValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    if (!out->insert(std::make_pair(name, element.AsBool())).second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}


// OFFSET: 0x005086D0
// FUNCTION: fn_00887
bool JsonCppLoader::GetStringValue(const JsonValue& node, const std::string& key, std::string* out) {
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::STRING_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.value.AsString();
  return true;
}


#include <fstream>

struct JsonCppLoaderBsonOptions {
  void* storage[4];
};

struct JsonCppLoaderJsonOptions {
  unsigned char storage[248];
};

extern "C" void JsonDefaultParsingHandler();

struct JsonCppLoaderJsonHandler {
  unsigned char storage[40];

  // INLINED
  JsonCppLoaderJsonHandler() {
    reinterpret_cast<void(__thiscall*)(JsonCppLoaderJsonHandler*, JsonCppLoaderJsonHandler*)>(JsonDefaultParsingHandler)(this, this);
  }

  JsonCppLoaderJsonHandler(const JsonCppLoaderJsonHandler&) = delete;

  // INLINED
  ~JsonCppLoaderJsonHandler() {
  }
};

extern "C" {
void BsonDecodeOptionsConstruct();
void JsonDecodeOptionsConstruct();
void JsonOptionsCommonDestroy();
void JsonDefaultParsingHandler();
void JsonSwap();
void JsonDestroy();
void JsonCopyConstruct();
void* __cdecl JsonNull();
}

// INLINED
static JsonValue CopyNullJsonValue() {
  JsonValue value;
  reinterpret_cast<void(__thiscall*)(JsonValue*, const JsonValue*)>(JsonCopyConstruct)(&value, static_cast<const JsonValue*>(JsonNull()));
  return value;
}

struct JsonCppLoaderParsedValue {
  JsonValue value;

  // INLINED
  JsonCppLoaderParsedValue& operator=(JsonCppLoaderParsedValue&& other) {
    if (this != &other) {
      reinterpret_cast<void(__thiscall*)(JsonValue*, JsonValue*)>(JsonSwap)(&this->value, &other.value);
    }

    return *this;
  }

  // INLINED
  ~JsonCppLoaderParsedValue() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->value);
  }
};

extern "C" {
JsonCppLoaderParsedValue __cdecl JsonDecodeBsonStream(std::ifstream* file, JsonCppLoaderBsonOptions* options);
JsonCppLoaderParsedValue __cdecl JsonDecodeBsonBuffer(const std::vector<unsigned char>* buffer, JsonCppLoaderBsonOptions* options);
JsonCppLoaderParsedValue __cdecl JsonParseStream(std::ifstream* file, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
JsonCppLoaderParsedValue __cdecl JsonParseString(const unsigned char* data, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
}

struct JsonCppLoaderJsonOptionsGuard {
  JsonCppLoaderJsonOptions* options;

  // INLINED
  ~JsonCppLoaderJsonOptionsGuard() {
    reinterpret_cast<void(__thiscall*)(JsonCppLoaderJsonOptions*)>(JsonOptionsCommonDestroy)(reinterpret_cast<JsonCppLoaderJsonOptions*>(reinterpret_cast<unsigned char*>(this->options) + 8));
  }
};

// OFFSET: 0x004F3CC0
// FUNCTION: fn_00487
bool JsonCppLoader::LoadBson(const std::string& path) {
  try {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) {
      return false;
    }

    JsonCppLoaderBsonOptions optionStorage;
    JsonCppLoaderBsonOptions* options = reinterpret_cast<JsonCppLoaderBsonOptions*(__thiscall*)(JsonCppLoaderBsonOptions*, int)>(BsonDecodeOptionsConstruct)(&optionStorage, 1);
    reinterpret_cast<JsonCppLoaderParsedValue&>(this->document) = JsonDecodeBsonStream(&file, options);
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

    JsonCppLoaderJsonOptions optionStorage;
    JsonCppLoaderJsonOptionsGuard options{reinterpret_cast<JsonCppLoaderJsonOptions*(__thiscall*)(JsonCppLoaderJsonOptions*, int)>(JsonDecodeOptionsConstruct)(&optionStorage, 1)};
    reinterpret_cast<JsonCppLoaderParsedValue&>(this->document) = JsonParseStream(&file, options.options, JsonCppLoaderJsonHandler{});
    file.close();
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadJson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004F3C70
// FUNCTION: fn_00486
bool JsonCppLoader::Load(std::vector<unsigned char>& buffer, bool bson) {
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
__declspec(noinline) bool JsonCppLoader::LoadBson(std::vector<unsigned char>& buffer) {
  if (buffer.empty()) {
    this->Fail("비어있는 버퍼가 전달되었습니다.");
    return false;
  }

  try {
    JsonCppLoaderBsonOptions optionStorage;
    JsonCppLoaderBsonOptions* options = reinterpret_cast<JsonCppLoaderBsonOptions*(__thiscall*)(JsonCppLoaderBsonOptions*, int)>(BsonDecodeOptionsConstruct)(&optionStorage, 1);
    reinterpret_cast<JsonCppLoaderParsedValue&>(this->document) = JsonDecodeBsonBuffer(&buffer, options);
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadBson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004F4040
// FUNCTION: fn_00490
__declspec(noinline) bool JsonCppLoader::LoadJson(std::vector<unsigned char>& buffer) {
  if (buffer.empty()) {
    this->Fail("비어있는 버퍼가 전달되었습니다.");
    return false;
  }

  if (buffer.back() != '\0') {
    this->Fail("문자열 끝이 존재하지 않습니다.");
    return false;
  }

  try {
    JsonCppLoaderJsonOptions optionStorage;
    JsonCppLoaderJsonOptionsGuard options{reinterpret_cast<JsonCppLoaderJsonOptions*(__thiscall*)(JsonCppLoaderJsonOptions*, int)>(JsonDecodeOptionsConstruct)(&optionStorage, 1)};
    reinterpret_cast<JsonCppLoaderParsedValue&>(this->document) = JsonParseString(buffer.data(), options.options, JsonCppLoaderJsonHandler{});
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadJson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004EEC60
// FUNCTION: fn_00337
JsonCppLoader::JsonCppLoader() : document(CopyNullJsonValue()), loaded(false), succeeded(true) {
}

// OFFSET: 0x004F2840
// FUNCTION: fn_00446
JsonCppLoader::~JsonCppLoader() {
  reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->document);
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
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::INT64_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.value.AsInt();
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
  if (value.IsNull() == true) {
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
  if (value.IsNull() == true) {
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
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::ARRAY_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonArray& items = value.value.ArrayRange();
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
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::BOOL_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.value.AsBool();
  return true;
}
