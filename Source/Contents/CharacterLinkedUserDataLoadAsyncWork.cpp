#include "Contents/CharacterLinkedUserDataLoadAsyncWork.h"
#include "Contents/CharacterLinkedUserDataMgr.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\CharacterLinkedUserData\\CCh"
                                "aracterLinkedUserDataLoadAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005E4A50
// FUNCTION: fn_03916
CCharacterLinkedUserDataLoadAsyncWork::CCharacterLinkedUserDataLoadAsyncWork()
    : NAsyncWork("CharacterLinkedUserData_Load"), m_accountId(0), m_characterId(0),
      m_diagnostic(Diagnostic(false, "unknwon", 29)), m_context(nullptr) {
}

// OFFSET: 0x005E4E20
// FUNCTION: fn_03917
CCharacterLinkedUserDataLoadAsyncWork::~CCharacterLinkedUserDataLoadAsyncWork() = default;

// OFFSET: 0x005E5190
// FUNCTION: fn_03919
void CCharacterLinkedUserDataLoadAsyncWork::Reset() {
}

// OFFSET: 0x005E4F10
// FUNCTION: fn_03918
void CCharacterLinkedUserDataLoadAsyncWork::Bind(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string accountName,
  std::string url,
  std::string token,
  CharacterConfigLoad& context
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_response.clear();
  this->m_context = &context;

  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 55);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005E5280
// FUNCTION: fn_03921
bool CCharacterLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  char characterId[64] = {};
  _snprintf_s(accountId, sizeof(accountId), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(characterId, sizeof(characterId), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_characterId));

  std::string name = ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", accountId, 4);
  handler.AddField("GID", characterId, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", name, 4);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "good", 103);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005E51A0
// FUNCTION: fn_03920
void CCharacterLinkedUserDataLoadAsyncWork::Complete() {
  g_characterLinkedUserDataMgr->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_characterId, this->m_accountName, this->m_response, *this->m_context
  );
}
