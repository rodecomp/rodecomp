#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Contents/MessageTable.h"
#include "File/File.h"
#include "File/ShortcutFilename.h"
#include "Network/TextEncoding.h"
#include "Script/LuaInstance.h"

#include <direct.h>
#include <io.h>

// OFFSET: 0x005BC230
// FUNCTION: fn_03463
void EmotionHotkeys::Load(Json::Value& data) {
  Json::Value values = data["EmotionHotkey"];
  this->saved.clear();
  this->saved.resize(10);

  for (Json::UInt i = 0; i < values.size(); ++i) {
    this->current[i] = FromUtf8(values[i].asString(), g_textCodePage);
    this->saved[i] = FromUtf8(values[i].asString(), g_textCodePage);
  }
}

// OFFSET: 0x00E24D10
// FUNCTION: fn_23900
bool EmotionHotkeys::HasChanges() const {
  if (!this->saved.empty()) {
    for (unsigned i = 0; i < 10; ++i) {
      if (this->saved[i] != this->current[i]) {
        return true;
      }
    }

    return false;
  }

  return true;
}

// OFFSET: 0x00E48460
// FUNCTION: fn_24220
void EmotionHotkeys::Save(std::uint32_t accountId, CRenderSystemBase* renderSystem) const {
  if (this->current.empty()) {
    return;
  }

  std::string output = "tbl = \r\n{\r\n";

  for (const std::string& entry : this->current) {
    output += "\t[=[";
    output += entry;
    output += "]=],\r\n";
  }

  output += "}\r\n";

  const char* directory = "savedata\\shortcutlist";
  if (_access_s(directory, 0) != 0) {
    _mkdir(directory);
  }

  CFile file;
  const std::string path = std::string(directory) + "\\" + ShortcutFilename(accountId) + ".lua";
  if (file.OpenWrite(path.c_str(), renderSystem)) {
    file.Write(output.data(), static_cast<std::uint32_t>(output.size()));
    file.Close();
  }
}

// OFFSET: 0x00E3D4D0
// FUNCTION: fn_24191
bool EmotionHotkeys::LoadDefaults(std::uint32_t accountId, bool languagePrefix, int serviceType, std::uint16_t language) {
  this->current.clear();

  for (int id : {544, 545, 546, 547, 549, 550, 551, 552, 553, 554}) {
    this->current.emplace_back(LookupMessage(id, languagePrefix, serviceType, language));
  }

  CLuaInstance script;
  const std::string path = "savedata\\shortcutlist\\" + ShortcutFilename(accountId) + ".lua";
  if (!script.LoadFile(path, false) || script.ReadStrings(this->current, "tbl") != 0) {
    return false;
  }

  return this->current.size() == 10;
}

// OFFSET: 0x005BCE50
// FUNCTION: fn_03469
void EmotionHotkeys::Export(Json::Value& data) const {
  Json::Value values;

  for (unsigned index = 0; index < 10; ++index) {
    values.append(ToUtf8(this->current[index], g_textCodePage));
  }

  data["EmotionHotkey"] = values;
}
