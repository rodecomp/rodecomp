#include "Contents/Emblems/EmblemDataDownloadAsyncWork.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <stdio.h>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataDownloadAsyncWork.cpp";

  // INLINED
  __forceinline AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }

  // INLINED
  }

// GLOBAL: gl_25609
const char* g_emblemDataDownloadPoolName = OBFUSCATE_BYTES_TEXT("Emblem_Download", 0x17359BF953C367B3ULL);

// OFFSET: 0x005E9C00
// FUNCTION: fn_03963
CEmblemDataDownloadAsyncWork::CEmblemDataDownloadAsyncWork()
    : NAsyncWork(g_emblemDataDownloadPoolName), m_accountId(0), m_guildId(0), m_version(0), m_responseCode(500),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0xC52FB5FBB94FF917ULL)), sourceFile, 29) {
}

// INLINED
CEmblemDataDownloadAsyncWork::~CEmblemDataDownloadAsyncWork() = default;

// OFFSET: 0x005EA350
// FUNCTION: fn_03966
void CEmblemDataDownloadAsyncWork::Reset() {
}

// OFFSET: 0x005EA0D0
// FUNCTION: fn_03965
void CEmblemDataDownloadAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::uint32_t version
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_url = url;
  this->m_accountId = accountId;
  this->m_token = token;
  this->m_accountName = accountName;
  this->m_guildId = guildId;
  this->m_version = version;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x93E7A3A1F587CDC5ULL)), sourceFile, 57);
}

// OFFSET: 0x005EA430
// FUNCTION: fn_03968
bool CEmblemDataDownloadAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char guild[64] = {};
  StringFormat(guild, sizeof(guild), _TRUNCATE, "%lu", this->m_guildId);
  char version[10] = {};
  StringFormat(version, sizeof(version), _TRUNCATE, "%lu", this->m_version);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);

  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x59278F63D79F3DA1ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x812BE72BCD5707B7ULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x3375B1C7DBB5FD55ULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GDID", 0x1D6D75EDE7CD311FULL), guild, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("Version", 0xD98F03A52D35636DULL), version, 4);

  if (handler.Perform() == 0) {
    this->m_responseCode = static_cast<int>(handler.ResponseCode());
    this->m_response = handler.Response();
    AsyncDiagnostic diagnostic = Diagnostic(true, OBFUSCATE_BYTES_TEXT("good", 0x0F0F8B6B8F9D19D1ULL), 96);
    this->m_diagnostic = std::move(diagnostic);
  } else {
    AsyncDiagnostic diagnostic = Diagnostic(false, OBFUSCATE_BYTES_TEXT("fail", 0xD3755DD95F47A909ULL), 105);
    this->m_diagnostic = std::move(diagnostic);
  }

  return false;
}

// OFFSET: 0x005EA360
// FUNCTION: fn_03967
void CEmblemDataDownloadAsyncWork::Complete() {
  CEmblemDataMgr* emblems = g_emblemDataMgr;
  emblems->CompleteDownload(this->m_diagnostic, this->m_responseCode, this->m_response, this->m_guildId, this->m_version);
}

template void std::deque<CEmblemDataDownloadAsyncWork*>::push_back(CEmblemDataDownloadAsyncWork*&&);
