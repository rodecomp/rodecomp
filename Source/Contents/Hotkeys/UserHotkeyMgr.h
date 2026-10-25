#ifndef RAGNAROK_SOURCE_CONTENTS_HOTKEYS_USERHOTKEYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_HOTKEYS_USERHOTKEYMGR_H

#include "Contents/Hotkeys/UserHotkeyScript.h"

#include <cstdint>
#include <list>
#include <string>

class CUserHotkeyMgr {
public:
  CUserHotkeyMgr();
  virtual ~CUserHotkeyMgr();
  static bool Initialize();
  static void Shutdown();
  void Reset();
  void LoadV2(LuaScript& script, Json::Value& data);
  void Capture(LuaScript& script);
  bool HasUserAssignments(LuaScript& script) const;
  bool HasChanges(LuaScript& script) const;
  void Export(LuaScript& script, Json::Value& data);

protected:
  friend class CAccountLinkedUserDataMgr;
  struct Hotkey {
    int tab;
    int index;
    int key1;
    int key2;
    std::string description;
  };
  /* 0004 */ bool m_enabled;
  /* 0005 */ bool m_loaded;
  /* 0008 */ std::uint32_t m_changes;
  /* 000C */ bool m_version2;
  /* 0010 */ std::list<Hotkey> m_original;
};

// STATIC: 0x013898E4
// GLOBAL: gl_24356
extern CUserHotkeyMgr* g_userHotkeyMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUserHotkeyMgr) == 24);
#endif

#endif
