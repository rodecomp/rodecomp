#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Contents/MessageTable.h"
#include "File/File.h"
#include "File/ShortcutFilename.h"
#include "Network/TextEncoding.h"
#include "Script/LuaInstance.h"
#include <direct.h>
#include <format>
#include <json/value.h>
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
    for (int i = 0; i < 10; ++i) {
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
void EmotionHotkeys::Save(std::uint32_t accountId, CRenderSystemBase*) const {
  if (this->current.empty()) {
    return;
  }

  std::string output;
  output += "tbl = \r\n";
  output += "{\r\n";
  for (const std::string& entry : this->current) {
    output += std::format("\t[=[{}]=],\r\n", entry);
  }
  output += "}\r\n";

  if (!output.empty()) {
    const std::string directory = "savedata\\shortcutlist";
    if (_access_s(directory.c_str(), 0) != 0) {
      _mkdir(directory.c_str());
    }

    CFile file;
    if (file.Open(std::format("{}\\{}.lua", directory, ShortcutFilename(accountId)).c_str(), 0x1001, false, true)) {
      file.Write(output.data(), static_cast<std::uint32_t>(output.size()));
      file.Close();
    }
  }
}

// OFFSET: 0x00E3D4D0
// FUNCTION: fn_24191
bool EmotionHotkeys::LoadDefaults(std::uint32_t accountId) {
  this->current.clear();
  this->current.emplace_back(LookupMessage(544));
  this->current.emplace_back(LookupMessage(545));
  this->current.emplace_back(LookupMessage(546));
  this->current.emplace_back(LookupMessage(547));
  this->current.emplace_back(LookupMessage(549));
  this->current.emplace_back(LookupMessage(550));
  this->current.emplace_back(LookupMessage(551));
  this->current.emplace_back(LookupMessage(552));
  this->current.emplace_back(LookupMessage(553));
  this->current.emplace_back(LookupMessage(554));

  CLuaInstance script;
  const bool loaded = script.LoadFile(std::format("savedata\\shortcutlist\\{}.lua", ShortcutFilename(accountId)), false);
  return loaded && script.ReadStrings(this->current, "tbl") == 0 && this->current.size() == 10;
}

// INLINED
void EmotionHotkeys::Export(Json::Value& data) const {
  Json::Value values;

  for (unsigned index = 0; index < 10; ++index) {
    values.append(ToUtf8(this->current[index], g_textCodePage));
  }

  data["EmotionHotkey"] = values;
}
