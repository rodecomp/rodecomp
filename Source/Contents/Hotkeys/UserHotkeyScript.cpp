#include "Client/Session.h"
#include "Contents/Hotkeys/UserHotkeyScript.h"
#include "Network/TextEncoding.h"

// OFFSET: 0x005F7D60
// FUNCTION: fn_04055
void ClearUserHotkeys() {
  std::string path = "SaveData\\UserKeys";
  path += ".lua";
  if (!path.empty()) {
    if (g_session.m_hotkeyScript != nullptr) {
      g_session.m_hotkeyScript->Call("ClearUserHotKeys", "s", path.c_str());
    }
  }
}

// OFFSET: 0x005F8910
// FUNCTION: fn_04063
void SaveUserHotkeys() {
  std::string path = "SaveData\\UserKeys";
  path += ".lua";
  if (!path.empty()) {
    if (g_session.m_hotkeyScript != nullptr) {
      g_session.m_hotkeyScript->Call("SaveUserHotKeys2", "s", path.c_str());
    }
  }
}

// OFFSET: 0x005F8B20
// FUNCTION: fn_04064
void __stdcall ChangeUserHotkey(int tab, int index, int key1, int key2, const std::string& description) {
  g_session.m_hotkeyScript->Call("ChangeUserHotKey", "ddsdd", tab + 1, index, description.c_str(), key1, key2);
}

// OFFSET: 0x005BD660
// FUNCTION: fn_03473
void __stdcall LoadUserHotkeyTab(Json::Value& values, int tab) {
  Json::Value entries = values;

  for (auto item = entries.begin(); item != entries.end(); ++item) {
    Json::Value& value = *item;
    if (value.isObject()) {
      int index = value.get("index", "-1").asInt();
      int key1 = value.get("key1", "-1").asInt();
      int key2 = value.get("key2", "-1").asInt();
      std::string description = FromUtf8(value.get("desc", "").asString(), g_textCodePage);

      if (index != -1 || description.empty()) {
        ChangeUserHotkey(tab, index, key1, key2, description);
      }
    }
  }
}
