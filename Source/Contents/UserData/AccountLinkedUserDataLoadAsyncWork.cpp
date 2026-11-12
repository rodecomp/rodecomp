#include "Contents/UserData/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
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

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataLoadAsyncWork.cpp";

  // INLINED
  __forceinline AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }

  // INLINED
    // INLINED
  }

// GLOBAL: gl_25600
const char* g_accountLinkedUserDataLoadPoolName = OBFUSCATE_BYTES_TEXT("AccountLinkedUserData_Load", 0x1D6D75EDE7CD311FULL);

// OFFSET: 0x005B9E50
// FUNCTION: fn_03440
CAccountLinkedUserDataLoadAsyncWork::CAccountLinkedUserDataLoadAsyncWork()
    : NAsyncWork(g_accountLinkedUserDataLoadPoolName), m_accountId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x59278F63D79F3DA1ULL)), sourceFile, 26) {
}

// INLINED
CAccountLinkedUserDataLoadAsyncWork::~CAccountLinkedUserDataLoadAsyncWork() = default;

// OFFSET: 0x005BA820
// FUNCTION: fn_03446
void CAccountLinkedUserDataLoadAsyncWork::Reset() {
}

// OFFSET: 0x005BA5D0
// FUNCTION: fn_03445
void CAccountLinkedUserDataLoadAsyncWork::Bind(std::uint32_t accountId, std::string accountName, std::string url) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_response = "";

  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x812BE72BCD5707B7ULL)), sourceFile, 50);
}

// OFFSET: 0x005BA910
// FUNCTION: fn_03449
bool CAccountLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);

  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0xC52FB5FBB94FF917ULL), accountId, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x93E7A3A1F587CDC5ULL), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = handler.Response();
    AsyncDiagnostic diagnostic = Diagnostic(true, OBFUSCATE_BYTES_TEXT("good", 0x2FF3C5036D070127ULL), 92);
    this->m_diagnostic = std::move(diagnostic);
  }

  return false;
}

// OFFSET: 0x005BA830
// FUNCTION: fn_03447
void CAccountLinkedUserDataLoadAsyncWork::Complete() {
  CAccountLinkedUserDataMgr* manager = g_accountLinkedUserDataMgr;
  manager->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_accountName, this->m_response
  );
}

template void std::deque<CAccountLinkedUserDataLoadAsyncWork*>::push_back(CAccountLinkedUserDataLoadAsyncWork*&&);
