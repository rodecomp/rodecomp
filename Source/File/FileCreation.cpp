#include "File/FileCreation.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Resource/Json/JsonCppLoader.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <direct.h>
#include <fstream>
#include <io.h>
#include <share.h>
#include <windows.h>

// OFFSET: 0x00AF6960
// FUNCTION: fn_18255
bool CreateDirectoryPath(std::string path) {
  if (path.empty()) {
    return false;
  }

  std::string::size_type end = path.find("\\");
  std::string::size_type separator = end;
  std::string prefix;
  std::string remaining(path);
  while (path.size() != prefix.size()) {
    prefix = path.substr(0, end);
    remaining = remaining.substr(separator + 1);
    separator = remaining.find("\\");
    end = separator == std::string::npos ? path.size() : end + separator + 1;
    if (_access(prefix.c_str(), 0) != 0 && _mkdir(prefix.c_str()) != 0) {
      return false;
    }
  }
  return true;
}

// OFFSET: 0x00AF6EA0
// FUNCTION: fn_18256
std::string CreateUniqueFileName(const std::string& filename) {
  char drive[MAX_PATH]{};
  char directory[MAX_PATH]{};
  char name[MAX_PATH]{};
  char extension[MAX_PATH]{};
  _splitpath_s(filename.c_str(), drive, sizeof(drive), directory, sizeof(directory), name, sizeof(name), extension, sizeof(extension));
  std::string parent = FormatOwnedString("%s%s", drive, directory);
  CreateDirectoryPath(parent);

  int index = 0;
  std::string result(filename);
  while (_access(result.c_str(), 0) == 0) {
    result = static_cast<const std::string&>(FormatOwnedString("%s%s%s(%d)%s", drive, directory, name, ++index, extension));
  }
  std::ofstream output(result);
  output.close();
  return result;
}

struct CopiedJsonValue {
  JsonValue value;

  // INLINED
  explicit CopiedJsonValue(const JsonValue& other) {
    reinterpret_cast<void(__thiscall*)(JsonValue*, const JsonValue*)>(JsonCopyConstruct)(&this->value, &other);
  }

  // INLINED
  ~CopiedJsonValue() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->value);
  }
};

// INLINED
static __forceinline bool GetDoubleValue(JsonCppLoader* loader, const JsonValue& node, const std::string& key, double* out) {
  const JsonCppLoaderParsedValue value = loader->GetValue(key, node, JsonValue::DOUBLE_VALUE);
  if (value.IsNull() == true) {
    loader->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", key.c_str());
    return false;
  }

  *out = value.AsDouble();
  return true;
}

// OFFSET: 0x0050C2A0
// FUNCTION: fn_00896
bool JsonCppLoader::GetEZ2STREffect(std::map<std::string, EZ2STREffectRecord>* out) {
  if (!this->loaded) {
    this->Fail("초기화가 되어야 합니다.");
    return false;
  }

  const JsonValue* effects = nullptr;
  if (const JsonObject* root = this->document.ObjectRange()) {
    for (const JsonMember& entry : *root) {
      if (entry.first == "EZ2STREffect") {
        effects = &entry.second;
        break;
      }
    }
  }

  if (effects == nullptr) {
    this->Fail("키에 해당하는 데이터가 없습니다. (key : %s)", "EZ2STREffect");
    return false;
  }

  const JsonObject* members = effects->ObjectRange();
  const JsonMember* const end = members ? members->data() + members->size() : nullptr;
  for (const JsonMember* entry = members ? members->data() : nullptr; entry != end; ++entry) {
    const JsonMember& member = *entry;
    const std::string name = member.first;
    const CopiedJsonValue node(member.second);
    EZ2STREffectRecord record;
    this->GetStringValue(node.value, "FilePath", &record.name);
    GetDoubleValue(this, node.value, "PosX", &record.offsetX);
    GetDoubleValue(this, node.value, "PosY", &record.offsetY);
    this->GetBoolValue(node.value, "IsInfinite", &record.infinite);
    this->GetBoolValue(node.value, "IsFloor", &record.floor);
    this->GetBoolValue(node.value, "IsAboveUI", &record.aboveUi);
    this->GetStringValue(node.value, "SoundPath", &record.soundPath);
    this->GetBoolValue(node.value, "IsSoundLoop", &record.soundLoop);
    this->GetIntValue(node.value, "QuakeDelay", &record.delays[0]);
    this->GetIntValue(node.value, "StartDelayTime", &record.delays[1]);
    this->GetBoolValue(node.value, "IsToGround", &record.toGround);

    if (!out->insert(std::pair<std::string, EZ2STREffectRecord>(name, record)).second) {
      this->Fail("중복키가 입력되었습니다. (key : %s)", name.c_str());
      return false;
    }
  }

  return this->succeeded;
}
