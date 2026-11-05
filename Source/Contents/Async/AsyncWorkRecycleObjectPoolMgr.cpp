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
  if (!this->m_pools.insert(std::make_pair("AccountLinkedUserData_Load", new (std::nothrow) NRecycleObjectPool<CAccountLinkedUserDataLoadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("AccountLinkedUserData_Save", new (std::nothrow) NRecycleObjectPool<CAccountLinkedUserDataSaveAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("CharacterLinkedUserData_Load", new (std::nothrow) NRecycleObjectPool<CCharacterLinkedUserDataLoadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("CharacterLinkedUserData_Save", new (std::nothrow) NRecycleObjectPool<CCharacterLinkedUserDataSaveAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("Emblem_Upload", new (std::nothrow) NRecycleObjectPool<CEmblemDataUploadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("Emblem_Download", new (std::nothrow) NRecycleObjectPool<CEmblemDataDownloadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("MerchantStoreInformation_Load", new (std::nothrow) NRecycleObjectPool<CMCStoreInfoBackupLoadAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("MerchantStoreInformation_Save", new (std::nothrow) NRecycleObjectPool<CMCStoreInfoBackupSaveAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("PartyList_Load", new (std::nothrow) NRecycleObjectPool<CLoadPartyListAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("MyParty_Load", new (std::nothrow) NRecycleObjectPool<CLoadMyPartyAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("Register_Party", new (std::nothrow) NRecycleObjectPool<CRegisterPartyAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("Stop_Recruiting", new (std::nothrow) NRecycleObjectPool<CStopRecruitingAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("Change_PartyMaster", new (std::nothrow) NRecycleObjectPool<CChangePartyMasterAsyncWork>(2))).second) {
    return false;
  }
  if (!this->m_pools.insert(std::make_pair("PartyInfo_Load", new (std::nothrow) NRecycleObjectPool<CLoadPartyInfoAsyncWork>(2))).second) {
    return false;
  }
  return true;
}
