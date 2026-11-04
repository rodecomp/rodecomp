#include "Contents/UserData/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

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

  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    /* 0000 */ std::uint8_t m_text[Length];
    bool m_encrypted;
  };

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// OFFSET: 0x005B9E50
// FUNCTION: fn_03440
CAccountLinkedUserDataLoadAsyncWork::CAccountLinkedUserDataLoadAsyncWork()
    : NAsyncWork("AccountLinkedUserData_Load"), m_accountId(0),
      m_diagnostic(Diagnostic(false, DecryptName<0x59278F63D79F3DA1ULL>({0xD4, 0x53, 0xF4, 0xB9, 0x14, 0xE0, 0x49, 0x59}), 26)) {
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

  const std::uint8_t unknown[] = {0xC2, 0x69, 0x3C, 0xA3, 0x5C, 0x88, 0x45, 0x81};
  AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x812BE72BCD5707B7ULL>(unknown), 50);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005BA910
// FUNCTION: fn_03449
bool CAccountLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);

  std::string name = ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  const std::uint8_t aid[] = {0x56, 0xB0, 0x0B, 0xB9};
  handler.AddField(DecryptName<0xC52FB5FBB94FF917ULL>(aid), accountId, 4);
  const std::uint8_t worldName[] = {0x92, 0xA2, 0xF5, 0x99, 0xC5, 0xED, 0x86, 0xFE, 0xA0, 0xCD};
  handler.AddField(DecryptName<0x93E7A3A1F587CDC5ULL>(worldName), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = handler.Response();
    const std::uint8_t good[] = {0x40, 0x6E, 0x68, 0x09, 0x03};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x2FF3C5036D070127ULL>(good), 92);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005BA830
// FUNCTION: fn_03447
void CAccountLinkedUserDataLoadAsyncWork::Complete() {
  g_accountLinkedUserDataMgr->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_accountName, this->m_response
  );
}
