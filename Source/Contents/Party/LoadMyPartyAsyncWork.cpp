#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadMyPartyAsyncWork.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadMyPartyAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25599
const char* g_loadMyPartyPoolName = DecryptName<0x93E7A3A1F587CDC5ULL>(CRYPT_BYTES(13, "MyParty_Load", 0x93E7A3A1F587CDC5ULL));

// OFFSET: 0x005CF1B0
// FUNCTION: fn_03685
CLoadMyPartyAsyncWork::CLoadMyPartyAsyncWork()
    : NAsyncWork("MyParty_Load"), m_accountId(0), m_characterId(0), m_diagnostic(false, std::string(DecryptName<0x2FF3C5036D070127ULL>({0x52, 0x6F, 0x6C, 0x03, 0x74, 0xAA, 0x9D, 0x2F})), sourceFile, 28) {
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
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x17359BF953C367B3ULL>({0xC6, 0x09, 0xA8, 0x3D, 0x8E, 0xF4, 0x5B, 0x17})), sourceFile, 49);
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
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>({0x14, 0xB4, 0xF1, 0xDB}), account, 4);
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>({0x58, 0x78, 0x89, 0xE7}), character, 4);
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>({0xF6, 0x52, 0xED, 0xBB, 0x07, 0xC1, 0x46, 0x34, 0xC4, 0x3D}), world, 4);
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>({0xF6, 0x72, 0x23, 0xA5, 0x7F, 0x88, 0x40, 0xE4, 0xD9, 0x07}), this->m_token, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0xD3755DD95F47A909ULL>({0x6F, 0xC8, 0x2E, 0x33, 0xD9})), sourceFile, 92);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x31D7C1EDA345E36DULL>({0x1E, 0x96, 0x26, 0xC0, 0x88, 0xB2, 0xA4, 0x31})), sourceFile, 101);
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
