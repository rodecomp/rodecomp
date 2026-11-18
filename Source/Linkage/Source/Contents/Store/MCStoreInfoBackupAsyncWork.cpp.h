#pragma once

#include <string>
#include "Contents/Store/MCStoreInfoBackup.h"
#include "Contents/Store/MCStoreInfoBackupAsyncWork.h"
#include "Client/Session/Session.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <array>
#include <cstdio>
#include <windows.h>
#include <timeapi.h>

extern template std::string& std::string::assign(const char*, std::string::size_type);
template void std::deque<CMCStoreInfoBackupSaveAsyncWork*>::push_back(CMCStoreInfoBackupSaveAsyncWork*&&);
template void std::deque<CMCStoreInfoBackupLoadAsyncWork*>::push_back(CMCStoreInfoBackupLoadAsyncWork*&&);
