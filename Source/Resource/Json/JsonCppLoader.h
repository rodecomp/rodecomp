#ifndef RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADER_H
#define RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADER_H

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include <jsoncons/json.hpp>

#include "Contents/Repute/ReputeRecords.h"

struct EZ2STREffectRecord;

class JsonCppLoader {
public:
  JsonCppLoader();
  virtual ~JsonCppLoader();

  jsoncons::json GetValue(const std::string& key, const jsoncons::json& node, jsoncons::json_type type);
  jsoncons::json_type GetValueType(const jsoncons::json& value);
  void Fail(const char* format, ...);
  bool LoadBson(const std::string& path);

  bool GetIntValue(const jsoncons::json& node, const std::string& key, int* out);
  bool GetStringValue(const jsoncons::json& node, const std::string& key, std::string* out);
  bool GetMapStringIntValue(const jsoncons::json& node, const std::string& key, std::map<std::string, int>* out);
  bool GetMapStringStringValue(const jsoncons::json& node, const std::string& key, std::map<std::string, std::string>* out);
  bool GetMapStringIntVectorValue(const jsoncons::json& node, const std::string& key, std::map<std::string, std::vector<unsigned int> >* out);
  bool GetMapStringBoolValue(const jsoncons::json& node, const std::string& key, std::map<std::string, bool>* out);
  bool GetIntVectorValue(const jsoncons::json& node, const std::string& key, std::vector<int>* out);
  bool GetBoolValue(const jsoncons::json& node, const std::string& key, bool* out);
  bool Load(const std::vector<unsigned char>& buffer, bool bson);
  bool LoadBson(const std::vector<unsigned char>& buffer);
  bool LoadJson(const std::string& path);
  bool LoadJson(const std::vector<unsigned char>& buffer);
  bool GetEZ2STREffect(std::map<std::string, EZ2STREffectRecord>* out);
  bool GetReputeGroupData(std::map<int, ReputeGroupRecord>* out);
  bool GetReputeInfoData(std::map<int, ReputeInfoRecord>* out);

private:
  JsonCppLoader(const JsonCppLoader&) = delete;

public:
  /* 0008 */ jsoncons::json document;
  /* 0018 */ std::string error;
  /* 0030 */ bool loaded;
  /* 0031 */ bool succeeded;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(jsoncons::json) == 0x10);
static_assert(offsetof(JsonCppLoader, document) == 0x08);
static_assert(offsetof(JsonCppLoader, error) == 0x18);
static_assert(offsetof(JsonCppLoader, loaded) == 0x30);
static_assert(offsetof(JsonCppLoader, succeeded) == 0x31);
static_assert(sizeof(JsonCppLoader) == 0x38);
#endif

#endif
