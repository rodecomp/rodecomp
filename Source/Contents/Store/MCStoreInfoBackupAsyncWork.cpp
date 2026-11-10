#include "Contents/Store/MCStoreInfoBackup.h"
#include "Contents/Store/MCStoreInfoBackupAsyncWork.h"
#include "Client/Session.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <array>
#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char loadSource[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\MCStoreBackUpData\\CMCStoreInfoBackupLoadAsyncWork."
                                "cpp";
  constexpr char saveSource[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\MCStoreBackUpData\\CMCStoreInfoBackupSaveAsyncWork."
                                "cpp";

  // INLINED
  __forceinline AsyncDiagnostic Diagnostic(bool success, const char* text, const char* file, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, file, &line);
  }

  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    // INLINED
    explicit EncryptedName(const std::array<std::uint8_t, Length>& source) : m_encrypted(true) {
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

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::array<std::uint8_t, Length>& source) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// OFFSET: 0x00607B10
// FUNCTION: fn_04241
CMCStoreInfoBackupLoadAsyncWork::CMCStoreInfoBackupLoadAsyncWork()
    : NAsyncWork("MerchantStoreInformation_Load"), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(DecryptName<0xF535E561137F138DULL>({0xF8, 0x7D, 0x14, 0x7D, 0x16, 0x8A, 0x5B, 0xF5})), loadSource, 28), m_type(0) {
}

// INLINED
CMCStoreInfoBackupLoadAsyncWork::~CMCStoreInfoBackupLoadAsyncWork() = default;

// OFFSET: 0x00608270
// FUNCTION: fn_04244
void CMCStoreInfoBackupLoadAsyncWork::Reset() {
}

// OFFSET: 0x00607FE0
// FUNCTION: fn_04243
void CMCStoreInfoBackupLoadAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string token, int type
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_response = "";
  const std::uint8_t diagnosticSource[] = {0xA4, 0x0F, 0x9A, 0xE7, 0x18, 0x3E, 0x8B, 0xD3};
  AsyncDiagnostic diagnostic(false, std::string(DecryptName<0xD3E5516F89F161D1ULL>(diagnosticSource)), loadSource, 55);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_type = type;
}

// OFFSET: 0x00608350
// FUNCTION: fn_04246
bool CMCStoreInfoBackupLoadAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  std::string name = ToUtf8(this->m_accountName);
  char type[64] = {};
  StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_type);

  CCURLPostHander handler(this->m_url);
  const std::array<std::uint8_t, 4> aid = {0xB4, 0x6C, 0x17, 0xE9};
  handler.AddField(DecryptName<0x2B8725D7E95325F5ULL>(aid), account, 4);
  const std::array<std::uint8_t, 4> gid = {0x4A, 0x12, 0x2D, 0x83};
  handler.AddField(DecryptName<0x899D11BB83695B0DULL>(gid), character, 4);
  const std::array<std::uint8_t, 10> authToken = {0x2A, 0x7E, 0xD1, 0xAD, 0xD3, 0xB2, 0xC6, 0xD8, 0x05, 0x0B};
  handler.AddField(DecryptName<0xBDADDD87C5A50B6BULL>(authToken), this->m_token, 4);
  const std::array<std::uint8_t, 5> typeName = {0xE9, 0xE4, 0x4B, 0x82, 0x0D};
  handler.AddField(DecryptName<0x7989830DE73B9DBDULL>(typeName), type, 4);
  const std::array<std::uint8_t, 10> worldName = {0xDE, 0x52, 0x1F, 0x89, 0xC3, 0x4F, 0x46, 0x42, 0xEC, 0x3D};
  handler.AddField(DecryptName<0x2F2701A7E56D3D89ULL>(worldName), name, 4);

  if (handler.Perform() == 0) {
    this->m_response = handler.Response();
    const std::array<std::uint8_t, 5> good = {0x42, 0xFE, 0x68, 0xC7, 0xE1};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x6B3301E1A3079125ULL>(good), loadSource, 115);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x00608280
// FUNCTION: fn_04245
void CMCStoreInfoBackupLoadAsyncWork::Complete() {
  g_session.m_storeInfoBackup.CompleteLoad(this->m_diagnostic, this->m_accountId, this->m_characterId, this->m_accountName, this->m_response);
}

// OFFSET: 0x00608D60
// FUNCTION: fn_04247
CMCStoreInfoBackupSaveAsyncWork::CMCStoreInfoBackupSaveAsyncWork()
    : NAsyncWork("MerchantStoreInformation_Save"), m_accountId(0), m_characterId(0),
      m_diagnostic(false, std::string(DecryptName<0x3FAF85D995034FD7ULL>({0xA2, 0x21, 0x68, 0xFB, 0xAE, 0xEA, 0xC1, 0x3F})), saveSource, 30), m_type(0) {
}

// INLINED
CMCStoreInfoBackupSaveAsyncWork::~CMCStoreInfoBackupSaveAsyncWork() = default;

// OFFSET: 0x00609520
// FUNCTION: fn_04250
void CMCStoreInfoBackupSaveAsyncWork::Reset() {
}

// OFFSET: 0x00609270
// FUNCTION: fn_04249
void CMCStoreInfoBackupSaveAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string token, std::string data, int type
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  const std::uint8_t diagnosticSource[] = {0xCC, 0x63, 0x44, 0xCF, 0x34, 0x94, 0x61, 0x49};
  AsyncDiagnostic diagnostic(false, std::string(DecryptName<0x490FFB43A12F0DB9ULL>(diagnosticSource)), saveSource, 57);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_type = type;
}

// OFFSET: 0x006095F0
// FUNCTION: fn_04252
bool CMCStoreInfoBackupSaveAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  char type[64] = {};
  StringFormat(type, sizeof(type), _TRUNCATE, "%d", this->m_type);
  std::string name = ToUtf8(this->m_accountName);

  CCURLPostHander handler(this->m_url);
  const std::array<std::uint8_t, 4> aid = {0x48, 0xE0, 0x03, 0x5F};
  handler.AddField(DecryptName<0xD3755DD95F47A909ULL>(aid), account, 4);
  const std::array<std::uint8_t, 4> gid = {0x2A, 0xAA, 0x01, 0xA3};
  handler.AddField(DecryptName<0x31D7C1EDA345E36DULL>(gid), character, 4);
  const std::array<std::uint8_t, 10> authToken = {0x2C, 0x16, 0x41, 0x45, 0xF1, 0x6C, 0xE4, 0xBC, 0x03, 0x63};
  handler.AddField(DecryptName<0xD98F03A52D35636DULL>(authToken), this->m_token, 4);
  const std::array<std::uint8_t, 10> worldName = {0x86, 0x76, 0xEF, 0xE3, 0x0F, 0xC5, 0x6E, 0x62, 0xB4, 0x19};
  handler.AddField(DecryptName<0x0F0F8B6B8F9D19D1ULL>(worldName), name, 4);
  const std::array<std::uint8_t, 5> typeName = {0x01, 0x84, 0xC5, 0xBE, 0xC7};
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>(typeName), type, 4);
  const std::array<std::uint8_t, 5> dataName = {0x7B, 0x50, 0xB9, 0x86, 0xED};
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>(dataName), this->m_data, 4);

  if (handler.Perform() != 0) {
    const std::array<std::uint8_t, 5> fail = {0xC7, 0x5C, 0xF6, 0xBB, 0x63};
    AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x59278F63D79F3DA1ULL>(fail), saveSource, 112);
    this->m_diagnostic.MoveFrom(diagnostic);
    this->m_response = handler.Response();
  } else {
    const std::array<std::uint8_t, 5> good = {0xD0, 0x68, 0x38, 0xA9, 0x2B};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x812BE72BCD5707B7ULL>(good), saveSource, 122);
    this->m_diagnostic.MoveFrom(diagnostic);
    this->m_response = handler.Response();
  }

  return false;
}

// OFFSET: 0x00609530
// FUNCTION: fn_04251
void CMCStoreInfoBackupSaveAsyncWork::Complete() {
  g_session.m_storeInfoBackup.CompleteSave(this->m_diagnostic, this->m_response);
}

template void std::deque<CMCStoreInfoBackupSaveAsyncWork*>::push_back(CMCStoreInfoBackupSaveAsyncWork*&&);
template void std::deque<CMCStoreInfoBackupLoadAsyncWork*>::push_back(CMCStoreInfoBackupLoadAsyncWork*&&);
