#include "Contents/AccountConfigLoad.h"
#include "Contents/AccountLinkedUserDataLoadAsyncWork.h"
#include "Contents/AccountLinkedUserDataMgr.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char sourceFile[] = "C:"
                                "\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AccountLinkedUserData\\CAcco"
                                "untLinkedUserDataLoadAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005B9E50
// FUNCTION: fn_03440
CAccountLinkedUserDataLoadAsyncWork::CAccountLinkedUserDataLoadAsyncWork()
    : NAsyncWork("AccountLinkedUserData_Load"), m_accountId(0), m_diagnostic(Diagnostic(false, "unknwon", 26)),
      m_context(nullptr) {
}

// Deleting entry.
// OFFSET: 0x005BA410
// FUNCTION: fn_03442
CAccountLinkedUserDataLoadAsyncWork::~CAccountLinkedUserDataLoadAsyncWork() = default;

// OFFSET: 0x005BA820
// FUNCTION: fn_03446
void CAccountLinkedUserDataLoadAsyncWork::Reset() {
}

// OFFSET: 0x005BA5D0
// FUNCTION: fn_03445
void CAccountLinkedUserDataLoadAsyncWork::Bind(
  std::uint32_t accountId,
  std::string accountName,
  std::string url,
  AccountConfigLoad& context
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_response.clear();
  this->m_context = &context;

  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 50);
  this->m_diagnostic.MoveFrom(diagnostic);
}

// OFFSET: 0x005BA910
// FUNCTION: fn_03449
bool CAccountLinkedUserDataLoadAsyncWork::Execute() {
  char accountId[64] = {};
  _snprintf_s(accountId, sizeof(accountId), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));

  std::string name = ToUtf8(this->m_accountName);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", accountId, 4);
  handler.AddField("WorldName", name, 4);

  if (handler.Perform() == 0) {
    // Unlike character load, account load passes UTF-8 JSON through unchanged.
    this->m_response = handler.Response();
    AsyncDiagnostic diagnostic = Diagnostic(true, "good", 92);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005BA830
// FUNCTION: fn_03447
void CAccountLinkedUserDataLoadAsyncWork::Complete() {
  g_accountLinkedUserDataMgr->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_accountName, this->m_response, this->m_context->emotions,
    this->m_context->script, this->m_context->persistenceAccountId, this->m_context->renderSystem
  );
}
