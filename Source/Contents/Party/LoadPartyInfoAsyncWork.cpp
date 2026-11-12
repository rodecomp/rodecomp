#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyInfoAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25603
const char* g_loadPartyInfoPoolName = DecryptName<0x93E7A3A1F587CDC5ULL>(CRYPT_BYTES(15, "PartyInfo_Load", 0x93E7A3A1F587CDC5ULL));

// OFFSET: 0x005D0470
// FUNCTION: fn_03692
CLoadPartyInfoAsyncWork::CLoadPartyInfoAsyncWork()
    : NAsyncWork(g_loadPartyInfoPoolName), m_accountId(0), m_characterId(0), m_diagnostic(false, std::string(DecryptName<0x2FF3C5036D070127ULL>({0x52, 0x6F, 0x6C, 0x03, 0x6C, 0xB2, 0x9D, 0x2F})), sourceFile, 25) {
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
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x17359BF953C367B3ULL>({0xC6, 0x09, 0xA8, 0x3D, 0x8E, 0xF4, 0x5B, 0x17})), sourceFile, 52);
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
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>({0x14, 0xB4, 0xF1, 0xDB}), account, 4);
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>({0x58, 0x78, 0x89, 0xE7}), character, 4);
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>({0xE0, 0x48, 0xEB, 0xBF, 0x37, 0xE0, 0x4C, 0x3C, 0xCF, 0x3D}), this->m_token, 4);
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>({0xE0, 0x68, 0x25, 0xA1, 0x4F, 0xA9, 0x4A, 0xEC, 0xD2, 0x07}), world, 4);
  handler.AddField(DecryptName<0xD3755DD95F47A909ULL>({0x58, 0xDC, 0x22, 0x2D, 0xA0, 0x1C, 0x3C, 0x97, 0x09}), query, 4);

  if (handler.Perform() == 0) {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x31D7C1EDA345E36DULL>({0x1E, 0x96, 0x26, 0xC0, 0x88, 0xB2, 0xA4, 0x31})), sourceFile, 97);
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
