#include "Contents/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/AccountLinkedUserDataSaveAsyncWork.h"
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

#include <memory>
#include <new>

// OFFSET: 0x00576660
CAsyncWorkRecycleObjectPoolMgr::CAsyncWorkRecycleObjectPoolMgr() : m_registered(false) {
  if (this->RegisterPools()) {
    this->m_registered = true;
  }
}

// OFFSET: 0x00578230
// FUNCTION: fn_02550
CAsyncWorkRecycleObjectPoolMgr::~CAsyncWorkRecycleObjectPoolMgr() {
  for (const auto& entry : this->m_pools) {
    delete entry.second;
  }
}

// OFFSET: 0x00578F30
// FUNCTION: fn_02566
bool CAsyncWorkRecycleObjectPoolMgr::RegisterPools() {
  // The original inserts all fourteen owning pointers, including null on
  // nothrow allocation failure. Duplicate keys fail registration, not null pools.
  const auto insert = [&]<class T>(const char* name) {
    std::unique_ptr<NRecycleObjectPoolInter> pool(new (std::nothrow) NRecycleObjectPool<T>(2));
    if (!this->m_pools.emplace(name, pool.get()).second) {
      return false;
    }

    pool.release();
    return true;
  };

  return insert.operator()<CAccountLinkedUserDataLoadAsyncWork>("AccountLinkedUserData_Load") &&
         insert.operator()<CAccountLinkedUserDataSaveAsyncWork>("AccountLinkedUserData_Save") &&
         insert.operator()<CCharacterLinkedUserDataLoadAsyncWork>("CharacterLinkedUserData_Load") &&
         insert.operator()<CCharacterLinkedUserDataSaveAsyncWork>("CharacterLinkedUserData_Save") &&
         insert.operator()<CEmblemDataUploadAsyncWork>("Emblem_Upload") &&
         insert.operator()<CEmblemDataDownloadAsyncWork>("Emblem_Download") &&
         insert.operator()<CMCStoreInfoBackupLoadAsyncWork>("MerchantStoreInformation_Load") &&
         insert.operator()<CMCStoreInfoBackupSaveAsyncWork>("MerchantStoreInformation_Save") &&
         insert.operator()<CLoadPartyListAsyncWork>("PartyList_Load") &&
         insert.operator()<CLoadMyPartyAsyncWork>("MyParty_Load") &&
         insert.operator()<CRegisterPartyAsyncWork>("Register_Party") &&
         insert.operator()<CStopRecruitingAsyncWork>("Stop_Recruiting") &&
         insert.operator()<CChangePartyMasterAsyncWork>("Change_PartyMaster") &&
         insert.operator()<CLoadPartyInfoAsyncWork>("PartyInfo_Load");
}
