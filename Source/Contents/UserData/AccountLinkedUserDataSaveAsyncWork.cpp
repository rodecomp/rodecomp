#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/AccountLinkedUserDataSaveAsyncWork.h"
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
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataSaveAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25601
const char* g_accountLinkedUserDataSavePoolName = DecryptName<0x1D6D75EDE7CD311FULL>(CRYPT_BYTES(27, "AccountLinkedUserData_Save", 0x1D6D75EDE7CD311FULL));

// OFFSET: 0x005BDAA0
// FUNCTION: fn_03478
CAccountLinkedUserDataSaveAsyncWork::CAccountLinkedUserDataSaveAsyncWork()
    : NAsyncWork(g_accountLinkedUserDataSavePoolName), m_accountId(0),
      m_diagnostic(false, std::string(DecryptName<0x59278F63D79F3DA1ULL>({0xD4, 0x53, 0xF4, 0xB9, 0x14, 0xE0, 0x49, 0x59})), sourceFile, 26) {
}

// INLINED
CAccountLinkedUserDataSaveAsyncWork::~CAccountLinkedUserDataSaveAsyncWork() = default;

// OFFSET: 0x005BE1F0
// FUNCTION: fn_03481
void CAccountLinkedUserDataSaveAsyncWork::Reset() {
}

// OFFSET: 0x005BDF50
// FUNCTION: fn_03480
void CAccountLinkedUserDataSaveAsyncWork::Bind(std::uint32_t accountId, std::string accountName, std::string url, std::string token, std::string data) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_accountId = accountId;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  constexpr std::array<std::uint8_t, 8> diagnosticSourceArray = CRYPT_NAME("unknwon", 0x812BE72BCD5707B7ULL);
  const std::uint8_t (&diagnosticSource)[8] = diagnosticSourceArray._Elems;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x812BE72BCD5707B7ULL>(diagnosticSource)), sourceFile, 54);
}

// OFFSET: 0x005BE210
// FUNCTION: fn_03483
bool CAccountLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  constexpr std::array<std::uint8_t, 4> aidArray = CRYPT_NAME("AID", 0xC52FB5FBB94FF917ULL);
  const std::uint8_t (&aid)[4] = aidArray._Elems;
  handler.AddField(DecryptName<0xC52FB5FBB94FF917ULL>(aid), accountId, 4);
  constexpr std::array<std::uint8_t, 10> tokenArray = CRYPT_NAME("AuthToken", 0x93E7A3A1F587CDC5ULL);
  const std::uint8_t (&token)[10] = tokenArray._Elems;
  handler.AddField(DecryptName<0x93E7A3A1F587CDC5ULL>(token), this->m_token, 4);
  const std::uint8_t worldName[] = CRYPT_BYTES(10, "WorldName", 0x2FF3C5036D070127ULL);
  handler.AddField(DecryptName<0x2FF3C5036D070127ULL>(worldName), name, 4);
  const std::uint8_t data[] = CRYPT_BYTES(5, "data", 0x17359BF953C367B3ULL);
  handler.AddField(DecryptName<0x17359BF953C367B3ULL>(data), this->m_data, 4);

  if (handler.Perform() != 0) {
    constexpr std::array<std::uint8_t, 5> errorArray = CRYPT_NAME("fail", 0x91651D81F559F971ULL);
    const std::uint8_t (&error)[5] = errorArray._Elems;
    this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x91651D81F559F971ULL>(error)), sourceFile, 102);
  } else {
    const std::uint8_t good[] = CRYPT_BYTES(5, "good", 0x73D1674B61DF7B0BULL);
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x73D1674B61DF7B0BULL>(good)), sourceFile, 106);
  }

  return false;
}

// OFFSET: 0x005BE200
// FUNCTION: fn_03482
void CAccountLinkedUserDataSaveAsyncWork::Complete() {
  g_accountLinkedUserDataMgr->CompleteSave();
}

template void std::deque<CAccountLinkedUserDataSaveAsyncWork*>::push_back(CAccountLinkedUserDataSaveAsyncWork*&&);
