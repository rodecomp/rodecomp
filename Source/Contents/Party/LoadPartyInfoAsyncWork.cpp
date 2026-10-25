#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadPartyInfoAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"

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
}

// OFFSET: 0x005D0470
// FUNCTION: fn_03692
CLoadPartyInfoAsyncWork::CLoadPartyInfoAsyncWork()
    : NAsyncWork("PartyInfo_Load"), m_accountId(0), m_characterId(0), m_diagnostic(Diagnostic(false, "unknwon", 25)), m_context(nullptr) {
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
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::uint32_t queryAccountId,
  PartyInfoContext& context
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
  this->m_context = &context;
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
  handler.AddField("AID", account, 4);
  handler.AddField("GID", character, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", world, 4);
  handler.AddField("QueryAID", query, 4);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "success", 97);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005D0BC0
// FUNCTION: fn_03696
void CLoadPartyInfoAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompletePartyInfo(this->m_diagnostic, this->m_response, *this->m_context);
}
