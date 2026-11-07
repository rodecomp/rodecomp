#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataSaveAsyncWork.h"
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
                                "aracterLinkedUserDataSaveAsyncWork.cpp";

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

// OFFSET: 0x005E7C10
// FUNCTION: fn_03944
CCharacterLinkedUserDataSaveAsyncWork::CCharacterLinkedUserDataSaveAsyncWork()
    : NAsyncWork("CharacterLinkedUserData_Save"), m_accountId(0), m_characterId(0),
      m_diagnostic(Diagnostic(false, DecryptName<0xC52FB5FBB94FF917ULL>({0x62, 0x97, 0x24, 0xD7, 0x8C, 0xDA, 0x41, 0xC5}), 28)) {
}

// INLINED
CCharacterLinkedUserDataSaveAsyncWork::~CCharacterLinkedUserDataSaveAsyncWork() = default;

// OFFSET: 0x005E8370
// FUNCTION: fn_03947
void CCharacterLinkedUserDataSaveAsyncWork::Reset() {
}

// OFFSET: 0x005E80D0
// FUNCTION: fn_03946
void CCharacterLinkedUserDataSaveAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string url, std::string token, std::string data
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x93E7A3A1F587CDC5ULL>({0xB0, 0xA3, 0xEC, 0x9B, 0xD6, 0xCC, 0x89, 0x93}), 54);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005E8390
// FUNCTION: fn_03949
bool CCharacterLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  StringFormat(accountId, sizeof(accountId), _TRUNCATE, "%lu", this->m_accountId);
  char characterId[64] = {};
  StringFormat(characterId, sizeof(characterId), _TRUNCATE, "%lu", this->m_characterId);
  std::string name = ToUtf8(this->m_accountName);
  std::string data = ToUtf8(this->m_data);
  CCURLPostHander handler(this->m_url);
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>({0xE0, 0x74, 0xDB, 0xD7}), accountId, 4);
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>({0xF0, 0x4E, 0x13, 0xCD}), characterId, 4);
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>({0x14, 0x88, 0xC1, 0xB3, 0x93, 0xDE, 0x1E, 0x56, 0x3B, 0xFD}), this->m_token, 4);
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>({0x48, 0x5E, 0xBF, 0x8B, 0x89, 0x3B, 0x0C, 0x70, 0x7A, 0x31}), name, 4);
  handler.AddField(DecryptName<0xD98F03A52D35636DULL>({0x09, 0x02, 0x41, 0x4C, 0xA5}), data, 4);

  if (handler.Perform() != 0) {
    AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x0F0F8B6B8F9D19D1ULL>({0xB7, 0x78, 0xF4, 0xE3, 0x6B}), 98);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0xD3755DD95F47A909ULL>({0x6E, 0xC6, 0x28, 0x3B, 0xD9}), 102);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005E8380
// FUNCTION: fn_03948
void CCharacterLinkedUserDataSaveAsyncWork::Complete() {
  g_characterLinkedUserDataMgr->CompleteSave();
}
