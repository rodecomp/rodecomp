#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Script/LuaScript.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

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
  const std::string base = g_session.ResolveAccountServiceBase();
  if (!base.empty()) {
    this->m_saveUrl = base + OBFUSCATE_TEXT_NAMED("/charconfig/save", 0xD3E5516F89F161D1ULL, savePath);

    this->m_loadUrl = base + OBFUSCATE_TEXT_NAMED("/charconfig/load", 0x5FFD7FABB5D1EF57ULL, loadPath);
  }
}

// OFFSET: 0x005E73B0
// FUNCTION: fn_03934
void CCharacterLinkedUserDataMgr::Reset() {
  this->m_accountId = 0;
  this->m_characterId = 0;
  this->m_accountName = "unknown";
  this->m_loaded = false;
  this->RefreshUrls();
}
