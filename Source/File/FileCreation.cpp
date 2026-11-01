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
    std::string filePath;
    std::string soundPath;
    double posX;
    double posY;
    bool isInfinite;
    bool isFloor;
    bool isAboveUI;
    bool isSoundLoop;
    int quakeDelay;
    int startDelayTime;
    bool isToGround;

    this->GetStringValue(node, "FilePath", &filePath);

    JsonValue x;
    this->GetValue(x, "PosX", node, JsonValue::DOUBLE_VALUE);
    if (x.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "PosX");
    } else {
      posX = x.AsDouble();
    }

    JsonValue y;
    this->GetValue(y, "PosY", node, JsonValue::DOUBLE_VALUE);
    if (y.IsNull()) {
      this->Fail("key 에 해당하는 데이터를 찾지 못했습니다. (keyName : %s)", "PosY");
    } else {
      posY = y.AsDouble();
    }

    this->GetBoolValue(node, "IsInfinite", &isInfinite);
    this->GetBoolValue(node, "IsFloor", &isFloor);
    this->GetBoolValue(node, "IsAboveUI", &isAboveUI);
    this->GetStringValue(node, "SoundPath", &soundPath);
    this->GetBoolValue(node, "IsSoundLoop", &isSoundLoop);
    this->GetIntValue(node, "QuakeDelay", &quakeDelay);
    this->GetIntValue(node, "StartDelayTime", &startDelayTime);
    this->GetBoolValue(node, "IsToGround", &isToGround);

    EZ2STREffectRecord record;
    record.name = filePath;
    record.offsetX = posX;
    record.offsetY = posY;
    record.unknown_0028 = isInfinite;
    record.unknown_0029 = isFloor;
    record.unknown_002A = isAboveUI;
    record.unknown_002C = soundPath;
    record.unknown_0044 = isSoundLoop;
    record.unknown_0048[0] = quakeDelay;
    record.unknown_0048[1] = startDelayTime;
    record.unknown_0050 = isToGround;
    if (!out->insert(std::make_pair(name, record)).second) {
      this->Fail("중복키가 입력되었습니다. (key : %s)", name.c_str());
      return false;
    }
  }

  return this->succeeded;
}
