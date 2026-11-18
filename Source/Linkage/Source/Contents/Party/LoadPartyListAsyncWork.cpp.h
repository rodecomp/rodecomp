#pragma once

#include <string>
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyListAsyncWork.h"
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

extern template std::string& std::string::assign(const char*, std::string::size_type);
template __declspec(noinline) std::string& std::string::operator=(std::string&&) noexcept;
template void std::deque<CLoadPartyListAsyncWork*>::push_back(CLoadPartyListAsyncWork*&&);
template __declspec(noinline) AsyncDiagnostic& AsyncDiagnostic::operator=<AsyncDiagnostic, bool, 0>(AsyncDiagnostic&&) noexcept;
