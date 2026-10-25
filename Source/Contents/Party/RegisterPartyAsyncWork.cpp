#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/RegisterPartyAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CRegisterPartyAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005D3600
// FUNCTION: fn_03704
CRegisterPartyAsyncWork::CRegisterPartyAsyncWork()
    : NAsyncWork("Register_Party"), m_accountId(0), m_characterId(0), m_diagnostic(Diagnostic(false, "unknwon", 26)), m_context(nullptr) {
}

// OFFSET: 0x005D3A60
// FUNCTION: fn_03705
CRegisterPartyAsyncWork::~CRegisterPartyAsyncWork() = default;

// OFFSET: 0x005D3E80
// FUNCTION: fn_03707
void CRegisterPartyAsyncWork::Reset() {
}

// OFFSET: 0x005D3B80
// FUNCTION: fn_03706
void CRegisterPartyAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, const CAdvenPartyInfo& info,
  RegisterPartyContext& context
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_info = info;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 50);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_context = &context;
}

// OFFSET: 0x005D3F50
// FUNCTION: fn_03709
bool CRegisterPartyAsyncWork::Execute() {
  char account[64] = {};
  char character[64] = {};
  char minimum[10] = {};
  char maximum[10] = {};
  char type[10] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_characterId));
  _snprintf_s(minimum, sizeof(minimum), _TRUNCATE, "%d", this->m_info.minimumLevel);
  _snprintf_s(maximum, sizeof(maximum), _TRUNCATE, "%d", this->m_info.maximumLevel);
  _snprintf_s(type, sizeof(type), _TRUNCATE, "%d", this->m_info.type);
  const std::string world = ToUtf8(this->m_world);
  const std::string name = ToUtf8(this->m_context->characterName);
  const std::string memo = ToUtf8(this->m_info.memo);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("GID", character, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", world, 4);
  handler.AddField("CharName", name, 4);
  handler.AddField("MinLV", minimum, 4);
  handler.AddField("MaxLV", maximum, 4);
  handler.AddField("Tanker", this->m_info.tanker ? "1" : "0", 4);
  handler.AddField("Healer", this->m_info.healer ? "1" : "0", 4);
  handler.AddField("Dealer", this->m_info.dealer ? "1" : "0", 4);
  handler.AddField("Assist", this->m_info.assist ? "1" : "0", 4);
  handler.AddField("Type", type, 4);
  handler.AddField("Memo", memo, 4);

  if (handler.Perform() != 0) {
    AsyncDiagnostic diagnostic = Diagnostic(false, "fail", 118);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "success", 127);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005D3E90
// FUNCTION: fn_03708
void CRegisterPartyAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompleteRegistration(this->m_diagnostic, this->m_response, *this->m_context);
}
