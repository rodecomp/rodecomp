#include "Contents/UserData/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/AccountLinkedUserDataSaveAsyncWork.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Async/AsyncWorkRecycleObjectPoolMgr.h"
#include "Contents/Party/ChangePartyMasterAsyncWork.h"
#include "Contents/UserData/CharacterLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/CharacterLinkedUserDataSaveAsyncWork.h"
#include "Contents/Emblems/EmblemDataDownloadAsyncWork.h"
#include "Contents/Emblems/EmblemDataUploadAsyncWork.h"
#include "Contents/Party/LoadMyPartyAsyncWork.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
#include "Contents/Party/LoadPartyListAsyncWork.h"
#include "Contents/Store/MCStoreInfoBackupAsyncWork.h"
#include "Contents/Party/RegisterPartyAsyncWork.h"
#include "Contents/Party/StopRecruitingAsyncWork.h"

#include <new>

// STATIC: 0x0138968C
// GLOBAL: gl_24349
CAsyncWorkMgr* g_asyncWorkMgr;

// INLINED
CAsyncWorkMgr::CAsyncWorkMgr(std::uint32_t workers) : NAsyncWorkMgr(workers), m_pools(nullptr) {
  this->m_ready = false;
  this->m_pools = new (std::nothrow) CAsyncWorkRecycleObjectPoolMgr;
  if (this->m_pools) {
    if (!this->m_pools->Ready()) {
      delete this->m_pools;
      this->m_pools = nullptr;
    } else {
      this->m_ready = true;
    }
  }
}

// INLINED
CAsyncWorkMgr::~CAsyncWorkMgr() {
  this->DestroyQueues();
  delete this->m_pools;
  this->m_pools = nullptr;
}

// OFFSET: 0x00573C50
// FUNCTION: fn_02497
bool CAsyncWorkMgr::Initialize(std::uint32_t workers) {
  if (g_asyncWorkMgr) {
    return false;
  }

  g_asyncWorkMgr = new (std::nothrow) CAsyncWorkMgr(workers);

  if (!g_asyncWorkMgr) {
    return false;
  }

  if (!g_asyncWorkMgr->m_ready) {
    delete g_asyncWorkMgr;
    g_asyncWorkMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00573D90
// FUNCTION: fn_02498
void CAsyncWorkMgr::Shutdown() {
  if (g_asyncWorkMgr) {
    delete g_asyncWorkMgr;
    g_asyncWorkMgr = nullptr;
  }
}

// OFFSET: 0x00573DB0
// FUNCTION: fn_02499
void CAsyncWorkMgr::LoadAccount(std::uint32_t accountId, std::string world, std::string url) {
  NAsyncWorkPtr work(this->m_pools, g_accountLinkedUserDataLoadPoolName);

  if (work.Get()) {
    static_cast<CAccountLinkedUserDataLoadAsyncWork*>(work.Get())->Bind(accountId, world, url);
    this->Submit(work);
  }
}

// OFFSET: 0x00573EC0
// FUNCTION: fn_02500
void CAsyncWorkMgr::SaveAccount(std::uint32_t accountId, std::string world, std::string url, std::string token, std::string data) {
  NAsyncWorkPtr work(this->m_pools, g_accountLinkedUserDataSavePoolName);

  if (work.Get()) {
    static_cast<CAccountLinkedUserDataSaveAsyncWork*>(work.Get())->Bind(accountId, world, url, token, data);
    this->Submit(work);
  }
}

// OFFSET: 0x00574010
// FUNCTION: fn_02501
void CAsyncWorkMgr::ChangePartyMaster(
  std::uint32_t accountId, std::uint32_t characterId, std::string characterName, std::uint32_t targetAccountId, std::uint32_t targetCharacterId,
  std::string targetName, std::string world, std::string url, std::string token
) {
  NAsyncWorkPtr work(this->m_pools, g_changePartyMasterPoolName);

  if (work.Get()) {
    static_cast<CChangePartyMasterAsyncWork*>(work.Get())
      ->Bind(accountId, characterId, characterName, targetAccountId, targetCharacterId, targetName, world, url, token);
    this->Submit(work);
  }
}

// OFFSET: 0x00574190
// FUNCTION: fn_02502
void CAsyncWorkMgr::LoadCharacter(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token
) {
  NAsyncWorkPtr work(this->m_pools, g_characterLinkedUserDataLoadPoolName);

  if (work.Get()) {
    static_cast<CCharacterLinkedUserDataLoadAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token);
    this->Submit(work);
  }
}

// OFFSET: 0x005742D0
// FUNCTION: fn_02503
void CAsyncWorkMgr::SaveCharacter(std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::string data) {
  NAsyncWorkPtr work(this->m_pools, g_characterLinkedUserDataSavePoolName);

  if (work.Get()) {
    static_cast<CCharacterLinkedUserDataSaveAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token, data);
    this->Submit(work);
  }
}

// OFFSET: 0x00574430
// FUNCTION: fn_02504
void CAsyncWorkMgr::DownloadEmblem(
  std::string url, std::uint32_t accountId, std::string token, std::string world, std::uint32_t guildId, std::uint32_t version
) {
  NAsyncWorkPtr work(this->m_pools, g_emblemDataDownloadPoolName);

  if (work.Get()) {
    static_cast<CEmblemDataDownloadAsyncWork*>(work.Get())->Bind(url, accountId, token, world, guildId, version);
    this->Submit(work);
  }
}

// OFFSET: 0x00574570
// FUNCTION: fn_02505
void CAsyncWorkMgr::UploadEmblem(std::string url, std::uint32_t accountId, std::string token, std::string world, std::uint32_t guildId, std::string path) {
  NAsyncWorkPtr work(this->m_pools, g_emblemDataUploadPoolName);

  if (work.Get()) {
    static_cast<CEmblemDataUploadAsyncWork*>(work.Get())->Bind(url, accountId, token, world, guildId, path);
    this->Submit(work);
  }
}

// OFFSET: 0x005746D0
// FUNCTION: fn_02506
void CAsyncWorkMgr::LoadMyParty(std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token) {
  NAsyncWorkPtr work(this->m_pools, g_loadMyPartyPoolName);

  if (work.Get()) {
    static_cast<CLoadMyPartyAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token);
    this->Submit(work);
  }
}

// OFFSET: 0x00574810
// FUNCTION: fn_02507
void CAsyncWorkMgr::LoadPartyInfo(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::uint32_t queryAccountId
) {
  NAsyncWorkPtr work(this->m_pools, g_loadPartyInfoPoolName);

  if (work.Get()) {
    static_cast<CLoadPartyInfoAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token, queryAccountId);
    this->Submit(work);
  }
}

// OFFSET: 0x00574950
// FUNCTION: fn_02508
void CAsyncWorkMgr::LoadPartyList(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, int search, CAdvenPartyInfo filter, int page
) {
  NAsyncWorkPtr work(this->m_pools, g_loadPartyListPoolName);

  if (work.Get()) {
    static_cast<CLoadPartyListAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token, filter, search, page);
    this->Submit(work);
  }
}

// OFFSET: 0x00574B30
// FUNCTION: fn_02509
void CAsyncWorkMgr::LoadStore(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, int type
) {
  NAsyncWorkPtr work(this->m_pools, g_mcStoreInfoBackupLoadPoolName);

  if (work.Get()) {
    static_cast<CMCStoreInfoBackupLoadAsyncWork*>(work.Get())->Bind(url, accountId, characterId, world, token, type);
    this->Submit(work);
  }
}

// OFFSET: 0x00574C70
// FUNCTION: fn_02510
void CAsyncWorkMgr::SaveStore(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::string data, int type
) {
  NAsyncWorkPtr work(this->m_pools, g_mcStoreInfoBackupSavePoolName);

  if (work.Get()) {
    static_cast<CMCStoreInfoBackupSaveAsyncWork*>(work.Get())->Bind(url, accountId, characterId, world, token, data, type);
    this->Submit(work);
  }
}

// OFFSET: 0x00574DD0
// FUNCTION: fn_02511
void CAsyncWorkMgr::RegisterParty(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, CAdvenPartyInfo info
) {
  NAsyncWorkPtr work(this->m_pools, g_registerPartyPoolName);

  if (work.Get()) {
    static_cast<CRegisterPartyAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token, info);
    this->Submit(work);
  }
}

// OFFSET: 0x00574F40
// FUNCTION: fn_02512
void CAsyncWorkMgr::StopRecruiting(std::uint32_t accountId, std::string world, std::string url, std::string token, std::uint32_t masterAccountId) {
  NAsyncWorkPtr work(this->m_pools, g_stopRecruitingPoolName);

  if (work.Get()) {
    static_cast<CStopRecruitingAsyncWork*>(work.Get())->Bind(accountId, world, url, token, masterAccountId);
    this->Submit(work);
  }
}
