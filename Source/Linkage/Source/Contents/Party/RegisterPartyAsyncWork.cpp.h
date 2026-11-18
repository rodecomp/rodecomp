#pragma once

#include <string>
#include "Client/Session/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/RegisterPartyAsyncWork.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <cstdio>
#include <windows.h>
#include <timeapi.h>

extern template std::string& std::string::operator=(std::string&&) noexcept;
template void std::deque<CRegisterPartyAsyncWork*>::push_back(CRegisterPartyAsyncWork*&&);
template __declspec(noinline) AsyncDiagnostic& AsyncDiagnostic::operator=<AsyncDiagnostic, bool, 0>(AsyncDiagnostic&&) noexcept;
