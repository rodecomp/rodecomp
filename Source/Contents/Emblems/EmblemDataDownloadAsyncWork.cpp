#include "Contents/Emblems/EmblemDataDownloadAsyncWork.h"
#include "Contents/Emblems/EmblemDownload.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <stdio.h>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataDownloadAsyncWork.cpp";

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

// OFFSET: 0x005E9C00
// FUNCTION: fn_03963
CEmblemDataDownloadAsyncWork::CEmblemDataDownloadAsyncWork()
    : NAsyncWork("Emblem_Download"), m_accountId(0), m_guildId(0), m_version(0), m_responseCode(500),
      m_diagnostic(Diagnostic(false, DecryptName<0xC52FB5FBB94FF917ULL>({0x62, 0x97, 0x24, 0xD7, 0x8C, 0xDA, 0x41, 0xC5}), 29)), m_connection(nullptr) {
}

// INLINED
CEmblemDataDownloadAsyncWork::~CEmblemDataDownloadAsyncWork() = default;

// OFFSET: 0x005EA350
// FUNCTION: fn_03966
void CEmblemDataDownloadAsyncWork::Reset() {
}

// OFFSET: 0x005EA0D0
// FUNCTION: fn_03965
void CEmblemDataDownloadAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::uint32_t version, CConnection& connection
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_url = url;
  this->m_accountId = accountId;
  this->m_token = token;
  this->m_accountName = accountName;
  this->m_guildId = guildId;
  this->m_version = version;
  const std::uint8_t unknown[] = {0xB0, 0xA3, 0xEC, 0x9B, 0xD6, 0xCC, 0x89, 0x93};
  AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x93E7A3A1F587CDC5ULL>(unknown), 57);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_connection = &connection;
}

// OFFSET: 0x005EA430
// FUNCTION: fn_03968
bool CEmblemDataDownloadAsyncWork::Execute() {
  char account[64] = {}, guild[64] = {}, version[10] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  StringFormat(guild, sizeof(guild), _TRUNCATE, "%lu", this->m_guildId);
  StringFormat(version, sizeof(version), _TRUNCATE, "%lu", this->m_version);
  std::string name = ToUtf8(this->m_accountName);

  CCURLPostHander handler(this->m_url);
  const std::uint8_t aid[] = {0xE0, 0x74, 0xDB, 0xD7};
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>(aid), account, 4);
  const std::uint8_t authToken[] = {0xF6, 0x72, 0x23, 0xA5, 0x7F, 0x88, 0x40, 0xE4, 0xD9, 0x07};
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>(authToken), this->m_token, 4);
  const std::uint8_t worldName[] = {0x02, 0x92, 0xC7, 0xB7, 0xA3, 0xFF, 0x14, 0x5E, 0x30, 0xFD};
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>(worldName), name, 4);
  const std::uint8_t gdid[] = {0x58, 0x75, 0x84, 0xA3, 0xED};
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>(gdid), guild, 4);
  const std::uint8_t versionName[] = {0x3B, 0x06, 0x47, 0x5E, 0xCC, 0x6C, 0xE1, 0xD9};
  handler.AddField(DecryptName<0xD98F03A52D35636DULL>(versionName), version, 4);

  if (handler.Perform() == 0) {
    this->m_responseCode = static_cast<int>(handler.ResponseCode());
    this->m_response = handler.Response();
    const std::uint8_t good[] = {0xB6, 0x76, 0xF2, 0xEB, 0x6B};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x0F0F8B6B8F9D19D1ULL>(good), 96);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    const std::uint8_t fail[] = {0x6F, 0xC8, 0x2E, 0x33, 0xD9};
    AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0xD3755DD95F47A909ULL>(fail), 105);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005EA360
// FUNCTION: fn_03967
void CEmblemDataDownloadAsyncWork::Complete() {
  CompleteEmblemDownload(this->m_diagnostic, this->m_responseCode, this->m_response, this->m_guildId, this->m_version, *this->m_connection);
}
