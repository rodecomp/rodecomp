#include "Contents/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/AccountLinkedUserDataSaveAsyncWork.h"
#include "Contents/AsyncWorkMgr.h"
#include "Contents/AsyncWorkRecycleObjectPoolMgr.h"
#include "Contents/ChangePartyMasterAsyncWork.h"
#include "Contents/CharacterLinkedUserDataLoadAsyncWork.h"
#include "Contents/CharacterLinkedUserDataSaveAsyncWork.h"
#include "Contents/EmblemDataDownloadAsyncWork.h"
#include "Contents/EmblemDataUploadAsyncWork.h"
#include "Contents/LoadMyPartyAsyncWork.h"
#include "Contents/LoadPartyInfoAsyncWork.h"
#include "Contents/LoadPartyListAsyncWork.h"
#include "Contents/MCStoreInfoBackupAsyncWork.h"
#include "Contents/RegisterPartyAsyncWork.h"
#include "Contents/StopRecruitingAsyncWork.h"

#include <new>

// STATIC: 0x0138968C
// GLOBAL: gl_24349
CAsyncWorkMgr* g_asyncWorkMgr;

// OFFSET: 0x00573C50
// FUNCTION: fn_02497
CAsyncWorkMgr::CAsyncWorkMgr(std::uint32_t workers) : NAsyncWorkMgr(workers), m_ready(false), m_pools(nullptr) {
  this->m_pools = new (std::nothrow) CAsyncWorkRecycleObjectPoolMgr;

  if (this->m_pools) {
    if (this->m_pools->Ready()) {
      this->m_ready = true;
    } else {
      delete this->m_pools;
      this->m_pools = nullptr;
    }
  }
}

// OFFSET: 0x00573B20
// FUNCTION: fn_02496
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
void CAsyncWorkMgr::LoadAccount(
  std::uint32_t accountId,
  std::string world,
  std::string url,
  AccountConfigLoad& context
) {
  NAsyncWorkPtr work(this->m_pools, "AccountLinkedUserData_Load");

  if (work.Get()) {
    static_cast<CAccountLinkedUserDataLoadAsyncWork*>(work.Get())->Bind(accountId, world, url, context);
    this->Submit(work);
  }
}

// OFFSET: 0x00573EC0
// FUNCTION: fn_02500
void CAsyncWorkMgr::SaveAccount(
  std::uint32_t accountId,
  std::string world,
  std::string url,
  std::string token,
  std::string data
) {
  NAsyncWorkPtr work(this->m_pools, "AccountLinkedUserData_Save");

  if (work.Get()) {
    static_cast<CAccountLinkedUserDataSaveAsyncWork*>(work.Get())->Bind(accountId, world, url, token, data);
    this->Submit(work);
  }
}

// OFFSET: 0x00574010
// FUNCTION: fn_02501
void CAsyncWorkMgr::ChangePartyMaster(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string characterName,
  std::uint32_t targetAccountId,
  std::uint32_t targetCharacterId,
  std::string targetName,
  std::string world,
  std::string url,
  std::string token,
  WindowManagerWindows& windows
) {
  NAsyncWorkPtr work(this->m_pools, "Change_PartyMaster");

  if (work.Get()) {
    static_cast<CChangePartyMasterAsyncWork*>(work.Get())
      ->Bind(
        accountId, characterId, characterName, targetAccountId, targetCharacterId, targetName, world, url, token,
        windows
      );
    this->Submit(work);
  }
}

// OFFSET: 0x00574190
// FUNCTION: fn_02502
void CAsyncWorkMgr::LoadCharacter(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  CharacterConfigLoad& context
) {
  NAsyncWorkPtr work(this->m_pools, "CharacterLinkedUserData_Load");

  if (work.Get()) {
    static_cast<CCharacterLinkedUserDataLoadAsyncWork*>(work.Get())
      ->Bind(accountId, characterId, world, url, token, context);
    this->Submit(work);
  }
}

// OFFSET: 0x005742D0
// FUNCTION: fn_02503
void CAsyncWorkMgr::SaveCharacter(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  std::string data
) {
  NAsyncWorkPtr work(this->m_pools, "CharacterLinkedUserData_Save");

  if (work.Get()) {
    static_cast<CCharacterLinkedUserDataSaveAsyncWork*>(work.Get())
      ->Bind(accountId, characterId, world, url, token, data);
    this->Submit(work);
  }
}

// OFFSET: 0x00574430
// FUNCTION: fn_02504
void CAsyncWorkMgr::DownloadEmblem(
  std::string url,
  std::uint32_t accountId,
  std::string token,
  std::string world,
  std::uint32_t guildId,
  std::uint32_t version,
  CConnection& connection
) {
  NAsyncWorkPtr work(this->m_pools, "Emblem_Download");

  if (work.Get()) {
    static_cast<CEmblemDataDownloadAsyncWork*>(work.Get())
      ->Bind(url, accountId, token, world, guildId, version, connection);
    this->Submit(work);
  }
}

// OFFSET: 0x00574570
// FUNCTION: fn_02505
void CAsyncWorkMgr::UploadEmblem(
  std::string url,
  std::uint32_t accountId,
  std::string token,
  std::string world,
  std::uint32_t guildId,
  std::string path,
  CEmblemDataMgr& owner,
  EmblemUploadContext& context
) {
  NAsyncWorkPtr work(this->m_pools, "Emblem_Upload");

  if (work.Get()) {
    static_cast<CEmblemDataUploadAsyncWork*>(work.Get())
      ->Bind(url, accountId, token, world, guildId, path, owner, context);
    this->Submit(work);
  }
}

// OFFSET: 0x005746D0
// FUNCTION: fn_02506
void CAsyncWorkMgr::LoadMyParty(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  WindowManagerWindows& windows
) {
  NAsyncWorkPtr work(this->m_pools, "MyParty_Load");

  if (work.Get()) {
    static_cast<CLoadMyPartyAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token, windows);
    this->Submit(work);
  }
}

// OFFSET: 0x00574810
// FUNCTION: fn_02507
void CAsyncWorkMgr::LoadPartyInfo(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  std::uint32_t queryAccountId,
  PartyInfoContext& context
) {
  NAsyncWorkPtr work(this->m_pools, "PartyInfo_Load");

  if (work.Get()) {
    static_cast<CLoadPartyInfoAsyncWork*>(work.Get())
      ->Bind(accountId, characterId, world, url, token, queryAccountId, context);
    this->Submit(work);
  }
}

// OFFSET: 0x00574950
// FUNCTION: fn_02508
void CAsyncWorkMgr::LoadPartyList(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  int search,
  CAdvenPartyInfo filter,
  int page,
  WindowManagerWindows& windows
) {
  NAsyncWorkPtr work(this->m_pools, "PartyList_Load");

  if (work.Get()) {
    static_cast<CLoadPartyListAsyncWork*>(work.Get())
      ->Bind(accountId, characterId, world, url, token, filter, search, page, windows);
    this->Submit(work);
  }
}

// OFFSET: 0x00574B30
// FUNCTION: fn_02509
void CAsyncWorkMgr::LoadStore(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  int type,
  CMCStoreInfoBackup& owner,
  StoreBackupContext& context
) {
  NAsyncWorkPtr work(this->m_pools, "MerchantStoreInformation_Load");

  if (work.Get()) {
    static_cast<CMCStoreInfoBackupLoadAsyncWork*>(work.Get())
      ->Bind(url, accountId, characterId, world, token, type, owner, context);
    this->Submit(work);
  }
}

// OFFSET: 0x00574C70
// FUNCTION: fn_02510
void CAsyncWorkMgr::SaveStore(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  std::string data,
  int type,
  CMCStoreInfoBackup& owner,
  StoreBackupContext& context
) {
  NAsyncWorkPtr work(this->m_pools, "MerchantStoreInformation_Save");

  if (work.Get()) {
    static_cast<CMCStoreInfoBackupSaveAsyncWork*>(work.Get())
      ->Bind(url, accountId, characterId, world, token, data, type, owner, context);
    this->Submit(work);
  }
}

// OFFSET: 0x00574DD0
// FUNCTION: fn_02511
void CAsyncWorkMgr::RegisterParty(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  CAdvenPartyInfo info,
  RegisterPartyContext& context
) {
  NAsyncWorkPtr work(this->m_pools, "Register_Party");

  if (work.Get()) {
    static_cast<CRegisterPartyAsyncWork*>(work.Get())->Bind(accountId, characterId, world, url, token, info, context);
    this->Submit(work);
  }
}

// OFFSET: 0x00574F40
// FUNCTION: fn_02512
void CAsyncWorkMgr::StopRecruiting(
  std::uint32_t accountId,
  std::string world,
  std::string url,
  std::string token,
  std::uint32_t masterAccountId,
  StopRecruitingContext& context
) {
  NAsyncWorkPtr work(this->m_pools, "Stop_Recruiting");

  if (work.Get()) {
    static_cast<CStopRecruitingAsyncWork*>(work.Get())->Bind(accountId, world, url, token, masterAccountId, context);
    this->Submit(work);
  }
}
