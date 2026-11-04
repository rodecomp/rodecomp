#include "File/FileCreation.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Utility/JsonCppLoader.h"
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

  std::string::size_type separator = path.find('\\');
  std::string::size_type end = separator;
  std::string prefix;
  std::string remaining(path);
  while (prefix.size() != path.size()) {
    prefix = path.substr(0, end);
    remaining = remaining.substr(separator + 1);
    separator = remaining.find('\\');
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
    result = FormatOwnedString("%s%s%s(%d)%s", drive, directory, name, ++index, extension);
  }
  std::ofstream output(result.c_str(), std::ios::out, _SH_DENYNO);
  output.close();
  return result;
}

// OFFSET: 0x0050C2A0
// FUNCTION: fn_00896
bool JsonCppLoader::sub_50C2A0(std::map<std::string, EZ2STREffectRecord>* out) {
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
  const JsonObject none;
  for (const JsonMember& member : members ? *members : none) {
    const std::string name = member.first;
    const JsonValue node = member.second;
    EZ2STREffectRecord record;
    this->GetStringValue(node, "FilePath", &record.name);

    JsonValue x;
    this->GetValue(x, "PosX", node, JsonValue::DOUBLE_VALUE);
    if (x.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "PosX");
    } else {
      record.offsetX = x.AsDouble();
    }

    JsonValue y;
    this->GetValue(y, "PosY", node, JsonValue::DOUBLE_VALUE);
    if (y.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "PosY");
    } else {
      record.offsetY = y.AsDouble();
    }

    this->GetBoolValue(node, "IsInfinite", &record.unknown_0028);
    this->GetBoolValue(node, "IsFloor", &record.unknown_0029);
    this->GetBoolValue(node, "IsAboveUI", &record.unknown_002A);
    this->GetStringValue(node, "SoundPath", &record.unknown_002C);
    this->GetBoolValue(node, "IsSoundLoop", &record.unknown_0044);
    this->GetIntValue(node, "QuakeDelay", &record.unknown_0048[0]);
    this->GetIntValue(node, "StartDelayTime", &record.unknown_0048[1]);
    this->GetBoolValue(node, "IsToGround", &record.unknown_0050);

    if (!out->insert(std::make_pair(name, record)).second) {
      this->Fail("중복키가 입력되었습니다. (key : %s)", name.c_str());
      return false;
    }
  }

  return this->succeeded;
}
