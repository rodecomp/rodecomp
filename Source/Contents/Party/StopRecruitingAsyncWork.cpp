#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/StopRecruitingAsyncWork.h"
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
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CStopRecruitingAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25606
const char* g_stopRecruitingPoolName = DecryptName<0xD97D5DBDBB2F2709ULL>(CRYPT_BYTES(16, "Stop_Recruiting", 0xD97D5DBDBB2F2709ULL));

// OFFSET: 0x005D5D70
// FUNCTION: fn_03711
CStopRecruitingAsyncWork::CStopRecruitingAsyncWork()
    : NAsyncWork(g_stopRecruitingPoolName), m_accountId(0), m_masterAccountId(0),
      m_diagnostic(false, std::string(DecryptName<0xA5FD29D7AB6DC59FULL>({0xEA, 0xAB, 0x06, 0xC5, 0xA0, 0x46, 0x93, 0xA5})), sourceFile, 28) {
}

// INLINED
CStopRecruitingAsyncWork::~CStopRecruitingAsyncWork() = default;

// OFFSET: 0x005D64A0
// FUNCTION: fn_03714
void CStopRecruitingAsyncWork::Reset() {
}

// OFFSET: 0x005D6230
// FUNCTION: fn_03713
void CStopRecruitingAsyncWork::Bind(std::uint32_t accountId, std::string world, std::string url, std::string token, std::uint32_t masterAccountId) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_masterAccountId = masterAccountId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0xF14F79170939C7F1ULL>({0x84, 0xA9, 0x52, 0x67, 0x60, 0x16, 0x21, 0xF1})), sourceFile, 50);
}

// OFFSET: 0x005D6570
// FUNCTION: fn_03716
bool CStopRecruitingAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char master[64] = {};
  StringFormat(master, sizeof(master), _TRUNCATE, "%lu", this->m_masterAccountId);
  const std::string world = CCURLMgr::GetInstance()->ToUtf8(this->m_world);
  CCURLPostHander handler(this->m_url);
  handler.AddField(DecryptName<0x050505679903579BULL>({0xDA, 0x1E, 0x47, 0x99}), account, 4);
  handler.AddField(DecryptName<0xDD3913D5D7F97BE1ULL>({0xB6, 0x14, 0x8B, 0xBB, 0xB1, 0x5D, 0x58, 0xB0, 0x84, 0x7B}), world, 4);
  handler.AddField(DecryptName<0xBF2B4B31397FF5A7ULL>({0xE6, 0x80, 0x0B, 0x51, 0x65, 0x24, 0x40, 0xDA, 0xC9, 0xF5}), this->m_token, 4);
  handler.AddField(DecryptName<0x83EFBFBB37EDBDBDULL>({0xF0, 0xDC, 0x9E, 0x43, 0xDE, 0xCD, 0xAE, 0xCA, 0xF9, 0xBD}), master, 4);

  if (handler.Perform() != 0) {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x39FF7FB50DEB998FULL>({0xE9, 0xF8, 0x82, 0x61, 0xB5})), sourceFile, 92);
  } else {
    this->m_response = CCURLMgr::GetInstance()->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x2FE993D1DDE1F197ULL>({0xE4, 0x84, 0x82, 0xBE, 0xB4, 0xE0, 0x9A, 0x2F})), sourceFile, 101);
  }

  return false;
}

// OFFSET: 0x005D64B0
// FUNCTION: fn_03715
void CStopRecruitingAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompleteStopRecruiting(this->m_diagnostic, this->m_response);
}

template void std::deque<CStopRecruitingAsyncWork*>::push_back(CStopRecruitingAsyncWork*&&);
