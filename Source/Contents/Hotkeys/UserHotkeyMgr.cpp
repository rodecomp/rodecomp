#include "Contents/Hotkeys/UserHotkeyMgr.h"
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
    switch (index) {
    case HK_BATTLEFIELDWND:
    case HK_PARTY_RECRUIT:
    case HK_PARTY_BOOKING_LIST:
    case HK_EXMACRO_SETUPWND:
    case HK_BUTTONS_RELOCATE:
    case HK_MENUICON_HIDE:
    case HK_TWITTERWND:
    case HK_HUNTINGQUEST_INFO_ONOFF:
    case HK_UNKNOWN_62:
    case HK_UNKNOWN_64:
    case HK_UNKNOWN_65:
    case HK_UNKNOWN_66:
    case HK_UNKNOWN_67:
      return true;
    default:
      return false;
    }
  }

  // OFFSET: 0x00E0A4C0
  // FUNCTION: fn_23797
  int HotkeyCount(LuaScript& script, int tab) {
    int count = 0;
    if (script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count)) {
      if (tab == 1) {
        int hidden = 0;

        for (int index = HK_FIRST_HIDDEN; index < count; ++index) {
          hidden += HiddenInterfaceIndex(index);
        }

        return count - hidden;
      }
    }

    return count;
  }

  // OFFSET: 0x00E12E50
  // FUNCTION: fn_23820
  int HotkeyIndex(LuaScript& script, int tab, int index) {
    if (tab == 1 && index >= HK_FIRST_HIDDEN) {
      for (int candidate = HK_FIRST_HIDDEN; candidate <= index; ++candidate) {
        index += HiddenInterfaceIndex(candidate);
      }
    }

    int count = 0;
    script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count);
    return index >= count ? -1 : index;
  }
}

// STATIC: 0x013898E4
// GLOBAL: gl_24356
CUserHotkeyMgr* g_userHotkeyMgr;

// OFFSET: 0x005F7AA0
// FUNCTION: fn_04052
CUserHotkeyMgr::CUserHotkeyMgr() : m_enabled(true), m_loaded(false), m_changes(0), m_version2(false) {
}

// OFFSET: 0x005F7CE0
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
  switch (position) {
  case 1:
    return 3;
  case 2:
    return 1;
  case 3:
    return 2;
  default:
    return 0;
  }
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

  for (int tab : {0, 3, 1, 2}) {
    const int count = HotkeyCount(script, tab);

    for (int row = 0; row < count; ++row) {
      const int index = HotkeyIndex(script, tab, row);
      if (index == -1) {
        continue;
      }

      int key1, key2;
      const char* keyText = nullptr;
      const char* description = nullptr;

      if (script.Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description)) {
        std::string text = description;
        if (!text.empty()) {
          this->m_original.push_back({tab, index, key1, key2, text});
        }
      }
    }
  }
}

// OFFSET: 0x005F8100
// FUNCTION: fn_04060
bool CUserHotkeyMgr::HasUserAssignments(LuaScript& script) const {
  for (int tab : {0, 3, 1, 2}) {
    const int count = HotkeyCount(script, tab);

    for (int row = 0; row < count; ++row) {
      const int index = HotkeyIndex(script, tab, row);
      if (index == -1) {
        continue;
      }

      int key1 = 0, key2 = 0;
      const char* keyText = nullptr;
      const char* description = nullptr;

      if (script.Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description) && std::string(description).size()) {
        if (script.Call("GetOriginalHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description) && std::string(description).size()) {
          return true;
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
