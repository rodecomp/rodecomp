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

// INLINED
CAsyncWorkRecycleObjectPoolMgr::~CAsyncWorkRecycleObjectPoolMgr() {
  for (const auto& entry : this->m_pools) {
    delete entry.second;
  }
}

// OFFSET: 0x00578F30
// FUNCTION: fn_02566
bool CAsyncWorkRecycleObjectPoolMgr::RegisterPools() {
  if (!this->m_pools.insert(std::make_pair(g_accountLinkedUserDataLoadPoolName, new (std::nothrow) NRecycleObjectPool<CAccountLinkedUserDataLoadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_accountLinkedUserDataSavePoolName, new (std::nothrow) NRecycleObjectPool<CAccountLinkedUserDataSaveAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_characterLinkedUserDataLoadPoolName, new (std::nothrow) NRecycleObjectPool<CCharacterLinkedUserDataLoadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_characterLinkedUserDataSavePoolName, new (std::nothrow) NRecycleObjectPool<CCharacterLinkedUserDataSaveAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_emblemDataUploadPoolName, new (std::nothrow) NRecycleObjectPool<CEmblemDataUploadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_emblemDataDownloadPoolName, new (std::nothrow) NRecycleObjectPool<CEmblemDataDownloadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_mcStoreInfoBackupLoadPoolName, new (std::nothrow) NRecycleObjectPool<CMCStoreInfoBackupLoadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_mcStoreInfoBackupSavePoolName, new (std::nothrow) NRecycleObjectPool<CMCStoreInfoBackupSaveAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_loadPartyListPoolName, new (std::nothrow) NRecycleObjectPool<CLoadPartyListAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_loadMyPartyPoolName, new (std::nothrow) NRecycleObjectPool<CLoadMyPartyAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_registerPartyPoolName, new (std::nothrow) NRecycleObjectPool<CRegisterPartyAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_stopRecruitingPoolName, new (std::nothrow) NRecycleObjectPool<CStopRecruitingAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_changePartyMasterPoolName, new (std::nothrow) NRecycleObjectPool<CChangePartyMasterAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair(g_loadPartyInfoPoolName, new (std::nothrow) NRecycleObjectPool<CLoadPartyInfoAsyncWork>(2))).second) {
    return false;
  }
  return true;
}
