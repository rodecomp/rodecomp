#include "Client/Session/Session.h"
#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Contents/Hotkeys/UserHotkeyScript.h"
#include "Network/TextEncoding.h"

// OFFSET: 0x005F7D60
// FUNCTION: fn_04055
void CUserHotkeyMgr::ClearUserHotkeys() {
  std::string path = "SaveData\\UserKeys";
  path = path + ".lua";
  if (!path.empty()) {
    g_session.m_hotkeyScript->Call("ClearUserHotKeys", "s", path.c_str());
  }
}

// OFFSET: 0x005F8910
// FUNCTION: fn_04063
void CUserHotkeyMgr::SaveUserHotkeys() {
  std::string path = "SaveData\\UserKeys";
  path = path + ".lua";
  if (!path.empty()) {
    g_session.m_hotkeyScript->Call("SaveUserHotKeys2", "s", path.c_str());
  }
}

// OFFSET: 0x005F8B20
// FUNCTION: fn_04064
__declspec(noinline) void CUserHotkeyMgr::ChangeUserHotkey(int tab, int index, int key1, int key2, const std::string& description) {
  g_session.m_hotkeyScript->Call("ChangeUserHotKey", "ddsdd", tab + 1, index, description.c_str(), key1, key2);
}

// OFFSET: 0x005BD660
// FUNCTION: fn_03473
void CUserHotkeyMgr::LoadUserHotkeyTab(Json::Value& values, int tab) {
  Json::Value entries = values;

  for (auto item = entries.begin(); item != entries.end(); ++item) {
    if ((*item).isObject()) {
      int index = (*item).get("index", "-1").asInt();
      int key1 = (*item).get("key1", "-1").asInt();
      int key2 = (*item).get("key2", "-1").asInt();
      std::string description = FromUtf8((*item).get("desc", "").asString(), g_textCodePage);

      if (index != -1 || description.empty()) {
        g_userHotkeyMgr->ChangeUserHotkey(tab, index, key1, key2, description);
      }
    }
  }
}
