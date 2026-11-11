#pragma once

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

extern template void std::deque<CAccountLinkedUserDataLoadAsyncWork*>::push_back(CAccountLinkedUserDataLoadAsyncWork*&&);
extern template void std::deque<CAccountLinkedUserDataSaveAsyncWork*>::push_back(CAccountLinkedUserDataSaveAsyncWork*&&);
extern template void std::deque<CChangePartyMasterAsyncWork*>::push_back(CChangePartyMasterAsyncWork*&&);
extern template void std::deque<CCharacterLinkedUserDataLoadAsyncWork*>::push_back(CCharacterLinkedUserDataLoadAsyncWork*&&);
extern template void std::deque<CCharacterLinkedUserDataSaveAsyncWork*>::push_back(CCharacterLinkedUserDataSaveAsyncWork*&&);
extern template void std::deque<CEmblemDataDownloadAsyncWork*>::push_back(CEmblemDataDownloadAsyncWork*&&);
extern template void std::deque<CEmblemDataUploadAsyncWork*>::push_back(CEmblemDataUploadAsyncWork*&&);
extern template void std::deque<CLoadMyPartyAsyncWork*>::push_back(CLoadMyPartyAsyncWork*&&);
extern template void std::deque<CLoadPartyInfoAsyncWork*>::push_back(CLoadPartyInfoAsyncWork*&&);
extern template void std::deque<CLoadPartyListAsyncWork*>::push_back(CLoadPartyListAsyncWork*&&);
extern template void std::deque<CMCStoreInfoBackupSaveAsyncWork*>::push_back(CMCStoreInfoBackupSaveAsyncWork*&&);
extern template void std::deque<CMCStoreInfoBackupLoadAsyncWork*>::push_back(CMCStoreInfoBackupLoadAsyncWork*&&);
extern template void std::deque<CRegisterPartyAsyncWork*>::push_back(CRegisterPartyAsyncWork*&&);
extern template void std::deque<CStopRecruitingAsyncWork*>::push_back(CStopRecruitingAsyncWork*&&);
