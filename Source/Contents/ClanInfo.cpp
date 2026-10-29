#include "Contents/ClanInfo.h"

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
  this->unknown_0034 = 0;
  this->m_connectedMembers = 0;
  this->m_totalMembers = 0;
  this->m_mapName = "";
  this->m_clanId = 0;
  this->m_hasClan = false;
  this->m_relations.clear();
}
