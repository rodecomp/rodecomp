#ifndef RAGNAROK_SOURCE_CONTENTS_SPECIALPOPUPMGR_H
#define RAGNAROK_SOURCE_CONTENTS_SPECIALPOPUPMGR_H

#include <map>
#include <string>
#include <vector>

struct lua_State;

// VTABLE: vt_00752
struct PopupData {
  // INLINED
  PopupData() : m_messageId(0), m_color(0xFFFFFFFF) {
  }
  virtual ~PopupData() {
  }

  /* 0004 */ int m_messageId;
  /* 0008 */ unsigned int m_color;
  /* 000C */ std::vector<std::string> m_descriptions;
};

// VTABLE: vt_00753
class CSpecialPopupMgr {
public:
  // INLINED
  CSpecialPopupMgr() {
    this->m_initialized = true;
  }
  virtual ~CSpecialPopupMgr() {
  }

  static bool Create();
  static void Destroy();
  PopupData* GetPopup(int id);
  static bool Init();
  static int LuaAddDesc(lua_State* state);
  static int LuaAddMsg(lua_State* state);

  // INLINED
  void AddDescription(int id, std::string description) {
    this->m_popups[id].m_descriptions.push_back(description);
  }

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::map<int, PopupData> m_popups;
};

extern CSpecialPopupMgr* g_specialPopupMgr;

std::string FormatPopupString(const char* format, ...);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PopupData) == 0x18);
static_assert(sizeof(CSpecialPopupMgr) == 0x10);
#endif

#endif
