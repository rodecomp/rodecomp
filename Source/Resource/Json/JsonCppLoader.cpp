#include "Resource/Json/JsonCppLoader.h"
#include "Resource/Json/JsonCppLoaderExternal.h"
#include "Resource/Json/JsonCppLoaderParsing.h"

#include <cstdarg>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>
#include <locale.h>

namespace jsoncons {

class json_exception {
public:
  virtual ~json_exception() noexcept = default;
  virtual const char* what() const noexcept = 0;
};

template <class Base, class Enable = void>
class json_runtime_error : public Base, public virtual json_exception {
public:
  explicit json_runtime_error(const std::string& text) noexcept;

  // INLINED
  ~json_runtime_error() noexcept {
  }

  // INLINED
  const char* what() const noexcept override {
    return Base::what();
  }
};

// OFFSET: 0x004EE600
// FUNCTION: fn_00330
// OFFSET: 0x004EE790
// FUNCTION: fn_00332
template <class Base, class Enable>
json_runtime_error<Base, Enable>::json_runtime_error(const std::string& text) noexcept : Base(text) {
}

namespace binary {

// OFFSET: 0x004FC250
// FUNCTION: fn_00671
__declspec(noinline) double decode_half(std::uint16_t half) {
  const std::int64_t exponent = (half >> 10) & 0x1f;
  const std::int64_t mantissa = half & 0x3ff;
  double value;
  if (exponent == 0) {
    value = std::ldexp(static_cast<double>(mantissa), -24);
  } else if (exponent != 31) {
    value = std::ldexp(static_cast<double>(mantissa) + 1024.0, static_cast<int>(exponent - 25));
  } else {
    value = mantissa == 0 ? std::numeric_limits<double>::infinity() : std::nan("");
  }

  return half & 0x8000 ? -value : value;
}

}

namespace detail {

class chars_to {
public:
  // INLINED
  chars_to() : locale_(_create_locale(LC_NUMERIC, "C")) {
  }

  // INLINED
  ~chars_to() noexcept {
    _free_locale(this->locale_);
  }

  double operator()(const char* text, std::size_t length) const;

private:
  /* 0000 */ _locale_t locale_;
};

// OFFSET: 0x004E0700
// FUNCTION: fn_00202
__declspec(noinline) double chars_to::operator()(const char* text, std::size_t length) const {
  char* end = nullptr;
  const double value = _strtod_l(text, &end, this->locale_);
  if (text == end) {
    throw json_runtime_error<std::invalid_argument>("Convert string to double failed");
  }

  return value;
}

}

}

// OFFSET: 0x0051E420
// FUNCTION: fn_01065
const char* JsonValue::AsCString() const {
  switch (this->type & 0x0f) {
    case short_string_value:
      return reinterpret_cast<const char*>(this) + 2;
    case long_string_value:
      return reinterpret_cast<const char*>(static_cast<const unsigned int*>(this->pointer)[1]);
    case json_const_pointer:
      return static_cast<const JsonValue*>(this->pointer)->AsCString();
    default:
      throw jsoncons::json_runtime_error<std::domain_error>("Not a cstring");
  }
}

// OFFSET: 0x0051E630
// FUNCTION: fn_01067
std::string_view JsonValue::AsStringView() const {
  switch (this->type & 0x0f) {
    case short_string_value:
      return std::string_view(reinterpret_cast<const char*>(this) + 2, this->type >> 4);
    case long_string_value: {
      const unsigned int* view = static_cast<const unsigned int*>(this->pointer);
      return std::string_view(reinterpret_cast<const char*>(view[1]), view[2]);
    }
    case json_const_pointer:
      return static_cast<const JsonValue*>(this->pointer)->AsStringView();
    default:
      throw jsoncons::json_runtime_error<std::domain_error>("Not a string");
  }
}

// OFFSET: 0x0051E4C0
// FUNCTION: fn_01066
double JsonValue::AsDouble() const {
  switch (this->type & 0x0f) {
    case short_string_value:
    case long_string_value: {
      jsoncons::detail::chars_to toDouble;
      return toDouble(this->AsCString(), this->AsStringView().length());
    }
    case half_value:
      return jsoncons::binary::decode_half(this->smallValue);
    case double_value: {
      double value;
      std::memcpy(&value, &this->integer, sizeof(value));
      return value;
    }
    case int64_value:
      return static_cast<double>(this->integer);
    case uint64_value:
      return static_cast<double>(static_cast<unsigned long long>(this->integer));
    case json_const_pointer:
      return static_cast<const JsonValue*>(this->pointer)->AsDouble();
    default:
      throw jsoncons::json_runtime_error<std::invalid_argument>("Not a double");
  }
}

struct JsonValueLocal {
  JsonValue value;

  // INLINED
  ~JsonValueLocal() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->value);
  }
};

struct JsonCopiedValue {
  JsonValue value;

  // INLINED
  explicit JsonCopiedValue(const JsonValue& other) {
    reinterpret_cast<void(__thiscall*)(JsonValue*, const JsonValue*)>(JsonCopyConstruct)(&this->value, &other);
  }

  // INLINED
  ~JsonCopiedValue() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->value);
  }
};

// OFFSET: 0x005058B0

// OFFSET: 0x004F3820
// FUNCTION: fn_00484
JsonValue::ValueType JsonCppLoader::GetValueType(const JsonValue& value) {
  const JsonValue::ValueType type = value.Type();
  if (type == JsonValue::UINT64_VALUE) {
    return JsonValue::INT64_VALUE;
  }

  return type;
}

// OFFSET: 0x00509940
// FUNCTION: fn_00891
bool JsonCppLoader::GetMapStringBoolValue(const JsonValue& node, const std::string& key, std::map<std::string, bool>* out) {
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::OBJECT_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject* members = value.value.ObjectRange();
  if (members == nullptr) {
    return true;
  }
  for (const JsonMember& member : *members) {
    std::string name = member.first;
    JsonCopiedValue element(member.second);
    if (element.value.GetType() != JsonValue::BOOL_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. boolValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    const std::pair<std::string, bool> item(name, element.value.AsBool());
    if (!out->insert(item).second) {
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


struct JsonCppLoaderJsonOptionsGuard {
  JsonCppLoaderJsonOptions* options;

  // INLINED
  ~JsonCppLoaderJsonOptionsGuard() {
    reinterpret_cast<void(__thiscall*)(JsonCppLoaderCommonJsonOptions*)>(JsonOptionsCommonDestroy)(&this->options->common);
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
    this->document = JsonDecodeBsonStream(&file, options);
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
    this->document = JsonParseStream(&file, options.options, JsonCppLoaderJsonHandler{});
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
    this->document = JsonDecodeBsonBuffer(&buffer, options);
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
    this->document = JsonParseString(buffer.data(), options.options, JsonCppLoaderJsonHandler{});
  } catch (const std::exception& exception) {
    this->Fail("error : JsonCppLoader::LoadJson failed (reason : %s)", exception.what());
    return false;
  }

  return true;
}

// OFFSET: 0x004EEC60
// FUNCTION: fn_00337
JsonCppLoader::JsonCppLoader() : loaded(false), succeeded(true) {
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
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::OBJECT_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject* members = value.value.ObjectRange();
  if (members == nullptr) {
    return true;
  }
  for (const JsonMember& member : *members) {
    std::string name = member.first;
    JsonCopiedValue element(member.second);
    if (element.value.GetType() != JsonValue::INT64_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. intValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    const std::map<std::string, int>::value_type item(name, element.value.AsInt());
    if (!out->insert(item).second) {
      this->Fail("중복키가 입력되었습니다. (keyName : %s)", key.c_str());
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00508BD0
// FUNCTION: fn_00889
bool JsonCppLoader::GetMapStringStringValue(const JsonValue& node, const std::string& key, std::map<std::string, std::string>* out) {
  JsonValueLocal value;
  this->GetValue(value.value, key, node, JsonValue::OBJECT_VALUE);
  if (value.value.IsNull() == true) {
    this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  const JsonObject* members = value.value.ObjectRange();
  if (members == nullptr) {
    return true;
  }
  for (const JsonMember& member : *members) {
    std::string name = member.first;
    JsonCopiedValue element(member.second);
    if (element.value.GetType() != JsonValue::STRING_VALUE) {
      this->Fail("잘못된 데이터가 입력되었습니다. stringValue 가 입력되어야 합니다. (keyName : %s)", key.c_str());
      return false;
    }

    const std::map<std::string, std::string>::value_type item(name, element.value.AsString());
    if (!out->insert(item).second) {
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
