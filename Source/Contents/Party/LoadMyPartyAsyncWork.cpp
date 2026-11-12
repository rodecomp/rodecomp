#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadMyPartyAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadMyPartyAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25599
const char* g_loadMyPartyPoolName = OBFUSCATE_BYTES_TEXT("MyParty_Load", 0x93E7A3A1F587CDC5ULL);

// OFFSET: 0x005CF1B0
// FUNCTION: fn_03685
CLoadMyPartyAsyncWork::CLoadMyPartyAsyncWork()
    : NAsyncWork(g_loadMyPartyPoolName), m_accountId(0), m_characterId(0), m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x2FF3C5036D070127ULL)), sourceFile, 28) {
}

// INLINED
CLoadMyPartyAsyncWork::~CLoadMyPartyAsyncWork() = default;

// OFFSET: 0x005CF8E0
// FUNCTION: fn_03688
void CLoadMyPartyAsyncWork::Reset() {
}

// OFFSET: 0x005CF670
// FUNCTION: fn_03687
void CLoadMyPartyAsyncWork::Bind(std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x17359BF953C367B3ULL)), sourceFile, 49);
}

// OFFSET: 0x005CF9B0
// FUNCTION: fn_03690
bool CLoadMyPartyAsyncWork::Execute() {
  char account[64] = {};
  char character[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  const std::string world = CCURLMgr::GetInstance()->ToUtf8(this->m_world);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x3375B1C7DBB5FD55ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x1D6D75EDE7CD311FULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x59278F63D79F3DA1ULL), world, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x812BE72BCD5707B7ULL), this->m_token, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0xD3755DD95F47A909ULL)), sourceFile, 92);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("success", 0x31D7C1EDA345E36DULL)), sourceFile, 101);
  }

  return false;
}

// OFFSET: 0x005CF8F0
// FUNCTION: fn_03689
void CLoadMyPartyAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompleteMyParty(this->m_diagnostic, this->m_response);
}

template void std::deque<CLoadMyPartyAsyncWork*>::push_back(CLoadMyPartyAsyncWork*&&);
