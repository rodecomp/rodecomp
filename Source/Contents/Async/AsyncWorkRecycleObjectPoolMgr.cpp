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
         insert.operator()<CEmblemDataUploadAsyncWork>("Emblem_Upload") && insert.operator()<CEmblemDataDownloadAsyncWork>("Emblem_Download") &&
         insert.operator()<CMCStoreInfoBackupLoadAsyncWork>("MerchantStoreInformation_Load") &&
         insert.operator()<CMCStoreInfoBackupSaveAsyncWork>("MerchantStoreInformation_Save") && insert.operator()<CLoadPartyListAsyncWork>("PartyList_Load") &&
         insert.operator()<CLoadMyPartyAsyncWork>("MyParty_Load") && insert.operator()<CRegisterPartyAsyncWork>("Register_Party") &&
         insert.operator()<CStopRecruitingAsyncWork>("Stop_Recruiting") && insert.operator()<CChangePartyMasterAsyncWork>("Change_PartyMaster") &&
         insert.operator()<CLoadPartyInfoAsyncWork>("PartyInfo_Load");
}
