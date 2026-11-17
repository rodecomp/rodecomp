#ifndef RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADER_H
#define RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADER_H

#include "Resource/Json/JsonCppLoaderExternal.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Contents/Repute/ReputeRecords.h"

struct EZ2STREffectRecord;

class JsonValue;

typedef std::pair<std::string, JsonValue> JsonMember;
typedef std::vector<JsonMember> JsonObject;
typedef std::vector<JsonValue> JsonArray;

template <class Elements>
struct JsonStorage {
  /* 0000 */ std::allocator<char> allocator;
  /* 0004 */ Elements elements;
};
static_assert(offsetof(JsonStorage<JsonObject>, elements) == 0x04);

class JsonValue {
public:
  enum ValueType : unsigned char {
    NULL_VALUE = 0,
    BOOL_VALUE = 1,
    INT64_VALUE = 2,
    UINT64_VALUE = 3,
    HALF_VALUE = 4,
    DOUBLE_VALUE = 5,
    STRING_VALUE = 6,
    BYTE_STRING_VALUE = 7,
    ARRAY_VALUE = 8,
    OBJECT_VALUE = 9,
  };

  enum StorageKind : unsigned char {
    null_value = 0,
    bool_value = 1,
    int64_value = 2,
    uint64_value = 3,
    half_value = 4,
    double_value = 5,
    short_string_value = 6,
    long_string_value = 7,
    byte_string_value = 8,
    array_value = 9,
    empty_object_value = 10,
    object_value = 11,
    json_const_pointer = 12,
  };

  ValueType Type() const;
  int GetType() const;
  bool IsNull() const;
  bool IsObject() const;
  bool AsBool() const;
  int AsInt() const;
  double AsDouble() const;
  const char* AsCString() const;
  std::string_view AsStringView() const;
  std::string AsString() const;
  const JsonObject& GetObject() const;
  const JsonArray& GetArray() const;
  const JsonArray& ArrayRange() const;
  const JsonObject* ObjectRange() const;
  const JsonValue* Find(std::string_view name) const;

  /* 0000 */ unsigned char type;
  /* 0001 */ unsigned char padding_0001;
  /* 0002 */ unsigned short smallValue;
  /* 0004 */ void* pointer;
  /* 0008 */ long long integer;

private:
  const JsonValue* Dereference() const;
};

inline const JsonValue* JsonValue::Dereference() const {
  const JsonValue* value = this;
  while ((value->type & 0x0f) == json_const_pointer) {
    value = static_cast<const JsonValue*>(value->pointer);
  }

  return value;
}

inline int JsonValue::GetType() const {
  const JsonValue* value = this->Dereference();
  switch (value->type & 0x0f) {
    case null_value:
      return NULL_VALUE;
    case bool_value:
      return BOOL_VALUE;
    case int64_value:
    case uint64_value:
      return INT64_VALUE;
    case half_value:
      return HALF_VALUE;
    case double_value:
      return DOUBLE_VALUE;
    case short_string_value:
    case long_string_value:
      return STRING_VALUE;
    case byte_string_value:
      return BYTE_STRING_VALUE;
    case array_value:
      return ARRAY_VALUE;
    case empty_object_value:
    case object_value:
      return OBJECT_VALUE;
    default:
      return NULL_VALUE;
  }
}

inline bool JsonValue::IsNull() const {
  switch (static_cast<StorageKind>(this->type & 0x0f)) {
    case null_value:
      return true;
    case json_const_pointer:
      return static_cast<const JsonValue*>(this->pointer)->IsNull();
    default:
      return false;
  }
}

inline bool JsonValue::IsObject() const {
  unsigned char tag = this->Dereference()->type & 0x0f;
  return tag == empty_object_value || tag == object_value;
}

inline bool JsonValue::AsBool() const {
  const JsonValue* value = this->Dereference();
  switch (value->type & 0x0f) {
    case bool_value:
      return value->smallValue != 0;
    case int64_value:
    case uint64_value:
      return value->integer != 0;
    default:
      throw std::domain_error("Not a bool");
  }
}

inline std::string JsonValue::AsString() const {
  const JsonValue* value = this->Dereference();
  switch (value->type & 0x0f) {
    case short_string_value:
      return std::string(reinterpret_cast<const char*>(value) + 2, value->type >> 4);
    case long_string_value: {
      const unsigned int* view = static_cast<const unsigned int*>(value->pointer);
      return std::string(reinterpret_cast<const char*>(view[1]), view[2]);
    }
    default:
      throw std::domain_error("Not a string");
  }
}

inline int JsonValue::AsInt() const {
  const JsonValue* value = this->Dereference();
  double real = 0.0;
  switch (value->type & 0x0f) {
    case bool_value:
      return value->smallValue != 0;
    case int64_value:
    case uint64_value:
      return static_cast<int>(value->integer);
    case half_value:
      return value->smallValue;
    case double_value:
      std::memcpy(&real, &value->integer, sizeof(real));
      return static_cast<int>(real);
    case short_string_value:
    case long_string_value:
      return std::atoi(this->AsString().c_str());
    default:
      throw std::domain_error("Not an integer");
  }
}

inline const JsonObject& JsonValue::GetObject() const {
  return static_cast<const JsonStorage<JsonObject>*>(this->Dereference()->pointer)->elements;
}

inline const JsonArray& JsonValue::GetArray() const {
  return static_cast<const JsonStorage<JsonArray>*>(this->Dereference()->pointer)->elements;
}

inline const JsonArray& JsonValue::ArrayRange() const {
  if ((this->type & 0x0f) != array_value) {
    throw std::domain_error("Not an array");
  }

  return static_cast<const JsonStorage<JsonArray>*>(this->pointer)->elements;
}

inline const JsonObject* JsonValue::ObjectRange() const {
  switch (this->type & 0x0f) {
    case empty_object_value:
      return nullptr;
    case object_value:
      return &static_cast<const JsonStorage<JsonObject>*>(this->pointer)->elements;
    default:
      throw std::domain_error("Not an object");
  }
}

// INLINED
inline const JsonValue* JsonValue::Find(std::string_view name) const {
  const JsonObject* members = this->Dereference()->ObjectRange();
  if (members == nullptr) {
    return nullptr;
  }

  const JsonObject::const_iterator member = std::lower_bound(
    members->begin(), members->end(), name, [](const JsonMember& entry, std::string_view key) { return std::string_view(entry.first).compare(key) < 0; }
  );
  if (member == members->end() || std::string_view(member->first) != name) {
    return nullptr;
  }

  return &member->second;
}

struct JsonCppLoaderParsedValue : JsonValue {
  // INLINED
  JsonCppLoaderParsedValue() {
    reinterpret_cast<void(__thiscall*)(JsonValue*, const JsonValue*)>(JsonCopyConstruct)(this, static_cast<const JsonValue*>(JsonNull()));
  }

  // INLINED
  explicit JsonCppLoaderParsedValue(const JsonValue& other) {
    reinterpret_cast<void(__thiscall*)(JsonValue*, const JsonValue*)>(JsonCopyConstruct)(this, &other);
  }

  // INLINED
  JsonCppLoaderParsedValue& operator=(JsonCppLoaderParsedValue&& other) {
    if (this != &other) {
      reinterpret_cast<void(__thiscall*)(JsonValue*, JsonValue*)>(JsonSwap)(this, &other);
    }

    return *this;
  }

  // INLINED
  ~JsonCppLoaderParsedValue() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(this);
  }
};

class JsonCppLoader {
public:
  JsonCppLoader();
  virtual ~JsonCppLoader();

  JsonCppLoaderParsedValue GetValue(const std::string& key, const JsonValue& node, JsonValue::ValueType expectedType);
  JsonValue::ValueType GetValueType(const JsonValue& value);
  void Fail(const char* format, ...);
  bool LoadBson(const std::string& path);

  bool GetIntValue(const JsonValue& node, const std::string& key, int* out);
  bool GetStringValue(const JsonValue& node, const std::string& key, std::string* out);
  bool GetMapStringIntValue(const JsonValue& node, const std::string& key, std::map<std::string, int>* out);
  bool GetMapStringStringValue(const JsonValue& node, const std::string& key, std::map<std::string, std::string>* out);
  bool GetMapStringIntVectorValue(const JsonValue& node, const std::string& key, std::map<std::string, std::vector<int> >* out);
  bool GetMapStringBoolValue(const JsonValue& node, const std::string& key, std::map<std::string, bool>* out);
  bool GetIntVectorValue(const JsonValue& node, const std::string& key, std::vector<int>* out);
  bool GetBoolValue(const JsonValue& node, const std::string& key, bool* out);
  bool Load(std::vector<unsigned char>& buffer, bool bson);
  bool LoadBson(std::vector<unsigned char>& buffer);
  bool LoadJson(const std::string& path);
  bool LoadJson(std::vector<unsigned char>& buffer);
  bool GetEZ2STREffect(std::map<std::string, EZ2STREffectRecord>* out);
  bool GetReputeGroupData(std::map<int, ReputeGroupRecord>* out);
  bool GetReputeInfoData(std::map<int, ReputeInfoRecord>* out);

private:
  JsonCppLoader(const JsonCppLoader&) = delete;

public:
  /* 0008 */ JsonCppLoaderParsedValue document;
  /* 0018 */ std::string error;
  /* 0030 */ bool loaded;
  /* 0031 */ bool succeeded;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(JsonValue) == 0x10);
static_assert(sizeof(JsonCppLoaderParsedValue) == 0x10);
static_assert(offsetof(JsonValue, smallValue) == 0x02);
static_assert(offsetof(JsonValue, pointer) == 0x04);
static_assert(offsetof(JsonValue, integer) == 0x08);
static_assert(offsetof(JsonCppLoader, document) == 0x08);
static_assert(offsetof(JsonCppLoader, error) == 0x18);
static_assert(offsetof(JsonCppLoader, loaded) == 0x30);
static_assert(offsetof(JsonCppLoader, succeeded) == 0x31);
static_assert(sizeof(JsonCppLoader) == 0x38);
#endif

#endif
