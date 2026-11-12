#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/ChangePartyMasterAsyncWork.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CChangePartyMasterAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// OFFSET: 0x005CD7B0
// FUNCTION: fn_03677
CChangePartyMasterAsyncWork::CChangePartyMasterAsyncWork()
    : NAsyncWork("Change_PartyMaster"), m_accountId(0), m_characterId(0), m_token(""), m_worldName(""), m_response(""), m_url(""),
      m_diagnostic(false, std::string(DecryptName<0x2FF3C5036D070127ULL>({0x52, 0x6F, 0x6C, 0x03, 0x74, 0xAA, 0x9D, 0x2F})), sourceFile, 27) {
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
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x17359BF953C367B3ULL>({0xC6, 0x09, 0xA8, 0x3D, 0x8E, 0xF4, 0x5B, 0x17})), sourceFile, 55);
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
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>({0x14, 0xB4, 0xF1, 0xDB}), account, 4);
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>({0x58, 0x78, 0x89, 0xE7}), character, 4);
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>({0xE2, 0x55, 0xFE, 0xA5, 0x2D, 0xEE, 0x4A, 0x3C, 0xA1}), name, 4);
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>({0xE3, 0x68, 0x08, 0x8C, 0x62, 0xA3, 0x2B}), targetAccount, 4);
  handler.AddField(DecryptName<0xD3755DD95F47A909ULL>({0x5D, 0xC6, 0x18, 0x18, 0x90, 0x19, 0x75}), targetCharacter, 4);
  handler.AddField(DecryptName<0x31D7C1EDA345E36DULL>({0x39, 0x8C, 0x1A, 0xE0, 0x85, 0xA0, 0xA5, 0x7F, 0x0C, 0x8E, 0x20, 0xA3}), targetName, 4);
  handler.AddField(DecryptName<0xD98F03A52D35636DULL>({0x3A, 0x0C, 0x47, 0x41, 0xC1, 0x4D, 0xEE, 0xB4, 0x08, 0x63}), world, 4);
  handler.AddField(DecryptName<0x0F0F8B6B8F9D19D1ULL>({0x90, 0x6C, 0xE9, 0xE7, 0x3F, 0xE4, 0x64, 0x6A, 0xBF, 0x19}), this->m_token, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x91651D81F559F971ULL>({0x17, 0x98, 0x30, 0x99, 0x81})), sourceFile, 113);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x73D1674B61DF7B0BULL>({0x78, 0x0E, 0xBC, 0x02, 0x2E, 0x14, 0xA2, 0x73})), sourceFile, 122);
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
