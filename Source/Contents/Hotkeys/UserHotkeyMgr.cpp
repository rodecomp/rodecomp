#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Network/TextEncoding.h"

#include <new>

namespace {
  enum InterfaceHotkey {
    HK_BATTLEFIELDWND = 30,
    HK_PARTY_RECRUIT = 33,
    HK_PARTY_BOOKING_LIST = 34,
    HK_EXMACRO_SETUPWND = 36,
    HK_BUTTONS_RELOCATE = 43,
    HK_MENUICON_HIDE = 45,
    HK_TWITTERWND = 53,
    HK_HUNTINGQUEST_INFO_ONOFF = 58,
    HK_UNKNOWN_62 = 62,
    HK_UNKNOWN_64 = 64,
    HK_UNKNOWN_65 = 65,
    HK_UNKNOWN_66 = 66,
    HK_UNKNOWN_67 = 67,
    HK_FIRST_HIDDEN = HK_BATTLEFIELDWND,
  };

  // INLINED
  bool HiddenInterfaceIndex(int index) {
    if (index == HK_BATTLEFIELDWND) {
      return true;
    }
    if (index == HK_PARTY_RECRUIT) {
      return true;
    }
    if (index == HK_PARTY_BOOKING_LIST) {
      return true;
    }
    if (index == HK_EXMACRO_SETUPWND) {
      return true;
    }
    if (index == HK_TWITTERWND) {
      return true;
    }
    if (index == HK_HUNTINGQUEST_INFO_ONOFF) {
      return true;
    }
    if (index == HK_UNKNOWN_62) {
      return true;
    }
    if (index == HK_UNKNOWN_64) {
      return true;
    }
    if (index == HK_UNKNOWN_66) {
      return true;
    }
    if (index == HK_UNKNOWN_65) {
      return true;
    }
    if (index == HK_UNKNOWN_67) {
      return true;
    }
    if (index == HK_BUTTONS_RELOCATE) {
      return true;
    }
    return index == HK_MENUICON_HIDE;
  }

  // OFFSET: 0x00E12E50
  // FUNCTION: fn_23820
  int HotkeyIndex(LuaScript& script, int tab, int index) {
    if (tab == 1 && index >= HK_FIRST_HIDDEN) {
      for (int candidate = HK_FIRST_HIDDEN; candidate <= index; ++candidate) {
        if (HiddenInterfaceIndex(candidate)) {
          ++index;
        }
      }
    }

    int count = 0;
    script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count);
    return index >= count ? -1 : index;
  }
}

// OFFSET: 0x00E0A4C0
// FUNCTION: fn_23797
int HotkeyCount(LuaScript& script, int tab) {
  int count = 0;
  if (script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count)) {
    int hidden = 0;
    if (tab == 1) {
      for (int index = HK_FIRST_HIDDEN; index < count; ++index) {
        if (HiddenInterfaceIndex(index)) {
          ++hidden;
        }
      }
    }

    return count - hidden;
  }

  return count;
}

// STATIC: 0x013898E4
// GLOBAL: gl_24356
CUserHotkeyMgr* g_userHotkeyMgr;

// OFFSET: 0x005F7AA0
// FUNCTION: fn_04052
CUserHotkeyMgr::CUserHotkeyMgr() : m_loaded(false), m_changes(0), m_version2(false) {
  this->m_enabled = false;
  this->m_original.clear();
  this->m_enabled = true;
}

CUserHotkeyMgr::~CUserHotkeyMgr() = default;

// OFFSET: 0x005F7F70
// FUNCTION: fn_04056
bool CUserHotkeyMgr::Initialize() {
  if (g_userHotkeyMgr) {
    return false;
  }

  g_userHotkeyMgr = new (std::nothrow) CUserHotkeyMgr;
  if (!g_userHotkeyMgr) {
    return false;
  }

  if (!g_userHotkeyMgr->m_enabled) {
    delete g_userHotkeyMgr;
    g_userHotkeyMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x005F8080
// FUNCTION: fn_04057
void CUserHotkeyMgr::Shutdown() {
  if (g_userHotkeyMgr) {
    delete g_userHotkeyMgr;
    g_userHotkeyMgr = nullptr;
  }
}

// OFFSET: 0x005F80A0
// FUNCTION: fn_04058
int CUserHotkeyMgr::TabAt(int position) const {
  int tab = 0;
  switch (position) {
  case 1:
    tab = 3;
    break;
  case 2:
    tab = 1;
    break;
  case 3:
    tab = 2;
    break;
  }
  return tab;
}

// OFFSET: 0x005F88D0
// FUNCTION: fn_04062
void CUserHotkeyMgr::Reset() {
  this->m_original.clear();
  this->m_loaded = false;
  this->m_changes = 0;
  this->m_version2 = false;
}

// OFFSET: 0x005BC420
// FUNCTION: fn_03464
void CUserHotkeyMgr::LoadV2(LuaScript& script, Json::Value& data) {
  Json::Value values = data["UserHotkey_V2"];
  if (values.empty()) {
    this->m_version2 = false;
  } else {
    this->m_version2 = true;
    ClearUserHotkeys(script);
    LoadUserHotkeyTab(script, values["SkillBar_1Tab"], 0);
    LoadUserHotkeyTab(script, values["SkillBar_2Tab"], 3);
    LoadUserHotkeyTab(script, values["InterfaceTab"], 1);
    LoadUserHotkeyTab(script, values["EmotionTab"], 2);
    SaveUserHotkeys(script);
  }
}

// OFFSET: 0x005F85A0
// FUNCTION: fn_04061
void CUserHotkeyMgr::Capture(LuaScript& script) {
  this->m_original.clear();

  for (int position = 0; position < 4; ++position) {
    const int tab = this->TabAt(position);
    const int count = HotkeyCount(script, tab);

    for (int row = 0; row < count; ++row) {
      const int index = HotkeyIndex(script, tab, row);
      if (index == -1) {
        continue;
      }

      std::string text;
      int key1, key2;
      const char* keyText = nullptr;
      const char* description = nullptr;

      if (script.Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description)) {
        text = description;
        if (!text.empty()) {
          this->m_original.emplace_back(tab, index, key1, key2, text);
        }
      }
    }
  }
}

// OFFSET: 0x005F8100
// FUNCTION: fn_04060
bool CUserHotkeyMgr::HasUserAssignments(LuaScript& script) const {
  for (int position = 0; position < 4; ++position) {
    const int tab = this->TabAt(position);
    const int count = HotkeyCount(script, tab);

    for (int row = 0; row < count; ++row) {
      const int index = HotkeyIndex(script, tab, row);
      if (index == -1) {
        continue;
      }

      std::string keyName;
      std::string userDescription;
      int key1, key2;
      const char* keyText = nullptr;
      const char* description = nullptr;

      if (script.Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description)) {
        userDescription = description;
        if (!userDescription.empty() &&
            script.Call("GetOriginalHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description)) {
          std::string originalDescription = description;
          if (!originalDescription.empty()) {
            return true;
          }
        }
      }
    }
  }

  return false;
}

// OFFSET: 0x005F80E0
// FUNCTION: fn_04059
bool CUserHotkeyMgr::HasChanges(LuaScript& script) const {
  return static_cast<std::int32_t>(this->m_changes) > 0 || this->m_version2 || this->HasUserAssignments(script);
}

// OFFSET: 0x005BD2C0
// FUNCTION: fn_03471
void CUserHotkeyMgr::Export(LuaScript& script, Json::Value& data) {
  this->Capture(script);
  const std::list<Hotkey> snapshot = this->m_original;
  Json::Value tabs[4];

  for (const Hotkey& hotkey : snapshot) {
    Json::Value value;
    value["index"] = hotkey.index;
    value["key1"] = hotkey.key1;
    value["key2"] = hotkey.key2;
    value["desc"] = ToUtf8(hotkey.description, g_textCodePage);

    if (hotkey.tab >= 0 && hotkey.tab < 4) {
      tabs[hotkey.tab].append(value);
    }
  }

  Json::Value version2;
  version2["SkillBar_1Tab"] = tabs[0];
  version2["SkillBar_2Tab"] = tabs[3];
  version2["InterfaceTab"] = tabs[1];
  version2["EmotionTab"] = tabs[2];
  data["UserHotkey_V2"] = version2;
  data["UserHotkey"] = Json::Value();
}
