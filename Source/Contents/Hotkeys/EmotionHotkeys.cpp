#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "File/ShortcutFilename.h"
#include "Network/TextEncoding.h"
#include "Script/LuaInstance.h"
#include "Resource/MSGSTRINGID.h"
#include <direct.h>
#include <format>
#include <json/value.h>
#include <io.h>

// OFFSET: 0x005BC230
// FUNCTION: fn_03463
void EmotionHotkeys::Load(Json::Value& data) {
  Json::Value values = data["EmotionHotkey"];
  EmotionHotkeys& hotkeys = g_session.m_emotionHotkeys;
  hotkeys.saved.clear();
  hotkeys.saved.resize(10);

  for (Json::UInt i = 0; i < values.size(); ++i) {
    hotkeys.current[i] = static_cast<const std::string&>(FromUtf8(values[i].asString(), g_textCodePage));
    hotkeys.saved[i] = static_cast<const std::string&>(FromUtf8(values[i].asString(), g_textCodePage));
  }
}

// OFFSET: 0x00E24D10
// FUNCTION: fn_23900
bool CSession::HasEmotionHotkeyChanges() const {
  if (!this->m_emotionHotkeys.saved.empty()) {
    const std::string* current = this->m_emotionHotkeys.current.data();
    const std::string* saved = this->m_emotionHotkeys.saved.data();
    for (int i = 0; i < 10; ++i) {
      if (saved[i] != current[i]) {
        return true;
      }
    }

    return false;
  }

  return true;
}

// OFFSET: 0x00E48460
// FUNCTION: fn_24220
void CSession::SaveEmotionHotkeys() const {
  if (this->m_emotionHotkeys.current.empty()) {
    return;
  }

  std::string output;
  output += "tbl = \r\n";
  output += "{\r\n";
  for (const std::string& entry : this->m_emotionHotkeys.current) {
    output += std::format("\t[=[{}]=],\r\n", entry);
  }
  output += "}\r\n";

  if (!output.empty()) {
    const std::string directory = "savedata\\shortcutlist";
    if (_access_s(directory.c_str(), 0) != 0) {
      _mkdir(directory.c_str());
    }

    CFile file;
    if (file.Open(std::format("{}\\{}.lua", directory, this->ShortcutFilename(this->m_aid)).c_str(), 0x1001, false, true)) {
      file.Write(output.data(), static_cast<std::uint32_t>(output.size()));
      file.Close();
    }
  }
}

// OFFSET: 0x00E3D4D0
// FUNCTION: fn_24191
bool CSession::LoadEmotionHotkeyDefaults() {
  this->m_emotionHotkeys.current.clear();
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_SURPRISE));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_QUESTION));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_DELIGHT));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_THROB));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_SWEAT));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_AHA));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_FRET));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_ANGER));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_MONEY));
  this->m_emotionHotkeys.current.emplace_back(LookupMessage(MSI_EMOTION_THINK));

  CLuaInstance script;
  const bool loaded = script.LoadFile(std::format("savedata\\shortcutlist\\{}.lua", this->ShortcutFilename(this->m_aid)), false);
  return loaded && script.ReadStrings(this->m_emotionHotkeys.current, "tbl") == 0 && this->m_emotionHotkeys.current.size() == 10;
}

// INLINED
void EmotionHotkeys::Export(Json::Value& data) const {
  Json::Value values;

  for (unsigned index = 0; index < 10; ++index) {
    values.append(ToUtf8(this->current[index], g_textCodePage));
  }

  data["EmotionHotkey"] = values;
}
