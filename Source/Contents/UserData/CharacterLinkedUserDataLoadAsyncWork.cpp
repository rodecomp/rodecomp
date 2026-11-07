#include "Contents/UserData/CharacterLinkedUserDataLoadAsyncWork.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\CharacterLinkedUserData\\CCh"
                                "aracterLinkedUserDataLoadAsyncWork.cpp";

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

// OFFSET: 0x005E4A50
// FUNCTION: fn_03916
CCharacterLinkedUserDataLoadAsyncWork::CCharacterLinkedUserDataLoadAsyncWork()
    : NAsyncWork("CharacterLinkedUserData_Load"), m_accountId(0), m_characterId(0),
      m_diagnostic(Diagnostic(false, DecryptName<0x17359BF953C367B3ULL>({0xC6, 0x09, 0xA8, 0x3D, 0x8E, 0xF4, 0x5B, 0x17}), 29)) {
}

// INLINED
CCharacterLinkedUserDataLoadAsyncWork::~CCharacterLinkedUserDataLoadAsyncWork() = default;

// OFFSET: 0x005E5190
// FUNCTION: fn_03919
void CCharacterLinkedUserDataLoadAsyncWork::Reset() {
}

// OFFSET: 0x005E4F10
// FUNCTION: fn_03918
void CCharacterLinkedUserDataLoadAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string url, std::string token
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_response = "";

  AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x2FF3C5036D070127ULL>({0x52, 0x6F, 0x6C, 0x03, 0x74, 0xAA, 0x9D, 0x2F}), 55);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005E5280
// FUNCTION: fn_03921
bool CCharacterLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  char characterId[64] = {};
  StringFormat(characterId, sizeof(characterId), _TRUNCATE, "%lu", this->m_characterId);

  std::string name = ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>({0x5E, 0x78, 0x89, 0xE7}), accountId, 4);
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>({0x12, 0xB4, 0xF1, 0xDB}), characterId, 4);
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>({0xF6, 0x72, 0x23, 0xA5, 0x7F, 0x88, 0x40, 0xE4, 0xD9, 0x07}), this->m_token, 4);
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>({0xF6, 0x52, 0xED, 0xBB, 0x07, 0xC1, 0x46, 0x34, 0xC4, 0x3D}), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x31D7C1EDA345E36DULL>({0x0A, 0x8C, 0x2A, 0xC7, 0xED}), 103);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005E51A0
// FUNCTION: fn_03920
void CCharacterLinkedUserDataLoadAsyncWork::Complete() {
  g_characterLinkedUserDataMgr->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_characterId, this->m_accountName, this->m_response
  );
}
