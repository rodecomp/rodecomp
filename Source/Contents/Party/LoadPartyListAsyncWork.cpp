#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyListAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyLIstAsyncWork.cpp";

  // INLINED
  __forceinline AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// GLOBAL: gl_25604
const char* g_loadPartyListPoolName = OBFUSCATE_BYTES_TEXT("PartyList_Load", 0x07112F85A39F4FABULL);

// OFFSET: 0x005D1640
// FUNCTION: fn_03698
CLoadPartyListAsyncWork::CLoadPartyListAsyncWork()
    : NAsyncWork(g_loadPartyListPoolName), m_accountId(0), m_characterId(0), m_page(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknown", 0x2D33791F67AD47DFULL)), sourceFile, 25) {
}

// INLINED
CLoadPartyListAsyncWork::~CLoadPartyListAsyncWork() = default;

// OFFSET: 0x005D1F30
// FUNCTION: fn_03701
void CLoadPartyListAsyncWork::Reset() {
}

// OFFSET: 0x005D1BD0
// FUNCTION: fn_03700
void CLoadPartyListAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, CAdvenPartyInfo filter, int search, int page
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_filter = filter;
  this->m_page = page;
  this->m_search = search;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x936F3D2FFDFD87F5ULL)), sourceFile, 54);
}

// OFFSET: 0x005D2000
// FUNCTION: fn_03703
bool CLoadPartyListAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  char page[10] = {};
  StringFormat(page, sizeof(page), _TRUNCATE, "%d", this->m_page);
  const std::string world = CCURLMgr::GetInstance()->ToUtf8(this->m_world);
  CCURLPostHander handler(this->m_url);
  constexpr std::array<std::uint8_t, 4> gidArray = OBFUSCATE_SOURCE("GID", 0x2F2701A7E56D3D89ULL);
  const std::uint8_t (&gid)[4] = gidArray._Elems;
  handler.AddField(OBFUSCATE_DECRYPT(0x2F2701A7E56D3D89ULL, gid), character, 4);
  constexpr std::array<std::uint8_t, 4> aidArray = OBFUSCATE_SOURCE("AID", 0x6B3301E1A3079125ULL);
  const std::uint8_t (&aid)[4] = aidArray._Elems;
  handler.AddField(OBFUSCATE_DECRYPT(0x6B3301E1A3079125ULL, aid), account, 4);
  constexpr std::array<std::uint8_t, 10> worldNameArray = OBFUSCATE_SOURCE("WorldName", 0x71B7691737C5A1AFULL);
  const std::uint8_t (&worldName)[10] = worldNameArray._Elems;
  handler.AddField(OBFUSCATE_DECRYPT(0x71B7691737C5A1AFULL, worldName), world, 4);
  const std::uint8_t authToken[] = OBFUSCATE_SOURCE_BYTES(10, "AuthToken", 0xC39FB559A17D3963ULL);
  handler.AddField(OBFUSCATE_DECRYPT(0xC39FB559A17D3963ULL, authToken), this->m_token, 4);

  switch (this->m_search) {
  case 0: {
    constexpr std::array<std::uint8_t, 5> pageNameArray = OBFUSCATE_SOURCE("page", 0x2B8725D7E95325F5ULL);
    const std::uint8_t (&pageName)[5] = pageNameArray._Elems;
    handler.AddField(OBFUSCATE_DECRYPT(0x2B8725D7E95325F5ULL, pageName), page, 4);
    break;
  }
  case 1: {
    char minimum[10] = {};
    StringFormat(minimum, sizeof(minimum), _TRUNCATE, "%d", this->m_filter.minimumLevel);
    const std::uint8_t minLevel[] = OBFUSCATE_SOURCE_BYTES(6, "MinLV", 0x899D11BB83695B0DULL);
    handler.AddField(OBFUSCATE_DECRYPT(0x899D11BB83695B0DULL, minLevel), minimum, 4);
    char maximum[10] = {};
    StringFormat(maximum, sizeof(maximum), _TRUNCATE, "%d", this->m_filter.maximumLevel);
    const std::uint8_t maxLevel[] = OBFUSCATE_SOURCE_BYTES(6, "MaxLV", 0xBDADDD87C5A50B6BULL);
    handler.AddField(OBFUSCATE_DECRYPT(0xBDADDD87C5A50B6BULL, maxLevel), maximum, 4);

    if (this->m_filter.tanker) {
      const std::uint8_t tankerOn[] = OBFUSCATE_SOURCE_BYTES(2, "1", 0x5FFD7FABB5D1EF57ULL);
      const std::uint8_t tanker[] = OBFUSCATE_SOURCE_BYTES(7, "Tanker", 0x7989830DE73B9DBDULL);
      handler.AddField(OBFUSCATE_DECRYPT(0x7989830DE73B9DBDULL, tanker), OBFUSCATE_DECRYPT(0x5FFD7FABB5D1EF57ULL, tankerOn), 4);
    }

    if (this->m_filter.dealer) {
      const std::uint8_t dealerOn[] = OBFUSCATE_SOURCE_BYTES(2, "1", 0xF535E561137F138DULL);
      const std::uint8_t dealer[] = OBFUSCATE_SOURCE_BYTES(7, "Dealer", 0xA7471985F93F9BDFULL);
      handler.AddField(OBFUSCATE_DECRYPT(0xA7471985F93F9BDFULL, dealer), OBFUSCATE_DECRYPT(0xF535E561137F138DULL, dealerOn), 4);
    }

    if (this->m_filter.healer) {
      const std::uint8_t healerOn[] = OBFUSCATE_SOURCE_BYTES(2, "1", 0x01E7898D9D235DDFULL);
      const std::uint8_t healer[] = OBFUSCATE_SOURCE_BYTES(7, "Healer", 0xD3E5516F89F161D1ULL);
      handler.AddField(OBFUSCATE_DECRYPT(0xD3E5516F89F161D1ULL, healer), OBFUSCATE_DECRYPT(0x01E7898D9D235DDFULL, healerOn), 4);
    }

    if (this->m_filter.assist) {
      const std::uint8_t assistOn[] = OBFUSCATE_SOURCE_BYTES(2, "1", 0x9DEFC105A1EB2F71ULL);
      const std::uint8_t assist[] = OBFUSCATE_SOURCE_BYTES(7, "Assist", 0x0DF5B33D9F21EB79ULL);
      handler.AddField(OBFUSCATE_DECRYPT(0x0DF5B33D9F21EB79ULL, assist), OBFUSCATE_DECRYPT(0x9DEFC105A1EB2F71ULL, assistOn), 4);
    }

    if (this->m_filter.type != 0) {
      char type[10] = {};
      StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_filter.type);
      const std::uint8_t typeName[] = OBFUSCATE_SOURCE_BYTES(5, "Type", 0x6961FF056999A303ULL);
      handler.AddField(OBFUSCATE_DECRYPT(0x6961FF056999A303ULL, typeName), type, 4);
    }

    if (!this->m_filter.memo.empty()) {
      std::string memo = CCURLMgr::GetInstance()->ToUtf8(this->m_filter.memo);
      const std::uint8_t memoName[] = OBFUSCATE_SOURCE_BYTES(5, "Memo", 0xBD3F430159B551FDULL);
      handler.AddField(OBFUSCATE_DECRYPT(0xBD3F430159B551FDULL, memoName), memo, 4);
    }

    const std::uint8_t pageName[] = OBFUSCATE_SOURCE_BYTES(5, "page", 0xC7FB6DF18577D34BULL);
    handler.AddField(OBFUSCATE_DECRYPT(0xC7FB6DF18577D34BULL, pageName), page, 4);
    break;
  }
  }

  if (handler.Perform() == 0) {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    const std::uint8_t success[] = OBFUSCATE_SOURCE_BYTES(8, "success", 0x5B832361BDA77197ULL);
    AsyncDiagnostic diagnostic = Diagnostic(true, OBFUSCATE_DECRYPT(0x5B832361BDA77197ULL, success), 138);
    this->m_diagnostic = std::move(diagnostic);
  }

  return false;
}

// OFFSET: 0x005D1F40
// FUNCTION: fn_03702
void CLoadPartyListAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompleteList(this->m_diagnostic, this->m_response);
}

template void std::deque<CLoadPartyListAsyncWork*>::push_back(CLoadPartyListAsyncWork*&&);
