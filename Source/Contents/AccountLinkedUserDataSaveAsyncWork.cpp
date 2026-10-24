#include "Contents/AccountLinkedUserDataMgr.h"
#include "Contents/AccountLinkedUserDataSaveAsyncWork.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  // Original diagnostic provenance, not the reconstructed host file path.
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataSaveAsyncWork.cpp";
  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005BDAA0
// FUNCTION: fn_03478
CAccountLinkedUserDataSaveAsyncWork::CAccountLinkedUserDataSaveAsyncWork()
    : NAsyncWork("AccountLinkedUserData_Save"), m_accountId(0), m_diagnostic(Diagnostic(false, "unknwon", 26)) {
}

// Deleting entry.
// OFFSET: 0x005BDE60
// FUNCTION: fn_03479
CAccountLinkedUserDataSaveAsyncWork::~CAccountLinkedUserDataSaveAsyncWork() = default;

// OFFSET: 0x005BE1F0
// FUNCTION: fn_03481
void CAccountLinkedUserDataSaveAsyncWork::Reset() {
}

// OFFSET: 0x005BDF50
// FUNCTION: fn_03480
void CAccountLinkedUserDataSaveAsyncWork::Bind(
  std::uint32_t accountId,
  std::string accountName,
  std::string url,
  std::string token,
  std::string data
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_accountId = accountId;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 54);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005BE210
// FUNCTION: fn_03483
bool CAccountLinkedUserDataSaveAsyncWork::Execute() {
  char accountId[64] = {};
  _snprintf_s(accountId, sizeof(accountId), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  std::string name = ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", accountId, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", name, 4);
  handler.AddField("data", this->m_data, 4);
  const bool success = handler.Perform() == 0;
  AsyncDiagnostic diagnostic = Diagnostic(success, success ? "good" : "fail", success ? 106 : 102);
  this->m_diagnostic.MoveFrom(diagnostic);
  return false;
}

// OFFSET: 0x005BE200
// FUNCTION: fn_03482
void CAccountLinkedUserDataSaveAsyncWork::Complete() {
  g_accountLinkedUserDataMgr->CompleteSave();
}
