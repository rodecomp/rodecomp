#include "Client/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x005CB3B0
// FUNCTION: fn_03657
void CAdventurerAgencyMgr::RequestChangeMaster(std::uint32_t targetAccountId, std::uint32_t targetCharacterId, std::string targetName) {
  const std::uint32_t account = g_session.m_aid;
  const std::uint32_t character = g_session.m_gid;
  g_asyncWorkMgr->ChangePartyMaster(
    account, character, std::string(g_session.DecodePlayerName()), targetAccountId, targetCharacterId, targetName, std::string(g_session.m_accountName.c_str()),
    this->ServiceUrl(6), std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), g_windowMgr
  );
  ++this->pending;
}

// OFFSET: 0x005CB710
// FUNCTION: fn_03659
void CAdventurerAgencyMgr::RequestMyParty() {
  const std::uint32_t account = g_session.m_aid;
  const std::uint32_t character = g_session.m_gid;
  g_asyncWorkMgr->LoadMyParty(
    account, character, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(5),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), g_windowMgr
  );
  ++this->pending;
}

// OFFSET: 0x005CBEE0
// FUNCTION: fn_03661
void CAdventurerAgencyMgr::RequestList(int page) {
  const std::uint32_t account = g_session.m_aid;
  const std::uint32_t character = g_session.m_gid;
  g_asyncWorkMgr->LoadPartyList(
    account, character, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(1),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), 0, CAdvenPartyInfo(), page, g_windowMgr
  );
  ++this->pending;
}

// OFFSET: 0x005CC290
// FUNCTION: fn_03663
void CAdventurerAgencyMgr::RequestInfo(std::uint32_t queryAccountId) {
  const std::uint32_t account = g_session.m_aid;
  const std::uint32_t character = g_session.m_gid;
  g_asyncWorkMgr->LoadPartyInfo(
    account, character, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(7),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), queryAccountId
  );
  ++this->pending;
}

// OFFSET: 0x005CCC00
// FUNCTION: fn_03665
void CAdventurerAgencyMgr::RequestRegistration(const CAdvenPartyInfo& info) {
  const std::uint32_t account = g_session.m_aid;
  const std::uint32_t character = g_session.m_gid;
  g_asyncWorkMgr->RegisterParty(
    account, character, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(0),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), info
  );
  ++this->pending;
}

// OFFSET: 0x005CCF80
// FUNCTION: fn_03667
void CAdventurerAgencyMgr::RequestSearch(const CAdvenPartyInfo& filter, int page) {
  const std::uint32_t account = g_session.m_aid;
  const std::uint32_t character = g_session.m_gid;
  g_asyncWorkMgr->LoadPartyList(
    account, character, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(3),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), 1, filter, page, g_windowMgr
  );
  ++this->pending;
}

// OFFSET: 0x005CD340
// FUNCTION: fn_03669
void CAdventurerAgencyMgr::RequestStop(std::uint32_t masterAccountId) {
  this->masterAccountId = masterAccountId;
  const std::uint32_t account = g_session.m_aid;
  g_asyncWorkMgr->StopRecruiting(
    account, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(4),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), masterAccountId
  );
  ++this->pending;
}

// OFFSET: 0x005CD440
// FUNCTION: fn_03670
void CAdventurerAgencyMgr::RequestStop() {
  if (!this->registered) {
    return;
  }

  this->masterAccountId = g_session.m_aid;
  const std::uint32_t account = g_session.m_aid;
  g_asyncWorkMgr->StopRecruiting(
    account, std::string(g_session.m_accountName.c_str()), this->ServiceUrl(4),
    std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), account
  );
  ++this->pending;
}
