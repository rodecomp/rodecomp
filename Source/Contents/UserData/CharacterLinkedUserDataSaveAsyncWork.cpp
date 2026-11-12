#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataSaveAsyncWork.h"
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
                                "aracterLinkedUserDataSaveAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25608
const char* g_characterLinkedUserDataSavePoolName = OBFUSCATE_BYTES_TEXT("CharacterLinkedUserData_Save", 0x17359BF953C367B3ULL);

// OFFSET: 0x005E7C10
// FUNCTION: fn_03944
CCharacterLinkedUserDataSaveAsyncWork::CCharacterLinkedUserDataSaveAsyncWork()
    : NAsyncWork(g_characterLinkedUserDataSavePoolName), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0xC52FB5FBB94FF917ULL)), sourceFile, 28) {
}

// INLINED
CCharacterLinkedUserDataSaveAsyncWork::~CCharacterLinkedUserDataSaveAsyncWork() = default;

// OFFSET: 0x005E8370
// FUNCTION: fn_03947
void CCharacterLinkedUserDataSaveAsyncWork::Reset() {
}

// OFFSET: 0x005E80D0
// FUNCTION: fn_03946
void CCharacterLinkedUserDataSaveAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string url, std::string token, std::string data
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x93E7A3A1F587CDC5ULL)), sourceFile, 54);
}

// OFFSET: 0x005E8390
// FUNCTION: fn_03949
bool CCharacterLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  char characterId[64] = {};
  StringFormat(characterId, sizeof(characterId), _TRUNCATE, "%lu", this->m_characterId);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  std::string data = CCURLMgr::GetInstance()->ToUtf8(this->m_data);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x59278F63D79F3DA1ULL), accountId, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x812BE72BCD5707B7ULL), characterId, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x3375B1C7DBB5FD55ULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x1D6D75EDE7CD311FULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("data", 0xD98F03A52D35636DULL), data, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0x0F0F8B6B8F9D19D1ULL)), sourceFile, 98);
  } else {
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("good", 0xD3755DD95F47A909ULL)), sourceFile, 102);
  }

  return false;
}

// OFFSET: 0x005E8380
// FUNCTION: fn_03948
void CCharacterLinkedUserDataSaveAsyncWork::Complete() {
  g_characterLinkedUserDataMgr->CompleteSave();
}

template void std::deque<CCharacterLinkedUserDataSaveAsyncWork*>::push_back(CCharacterLinkedUserDataSaveAsyncWork*&&);
