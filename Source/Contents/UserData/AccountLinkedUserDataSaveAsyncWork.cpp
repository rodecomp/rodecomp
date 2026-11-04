#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/AccountLinkedUserDataSaveAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataSaveAsyncWork.cpp";

  // INLINED
  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
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
}

// OFFSET: 0x005BDAA0
// FUNCTION: fn_03478
CAccountLinkedUserDataSaveAsyncWork::CAccountLinkedUserDataSaveAsyncWork()
    : NAsyncWork("AccountLinkedUserData_Save"), m_accountId(0), m_diagnostic(Diagnostic(false, "unknwon", 26)) {
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
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 54);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005BE210
// FUNCTION: fn_03483
bool CAccountLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  _snprintf_s(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  std::string name = ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  const std::uint8_t aidSource[] = {0x56, 0xB0, 0x0B, 0xB9};
  thread_local EncryptedName<sizeof(aidSource), 0xC52FB5FBB94FF917ULL> aidName(aidSource);
  handler.AddField(aidName.Text(), accountId, 4);
  const std::uint8_t tokenSource[] = {0x84, 0xB8, 0xF3, 0x9D, 0xF5, 0xCC, 0x8C, 0xF6, 0xAB, 0xCD};
  thread_local EncryptedName<sizeof(tokenSource), 0x93E7A3A1F587CDC5ULL> tokenName(tokenSource);
  handler.AddField(tokenName.Text(), this->m_token, 4);
  const std::uint8_t worldSource[] = {0x70, 0x6E, 0x75, 0x01, 0x67, 0x8B, 0x92, 0x42, 0x42, 0x01};
  thread_local EncryptedName<sizeof(worldSource), 0x2FF3C5036D070127ULL> worldName(worldSource);
  handler.AddField(worldName.Text(), name, 4);
  const std::uint8_t dataSource[] = {0xD7, 0x06, 0xB7, 0x32, 0xF9};
  thread_local EncryptedName<sizeof(dataSource), 0x17359BF953C367B3ULL> dataName(dataSource);
  handler.AddField(dataName.Text(), this->m_data, 4);

  if (handler.Perform() != 0) {
    const std::uint8_t source[] = {0x17, 0x98, 0x30, 0x99, 0x81};
    thread_local EncryptedName<sizeof(source), 0x91651D81F559F971ULL> text(source);
    AsyncDiagnostic diagnostic = Diagnostic(false, text.Text(), 102);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    const std::uint8_t source[] = {0x6C, 0x14, 0xB0, 0x05, 0x4B};
    thread_local EncryptedName<sizeof(source), 0x73D1674B61DF7B0BULL> text(source);
    AsyncDiagnostic diagnostic = Diagnostic(true, text.Text(), 106);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005BE200
// FUNCTION: fn_03482
void CAccountLinkedUserDataSaveAsyncWork::Complete() {
  g_accountLinkedUserDataMgr->CompleteSave();
}
