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

namespace {
  constexpr char loadSource[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\MCStoreBackUpData\\CMCStoreInfoBackupLoadAsyncWork."
                                "cpp";
  constexpr char saveSource[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\MCStoreBackUpData\\CMCStoreInfoBackupSaveAsyncWork."
                                "cpp";

  // INLINED
  __forceinline AsyncDiagnostic Diagnostic(bool success, const char* text, const char* file, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, file, &line);
  }

  // INLINED
    // INLINED
  }

// GLOBAL: gl_25611
const char* g_mcStoreInfoBackupLoadPoolName = OBFUSCATE_BYTES_TEXT("MerchantStoreInformation_Load", 0xA7471985F93F9BDFULL);

// GLOBAL: gl_25612
const char* g_mcStoreInfoBackupSavePoolName = OBFUSCATE_BYTES_TEXT("MerchantStoreInformation_Save", 0x73D1674B61DF7B0BULL);

// OFFSET: 0x00607B10
// FUNCTION: fn_04241
CMCStoreInfoBackupLoadAsyncWork::CMCStoreInfoBackupLoadAsyncWork()
    : NAsyncWork(g_mcStoreInfoBackupLoadPoolName), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0xF535E561137F138DULL)), loadSource, 28), m_type(0) {
}

// INLINED
CMCStoreInfoBackupLoadAsyncWork::~CMCStoreInfoBackupLoadAsyncWork() = default;

// OFFSET: 0x00608270
// FUNCTION: fn_04244
void CMCStoreInfoBackupLoadAsyncWork::Reset() {
}

// OFFSET: 0x00607FE0
// FUNCTION: fn_04243
void CMCStoreInfoBackupLoadAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string token, int type
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_response = "";
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0xD3E5516F89F161D1ULL)), loadSource, 55);
  this->m_type = type;
}

// OFFSET: 0x00608350
// FUNCTION: fn_04246
bool CMCStoreInfoBackupLoadAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  char type[64] = {};
  StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_type);

  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x2B8725D7E95325F5ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x899D11BB83695B0DULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0xBDADDD87C5A50B6BULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("Type", 0x7989830DE73B9DBDULL), type, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x2F2701A7E56D3D89ULL), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = handler.Response();
    AsyncDiagnostic diagnostic = Diagnostic(true, OBFUSCATE_BYTES_TEXT("good", 0x6B3301E1A3079125ULL), loadSource, 115);
    this->m_diagnostic = std::move(diagnostic);
  }

  return false;
}

// OFFSET: 0x00608280
// FUNCTION: fn_04245
void CMCStoreInfoBackupLoadAsyncWork::Complete() {
  g_session.m_storeInfoBackup.CompleteLoad(this->m_diagnostic, this->m_accountId, this->m_characterId, this->m_accountName, this->m_response);
}

// OFFSET: 0x00608D60
// FUNCTION: fn_04247
CMCStoreInfoBackupSaveAsyncWork::CMCStoreInfoBackupSaveAsyncWork()
    : NAsyncWork(g_mcStoreInfoBackupSavePoolName), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x3FAF85D995034FD7ULL)), saveSource, 30), m_type(0) {
}

// INLINED
CMCStoreInfoBackupSaveAsyncWork::~CMCStoreInfoBackupSaveAsyncWork() = default;

// OFFSET: 0x00609520
// FUNCTION: fn_04250
void CMCStoreInfoBackupSaveAsyncWork::Reset() {
}

// OFFSET: 0x00609270
// FUNCTION: fn_04249
void CMCStoreInfoBackupSaveAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string token, std::string data, int type
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x490FFB43A12F0DB9ULL)), saveSource, 57);
  this->m_type = type;
}

// OFFSET: 0x006095F0
// FUNCTION: fn_04252
bool CMCStoreInfoBackupSaveAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  char type[64] = {};
  StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_type);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);

  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0xD3755DD95F47A909ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x31D7C1EDA345E36DULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0xD98F03A52D35636DULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x0F0F8B6B8F9D19D1ULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("Type", 0x3375B1C7DBB5FD55ULL), type, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("data", 0x1D6D75EDE7CD311FULL), this->m_data, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0x59278F63D79F3DA1ULL)), saveSource, 112);
    this->m_response = handler.Response();
  } else {
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("good", 0x812BE72BCD5707B7ULL)), saveSource, 122);
    this->m_response = handler.Response();
  }

  return false;
}

// OFFSET: 0x00609530
// FUNCTION: fn_04251
void CMCStoreInfoBackupSaveAsyncWork::Complete() {
  g_session.m_storeInfoBackup.CompleteSave(this->m_diagnostic, this->m_response);
}

template void std::deque<CMCStoreInfoBackupSaveAsyncWork*>::push_back(CMCStoreInfoBackupSaveAsyncWork*&&);
template void std::deque<CMCStoreInfoBackupLoadAsyncWork*>::push_back(CMCStoreInfoBackupLoadAsyncWork*&&);
