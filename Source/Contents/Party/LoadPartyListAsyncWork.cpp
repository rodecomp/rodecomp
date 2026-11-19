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
}

// GLOBAL: gl_25604
const char* g_loadPartyListPoolName = OBFUSCATE_BYTES_TEXT("PartyList_Load", 0x07112F85A39F4FABULL);

// OFFSET: 0x005D1640
// FUNCTION: fn_03698
CLoadPartyListAsyncWork::CLoadPartyListAsyncWork()
    : NAsyncWork(g_loadPartyListPoolName), m_accountId(0), m_characterId(0), m_page(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknown", 0x2D33791F67AD47DFULL)), sourceFile, 25) {
}

// OFFSET: 0x005D19B0
// FUNCTION: fn_46411
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
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x2F2701A7E56D3D89ULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x6B3301E1A3079125ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x71B7691737C5A1AFULL), world, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0xC39FB559A17D3963ULL), this->m_token, 4);

  switch (this->m_search) {
  case 0: {
    handler.AddField(OBFUSCATE_BYTES_TEXT("page", 0x2B8725D7E95325F5ULL), page, 4);
    break;
  }
  case 1: {
    char minimum[10] = {};
    StringFormat(minimum, sizeof(minimum), _TRUNCATE, "%d", this->m_filter.minimumLevel);
    handler.AddField(OBFUSCATE_BYTES_TEXT("MinLV", 0x899D11BB83695B0DULL), minimum, 4);
    char maximum[10] = {};
    StringFormat(maximum, sizeof(maximum), _TRUNCATE, "%d", this->m_filter.maximumLevel);
    handler.AddField(OBFUSCATE_BYTES_TEXT("MaxLV", 0xBDADDD87C5A50B6BULL), maximum, 4);

    if (this->m_filter.tanker) {
      handler.AddField(OBFUSCATE_BYTES_TEXT("Tanker", 0x7989830DE73B9DBDULL), OBFUSCATE_BYTES_TEXT("1", 0x5FFD7FABB5D1EF57ULL), 4);
    }

    if (this->m_filter.dealer) {
      handler.AddField(OBFUSCATE_BYTES_TEXT("Dealer", 0xA7471985F93F9BDFULL), OBFUSCATE_BYTES_TEXT("1", 0xF535E561137F138DULL), 4);
    }

    if (this->m_filter.healer) {
      handler.AddField(OBFUSCATE_BYTES_TEXT("Healer", 0xD3E5516F89F161D1ULL), OBFUSCATE_BYTES_TEXT("1", 0x01E7898D9D235DDFULL), 4);
    }

    if (this->m_filter.assist) {
      handler.AddField(OBFUSCATE_BYTES_TEXT("Assist", 0x0DF5B33D9F21EB79ULL), OBFUSCATE_BYTES_TEXT("1", 0x9DEFC105A1EB2F71ULL), 4);
    }

    if (this->m_filter.type != 0) {
      char type[10] = {};
      StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_filter.type);
      handler.AddField(OBFUSCATE_BYTES_TEXT("Type", 0x6961FF056999A303ULL), type, 4);
    }

    if (!this->m_filter.memo.empty()) {
      std::string memo = CCURLMgr::GetInstance()->ToUtf8(this->m_filter.memo);
      handler.AddField(OBFUSCATE_BYTES_TEXT("Memo", 0xBD3F430159B551FDULL), memo, 4);
    }

    handler.AddField(OBFUSCATE_BYTES_TEXT("page", 0xC7FB6DF18577D34BULL), page, 4);
    break;
  }
  }

  if (handler.Perform() == 0) {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("success", 0x5B832361BDA77197ULL)), sourceFile, 138);
  }

  return false;
}

// OFFSET: 0x005D1F40
// FUNCTION: fn_03702
void CLoadPartyListAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompleteList(this->m_diagnostic, this->m_response);
}
