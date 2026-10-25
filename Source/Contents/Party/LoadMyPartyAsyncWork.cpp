#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/LoadMyPartyAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadMyPartyAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005CF1B0
// FUNCTION: fn_03685
CLoadMyPartyAsyncWork::CLoadMyPartyAsyncWork()
    : NAsyncWork("MyParty_Load"), m_accountId(0), m_characterId(0), m_diagnostic(Diagnostic(false, "unknwon", 28)), m_windows(nullptr) {
}

// OFFSET: 0x005CF580
// FUNCTION: fn_03686
CLoadMyPartyAsyncWork::~CLoadMyPartyAsyncWork() = default;

// OFFSET: 0x005CF8E0
// FUNCTION: fn_03688
void CLoadMyPartyAsyncWork::Reset() {
}

// OFFSET: 0x005CF670
// FUNCTION: fn_03687
void CLoadMyPartyAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, WindowManagerWindows& windows
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 49);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_windows = &windows;
}

// OFFSET: 0x005CF9B0
// FUNCTION: fn_03690
bool CLoadMyPartyAsyncWork::Execute() {
  char account[64] = {};
  char character[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_characterId));
  const std::string world = ToUtf8(this->m_world);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("GID", character, 4);
  handler.AddField("WorldName", world, 4);
  handler.AddField("AuthToken", this->m_token, 4);

  if (handler.Perform() != 0) {
    AsyncDiagnostic diagnostic = Diagnostic(false, "fail", 92);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "success", 101);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005CF8F0
// FUNCTION: fn_03689
void CLoadMyPartyAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompleteMyParty(this->m_diagnostic, this->m_response, *this->m_windows);
}
