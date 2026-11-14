#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Client/Session/Session.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Hotkeys/InterfaceHotkey.h"
#include "Network/TextEncoding.h"

#include <new>

namespace {

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
    if (index == HK_HIDDEN_62) {
      return true;
    }
    if (index == HK_HIDDEN_64) {
      return true;
    }
    if (index == HK_HIDDEN_66) {
      return true;
    }
    if (index == HK_HIDDEN_65) {
      return true;
    }
    if (index == HK_HIDDEN_67) {
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
int CSession::HotkeyCount(int tab) const {
  int count = 0;
  if (g_session.m_hotkeyScript->Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count)) {
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
void CUserHotkeyMgr::LoadV2(Json::Value& data) {
  Json::Value values = data["UserHotkey_V2"];
  if (values.empty()) {
    this->m_version2 = false;
  } else {
    this->m_version2 = true;
    ClearUserHotkeys();
    this->LoadUserHotkeyTab(values["SkillBar_1Tab"], 0);
    this->LoadUserHotkeyTab(values["SkillBar_2Tab"], 3);
    this->LoadUserHotkeyTab(values["InterfaceTab"], 1);
    this->LoadUserHotkeyTab(values["EmotionTab"], 2);
    SaveUserHotkeys();
  }
}

// OFFSET: 0x005F85A0
// FUNCTION: fn_04061
void CUserHotkeyMgr::Capture() {
  this->m_original.clear();

  for (int position = 0; position < 4; ++position) {
    const int tab = this->TabAt(position);
    const int count = HotkeyCount(*g_session.m_hotkeyScript, tab);

    for (int row = 0; row < count; ++row) {
      const int index = HotkeyIndex(*g_session.m_hotkeyScript, tab, row);
      if (index == -1) {
        continue;
      }

      std::string text;
      int key1, key2;
      const char* keyText = nullptr;
      const char* description = nullptr;

      if (g_session.m_hotkeyScript->Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &keyText, &description)) {
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
bool CUserHotkeyMgr::HasChanges() const {
  if (static_cast<std::int32_t>(this->m_changes) > 0) {
    return true;
  }
  if (!this->m_version2 && this->HasUserAssignments(*g_session.m_hotkeyScript) == true) {
    return true;
  }
  return false;
}

// OFFSET: 0x005BD2C0
// FUNCTION: fn_03471
void CAccountLinkedUserDataMgr::ExportUserHotkey(Json::Value& data) {
  g_userHotkeyMgr->Capture();
  Json::Value version2;
  const std::list<CUserHotkeyMgr::Hotkey> snapshot = g_userHotkeyMgr->m_original;
  Json::Value skillBar1;
  Json::Value skillBar2;
  Json::Value interfaceTab;
  Json::Value emotionTab;

  for (const CUserHotkeyMgr::Hotkey& hotkey : snapshot) {
    Json::Value value;
    value["index"] = hotkey.index;
    value["key1"] = hotkey.key1;
    value["key2"] = hotkey.key2;
    value["desc"] = ToUtf8(hotkey.description, g_textCodePage);

    switch (hotkey.tab) {
    case 0:
      skillBar1.append(value);
      break;
    case 3:
      skillBar2.append(value);
      break;
    case 1:
      interfaceTab.append(value);
      break;
    case 2:
      emotionTab.append(value);
      break;
    }
  }

  version2["SkillBar_1Tab"] = skillBar1;
  version2["SkillBar_2Tab"] = skillBar2;
  version2["InterfaceTab"] = interfaceTab;
  version2["EmotionTab"] = emotionTab;
  data["UserHotkey_V2"] = version2;
  data["UserHotkey"] = Json::Value();
}
