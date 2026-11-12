#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/AccountLinkedUserDataSaveAsyncWork.h"
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

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataSaveAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25601
const char* g_accountLinkedUserDataSavePoolName = OBFUSCATE_BYTES_TEXT("AccountLinkedUserData_Save", 0x1D6D75EDE7CD311FULL);

// OFFSET: 0x005BDAA0
// FUNCTION: fn_03478
CAccountLinkedUserDataSaveAsyncWork::CAccountLinkedUserDataSaveAsyncWork()
    : NAsyncWork(g_accountLinkedUserDataSavePoolName), m_accountId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x59278F63D79F3DA1ULL)), sourceFile, 26) {
}

// INLINED
CAccountLinkedUserDataSaveAsyncWork::~CAccountLinkedUserDataSaveAsyncWork() = default;

// OFFSET: 0x005BE1F0
// FUNCTION: fn_03481
void CAccountLinkedUserDataSaveAsyncWork::Reset() {
}

// OFFSET: 0x005BDF50
// FUNCTION: fn_03480
void CAccountLinkedUserDataSaveAsyncWork::Bind(std::uint32_t accountId, std::string accountName, std::string url, std::string token, std::string data) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_accountId = accountId;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x812BE72BCD5707B7ULL)), sourceFile, 54);
}

// OFFSET: 0x005BE210
// FUNCTION: fn_03483
bool CAccountLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0xC52FB5FBB94FF917ULL), accountId, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x93E7A3A1F587CDC5ULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x2FF3C5036D070127ULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("data", 0x17359BF953C367B3ULL), this->m_data, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0x91651D81F559F971ULL)), sourceFile, 102);
  } else {
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("good", 0x73D1674B61DF7B0BULL)), sourceFile, 106);
  }

  return false;
}

// OFFSET: 0x005BE200
// FUNCTION: fn_03482
void CAccountLinkedUserDataSaveAsyncWork::Complete() {
  g_accountLinkedUserDataMgr->CompleteSave();
}

template void std::deque<CAccountLinkedUserDataSaveAsyncWork*>::push_back(CAccountLinkedUserDataSaveAsyncWork*&&);
