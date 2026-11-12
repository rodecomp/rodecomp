#include "Contents/UserData/CharacterLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
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
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\CharacterLinkedUserData\\CCh"
                                "aracterLinkedUserDataLoadAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25607
const char* g_characterLinkedUserDataLoadPoolName = OBFUSCATE_BYTES_TEXT("CharacterLinkedUserData_Load", 0xC52FB5FBB94FF917ULL);

// OFFSET: 0x005E4A50
// FUNCTION: fn_03916
CCharacterLinkedUserDataLoadAsyncWork::CCharacterLinkedUserDataLoadAsyncWork()
    : NAsyncWork(g_characterLinkedUserDataLoadPoolName), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x17359BF953C367B3ULL)), sourceFile, 29) {
}

// INLINED
CCharacterLinkedUserDataLoadAsyncWork::~CCharacterLinkedUserDataLoadAsyncWork() = default;

// OFFSET: 0x005E5190
// FUNCTION: fn_03919
void CCharacterLinkedUserDataLoadAsyncWork::Reset() {
}

// OFFSET: 0x005E4F10
// FUNCTION: fn_03918
void CCharacterLinkedUserDataLoadAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string url, std::string token
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_response = "";

  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x2FF3C5036D070127ULL)), sourceFile, 55);
}

// OFFSET: 0x005E5280
// FUNCTION: fn_03921
bool CCharacterLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  char characterId[64] = {};
  StringFormat(characterId, sizeof(characterId), _TRUNCATE, "%lu", this->m_characterId);

  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x1D6D75EDE7CD311FULL), accountId, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x3375B1C7DBB5FD55ULL), characterId, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x812BE72BCD5707B7ULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x59278F63D79F3DA1ULL), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("good", 0x31D7C1EDA345E36DULL)), sourceFile, 103);
  }

  return false;
}

// OFFSET: 0x005E51A0
// FUNCTION: fn_03920
void CCharacterLinkedUserDataLoadAsyncWork::Complete() {
  CCharacterLinkedUserDataMgr* manager = g_characterLinkedUserDataMgr;
  manager->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_characterId, this->m_accountName, this->m_response
  );
}

template void std::deque<CCharacterLinkedUserDataLoadAsyncWork*>::push_back(CCharacterLinkedUserDataLoadAsyncWork*&&);
