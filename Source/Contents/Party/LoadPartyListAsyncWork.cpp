#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyListAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyLIstAsyncWork.cpp";

  // INLINED
  __forceinline AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// OFFSET: 0x005D1640
// FUNCTION: fn_03698
CLoadPartyListAsyncWork::CLoadPartyListAsyncWork()
    : NAsyncWork("PartyList_Load"), m_accountId(0), m_characterId(0), m_page(0),
      m_diagnostic(false, std::string(DecryptName<0x2D33791F67AD47DFULL>({0xAA, 0x29, 0xC6, 0x09, 0x70, 0x0E, 0x5D, 0x2D})), sourceFile, 25), m_windows(nullptr) {
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
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, CAdvenPartyInfo filter, int search, int page,
  WindowManagerWindows& windows
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
  constexpr std::array<std::uint8_t, 8> diagnosticSourceArray = CRYPT_NAME("unknwon", 0x936F3D2FFDFD87F5ULL);
  const std::uint8_t (&diagnosticSource)[8] = diagnosticSourceArray._Elems;
  AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x936F3D2FFDFD87F5ULL>(diagnosticSource), 54);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_windows = &windows;
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
  constexpr std::array<std::uint8_t, 4> gidArray = CRYPT_NAME("GID", 0x2F2701A7E56D3D89ULL);
  const std::uint8_t (&gid)[4] = gidArray._Elems;
  handler.AddField(DecryptName<0x2F2701A7E56D3D89ULL>(gid), character, 4);
  constexpr std::array<std::uint8_t, 4> aidArray = CRYPT_NAME("AID", 0x6B3301E1A3079125ULL);
  const std::uint8_t (&aid)[4] = aidArray._Elems;
  handler.AddField(DecryptName<0x6B3301E1A3079125ULL>(aid), account, 4);
  constexpr std::array<std::uint8_t, 10> worldNameArray = CRYPT_NAME("WorldName", 0x71B7691737C5A1AFULL);
  const std::uint8_t (&worldName)[10] = worldNameArray._Elems;
  handler.AddField(DecryptName<0x71B7691737C5A1AFULL>(worldName), world, 4);
  const std::uint8_t authToken[] = {0x22, 0x4C, 0x09, 0xC9, 0x0D, 0xDA, 0xF4, 0xA6, 0x0D, 0x39};
  handler.AddField(DecryptName<0xC39FB559A17D3963ULL>(authToken), this->m_token, 4);

  switch (this->m_search) {
  case 0: {
    const std::uint8_t pageName[] = {0x85, 0x44, 0x34, 0x8C, 0xD7};
    handler.AddField(DecryptName<0x2B8725D7E95325F5ULL>(pageName), page, 4);
    break;
  }
  case 1: {
    char minimum[10] = {};
    StringFormat(minimum, sizeof(minimum), _TRUNCATE, "%d", this->m_filter.minimumLevel);
    const std::uint8_t minLevel[] = {0x40, 0x32, 0x07, 0xCF, 0xED, 0x11};
    handler.AddField(DecryptName<0x899D11BB83695B0DULL>(minLevel), minimum, 4);
    char maximum[10] = {};
    StringFormat(maximum, sizeof(maximum), _TRUNCATE, "%d", this->m_filter.maximumLevel);
    const std::uint8_t maxLevel[] = {0x26, 0x6A, 0xDD, 0x89, 0xD1, 0xDD};
    handler.AddField(DecryptName<0xBDADDD87C5A50B6BULL>(maxLevel), maximum, 4);

    if (this->m_filter.tanker) {
      const std::uint8_t tankerOn[] = {0x66, 0xEF};
      const std::uint8_t tanker[] = {0xE9, 0xFC, 0x55, 0x8C, 0x68, 0xF1, 0x89};
      handler.AddField(DecryptName<0x7989830DE73B9DBDULL>(tanker), DecryptName<0x5FFD7FABB5D1EF57ULL>(tankerOn), 4);
    }

    if (this->m_filter.dealer) {
      const std::uint8_t dealerOn[] = {0xBC, 0x13};
      const std::uint8_t dealer[] = {0x9B, 0xFE, 0x5E, 0x95, 0xE0, 0x6B, 0x47};
      handler.AddField(DecryptName<0xA7471985F93F9BDFULL>(dealer), DecryptName<0xF535E561137F138DULL>(dealerOn), 4);
    }

    if (this->m_filter.healer) {
      const std::uint8_t healerOn[] = {0xEE, 0x5D};
      const std::uint8_t healer[] = {0x99, 0x04, 0x90, 0xE5, 0x0A, 0x23, 0xE5};
      handler.AddField(DecryptName<0xD3E5516F89F161D1ULL>(healer), DecryptName<0x01E7898D9D235DDFULL>(healerOn), 4);
    }

    if (this->m_filter.assist) {
      const std::uint8_t assistOn[] = {0x40, 0x2F};
      const std::uint8_t assist[] = {0x38, 0x98, 0x52, 0xF6, 0x4E, 0xC7, 0xF5};
      handler.AddField(DecryptName<0x0DF5B33D9F21EB79ULL>(assist), DecryptName<0x9DEFC105A1EB2F71ULL>(assistOn), 4);
    }

    if (this->m_filter.type != 0) {
      char type[10] = {};
      StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_filter.type);
      const std::uint8_t typeName[] = {0x57, 0xDA, 0xE9, 0x0C, 0x05};
      handler.AddField(DecryptName<0x6961FF056999A303ULL>(typeName), type, 4);
    }

    if (!this->m_filter.memo.empty()) {
      std::string memo = CCURLMgr::GetInstance()->ToUtf8(this->m_filter.memo);
      const std::uint8_t memoName[] = {0xB0, 0x34, 0xD8, 0x36, 0x01};
      handler.AddField(DecryptName<0xBD3F430159B551FDULL>(memoName), memo, 4);
    }

    const std::uint8_t pageName[] = {0x3B, 0xB2, 0x10, 0xE0, 0xF1};
    handler.AddField(DecryptName<0xC7FB6DF18577D34BULL>(pageName), page, 4);
    break;
  }
  }

  if (handler.Perform() == 0) {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    const std::uint8_t success[] = {0xE4, 0x04, 0xC4, 0xDE, 0x04, 0x50, 0xF0, 0x5B};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x5B832361BDA77197ULL>(success), 138);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005D1F40
// FUNCTION: fn_03702
void CLoadPartyListAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompleteList(this->m_diagnostic, this->m_response, *this->m_windows);
}

template void std::deque<CLoadPartyListAsyncWork*>::push_back(CLoadPartyListAsyncWork*&&);
