#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/ChangePartyMasterAsyncWork.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CChangePartyMasterAsyncWork.cpp";

  // INLINED
  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005CD7B0
// FUNCTION: fn_03677
CChangePartyMasterAsyncWork::CChangePartyMasterAsyncWork()
    : NAsyncWork("Change_PartyMaster"), m_accountId(0), m_characterId(0), m_diagnostic(Diagnostic(false, "unknwon", 27)), m_windows(nullptr) {
}

// INLINED
CChangePartyMasterAsyncWork::~CChangePartyMasterAsyncWork() = default;

// OFFSET: 0x005CE080
// FUNCTION: fn_03681
void CChangePartyMasterAsyncWork::Reset() {
}

// OFFSET: 0x005CDDA0
// FUNCTION: fn_03680
void CChangePartyMasterAsyncWork::Bind(
  std::uint32_t accountId, std::uint32_t characterId, std::string characterName, std::uint32_t targetAccountId, std::uint32_t targetCharacterId,
  std::string targetName, std::string worldName, std::string url, std::string token, WindowManagerWindows& windows
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_characterName = characterName;
  this->m_targetAccountId = targetAccountId;
  this->m_targetCharacterId = targetCharacterId;
  this->m_targetName = targetName;
  this->m_worldName = worldName;
  this->m_url = url;
  this->m_token = token;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 55);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_windows = &windows;
}

// OFFSET: 0x005CE150
// FUNCTION: fn_03683
bool CChangePartyMasterAsyncWork::Execute() {
  char account[64] = {};
  char character[64] = {};
  char targetAccount[64] = {};
  char targetCharacter[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_characterId));
  _snprintf_s(targetAccount, sizeof(targetAccount), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_targetAccountId));
  _snprintf_s(targetCharacter, sizeof(targetCharacter), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_targetCharacterId));
  std::string world = ToUtf8(this->m_worldName);
  std::string name = ToUtf8(this->m_characterName);
  std::string targetName = ToUtf8(this->m_targetName);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("GID", character, 4);
  handler.AddField("CharName", name, 4);
  handler.AddField("To_AID", targetAccount, 4);
  handler.AddField("To_GID", targetCharacter, 4);
  handler.AddField("To_CharName", targetName, 4);
  handler.AddField("WorldName", world, 4);
  handler.AddField("AuthToken", this->m_token, 4);

  if (handler.Perform() != 0) {
    AsyncDiagnostic diagnostic = Diagnostic(false, "fail", 113);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "success", 122);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005CE090
// FUNCTION: fn_03682
void CChangePartyMasterAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompleteChangeMaster(this->m_diagnostic, this->m_response, *this->m_windows);
}
