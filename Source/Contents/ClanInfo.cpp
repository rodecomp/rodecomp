#include "Contents/ClanInfo.h"

#include <new>

// GLOBAL: gl_24376
CClanInfo* g_clanInfo;

CClanInfo::CClanInfo() {
  this->m_isInitialized = false;
  this->Clear();
  this->m_isInitialized = true;
}

CClanInfo::~CClanInfo() {
  this->m_relations.clear();
}

// OFFSET: 0x00ADF090
// FUNCTION: fn_17964
bool CClanInfo::Create() {
  if (g_clanInfo) {
    return false;
  }

  g_clanInfo = new (std::nothrow) CClanInfo();
  if (!g_clanInfo) {
    return false;
  }

  if (!g_clanInfo->m_isInitialized) {
    delete g_clanInfo;
    g_clanInfo = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00ADF240
// FUNCTION: fn_17965
void CClanInfo::Destroy() {
  if (g_clanInfo) {
    delete g_clanInfo;
    g_clanInfo = nullptr;
  }
}

// OFFSET: 0x00ADF260
// FUNCTION: fn_17966
void CClanInfo::Clear() {
  this->m_clanName = "";
  this->m_masterName = "";
  this->allyClanCount = 0;
  this->m_connectedMembers = 0;
  this->m_totalMembers = 0;
  this->m_mapName = "";
  this->m_clanId = 0;
  this->m_hasClan = false;
  this->m_relations.clear();
}
