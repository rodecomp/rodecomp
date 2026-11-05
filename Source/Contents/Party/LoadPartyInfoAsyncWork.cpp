#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyInfoAsyncWork.cpp";

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

// OFFSET: 0x005D0470
// FUNCTION: fn_03692
CLoadPartyInfoAsyncWork::CLoadPartyInfoAsyncWork()
    : NAsyncWork("PartyInfo_Load"), m_accountId(0), m_characterId(0), m_diagnostic(Diagnostic(false, DecryptName<0x2FF3C5036D070127ULL>({0x52, 0x6F, 0x6C, 0x03, 0x6C, 0xB2, 0x9D, 0x2F}), 25)) {
}

// INLINED
CLoadPartyInfoAsyncWork::~CLoadPartyInfoAsyncWork() = default;

// OFFSET: 0x005D0BB0
// FUNCTION: fn_03695
void CLoadPartyInfoAsyncWork::Reset() {
}

// OFFSET: 0x005D0930
// FUNCTION: fn_03694
void CLoadPartyInfoAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::uint32_t queryAccountId
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_queryAccountId = queryAccountId;
  const std::uint8_t unknown[] = {0xC6, 0x09, 0xA8, 0x3D, 0x8E, 0xF4, 0x5B, 0x17};
  AsyncDiagnostic diagnostic = Diagnostic(false, DecryptName<0x17359BF953C367B3ULL>(unknown), 52);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005D0C80
// FUNCTION: fn_03697
bool CLoadPartyInfoAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char character[64] = {};
  StringFormat(character, sizeof(character), _TRUNCATE, "%lu", this->m_characterId);
  std::string world = ToUtf8(this->m_world);
  char query[64] = {};
  StringFormat(query, sizeof(query), _TRUNCATE, "%lu", this->m_queryAccountId);
  CCURLPostHander handler(this->m_url);
  const std::uint8_t aidSource[] = {0x14, 0xB4, 0xF1, 0xDB};
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>(aidSource), account, 4);
  const std::uint8_t gidSource[] = {0x58, 0x78, 0x89, 0xE7};
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>(gidSource), character, 4);
  const std::uint8_t tokenSource[] = {0xE0, 0x48, 0xEB, 0xBF, 0x37, 0xE0, 0x4C, 0x3C, 0xCF, 0x3D};
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>(tokenSource), this->m_token, 4);
  const std::uint8_t worldSource[] = {0xE0, 0x68, 0x25, 0xA1, 0x4F, 0xA9, 0x4A, 0xEC, 0xD2, 0x07};
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>(worldSource), world, 4);
  const std::uint8_t querySource[] = {0x58, 0xDC, 0x22, 0x2D, 0xA0, 0x1C, 0x3C, 0x97, 0x09};
  handler.AddField(DecryptName<0xD3755DD95F47A909ULL>(querySource), query, 4);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    const std::uint8_t source[] = {0x1E, 0x96, 0x26, 0xC0, 0x88, 0xB2, 0xA4, 0x31};
    AsyncDiagnostic diagnostic = Diagnostic(true, DecryptName<0x31D7C1EDA345E36DULL>(source), 97);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005D0BC0
// FUNCTION: fn_03696
void CLoadPartyInfoAsyncWork::Complete() {
  CAdventurerAgencyMgr* manager = g_adventurerAgencyMgr;
  manager->CompletePartyInfo(this->m_diagnostic, this->m_response);
}
