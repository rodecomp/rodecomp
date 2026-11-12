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

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CRegisterPartyAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25605
const char* g_registerPartyPoolName = DecryptName<0x07112F85A39F4FABULL>(CRYPT_BYTES(15, "Register_Party", 0x07112F85A39F4FABULL));

// OFFSET: 0x005D3600
// FUNCTION: fn_03704
CRegisterPartyAsyncWork::CRegisterPartyAsyncWork()
    : NAsyncWork(g_registerPartyPoolName), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(DecryptName<0x2D33791F67AD47DFULL>({0xAA, 0x29, 0xC6, 0x09, 0x68, 0x16, 0x5D, 0x2D})), sourceFile, 26) {
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
  constexpr std::array<std::uint8_t, 8> diagnosticSourceArray = CRYPT_NAME("unknwon", 0x936F3D2FFDFD87F5ULL);
  const std::uint8_t (&diagnosticSource)[8] = diagnosticSourceArray._Elems;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x936F3D2FFDFD87F5ULL>(diagnosticSource)), sourceFile, 50);
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
  handler.AddField(DecryptName<0x2F2701A7E56D3D89ULL>({0xC8, 0x74, 0x29, 0xE5}), account, 4);
  handler.AddField(DecryptName<0x6B3301E1A3079125ULL>({0x62, 0xD8, 0x43, 0xA3}), character, 4);
  handler.AddField(DecryptName<0x71B7691737C5A1AFULL>({0xEE, 0xD4, 0xB1, 0x5F, 0x43, 0x06, 0xDC, 0x14, 0xC1, 0xA1}), this->m_token, 4);
  handler.AddField(DecryptName<0xC39FB559A17D3963ULL>({0x34, 0x56, 0x0F, 0xCD, 0x3D, 0xFB, 0xFE, 0xAE, 0x06, 0x39}), world, 4);
  handler.AddField(DecryptName<0x2B8725D7E95325F5ULL>({0xB6, 0x4D, 0x32, 0x9B, 0x99, 0x44, 0xEA, 0x4E, 0xF5}), name, 4);
  handler.AddField(DecryptName<0x899D11BB83695B0DULL>({0x40, 0x32, 0x07, 0xCF, 0xED, 0x11}), minimum, 4);
  handler.AddField(DecryptName<0xBDADDD87C5A50B6BULL>({0x26, 0x6A, 0xDD, 0x89, 0xD1, 0xDD}), maximum, 4);
  handler.AddField(
    DecryptName<0x7989830DE73B9DBDULL>({0xE9, 0xFC, 0x55, 0x8C, 0x68, 0xF1, 0x89}),
    this->m_info.tanker ? std::string(DecryptName<0x5FFD7FABB5D1EF57ULL>({0x66, 0xEF})) : std::string(DecryptName<0xA7471985F93F9BDFULL>({0xEF, 0x9B})), 4
  );
  handler.AddField(
    DecryptName<0xF535E561137F138DULL>({0xC5, 0x76, 0x1E, 0x7F, 0x04, 0x97, 0x35}),
    this->m_info.healer ? std::string(DecryptName<0xD3E5516F89F161D1ULL>({0xE0, 0x61})) : std::string(DecryptName<0x01E7898D9D235DDFULL>({0xEF, 0x5D})), 4
  );
  handler.AddField(
    DecryptName<0x0DF5B33D9F21EB79ULL>({0x3D, 0x8E, 0x40, 0xF3, 0x58, 0xC1, 0xF5}),
    this->m_info.dealer ? std::string(DecryptName<0x9DEFC105A1EB2F71ULL>({0x40, 0x2F})) : std::string(DecryptName<0x6961FF056999A303ULL>({0x33, 0xA3})), 4
  );
  handler.AddField(
    DecryptName<0xBD3F430159B551FDULL>({0xBC, 0x22, 0xC6, 0x30, 0x72, 0x37, 0x3F}),
    this->m_info.assist ? std::string(DecryptName<0xC7FB6DF18577D34BULL>({0x7A, 0xD3})) : std::string(DecryptName<0x5B832361BDA77197ULL>({0xA7, 0x71})), 4
  );
  handler.AddField(DecryptName<0x2B477F9B5B39B301ULL>({0x55, 0xCA, 0x49, 0x3E, 0x9B}), type, 4);
  handler.AddField(DecryptName<0xFFBF43D1FFD5FF49ULL>({0x04, 0x9A, 0xB8, 0x90, 0xD1}), memo, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0xEB3D4FAB0FC183DBULL>({0xBD, 0xE2, 0xA8, 0x63, 0xAB})), sourceFile, 118);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x8F3F2FA959977F9FULL>({0xEC, 0x0A, 0xF4, 0x3A, 0xCC, 0x5C, 0x4C, 0x8F})), sourceFile, 127);
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
