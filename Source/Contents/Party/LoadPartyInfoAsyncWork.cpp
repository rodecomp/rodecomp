#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyInfoAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25603
const char* g_loadPartyInfoPoolName = OBFUSCATE_BYTES_TEXT("PartyInfo_Load", 0x93E7A3A1F587CDC5ULL);

// OFFSET: 0x005D0470
// FUNCTION: fn_03692
CLoadPartyInfoAsyncWork::CLoadPartyInfoAsyncWork()
    : NAsyncWork(g_loadPartyInfoPoolName), m_accountId(0), m_characterId(0), m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknown", 0x2FF3C5036D070127ULL)), sourceFile, 25) {
}

// INLINED
CLoadPartyInfoAsyncWork::~CLoadPartyInfoAsyncWork() = default;

// OFFSET: 0x005D0BB0
// FUNCTION: fn_03695
void CLoadPartyInfoAsyncWork::Reset() {
}

// OFFSET: 0x005D0930
// FUNCTION: fn_03694
void CLoadPartyInfoAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::uint32_t queryAccountId
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_queryAccountId = queryAccountId;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x17359BF953C367B3ULL)), sourceFile, 52);
}

// OFFSET: 0x005D0C80
// FUNCTION: fn_03697
bool CLoadPartyInfoAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  std::string world = CCURLMgr::GetInstance()->ToUtf8(this->m_world);
  char query[64] = {};
  StringFormat(query, sizeof(query), _TRUNCATE, "%lu", this->m_queryAccountId);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x3375B1C7DBB5FD55ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x1D6D75EDE7CD311FULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x59278F63D79F3DA1ULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x812BE72BCD5707B7ULL), world, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("QueryAID", 0xD3755DD95F47A909ULL), query, 4);

  if (handler.Perform() == 0) {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("success", 0x31D7C1EDA345E36DULL)), sourceFile, 97);
  }

  return false;
}

// OFFSET: 0x005D0BC0
// FUNCTION: fn_03696
void CLoadPartyInfoAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompletePartyInfo(this->m_diagnostic, this->m_response);
}

template void std::deque<CLoadPartyInfoAsyncWork*>::push_back(CLoadPartyInfoAsyncWork*&&);
