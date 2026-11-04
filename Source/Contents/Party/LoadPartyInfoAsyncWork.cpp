#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyInfoAsyncWork.cpp";

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

// OFFSET: 0x005D0470
// FUNCTION: fn_03692
CLoadPartyInfoAsyncWork::CLoadPartyInfoAsyncWork()
    : NAsyncWork("PartyInfo_Load"), m_accountId(0), m_characterId(0), m_diagnostic(Diagnostic(false, "unknwon", 25)) {
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
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 52);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005D0C80
// FUNCTION: fn_03697
bool CLoadPartyInfoAsyncWork::Execute() {
  char account[64] = {};
  char character[64] = {};
  char query[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_characterId));
  const std::string world = ToUtf8(this->m_world);
  _snprintf_s(query, sizeof(query), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_queryAccountId));
  CCURLPostHander handler(this->m_url);
  const std::uint8_t aidSource[] = {0x14, 0xB4, 0xF1, 0xDB};
  thread_local EncryptedName<sizeof(aidSource), 0x3375B1C7DBB5FD55ULL> aidName(aidSource);
  handler.AddField(aidName.Text(), account, 4);
  const std::uint8_t gidSource[] = {0x58, 0x78, 0x89, 0xE7};
  thread_local EncryptedName<sizeof(gidSource), 0x1D6D75EDE7CD311FULL> gidName(gidSource);
  handler.AddField(gidName.Text(), character, 4);
  const std::uint8_t tokenSource[] = {0xE0, 0x48, 0xEB, 0xBF, 0x37, 0xE0, 0x4C, 0x3C, 0xCF, 0x3D};
  thread_local EncryptedName<sizeof(tokenSource), 0x59278F63D79F3DA1ULL> tokenName(tokenSource);
  handler.AddField(tokenName.Text(), this->m_token, 4);
  const std::uint8_t worldSource[] = {0xE0, 0x68, 0x25, 0xA1, 0x4F, 0xA9, 0x4A, 0xEC, 0xD2, 0x07};
  thread_local EncryptedName<sizeof(worldSource), 0x812BE72BCD5707B7ULL> worldName(worldSource);
  handler.AddField(worldName.Text(), world, 4);
  const std::uint8_t querySource[] = {0x58, 0xDC, 0x22, 0x2D, 0xA0, 0x1C, 0x3C, 0x97, 0x09};
  thread_local EncryptedName<sizeof(querySource), 0xD3755DD95F47A909ULL> queryName(querySource);
  handler.AddField(queryName.Text(), query, 4);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    const std::uint8_t source[] = {0x1E, 0x96, 0x26, 0xC0, 0x88, 0xB2, 0xA4, 0x31};
    thread_local EncryptedName<sizeof(source), 0x31D7C1EDA345E36DULL> text(source);
    AsyncDiagnostic diagnostic = Diagnostic(true, text.Text(), 97);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005D0BC0
// FUNCTION: fn_03696
void CLoadPartyInfoAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompletePartyInfo(this->m_diagnostic, this->m_response);
}
