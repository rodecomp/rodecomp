#include "Contents/CharacterLinkedUserDataMgr.h"
#include "Contents/CharacterLinkedUserDataSaveAsyncWork.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\CharacterLinkedUserData\\CCh"
                                "aracterLinkedUserDataSaveAsyncWork.cpp";
  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005E7C10
// FUNCTION: fn_03944
CCharacterLinkedUserDataSaveAsyncWork::CCharacterLinkedUserDataSaveAsyncWork()
    : NAsyncWork("CharacterLinkedUserData_Save"), m_accountId(0), m_characterId(0),
      m_diagnostic(Diagnostic(false, "unknwon", 28)) {
}

// OFFSET: 0x005E7FE0
// FUNCTION: fn_03945
CCharacterLinkedUserDataSaveAsyncWork::~CCharacterLinkedUserDataSaveAsyncWork() = default;

// OFFSET: 0x005E8370
// FUNCTION: fn_03947
void CCharacterLinkedUserDataSaveAsyncWork::Reset() {
}

// OFFSET: 0x005E80D0
// FUNCTION: fn_03946
void CCharacterLinkedUserDataSaveAsyncWork::Bind(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string accountName,
  std::string url,
  std::string token,
  std::string data
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 54);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005E8390
// FUNCTION: fn_03949
bool CCharacterLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  char characterId[64] = {};
  _snprintf_s(accountId, sizeof(accountId), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(characterId, sizeof(characterId), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_characterId));
  std::string name = ToUtf8(this->m_accountName);
  std::string data = ToUtf8(this->m_data);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", accountId, 4);
  handler.AddField("GID", characterId, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", name, 4);
  handler.AddField("data", data, 4);
  const bool success = handler.Perform() == 0;
  AsyncDiagnostic diagnostic = Diagnostic(success, success ? "good" : "fail", success ? 102 : 98);
  this->m_diagnostic.MoveFrom(diagnostic);
  return false;
}

// OFFSET: 0x005E8380
// FUNCTION: fn_03948
void CCharacterLinkedUserDataSaveAsyncWork::Complete() {
  g_characterLinkedUserDataMgr->CompleteSave();
}
