#ifndef RAGNAROK_SOURCE_UTILITY_JSONCPPLOADER_H
#define RAGNAROK_SOURCE_UTILITY_JSONCPPLOADER_H

#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Contents/ReputeRecords.h"

struct EZ2STREffectRecord;

class JsonValue;

typedef std::pair<std::string, JsonValue> JsonMember;
typedef std::vector<JsonMember> JsonObject;
typedef std::vector<JsonValue> JsonArray;

class JsonValue {
public:
  enum ValueType {
    NULL_VALUE = 0,
    BOOL_VALUE = 1,
    INT64_VALUE = 2,
    HALF_VALUE = 4,
    DOUBLE_VALUE = 5,
    STRING_VALUE = 6,
    BYTE_STRING_VALUE = 7,
    ARRAY_VALUE = 8,
    OBJECT_VALUE = 9,
  };

  int GetType() const;
  bool IsNull() const;
  bool IsObject() const;
  bool AsBool() const;
  int AsInt() const;
  std::string AsString() const;
  const JsonObject& GetObject() const;
  const JsonArray& GetArray() const;
  const JsonArray& ArrayRange() const;

  /* 0000 */ unsigned char type;
  /* 0001 */ unsigned char unknown_0001;
  /* 0002 */ unsigned short smallValue;
  /* 0004 */ void* pointer;
  /* 0008 */ long long integer;

private:
  const JsonValue* Dereference() const;
};

inline const JsonValue* JsonValue::Dereference() const {
  const JsonValue* value = this;
  while ((value->type & 0x0f) == 0x0c) {
    value = static_cast<const JsonValue*>(value->pointer);
  }

  return value;
}

inline int JsonValue::GetType() const {
  const JsonValue* value = this->Dereference();
  switch (value->type & 0x0f) {
    case 0:
      return NULL_VALUE;
    case 1:
      return BOOL_VALUE;
    case 2:
    case 3:
      return INT64_VALUE;
    case 4:
      return HALF_VALUE;
    case 5:
      return DOUBLE_VALUE;
    case 6:
    case 7:
      return STRING_VALUE;
    case 8:
      return BYTE_STRING_VALUE;
    case 9:
      return ARRAY_VALUE;
    case 10:
    case 11:
      return OBJECT_VALUE;
    default:
      return NULL_VALUE;
  }
}

inline bool JsonValue::IsNull() const {
  return (this->Dereference()->type & 0x0f) == 0;
}

inline bool JsonValue::IsObject() const {
  unsigned char tag = this->Dereference()->type & 0x0f;
  return tag == 10 || tag == 11;
}

inline bool JsonValue::AsBool() const {
  const JsonValue* value = this->Dereference();
  switch (value->type & 0x0f) {
    case 1:
      return value->smallValue != 0;
    case 2:
    case 3:
      return value->integer != 0;
    default:
      throw std::domain_error("Not a bool");
  }
}

inline std::string JsonValue::AsString() const {
  const JsonValue* value = this->Dereference();
  switch (value->type & 0x0f) {
    case 6:
      return std::string(reinterpret_cast<const char*>(value) + 2, value->type >> 4);
    case 7: {
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
    case 1:
      return value->smallValue != 0;
    case 2:
    case 3:
      return static_cast<int>(value->integer);
    case 4:
      return value->smallValue;
    case 5:
      std::memcpy(&real, &value->integer, sizeof(real));
      return static_cast<int>(real);
    case 6:
    case 7:
      return std::atoi(this->AsString().c_str());
    default:
      throw std::domain_error("Not an integer");
  }
}

inline const JsonObject& JsonValue::GetObject() const {
  return *static_cast<const JsonObject*>(this->Dereference()->pointer);
}

inline const JsonArray& JsonValue::GetArray() const {
  return *static_cast<const JsonArray*>(this->Dereference()->pointer);
}

inline const JsonArray& JsonValue::ArrayRange() const {
  if ((this->type & 0x0f) != 9) {
    throw std::domain_error("Not an array");
  }

  return *static_cast<const JsonArray*>(this->pointer);
}

class JsonCppLoader {
public:
  JsonValue& GetValue(JsonValue& out, const std::string& key, const JsonValue& node, int expectedType);
  void Fail(const char* format, ...);

  bool GetIntValue(const JsonValue& node, const std::string& key, int* out);
  bool GetStringValue(const JsonValue& node, const std::string& key, std::string* out);
  bool GetMapStringIntValue(const JsonValue& node, const std::string& key, std::map<std::string, int>* out);
  bool GetMapStringStringValue(const JsonValue& node, const std::string& key, std::map<std::string, std::string>* out);
  bool GetMapStringIntVectorValue(const JsonValue& node, const std::string& key, std::map<std::string, std::vector<int> >* out);
  bool GetMapStringBoolValue(const JsonValue& node, const std::string& key, std::map<std::string, bool>* out);
  bool GetIntVectorValue(const JsonValue& node, const std::string& key, std::vector<int>* out);
  bool GetBoolValue(const JsonValue& node, const std::string& key, bool* out);
  bool sub_4F3C70(std::vector<unsigned char>& buffer, bool bson);
  bool sub_4F3DF0(std::vector<unsigned char>& buffer);
  bool sub_4F4040(std::vector<unsigned char>& buffer);
  bool sub_50C2A0(std::map<std::string, EZ2STREffectRecord>* out);
  bool sub_50AE30(std::map<int, ReputeGroupRecord>* out);
  bool sub_50B830(std::map<int, ReputeInfoRecord>* out);

  /* 0000 */ unsigned char unknown_0000[0x08];
  /* 0008 */ JsonValue document;
  /* 0018 */ std::string error;
  /* 0030 */ bool loaded;
  /* 0031 */ bool succeeded;
};

inline JsonValue& JsonCppLoader::GetValue(JsonValue& out, const std::string& key, const JsonValue& node, int expectedType) {
  out = JsonValue();
  if (node.IsNull() || !node.IsObject()) {
    return out;
  }

  const JsonObject& members = node.GetObject();
  JsonObject::const_iterator member = members.begin();
  for (; member != members.end(); ++member) {
    if (member->first == key) {
      break;
    }
  }

  if (member == members.end()) {
    return out;
  }

  if (member->second.GetType() == expectedType) {
    out = member->second;
  }

  return out;
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(JsonValue) == 0x10);
static_assert(offsetof(JsonValue, smallValue) == 0x02);
static_assert(offsetof(JsonValue, pointer) == 0x04);
static_assert(offsetof(JsonValue, integer) == 0x08);
static_assert(offsetof(JsonCppLoader, document) == 0x08);
static_assert(offsetof(JsonCppLoader, error) == 0x18);
static_assert(offsetof(JsonCppLoader, loaded) == 0x30);
static_assert(offsetof(JsonCppLoader, succeeded) == 0x31);
#endif

#endif
