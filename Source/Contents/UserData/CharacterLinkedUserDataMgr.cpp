#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/UserData/AccountService.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Script/LuaScript.h"
#include "Utility/EncryptedName.h"

#include <new>

// STATIC: 0x013898BC
// GLOBAL: gl_24353
CCharacterLinkedUserDataMgr* g_characterLinkedUserDataMgr;

// INLINED
CCharacterLinkedUserDataMgr::CCharacterLinkedUserDataMgr()
    : m_enabled(true), m_accountId(0), m_characterId(0), m_accountName("unknown"), m_loaded(false), m_pendingSaves(0) {
}

// INLINED
CCharacterLinkedUserDataMgr::~CCharacterLinkedUserDataMgr() = default;

// OFFSET: 0x005E5DD0
// FUNCTION: fn_03923
bool CCharacterLinkedUserDataMgr::Initialize() {
  if (g_characterLinkedUserDataMgr) {
    return false;
  }

  g_characterLinkedUserDataMgr = new (std::nothrow) CCharacterLinkedUserDataMgr;
  if (!g_characterLinkedUserDataMgr) {
    return false;
  }

  if (!g_characterLinkedUserDataMgr->m_enabled) {
    delete g_characterLinkedUserDataMgr;
    g_characterLinkedUserDataMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x005E5FB0
// FUNCTION: fn_03924
void CCharacterLinkedUserDataMgr::Shutdown() {
  if (g_characterLinkedUserDataMgr) {
    delete g_characterLinkedUserDataMgr;
    g_characterLinkedUserDataMgr = nullptr;
  }
}

// OFFSET: 0x005E5FD0
// FUNCTION: fn_03925
std::string CCharacterLinkedUserDataMgr::LoadUrl() const {
  return this->m_loadUrl;
}

// OFFSET: 0x005E6030
// FUNCTION: fn_03926
std::string CCharacterLinkedUserDataMgr::SaveUrl() const {
  return this->m_saveUrl;
}

// OFFSET: 0x005E78B0
// FUNCTION: fn_03936
void CCharacterLinkedUserDataMgr::CompleteSave() {
  --this->m_pendingSaves;
}

// OFFSET: 0x005E6BB0
// FUNCTION: fn_03931
__declspec(noinline) void CCharacterLinkedUserDataMgr::RefreshUrls() {
  const std::string base =
    ResolveAccountServiceBase(*g_session.m_hotkeyScript, *g_session.m_hotkeyScript, g_serviceHost, g_servicePort, g_serviceOverrideKey);
  if (!base.empty()) {
    const char saveSource[] = {'\xFE', '\x02', '\x99', '\xE8', '\x1D', '\x32', '\x8A', '\xBD', '\xB7',
                               '\x08', '\x96', '\xA6', '\x1C', '\x30', '\x93', '\xB6', '\xD1'};
    thread_local EncryptedName<sizeof(saveSource), 0xD3E5516F89F161D1ULL> savePath(saveSource);
    this->m_saveUrl = base + savePath.Text();

    const char loadSource[] = {'\x78', '\x8C', '\xB9', '\xD4', '\xD9', '\x1C', '\x92', '\x31', '\x31',
                               '\x86', '\xB6', '\x9A', '\xC7', '\x10', '\x9C', '\x3B', '\x57'};
    thread_local EncryptedName<sizeof(loadSource), 0x5FFD7FABB5D1EF57ULL> loadPath(loadSource);
    this->m_loadUrl = base + loadPath.Text();
  }
}

// OFFSET: 0x005E73B0
// FUNCTION: fn_03934
void CCharacterLinkedUserDataMgr::Reset(LuaScript& script, const char* host, const char* port, const char* overrideKey) {
  this->m_accountId = 0;
  this->m_characterId = 0;
  this->m_accountName = "unknown";
  this->m_loaded = false;
  this->RefreshUrls();
}
