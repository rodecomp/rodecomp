#include "Contents/UserData/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <array>
#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataLoadAsyncWork.cpp";

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

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::array<std::uint8_t, Length>& source) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25600
const char* g_accountLinkedUserDataLoadPoolName = DecryptName<0x1D6D75EDE7CD311FULL>(CRYPT_BYTES(27, "AccountLinkedUserData_Load", 0x1D6D75EDE7CD311FULL));

// OFFSET: 0x005B9E50
// FUNCTION: fn_03440
CAccountLinkedUserDataLoadAsyncWork::CAccountLinkedUserDataLoadAsyncWork()
    : NAsyncWork(g_accountLinkedUserDataLoadPoolName), m_accountId(0),
      m_diagnostic(false, std::string(DecryptName<0x59278F63D79F3DA1ULL>({0xD4, 0x53, 0xF4, 0xB9, 0x14, 0xE0, 0x49, 0x59})), sourceFile, 26) {
}

// INLINED
CAccountLinkedUserDataLoadAsyncWork::~CAccountLinkedUserDataLoadAsyncWork() = default;

// OFFSET: 0x005BA820
// FUNCTION: fn_03446
void CAccountLinkedUserDataLoadAsyncWork::Reset() {
}

// OFFSET: 0x005BA5D0
// FUNCTION: fn_03445
void CAccountLinkedUserDataLoadAsyncWork::Bind(std::uint32_t accountId, std::string accountName, std::string url) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_response = "";

  constexpr std::array<std::uint8_t, 8> diagnosticSourceArray = CRYPT_NAME("unknwon", 0x812BE72BCD5707B7ULL);
  const std::uint8_t (&diagnosticSource)[8] = diagnosticSourceArray._Elems;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0x812BE72BCD5707B7ULL>(diagnosticSource)), sourceFile, 50);
}

// OFFSET: 0x005BA910
// FUNCTION: fn_03449
bool CAccountLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);

  std::string name = CCURLMgr::GetInstance()->ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  constexpr std::array<std::uint8_t, 4> aid = CRYPT_NAME("AID", 0xC52FB5FBB94FF917ULL);
  handler.AddField(DecryptName<0xC52FB5FBB94FF917ULL>(aid), accountId, 4);
  constexpr std::array<std::uint8_t, 10> worldName = CRYPT_NAME("WorldName", 0x93E7A3A1F587CDC5ULL);
  handler.AddField(DecryptName<0x93E7A3A1F587CDC5ULL>(worldName), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = handler.Response();
    constexpr std::array<std::uint8_t, 5> good = CRYPT_NAME("good", 0x2FF3C5036D070127ULL);
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x2FF3C5036D070127ULL>(good), 92);
    this->m_diagnostic = std::move(diagnostic);
  }

  return false;
}

// OFFSET: 0x005BA830
// FUNCTION: fn_03447
void CAccountLinkedUserDataLoadAsyncWork::Complete() {
  CAccountLinkedUserDataMgr* manager = g_accountLinkedUserDataMgr;
  manager->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_accountName, this->m_response
  );
}

template void std::deque<CAccountLinkedUserDataLoadAsyncWork*>::push_back(CAccountLinkedUserDataLoadAsyncWork*&&);
