#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Emblems/EmblemDataUploadAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cctype>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataUploadAsyncWork.cpp";

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

// OFFSET: 0x005EBF50
// FUNCTION: fn_03978
CEmblemDataUploadAsyncWork::CEmblemDataUploadAsyncWork()
    : NAsyncWork("Emblem_Upload"), m_accountId(0), m_guildId(0),
      m_diagnostic(false, std::string(DecryptName<0x93E7A3A1F587CDC5ULL>({0xB0, 0xA3, 0xEC, 0x9B, 0xD6, 0xCC, 0x89, 0x93})), sourceFile, 27) {
}

// INLINED
CEmblemDataUploadAsyncWork::~CEmblemDataUploadAsyncWork() = default;

// OFFSET: 0x005EC6F0
// FUNCTION: fn_03981
void CEmblemDataUploadAsyncWork::Reset() {
}

// OFFSET: 0x005EC450
// FUNCTION: fn_03980
void CEmblemDataUploadAsyncWork::Bind(std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::string path) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_url = url;
  this->m_accountId = accountId;
  this->m_token = token;
  this->m_accountName = accountName;
  this->m_guildId = guildId;
  this->m_path = path;
  const std::uint8_t unknown[] = {0x62, 0x97, 0x24, 0xD7, 0x8C, 0xDA, 0x41, 0xC5};
  AsyncDiagnostic diagnostic(false, std::string(DecryptName<0xC52FB5FBB94FF917ULL>(unknown)), sourceFile, 54);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005EC7C0
// FUNCTION: fn_03983
bool CEmblemDataUploadAsyncWork::Execute() {
  char account[64] = {}, guild[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  StringFormat(guild, sizeof(guild), _TRUNCATE, "%lu", this->m_guildId);
  std::string name = ToUtf8(this->m_accountName);
  std::string path = ToUtf8(this->m_path);
  std::string extension = this->m_path.substr(this->m_path.size() - 3, this->m_path.size());

  for (char& value : extension) {
    value = static_cast<char>(std::toupper(static_cast<unsigned char>(value)));
  }

  std::string imageType = ToUtf8(extension);
  CCURLPostHander handler(this->m_url);
  const std::uint8_t aid[] = {0xF6, 0x4E, 0x13, 0xCD};
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>(aid), account, 4);
  const std::uint8_t authToken[] = {0xE0, 0x48, 0xEB, 0xBF, 0x37, 0xE0, 0x4C, 0x3C, 0xCF, 0x3D};
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>(authToken), this->m_token, 4);
  const std::uint8_t worldName[] = {0x48, 0x5E, 0xBF, 0x8B, 0x89, 0x3B, 0x0C, 0x70, 0x7A, 0x31};
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>(worldName), name, 4);
  const std::uint8_t gdid[] = {0x12, 0xB9, 0xFC, 0x9F, 0xC7};
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>(gdid), guild, 4);
  const std::uint8_t imgType[] = {0x98, 0x74, 0xFA, 0xDB, 0x12, 0xFB, 0x6A, 0x0F};
  handler.AddField(DecryptName<0x0F0F8B6B8F9D19D1ULL>(imgType), imageType, 4);
  const std::uint8_t img[] = {0x24, 0x0E, 0x52, 0x2D};
  handler.AddField(DecryptName<0xD98F03A52D35636DULL>(img), path, 10);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    const std::uint8_t good[] = {0x0A, 0x8C, 0x2A, 0xC7, 0xED};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x31D7C1EDA345E36DULL>(good), 100);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    const std::uint8_t fail[] = {0x6F, 0xC8, 0x2E, 0x33, 0xD9};
    AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0xD3755DD95F47A909ULL>(fail), 109);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005EC700
// FUNCTION: fn_03982
void CEmblemDataUploadAsyncWork::Complete() {
  CEmblemDataMgr* emblems = g_emblemDataMgr;
  emblems->CompleteUpload(this->m_diagnostic, this->m_response, this->m_guildId);
}

template void std::deque<CEmblemDataUploadAsyncWork*>::push_back(CEmblemDataUploadAsyncWork*&&);
