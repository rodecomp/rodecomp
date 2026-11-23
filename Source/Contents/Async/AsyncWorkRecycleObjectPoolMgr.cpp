#include "Contents/UserData/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/AccountLinkedUserDataSaveAsyncWork.h"
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

#include <utility>
#include <new>

// OFFSET: 0x00576660
// FUNCTION: fn_02531
CAsyncWorkRecycleObjectPoolMgr::CAsyncWorkRecycleObjectPoolMgr() : m_registered(false) {
  if (this->RegisterPools()) {
    this->m_registered = true;
  }
}

// OFFSET: 0x00577400
// FUNCTION: fn_46369
__declspec(noinline) CAsyncWorkRecycleObjectPoolMgr::~CAsyncWorkRecycleObjectPoolMgr() {
  this->DeletePools();
}

// OFFSET: 0x0057A6C0
// FUNCTION: fn_45925
void CAsyncWorkRecycleObjectPoolMgr::DeletePools() {
  for (auto entry = this->m_pools.begin(); entry != this->m_pools.end(); ++entry) {
    delete entry->second;
  }
}

// OFFSET: 0x00578F30
// FUNCTION: fn_02566
bool CAsyncWorkRecycleObjectPoolMgr::RegisterPools() {
  const auto inserted1 = this->m_pools.insert(std::make_pair(g_accountLinkedUserDataLoadPoolName, new (std::nothrow) NRecycleObjectPool<CAccountLinkedUserDataLoadAsyncWork>(2)));
  if (!inserted1.second) {
    return false;
  }
  const auto inserted2 = this->m_pools.insert(std::make_pair(g_accountLinkedUserDataSavePoolName, new (std::nothrow) NRecycleObjectPool<CAccountLinkedUserDataSaveAsyncWork>(2)));
  if (!inserted2.second) {
    return false;
  }
  const auto inserted3 = this->m_pools.insert(std::make_pair(g_characterLinkedUserDataLoadPoolName, new (std::nothrow) NRecycleObjectPool<CCharacterLinkedUserDataLoadAsyncWork>(2)));
  if (!inserted3.second) {
    return false;
  }
  const auto inserted4 = this->m_pools.insert(std::make_pair(g_characterLinkedUserDataSavePoolName, new (std::nothrow) NRecycleObjectPool<CCharacterLinkedUserDataSaveAsyncWork>(2)));
  if (!inserted4.second) {
    return false;
  }
  const auto inserted5 = this->m_pools.insert(std::make_pair(g_emblemDataUploadPoolName, new (std::nothrow) NRecycleObjectPool<CEmblemDataUploadAsyncWork>(2)));
  if (!inserted5.second) {
    return false;
  }
  const auto inserted6 = this->m_pools.insert(std::make_pair(g_emblemDataDownloadPoolName, new (std::nothrow) NRecycleObjectPool<CEmblemDataDownloadAsyncWork>(2)));
  if (!inserted6.second) {
    return false;
  }
  const auto inserted7 = this->m_pools.insert(std::make_pair(g_mcStoreInfoBackupLoadPoolName, new (std::nothrow) NRecycleObjectPool<CMCStoreInfoBackupLoadAsyncWork>(2)));
  if (!inserted7.second) {
    return false;
  }
  const auto inserted8 = this->m_pools.insert(std::make_pair(g_mcStoreInfoBackupSavePoolName, new (std::nothrow) NRecycleObjectPool<CMCStoreInfoBackupSaveAsyncWork>(2)));
  if (!inserted8.second) {
    return false;
  }
  const auto inserted9 = this->m_pools.insert(std::make_pair(g_loadPartyListPoolName, new (std::nothrow) NRecycleObjectPool<CLoadPartyListAsyncWork>(2)));
  if (!inserted9.second) {
    return false;
  }
  const auto inserted10 = this->m_pools.insert(std::make_pair(g_loadMyPartyPoolName, new (std::nothrow) NRecycleObjectPool<CLoadMyPartyAsyncWork>(2)));
  if (!inserted10.second) {
    return false;
  }
  const auto inserted11 = this->m_pools.insert(std::make_pair(g_registerPartyPoolName, new (std::nothrow) NRecycleObjectPool<CRegisterPartyAsyncWork>(2)));
  if (!inserted11.second) {
    return false;
  }
  const auto inserted12 = this->m_pools.insert(std::make_pair(g_stopRecruitingPoolName, new (std::nothrow) NRecycleObjectPool<CStopRecruitingAsyncWork>(2)));
  if (!inserted12.second) {
    return false;
  }
  const auto inserted13 = this->m_pools.insert(std::make_pair(g_changePartyMasterPoolName, new (std::nothrow) NRecycleObjectPool<CChangePartyMasterAsyncWork>(2)));
  if (!inserted13.second) {
    return false;
  }
  const auto inserted14 = this->m_pools.insert(std::make_pair(g_loadPartyInfoPoolName, new (std::nothrow) NRecycleObjectPool<CLoadPartyInfoAsyncWork>(2)));
  if (!inserted14.second) {
    return false;
  }
  return true;
}
