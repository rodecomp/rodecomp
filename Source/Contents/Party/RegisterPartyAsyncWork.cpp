#include "Client/Session/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/RegisterPartyAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CRegisterPartyAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25605
const char* g_registerPartyPoolName = OBFUSCATE_BYTES_TEXT("Register_Party", 0x07112F85A39F4FABULL);

// OFFSET: 0x005D3600
// FUNCTION: fn_03704
CRegisterPartyAsyncWork::CRegisterPartyAsyncWork()
    : NAsyncWork(g_registerPartyPoolName), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x2D33791F67AD47DFULL)), sourceFile, 26) {
}

// INLINED
CRegisterPartyAsyncWork::~CRegisterPartyAsyncWork() = default;

// OFFSET: 0x005D3E80
// FUNCTION: fn_03707
void CRegisterPartyAsyncWork::Reset() {
}

// OFFSET: 0x005D3B80
// FUNCTION: fn_03706
void CRegisterPartyAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, const CAdvenPartyInfo& info
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_info = info;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x936F3D2FFDFD87F5ULL)), sourceFile, 50);
}

// OFFSET: 0x005D3F50
// FUNCTION: fn_03709
bool CRegisterPartyAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  char minimum[10] = {};
  StringFormat(minimum, sizeof(minimum), _TRUNCATE, "%d", this->m_info.minimumLevel);
  char maximum[10] = {};
  StringFormat(maximum, sizeof(maximum), _TRUNCATE, "%d", this->m_info.maximumLevel);
  char type[10] = {};
  StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_info.type);
  const std::string world = CCURLMgr::GetInstance()->ToUtf8(this->m_world);
  const std::string name = CCURLMgr::GetInstance()->ToUtf8(std::string(g_session.DecodePlayerName()));
  const std::string memo = CCURLMgr::GetInstance()->ToUtf8(this->m_info.memo);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x2F2701A7E56D3D89ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GID", 0x6B3301E1A3079125ULL), character, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x71B7691737C5A1AFULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0xC39FB559A17D3963ULL), world, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("CharName", 0x2B8725D7E95325F5ULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("MinLV", 0x899D11BB83695B0DULL), minimum, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("MaxLV", 0xBDADDD87C5A50B6BULL), maximum, 4);
  handler.AddField(
    OBFUSCATE_BYTES_TEXT("Tanker", 0x7989830DE73B9DBDULL),
    this->m_info.tanker ? std::string(OBFUSCATE_BYTES_TEXT("1", 0x5FFD7FABB5D1EF57ULL)) : std::string(OBFUSCATE_BYTES_TEXT("0", 0xA7471985F93F9BDFULL)), 4
  );
  handler.AddField(
    OBFUSCATE_BYTES_TEXT("Healer", 0xF535E561137F138DULL),
    this->m_info.healer ? std::string(OBFUSCATE_BYTES_TEXT("1", 0xD3E5516F89F161D1ULL)) : std::string(OBFUSCATE_BYTES_TEXT("0", 0x01E7898D9D235DDFULL)), 4
  );
  handler.AddField(
    OBFUSCATE_BYTES_TEXT("Dealer", 0x0DF5B33D9F21EB79ULL),
    this->m_info.dealer ? std::string(OBFUSCATE_BYTES_TEXT("1", 0x9DEFC105A1EB2F71ULL)) : std::string(OBFUSCATE_BYTES_TEXT("0", 0x6961FF056999A303ULL)), 4
  );
  handler.AddField(
    OBFUSCATE_BYTES_TEXT("Assist", 0xBD3F430159B551FDULL),
    this->m_info.assist ? std::string(OBFUSCATE_BYTES_TEXT("1", 0xC7FB6DF18577D34BULL)) : std::string(OBFUSCATE_BYTES_TEXT("0", 0x5B832361BDA77197ULL)), 4
  );
  handler.AddField(OBFUSCATE_BYTES_TEXT("Type", 0x2B477F9B5B39B301ULL), type, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("Memo", 0xFFBF43D1FFD5FF49ULL), memo, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0xEB3D4FAB0FC183DBULL)), sourceFile, 118);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("success", 0x8F3F2FA959977F9FULL)), sourceFile, 127);
  }

  return false;
}

// OFFSET: 0x005D3E90
// FUNCTION: fn_03708
void CRegisterPartyAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompleteRegistration(this->m_diagnostic, this->m_response);
}

template void std::deque<CRegisterPartyAsyncWork*>::push_back(CRegisterPartyAsyncWork*&&);
