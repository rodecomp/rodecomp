#include "Contents/AdventurerAgencyMgr.h"
#include "Contents/StopRecruitingAsyncWork.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char sourceFile[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CStopRecruitingAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005D5D70
// FUNCTION: fn_03711
CStopRecruitingAsyncWork::CStopRecruitingAsyncWork()
    : NAsyncWork("Stop_Recruiting"), m_accountId(0), m_masterAccountId(0),
      m_diagnostic(Diagnostic(false, "unknwon", 28)), m_context(nullptr) {
}

// OFFSET: 0x005D6140
// FUNCTION: fn_03712
CStopRecruitingAsyncWork::~CStopRecruitingAsyncWork() = default;

// OFFSET: 0x005D64A0
// FUNCTION: fn_03714
void CStopRecruitingAsyncWork::Reset() {
}

// OFFSET: 0x005D6230
// FUNCTION: fn_03713
void CStopRecruitingAsyncWork::Bind(
  std::uint32_t accountId,
  std::string world,
  std::string url,
  std::string token,
  std::uint32_t masterAccountId,
  StopRecruitingContext& context
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_masterAccountId = masterAccountId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 50);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_context = &context;
}

// OFFSET: 0x005D6570
// FUNCTION: fn_03716
bool CStopRecruitingAsyncWork::Execute() {
  char account[64] = {};
  char master[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_accountId));
  _snprintf_s(master, sizeof(master), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_masterAccountId));
  const std::string world = ToUtf8(this->m_world);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("WorldName", world, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("MasterAID", master, 4);

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

// OFFSET: 0x005D64B0
// FUNCTION: fn_03715
void CStopRecruitingAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompleteStopRecruiting(this->m_diagnostic, this->m_response, *this->m_context);
}
