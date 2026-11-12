#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/ChangePartyMasterAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CChangePartyMasterAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25602
const char* g_changePartyMasterPoolName = OBFUSCATE_BYTES_TEXT("Change_PartyMaster", 0x93E7A3A1F587CDC5ULL);

// OFFSET: 0x005CD7B0
// FUNCTION: fn_03677
CChangePartyMasterAsyncWork::CChangePartyMasterAsyncWork()
    : NAsyncWork(g_changePartyMasterPoolName), m_accountId(0), m_characterId(0), m_token(""), m_worldName(""), m_response(""), m_url(""),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x2FF3C5036D070127ULL)), sourceFile, 27) {
}

// INLINED
CChangePartyMasterAsyncWork::~CChangePartyMasterAsyncWork() = default;

// OFFSET: 0x005CE080
// FUNCTION: fn_03681
void CChangePartyMasterAsyncWork::Reset() {
}

// OFFSET: 0x005CDDA0
// FUNCTION: fn_03680
void CChangePartyMasterAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string characterName, std::uint32_t targetAccountId, std::uint32_t targetCharacterId,
  std::string targetName, std::string worldName, std::string url, std::string token
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_characterName = characterName;
  this->m_targetAccountId = targetAccountId;
  this->m_targetCharacterId = targetCharacterId;
  this->m_targetName = targetName;
  this->m_worldName = worldName;
  this->m_url = url;
  this->m_token = token;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x17359BF953C367B3ULL)), sourceFile, 55);
}

// OFFSET: 0x005CE150
// FUNCTION: fn_03683
bool CChangePartyMasterAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  char targetAccount[64] = {};
  StringFormat(targetAccount, sizeof(targetAccount), _TRUNCATE, "%lu", this->m_targetAccountId);
  char targetCharacter[64] = {};
  StringFormat(targetCharacter, sizeof(targetCharacter), _TRUNCATE, "%lu", this->m_targetCharacterId);
  std::string world = CCURLMgr::GetInstance()->ToUtf8(this->m_worldName);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_characterName);
  std::string targetName = CCURLMgr::GetInstance()->ToUtf8(this->m_targetName);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x3375B1C7DBB5FD55ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x1D6D75EDE7CD311FULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("CharName", 0x59278F63D79F3DA1ULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("To_AID", 0x812BE72BCD5707B7ULL), targetAccount, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("To_GID", 0xD3755DD95F47A909ULL), targetCharacter, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("To_CharName", 0x31D7C1EDA345E36DULL), targetName, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0xD98F03A52D35636DULL), world, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x0F0F8B6B8F9D19D1ULL), this->m_token, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0x91651D81F559F971ULL)), sourceFile, 113);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("success", 0x73D1674B61DF7B0BULL)), sourceFile, 122);
  }

  return false;
}

// OFFSET: 0x005CE090
// FUNCTION: fn_03682
void CChangePartyMasterAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompleteChangeMaster(this->m_diagnostic, this->m_response);
}

template void std::deque<CChangePartyMasterAsyncWork*>::push_back(CChangePartyMasterAsyncWork*&&);
